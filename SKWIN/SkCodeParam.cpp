#include <StdAfx.h>
#include <SkCodeParam.h>

bool SkCodeParam::bUseDM2ExtendedMode = true;
bool SkCodeParam::bUsePowerDebug = true;
bool SkCodeParam::bUseFixedMode = true;
bool SkCodeParam::bUseIngameDebug = false;

bool SkCodeParam::bUsePlayerWalkSound = true;


#if (DM2_EXTENDED_MODE == 0)
bool SkCodeParam::bUseVaryingPlaybackFrequency = false;
#else if (DM2_EXTENDED_MODE == 1)
bool SkCodeParam::bUseVaryingPlaybackFrequency = true;
#endif // DM2_DEBUG_SUPER_MODE



#if (DM2_DEBUG_SUPER_MODE == 0)
bool SkCodeParam::bUseSuperMode = false;
#else if (DM2_DEBUG_SUPER_MODE == 1)
bool SkCodeParam::bUseSuperMode = true;
#endif // DM2_DEBUG_SUPER_MODE