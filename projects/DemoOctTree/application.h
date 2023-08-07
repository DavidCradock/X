#pragma once
#include "../XLib/X.h"

namespace X
{
	// 
	class CApplication : public CApplicationBase
	{
	public:
		// Override this if you need it, by default, it does nothing.
		// This method is called only once upon initialisation after window and other stuff has been initialised
		void initOnce(void);

		// Override this if you need it, by default, it does nothing.
		// This is called each time the application is set as current from the application manager.
		// AKA, we're starting this application
		void onStart(void);

		// Override this if you need it, by default, it does nothing.
		// This is called each time the application is the current application and some other application is set as current from the application manager.
		// AKA, we're stopping this application
		void onStop(void);

		// Pure virtual, so we need to override this
		// It is called once per program loop for the currently set application
		// Return false if the app wants to shutdown
		bool onUpdate(void);
	private:
		CTimer timer;

		CSceneManagerSimple _mSM;
		COctTree _mOctTree;

		struct SEntity
		{
			CVector3f vPos;
			CVector3f vRot;
			std::string strName;
			CSMEntityLine* pEntLine;
			CSMEntityVertexBuffer* pEntTri;
		};
		std::vector<SEntity> _mvEntities;

		CFrustum _mTestFrustum;		// Test frustum
	};
}