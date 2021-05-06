/*!**********************************************************************************************************
\file       NavMeshGenerator.cpp
\author     Henry Brobeck
\date       3/8/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
************************************************************************************************************/
//========  Self Include  =================================================================================//
#include "NavMeshGenerator.hpp"
#include "../Engine.hpp"
#include "../SceneManagement/Scene.hpp"
#include "../SceneManagement/Transform.hpp"
#include "../External/SceneManipulator.hpp"
#include <algorithm>
#include <Logger.hpp>
#include <Vector3.hpp>
#include <Shapes.hpp>
#include <Vector3Serialization.hpp>

#include <utility>

#undef min
#undef max

engine* navMeshGenerator::m_engine = nullptr;
std::vector<bool>  navMeshGenerator::m_voxels;
std::multimap<int, navMeshGenerator::Span> navMeshGenerator::spans;
std::multimap<int, navMeshGenerator::Span> navMeshGenerator::openSpans;
int  navMeshGenerator::width;
int  navMeshGenerator::height;
int  navMeshGenerator::depth;
int  navMeshGenerator::minWalkHeight;
int navMeshGenerator::voxelStep = 50; // number of voxels on each axis
int navMeshGenerator::maxStepHeight = 5; // height in voxels of actor's maximum step up/down
int navMeshGenerator::minHeadSpace = 0; // min space between step down and agacent open space ("headbonk test")
vector3  navMeshGenerator::offset;
vector3  navMeshGenerator::voxelScale;
static logger loggerObj("NavMesh Generator");
namespace
{															//    0lbf     1rbf        2rbn     3lbn     4ltf       5rtf    6rtn      7ltn     
	const vector3 aabb_points[] = { {-1,-1,-1},{1,-1,-1},{1,-1,1},{-1,-1,1},{-1,1,-1},{1,1,-1},{1,1,1},{-1,1,1} };
	const vector3 half_aabb_points[] = { {-0.5,-0.5,-0.5},{0.5,-0.5,-0.5},{0.5,-0.5,0.5},{-0.5,-0.5,0.5},{-0.5,0.5,-0.5},{0.5,0.5,-0.5},{0.5,0.5,0.5},{-0.5,0.5,0.5} };
}

void navMeshGenerator::init(engine* p_engine)
{
	m_engine = p_engine;

	//testing
	frustum basic;
	basic.set(aabb_points[3], aabb_points[2], aabb_points[6], aabb_points[7], aabb_points[0], aabb_points[1], aabb_points[5], aabb_points[4]);
	aabb test;
	std::vector<vector3> points;
	std::copy(half_aabb_points, half_aabb_points + 8, std::back_inserter(points));
	matrix4x4 transform = matrix4x4::translation({ -100,-100,-100 }) * matrix4x4::scale({ 3,3,3 });
	for (auto& point : points)
	{
		vector4 translated = transform * vector4(point, 1);
		point.x = translated.x;
		point.y = translated.y;
		point.z = translated.z;
	}
	test.Compute(points);
	if (basic.collides(test))
	{
		std::cout << "collides" << std::endl;
	}
}

