/*!***************************************************************************************
\file       Transform.hpp
\author     Aaron Damyen
\date       5/17/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  Definition of the Transform component.
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "Component.hpp"
#include "GameObject.hpp"
#include "../Telemetry/QueryableInterface.hpp"


//========3rd Party Includes============================================================//
#include <ComponentTypeDetails.hpp>
#include <Vector3.hpp>
#include <Matrix4x4.hpp>
#include <Quaternion.hpp>
////////==========================================================================////////
////////  Class                                                                   ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: transform
\brief   This is the transform component each game object has.  It is responsible for tracking
		 a game objects position and orientation in game space.
*****************************************************************************************/
class transform final : public componentCRTP<transform>, public queryableInterface
{
	////////========================================================================////////
	////////   Public                                                               ////////
	////////========================================================================////////
public:

	//////==============================================================================//////
	//////    Non-Static                                                                //////
	//////==============================================================================//////

	/////================================================================================/////
	/////     Functions                                                                  /////
	/////================================================================================/////

	////==================================================================================////
	////      Overridden                                                                  ////
	////==================================================================================////

	//======================================================================================//
	//        Helper                                                                        //
	//======================================================================================//

	/*!***************************************************************************************
	\brief  Converts this into a type
	\return Type equivalent.
	*****************************************************************************************/
	typeRT toTypeRT() const override;

	/*!***************************************************************************************
	\brief  Constructs the component and initializes it to defaults
	*****************************************************************************************/
	transform();

	/*!***************************************************************************************
	\brief  Converts type to transform.
	\param p_type - type to convert
	*****************************************************************************************/
	transform(typeRT& p_type);

	/*!***************************************************************************************
	\brief  Frees any resources allocated by the component.
	*****************************************************************************************/
	virtual ~transform() = default;

	void updateFromTypeRT(typeRT & p_type) override;

	/*!***************************************************************************************
	\brief  Populates the values of this transform using the provided local matrix.
	\param p_local_matrix - the matrix containing the transform details
	*****************************************************************************************/
	void fromLocalMatrix(const matrix4x4 & p_local_matrix);

	/*!***************************************************************************************
	\brief  Populates the values of this transform using the provided global matrix.  This will
					account for the global transform of the parent, if present.
	\param p_global_matrix - the matrix containing the transform details
	*****************************************************************************************/
	void fromGlobalMatrix(const matrix4x4 & p_global_matrix);

	/*!***************************************************************************************
	\brief  Assigns a new position to the transform.
	\param p_new_position - the new position for the transform.
	*****************************************************************************************/
	void setPosition(const vector3 & p_new_position)
	{
		last_global_position = global_position;
		local_position = p_new_position; markDirty(); is_physicsDirty = true;
	}

	/*!***************************************************************************************
	\brief  Assigns a new rotation to the transform.
	\param p_new_rotation - the new rotation for the transform.
	*****************************************************************************************/
	void setRotation(const vector3 & p_new_rotation)
	{
		const quaternion xrot(vector3(1, 0, 0), p_new_rotation.x);
		const quaternion yrot(vector3(0, 1, 0), p_new_rotation.y);
		const quaternion zrot(vector3(0, 0, 1), p_new_rotation.z);

		local_rotation = xrot * yrot * zrot; markDirty(); is_physicsDirty = true;
	}

	void setRotation(const quaternion & p_new_rotation)
	{
		local_rotation = p_new_rotation; markDirty(); is_physicsDirty = true;
	}

	/*!***************************************************************************************
	\brief  Assigns a new scale to the transform.
	\param p_new_scale - the new scale for the transform.
	*****************************************************************************************/
	void setScale(const vector3 & p_new_scale) { local_scale = p_new_scale; markDirty(); }

	/*!***************************************************************************************
	\brief  Adjusts the position of the transform by a specified offset.
	\param p_offset - The amount to move the transform.
	*****************************************************************************************/
	void offsetPosition(const vector3 & p_offset) { local_position += p_offset; markDirty(); is_physicsDirty = true; }
	/*!***************************************************************************************
	\brief  Adjusts the rotation of the transform by a specified offset.
	\param p_offset - The amount to rotate the transform.
	*****************************************************************************************/
	void offsetRotation(const quaternion & p_offset) { local_rotation *= p_offset; markDirty();  is_physicsDirty = true; }
	/*!*****************************************x**********************************************
	\brief  Adjusts the scale of the transform by a specified offset.
	\param p_offset - The amount to scale the transform.
	*****************************************************************************************/
	void offsetScale(const vector3 & p_offset) { local_scale += p_offset; markDirty(); }

