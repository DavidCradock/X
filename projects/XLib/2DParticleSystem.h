#pragma once
#include "PCH.h"
#include "vector2f.h"
#include "colour.h"
#include "resourceManager.h"

namespace X
{
	// A 2D particle system class used with the SC2DRenderer class.
	// 
	// A particle system consists of multiple objects including emitters, effectors, particle types and the particles themselves.
	// We use just one particle system which holds all particles. This way, all particles being spawned will be able to be sorted correctly, otherwise
	// if we have multiple systems, each one, when being rendered near each other would have no way of sorting their own particles by depth of the other particle systems.
	// 
	// Emitters...
	// 
	// 
	// Effectors...
	// 
	// 
	// 
	// Particles...
	// A particle system spawns individual particles which each have their own lifespan.
	// A particle has multiple stages set throughout it's life span.
	// Each stage has some values which include, rotation, radius, colour(RGBA) and various images stored inside a CResourceTextureAtlas.
	// A particle's life span is represented by a float from 0.0f (birth) through to 1.0f (death)
	// As a particle ages, these stage values are interpolated between to create their current appearance.
	// For example, a particle could have 2 life stages, one at 0 (birth) with bright colours and high alpha and another at 1 (death) with dark colours and zero alpha, so it
	// fades out just before it dies.
	//
	// Particle types...
	// 
	// 
	// 
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
	private:

		// Struct to hold a particle system's settings
		struct SSettings
		{

		};

		// Struct to hold an individual particle's values
		struct SParticle
		{
			float fAge;		// Current age of the particle ranging from 0.0f (Birth) to 1.0f (Death)
			CVector2f vPos;	// The particle's position within the world.
			CVector3f vVel;	// The particle's current velocity.

		};
	};
}
