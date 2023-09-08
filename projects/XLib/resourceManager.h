#pragma once
#include "PCH.h"
#include "singleton.h"
#include "resourceDepthbuffer.h"
#include "resourceFont.h"
#include "resourceFramebuffer.h"
#include "resourceMouseCursor.h"
#include "resourceShader.h"
#include "resourceTexture2DAtlas.h"
#include "resourceTexture2DFromFile.h"
#include "resourceTexture2DFromImage.h"
#include "resourceVertexBufferCPT.h"
#include "resourceVertexBufferCPT2.h"
#include "resourceVertexBufferCPTBNT.h"
#include "resourceVertexBufferCPTInst.h"
#include "resourceVertexBufferLine.h"

namespace X
{
	// This resource manager holds various resources which require an OpenGL context.
	// It also holds some which do not.
	// All resources, once added are ready to go, aka RAII (Resource Acquistion Is Initialisation)
	// All resources are derived from the CResourceBase class and have the pure virtual methods defined in their own files.
	//
	// Default Resources.
	// There are quite a few default resources added upon initialisation which are used all over the place and
	// their type and names can be found by accessing the public SDefaultResourceNames struct defaultRes.
	// 
	// They are added/loaded/created by this class's addDefaultResources() method which is called from SCSingletons constructor
	// You can either remember the name and use that directly, or get the string holding the name of each default resource
	// by accessing the public SDefaultResourceNames struct defaultRes.
	// So instead of having to remember the name and go...
	// x->pResource->getFramebuffer("X:backbuffer");
	// We can go...
	// x->pResource->getFramebuffer(defaultRes.framebuffer_backbuffer_FB);
	class SCResourceManager : public CSingleton<SCResourceManager>
	{
	public:
		// Holds strings which store the default resource names which we can use when calling any of the resource manager's
		// methods which accept a unique resource name.
		// The variables are named... ResoureType_Name so for example, defaultRes.framebuffer_backbuffer_FB holds the name of a framebuffer which is used as the backbuffer.
		struct SDefaultResourceNames
		{
			std::string depthbuffer_shadows;					// Holds the name "X:shadows" of the default depth buffer which is used by scene managers to render shadows
			std::string framebuffer_backbuffer_FB;				// Holds the name "X:backbuffer" of the default framebuffer stuff is rendered to and then at the end of the program loop, rendered to the backbuffer
			std::string framebuffer_gui;						// Holds the name "X:gui" of the default framebuffer which the GUI is rendered to. It is set to the dimensions of the application's window as we don't want the GUI rendered to the possibly scaled back buffer
			std::string framebuffer_guitooltipFB;				// Holds the name "X:guitooltipFB" of the default framebuffer the GUI tooltips are rendered to
			std::string framebuffer_ui;							// Holds the name "X:ui" of the default framebuffer which the UI is rendered to. It is set to the dimensions of the application's window as we don't want the UI rendered to the possibly scaled back buffer
//			std::string framebuffer_uitooltipFB;				// Holds the name "X:uitooltipFB" of the default framebuffer the UI tooltips are rendered to
			std::string font_default;							// Holds the name "X:default" of the font object used for quick font rendering, usually used by debug code.
			std::string mouseCursorDefaultNormal;				// Holds the name "X:default_normal" of the mouse cursor object used for the default state of the mouse cursor
			std::string mouseCursorDefaultBusy;					// Holds the name "X:default_busy" of the mouse cursor object used for the busy state of the mouse cursor
			std::string mouseCursorDefaultResize_TtoB;			// Holds the name "X:default_resize_TtoB" of the mouse cursor object used for the resizing horizontal state of the mouse cursor
			std::string mouseCursorDefaultResize_LtoR;			// Holds the name "X:default_resize_LtoR" of the mouse cursor object used for the resizing vertical state of the mouse cursor
			std::string mouseCursorDefaultResize_TLtoBR;		// Holds the name "X:default_resize_TLtoBR" of the mouse cursor object used for the resizing diagonal TL to BR state of the mouse cursor
			std::string mouseCursorDefaultResize_TRtoBL;		// Holds the name "X:default_resize_TRtoBL" of the mouse cursor object used for the resizing diagonal TR to BL state of the mouse cursor
			std::string shader_DRNE;							// Holds the name "X:DRNE" of the default shader which has vertex position, colour, texture coordinates and diffuse, roughness, normals and emission textures and shadows
			std::string shader_DRNE_noshadows;					// Holds the name "X:DRNE_noshadows" of the default shader which has vertex position, colour, texture coordinates and diffuse, roughness, normals and emission textures and NO shadows
			std::string shader_depthbuffer_debug;				// Holds the name "X:depthbuffer_debug" of the default shader for rendering a bound depth buffer to a 2D quad so we can view the depth values in the depth buffer
			std::string shader_shadowdepthmap;					// Holds the name "X:shadowdepthmap" of the default shader used by the scene manager to render the depth map used for rendering shadows
			std::string shader_gui;								// Holds the name "X:gui" of the default shader used by the GUI to render everything.
			std::string shader_ui;								// Holds the name "X:ui" of the default shader used by the UI to render most things.
			std::string shader_VBCPT;							// Holds the name "X:VBCPT" of the default shader used with CResourceVertexBufferCPT
			std::string shader_VBCPT2;							// Holds the name "X:VBCPT2" of the default shader used with CResourceVertexBufferCPT
			std::string shader_VBCPTInst;						// Holds the name "X:VBCPTInst" of the default shader used with CResourceVertexBufferCPTInst
			std::string texture2DAtlas_default_particle;		// Holds the name "X:default_particle" of the default texture atlas for use with rendering generic particles
			std::string texture2DFromFile_default_white;		// Holds the name "X:default_white" of the default texture which is tiny and white.
			std::string texture2DFromFile_default_diffuse;		// Holds the name "X:default_diffuse" of the default texture which is grey for diffuse, used if not set
			std::string texture2DFromFile_default_emission;		// Holds the name "X:default_emission" of the default texture which is black for emission, used if not set
			std::string texture2DFromFile_default_normal;		// Holds the name "X:default_normal" of the default texture which is a flat normal map, used if not set
			std::string texture2DFromFile_default_roughness;	// Holds the name "X:default_roughness" of the default texture which is black for roughness used if not set
			std::string vertexbufferCPT_default;				// Holds the name "X:default" of the default vertex buffer CPT resource
			std::string vertexbufferCPT2_default;				// Holds the name "X:default" of the default vertex buffer CPT2 resource
			std::string vertexbufferCPTInst_default;			// Holds the name "X:default" of the default vertex buffer CPT instanced resource used for rendering various things such as the C2DParticleSystem's particles
			std::string vertexbufferCPTBNT_default;				// Holds the name "X:default" of the default vertex buffer CPTBNT resource used for rendering vertices with computed Binormal, Normal and Tangents used for normal mapping.
			std::string vertexbufferLine_default;				// Holds the name "X:default" of the default vertex buffer line resource used by the GUI when rendering lines
			
		};
		SDefaultResourceNames defaultRes;	// A struct holding the names of all default resources added to the resource manager

