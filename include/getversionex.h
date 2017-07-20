#pragma once
#ifdef _MSC_VER
// superseeding of deprecated GetVersionEx
// dwBuildNumber and wSuiteMask are not implemented, if need add it
BOOL MyGetVersionExA(OSVERSIONINFOA* param);
#endif // _MSC_VER