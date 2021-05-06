/*!***************************************************************************************
\file       Material.hpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Holds iformation about the objects color and how light interacts with it
*****************************************************************************************/
#pragma once
//========1st Party Includes============================================================//
#include "Color.hpp"
//========3rd Party Includes============================================================//
//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//

////////==========================================================================////////
////////  Struct / Class                                                          ////////
////////==========================================================================////////

class assetHandle;

/*!***************************************************************************************
\par class: material
\brief   This hold all data about how light interacts with an object material
*****************************************************************************************/
class material
{
    ///////============================================================================///////
    ///////   public                                                                   ///////
    ///////============================================================================///////
public:

    //======================================================================================//
    //        Constructor                                                                   //
    //======================================================================================//
    material() = default;

    /*!***************************************************************************************
    \brief  Copy Constructor (default)
    \param  material - the material to be copyed
    *****************************************************************************************/
    material(material const& material) = delete; 
	material(material&&) noexcept = default;

    //======================================================================================//
    //        Destructor                                                                    //
    //======================================================================================//

    /*!***************************************************************************************
    \brief  Destructor (default)
    *****************************************************************************************/
    ~material() = default;

    //======================================================================================//
    //        Getters & Setters                                                             //
    //======================================================================================//
    /*!***************************************************************************************
    \brief   Gets the materials name
    \return  The name of the material
    *****************************************************************************************/
    std::string const& getMaterialName() const noexcept { return m_materialName; }
    std::string const& getFileName() const noexcept { return m_fileName; }

    /*!***************************************************************************************
    \brief   Sets the materials name
    \param   p_name - Name of material
    \return  The material whos name was set
    *****************************************************************************************/
    material& setMaterialName(std::string const& p_name);

    /*!***************************************************************************************
    \brief   Gets the ambient color
    \return  A copy of the ambient color
    *****************************************************************************************/
    color getAmbientColor() const noexcept { return m_ambientColor; }

    /*!***************************************************************************************
    \brief   Sets the ambient color
    \param   p_color - color passed in
    \return  The material whos ambient color was set
    *****************************************************************************************/
    material& setAmbientColor(color const& p_color);

    /*!***************************************************************************************
    \brief   Gets the diffuse color
    \return  A copy of the diffuse color
    *****************************************************************************************/
    color getDiffuseColor() const noexcept { return m_diffuseColor; }

    /*!***************************************************************************************
    \brief   Sets the diffuse color
    \param   p_color - color passed in
    \return  The material whos diffuse color was set
    *****************************************************************************************/
    material& setDiffuseColor(color const& p_color);

    /*!***************************************************************************************
    \brief   Gets the specular color
    \return  A copy of the specular color
    *****************************************************************************************/
    color getSpecularColor() const noexcept { return m_specularColor; }

    /*!***************************************************************************************
    \brief   Sets the specular color
    \param   p_color - color passed in
    \return  The material whos specular color was set
    *****************************************************************************************/
    material& setSpecularColor(color const& p_color);

    /*!***************************************************************************************
    \brief   Get the emissive color
    \return  A copy of the emissive color
    *****************************************************************************************/
    color getEmissiveColor() const noexcept { return m_emissiveColor; }

    /*!***************************************************************************************
    \brief   Set the emissive color
    \param   p_color - color passed in
    \return  The material whos emissive color was set
    *****************************************************************************************/
    material& setEmissiveColor(color const& p_color);

    /*!***************************************************************************************
    \brief  Gets the specular exponent
    \return A copy of the specular exponent
    *****************************************************************************************/
    float getSpecularExponent() const noexcept { return m_specularExponent; }

    /*!***************************************************************************************
    \brief   Sets the specular exponent
    \param   p_pow - power of specular exponent
    \return  The material whos specular exponent was set
    *****************************************************************************************/
    material& setSpecularExponent(float p_pow);

    /*!***************************************************************************************
    \brief   Gets the IlluminationModel
    \return  A copy of the IlluminationModel number
    *****************************************************************************************/
    int getIlluminationModel() const noexcept { return m_illumModel; }

    /*!***************************************************************************************
    \brief   Sets the illumination model
    \param   p_index - index of illumination model
    \return  The material whos IlluminationModel was set
    *****************************************************************************************/
    material& setIlluminationModel(int p_index);

    bool hasDiffuseMap() const noexcept { return m_hasDiffuseMap; }
    bool hasSpecularMap() const noexcept { return m_hasSpecularMap; }
    bool hasNormalMap() const noexcept { return m_hasNormalMap; }
    bool hasAmbiantOcclussionMap() const noexcept { return m_hasAmbiantOcclussionMap; }
    bool hasHeightMap() const noexcept { return m_hasHeightMap; }

    bool isLoaded() const noexcept { return m_isLoaded;  }
   

    std::string const& getDiffuseMapName() const{ return m_diffuseMap; }
    std::string const& getAmbiantOcclussionMapName() const { return m_ambiantOcclussionMap; }
    std::string const& getSpecularMapName() const{ return m_specularMap; }
    std::string const& getNormalMapName() const{ return m_normalMap; }
    std::string const& getHeightMapName() const{ return m_heightMap; }

	void setDiffuseMapName(std::string const& n) { m_diffuseMap = n; m_hasDiffuseMap = true; };
	void setAOMapName(std::string const& n) { m_ambiantOcclussionMap = n; m_hasAmbiantOcclussionMap = true; };
	void setSpecularMapName(std::string const& n) { m_specularMap = n; m_hasSpecularMap = true; };
	void setNormalMapName(std::string const& n) { m_normalMap = n; m_hasNormalMap = true; };
	void setHeightMapName(std::string const& n) { m_heightMap = n; m_hasHeightMap = true; };

    void loadInFile(assetHandle p_assetPath) = delete;

    ///////============================================================================///////
    ///////   Private                                                                  ///////
    ///////============================================================================///////

private:
	friend class materialLoader;
    /////================================================================================/////
    /////     Data                                                                       /////
    /////================================================================================/////

    std::string m_materialName;     //!< The name of the material
    std::string m_fileName;

    color m_ambientColor = color(0,0,0);           //!< The Color that gets reflected
    color m_diffuseColor = color(0, 0, 0);           //!< The color of the object
    color m_specularColor = color(0, 0, 0);          //!< hightlights on a shinny surface
    color m_emissiveColor = color(0, 0, 0);          //!< self-illumination color (neon)

    int m_illumModel = 0;               //!< allows to turn on and off parts of shader
    float m_specularExponent = 0;       //!< shinny factor

    std::string m_diffuseMap;
    std::string m_specularMap;
    std::string m_normalMap;
    std::string m_ambiantOcclussionMap;
    std::string m_heightMap;

    bool m_hasDiffuseMap = false;
    bool m_hasSpecularMap = false;
    bool m_hasNormalMap = false;
    bool m_hasAmbiantOcclussionMap = false;
    bool m_hasHeightMap = false;

    bool m_isLoaded = false;
};
