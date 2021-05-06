/*!***************************************************************************************
\file       MaterialManager.hpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Manages all the materials on the GPU
*****************************************************************************************/
#pragma once

//========Self Include==================================================================//
//========1st Party Includes============================================================//
class assetHandle;
//========3rd Party Includes============================================================//
//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//
class material;

////////==========================================================================////////
////////  Struct / Class                                                          ////////
////////==========================================================================////////

/*!***************************************************************************************
\par class: materialManager
\brief   Manages all the materials on the GPU.
*****************************************************************************************/
class materialManager
{
	///////============================================================================///////
	///////   public                                                                   ///////
	///////============================================================================///////
public:

	materialManager() = default;
	/////================================================================================/////
	/////     Functions / Data                                                           /////
	/////================================================================================/////


	//======================================================================================//
	//         Getters & Setters                                                            //
	//======================================================================================//


	/*!***************************************************************************************
	\brief  adds a precreated materail to the manager (copy constructed)
	\param  p_name      - the name of the material being added
	\param  p_material  - a shared pointer to the material to be added
	\return               a shared pointer to the newly added material
	*****************************************************************************************/
    std::shared_ptr<material> addMaterial(std::shared_ptr<material> p_material);

    /*!***************************************************************************************
    \brief  Gets a material from the manager
    \param  p_matName  - the name of the material
    \return              a shared pointer to the material
    *****************************************************************************************/
    std::shared_ptr<material> getMaterial(std::string const& p_matName);

    /*!***************************************************************************************
    \brief  loads all materials from disk
	\param p_assetPath - the root path were assets can be obtained from.
    *****************************************************************************************/
    //void loadMaterials(std::string const & p_assetPath);

	///////============================================================================///////
	///////   Private                                                                  ///////
	///////============================================================================///////
private:

	/////================================================================================/////
	/////     Functions / Data                                                           /////
	/////================================================================================/////
		/*!***************************************************************************************
		\brief  Creates  a new material form a file
		\param  p_assetPath - the root path were assets can be obtained from.
		\param  p_name      - the name of the materail
		\param  p_filename  - the data file to be loaded in
		\return a shared pointer to the newly created material
		*****************************************************************************************/
    std::shared_ptr<material> createMaterial(assetHandle p_assetHandle);

    std::unordered_map<std::string, std::shared_ptr<material> > m_materials; //!< holds all materials
};

