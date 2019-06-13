#include "stdafx.h"

#include "httpdownloader.h"
#include "iniparser.h"
#include "logic.h"
#include "update.h"
#include "updatepathes.h"
#include "constants.h"
#include "getversionex.h"
#include "common.h"
#include "srvsettings.h"
#include "HttpDefs.h"


#ifdef _MSC_VER
#include <msi.h>
#endif


void MpGetCurrentSystemTime(int64_t* currentTime);

namespace Update
{
   const uint32_t MAX_DOWNLOAD_UPDATE_HELP_INI = 10000000;
   const uint32_t MAX_DOWNLOAD_UPDATE_INI = 16384;
   const uint32_t MAX_DOWNLOAD_UPDATE_HELP = 10000000;

   void CreateGuid(char* buff, int buffSize);
  
}

   using namespace Update;


   void Update::ReadInstalledVersion(std::string& prodVerMb)
   {
      uint32_t len = 0;
      unsigned short v0 = DECENT_PRODUCT_VERSION_MAJOR;
      unsigned short v1 = DECENT_PRODUCT_VERSION_MINOR;
      unsigned short v2 = DECENT_PRODUCT_VERSION_BUILD;
      unsigned short v3 = DECENT_PRODUCT_VERSION_SPECIAL;
      char prodVer[64];
      memset(prodVer, 0, sizeof(prodVer));
      uint32_t ret = 0;

      char vStr[64];
      len = 63;
      memset(vStr, 0, sizeof(vStr));
#if defined( _MSC_VER )
      ret = MsiGetProductInfo(
         DECENTSETUP_MSI_PRODUCT_CODE,
         INSTALLPROPERTY_VERSIONSTRING,
         vStr,
         (LPDWORD)&len
      );
#elif defined( __GNUC__ )
#if defined( __clang__ )
      ret = 1;
//#error "Write function which retrieves version number from installation database, or let version retrieved from above hard-coded defines"
#endif
#endif

      if (ret != 0) {
         v0 = DECENT_PRODUCT_VERSION_MAJOR;
         v1 = DECENT_PRODUCT_VERSION_MINOR;
         v2 = DECENT_PRODUCT_VERSION_BUILD;
         v3 = DECENT_PRODUCT_VERSION_SPECIAL;
         memset(prodVer, 0, sizeof(prodVer));
         sprintf(prodVer, "%i", v0);
         strcat(prodVer, ".");
         sprintf(&prodVer[strlen(prodVer)], "%i", v1);
         strcat(prodVer, ".");
         sprintf(&prodVer[strlen(prodVer)], "%i", v2);
         strcat(prodVer, ".");
         sprintf(&prodVer[strlen(prodVer)], "%i", v3);
      }
      else {
         strcat(prodVer, ".0"); // added fourth number, really *.msi works only with first three
         strcpy(prodVer, vStr);
      }
      prodVerMb = prodVer;
   }
#ifdef _MSC_VER
   typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
   typedef BOOL(WINAPI *LPFN_Wow64DisableWow64FsRedirection) (PVOID*);
   typedef BOOL(WINAPI *LPFN_Wow64RevertWow64FsRedirection) (PVOID);