		SCResourceManager();

		// Call just before the OpenGL window context is about to be destroyed, to free all resources which depend upon it.
		void onGLContextToBeDestroyed(void);

		// Call this after the OpenGL window context has been recreated, to return all the resource back to their original state.
		void onGLContextRecreated(void);

		// As the back buffer frame buffer is needed alot, here's a method to return a pointer to it.
		// This saves us from having to type...
		// CResourceFramebuffer* pBackbuffer = x->pResource->getFramebuffer(x->pResource->defaultRes.framebuffer_backbuffer_FB);
		// or
		// CResourceFramebuffer* pBackbuffer = x->pResource->getFramebuffer("X:backbuffer");
		CResourceFramebuffer* getBackbuffer(void);

		// As the back buffer frame buffer's dimensions are needed alot, here's a method to return them.
		// This saves us from having to type...
		// CResourceFramebuffer* pBackbuffer = x->pResource->getFramebuffer(x->pResource->defaultRes.framebuffer_backbuffer_FB);
		// CVector2f vBackbufferDimensions = pBackbuffer->getDimensions();
		// or
		// CResourceFramebuffer* pBackbuffer = x->pResource->getFramebuffer("X:backbuffer");
		// CVector2f vBackbufferDimensions = pBackbuffer->getDimensions();
		CVector2f getBackbufferDims(void);

