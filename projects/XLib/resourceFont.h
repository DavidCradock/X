#pragma once
#include "PCH.h"
#include "resourceBase.h"
#include "resourceTexture2D.h"

namespace X
{
	// Renders text using data stored in a pair of font files created from this class's buildFontFiles() method.
	// Requires a shader from the SCResourceManager called "X:font" which is loaded during startup. (See SCApplicationManager::mainLoop())
	class CResourceFont : public CResourceBase
	{
	public:
		CResourceFont(const std::string& strFontFilename);
		~CResourceFont();

		// Create all OpenGL context dependent objects when an OpenGL context exists.
		void onGLContextCreated(void);

		// Free all OpenGL context dependent objects before the OpenGL context is destroyed.
		void onGLContextToBeDestroyed(void);

		// Prints the given text as a single line
		void print(const std::string& strText, int iPosX, int iPosY, int iRenderTargetWidth, int iRenderTargetHeight, float fFontScaling = 1.0f, glm::vec4 colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		
		// Prints the given text as a single line, centered at iPosX and iPosY
		void printCentered(const std::string& strText, int iPosX, int iPosY, int iRenderTargetWidth, int iRenderTargetHeight, float fFontScaling = 1.0f, glm::vec4 colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		// Prints the given text as multiple lines to fit inside the given render target width and height.
		// If bRenderText is true, the text is rendered, else only the following values are computed...
		// Also sets the passed std::vector<std::string>& vstrTextLines to contain each line of text.
		// Also sets the passed int &iTotalRenderedHeight to hold the total amount of Y pixels the text takes up which may be larger than iRenderTargetHeight;
		void printInRect(bool bRenderText, const std::string& strText, int iPosX, int iPosY, int iRenderTargetWidth, int iRenderTargetHeight, std::vector<std::string>& vstrTextLines, int &iTotalRenderedHeight, float fFontScaling = 1.0f, glm::vec4 colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		// Get the width, in pixels, of the parsed text, if it were to be rendered.
		float getTextWidth(const std::string& strText, float fFontScaling = 1.0f);

		// Get the maximum height in pixels, of the font
		float getTextHeight(float fFontScaling = 1.0f);

		// Each character description (width, height, offset etc)
		struct CharDesc
		{
			float fABCa;		// The distance to add to the current position before drawing the character glyph.
			float fABCb;		// The width of the drawn portion of the character glyph. 
			float fABCc;		// The distance to add to the current position to provide white space to the right of the character glyph.
			glm::vec2 vTexMin;	// Bottom left texture coordinates
			glm::vec2 vTexMax;	// Top right texture coordinates
		};
		
		struct FontTypes
		{
			CharDesc charDesc[256];		// Each characters dims etc
			float fMaxCharHeight;		// The tallest character's height
		};
		FontTypes fontTypes;
		CResourceTexture2D* _mpResTexture;			// Texture holding each characters' image data
		glm::mat4 _mmatProjection;					// Projection used when rendering the text

	private:
		// Vertex buffer members used for rendering each character
		// A single vertex
		struct Vertex
		{
			glm::vec3 position;		// Vertex position
			glm::vec4 colour;		// Vertex colour
			glm::vec2 texCoord;		// Vertex texture coordinate
		};
		unsigned int vertexBufferObject;	// OpenGL vertex buffer object ID
		unsigned int vertexArrayObject;		// OpenGL vertex array object ID
		unsigned int elementBufferObject;	// OpenGL element buffer object ID
		std::vector<Vertex> vertices;		// Vector holding each unique vertex
		std::vector<int> indices;			// Vector holding indicies to each unique vertex
	};
}