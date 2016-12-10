#ifndef _SKCODE_PARAM_H_
#define _SKCODE_PARAM_H_

#include <SkVersionControl.h>

class SkCodeParam
{
public:
	static bool bUseDM2ExtendedMode;
	static bool bUsePowerDebug;
	static bool bUseFixedMode;
	static bool bUseIngameDebug;
	static bool bUseSuperMode;

	static bool bUseExtendedSound;
	static bool bUseMultilanguageExtended;

	static bool bUseVaryingPlaybackFrequency;
	static bool	bUsePlayerWalkSound;

public:
	SkCodeParam();
};

#endif // _SKCODE_PARAM_H_