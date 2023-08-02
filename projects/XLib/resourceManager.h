#pragma once
#include "PCH.h"
#include "singleton.h"
#include "resourceDepthbuffer.h"
#include "resourceFont.h"
#include "resourceFramebuffer.h"
#include "resourceShader.h"
#include "resourceTexture2DAtlas.h"
#include "resourceTexture2DFromFile.h"
#include "resourceTexture2DFromImage.h"
#include "resourceTriangle.h"
#include "resourceLine.h"

namespace X
{
	// This resource manager holds various resources which require an OpenGL context
	// All resources, once added are ready to go, aka RAII (Resource Acquistion Is Initialisation)
	// All resources are derived from the CResourceBase class and have the pure virtual methods defined in their own files.
	// There are several resources added upon initialisation which are used by various classes these are...
	// X:font					// A shader used by the CResourceFont class to render text.
	// X:line					// A shader used by the CResourceLine class to render lines.
	// X:DRNE					// A shader which has vertex position, colour, texture coordinates and diffuse, roughness, normals and emission textures and shadows
	// X:DRNE_noshadows			// A shader which has vertex position, colour, texture coordinates and diffuse, roughness, normals and emission textures and NO shadows
	// X:pos_col_tex			// A shader which accepts vertex position, colour and texture coodinates
	// X:depthbuffer_debug		// A shader for rendering a bound depth buffer to a 2D quad so we can view the depth values in the depth buffer
	// X:shadowdepthmap			// A shader used by the scene manager to render the depth map used for rendering shadows
	// X:gui					// A shader used by the GUI to render everything.
	// X:2D						// A shader used by SC2DRenderer for entities
	// X:default_particle		// A texture for use with rendering generic particles
	// X:default_white			// A texture which is tiny and white.
	// X:default_diffuse		// A texture which is grey for diffuse, used if not set
	// X:default_emission		// A texture which is black for emission, used if not set
	// X:default_normal			// A texture which is a flat normal map, used if not set
	// X:default_roughness		// A texture which is black for roughness used if not set
	// X:shadows				// A depth buffer which is used by scene managers to render shadows
	// X:default				// A triangle resource (vertex buffer) used for rendering 2D quads to the screen for debugging purposes, by the GUI and SC2DRenderer.
	// X:backbuffer_FB			// A framebuffer stuff is rendered to and then at the end of the program loop, rendered to the backbuffer
	// X:guitooltipFB			// A framebuffer the tooltips are rendered to
	// X:default				// A line vertex buffer resource used by the GUI when rendering lines
	// They are loaded by this class's addDefaultResources() method which is called from SCApplicationManager::mainLoop()
	class SCResourceManager : public CSingleton<SCResourceManager>
	{
	public:
		SCResourceManager();

		// Call just before the OpenGL window context is about to be destroyed, to free all resources which depend upon it.
		void onGLContextToBeDestroyed(void);

		// Call this after the OpenGL window context has been recreated, to return all the resource back to their original state.
		void onGLContextRecreated(void);

		// Builds a font and saves it to disk using font files installed on the current OS which can then be used by the CResourceFont class.
		// This is so that we don't have to deal with installing fonts on the end users' system and also gives us the ability to modify the generated character images inside a paint program if desired.
		// The output file names (the font.fnt and font.png files) are named based upon the strOutputBaseName.
		// For example, if the basename was BASE, the font height 12, then the output files would be BASE_12.fnt and BASE_12.png
		// strOutputBaseName is the base filename used to create the two filenames for the .fnt and .png file names
		// strFontName is the name of the font which is installed on the operating system which is used to generate the font
		// iFontHeight is the size of the font
		// bAntialiased is whether to perform antialiasing when generating the font's character images.
		// bBold, bItalic, bUnderlines and bStrikeout define the style of the font.
		// If an error occurred, an error message is shown and execution ends.
		void buildFontFiles(const std::string& strOutputBaseName, const std::string& strFontName, unsigned int iFontHeight, bool bAntialiased, bool bBold, bool bItalic, bool bUnderlined, bool bStrikeout) const;

		// Adds default required resources to the resource manager, called during startup from mainLoop
		void addDefaultResources(void);

