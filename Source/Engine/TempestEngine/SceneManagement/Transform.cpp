/*!***************************************************************************************
\file       Transform.cpp
\author     Aaron Damyen
\date       7/3/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is the implementation of the Transform class member functions.
*****************************************************************************************/

//========Self Include==================================================================//
#include "Transform.hpp"
//========1st Party Includes============================================================//
#include "GameObject.hpp"
#include <Logger.hpp>
#include <MathFunctions.hpp>
#include <Matrix4x4.hpp>
#include <Vector3Serialization.hpp>
#include <QuaternionSerialization.hpp>

////////==========================================================================////////
////////  Class(transform)                                                        ////////
////////==========================================================================////////

///////============================================================================///////
///////   Public                                                                   ///////
///////============================================================================///////

//////==============================================================================//////
//////    Non-Static                                                                //////
//////==============================================================================//////

/////================================================================================/////
/////     Functions                                                                  /////
/////================================================================================/////

std::string transform::getQueryableType() { return std::string(type().name()); }

std::string transform::getQueryableID()
{
	if (!getGameObject().expired()) { return std::to_string(getGameObject().lock()->getID()); }
	else { return "orphan transform component"; }
}

std::string transform::getQueryableProperty(std::string p_name)
{
	//convert this object to a typeRT
	typeRT temp = toTypeRT();

	//access the member with the name given
	typeRT temp2 = temp.member(p_name);

	//returns a typeRT equivalent to the type of the member variable

	//return string consisting of each data using string conversion defined in typeRT class
	std::string result;

	result += temp2.member("x").to_csv_format();
	result += temp2.member("y").to_csv_format();
	result += temp2.member("z").to_csv_format();

	return result;
}

std::vector<std::string> transform::getQueryablePropertyNames() { return property_names; }



////==================================================================================////
////      Overridden                                                                  ////
////==================================================================================////

//======================================================================================//
//        Helper                                                                        //
//======================================================================================//

typeRT transform::toTypeRT() const
{
	typeRT result;
	result.setTypeName(getType().name());
	result.setVariableName(getType().name());
	std::vector<typeRT> members;

	typeRT position = ::toTypeRT(getPosition());
	position.setVariableName("position");
	members.push_back(position);

	typeRT scale = ::toTypeRT(getScale());
	scale.setVariableName("scale");
	members.push_back(scale);

	typeRT rotation = ::toTypeRT(getRotation());
	rotation.setVariableName("rotation");
	members.push_back(rotation);

	for(auto i_member:members)
	{
		result.insertMember(i_member);
	}
	return result;
}

void transform::checkDirty()
{
	if (is_dirty)
	{
		const auto getParentMat =
		[this]
		{
			// get the parent's global matrix if available
			if (auto l_weak_parent = getGameObject(); !l_weak_parent.expired())
			{
				auto l_parent_ptr = l_weak_parent.lock()->getParent();
				if (!l_parent_ptr.expired())
				{
					auto l_parent_transform = l_parent_ptr.lock()->getComponent<transform>();
					if (l_parent_transform)
					{
						return l_parent_transform->getGlobalMatrix();
					}
				}
			}

			return matrix4x4{}; // identity if no value
		};



		// update matrices
		local_matrix = matrix4x4::build(local_position, local_rotation, local_scale);
		global_matrix =  getParentMat() * local_matrix;

		// decompose global properties
		global_matrix.decompose(global_position, global_rotation, global_scale);

		// build last matrix with only some global properties
		global_matrix_no_transform = matrix4x4::build(vector3(0, 0, 0), global_rotation, global_scale);


		is_dirty = false;
	}
}

void transform::printLocalData() const
{
	logger l_logger("Transform");
    l_logger.debug() << "loc_pos: " << local_position.toStringPtr();
    l_logger.debug() << "loc_rot: " << local_rotation.toStringPtr();
    l_logger.debug() << "loc_sca: " << local_scale.toStringPtr();
    l_logger.debug() << "\n";
    
}

void transform::printGlobalData() const
{
	logger l_logger("Transform");
    l_logger.debug() << "glb_pos: " << global_position.toStringPtr();
    l_logger.debug() << "glb_rot: " << global_rotation.toStringPtr();
    l_logger.debug() << "glb_sca: " << global_scale.toStringPtr();
    l_logger.debug() << "\n";
}

