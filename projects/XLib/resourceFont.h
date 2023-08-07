#pragma once
#include "PCH.h"
#include "resourceBase.h"
#include "resourceTexture2DFromFile.h"
#include "matrix.h"
#include "colour.h"
#include "vector2f.h"

namespace X
{
	// Renders text using data stored in a pair of font files created from this class's buildFontFiles() method.
	// Requires a shader from the SCResourceManager called "X:VBCPT" which is loaded during startup. (See SCApplicationManager::mainLoop())
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
		void print(const std::string& strText, int iPosX, int iPosY, int iRenderTargetWidth, int iRenderTargetHeight, float fFontScaling = 1.0f, CColour colour = CColour(1.0f, 1.0f, 1.0f, 1.0f)) const;
		
		// Prints the given text as a single line, centered at iPosX and iPosY
		void printCentered(const std::string& strText, int iPosX, int iPosY, int iRenderTargetWidth, int iRenderTargetHeight, float fFontScaling = 1.0f, CColour colour = CColour(1.0f, 1.0f, 1.0f, 1.0f)) const;

		// Prints the given text as multiple lines to fit inside the given render target width and height.
		// If bRenderText is true, the text is rendered, else only the following values are computed...
		// Sets the passed std::vector<std::string>& vstrTextLines to contain each line of text.
		// Sets the passed int &iTotalRenderedHeight to hold the total amount of Y pixels the text takes up which may be larger than iRenderTargetHeight;
		void printInRect(
			bool bRenderText,
			const std::string& strText,
			int iPosX,
			int iPosY,
			int iRenderTargetWidth,
			int iRenderTargetHeight,
			std::vector<std::string>& vstrTextLines,
			int &iTotalRenderedHeight,
			float fFontScaling = 1.0f,
			CColour colour = CColour(1.0f, 1.0f, 1.0f, 1.0f)) const;

		// Similar to printInRect except this also accepts a string holding a newline seperator which is
		// used to split the text into multiple lines using the newline seperator string to determine where
		// the text is to be split into a new line.
		// Example:
		// If strText is "Line01\nLine02\nMoretext" and "\n" is given as the seperator string, then strText is split
		// into multiple lines.
		// Prints the given text as multiple lines to fit inside the given render target width and height.
		// If bRenderText is true, the text is rendered, else only the following values are computed...
		// Sets the passed std::vector<std::string>& vstrTextLines to contain each line of text.
		// Sets the passed int &iTotalRenderedHeight to hold the total amount of Y pixels the text takes up which may be larger than iRenderTargetHeight;
		void printInRectNewline(bool bRenderText,
			const std::string& strText,
			int iPosX,
			int iPosY,
			int iRenderTargetWidth,
			int iRenderTargetHeight,
			std::vector<std::string>& vstrTextLines,
			int& iTotalRenderedHeight,
			const std::string& strNewlineCode = "\n",
			float fFontScaling = 1.0f,
			CColour colour = CColour(1.0f, 1.0f, 1.0f, 1.0f)) const;

		// Get the width, in pixels, of the parsed text, if it were to be rendered.
		float getTextWidth(const std::string& strText, float fFontScaling = 1.0f) const;

		// Get the maximum height in pixels, of the font
		float getTextHeight(float fFontScaling = 1.0f) const;

		// Each character description (width, height, offset etc)
		struct CharDesc
		{
			float fABCa;		// The distance to add to the current position before drawing the character glyph.
			float fABCb;		// The width of the drawn portion of the character glyph. 
			float fABCc;		// The distance to add to the current position to provide white space to the right of the character glyph.
			CVector2f vTexMin;	// Bottom left texture coordinates
			CVector2f vTexMax;	// Top right texture coordinates
		};
		
		struct FontTypes
		{
			CharDesc charDesc[256];		// Each characters dims etc
			float fMaxCharHeight;		// The tallest character's height
		};
		FontTypes fontTypes;
		CResourceTexture2DFromFile* _mpResTexture;		// Texture holding each characters' image data
		mutable CMatrix _mmatProjection;				// Projection used when rendering the text

	private:
		// Vertex buffer members used for rendering each character
		// A single vertex
		struct Vertex
		{
			CVector3f position;		// Vertex position
			CColour colour;			// Vertex colour
			CVector2f texCoord;		// Vertex texture coordinate
		};
		mutable unsigned int _mVertexBufferObject;	// OpenGL vertex buffer object ID
		mutable unsigned int _mVertexArrayObject;	// OpenGL vertex array object ID
		mutable unsigned int _mElementBufferObject;	// OpenGL element buffer object ID
		mutable std::vector<Vertex> _mvecVertices;	// Vector holding each unique vertex
		mutable std::vector<int> _mvecIndices;		// Vector holding indicies to each unique vertex
	};
}