#endif

   void Update::CreateGuid(char* buff, int buffSize)
   {
#ifdef _MSC_VER
      UUID guid;
      RPC_STATUS status = UuidCreate(&guid);
      RPC_CSTR guidStr = NULL;
      status = UuidToStringA(&guid, &guidStr);
      if(status == 0) {
         strncpy(buff, (const char*)guidStr, buffSize);
         RpcStringFreeA(&guidStr);
      }
#else
      //TODO: add for other platforms 
#endif
   }

   void Update::MakeRestOfURL(std::string& targetObj, int updateCounter, uint64_t licenseUserID)
   {
      //CIdentification identification;
      //identification.Load();
      targetObj += "?";
      /*
      wchar_t licenseUsrIDStr[32];
      memset(licenseUsrIDStr, 0, sizeof(licenseUsrIDStr));
      _snwprintf(licenseUsrIDStr, sizeof(licenseUsrIDStr), L"%016I64x", licenseUserID);
  
      targetObj += L"key=";
      targetObj += licenseUsrIDStr;
      targetObj += L"&";
 
      // identification:

      targetObj += L"int=";
      targetObj += identification.m_keyStrUni.c_str();
      // disk id:
      WCHAR diskid[64];
      char diskidMb[64];
      memset(diskid, 0, sizeof(diskid));
      memset(diskidMb, 0, sizeof(diskidMb));
      getHardDriveComputerID(diskidMb);
      mbstowcs(diskid, diskidMb, sizeof(diskid)/sizeof(WCHAR)-1);

      targetObj += L"&h=";
      targetObj += diskid;

      // update counter:
      targetObj += L"&";
      wchar_t num[16];
      memset(num, 0, sizeof(num));
      _ultow(updateCounter, num, 10);
      targetObj += L"c=";
      targetObj += num;
      */
      // product version
      std::string prodVerMb;
      
      ReadInstalledVersion(prodVerMb);
      targetObj += "&";
      targetObj += "version=";
      targetObj += prodVerMb;

      targetObj += "&";
      targetObj += "os=";

#if defined( _MSC_VER )
      OSVERSIONINFO ovi;
      ZeroMemory(&ovi, sizeof(ovi));
      ovi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
      MyGetVersionExA(&ovi);
      
      char verNum[16];
      memset(verNum, 0, sizeof(verNum));
      _itoa(ovi.dwMajorVersion, verNum, 10);
      targetObj += verNum;
      memset(verNum, 0, sizeof(verNum));
      _itoa(ovi.dwMinorVersion, verNum, 10);
      targetObj += verNum;
      LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandleA("kernel32"), "IsWow64Process");
      //LPFN_Wow64DisableWow64FsRedirection fnWow64DisableWow64FsRedirection = 
      //	(LPFN_Wow64DisableWow64FsRedirection)GetProcAddress(GetModuleHandleA("kernel32"), "Wow64DisableWow64FsRedirection");
      //LPFN_Wow64RevertWow64FsRedirection fnWow64RevertWow64FsRedirection =
      //	(LPFN_Wow64RevertWow64FsRedirection)GetProcAddress(GetModuleHandleA("kernel32"), "Wow64RevertWow64FsRedirection");
      PVOID OldValue = NULL;
      BOOL isWow64 = FALSE;
      //BOOL bRet = FALSE;
      if (fnIsWow64Process != NULL) {

         fnIsWow64Process(GetCurrentProcess(), &isWow64);
      }
      if (isWow64 == TRUE) {
         targetObj += "32";
      }
      else {
         targetObj += "64";
      }
      
      char name[64];
      memset(name, 0, sizeof(name));
      DWORD err = SrvSettings::Get_string_Value(name, sizeof(name) - 1, SrvSettings::UPDATE_NAME);
      if(err == ERROR_FILE_NOT_FOUND) {
         CreateGuid(name, sizeof(name) - 1);
         err = SrvSettings::Set_string_Value(name, SrvSettings::UPDATE_NAME);
         if(err != 0)
            name[0] = 0;
      }
      if(strlen(name)) {
         targetObj += "&";
         targetObj += "name=";
         targetObj += name;
      }

#elif defined( __GNUC__ )
#if defined( __clang__ )

#if __x86_64__ || __ppc64__
      targetObj += "64";
#else
      targetObj += "32";
#endif