	/*!***************************************************************************************
	\brief  Assigns the position's x coordinate to a specific value
	\param p_new_x_pos - the new value for the position's x coordinate
	*****************************************************************************************/
	void setPosX(float p_new_x_pos) { local_position.setX(p_new_x_pos); markDirty();  is_physicsDirty = true; }
	/*!***************************************************************************************
	\brief  Assigns the position's y coordinate to a specific value
	\param p_new_y_pos - the new value for the position's y coordinate.
	*****************************************************************************************/
	void setPosY(float p_new_y_pos) { local_position.setY(p_new_y_pos); markDirty();  is_physicsDirty = true; }
	/*!***************************************************************************************
	\brief  Assigns the position's z coordinate to a specific value.
	\param p_new_z_pos - the new value for the position's z coordinate.
	*****************************************************************************************/
	void setPosZ(float p_new_z_pos) { local_position.setZ(p_new_z_pos); markDirty();  is_physicsDirty = true; }
	/*!***************************************************************************************
	\brief  Assigns the transform's rotation about the x axis to a specific value.
	\param p_new_x_rot - the new value for the rotation about the x axis.
	*****************************************************************************************/
	void setRotX(float p_new_x_rot) { local_rotation = quaternion({ 1,0,0 }, p_new_x_rot); markDirty(); is_physicsDirty = true; }
	/*!***************************************************************************************
	\brief  Assigns the transform's rotation about the y axis to a specific value.
	\param p_new_y_rot - the new value for the rotation about the y axis.
	*****************************************************************************************/
	void setRotY(float p_new_y_rot) { local_rotation = quaternion({ 0,1,0 }, p_new_y_rot); markDirty(); is_physicsDirty = true; }
	/*!***************************************************************************************
	\brief  Assigns the transform's rotation about the z axis to a specific value.
	\param p_new_z_rot - the new value for the rotation about the z axis.
	*****************************************************************************************/
	void setRotZ(float p_new_z_rot) { local_rotation = quaternion({ 0,0,1 }, p_new_z_rot); markDirty();  is_physicsDirty = true; }

	/*!***************************************************************************************
	\brief  Assigns the transform's scale in the x dimension to a specific value.
	\param p_new_x_scale - the new value for the scale in the x dimension.
	*****************************************************************************************/
	void setScaleX(float p_new_x_scale) { local_scale.setX(p_new_x_scale); markDirty(); }
	/*!***************************************************************************************
	\brief  Assigns the transform's scale in the y dimension to a specific value.
	\param p_new_y_scale - the new value for the scale in the y dimension.
	*****************************************************************************************/

	void setScaleY(float p_new_y_scale) { local_scale.setY(p_new_y_scale); markDirty(); }
	/*!***************************************************************************************
	\brief  Assigns the transform's scale in the z dimension to a specific value.
	\param p_new_z_scale - the new value for the scale in the z dimension.
	*****************************************************************************************/
	void setScaleZ(float p_new_z_scale) { local_scale.setZ(p_new_z_scale); markDirty(); }