		// As the back buffer frame buffer's dimensions are needed alot, here's a methoed to return the it's width
		float getBackbufferWidth(void);

		// As the back buffer frame buffer's dimensions are needed alot, here's a methoed to return the it's height
		float getBackbufferHeight(void);

		// Builds a font and saves it to disk using font files installed on the current OS which can then be used by the CResourceFont class.
		// This is so that we don't have to deal with installing fonts on the end users' system and also gives us the ability to modify the generated character images inside a paint program if desired.
		// The output file names (the font.fnt and font.png files) are named based upon the strOutputBaseName.
		// For example, if the basename was BASE, the font height 12, then the output files would be BASE_12.fnt and BASE_12.png
		// strOutputBaseName is the base filename used to create the two filenames for the .fnt and .png file names
		// strFontName is the name of the font which is installed on the operating system which is used to generate the font
		// iFontHeight is the size of the font
		// bAntialiased is whether to perform antialiasing when generating the font's character images.
		// iWeight is how "bold" the font is, a value of around 400 is "normal", 800 is considered "bold"
		// bItalic, bUnderlines and bStrikeout define the style of the font.
		// If an error occurred, an error message is shown and execution ends.
		void buildFontFiles(const std::string& strOutputBaseName = "arial", const std::string& strFontName = "arial", unsigned int iFontHeight = 20, int iWeight = 400, bool bAntialiased = true, bool bItalic = false, bool bUnderlined = false, bool bStrikeout = false, bool bAppendHeightToFilename = true) const;

		// Adds default required resources to the resource manager, called during startup from SCCApplicationManager::mainLoop()
		void addDefaultResources(void);

		// Removes all resources from the resource manager, unless they have been marked as locked which is done when they are added and bLocked is true.
		void removeAllResources(void);

		/**************************************************************************************************************************************************
		Depth buffers
		**************************************************************************************************************************************************/

		// Adds a new depthbuffer object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		// uiWidth and uiHeight are the dimensions of the depthbuffer
		// If the named resource already exists, it has a count value which is incremented and the pointer to the existing resource is returned.
		// If bLocked is true, this resource will not be removed when calling any of the remove methods such as removeAll(). It's set to true for default resources.
		CResourceDepthbuffer* addDepthbuffer(const std::string& strResourceName, unsigned int uiWidth, unsigned int uiHeight, bool bLocked = false);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		CResourceDepthbuffer* getDepthbuffer(const std::string& strResourceName);

		// Returns whether a named resource exists
		bool getDepthbufferExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		// If the resource has been added multiple times and it's count value is greater than 1, the value is reduced, but the resource remains.
		// If the resource has been marked as locked, which is done when they are added and bLocked is true, it is not removed.
		void removeDepthbuffer(const std::string& strResourceName);

		/**************************************************************************************************************************************************
		Fonts
		**************************************************************************************************************************************************/

		// Adds a new font object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		// strFontFilename is the base name of the pair of files used to hold the font data
		// If the named resource already exists, it has a count value which is incremented and the pointer to the existing resource is returned.
		// If bLocked is true, this resource will not be removed when calling any of the remove methods such as removeAll(). It's set to true for default resources.
		CResourceFont* addFont(const std::string& strResourceName, const std::string& strFontFilename, bool bLocked = false);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		CResourceFont* getFont(const std::string& strResourceName);

		// Returns whether a named resource exists
		bool getFontExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		// If the resource has been added multiple times and it's count value is greater than 1, the value is reduced, but the resource remains.
		// If the resource has been marked as locked, which is done when they are added and bLocked is true, it is not removed.
		void removeFont(const std::string& strResourceName);

		/**************************************************************************************************************************************************
		Frame buffers
		**************************************************************************************************************************************************/

