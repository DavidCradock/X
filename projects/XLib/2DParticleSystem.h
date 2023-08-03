#pragma once
#include "PCH.h"
#include "vector2f.h"
#include "colour.h"
#include "resourceManager.h"

namespace X
{
	// A 2D particle system class used with the SC2DRenderer class.
	// A particle system spawns individual particles which each have their own lifespan and throughout a particle's life,
	// has various values which interpolate between values over the course of their life.
	// These values include position, rotation, radius, colour(RGBA) and various images stored inside a CResourceTextureAtlas.
	// A particle's life span is represented by a float from 0.0f (birth) through to 1.0f (death)
	// A particle can have multiple life stages set between their life, set between 0 and 1
	// For example, a particle could have 2 life stages, one at 0 (birth) with bright colours and high alpha and another at 1 (death) with dark colours and zero alpha, so it
	// fades out just before it dies.
	//
	// A particle system contains many quads rendered to the screen using instancing for reduced draw calls for great performance.
	// A particle system uses a CResourceTextureAtlas to hold the various images it uses when rendering each individual particle to reduce texture bindings.
	//
	// Implementation details
	// To deal with the multiple textures, a texture atlas is used and in the fragment shader, different texture coordinates are used to lookup the correct texture image.
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