	/*!***************************************************************************************
	\brief  Adjusts the position's x coordinate by a specified offset.
	\param p_offset_x_pos - The amount to move the position's x coordinate.
	*****************************************************************************************/
	void offsetPosX(float p_offset_x_pos) { local_position.offsetX(p_offset_x_pos); markDirty();  is_physicsDirty = true; }
	/*!***************************************************************************************
	\brief  Adjusts the position's y coordinate by a specified offset.
	\param p_offset_y_pos - The amount to move the position's y coordinate.
	*****************************************************************************************/
	void offsetPosY(float p_offset_y_pos) { local_position.offsetY(p_offset_y_pos); markDirty(); is_physicsDirty = true; }
	/*!***************************************************************************************
	\brief  Adjusts the position's z coordinate by a specified offset.
	\param p_offset_z_pos - The amount to move the position's z coordinate.
	*****************************************************************************************/
	void offsetPosZ(float p_offset_z_pos) { local_position.offsetZ(p_offset_z_pos); markDirty();  is_physicsDirty = true; }
	/*!***************************************************************************************
	\brief  Adjusts the transform's rotation about the x axis by a specified offset.
	\param p_offset_x_rot - The amount to rotate about the x axis.
	*****************************************************************************************/
	void offsetRotX(float p_offset_x_rot) { local_rotation = quaternion({ 1,0,0 }, p_offset_x_rot) * local_rotation; markDirty();  is_physicsDirty = true; }
	/*!***************************************************************************************
	\brief  Adjusts the transform's rotation about the y axis by a specified offset.
	\param p_offset_y_rot - The amount to rotate about the y axis.
	*****************************************************************************************/
	void offsetRotY(float p_offset_y_rot) { local_rotation = quaternion({ 0,1,0 }, p_offset_y_rot) * local_rotation; markDirty(); is_physicsDirty = true; }
	/*!***************************************************************************************
	\brief  Adjusts the transform's rotation about the z axis by a specified offset.
	\param p_offset_z_rot - The amount to rotate about the z axis.
	*****************************************************************************************/
	void offsetRotZ(float p_offset_z_rot) { local_rotation = quaternion({ 0,0,1 }, p_offset_z_rot) * local_rotation; markDirty(); is_physicsDirty = true; }
	/*!***************************************************************************************
	\brief  Adjusts the transform's scale in the x dimension by a specified offset.
	\param p_offset_x_scale - The amount to scale in the x dimension.
	*****************************************************************************************/
	void offsetScaleX(float p_offset_x_scale) { local_scale.offsetX(p_offset_x_scale); markDirty(); }
	/*!***************************************************************************************
	\brief  Adjusts the transform's scale in the y dimension by a specified offset.
	\param p_offset_y_scale - The amount to scale in the y dimension.
	*****************************************************************************************/
	void offsetScaleY(float p_offset_y_scale) { local_scale.offsetY(p_offset_y_scale); markDirty(); }
	/*!***************************************************************************************
	\brief  Adjusts the transform's scale in the z dimension by a specified offset.
	\param p_offset_z_scale - The amount to scale in the z dimension.
	*****************************************************************************************/
	void offsetScaleZ(float p_offset_z_scale) { local_scale.offsetZ(p_offset_z_scale); markDirty(); }

	/*!***************************************************************************************
	\brief  Rotates object to face direction of a point.
	\param p_point - The point we will face.
	*****************************************************************************************/
	void lookAt(vector3 p_point);

	/*!***************************************************************************************
	\brief  Retrieves the current position of the transform, relative to the parent.
	\return vector3 - The (x,y,z) position of the transform, relative to the parent.
	*****************************************************************************************/
	const vector3 & getPosition() const { return local_position; }
	/*!***************************************************************************************
	\brief  Retrieves the current rotation of the transform, relative to the parent.
	\return vector3 - The rotation about the axes (x,y,z) of the transform, relative to the parent.
	*****************************************************************************************/
	const quaternion & getRotation() const { return local_rotation; }
	/*!***************************************************************************************
	\brief  Retrieves the current scale of the transform, relative to the parent.
	\return vector3 - The scale in the dimensions (x,y,z) of the transform, relative to the parent.
	*****************************************************************************************/
	const vector3 & getScale() const { return local_scale; }
	/*!***************************************************************************************
	\brief  Retrieves the matrix that represents the current position, rotation and scale of the transform, relative to the parent.
	\return matrix4x4 - The affine matrix of the position, rotation and scale, relative to the parent.
	*****************************************************************************************/
	const matrix4x4 & getLocalMatrix();

	/*!***************************************************************************************
	\brief  Retrieves the position of the transform within global space (accumulates all positions
			from the parent chain).
	\return vector3 - the (x,y,z) position of the transform in global space.
	*****************************************************************************************/
	const vector3 & getGlobalPosition();

	/*!***************************************************************************************
	\brief  Retrieves the last position of the transform within global space (accumulates all positions
			from the parent chain).
	\return vector3 - the (x,y,z) position of the transform in global space.
	*****************************************************************************************/
	const vector3& getLastGlobalPosition();
	/*!***************************************************************************************
	\brief  Retrieves the rotation of the transform within global space (accumulates all rotations
			from the parent chain).
	\return vector3 - The rotation about the axes (x,y,z) of the transform in global space.
	*****************************************************************************************/
	const quaternion & getGlobalRotation();
	/*!***************************************************************************************
	\brief  Retrieves the scale of the transform within global space (accumulates all scales
			from the parent chain).
	\return vector3 - The scale in the dimensions (x,y,z) of the transform in global space.
	*****************************************************************************************/
	const vector3 & getGlobalScale();
	/*!***************************************************************************************
	\brief  Retrieves the matrix that represents the global position, rotation and scale of the
			transform (accumulates all values from parent chain).
	\return matrix4x4 - The affine matrix of the position, rotation and scale in global space.
	*****************************************************************************************/
	const matrix4x4 & getGlobalMatrix();

