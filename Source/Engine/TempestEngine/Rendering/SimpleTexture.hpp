/*!***************************************************************************************
\file       SimpleTexture.h
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
#include "GLTypes.hpp"
//========3rd Party Includes============================================================//
#include <DataTypes.hpp>
#include <vector>
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
class simpleTexture : public texture
{
	///////============================================================================///////
	///////   Public                                                                   ///////
	///////============================================================================///////
public:

	//======================================================================================//
	//        Constructor                                                                   //
	//======================================================================================//
	simpleTexture(assetHandle p_vertexName);
	simpleTexture(unsigned p_width, unsigned p_height, format p_format);

	~simpleTexture() override;

	//simpleTexture(const simpleTexture& texture1) = delete;

	void shutdown() override;
	void build() override;
	virtual void buildNoData();
	void bind(unsigned int slot) override;
	virtual void unbind(unsigned int slot);
	void unbind() override;

	int getBoundSlot() override { return m_boundSlot.size() ? m_boundSlot.back() : -1; }

	assetHandle getAssetHandle() { return m_assetHandle; }

	unsigned getWidth()
	{
		return m_width;
	}
	unsigned getHeight()
	{
		return m_height;
	}
	unsigned getColorChannels() const;

	unsigned char* getPixalData()
	{
		return m_pixelData;
	};


	GLuint getTextureHandle() { return m_textureHandle; }

	void setData(assetHandle p_assetHandle) override;


	///////============================================================================///////
	///////   Private                                                                  ///////
	///////============================================================================///////
private:
	virtual void initTex();

	assetHandle m_assetHandle;
	unsigned char* m_pixelData;
	unsigned m_width;
	unsigned m_height;
	format m_format;
	std::vector<int> m_boundSlot;


	GLuint m_textureHandle = 0;

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
