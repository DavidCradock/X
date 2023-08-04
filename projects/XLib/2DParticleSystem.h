#pragma once
#include "PCH.h"
#include "2DParticle.h"
#include "2DParticleAffector.h"
#include "2DParticleEmitter.h"
#include "2DParticleType.h"

namespace X
{
	// A 2D particle system class used with the SC2DRenderer class.
	// 
	// Particle system...
	// The particle system consists of multiple objects including emitters, affectors, particle types and the particles themselves.
	// We use just one particle system which holds all particles. This way, all particles being spawned will be able to be sorted correctly, otherwise
	// if we have multiple systems, each one, when being rendered near each other would have no way of sorting their own particles by depth of the other particle systems.
	// There is already a texture atlas created upon startup called "X:default_particle" and contains all the images stored in "data/x/textures/particles/".
	// By default, each particle type is set to use this texture atlas.
	// You can set your own texture atlas which you can create with the SCResourceManager class.
	// For performance reasons, only one texture atlas can be set for the entire particle system.
	// 
	// Emitters...
	// There can be multiple emitters and they are responsible for inserting particles into the system.
	// They have a position, a shape, birth rate and particle type.
	//
	// Particle types...
	// A particle type stores settings which are used to render a particle throughout it's life.
	// An emitter is set to emit particles of a given type.
	// A type can have multiple stages which represent a particle's appearance thoughout specific stages in a particle's life.
	// Each stage has colour, radius and image used for their apperance and also have physical properties such as mass.
	// 
	// Affectors...
	// These affect all particles within the system in certain ways, typically adjusting their velocity.
	// A common affector is gravity, where it applies a constant for downwards upon the particles to simulate gravity.
	// Affectors, as well as affecting all particles globally, can be assigned to particles by their particle type so that they
	// affect other particles too.
	// 
	// Particles...
	// The stars of the show! (Quite literally if given a star shaped texture :))
	// A particle has it's current position in it's lifespan, position, velocity, it's type given to it upon it's birth via an emitter.
	//
	// Implementation details
	// To deal with the multiple textures, a texture atlas is used and in the shaders, different texture coordinates are used
	// to lookup the correct texture image this reduces texture bindings.
	// A particle system contains many quads rendered to the screen using instancing for reduced draw calls for great performance.
	class C2DParticleSystem
	{
	public:
		// Default constructor, sets particle system to default settings.
		C2DParticleSystem();

		// Updates and renders the particle system
		void render(void);

		// Removes everything from the particle system.
		// Removes all particles, particle affectors, particle emitters and particle types
		void removeAll(void);

		// Adds a new affector and returns a pointer to it
		// If the named affector already exists, a pointer to that object is returned.
		C2DParticleAffector* addAffector(const std::string& strName);

		// Returns a pointer to a named particle affector.
		// If the named affector doesn't exist, an exception occurs.
		C2DParticleAffector* getAffector(const std::string& strName) const;

		// Returns true if the named affector exists
		bool getAffectorExists(const std::string& strName) const;

		// Removes the named affector.
		// If the named affector doesn't exist, this silently fails.
		void removeAffector(const std::string& strName);

		// Removes all affectors.
		void removeAllAffectors(void);

		// Returns the number of affectors
		int getNumberOfAffectors(void) const;

		// Returns the name of an affector at the specified index.
		// If an invalid index is given, an exception occurs.
		std::string getAffectorName(int iIndex) const;

		// Adds a new emitter and returns a pointer to it
		// If the named emitter already exists, a pointer to that object is returned.
		C2DParticleEmitter* addEmitter(const std::string& strName);

		// Returns a pointer to a named particle emitter.
		// If the named emitter doesn't exist, an exception occurs.
		C2DParticleEmitter* getEmitter(const std::string& strName) const;

		// Returns true if the named emitter exists
		bool getEmitterExists(const std::string& strName) const;

		// Removes the named emitter.
		// If the named emitter doesn't exist, this silently fails.
		void removeEmitter(const std::string& strName);

		// Removes all emitters.
		void removeAllEmitters(void);

		// Returns the number of emitters
		int getNumberOfEmitters(void) const;

		// Returns the name of an emitter at the specified index.
		// If an invalid index is given, an exception occurs.
		std::string getEmitterName(int iIndex) const;

		// Adds a new particle type and returns a pointer to it
		// If the named particle type already exists, a pointer to that object is returned.
		C2DParticleType* addParticleType(const std::string& strName);

		// Returns a pointer to a named particle type.
		// If the named particle type doesn't exist, an exception occurs.
		C2DParticleType* getParticleType(const std::string& strName) const;

		// Returns true if the named type exists
		bool getParticleTypeExists(const std::string& strName) const;

		// Removes the named type.
		// If the named type doesn't exist, this silently fails.
		void removeParticleType(const std::string& strName);

		// Removes all types.
		void removeAllParticleTypes(void);

		// Returns the number of types
		int getNumberOfParticleTypes(void) const;

		// Returns the name of a particle type at the specified index.
		// If an invalid index is given, an exception occurs.
		std::string getParticleTypeName(int iIndex) const;
	private:

		// Struct to hold a particle system's settings
		struct SSettings
		{

		};
		SSettings _mSettings;
				
		std::vector<C2DParticle> _mvParticles;									// A vector holding each particle within the system.
		std::vector<unsigned int> _mvIndiciesToDeadParticles;					// A vector holding indicies into the _mvParticles vector where there are particles which have died.
		mutable std::map<std::string, C2DParticleAffector*> _mmapAffectors;		// A map holding each uniquely named particle affector within the system.
		mutable std::map<std::string, C2DParticleEmitter*> _mmapEmitters;		// A map holding each uniquely named particle emitter within the system.
		mutable std::map<std::string, C2DParticleType*> _mmapTypes;				// A map holding each uniquely named particle type used by the particles.
	};
}
