#include "PCH.h"
#include "SMEntityBase.h"

namespace X
{
	CSMEntityBase::CSMEntityBase()
	{
		matrixWorld = glm::mat4(1.0f);
		_mbWorldNeedsUpdate = false;
		_mvPosition = glm::vec3(0, 0, 0);
		_mvScale = glm::vec3(1.0f, 1.0f, 1.0f);
		_mvRotTEMP = glm::vec3(0, 0, 0);
	}

	glm::mat4 CSMEntityBase::getWorldMatrix(void)
	{
		if (_mbWorldNeedsUpdate)
			_updateWorldMatrix();
		return matrixWorld;
	}

	void CSMEntityBase::setWorldPosition(glm::vec3 vNewPosition)
	{
		_mvPosition = vNewPosition;
		_mbWorldNeedsUpdate = true;
	}

	void CSMEntityBase::translateWorld(glm::vec3 vTranslation)
	{
		_mvPosition.x += vTranslation.x;
		_mvPosition.y += vTranslation.y;
		_mvPosition.z += vTranslation.z;
		_mbWorldNeedsUpdate = true;
	}

	glm::vec3 CSMEntityBase::getWorldPosition(void) const
	{
		return _mvPosition;
	}

	void CSMEntityBase::setScale(glm::vec3 vScale)
	{
		_mvScale = vScale;
		_mbWorldNeedsUpdate = true;
	}

	glm::vec3 CSMEntityBase::getScale(void) const
	{
		return _mvScale;
	}

	void CSMEntityBase::setRotation(float fX, float fY, float fZ)
	{
		_mvRotTEMP = glm::vec3(fX, fY, fZ);
		_mbWorldNeedsUpdate = true;
	}

	void CSMEntityBase::_updateWorldMatrix(void)
	{
		glm::mat4 trans = glm::mat4(1.0f);
		trans = glm::translate(trans, _mvPosition);
		
		glm::mat4 scale = glm::mat4(1.0f);
		scale = glm::scale(scale, _mvScale);

		// TEMP rotation
		glm::mat4 rotationX = glm::mat4(1.0f);	rotationX = glm::rotate(rotationX, glm::radians(_mvRotTEMP.x), glm::vec3(1.0, 0.0, 0.0));
		glm::mat4 rotationY = glm::mat4(1.0f);	rotationY = glm::rotate(rotationY, glm::radians(_mvRotTEMP.y), glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 rotationZ = glm::mat4(1.0f);	rotationZ = glm::rotate(rotationZ, glm::radians(_mvRotTEMP.z), glm::vec3(0.0, 0.0, 1.0));
		glm::mat4 rotation = rotationX * rotationY * rotationZ;

		matrixWorld = trans * scale * rotation;

		_mbWorldNeedsUpdate = false;
	}
}