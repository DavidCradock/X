#pragma once
#include "PCH.h"
#include "singleton.h"

namespace X
{
	// A Vulkan graphics pipeline
	// Usage:
	// Add a new graphics pipeline using the manager and get a pointer to it...
	// GraphicsPipeline *pGP = GraphicsPipelineManager::getPointer()->add("MyNewPipeline", "default");
	// Set the filenames of the vertex and fragment programs which have been compiled to byte code using glslc.exe
	// pGP->setFilenames("MyVertexProgram_v.spv", "MyFragmentProgram_f.spv");
	// Note: To compile the sourcecode of programs with glslc.exe, use something like... 
	// glslc.exe MyVertexProgram.v -o MyVertexProgram_v.spv
	// Now use the manager to load the thing so it's ready for use...
	// GraphicsPipelineManager::getPointer()->loadGroup("default");
	class GraphicsPipeline
	{
	public:
		GraphicsPipeline();
		~GraphicsPipeline();

		// Loads the resource so it's ready for use.
		// Used by the manager
		void load(void);

		// Unloads the resource, freeing memory.
		// Used by the manager 
		void unload(void);

		// Sets the filenames which hold the compiled .spv bytecode for the vertex and fragment programs
		void setFilenames(const std::string& strVertexProgram, const std::string& strFragmentProgram);
	private:
		// Reads in a file and returns a vector of chars
		// An exception is thrown if the file was unable to be opened.
		std::vector<char> _readFile(const std::string& filename);
		std::string mstrVertexProgramFilename;		// Holds the name of the file for the vertex program
		std::string mstrFragmentProgramFilename;	// Holds the name of the file for the fragment program
		VkPipelineLayout mvkPipelineLayout;			// Pipeline layout used to pass uniform variables such as matricies and textures to the programs
		VkPipeline mvkGraphicsPipeline;				// The graphics pipeline
	};

	// Use this to manage all graphics pipelines
	// By default, a group named "default" is created for use upon construction.
	class GraphicsPipelineManager : public Singleton<GraphicsPipelineManager>
	{
	public:
		GraphicsPipelineManager();

		// Return the number of resource groups which currently exist in the manager
		unsigned int getNumGroups(void);

		// Returns the number of resources which currently exist in the named group
		// If the named group doesn't exist, an exception occurs
		unsigned int getNumResInGroup(const std::string& strGroupName);

		// Returns the number of resources which are currently loaded in the named group
		// If the named group doesn't exist, an exception occurs
		unsigned int getNumResInGroupLoaded(const std::string& strGroupName);

		// Returns the group name, at specified index
		// If invalid index given, an exception occurs
		const std::string& getGroupName(unsigned int iGroupIndex);

		// Adds a new resource group with the given name which resources can be put into
		// If the named group already exists, this simply returns
		void addNewGroup(const std::string& strNewGroupName);

		// Returns true if the named group exists, else false
		bool groupExists(const std::string& strGroupName);

		// Will load all resources in the named group so that they're ready for use.
		// If the named group doesn't exist, an exception occurs
		// If the group was previously loaded, then some more resources were added, only the newly added resources will be loaded
		// as the previous resources will have already been loaded.
		void loadGroup(const std::string& strGroupName);

		// Will load a single unloaded resource in the named group so that it's ready for use.
		// This will load only a single unloaded resource within the named group, then return.
		// This method is usefull for creating loading screens, where you call this, then call getNumResInGroup and getNumResInGroupLoaded, when both return values from
		// these two methods are the same, all resources are loaded.
		// If the named group doesn't exist, an exception occurs
		// If the group was previously loaded, then some more resources were added, only the newly added resources will be loaded
		// as the previous resources will have already been loaded.
		void loadGroupSingle(const std::string& strGroupName);

		// Will unload all resources in the named group to free up memory.
		// If the named group doesn't exist, an exception occurs
		void unloadGroup(const std::string& strGroupName);

		// Adds a new resource to the named group
		// If the group name doesn't exist, an exception occurs.
		// If the resource name already exists, the resource's reference count is increased
		// If the resource doesn't previously exist and it's newly created, it'll be in it's unloaded state
		GraphicsPipeline* add(const std::string& strNewResourceName, const std::string& strGroupName = "default");

		// Returns a pointer to the named resource in it's named group
		// If either the group given doesn't exist, or the named resource doesn't exist, an exception occurs
		// Also, if the resource is in the unloaded state, it is loaded here
		GraphicsPipeline* get(const std::string& strResourceName, const std::string& strGroupName = "default");

		// Returns true if the named resource exists in the named group, else false
		bool getExists(const std::string& strResourceName, const std::string& strGroupName = "default");

		// Removes the named resource from the named group
		// If either the resource or the group that it's in doesn't exist, an exception occurs
		void remove(const std::string& strResourceName, const std::string& strGroupName);

	private:

		// A resource and various variables needed by the manager for each resource
		struct Resource
		{
			GraphicsPipeline* pResource;	// Pointer to the resource
			unsigned int uiReferenceCount;	// How many times the resource has been added/removed
			bool bLoaded;					// Whether the resource is loaded or not
		};
		// A resource group holding resources
		struct Group
		{
		public:
			std::map<std::string, Resource*> mmapResource;	// Hash map holding named resource
		};
		std::map<std::string, Group*> mmapGroup;			// Hash map holding named resource groups
	};

}