		// Adds a new depthbuffer object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		// uiWidth and uiHeight are the dimensions of the depthbuffer
		// If the named resource already exists, it has a count value which is incremented and the pointer to the existing resource is returned.
		CResourceDepthbuffer* addDepthbuffer(const std::string& strResourceName, unsigned int uiWidth, unsigned int uiHeight);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		CResourceDepthbuffer* getDepthbuffer(const std::string& strResourceName);

		// Returns whether a named resource exists
		bool getDepthbufferExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		// If the resource has been added multiple times and it's count value is greater than 1, the value is reduced, but the resource remains.
		void removeDepthbuffer(const std::string& strResourceName);

		// Adds a new font object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		// strFontFilename is the base name of the pair of files used to hold the font data
		// If the named resource already exists, it has a count value which is incremented and the pointer to the existing resource is returned.
		CResourceFont* addFont(const std::string& strResourceName, const std::string& strFontFilename);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		CResourceFont* getFont(const std::string& strResourceName);

		// Returns whether a named resource exists
		bool getFontExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		// If the resource has been added multiple times and it's count value is greater than 1, the value is reduced, but the resource remains.
		void removeFont(const std::string& strResourceName);

		// Adds a new framebuffer object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		// uiWidth and uiHeight are the dimensions of the framebuffer
		// If the named resource already exists, it has a count value which is incremented and the pointer to the existing resource is returned.
		CResourceFramebuffer* addFramebuffer(const std::string& strResourceName, unsigned int uiWidth, unsigned int uiHeight);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		CResourceFramebuffer* getFramebuffer(const std::string& strResourceName);

		// Returns whether a named resource exists
		bool getFramebufferExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		// If the resource has been added multiple times and it's count value is greater than 1, the value is reduced, but the resource remains.
		void removeFramebuffer(const std::string& strResourceName);

		// Adds a new line vertex buffer object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		// If the named resource already exists, it has a count value which is incremented and the pointer to the existing resource is returned.
		CResourceLine* addLine(const std::string& strResourceName);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		CResourceLine* getLine(const std::string& strResourceName);

		// Returns whether a named resource exists
		bool getLineExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		// If the resource has been added multiple times and it's count value is greater than 1, the value is reduced, but the resource remains.
		void removeLine(const std::string& strResourceName);

		// Adds a new shader object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		// strVertexProgramFilename and strFragmentProgramFilename are the names of the two files which hold the vertex and fragment code.
		// If the named resource already exists, it has a count value which is incremented and the pointer to the existing resource is returned.
		CResourceShader* addShader(const std::string& strResourceName, const std::string& strVertexProgramFilename, const std::string& strFragmentProgramFilename);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		CResourceShader* getShader(const std::string& strResourceName);

		// Returns whether a named resource existsgetTexture2DFromFileExists
		bool getShaderExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		// If the resource has been added multiple times and it's count value is greater than 1, the value is reduced, but the resource remains.
		void removeShader(const std::string& strResourceName);

		// Adds a new texture2D atlas object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		// const std::vector<std::string>& vecStrImageFilenames holds the name of the files which hold the image data for each of the images in the atlas/es.
		// If the named resource already exists, it has a count value which is incremented and the pointer to the existing resource is returned.
		// When the OpenGL context is destroyed and then recreated, the image data is reloaded from the Cimages containing the atlas images stored in memory.
		CResourceTexture2DAtlas* addTexture2DAtlas(const std::string& strResourceName, const std::vector<std::string>& vecStrImageFilenames, bool bAllowRotationOfImages = true, unsigned int uiImageSpacing = 1);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		CResourceTexture2DAtlas* getTexture2DAtlas(const std::string& strResourceName);

		// Returns whether a named resource exists
		bool getTexture2DAtlasExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		// If the resource has been added multiple times and it's count value is greater than 1, the value is reduced, but the resource remains.
		void removeTexture2DAtlas(const std::string& strResourceName);

