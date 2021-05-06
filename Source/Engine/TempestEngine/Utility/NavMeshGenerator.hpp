/*!**********************************************************************************************************
\file       NavMeshGenerator.hpp
\author     Henry Brobeck
\date       3/8/2019
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
************************************************************************************************************/
#pragma once
#include "../SceneManagement/GameObject.hpp"
#undef max
#undef min

#include <list>
#include <vector>
#include <map>
#include <memory>

struct vector3;
class engine;

class navMeshGenerator
{
public:
	static void init(engine* p_engine);
	static void buildNavMesh();
	static void showVoxelDebug();
	static void hideVoxelDebug();
	static void showSpanDebug();
	static void hideSpanDebug();
	static void showTopDebug();
	static void hideTopDebug();
protected:

private:
	struct Span
	{
		Span() :x(0), z(0), min(0), max(0), flagDelete(false) {}
		Span(int p_x, int p_z, int p_min = 0, int p_max = 0) : x(p_x), z(p_z), min(p_min), max(p_max), flagDelete(false) {}
		int x;
		int z;
		int min;
		int max;
		std::list<Span> neighbors;
		bool flagDelete;

	};
	struct Eraser
	{
		bool operator()(const Span& p_span)
		{
			return p_span.flagDelete;
		}
	};

	static engine* m_engine;
	static std::vector<bool> m_voxels;
	static int width;
	static int height;
	static int depth;
	static vector3 offset;
	static vector3 voxelScale;
	static std::multimap<int, Span> spans;
	static std::multimap<int, Span> openSpans;
	static int minWalkHeight;
	static int voxelStep;
	static int maxStepHeight;
	static int minHeadSpace;
	static int to3D(int width, int height, int depth);
	static int to2D(int width, int depth);
	static void getAabb(const std::vector<std::shared_ptr<gameObject>>& p_objects, vector3& p_min, vector3& p_max);
	static void validateNeighbors(int p_current, int p_potential);
	static vector3 spanToWorld(const Span& span);


};


// namespace <namespace name>

