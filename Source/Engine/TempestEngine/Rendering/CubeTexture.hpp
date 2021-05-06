/*!***************************************************************************************
\file       CubeTexture.cpp
\author     Cody Cannell
\date       6/19/18
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief      Holds all information about a texture
*****************************************************************************************/
#pragma once

//========1st Party Includes============================================================//
#include "TypeData.hpp"
#include "Texture.hpp"
#include "SimpleTexture.hpp"

//========3rd Party Includes============================================================//
//========Types=========================================================================//
//========Defines=======================================================================//
//========Forward Deceleration==========================================================//

////////==========================================================================////////
////////  Class                                                                   ////////
////////==========================================================================////////
/*!***************************************************************************************
\par class: texture
\brief   Holds all information about a texture.
*****************************************************************************************/
class cubeTexture : public texture
{
    ///////============================================================================///////
    ///////   Public                                                                   ///////
    ///////============================================================================///////
public:

    //======================================================================================//
    //        Constructor                                                                   //
    //======================================================================================//
    cubeTexture(std::vector<assetHandle> p_assetHandles, std::string const& p_textureLabel);
    cubeTexture(const cubeTexture& texture1) = delete;
    cubeTexture & operator=(cubeTexture const & other) = delete;


	void shutdown() override;
    void build() override;
    void bind(unsigned int slot) override;
    void unbind() override;

    virtual ~cubeTexture() = default;

    int getBoundSlot() override { return m_boundSlot; }

    void setData(assetHandle p_assetHandle) override;
    ///////============================================================================///////
    ///////   Private                                                                  ///////
    ///////============================================================================///////
private:

    std::vector<std::shared_ptr<simpleTexture> > m_textures;
    unsigned int m_cubeTextureHandle = 0;
    std::string m_cubeTextureLabel;
    int m_boundSlot = -1;

};


///////============================================================================///////
///////   Public/Protected/Private                                                 ///////
///////============================================================================///////

//////==============================================================================//////
//////    (Non-)Static                                                              //////
//////==============================================================================//////

/////================================================================================/////
/////     Functions / Data                                                           /////
/////================================================================================/////

////==================================================================================////
////      (Non-)Configurable                                                          ////
////==================================================================================////

////==================================================================================////
////      Overridden/(Non-/Pure-)Virtual                                              ////
////==================================================================================////

//======================================================================================//
//        (Conversion )Constructor / Operators / Getters & Setters / Helper Destructor  //
//======================================================================================//

/*!***************************************************************************************
\brief  This just for giving an example of a function format.
\param exampleIn - dat old boring int
\return What is this returning?
*****************************************************************************************/

/*======================================================================================/
/         Other                                                                         /
//======================================================================================*/

// namespace <namespace name>
#pragma once
