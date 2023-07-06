#pragma once
#include "PCH.h"
#include "vector2r.h"
#include "colour.h"

namespace X
{
	class C2DEntity
	{
	public:
		C2DEntity();
		~C2DEntity();

		// Set the position of the sprite entity
		void setPosition(const CVector2r& vPosition);

		// Returns the currently set position of the sprite entity
		CVector2r getPosition(void);

		// Set the amount of rotation of the entity in degrees
		void setRotationDeg(real rRotationDegress);

		// Returns the amount of rotation of the entity in degrees
		real getRotationDeg(void);

		// Set the scale of the sprite (a default of 1.0f is same size as the texture used to render it)
		void setScale(real rScaleX, real rScaleY);

		// Get the scale of the sprite
		CVector2r getScale(void);

		// Set the colour multiplier for the sprite entity (default = white, full opacity)
		void setColour(const CColour& colour);

		// Returns the currently set colour multiplier for the sprite entity
		CColour getColour(void);

		// Returns this entity's current animation frame number
		unsigned int getCurrentFrameNumber(void);

		// Sets this entity's current animation frame number
		void setCurrentFrameNumber(unsigned int uiFrameNumber);
	private:
		unsigned int _muiCurrentFrameNumber;	// Current frame number of this entity
		CVector2r _mv2rPosition;				// Position of this entity in the world
		CVector2r _mv2rScale;					// Scale of this entity
		real _mrRotationDegrees;				// Rotation of this entity in degrees.
		CColour _mColour;						// Colour used for the vertex colour of this entity.
	};
}