		// Adds a new texture2DFromFile object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		// strImageFilename is the name of the file which holds the image data for the texture.
		// If the named resource already exists, it has a count value which is incremented and the pointer to the existing resource is returned.
		// When the OpenGL context is destroyed and then recreated, the image data is reloaded from the stored filename.
		CResourceTexture2DFromFile* addTexture2DFromFile(const std::string& strResourceName, const std::string& strImageFilename, bool bFlipYaxis = false);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		CResourceTexture2DFromFile* getTexture2DFromFile(const std::string& strResourceName);

		// Returns whether a named resource exists
		bool getTexture2DFromFileExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		// If the resource has been added multiple times and it's count value is greater than 1, the value is reduced, but the resource remains.
		void removeTexture2DFromFile(const std::string& strResourceName);

		// Adds a new texture2D object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		// strImageFilename is the name of the file which holds the image data for the texture.
		// If the named resource already exists, it has a count value which is incremented and the pointer to the existing resource is returned.
		// When the OpenGL context is destroyed and then recreated, the image data is reloaded from the stored CImage object.
		CResourceTexture2DFromImage* addTexture2DFromImage(const std::string& strResourceName, const CImage& image);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		CResourceTexture2DFromImage* getTexture2DFromImage(const std::string& strResourceName);

		// Returns whether a named resource exists
		bool getTexture2DFromImageExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		// If the resource has been added multiple times and it's count value is greater than 1, the value is reduced, but the resource remains.
		void removeTexture2DFromImage(const std::string& strResourceName);

		// Adds a new triangle object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		// If the named resource already exists, it has a count value which is incremented and the pointer to the existing resource is returned.
		CResourceTriangle* addTriangle(const std::string& strResourceName);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		CResourceTriangle* getTriangle(const std::string& strResourceName);

		// Returns whether a named resource exists
		bool getTriangleExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		// If the resource has been added multiple times and it's count value is greater than 1, the value is reduced, but the resource remains.
		void removeTriangle(const std::string& strResourceName);

	private:
		struct SResourceDepthbuffer
		{
			CResourceDepthbuffer* pResource;	// Pointer to the resource
			unsigned int uiCount;			// Number of times the resource has been added
		};
		std::map<std::string, SResourceDepthbuffer>	_mmapResDepthbuffers;

		struct SResourceFont
		{
			CResourceFont* pResource;	// Pointer to the resource
			unsigned int uiCount;		// Number of times the resource has been added
		};
		std::map<std::string, SResourceFont> _mmapResFonts;

		struct SResourceFramebuffer
		{
			CResourceFramebuffer* pResource;	// Pointer to the resource
			unsigned int uiCount;			// Number of times the resource has been added
		};
		std::map<std::string, SResourceFramebuffer> _mmapResFramebuffers;

		struct SResourceLine
		{
			CResourceLine* pResource;	// Pointer to the resource
			unsigned int uiCount;		// Number of times the resource has been added
		};
		std::map<std::string, SResourceLine> _mmapResLines;

		struct SResourceShader
		{
			CResourceShader* pResource;	// Pointer to the resource
			unsigned int uiCount;		// Number of times the resource has been added
		};
		std::map<std::string, SResourceShader> _mmapResShaders;

		struct SResourceTexture2DAtlas
		{
			CResourceTexture2DAtlas* pResource;	// Pointer to the resource
			unsigned int uiCount;			// Number of times the resource has been added
		};
		std::map<std::string, SResourceTexture2DAtlas> _mmapResTextures2DAtlases;

		struct SResourceTexture2DFromFile
		{
			CResourceTexture2DFromFile* pResource;	// Pointer to the resource
			unsigned int uiCount;			// Number of times the resource has been added
		};
		std::map<std::string, SResourceTexture2DFromFile> _mmapResTextures2D;

		struct SResourceTexture2DFromImage
		{
			CResourceTexture2DFromImage* pResource;	// Pointer to the resource
			unsigned int uiCount;					// Number of times the resource has been added
		};
		std::map<std::string, SResourceTexture2DFromImage> _mmapResTextures2DFromImage;

		struct SResourceTriangle
		{
			CResourceTriangle* pResource;	// Pointer to the resource
			unsigned int uiCount;			// Number of times the resource has been added
		};
		std::map<std::string, SResourceTriangle> _mmapResTriangles;
	};

}