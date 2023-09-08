#include "PCH.h"
#include "resourceMouseCursor.h"

namespace X
{

	CResourceMouseCursor::CResourceMouseCursor(const std::string& strFilename)
	{
		_mstrFilename = strFilename;
		onGLContextCreated();
	}

	CResourceMouseCursor::~CResourceMouseCursor()
	{
		onGLContextToBeDestroyed();
	}

	void CResourceMouseCursor::onGLContextCreated(void)
	{
	}

	void CResourceMouseCursor::onGLContextToBeDestroyed(void)
	{

	}
}