#endif
#else
#error "Undefined compiler platform"
#endif

      targetObj += "\0";
   }

   bool Update::Update(bool invokedByUser, uint32_t updateCounter, uint32_t* abort, bool hideCertQuestions, uint64_t licenseUserID, bool* iniDownloadFinished)
   {
      uint32_t httpStatus = 0;
      bool result = true;
      bool maxSizeExceeded = false;
      uint32_t errInDownload = 0;
      uint32_t numOfChars = 0;
      ::UserUpdateResult userResult = UserUpdateResult_Undefined;
      
      CUpdatePathes pathes;
      
      // TODO if you need debug log
      CLogic logic;
      std::string targetObj = UPDATE_ROOT_DIR_ON_SERVER;
      targetObj += UPDATE_INI_FILENAME;
      MakeRestOfURL(targetObj, updateCounter, licenseUserID);
      
      std::string targetObjRevHist = UPDATE_ROOT_DIR_ON_SERVER;
      targetObjRevHist += REV_HISTORY_FILENAME;

      MakeRestOfURL(targetObjRevHist, updateCounter, licenseUserID);
      try {
         std::unique_ptr<CHttpDownloader>
            iniDownloader(
               new
               CHttpDownloader(
                  SERVER_ADDRESS,
                  targetObj.c_str(),
                  pathes.m_updateDir.c_str(),
                  UPDATE_INI_FILENAME,
                  SSL_SEC_CHANNEL,
                  USED_PORT,
                  MAX_DOWNLOAD_UPDATE_INI,	// max download size
                  true,
                  false
               )
            );

         if (iniDownloader.get()) {

            CURL* curlSession = nullptr;
            bool iniDownloadResult = iniDownloader->Download(httpStatus, maxSizeExceeded, errInDownload, abort, &curlSession, hideCertQuestions, NULL);
            if (iniDownloadFinished)
               *iniDownloadFinished = true;
            if (iniDownloadResult == false) {
               if (httpStatus == 402) {// payment required
                  // alert
               }
               else {
                  //if(invokedByUser)
                     // alert
               }
               return false;
            }
            else {

               if (httpStatus == 200 && errInDownload == 0 && maxSizeExceeded == false) {

                  // updateCounter a lastUpdate (time) sa menia tu, pretoze inak by sa stale ozyval a log by bol plny kontaktov v minutovych intervaloch
                  updateCounter++;
                  SrvSettings::Set_uint32_t_Value(updateCounter, SrvSettings::DETECT_UPDATE_COUNTER_VAL_NAME);
                  uint64_t lastUpdateFt = 0ULL;

                  MpGetCurrentSystemTime((int64_t*)&lastUpdateFt);              
                  SrvSettings::Set_uint64_t_Value(lastUpdateFt, SrvSettings::LAST_UPDATE_FILETIME_VAL_NAME);

                  CIniParser parser(iniDownloader->m_stream);
                  if (parser.Parse() == false)
                     return false;

                  if (parser.m_lines.size() == 0) {                
                     // alert
                     result = false;
                  }

                  bool foundUpdate = false;
                  result = logic.InstallSuitableVersion(
                     targetObjRevHist,
                     parser.m_lines,
                     userResult,
                     abort,
                     &curlSession,
                     hideCertQuestions,
                     updateCounter,       
                     foundUpdate,
                     httpStatus,
                     licenseUserID
                  );             
               }
               else {
                  return false;
               }
            }
         }
         result = result;

      } 
      catch (std::exception e) {
         userResult = UserUpdateResult_Error;
         result = result;
      }
      catch (...) {
         userResult = UserUpdateResult_Error;
      }

      if (invokedByUser) {
         switch (userResult) {
         case UserUpdateResult_Error:
            // alert
            break;
         case UserUpdateResult_ErrorHttpStatus: {
            char statusCode[16];
            
#if defined( _MSC_VER )
            memset(statusCode, 0, sizeof(statusCode));
            _ultoa(httpStatus, statusCode, 10);
#else
            sprintf(statusCode, "%i", httpStatus);
#endif
            // alert
         } break;
         case UserUpdateResult_ErrorMsiStarting:
            // alert
            break;
         case UserUpdateResult_NoNewerVersion:
            // alert
            break;
         }
      }

      return result;
   }

   bool Update::TryUpdate(bool invokedByUser, uint32_t updateCounter, uint32_t* abort, bool hideCertQuestions, uint64_t licenseUserID, bool* iniDownloadFinished)
   {
      bool result = false;

      try
      {
         result = Update(invokedByUser, updateCounter, abort, hideCertQuestions, licenseUserID, iniDownloadFinished);
      }
      catch (...) {
         result = result;
      }

      return result;
   }



