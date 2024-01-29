#pragma once
#include "PCH.h"
#include "2DMapTile.h"

namespace X
{
	// Editor of 2D maps
	class CUIDefConMapEditor
	{
	public:
		CUIDefConMapEditor();

		// Called from CUIDefaultContainers::initialise() to initialise everything
		void initialise(void);

		// Called from CUIDefaultContainers::update() to update everything
		void update(float fTimeDeltaSecs);

	private:
		C2DMapTile* _mpMapTilePrev;
	};
}