		// Adds a new framebuffer object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		// uiWidth and uiHeight are the dimensions of the framebuffer
		// If the named resource already exists, it has a count value which is incremented and the pointer to the existing resource is returned.
		// If bLocked is true, this resource will not be removed when calling any of the remove methods such as removeAll(). It's set to true for default resources.
		CResourceFramebuffer* addFramebuffer(const std::string& strResourceName, unsigned int uiWidth, unsigned int uiHeight, bool bLocked = false);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		CResourceFramebuffer* getFramebuffer(const std::string& strResourceName);

		// Returns whether a named resource exists
		bool getFramebufferExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		// If the resource has been added multiple times and it's count value is greater than 1, the value is reduced, but the resource remains.
		// If the resource has been marked as locked, which is done when they are added and bLocked is true, it is not removed.
		void removeFramebuffer(const std::string& strResourceName);

		/**************************************************************************************************************************************************
		Mouse cursors
		**************************************************************************************************************************************************/

		// Adds a new mouse cursor object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		// strFilename is filename of either a .cur or .ani file holding the cursor's data.
		// If the named resource already exists, it has a count value which is incremented and the pointer to the existing resource is returned.
		// If bLocked is true, this resource will not be removed when calling any of the remove methods such as removeAll(). It's set to true for default resources.
		CResourceMouseCursor* addMouseCursor(const std::string& strResourceName, const std::string& strFilename, bool bLocked = false);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		CResourceMouseCursor* getMouseCursor(const std::string& strResourceName);

		// Returns whether a named resource exists
		bool getMouseCursorExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		// If the resource has been added multiple times and it's count value is greater than 1, the value is reduced, but the resource remains.
		// If the resource has been marked as locked, which is done when they are added and bLocked is true, it is not removed.
		void removeMouseCursor(const std::string& strResourceName);

		/**************************************************************************************************************************************************
		Shader programs
		**************************************************************************************************************************************************/

		// Adds a new shader object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		// strVertexProgramFilename and strFragmentProgramFilename are the names of the two files which hold the vertex and fragment code.
		// If the named resource already exists, it has a count value which is incremented and the pointer to the existing resource is returned.
		// If bLocked is true, this resource will not be removed when calling any of the remove methods such as removeAll(). It's set to true for default resources.
		CResourceShader* addShader(const std::string& strResourceName, const std::string& strVertexProgramFilename, const std::string& strFragmentProgramFilename, bool bLocked = false);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		CResourceShader* getShader(const std::string& strResourceName);

		// Returns whether a named resource existsgetTexture2DFromFileExists
		bool getShaderExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		// If the resource has been added multiple times and it's count value is greater than 1, the value is reduced, but the resource remains.
		// If the resource has been marked as locked, which is done when they are added and bLocked is true, it is not removed.
		void removeShader(const std::string& strResourceName);

		/**************************************************************************************************************************************************
		Texture atlas
		**************************************************************************************************************************************************/

		// Adds a new texture2D atlas object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		// const std::vector<std::string>& vecStrImageFilenames holds the name of the files which hold the image data for each of the images in the atlas/es.
		// If the named resource already exists, it has a count value which is incremented and the pointer to the existing resource is returned.
		// When the OpenGL context is destroyed and then recreated, the image data is reloaded from the Cimages containing the atlas images stored in memory.
		// If bLocked is true, this resource will not be removed when calling any of the remove methods such as removeAll(). It's set to true for default resources.
		CResourceTexture2DAtlas* addTexture2DAtlas(const std::string& strResourceName, const std::vector<std::string>& vecStrImageFilenames, bool bAllowRotationOfImages = true, unsigned int uiImageSpacing = 1, bool bLocked = false, bool bFilteringNearest = false);

		// Adds a new texture2D atlas object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		// const std::string>& strDirectoryContainingImages holds the name of the directory holding the files which hold the image data for each of the images in the atlas/es.
		// If the named resource already exists, it has a count value which is incremented and the pointer to the existing resource is returned.
		// When the OpenGL context is destroyed and then recreated, the image data is reloaded from the Cimages containing the atlas images stored in memory.
		// If bLocked is true, this resource will not be removed when calling any of the remove methods such as removeAll(). It's set to true for default resources.
		CResourceTexture2DAtlas* addTexture2DAtlas(const std::string& strResourceName, const std::string& strDirectoryContainingImages, bool bAllowRotationOfImages = true, unsigned int uiImageSpacing = 1, bool bLocked = false, bool bFilteringNearest = false);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		CResourceTexture2DAtlas* getTexture2DAtlas(const std::string& strResourceName);

