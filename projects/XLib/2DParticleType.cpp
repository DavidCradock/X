#include "PCH.h"
#include "2DParticleType.h"

namespace X
{
	C2DParticleType::C2DParticleType()
	{
		stageBirth.colourMin.set(1.0f, 1.0f, 1.0f, 0.5f);
		stageBirth.colourMax.set(1.0f, 1.0f, 1.0f, 1.0f);
		stageBirth.fMassMin = 0.1f;
		stageBirth.fMassMax = 1.0f;
		stageBirth.fRadiusMin = 5.0f;
		stageBirth.fRadiusMax = 10.0f;
		stageBirth.strTextureAtlasImageName = "data/x/textures/particles/particle0.png";

		stageDeath.colourMin.set(1.0f, 1.0f, 1.0f, 0.0f);
		stageDeath.colourMax.set(1.0f, 1.0f, 1.0f, 0.0f);
		stageDeath.fMassMin = 0.1f;
		stageDeath.fMassMax = 1.0f;
		stageDeath.fRadiusMin = 5.0f;
		stageDeath.fRadiusMax = 10.0f;
		stageDeath.strTextureAtlasImageName = "data/x/textures/particles/particle0.png";

		fLifeSpanMinSeconds = 1.0f;
		fLifeSpanMaxSeconds = 2.0f;
		mpTypeToSpawnOnDeath = 0;
	}
}