	const matrix4x4 & getGlobalMatrixNoTransform();

	vector3 getRight();
	vector3 getUp();
	vector3 getForward();

	/*!***************************************************************************************
	\brief  Kind of like checkDirty but only for the physics (bullet) transform
	\return bool - Am I a dirty little physics transform?
	*****************************************************************************************/
	bool getPhysicsDirtyFlag();

	/*!***************************************************************************************
	\brief  Writes all local values to the logger.  FOR DEBUGGING ONLY.
	*****************************************************************************************/
	void printLocalData() const;
	/*!***************************************************************************************
	\brief  Writes all global values to the logger.  FOR DEBUGGING ONLY.
	*****************************************************************************************/
	void printGlobalData() const;
	/*!***************************************************************************************
	\brief  Sets the dirty flag so the matrices for this transform are recalculated.
	*****************************************************************************************/
	void markDirty();

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	/*!***************************************************************************************
	\brief  Retrieves the type of component this instance is. SceneSystem requirement.
	\return componentType - The type of this component.
	*****************************************************************************************/
	virtual componentType const type() const override { return transform::getType(); }
	/*!***************************************************************************************
	\brief  Retrieves the type of component this class is.  SceneSystem requirement.
	\return componentType - The type of component this class represents.
	*****************************************************************************************/
	static componentType const getType() { static componentType const s_type = componentType::forConcrete<transform>(); return s_type; }
	static componentTypeDetails getTypeDetail() { return componentTypeDetails(getType(), getType().name(), "Transform", false, false, false); }

	/*!***************************************************************************************
	\brief  Retrieves the type this class, for use with the queryable interface.
	\return string - The type of class this represents.
	*****************************************************************************************/
	std::string getQueryableType() override;

	/*!***************************************************************************************
	\brief  Retrieves the ID of this object's parent, given the parent is valid
	\return string - The ID of the object.
	*****************************************************************************************/
	std::string getQueryableID() override;

	/*!***************************************************************************************
	\brief  Retrieves a property of the object by name
	\param p_name the name of the property(data)
	\return string - Returns the desired data
	*****************************************************************************************/
	std::string getQueryableProperty(std::string p_name) override;

	/*!***************************************************************************************
	\brief  Retrieves a property of the object by name
	\param p_name the name of the property(data)
	\return string - Returns the desired data
	*****************************************************************************************/
	std::vector<std::string> getQueryablePropertyNames() override;

  /*!***************************************************************************************
  \brief  Do not use this function without asking Ryan. Its a workaround not intended for use.
  \param p_dirty is physics dirty
  *****************************************************************************************/
  void setPhysicsIsDirtyFlag(bool p_dirty) { is_physicsDirty = p_dirty; }



	////////========================================================================////////
	////////   Private                                                              ////////
	////////========================================================================////////
private:

	/*!***************************************************************************************
	\brief  Updates the matrices and global data if any of the local values have changed.
	*****************************************************************************************/
	void checkDirty();

	vector3 local_position; //!< The current local position of this transform (relative to parent)
	quaternion local_rotation; //!< The current local rotation of this transform (relative to parent)// yaw (Z), pitch (Y), roll (X)
	vector3 local_scale; //!< The current local scale of this transform (relative to parent)
	matrix4x4 local_matrix; //!< Affine matrix of the current local parameters (relative to parent)

	vector3 global_position; //!< The current global position of this transform.
	quaternion global_rotation; //!< The current global rotation of this transform.// yaw (Z), pitch (Y), roll (X)
	vector3 global_scale; //!< The current global scale of this transform.
	matrix4x4 global_matrix; //!< Affine matrix fo the current global parameters.
	matrix4x4 global_matrix_no_transform;

	vector3 last_global_position;

	bool is_dirty; //!< Indicator that the matrices are not consistent with vector values.

	bool is_physicsDirty; //!< is_dirty flag for physics (bullet's transform)

	  //! The names of properties relevant to the queryableInterface, and prepped for telemetry tracking.
	std::vector<std::string> property_names = { "local_position",
												"global_position" };

	//////==============================================================================//////
	//////    (Non-)Static                                                              //////
	//////==============================================================================//////

	friend class physicsSystem;

};


