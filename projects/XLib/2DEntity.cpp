#include "PCH.h"
#include "2DEntity.h"

namespace X
{
	C2DEntity::C2DEntity()
	{
		_muiCurrentFrameNumber = 0;
		_mv2rPosition.setZero();
		_mv2rScale.set(1, 1);
		_mColour.set(1.0f, 1.0f, 1.0f, 1.0f);
		_mrRotationDegrees = 0;
	}

	C2DEntity::~C2DEntity()
	{

	}

	void C2DEntity::setPosition(const CVector2r& vPosition)
	{
		_mv2rPosition = vPosition;
	}

	CVector2r C2DEntity::getPosition(void)
	{
		return _mv2rPosition;
	}

	glm::vec3 C2DEntity::getPositionGLM(void)
	{
		glm::vec3 vPos;
		vPos.x = (float)_mv2rPosition.x;
		vPos.y = (float)_mv2rPosition.y;
		vPos.z = 0.0f;
		return vPos;
	}

	void C2DEntity::setRotationDeg(real rRotationDegress)
	{
		_mrRotationDegrees = rRotationDegress;
	}

	real C2DEntity::getRotationDeg(void)
	{
		return _mrRotationDegrees;
	}

	void C2DEntity::setScale(real rScaleX, real rScaleY)
	{
		_mv2rScale.set(rScaleX, rScaleY);
	}

	CVector2r C2DEntity::getScale(void)
	{
		return _mv2rScale;
	}

	void C2DEntity::setColour(const CColour& colour)
	{
		_mColour = colour;
	}

	CColour C2DEntity::getColour(void)
	{
		return _mColour;
	}

	unsigned int C2DEntity::getCurrentFrameNumber(void)
	{
		return _muiCurrentFrameNumber;
	}

	void C2DEntity::setCurrentFrameNumber(unsigned int uiFrameNumber)
	{
		_muiCurrentFrameNumber = uiFrameNumber;
	}
}