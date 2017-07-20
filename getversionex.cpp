#include "stdafx.h"
#ifdef _MSC_VER
#include <Windows.h>
#include <VersionHelpers.h>

#include "getversionex.h"

WORD GetServicePackMajor(void)
{
	OSVERSIONINFOEXW diagOvi;
	DWORDLONG dwlConditionMask = 0;
	WORD curNum = 0;

	while(1) {
		memset(&diagOvi, 0, sizeof(diagOvi));
		diagOvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);

		dwlConditionMask = 0;
		diagOvi.wServicePackMajor = curNum;
		VER_SET_CONDITION(dwlConditionMask, VER_SERVICEPACKMAJOR, VER_EQUAL);
		if(VerifyVersionInfoW(&diagOvi, VER_SERVICEPACKMAJOR, dwlConditionMask) == TRUE) {
			return curNum;
		}
		curNum++;
	}
}

WORD GetServicePackMinor(void)
{
	OSVERSIONINFOEXW diagOvi;
	DWORDLONG dwlConditionMask = 0;
	WORD curNum = 0;

	while(1) {
		memset(&diagOvi, 0, sizeof(diagOvi));
		diagOvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);

		dwlConditionMask = 0;
		diagOvi.wServicePackMinor = curNum;
		VER_SET_CONDITION(dwlConditionMask, VER_SERVICEPACKMINOR, VER_EQUAL);
		if(VerifyVersionInfoW(&diagOvi, VER_SERVICEPACKMINOR, dwlConditionMask) == TRUE) {
			return curNum;
		}
		curNum++;
	}
}

BOOL MyGetVersionExA(OSVERSIONINFOA* param)
{
	BOOL isEx = FALSE;
	OSVERSIONINFOA* ovi = param;
	//OSVERSIONINFOEXW* oviex = NULL;
	OSVERSIONINFOEXA diagOvi;
	memset(&diagOvi, 0, sizeof(diagOvi));
	diagOvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);

	if(param->dwOSVersionInfoSize == sizeof(OSVERSIONINFOEXA))
		isEx = TRUE;

	if(IsWindows7OrGreater() == FALSE) {
		//assert(0);
		return FALSE;
	}

	ovi->dwMajorVersion = 6;
	ovi->dwMinorVersion = 1;
	ovi->dwPlatformId = VER_PLATFORM_WIN32_NT;

	if(isEx) {
		DWORDLONG dwlConditionMask = 0;

		while(1) {
			memset(&diagOvi, 0, sizeof(diagOvi));
			diagOvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXA);

			dwlConditionMask = 0;
			diagOvi.wProductType = VER_NT_WORKSTATION;
			VER_SET_CONDITION(dwlConditionMask, VER_PRODUCT_TYPE, VER_EQUAL);
			if(VerifyVersionInfo(&diagOvi, VER_PRODUCT_TYPE, dwlConditionMask) == TRUE) {
				((OSVERSIONINFOEXA*)ovi)->wProductType = VER_NT_WORKSTATION;
				break;
			}
			dwlConditionMask = 0;
			diagOvi.wProductType = VER_NT_SERVER;
			VER_SET_CONDITION(dwlConditionMask, VER_PRODUCT_TYPE, VER_EQUAL);
			if(VerifyVersionInfo(&diagOvi, VER_PRODUCT_TYPE, dwlConditionMask) == TRUE) {
				((OSVERSIONINFOEXA*)ovi)->wProductType = VER_NT_SERVER;
				break;
			}
			dwlConditionMask = 0;
			diagOvi.wProductType = VER_NT_DOMAIN_CONTROLLER;
			VER_SET_CONDITION(dwlConditionMask, VER_PRODUCT_TYPE, VER_EQUAL);
			if(VerifyVersionInfo(&diagOvi, VER_PRODUCT_TYPE, dwlConditionMask) == TRUE) {
				((OSVERSIONINFOEXA*)ovi)->wProductType = VER_NT_DOMAIN_CONTROLLER;
				break;
			}
			break;
		}
	}

	WORD spMajor = GetServicePackMajor();
	WORD spMinor = GetServicePackMinor();
	if(isEx) {
		((OSVERSIONINFOEXA*)ovi)->wServicePackMajor = spMajor;
		((OSVERSIONINFOEXA*)ovi)->wServicePackMinor = spMinor;
	}
	if(spMajor) {
		char spString[128];

		memset(spString, 0, sizeof(spString));
		lstrcpy(spString, "Service Pack ");
		_itoa(spMajor, &spString[lstrlen(spString)], 10);
		lstrcpy(ovi->szCSDVersion, spString);
	}

	// not needed:
	//if(IsWindows7SP1OrGreater() == FALSE) {
	//return TRUE;
	//}

	if(IsWindows8OrGreater() == FALSE) {
		if(isEx) {
			//((OSVERSIONINFOEXW*)ovi)->wServicePackMajor = 1;
		}
		return TRUE;
	}

	//memset(ovi->szCSDVersion, 0, sizeof(ovi->szCSDVersion));
	ovi->dwMinorVersion = 2;

	if(IsWindows8Point1OrGreater() == FALSE) {
		return TRUE;
	}

	ovi->dwMinorVersion = 3;

	if(IsWindows10OrGreater() == FALSE) {
		return TRUE;
	}

	ovi->dwMajorVersion = 10;
	ovi->dwMinorVersion = 0;

	return TRUE;
}
#endif // _MSC_VER
