#pragma once
#include "PCH.h"
#include "singleton.h"
#include "resourceDepthbuffer.h"
#include "resourceFont.h"
#include "resourceFramebuffer.h"
#include "resourceShader.h"
#include "resourceTexture2D.h"
#include "resourceTriangle.h"
#include "resourceLine.h"

namespace X
{
	// This resource manager holds various resources which require an OpenGL context
	// All resources, once added are ready to go, aka RAII (Resource Acquistion Is Initialisation)
	// All resources are derived from the ResourceBase class and have the pure virtual methods defined in their own files.
	// There are several resources added upon initialisation which are used by various classes these are...
	// X:shader_font				// A shader used by the ResourceFont class to render text.
	// X:shader_line				// A shader used by the ResourceLine class to render lines.
	// X:shader_DRNE				// A shader which has vertex position, colour, texture coordinates and diffuse, roughness, normals and emission textures and shadows
	// X:shader_DRNE_noshadows		// A shader which has vertex position, colour, texture coordinates and diffuse, roughness, normals and emission textures and NO shadows
	// X:shader_pos_col_tex			// A shader which accepts vertex position, colour and texture coodinates
	// X:shader_depthbuffer_debug	// A shader for rendering a bound depth buffer to a 2D quad so we can view the depth values in the depth buffer
	// X:shader_shadowdepthmap		// A shader used by the scene manager to render the depth map used for rendering shadows
	// X:texture_default_particle	// A texture for use with rendering generic particles
	// X:texture_default_white		// A texture which is tiny and white.
	// X:texture_default_diffuse	// A texture which is grey for diffuse, used if not set
	// X:texture_default_emission	// A texture which is black for emiision, used if not set
	// X:texture_default_normal		// A texture which is a flat normal map, used if not set
	// X:texture_default_roughness	// A texture which is grey for roughnessm used if not set
	// X:depthbuffer_shadows		// A depth buffer which is used by scene managers to render shadows
	// X:triangle_debug				// A triangle resource (vertex buffer) used for rendering 2D quads to the screen for debugging purposes.
	// X:framebuffer_scenemanager;	// A framebuffer which the scene managers use to render to by default.
	// They are loaded by the ApplicationManager class in it's mainLoop() method
	class ResourceManager : public Singleton<ResourceManager>
	{
	public:
		ResourceManager();

		// Call just before the OpenGL window context is about to be destroyed, to free all resources which depend upon it.
		void onGLContextToBeDestroyed(void);

		// Call this after the OpenGL window context has been recreated, to return all the resource back to their original state.
		void onGLContextRecreated(void);

		// Adds a new font object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		// strFontFilename is the base name of the pair of files used to hold the font data
		ResourceFont* addFont(const std::string& strResourceName, const std::string& strFontFilename);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		ResourceFont* getFont(const std::string& strResourceName);

		// Returns whether a named resource exists
		bool getFontExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		void removeFont(const std::string& strResourceName);

		// Adds a new framebuffer object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		// uiWidth and uiHeight are the dimensions of the framebuffer
		ResourceFramebuffer* addFramebuffer(const std::string& strResourceName, unsigned int uiWidth, unsigned int uiHeight);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		ResourceFramebuffer* getFramebuffer(const std::string& strResourceName);

		// Returns whether a named resource exists
		bool getFramebufferExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		void removeFramebuffer(const std::string& strResourceName);

		// Adds a new depthbuffer object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		// uiWidth and uiHeight are the dimensions of the depthbuffer
		ResourceDepthbuffer* addDepthbuffer(const std::string& strResourceName, unsigned int uiWidth, unsigned int uiHeight);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		ResourceDepthbuffer* getDepthbuffer(const std::string& strResourceName);

		// Returns whether a named resource exists
		bool getDepthbufferExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		void removeDepthbuffer(const std::string& strResourceName);

		// Adds a new shader object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		// strVertexProgramFilename and strFragmentProgramFilename are the names of the two files which hold the vertex and fragment code.
		ResourceShader* addShader(const std::string& strResourceName, const std::string& strVertexProgramFilename, const std::string& strFragmentProgramFilename);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		ResourceShader* getShader(const std::string& strResourceName);

		// Returns whether a named resource exists
		bool getShaderExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		void removeShader(const std::string& strResourceName);

		// Adds a new texture2D object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		// strImageFilename is the name of the file which holds the image data for the texture.
		ResourceTexture2D* addTexture2D(const std::string& strResourceName, const std::string& strImageFilename, bool bFlipYaxis = false);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		ResourceTexture2D* getTexture2D(const std::string& strResourceName);

		// Returns whether a named resource exists
		bool getTexture2DExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		void removeTexture2D(const std::string& strResourceName);

		// Adds a new triangle object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		ResourceTriangle* addTriangle(const std::string& strResourceName);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		ResourceTriangle* getTriangle(const std::string& strResourceName);

		// Returns whether a named resource exists
		bool getTriangleExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		void removeTriangle(const std::string& strResourceName);

		// Adds a new line vertex buffer object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		ResourceLine* addLine(const std::string& strResourceName);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		ResourceLine* getLine(const std::string& strResourceName);

		// Returns whether a named resource exists
		bool getLineExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		void removeLine(const std::string& strResourceName);

		// Builds a font and saves it to disk using font files installed on the current OS which can then be used by the ResourceFont class.
		// This is so that we don't have to deal with installing fonts on the end users' system and also gives us the ability to modify the generated character images inside a paint program if desired.
		// The output file names (the font.fnt and font.png files) are named based upon the strOutputBaseName.
		// For example, if the basename was BASE, the font height 12, then the output files would be BASE_12.fnt and BASE_12.png
		// strOutputBaseName is the base filename used to create the two filenames for the .fnt and .png file names
		// strFontName is the name of the font which is installed on the operating system which is used to generate the font
		// iFontHeight is the size of the font
		// bAntialiased is whether to perform antialiasing when generating the font's character images.
		// bBold, bItalic, bUnderlines and bStrikeout define the style of the font.
		// If an error occurred, an error message is shown and execution ends.
		void buildFontFiles(const std::string& strOutputBaseName, const std::string& strFontName, unsigned int iFontHeight, bool bAntialiased, bool bBold, bool bItalic, bool bUnderlined, bool bStrikeout);

	private:
		std::map<std::string, ResourceFont*>		_mmapResFonts;			// A hash map holding each named font resource
		std::map<std::string, ResourceDepthbuffer*>	_mmapResDepthbuffers;	// A hash map holding each named depthbuffer resource
		std::map<std::string, ResourceFramebuffer*>	_mmapResFramebuffers;	// A hash map holding each named framebuffer resource
		std::map<std::string, ResourceShader*>		_mmapResShaders;		// A hash map holding each named shader resource
		std::map<std::string, ResourceTexture2D*>	_mmapResTextures2D;		// A hash map holding each named 2D texture resource
		std::map<std::string, ResourceTriangle*>	_mmapResTriangles;		// A hash map holding each named triangle resource
		std::map<std::string, ResourceLine*>		_mmapResLines;			// A hash map holding each named line resource
	};

}