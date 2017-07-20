#pragma once


#define CFG_DIR_NAME L"DECENT"


   enum UserUpdateResult
   {
      UserUpdateResult_Undefined = 0,
      UserUpdateResult_Error,
      UserUpdateResult_AbortedByUser,
      UserUpdateResult_NoNewerVersion,
      UserUpdateResult_InstallationStarted,
      UserUpdateResult_ErrorHttpStatus,
      UserUpdateResult_ErrorMsiStarting,
   };


#ifdef _MSC_VER
   uint32_t MpGetAppCfgDir(wchar_t* path, int maxChars);
#elif
#define MAX_PATH 256
#endif