		// Returns whether a named resource exists
		bool getTexture2DAtlasExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		// If the resource has been added multiple times and it's count value is greater than 1, the value is reduced, but the resource remains.
		// If the resource has been marked as locked, which is done when they are added and bLocked is true, it is not removed.
		void removeTexture2DAtlas(const std::string& strResourceName);

		/**************************************************************************************************************************************************
		Texture 2D from file
		**************************************************************************************************************************************************/

		// Adds a new texture2DFromFile object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		// strImageFilename is the name of the file which holds the image data for the texture.
		// If the named resource already exists, it has a count value which is incremented and the pointer to the existing resource is returned.
		// When the OpenGL context is destroyed and then recreated, the image data is reloaded from the stored filename.
		// If bLocked is true, this resource will not be removed when calling any of the remove methods such as removeAll(). It's set to true for default resources.
		CResourceTexture2DFromFile* addTexture2DFromFile(const std::string& strResourceName, const std::string& strImageFilename, bool bFlipYaxis = false, bool bLocked = false);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		CResourceTexture2DFromFile* getTexture2DFromFile(const std::string& strResourceName);

		// Returns whether a named resource exists
		bool getTexture2DFromFileExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		// If the resource has been added multiple times and it's count value is greater than 1, the value is reduced, but the resource remains.
		// If the resource has been marked as locked, which is done when they are added and bLocked is true, it is not removed.
		void removeTexture2DFromFile(const std::string& strResourceName);

		/**************************************************************************************************************************************************
		Texture 2D from image
		**************************************************************************************************************************************************/

		// Adds a new texture2D object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		// strImageFilename is the name of the file which holds the image data for the texture.
		// If the named resource already exists, it has a count value which is incremented and the pointer to the existing resource is returned.
		// When the OpenGL context is destroyed and then recreated, the image data is reloaded from the stored CImage object.
		// If bLocked is true, this resource will not be removed when calling any of the remove methods such as removeAll(). It's set to true for default resources.
		CResourceTexture2DFromImage* addTexture2DFromImage(const std::string& strResourceName, const CImage& image, bool bLocked = false);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		CResourceTexture2DFromImage* getTexture2DFromImage(const std::string& strResourceName);

		// Returns whether a named resource exists
		bool getTexture2DFromImageExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		// If the resource has been added multiple times and it's count value is greater than 1, the value is reduced, but the resource remains.
		// If the resource has been marked as locked, which is done when they are added and bLocked is true, it is not removed.
		void removeTexture2DFromImage(const std::string& strResourceName);

		/**************************************************************************************************************************************************
		Vertex buffer CPT (Colour, Position and Texture coordinates)
		**************************************************************************************************************************************************/

		// Adds a new vertex buffer object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		// If the named resource already exists, it has a count value which is incremented and the pointer to the existing resource is returned.
		// If bLocked is true, this resource will not be removed when calling any of the remove methods such as removeAll(). It's set to true for default resources.
		CResourceVertexBufferCPT* addVertexBufferCPT(const std::string& strResourceName, bool bLocked = false);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		CResourceVertexBufferCPT* getVertexBufferCPT(const std::string& strResourceName);

		// Returns whether a named resource exists
		bool getVertexBufferCPTExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		// If the resource has been added multiple times and it's count value is greater than 1, the value is reduced, but the resource remains.
		// If the resource has been marked as locked, which is done when they are added and bLocked is true, it is not removed.
		void removeVertexBufferCPT(const std::string& strResourceName);

		/**************************************************************************************************************************************************
		Vertex buffer CPT2 (Colour, Position and 2 x Texture coordinates)
		**************************************************************************************************************************************************/

