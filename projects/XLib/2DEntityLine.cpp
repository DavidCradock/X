#include "PCH.h"
#include "2DEntityLine.h"
#include "log.h"

namespace X
{
	C2DEntityLine::C2DEntityLine()
	{
		_mv2fPosition.setZero();
		_mv2fScale.set(1, 1);
		_mfRotationRadians = 0.0f;
		_mColour.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mbVisible = true;

		_mbSetAsQuadNotCircle = true;
		_mvQuadDims.set(1, 1);
		_mfCircleRadius = 10;
		_miCircleNumSegments = 6;
	}

	C2DEntityLine::~C2DEntityLine()
	{

	}

	void C2DEntityLine::setPosition(const CVector2f& vPosition)
	{
		_mv2fPosition = vPosition;
	}

	CVector2f C2DEntityLine::getPosition(void) const
	{
		return _mv2fPosition;
	}

	void C2DEntityLine::setScale(float fScaleX, float fScaleY)
	{
		_mv2fScale.set(fScaleX, fScaleY);
	}

	CVector2f C2DEntityLine::getScale(void) const
	{
		return _mv2fScale;
	}

	void C2DEntityLine::setRotationDegrees(float fAngleDegrees)
	{
		_mfRotationRadians = deg2rad(fAngleDegrees);
	}

	void C2DEntityLine::setRotationRadians(float fAngleRadians)
	{
		_mfRotationRadians = fAngleRadians;
	}

	float C2DEntityLine::getRotationDegrees(void)
	{
		return rad2deg(_mfRotationRadians);
	}

	float C2DEntityLine::getRotationRadians(void)
	{
		return _mfRotationRadians;
	}

	void C2DEntityLine::setColour(const CColour& colour)
	{
		_mColour = colour;
	}

	CColour C2DEntityLine::getColour(void) const
	{
		return _mColour;
	}

	void C2DEntityLine::setVisible(bool bVisible)
	{
		_mbVisible = bVisible;
	}

	bool C2DEntityLine::getVisible(void) const
	{
		return _mbVisible;
	}

	void C2DEntityLine::render(CResourceVertexBufferLine* pVBLine, CResourceShader* pShader)
	{
		// If this entity is set as invisible, do nuffin'
		if (!_mbVisible)
			return;

		// Clear the line vertex buffer
		pVBLine->removeGeom();

		if (_mbSetAsQuadNotCircle)
		{
			pVBLine->addQuad(CVector2f(), _mvQuadDims.x, _mvQuadDims.y, _mColour);
		}
		else
		{
			pVBLine->addCircle(CVector2f(), _mfCircleRadius, _miCircleNumSegments, _mColour);
		}
		pVBLine->update();

		// Create rotation matrix for this entity
		CMatrix matrixRotation;
		matrixRotation.setFromAxisAngleRadians(CVector3f(0.0f, 0.0f, 1.0f), _mfRotationRadians);

		// Create scale matrix for this entity
		CMatrix matrixScale;
		CVector3f v3fScale(_mv2fScale.x, _mv2fScale.y, 1.0f);
		matrixScale.setScale(v3fScale);

		// Create translation matrix for this entity
		CMatrix matrixTranslation;
		matrixTranslation.setTranslation(_mv2fPosition.x, _mv2fPosition.y, 0.0f);

		// Create world matrix
		CMatrix matrixWorld = matrixTranslation * matrixRotation * matrixScale;

		// Send world matrix to the shader
		pShader->setMat4("matrixWorld", matrixWorld);

		pVBLine->render();
	}

	void C2DEntityLine::setAsCircle(float fRadius, int iNumberOfSegments)
	{
		_mbSetAsQuadNotCircle = false;
		_mfCircleRadius = fRadius;
		_miCircleNumSegments = iNumberOfSegments;
	}

	void C2DEntityLine::setAsQuad(float fWidth, float fHeight)
	{
		_mbSetAsQuadNotCircle = true;
		_mvQuadDims.set(fWidth, fHeight);
	}

	void C2DEntityLine::setAsQuad(const CVector2f& vDimensions)
	{
		_mbSetAsQuadNotCircle = true;
		_mvQuadDims = vDimensions;
	}
}