void navMeshGenerator::buildNavMesh()
{
	//voxelize terrain
	auto level = m_engine->getCurrentScene().lock()->getSpace("Terrain");
	if (level == nullptr)
	{
		return;
	}
	auto objects = level->getGameObjectList();

	vector3 min;
	vector3 max;
	getAabb(objects, min, max);


	vector3 dim = max - min;
	offset = min;
	voxelScale = dim / float(voxelStep);

	//generate frustums for each object
	std::vector<frustum> frusta;
	frusta.reserve(objects.size());
	for (auto iter : objects)
	{
		if (iter->getName() == "Skybox")
		{
			continue; //...
		}
		vector3 points[8] = { {0,0,0} };
		//build points list

		std::transform(std::cbegin(aabb_points), std::cend(aabb_points), std::begin(points),
			[matrix = iter->getComponent<transform>()->getGlobalMatrix()]
			(vector3 const& aabb_point) -> vector3
			{
				return vector3(matrix * vector4(aabb_point, 1.f));
			});

		//build frustume
		frustum frust;
		//    0lbf     1rbf        2rbn     3lbn     4ltf       5rtf    6rtn      7ltn     
							//lbn        rbn         rtn         ltn        lbf       rbf        rtf         ltf
		frust.set(points[3], points[2], points[6], points[7], points[0], points[1], points[5], points[4]);
		frusta.emplace_back(std::move(frust));

	}

	m_voxels.clear();

	constexpr auto getSize = 
	[](const int voxelStep) constexpr noexcept -> size_t
	{
		const auto casted = static_cast<size_t>(voxelStep);
		return casted * casted * casted;
	};

	m_voxels.resize(getSize(voxelStep));

	//for each voxel
	for (int i = 0; i < voxelStep; ++i)
	{
		for (int j = 0; j < voxelStep; ++j)
		{
			for (int k = 0; k < voxelStep; ++k)
			{
				//build voxel aabb
				vector3 min_aabb = offset;
				min_aabb.x += voxelScale.x * i;
				min_aabb.y += voxelScale.y * j;
				min_aabb.z += voxelScale.z * k;
				aabb vox_aabb(min_aabb, min_aabb + voxelScale);
				//black the voxel if any frustum touches it
				for (auto& frust : frusta)
				{
					//expanded for debug
					int idx = to3D(i, j, k);
					if (frust.collides(vox_aabb))
					{
						m_voxels[idx] = true;
						break;
					}
					else
					{
						m_voxels[idx] = false;
					}

					continue;
				}
			}
		}
		loggerObj.info(std::string("Voxelization: ") + std::to_string(float(i) / float(voxelStep)) + " complete");
	}

	//spannerize
	//for each width/depth column, generate spans
	for (int i = 0; i < voxelStep; ++i)
	{
		for (int k = 0; k < voxelStep; ++k)
		{
			bool solid_span = false;// = m_voxels[to3D(i, 0, k)];
			navMeshGenerator::Span current(i, k);
			current.min = 0;
			for (int j = 0; j < voxelStep; ++j)
			{
				int loc = to3D(i, j, k);
				bool black = m_voxels[loc];
				//currently solid
				if (solid_span)
				{
					//extend solid
					if (black)
					{
						current.max = j;
						continue;
					}
					else
					{
						//solid ended, send it to spans list
						spans.insert(std::pair(to2D(i, k), current));
						//start open span
						current = Span(i, k);
						current.min = j;
						current.max = j;
						solid_span = false;
					}
				}
				else // currently open
				{
					if (!black)
					{
						//extend open
						current.max = j;
						continue;
					}
					else
					{
						//open ended, send it to open list
						openSpans.insert(std::pair(to2D(i, k), current));
						current.min = j;
						current.max = j;
						solid_span = true;
					}
				}
			}
			//loop reached top of box, add current span to correct list
			// current.max = std::numeric_limits<int>::max();
			current.max = voxelStep;
			if (solid_span)
			{
				spans.insert(std::pair(to2D(i, k), current));
			}
			else
			{
				openSpans.insert(std::pair(to2D(i, k), current));
			}
		}
	}

	//cull spans
	//height check
	for (int i = 0; i < voxelStep; ++i)
	{
		for (int k = 0; k < voxelStep; ++k)
		{
			auto column = spans.equal_range(to2D(i, k));
			auto openColumn = spans.equal_range(to2D(i, k));
			for (auto spanIter = column.first; spanIter != column.second; ++spanIter)
			{
				auto& span = spanIter->second;
				if (span.max == std::numeric_limits<int>::max()) // flag any solid span with infinite hieght for deletion
				{
					//this might not be necessary.
					// span.flagDelete = true;
					continue;
				}
				//for each span, find open Span above it
				for (auto openIter = openColumn.first; openIter != openColumn.second; ++openIter)
				{
					auto& openSpan = openIter->second;
					if (openSpan.min == span.max + 1)
					{
						//if height is too small, flag solid span for delete
						if (openSpan.max - openSpan.min < minWalkHeight)
						{
							// span.flagDelete = true;
						}
						continue; // either way, we found the correct open span, move to next object
					}
				}
			}
		}
	}

	//erase any spans flagged for deletion
	for (auto eraseIter = spans.begin(); eraseIter != spans.end();)
	{
		if (eraseIter->second.flagDelete)
		{
			eraseIter = spans.erase(eraseIter);
		}
		else
		{
			++eraseIter;
		}
	}

	//find valid neighbor spans (diagonals are done through neighbor-of-neighbor checks)
	for (int i = 0; i < voxelStep; ++i)
	{
		for (int k = 0; k < voxelStep; ++k)
		{
			int current = to2D(i, k);
			//check +z
			if (k + 1 < voxelStep)
			{
				validateNeighbors(current, to2D(i, k + 1));
			}
			//check-z
			if (k - 1 >= 0)
			{
				validateNeighbors(current, to2D(i, k - 1));
			}
			//check +x
			if (i + 1 < voxelStep)
			{
				validateNeighbors(current, to2D(i + 1, k));
			}
			//check -x
			if (i - 1 >= 0)
			{
				validateNeighbors(current, to2D(i - 1, k));
			}
		}
	}



}