		// Adds a new vertex buffer object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		// If the named resource already exists, it has a count value which is incremented and the pointer to the existing resource is returned.
		// If bLocked is true, this resource will not be removed when calling any of the remove methods such as removeAll(). It's set to true for default resources.
		CResourceVertexBufferCPT2* addVertexBufferCPT2(const std::string& strResourceName, bool bLocked = false);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		CResourceVertexBufferCPT2* getVertexBufferCPT2(const std::string& strResourceName);

		// Returns whether a named resource exists
		bool getVertexBufferCPT2Exists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		// If the resource has been added multiple times and it's count value is greater than 1, the value is reduced, but the resource remains.
		// If the resource has been marked as locked, which is done when they are added and bLocked is true, it is not removed.
		void removeVertexBufferCPT2(const std::string& strResourceName);

		/**************************************************************************************************************************************************
		Vertex buffer CPT BNT (Colour, Position, Texture coordinate, Binormal, Normal and Tangent)
		**************************************************************************************************************************************************/

		// Adds a new vertex buffer CPT BNT (Colour, Position, Texture coordinate, Binormal, Normal and Tangent) object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		// If the named resource already exists, it has a count value which is incremented and the pointer to the existing resource is returned.
		// If bLocked is true, this resource will not be removed when calling any of the remove methods such as removeAll(). It's set to true for default resources.
		CResourceVertexBufferCPTBNT* addVertexBufferCPTBNT(const std::string& strResourceName, bool bLocked = false);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		CResourceVertexBufferCPTBNT* getVertexBufferCPTBNT(const std::string& strResourceName);

		// Returns whether a named resource exists
		bool getVertexBufferCPTBNTExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		// If the resource has been added multiple times and it's count value is greater than 1, the value is reduced, but the resource remains.
		// If the resource has been marked as locked, which is done when they are added and bLocked is true, it is not removed.
		void removeVertexBufferCPTBNT(const std::string& strResourceName);

		/**************************************************************************************************************************************************
		Vertex buffer instanced
		**************************************************************************************************************************************************/

		// Adds a new vertex buffer object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		// If the named resource already exists, it has a count value which is incremented and the pointer to the existing resource is returned.
		// If bLocked is true, this resource will not be removed when calling any of the remove methods such as removeAll(). It's set to true for default resources.
		CResourceVertexBufferCPTInst* addVertexBufferCPTInst(const std::string& strResourceName, bool bLocked = false);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		CResourceVertexBufferCPTInst* getVertexBufferCPTInst(const std::string& strResourceName);

		// Returns whether a named resource exists
		bool getVertexBufferCPTInstExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		// If the resource has been added multiple times and it's count value is greater than 1, the value is reduced, but the resource remains.
		// If the resource has been marked as locked, which is done when they are added and bLocked is true, it is not removed.
		void removeVertexBufferCPTInst(const std::string& strResourceName);

		/**************************************************************************************************************************************************
		Vertex buffer, line rendering
		**************************************************************************************************************************************************/

		// Adds a new line vertex buffer object to the manager.
		// strResourceName is the name of the new resource which we can use to refer to it with other methods in the manager.
		// If the named resource already exists, it has a count value which is incremented and the pointer to the existing resource is returned.
		// If bLocked is true, this resource will not be removed when calling any of the remove methods such as removeAll(). It's set to true for default resources.
		CResourceVertexBufferLine* addVertexBufferLine(const std::string& strResourceName, bool bLocked = false);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		CResourceVertexBufferLine* getVertexBufferLine(const std::string& strResourceName);

		// Returns whether a named resource exists
		bool getVertexBufferLineExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		// If the resource has been added multiple times and it's count value is greater than 1, the value is reduced, but the resource remains.
		// If the resource has been marked as locked, which is done when they are added and bLocked is true, it is not removed.
		void removeVertexBufferLine(const std::string& strResourceName);
	private:
		struct SResourceDepthbuffer
		{
			CResourceDepthbuffer* pResource;	// Pointer to the resource
			unsigned int uiCount;				// Number of times the resource has been added
			bool bLocked;						// Given to add method to prevent removal of this resource.
		};
		std::map<std::string, SResourceDepthbuffer>	_mmapResDepthbuffers;