void transform::markDirty()
{
	is_dirty = true;

	std::vector<std::shared_ptr<gameObject>> children = getGameObject().lock()->getChildren();

	for (auto& child : children) {
		child->getComponent<transform>()->markDirty();
	}
}

////==================================================================================////
////      (Non-)Configurable                                                          ////
////==================================================================================////

////==================================================================================////
////      Overridden/(Non-/Pure-)Virtual                                              ////
////==================================================================================////

//======================================================================================//
//        (Conversion )Constructor / Operators / Getters & Setters / Helper Destructor  //
//======================================================================================//

transform::transform()
    : local_position(0, 0, 0), local_rotation(0,0,0,1), local_scale(1,1,1),
      is_dirty(true), is_physicsDirty(false)
{
}

transform::transform(typeRT & p_type)
	: is_dirty{true}, is_physicsDirty{false}
{
	updateFromTypeRT(p_type);
}

void transform::updateFromTypeRT(typeRT & p_type)
{
	auto l_members = p_type.members();

	if(l_members.find("position") != l_members.end())
	{
		setPosition(::toVector3(p_type.member("position")));
	}
	if (l_members.find("rotation") != l_members.end())
	{
		setRotation(::toQuaternion(p_type.member("rotation")));
	}
	if (l_members.find("scale") != l_members.end())
	{
		setScale(::toVector3(p_type.member("scale")));
	}
}

void transform::fromLocalMatrix(const matrix4x4 & p_local_matrix)
{
	vector3 l_scale, l_position, l_shear;
  quaternion l_rotation;
	p_local_matrix.decompose(l_position, l_rotation, l_scale);
	setPosition(l_position);
	setRotation(l_rotation);
	setScale(l_scale);

	// for all children
}

void transform::fromGlobalMatrix(const matrix4x4& p_global_matrix)
{
	matrix4x4 l_matrix = p_global_matrix;

	// convert global matrix to a local matrix, if gameobject exists and if parent exists
	if (!getGameObject().expired())
	{
		std::weak_ptr<gameObject> l_parent = getGameObject().lock()->getParent();
		if (!l_parent.expired())
		{
			l_matrix = l_parent.lock()->getComponent<transform>()->getGlobalMatrix().inverse() * p_global_matrix;


		}
	}


	fromLocalMatrix(l_matrix);
}


void transform::lookAt(vector3 p_point)
{
	//matrix4x4::computeModelMatrixFromLookAtVector(global_position, p_point).decompose(
	//	local_scale, local_rotation, local_position);

	matrix4x4 mat;
	mat.computeModelMatrixFromLookAtVector2(getGlobalPosition(), p_point);
	quaternion rot = mat.toQuaternion();
	setRotation(rot);
	
	markDirty();
}

const matrix4x4 & transform::getLocalMatrix()
{
	checkDirty();
	return local_matrix;
}

const vector3 & transform::getGlobalPosition()
{
	checkDirty();
	return global_position;
}

const vector3& transform::getLastGlobalPosition()
{
	return last_global_position;
}

const quaternion & transform::getGlobalRotation()
{
	checkDirty();
	return global_rotation;
}

const vector3 & transform::getGlobalScale()
{
	checkDirty();
	return global_scale;
}

const matrix4x4 & transform::getGlobalMatrix()
{
	checkDirty();
	return global_matrix;
}

const matrix4x4& transform::getGlobalMatrixNoTransform()
{
    checkDirty();
    return global_matrix_no_transform;
}

vector3 transform::getRight()
{
	checkDirty();
	return global_rotation.getRight();
}

vector3 transform::getUp()
{
	checkDirty();
	return global_rotation.getUp();
}

vector3 transform::getForward()
{
	checkDirty();
	return global_rotation.getForward();
}

bool transform::getPhysicsDirtyFlag()
{
  if(is_physicsDirty)
  {
    is_physicsDirty = false;
    return true;
  }

  return false;
}


/*======================================================================================/
/         Other                                                                         /
//======================================================================================*/

// namespace <namespace name>


