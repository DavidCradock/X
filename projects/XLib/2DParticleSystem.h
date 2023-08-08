#pragma once
#include "PCH.h"
#include "2DParticle.h"
#include "2DParticleAffector.h"
#include "2DParticleEmitter.h"
#include "2DParticleType.h"
#include "matrix.h"
#include "timer.h"

namespace X
{
	// A 2D particle system class used with the SC2DRenderer class.
	// 
	// This particle system is NOT fast, but it is quite flexible.
	// 
	// Particle system...
	// The particle system consists of multiple objects including emitters, affectors, particle types and the particles themselves.
	// We use just one particle system which holds all particles. This way, all particles being spawned will be able to be sorted correctly, otherwise
	// if we have multiple systems, each one, when being rendered near each other would have no way of sorting their own particles by depth of the other particle systems.
	// There is already a texture atlas created upon startup called "X:default_particle" and contains all the images stored in "data/x/textures/particles/".
	// This is the default texture atlas for the particle system when it is created.
	// You can set your own texture atlas which you can create with the SCResourceManager class.
	// For performance reasons, only one texture atlas can be set for the entire particle system.
	// 
	// See each of the classes for more information.
	// 	
	// Implementation details
	// To deal with the multiple textures, a texture atlas is used and in the shaders, different texture coordinates are used
	// to lookup the correct texture image this reduces texture bindings.
	class C2DParticleSystem
	{
		friend class C2DParticleEmitter;
	public:
		// Default constructor, sets particle system to default settings.
		C2DParticleSystem();

		// Updates and renders the particle system
		// Matricies are given from SC2DRenderer::render(), the orthographic projection has already been set.
		void render(const CMatrix& matrixView, const CMatrix& matrixProjection);

		// Removes everything from the particle system.
		// Removes all particles, particle affectors, particle emitters and particle types
		void removeAll(void);

		// Sets the texture atlas stored in SCResourceManager which all particle types use for the images.
		// By default, this is set to "X:default_particle" which stores all the images found in "data/x/textures/particles/"
		void setTextureAtlas(const std::string strName);

		/**************************************************************************************************************************************************
		Affectors
		**************************************************************************************************************************************************/

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

		/**************************************************************************************************************************************************
		Emitters
		**************************************************************************************************************************************************/

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

		/**************************************************************************************************************************************************
		Particle types
		**************************************************************************************************************************************************/

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

		// Returns the size of the particle vector which may contain dead particles
		int getNumParticles(void) {	return (int)_mvParticles.size();	}

		// Returns the size of the vector which holds indicies to dead particles in the vector which holds all particles, dead or alive
		int getNumDeadParticles(void) {	return (int)_mvIndiciesToDeadParticles.size();	}
	private:
		CTimer _mTimer;

		// Struct to hold a particle system's settings
		struct SSettings
		{
			std::string strTextureAtlasName;		// The name of the texture atlas stored in SCResourceManager used by this particle system.
		};
		SSettings _mSettings;
				
		std::vector<C2DParticle> _mvParticles;									// A vector holding each particle within the system.
		std::vector<unsigned int> _mvIndiciesToDeadParticles;					// A vector holding indicies into the _mvParticles vector where there are particles which have died.
		mutable std::map<std::string, C2DParticleAffector*> _mmapAffectors;		// A map holding each uniquely named particle affector within the system.
		mutable std::map<std::string, C2DParticleEmitter*> _mmapEmitters;		// A map holding each uniquely named particle emitter within the system.
		mutable std::map<std::string, C2DParticleType*> _mmapTypes;				// A map holding each uniquely named particle type used by the particles.

		bool _mbPaused;		// Whether the particle system is paused or not
	};
}