		struct SResourceFont
		{
			CResourceFont* pResource;	// Pointer to the resource
			unsigned int uiCount;		// Number of times the resource has been added
			bool bLocked;				// Given to add method to prevent removal of this resource.
		};
		std::map<std::string, SResourceFont> _mmapResFonts;

		struct SResourceFramebuffer
		{
			CResourceFramebuffer* pResource;	// Pointer to the resource
			unsigned int uiCount;				// Number of times the resource has been added
			bool bLocked;						// Given to add method to prevent removal of this resource.
		};
		std::map<std::string, SResourceFramebuffer> _mmapResFramebuffers;

		struct SResourceMouseCursor
		{
			CResourceMouseCursor* pResource;	// Pointer to the resource
			unsigned int uiCount;				// Number of times the resource has been added
			bool bLocked;						// Given to add method to prevent removal of this resource.
		};
		std::map<std::string, SResourceMouseCursor> _mmapResMouseCursors;

		struct SResourceShader
		{
			CResourceShader* pResource;	// Pointer to the resource
			unsigned int uiCount;		// Number of times the resource has been added
			bool bLocked;				// Given to add method to prevent removal of this resource.
		};
		std::map<std::string, SResourceShader> _mmapResShaders;

		struct SResourceTexture2DAtlas
		{
			CResourceTexture2DAtlas* pResource;	// Pointer to the resource
			unsigned int uiCount;				// Number of times the resource has been added
			bool bLocked;						// Given to add method to prevent removal of this resource.
		};
		std::map<std::string, SResourceTexture2DAtlas> _mmapResTextures2DAtlases;

		struct SResourceTexture2DFromFile
		{
			CResourceTexture2DFromFile* pResource;	// Pointer to the resource
			unsigned int uiCount;					// Number of times the resource has been added
			bool bLocked;							// Given to add method to prevent removal of this resource.
		};
		std::map<std::string, SResourceTexture2DFromFile> _mmapResTextures2D;

		struct SResourceTexture2DFromImage
		{
			CResourceTexture2DFromImage* pResource;	// Pointer to the resource
			unsigned int uiCount;					// Number of times the resource has been added
			bool bLocked;							// Given to add method to prevent removal of this resource.
		};
		std::map<std::string, SResourceTexture2DFromImage> _mmapResTextures2DFromImage;

		struct SResourceVertexBufferCPT
		{
			CResourceVertexBufferCPT* pResource;	// Pointer to the resource
			unsigned int uiCount;					// Number of times the resource has been added
			bool bLocked;							// Given to add method to prevent removal of this resource.
		};
		std::map<std::string, SResourceVertexBufferCPT> _mmapResVertexBufferCPTs;

		struct SResourceVertexBufferCPT2
		{
			CResourceVertexBufferCPT2* pResource;	// Pointer to the resource
			unsigned int uiCount;					// Number of times the resource has been added
			bool bLocked;							// Given to add method to prevent removal of this resource.
		};
		std::map<std::string, SResourceVertexBufferCPT2> _mmapResVertexBufferCPT2s;

		struct SResourceVertexBufferCPTBNT
		{
			CResourceVertexBufferCPTBNT* pResource;	// Pointer to the resource
			unsigned int uiCount;					// Number of times the resource has been added
			bool bLocked;							// Given to add method to prevent removal of this resource.
		};
		std::map<std::string, SResourceVertexBufferCPTBNT> _mmapResVertexBufferCPTBNTs;

		struct SResourceVertexBufferCPTInst
		{
			CResourceVertexBufferCPTInst* pResource;	// Pointer to the resource
			unsigned int uiCount;						// Number of times the resource has been added
			bool bLocked;								// Given to add method to prevent removal of this resource.
		};
		std::map<std::string, SResourceVertexBufferCPTInst> _mmapResVertexBufferCPTInsts;

		struct SResourceVertexBufferLine
		{
			CResourceVertexBufferLine* pResource;	// Pointer to the resource
			unsigned int uiCount;					// Number of times the resource has been added
			bool bLocked;							// Given to add method to prevent removal of this resource.
		};
		std::map<std::string, SResourceVertexBufferLine> _mmapResVertexBufferLines;
	};

}