int navMeshGenerator::to3D(int p_x, int p_y, int p_z)
{
	return p_x + voxelStep * (p_y + voxelStep * p_z);
	// return p_x + width * (p_y + height * p_z);
}

int navMeshGenerator::to2D(int p_x, int p_y)
{
	return p_y * voxelStep + p_x;
}

void navMeshGenerator::getAabb(const std::vector<std::shared_ptr<gameObject>>& p_objects, vector3& p_min, vector3& p_max)
{
	//naive
#undef max
#undef min
	vector3 vmin(std::numeric_limits<float>::max());
	vector3 vmax(-std::numeric_limits<float>::max());


	for (auto obj : p_objects)
	{
		if (obj->getName() == "Skybox")
		{
			continue; //...
		}
		//get transform
		auto trans = obj->getComponent<transform>();
		auto matrix = trans->getGlobalMatrix();
		for (int i = 0; i < 8; ++i)
		{
			const auto point = matrix * vector4(aabb_points[i], 1);
			for (int i = 0; i < 3; ++i)
			{
				vmin[i] = std::min(vmin[i], point[i]);
				vmax[i] = std::max(vmax[i], point[i]);
			}
		}
	}
	p_min = vmin;
	p_max = vmax;
}

void navMeshGenerator::validateNeighbors(int p_current, int p_potential)
{
	//for each span at location
	auto column = spans.equal_range(p_current);
	auto neighborColumn = spans.equal_range(p_potential);
	auto openColumn = openSpans.equal_range(p_potential);
	for (auto& spanIter = column.first; spanIter != column.second; ++spanIter)
	{
		Span& span = spanIter->second;
		//find a solid span with valid step up/down
		for (auto& nIter = neighborColumn.first; nIter != neighborColumn.second; ++nIter)
		{
			Span& nSpan = nIter->second;
			if (nSpan.max < span.max + maxStepHeight && nSpan.min > span.max - maxStepHeight)
			{
				//"headbonk" test//find open span above potential span
				for (auto& openIter = openColumn.first; openIter != openColumn.second; ++openIter)
				{
					Span& openSpan = openIter->second;
					if (openSpan.min == nSpan.max + 1)
					{
						if (openSpan.max - openSpan.min >= minHeadSpace)
						{
							//passes all tests, valid neighbor, add to neigbhor list
							span.neighbors.push_back(nSpan);
							break; // only one openSpan above each solidSpan, skip to next span
						}
					}
				}
			}
		}
	}
}

vector3 navMeshGenerator::spanToWorld(const Span& span)
{
	vector3 result = offset;
	result.x += voxelScale.x * span.x;
	result.z += voxelScale.z * span.z;
	result += voxelScale / 2.0f;
	result.y = offset.y + voxelScale.y * (span.max - span.min);
	return result;
}

