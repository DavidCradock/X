#include "PCH.h"
#include "2DParticleType.h"

namespace X
{
	C2DParticleType::C2DParticleType()
	{
		fLifeSpanMinSeconds = 1.0f;
		fLifeSpanMaxSeconds = 2.0f;
		mpTypeToSpawnOnDeath = 0;
		stageBirth.colourMin.set(1.0f, 1.0f, 1.0f, 0.5f);
		stageBirth.colourMax.set(1.0f, 1.0f, 1.0f, 1.0f);
		stageBirth.fMassMin = 0.1f;
		stageBirth.fMassMax = 1.0f;
		stageBirth.fRadiusMin = 5.0f;
		stageBirth.fRadiusMax = 10.0f;
		stageBirth.strTextureAtlasImageName = "";
	}
}