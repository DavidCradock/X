#pragma once
#include "PCH.h"
#include "vector2f.h"
#include "colour.h"
#include "resourceManager.h"

namespace X
{
	// A C2DEntityLine is an entity which holds an individual entity to be rendered to a layer and holds it's
	// position, rotation and scale used whilst rendering itself.
	// I guess this could be used for a commercial project, but it's meant to be used for debug rendering of things.
	// Once the entity has been created, we need to set it's shape with a call to either setAsCircle or setAsQuad.
	class C2DEntityLine
	{
	public:
		friend class SC2DRenderer;

		C2DEntityLine();
		~C2DEntityLine();

		// Set the position of the entity
		// This sets the top left position of the entity, but in the rendering code, this if offset
		// by half the entity's dimensions, so the this position is the center of the entity.
		void setPosition(const CVector2f& vPosition);

		// Returns the currently set position of the entity
		CVector2f getPosition(void) const;

		// Set the scale of the entity
		void setScale(float fScaleX, float fScaleY);

		// Get the scale of the entity
		CVector2f getScale(void) const;

		// Set the rotation of this entity in degrees
		// Working in degrees is slower than in radians as there's a conversion done.
		void setRotationDegrees(float fAngleDegrees);

		// Set the rotation of this entity in radians
		// Working in radians is faster than degrees as there's no conversion to do.
		void setRotationRadians(float fAngleRadians);

		// Get the rotation of this entity in degrees
		// Working in degrees is slower than in radians as there's a conversion done.
		float getRotationDegrees(void);

		// Get the rotation of this entity in radians
		// Working in radians is faster than degrees as there's no conversion to do.
		float getRotationRadians(void);

		// Set the colour multiplier for the entity (default = white, full opacity)
		void setColour(const CColour& colour);

		// Returns the currently set colour multiplier for the entity
		CColour getColour(void) const;

		// Sets whether this entity is rendered or not.
		// By default, they are visible
		void setVisible(bool bVisible);

		// Returns whether this entity is currently set to be visible and rendered or not
		bool getVisible(void) const;

		// Renders this entity.
		// It's called from the SC2DRenderer::render() method.
		void render(CResourceVertexBufferLine* pVBLine, CResourceShader* pShader);

		// Sets this entity to be rendered as a circle
		void setAsCircle(float fRadius, int iNumberOfSegments);

		// Sets this entity to be rendered as a quad
		void setAsQuad(float fWidth, float fHeight);

		// Sets this entity to be rendered as a quad
		void setAsQuad(const CVector2f& vDimensions);

	private:
		CVector2f _mv2fPosition;						// Position of this entity in the world
		CVector2f _mv2fScale;							// Scale of this entity
		float _mfRotationRadians;						// Rotation of the entity in radians
		CColour _mColour;								// Colour used for the vertex colour of this entity.
		bool _mbVisible;								// Whether this entity is visible and rendered or not

		// Whether the last call was either to setAsCircle or setAsQuad
		bool _mbSetAsQuadNotCircle;
		CVector2f _mvQuadDims;		// If set to be rendered as a quad, this is it's dimenions
		float _mfCircleRadius;		// If set to be rendered as a circle, this is it's radius
		int _miCircleNumSegments;	// If set to be rendered as a circle, this is the number of segments used.

		// The layer this entity belongs to.
		// This is set in the constructor and is used in various methods such as setPosition,
		// so that the entity's position is updated in the layer's quad tree
		//C2DLayer* _mpLayerOwner;
	};
}