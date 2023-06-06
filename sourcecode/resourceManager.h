#pragma once
#include "PCH.h"
#include "singleton.h"
#include "resourceFramebuffer.h"
#include "resourceShader.h"
#include "resourceTexture2D.h"

namespace X
{
	// This resource manager holds various resources which require an OpenGL context
	// All resources, once added are ready to go, aka RAII (Resource Acquistion Is Initialisation)
	// All resources are derived from the ResourceBase class and have the pure virtual methods defined in their own files.
	class ResourceManager : public Singleton<ResourceManager>
	{
	public:

		// Call just before the OpenGL window context is about to be destroyed, to free all resources which depend upon it.
		void onGLContextToBeDestroyed(void);

		// Call this after the OpenGL window context has been recreated, to return all the resource back to their original state.
		void onGLContextRecreated(void);


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
		ResourceTexture2D* addTexture2D(const std::string& strResourceName, const std::string& strImageFilename);

		// Returns a pointer to an existing resource
		// If the resource couldn't be found, an exception is thrown
		ResourceTexture2D* getTexture2D(const std::string& strResourceName);

		// Returns whether a named resource exists
		bool getTexture2DExists(const std::string& strResourceName);

		// Removes a previously added resource from this manager
		// If the resource doesn't exist, this silently fails.
		void removeTexture2D(const std::string& strResourceName);

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

	private:
		std::map<std::string, ResourceShader*>		_mmapResShaders;		// A hash map holding each named shader resource
		std::map<std::string, ResourceTexture2D*>	_mmapResTextures2D;		// A hash map holding each named 2D texture resource
		std::map<std::string, ResourceFramebuffer*>	_mmapResFramebuffers;	// A hash map holding each named framebuffer resource
	};

}