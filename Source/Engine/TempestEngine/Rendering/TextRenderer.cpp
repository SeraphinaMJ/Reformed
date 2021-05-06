/*!**********************************************************************************************************
\file       TextRenderer.cpp
\author     Cody Cannell
\date       11/13/2018
\copyright  All content © 2018-2019 DigiPen (USA) Corporation, all rights reserved.
\par        Project: Boomerang
\brief
************************************************************************************************************/
#include "../Precompiled.h"
#include "TextRenderer.hpp"
#include "ShaderProgram.hpp"
#include "Color.hpp"
#include "../SceneManagement/Transform.hpp"

#include <Logger.hpp>
#include <Vector4.hpp>

#include <iomanip>

#include <GL/glew.h>

using namespace std::string_literals;

#define LIMIT_OF_PRINTS_PER_FRAME = 100


// static declairs
std::map<std::string, characterSet> textRenderer::m_characterSets;
std::shared_ptr<shaderProgram> textRenderer::m_shaderProgram;
GLuint textRenderer::m_vao = 0, textRenderer::m_vbo = 0;
std::queue<textData> textRenderer::m_textToPrint;
FT_Face textRenderer::m_fontFace = nullptr;
FT_Library textRenderer::m_ft = nullptr;
std::string textRenderer::m_assetPath;







void textRenderer::createTextQuad()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

textData textRenderer::getNextTextToPrint()
{
	if(!m_textToPrint.empty())
	{
		auto l_element = m_textToPrint.front();
		m_textToPrint.pop();
		return l_element;
	}

	return { "queue was empty", "arial" , 0 ,0, color::red , 1 };
}

void textRenderer::initialize()
{
	// build a reusable quad
	createTextQuad();

	// set the asset path
   m_assetPath = "Assets/" /*getSystemManager()->getConfigurationFunction()().asset_path*/;

	// initialize the free_type
	if (FT_Init_FreeType(&m_ft))
		logger("FreeType").debug() << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	//build shader programe
	m_shaderProgram = std::make_shared<shaderProgram>(
		assetManager::getAsset(L"shaders/vertex/Text.vert"),
		assetManager::getAsset(L"shaders/fragment/Text.frag"));
	m_shaderProgram->build();
	m_shaderProgram->bind();

}

void textRenderer::shutdown()
{
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vao);
}


void textRenderer::update()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while(!m_textToPrint.empty())
	{
		auto l_text = getNextTextToPrint();

		draw(l_text);
	}

	glBlendFunc(GL_ONE, GL_ZERO);
}


void textRenderer::displayText(std::string p_text, std::string p_style, float p_x, float p_y, float p_size, vector4 p_color)
{
	// pushes struct holding text and style
	m_textToPrint.push({ p_text, p_style, p_x, p_y, p_color, p_size });
}


// private functions //

std::map<GLchar, character> & textRenderer::loadCharacterSet(assetHandle & p_font, const float)
{

	/* OpenGL requires that textures all have a 4-byte alignment e.g.
	their size is always a multiple of 4 bytes. Normally this won't be
	a problem since most textures have a width that is a multiple of 4
	and/or use 4 bytes per pixel, but since we now only use a single byte
	per pixel they can have any possible width. By setting its unpack
	alignment equal to 1 we ensure there are no alignment issues
	(which could cause segmentation faults). */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	std::string l_fileName = p_font->getFullPath().filename().generic_string();
	auto l_keyboolPair = m_characterSets.insert(std::pair<const std::string, characterSet>(l_fileName, characterSet()));

	// if inserted
	if(l_keyboolPair.second)
	{
		if (auto ret = FT_New_Face(m_ft, p_font->getFullPath().generic_string().c_str(), 0, &m_fontFace); ret)
			logger("FreeType").debug() << "ERROR::FREETYPE: Failed to load font. Error code: " << ret << std::endl;

		FT_Set_Pixel_Sizes(m_fontFace, 0, 48);

		for (GLubyte c = 0; c < 128; c++)
		{
			// Load character glyph
			if (FT_Load_Char(m_fontFace, c, FT_LOAD_RENDER))
			{
				logger("textRenderer").error() << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}

			/*The bitmap generated from the glyph is a grayscale 8 - bit
			 * image where each color is represented by a single byte. For
			this reason we'd like to store each byte of the bitmap buffer
			as a texture's color value.We accomplish this by creating a texture
			where each byte corresponds to the texture color's red component
			(first byte of its color vector). If we use a single byte to
			represent the colors of a texture we do need to take care of a
			restriction of OpenGL:*/


			// Generate texture
			GLuint l_textureID;
			glGenTextures(1, &l_textureID);
			glBindTexture(GL_TEXTURE_2D, l_textureID);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				m_fontFace->glyph->bitmap.width,
				m_fontFace->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				m_fontFace->glyph->bitmap.buffer
			);
			// Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Now store character for later use
			character l_character = {
				l_textureID,                          // texture id
				m_fontFace->glyph->bitmap.width,  // width
				m_fontFace->glyph->bitmap.rows,   // height
				m_fontFace->glyph->bitmap_left,   // distance form left
				m_fontFace->glyph->bitmap_top,    // distance from top
				static_cast<GLuint>(m_fontFace->glyph->advance.x)
			};

			// from the key item pair add a character to the item
			l_keyboolPair.first->second.m_characters.insert(std::pair<GLchar, character>(c, l_character));
		}

		FT_Done_Face(m_fontFace);
		FT_Done_FreeType(m_ft);
	}

	// send back a ref of the characters from the found/created character set
	return l_keyboolPair.first->second.m_characters;

}

void textRenderer::RenderText(std::string p_text, GLfloat p_x, GLfloat p_y, GLfloat p_scale, vector4 p_color, std::map<GLchar, character> & p_characters)
{
	// Activate corresponding render state
	m_shaderProgram->bind();
	m_shaderProgram->setUniforms("textColor", p_color);

	// TODO(patrick): this only works because the orderof the matrix components was reversed
	m_shaderProgram->setUniforms("projection", matrix4x4::computeOrthographicMatrix(1024, 0, 0, 768, -1, 1)); // TODO(patrick):
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(m_vao);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = p_text.begin(); c != p_text.end(); c++)
	{
		character ch = p_characters[*c];

		GLfloat xpos = p_x + ch.m_leftBearing * p_scale;
		GLfloat ypos = p_y - (ch.m_height - ch.m_topBearing) * p_scale;

		GLfloat w = ch.m_width * p_scale;
		GLfloat h = ch.m_height * p_scale;
		// Update VBO for each character
		const std::array<vector4, 6> vertices
		{
			vector4{ xpos,     ypos + h,   0.0, 0.0 },
			vector4{ xpos,     ypos,       0.0, 1.0 },
			vector4{ xpos + w, ypos,       1.0, 1.0 },

			vector4{ xpos,     ypos + h,   0.0, 0.0 },
			vector4{ xpos + w, ypos,       1.0, 1.0 },
			vector4{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.m_textureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(vector4), vertices.front().toFloatPtr());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		p_x += (ch.m_offset >> 6) * p_scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}


void textRenderer::draw(textData p_textData)
{
	// get the asset path
	auto l_assetPath = "fonts/" + p_textData.m_fontType + ".ttf";
	assetHandle font = assetManager::getAsset(l_assetPath);

	// build a map of chars (cached)
	auto & mapOfChars = loadCharacterSet(font, 50);

	// render the text
	RenderText(p_textData.m_text, p_textData.p_x, p_textData.p_y, p_textData.m_size, p_textData.m_color, mapOfChars);
}




