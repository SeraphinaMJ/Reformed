/*!***************************************************************************************
\file       GraphicsSystem.hpp
\author     Cody Cannell
\date       3/1/19
\copyright  All content � 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief			Renders text on screen
*****************************************************************************************/
#pragma once
#include "../SystemBase.hpp"
#include <ft2build.h>
#include FT_FREETYPE_H  
#include "Color.hpp"

#include "GLTypes.hpp"

#include <string>
#include <map>
#include <memory>
#include <queue>

struct vector4;
struct vector3;
class shaderProgram;
class assetHandle;

/*!***************************************************************************************
\par struct: character
\brief holds user data for text
*****************************************************************************************/
struct textData
{
	std::string m_text;
	std::string m_fontType;
	float p_x = 0;
	float p_y = 0;
	vector4 m_color;
	float m_size = 10.0f;
};


/*!***************************************************************************************
\par struct: character
\brief holds data about a single character
*****************************************************************************************/
struct character
{
	GLuint m_textureID = 0;  // ID handle of the glyph texture
	unsigned m_width = 0;
	unsigned m_height = 0;       // Size of glyph
	int m_leftBearing = 0;    // Offset from baseline to left/top of glyph
	int m_topBearing = 0;
	GLuint m_offset = 0;    // Offset to advance to next glyph
};

/*!***************************************************************************************
\par struct: characterSet
\brief holds a set of character for specific font type
*****************************************************************************************/
struct characterSet
{
	std::map<GLchar, character> m_characters;
};

/*!***************************************************************************************
\par class: textRenderer
\brief renders text on screen
*****************************************************************************************/
class textRenderer {
	
public:

	/*!*******************************************************************************
	\brief  builds and stores quad, shader program, and sets up freetype
	*********************************************************************************/
	static void initialize();

	/*!*******************************************************************************
	\brief  draws all texts in queue
	*********************************************************************************/
	static void update();

	/*!*******************************************************************************
	\brief  deletes gpu buffers
	*********************************************************************************/
	static void shutdown();

private:

	/*!*******************************************************************************
	\brief  pushes user text onto queue
	\param  p_text  - text to display
	\param  p_style - name of the font file
	\param  p_x     - x coord
	\param  p_y     - y coord
	\param  p_size  - size of the text
	\param  p_color - color of the text
	*********************************************************************************/
	static void displayText(std::string p_text, std::string p_style, float p_x, float p_y, float p_size, vector4 p_color);

	/*!*******************************************************************************
	\brief  holds all functions to draw text
	\param  p_textData - user data for style and placement
	*********************************************************************************/
	static void draw(textData p_textData);

	/*!*******************************************************************************
	\brief   gets the next text in queue
	\return  user data for style and placement
	*********************************************************************************/
	static 	textData getNextTextToPrint();

	/*!*******************************************************************************
	\brief   loads a character set of 128 for font type and stores it
	\param   p_rPath  - relative path to font
	\param   p_size   - the loading size(currently unused hardcoded to 50)
	\return  map of characters
	*********************************************************************************/
	static std::map<GLchar, character> & loadCharacterSet(assetHandle & p_font, const float p_size);

	/*!*******************************************************************************
	\brief  Does the actual rendering of the text
	\param  p_text       - text to display
	\param  p_x          - x coord
	\param  p_y          - y coord
	\param  p_size       - size of the text
	\param  p_color      - color of the text
	\param  p_characters -  map of characters
	*********************************************************************************/
	static void RenderText(std::string p_text, GLfloat p_x, GLfloat p_y, GLfloat p_scale, vector4 p_color, std::map<GLchar, character> & p_characters);

	/*helper functions*/

	/*!*******************************************************************************
	\brief   builds a quad on the gpu to be held
	*********************************************************************************/
	static 	void createTextQuad();



	static	std::map<std::string, characterSet> m_characterSets;    //!< holds each font types characters
	static	std::string m_assetPath;																//!< the asset path for the engine
	static	FT_Library m_ft;																				//!< a handle to free text
	static	FT_Face m_fontFace;																			//!< a handle to the loaded font
	static	GLuint m_vao, m_vbo;																		//!< graphics buffers
	static	std::shared_ptr<shaderProgram> m_shaderProgram;         //!< shader program
	static	std::queue<textData> m_textToPrint;                     //!< text that needs to be printed to screen


	// the front end to all debugging calls
	friend class debug;

};