void navMeshGenerator::showVoxelDebug()
{
	auto sceneManip = m_engine->getSceneManipulator().lock();
	auto debug = sceneManip->addEmptySpace("voxel_debug");

	if (m_voxels.size() != 0)
	{
		for (int i = 0; i < voxelStep; ++i)
		{
			for (int j = 0; j < voxelStep; ++j)
			{
				for (int k = 0; k < voxelStep; ++k)
				{
					if (m_voxels[to3D(i, j, k)])
					{
						auto obj = sceneManip->addEmptyGameObject(debug, "VoxelDebug" + std::to_string(i) + "," + std::to_string(j) + "," + std::to_string(k));

						auto obj_trt = sceneManip->getTypeRT(obj);
						auto& comps = obj_trt.member("components").array();
						for (auto& comp : comps)
						{
							if (comp.getTypeName() == "class transform")
							{
								vector3 pos = offset;
								pos.x += voxelScale.x * i;
								pos.y += voxelScale.y * j;
								pos.z += voxelScale.z * k;
								pos += voxelScale / 2.0f;
								comp.member("position") = ::toTypeRT(pos);
								comp.member("scale") = ::toTypeRT(voxelScale * 0.5f);
							}
						}
						sceneManip->applyTypeRT(obj, obj_trt);
						sceneManip->addGameObjectComponent(obj, "class renderer");
					}
				}
			}
		}
	}
}

void navMeshGenerator::hideVoxelDebug()
{
	auto sceneManip = m_engine->getSceneManipulator().lock();
	sceneManip->deleteSpace("voxel_debug");
}

void navMeshGenerator::showSpanDebug()
{
	auto sceneManip = m_engine->getSceneManipulator().lock();
	auto debug = sceneManip->addEmptySpace("span_debug");
	for (auto& spanIter : spans)
	{
		Span& span = spanIter.second;
		auto obj = sceneManip->addEmptyGameObject(debug, "SpanDebug" + std::to_string(span.x) + "," + std::to_string(span.min) + "," + std::to_string(span.z));

		auto obj_trt = sceneManip->getTypeRT(obj);
		auto& comps = obj_trt.member("components").array();
		for (auto& comp : comps)
		{
			if (comp.getTypeName() == "class transform")
			{
				vector3 pos = offset;
				pos.x += voxelScale.x * span.x;
				pos.z += voxelScale.z * span.z;
				pos += voxelScale / 2.0f;
				pos.y = offset.y + voxelScale.y * (float(span.min) + float(span.max - span.min) / 2.0f) + voxelScale.y;
				comp.member("position") = ::toTypeRT(pos);
				vector3 scale = voxelScale / 2.0f;
				scale.y *= float(span.max - span.min);
				comp.member("scale") = ::toTypeRT(scale * 1.f);
			}
		}
		sceneManip->applyTypeRT(obj, obj_trt);
		sceneManip->addGameObjectComponent(obj, "class renderer");

	}
}


void navMeshGenerator::hideSpanDebug()
{
	auto sceneManip = m_engine->getSceneManipulator().lock();
	sceneManip->deleteSpace("span_debug");
}

void navMeshGenerator::showTopDebug()
{
	auto sceneManip = m_engine->getSceneManipulator().lock();
	auto debug = sceneManip->addEmptySpace("top_debug");
	for (auto& spanIter : spans)
	{
		Span& span = spanIter.second;
		auto obj = sceneManip->addEmptyGameObject(debug, "TopDebug" + std::to_string(span.x) + "," + std::to_string(span.max) + "," + std::to_string(span.z));

		auto obj_trt = sceneManip->getTypeRT(obj);
		auto& comps = obj_trt.member("components").array();
		for (auto& comp : comps)
		{
			if (comp.getTypeName() == "class transform")
			{
				vector3 pos = spanToWorld(span);
				comp.member("position") = ::toTypeRT(pos);
				vector3 scale = voxelScale / 2.0f;

				comp.member("scale") = ::toTypeRT(scale * 1.f);
			}
		}
		sceneManip->applyTypeRT(obj, obj_trt);
		sceneManip->addGameObjectComponent(obj, "class renderer");

	}
}

void navMeshGenerator::hideTopDebug()
{
	auto sceneManip = m_engine->getSceneManipulator().lock();
	sceneManip->deleteSpace("top_debug");
}
