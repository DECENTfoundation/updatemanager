#include "stdafx.h"

#include "common.h"

/*
#ifdef _MSC_VER
uint32_t MpGetAppCfgDir(wchar_t* path, int maxPathLen)
{
#if	defined( _MSC_VER )
   if (maxPathLen < ((int)MAX_PATH + (int)wcslen(CFG_DIR_NAME) + 2))
      return ERROR_INSUFFICIENT_BUFFER;
   uint32_t err = SHGetFolderPathW(NULL, CSIDL_COMMON_APPDATA, NULL, SHGFP_TYPE_CURRENT, (wchar_t*)path);
   if (err == 0) {
      wcscat((wchar_t*)path, L"\\");
      wcscat((wchar_t*)path, CFG_DIR_NAME);
   }

#elif	defined( __GNUC__ ) // TODO
   char asPath[128];
   memset(asPath, 0, sizeof(asPath));
   wcsncpy(path, u"/var/lib/", maxPathLen - 1);
   wcsncat((wchar*)path, CFG_DIR_NAME, maxPathLen - (int)wcslen(path) - 1);
#else
#error "Undefined Compiler platform"
#endif

   return 0;
}
#endif*/