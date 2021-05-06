#pragma once
/*!***************************************************************************************
\file       DepthTexture.hpp
\author     Patrick Cook
\date       9/13/19
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief  This is a work in progress example sheet for section headers.
*****************************************************************************************/

//======== 1st Party Includes ==========================================================//
#include "Precompiled.h"
#include "Texture.hpp" // texture
#include "AssetManager.hpp" // assetHandle

#include "GLTypes.hpp"

//======== 3rd Party Includes ==========================================================//
//======== Types =======================================================================//
//======== Defines =====================================================================//
//======== Forward Declarations=========================================================//


/*!***************************************************************************************
\par class: depthTexture
\brief texture handle to a depth buffer
*****************************************************************************************/
class depthTexture : public texture
{
public:
	depthTexture(unsigned width, unsigned height);
	~depthTexture() override;
	
	void shutdown() override;
	void build() override;
	// ill formed?
	virtual void rebuild();
	void bind(unsigned int slot) override;
	void unbind() override;
	int getBoundSlot() override;
	void setData(assetHandle) override;

	unsigned width() const;
	unsigned height() const;
	GLuint getGLHandle();

protected:
	unsigned m_width, m_height;
	int m_boundSlot{ -1 };
	GLuint m_depthHandle{ 0 };

	/*!***************************************************************************************
	\brief  This just for giving an example of a function format.
	\param exampleIn - dat old boring int
	\return What is this returning?
	*****************************************************************************************/

};


