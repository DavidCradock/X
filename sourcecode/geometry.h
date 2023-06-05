#pragma once
#include "PCH.h"
#include "singleton.h"

namespace X
{
	// Holds geometry information for rendering
	// Usage:
	// First we create some named geometry with the Geometry manager...
	// Geometry *pGeom = GeometryManager::getPointer->add("MyFirstGeom", "default");
	// Then we add some vertex information into system memory with the various add??? methods..
	// pGeom->addFromFile("data/geometry/MyFirstGeom.geo");	// Load geometry info into system memory
	// Then create the various OpenGL objects which point to the data with...
	// GeometryManager::loadGroup("default");
	// Now we're ready to draw the geometry with...
	// Geometry *pGeom = GeometryManager::getPointer()->get("MyFirstGeom", "default");
	// pGeom->draw();
	// If you wish to create geometry which changes all the time, simply to as above but then when changing
	// the geometry, simply call removeGeom() to remove all geometry from system memory, then call the various
	// add??? methods to re-create the geometry into system memory and we're all done, call draw as normal.
	// If you forget to add geometry after calling removeGeom, the draw() call will detect this and silently fail.
	class Geometry
	{
	public:
		Geometry();
		~Geometry();

		// A single vertex
		struct Vertex
		{
			glm::vec3 position;		// Vertex position
			glm::vec3 normal;		// Vertex normal
			glm::vec4 colour;		// Vertex colour
			glm::vec2 texCoord;		// Vertex texture coordinate
		};

		// Deletes OpenGL objects, but does not remove any previously added vertices
		void unload(void);

		// Removes all previously added vertices
		void removeGeom(void);

		// Once all vertices have been added or loaded with eiter of the the various add?? methods,
		// this creates the OpenGL objects and copies stuff to the GPU, ready for use.
		// If no vertices have previously been added, this silently fails.
		void load(void);

		// Binds the vertexArrayObject and calls glDrawElements()
		// If there's no vertex data, this silently fails.
		void draw(bool bWireframeMode = false);

		// Adds a new vertex, once all indicies and vertices are added, call load()
		void addVertex(const Vertex& newVertex);

		// Add a new index, once all indicies and vertices have been added, call load()
		void addIndex(int newIndex);

		// Adds a bunch of vertices and indices which represent a 2D quad
		void addQuad(const glm::vec2& vPosition, const glm::vec2& vDimensions, const glm::vec4& colour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), const glm::vec2& textureCoordinateBottomLeft = glm::vec2(0, 0), const glm::vec2& textureCoordinateBottomRight = glm::vec2(1, 0), const glm::vec2& textureCoordinateTopRight = glm::vec2(1, 1), const glm::vec2& textureCoordinateTopLeft = glm::vec2(0, 1));

		// Use the GeometryManager::convertObj() method to generate a binary file from an .obj file exported from
		// some 3D modelling package such as Blender.
		// The method will load in, convert and save out the stored geometry information into an efficient to load file which we use here
		// to load the vertex information into memory.
		// This does create the OpenGL objects, a call to load() does that.
		void addFromFile(const std::string& strGeometryFilename);

		// Return number of vertices which have been added so far.
		inline size_t getNumVertices(void)
		{
			return vertices.size();
		}

		// Return nummber of indices which have been added so far.
		inline size_t getNumIndicies(void)
		{
			return indices.size();
		}
	private:
		unsigned int vertexBufferObject;	// OpenGL vertex buffer object ID
		unsigned int vertexArrayObject;		// OpenGL vertex array object ID
		unsigned int elementBufferObject;	// OpenGL element buffer object ID
		std::vector<Vertex> vertices;		// Vector holding each unique vertex
		std::vector<int> indices;			// Vector holding indicies to each unique vertex=
	};

	// Use this to manage all geometry
	// By default, a group named "default" is created for use upon construction.
	class GeometryManager : public Singleton<GeometryManager>
	{
	public:
		GeometryManager();

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

		// Reloads the geometry into memory
		// This is used if we're modifying geometry per frame
		// Recreates the required OpenGL objects
		// It simply calls unload and then load of the object.
		void reload(const std::string& strResourceName, const std::string& strGroupName = "default");

		// Adds a new resource to the named group
		// If the group name doesn't exist, an exception occurs.
		// If the resource name already exists, the resource's reference count is increased
		// If the resource doesn't previously exist and it's newly created, it'll be in it's unloaded state
		Geometry* add(const std::string& strNewResourceName, const std::string& strGroupName = "default");

		// Returns a pointer to the named resource in it's named group
		// If either the group given doesn't exist, or the named resource doesn't exist, an exception occurs
		// Also, if the resource is in the unloaded state, it is loaded here
		Geometry* get(const std::string& strResourceName, const std::string& strGroupName = "default");

		// Returns true if the named resource exists in the named group, else false
		bool getExists(const std::string& strResourceName, const std::string& strGroupName = "default");

		// Removes the named resource from the named group
		// If either the resource or the group that it's in doesn't exist, an exception occurs
		void remove(const std::string& strResourceName, const std::string& strGroupName);

		// Call this from where ever we can detect that the OpenGL context is about to be destoyed (ApplicationManager is a good choice)
		// so that this manager can go each of the groups and store which resources are currently loaded and then unload them.
		// After the context has been lost we then call _onOpenGLContextRestored() to reload those resources back in again.
		void _onOpenGLContextLost(void);

		// Pair with _onOpenGLContextLost()
		void _onOpenGLContextRestored(void);

		// Converts an .obj file to our custom geometry file format and saves to disk
		void convertObj(const std::string filename);

	private:

		// A resource and various variables needed by the manager for each resource
		struct Resource
		{
			Geometry* pResource;			// Pointer to the resource
			unsigned int uiReferenceCount;	// How many times the resource has been added/removed
			bool bLoaded;					// Whether the resource is loaded or not
			bool bLoadedContextLostBackup;	// Whether the resource is loaded or not BACKUP for OpenGL lost context methods.
											
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