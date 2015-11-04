﻿//
//
//          InputError.h
//
//


#pragma once


//
//     インクルードファイル
//
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif
#include <dinput.h>
#include "../source/utillity/String.h"


#define RETURN_ERROR(hr) case hr:return #hr##"\n";


inline org::String Get_InputErrorString(HRESULT hr)
{
	switch (hr)
	{
		RETURN_ERROR(DIERR_OLDDIRECTINPUTVERSION);
		RETURN_ERROR(DIERR_BETADIRECTINPUTVERSION);
		RETURN_ERROR(DIERR_BADDRIVERVER);
		RETURN_ERROR(DIERR_DEVICENOTREG);

	case DIERR_NOTFOUND:
		return "DIERR_NOTFOUND , DIERR_OBJECTNOTFOUND";

		RETURN_ERROR(DIERR_INVALIDPARAM);
		RETURN_ERROR(DIERR_NOINTERFACE);
		RETURN_ERROR(DIERR_GENERIC);
		RETURN_ERROR(DIERR_OUTOFMEMORY);
		RETURN_ERROR(DIERR_UNSUPPORTED);
		RETURN_ERROR(DIERR_NOTINITIALIZED);
		RETURN_ERROR(DIERR_ALREADYINITIALIZED);
		RETURN_ERROR(DIERR_NOAGGREGATION);

	case DIERR_OTHERAPPHASPRIO:
		return "DIERR_OTHERAPPHASPRIO , DIERR_READONLY , DIERR_HANDLEEXISTS";

		RETURN_ERROR(DIERR_INPUTLOST);
		RETURN_ERROR(DIERR_ACQUIRED);
		RETURN_ERROR(DIERR_NOTACQUIRED);
		RETURN_ERROR(E_PENDING);
		RETURN_ERROR(DIERR_INSUFFICIENTPRIVS);
		RETURN_ERROR(DIERR_DEVICEFULL);
		RETURN_ERROR(DIERR_MOREDATA);
		RETURN_ERROR(DIERR_NOTDOWNLOADED);
		RETURN_ERROR(DIERR_HASEFFECTS);
		RETURN_ERROR(DIERR_NOTEXCLUSIVEACQUIRED);
		RETURN_ERROR(DIERR_INCOMPLETEEFFECT);
		RETURN_ERROR(DIERR_NOTBUFFERED);
		RETURN_ERROR(DIERR_EFFECTPLAYING);
		RETURN_ERROR(DIERR_UNPLUGGED);
		RETURN_ERROR(DIERR_REPORTFULL);
		RETURN_ERROR(DIERR_MAPFILEFAIL);
		RETURN_ERROR(DI_OK);

	default:
		break;
	}
	return "";
}
