#include "stdafx.h"

#include "logic.h"
#include "update.h"
#include "version.h"
#include "iniparser.h"
#include "httpdownloader.h"
#include "updatepathes.h"
#include "httpdefs.h"

#include "constants.h" 


namespace Update
{

fn_StartRevHistoryDlg g_fn_StartRevHistoryDlg = NULL;
fn_CreateProgBar g_fn_CreateProgBar = NULL;
fn_SetProgBarPos g_fn_SetProgBarPos = NULL;
fn_SetProgBarTitle g_fn_SetProgBarTitle = NULL;
fn_DestroyProgBar g_fn_DestroyProgBar = NULL;

}

using namespace Update;

const uint32_t MAX_DOWNLOAD_SETUP_SIZE = 800000000;
const uint32_t MAX_DOWNLOAD_REVHISTORY_SIZE = 0xffff;



CLogic::CLogic(void)
{
	LoadInstalledVersion();
}

CLogic::~CLogic(void)
{
}

void CLogic::LoadInstalledVersion(void)
{
	std::string versionStr;

	try
	{
		ReadInstalledVersion(versionStr);
		// system service ma problem so ziskanim product version, preto riesime touto cestou:
		m_installed = CVersion(versionStr);
		m_installed.Parse();

	} catch(std::exception e)	{
		// alert
	} catch(...)	{
		// alert
	}
}
uint32_t 
CLogic::DownloadSetupVersion(
	const char* updateSetupFileName, 
	const CVersion* downloadVersion, 
	uint32_t const maxDownloadSize, 
	uint32_t* abort, 
   CURL** hRequest,
	uint32_t& httpStatus, 
	bool& maxSizeExceeded, 
	bool hideCertQuestions,
	int updateCounter,
	unsigned __int64 licenseUserID
	)
{
	std::string targetObj;
	uint32_t errInDownload = 0;
	uint32_t numOfChars = 0;

	CUpdatePathes pathes;

	// highestVer now contains the highest offered version, try downloadf
	try {

      // setup je ulozeny v podadresari
		targetObj = UPDATE_ROOT_DIR_ON_SERVER;		
      targetObj += downloadVersion->GetVersionStr();
		targetObj += "/";
		targetObj +=  updateSetupFileName;
		
		MakeRestOfURL(targetObj, updateCounter, licenseUserID);

		std::unique_ptr<CHttpDownloader>
		downloader(
		new 
		CHttpDownloader(
			SERVER_ADDRESS,
			targetObj.c_str(),
			pathes.m_updateDir.c_str(),
			updateSetupFileName,
			SSL_SEC_CHANNEL,
			USED_PORT,
			maxDownloadSize,
			false,
			true
			)
			);
		if(downloader.get())	{
			downloader->Download(httpStatus, maxSizeExceeded, errInDownload, abort, 
            hRequest, 
            hideCertQuestions, "Downloading newer version...");
		}

	} catch(std::exception e)	{
		// alert
	} catch(...)	{
		// alert
	}
		
	return errInDownload;
}
bool CLogic::CanBeApplied(const CVersion* update, const CVersion* from, const CVersion* to)
{
	if(m_installed.GetVersionNum64() == (uint64_t)(-1))	{
		assert(0);
      return false;
	}

	if(update == nullptr)	{
		assert(0);
      return false;
	}

	if(update->GetVersionNum64() <= m_installed.GetVersionNum64())
		return false;

	if(from == nullptr && to == nullptr)
		return true;

	if(m_installed.GetVersionNum64() >= from->GetVersionNum64() && m_installed.GetVersionNum64() < to->GetVersionNum64())
		return true;
	else
		return false;
}

bool 
CLogic::InstallSuitableVersion(
	std::string targetObjRevHist, 
	IniLineMap& lines, 
	::UserUpdateResult& userResult, 
	uint32_t* abort, 
   CURL** hRequest,
	bool hideCertQuestions, 
	int updateCounter,
	bool& foundUpdate,
	uint32_t& httpStatus,
	unsigned __int64 licenseUserID
	)
{
	uint32_t err = 0;
	IniLineMap::const_iterator iter = lines.end();
	IniLineMap::const_iterator iterNewest = lines.end();
	httpStatus = 0;
	bool maxSizeExceeded = false;
	
	int numOfConverted = 0;
	int numArgs = 0;
	bool result = false;
	char updateSetupFileName[64];

	userResult = UserUpdateResult_Undefined;

   bool userClosesRevHistory = false;

	if(lines.size() == 0)
		return false;

	CUpdatePathes pathes;

	iter--;
	iterNewest = iter;
	foundUpdate = false;
	
   // finds the newest version which can be applied and installs it
	while(httpStatus != 200)	{

		if(*abort == 1)	{
			result = false;
			break;
		}
        if(CanBeApplied(iter->second.m_updateVer, iter->second.m_acumulFrom, iter->second.m_acumulTo) == TRUE)	{
			
			foundUpdate = true;

			if(DisplayRevisionHistory(targetObjRevHist, userResult, httpStatus, abort, hRequest, hideCertQuestions) == FALSE)	{
            userClosesRevHistory = true;
				break;
			}

			memset(updateSetupFileName, 0, sizeof(updateSetupFileName));
         strcpy(updateSetupFileName, iter->second.m_serverSetupFileName.c_str());
			err = 
			DownloadSetupVersion(
				updateSetupFileName, 
				iter->second.m_updateVer, 
				MAX_DOWNLOAD_SETUP_SIZE, 
				abort, 
				hRequest, 
				httpStatus, 
				maxSizeExceeded, 
				hideCertQuestions,
				updateCounter,
				licenseUserID
				);
			//httpStatus = 200;
			if(httpStatus == 200 && maxSizeExceeded == FALSE && err == 0 && *abort == FALSE)	{
#if defined( _MSC_VER )
            std::string wCmdLine1;// first part of command line (next follows path to *.msi)
            std::string wCmdLine2;// second part of command line
            STARTUPINFO si;
            PROCESS_INFORMATION pi;
            BOOL created = FALSE;

            memset(&si, 0, sizeof(si));
            si.cb = sizeof(si);
            memset(&pi, 0, sizeof(pi));

            std::string cmdLine = "msiexec ";

				//wCmdLine1.resize(512);
				//memset((void*)wCmdLine1.data(), 0, wCmdLine1.size() * sizeof(WCHAR));
				//numOfConverted = (int)mbstowcs((WCHAR*)wCmdLine1.data(), iter->second.m_msiCmdLine1.c_str(), strlen(iter->second.m_msiCmdLine1.c_str()));
				//wCmdLine1.resize(numOfConverted);
            wCmdLine1 = iter->second.m_msiCmdLine1.c_str();
            numOfConverted = (int)wCmdLine1.size();
				cmdLine += wCmdLine1;
				
				cmdLine += " \"";
				cmdLine += pathes.m_updateSetupFilePath;
				cmdLine += updateSetupFileName;
				cmdLine += "\" ";

				//wCmdLine2.resize(512);
				//memset((void*)wCmdLine2.data(), 0, wCmdLine2.size() * sizeof(WCHAR));
				//numOfConverted = (int)mbstowcs((WCHAR*)wCmdLine2.data(), iter->second.m_msiCmdLine2.c_str(), strlen(iter->second.m_msiCmdLine2.c_str()));
				//wCmdLine2.resize(numOfConverted);
            wCmdLine2 = iter->second.m_msiCmdLine2.c_str();
            numOfConverted = (int)wCmdLine2.size();
				cmdLine += wCmdLine2;
				
				created = 
				CreateProcess(
					NULL,
					(LPSTR)cmdLine.c_str(),
					NULL,
					NULL,
					FALSE,
					CREATE_NEW_CONSOLE,
					NULL,
					NULL,
					&si,
					&pi
					);
				
				if(created)	{
					Sleep(1000);
					CloseHandle(pi.hThread);
					CloseHandle(pi.hProcess);
					userResult = UserUpdateResult_InstallationStarted;
					result = true;

				} else {
					userResult = UserUpdateResult_ErrorMsiStarting;
					result = false;
				}
				
				result = true;
#elif defined( __GNUC__ )
#if defined( __clang__ )
//#error "Create code which installs package: updateSetupFileName downloaded in path: pathes.m_updateSetupFilePath"
            std::string pathToPackage = pathes.m_updateSetupFilePath;
            pathToPackage += updateSetupFileName;
            std::string cmdLine = "sudo installer -pkg ";
            cmdLine += pathToPackage;
            cmdLine += " -target /";
            
            int result = system(cmdLine.c_str());
// For example Windows creates msiexec process which runs installation package and it executes upgrade. 
// It is up on Msi to detect installed application with different version, deciding about superseeding some components
// removing old binaries and install new binaries and other tasks relating to installation package database.
#else
#error "Undefined compiler platform"
#endif
#elif
#error "Undefined compiler platform"
#endif
				break;
			} else {
				result = false;
				break;
			}
		}
		
		if(iter == lines.begin())
			break;

		iter--;
	}

	if(foundUpdate && (httpStatus != 200) )	{
		userResult = UserUpdateResult_ErrorHttpStatus;
		result = false;
	} else
	if(foundUpdate == false)	{
		userResult = UserUpdateResult_NoNewerVersion;
		result = false;
	} else 
	if(userClosesRevHistory)
	{
		result = false;
	}

	return result;
}

bool 
CLogic::DisplayRevisionHistory(
	std::string targetObjRevHist, 
	::UserUpdateResult& userResult, 
	uint32_t& httpStatus, 
	uint32_t* abort, 
   CURL** hRequest,
	bool hideCertQuestions
	)
{
	Update::CUpdatePathes pathes;
	bool maxSizeExceeded = false;
	uint32_t errInDownload = 0;
	std::string revHistory;
	uint32_t revHistSize = 0;
   uint32_t revHistDlgRes = 0;
	// download revision history:
	std::unique_ptr<CHttpDownloader>
	revHistDownloader( 
		new 
		CHttpDownloader(
			SERVER_ADDRESS,
			targetObjRevHist.c_str(),
			pathes.m_updateDir.c_str(),
			REV_HISTORY_FILENAME,
			SSL_SEC_CHANNEL,
			USED_PORT,
			MAX_DOWNLOAD_REVHISTORY_SIZE,	// max download size
			true,
			false
			) 
		);

	if(revHistDownloader.get())	{
		if(revHistDownloader->Download(httpStatus, maxSizeExceeded, errInDownload, abort, hRequest, hideCertQuestions, "") == TRUE)	{
			revHistSize = ftell(revHistDownloader->m_stream);
			if(revHistSize) {
#ifdef _MSC_VER
				revHistDownloader->Unix2Dos();
#endif
				revHistSize = ftell(revHistDownloader->m_stream);
				revHistory.resize(revHistSize );
				memset((char*)revHistory.data(), 0, revHistSize);
				fseek(revHistDownloader->m_stream, 0, SEEK_SET);
				fread((char*)revHistory.data(), sizeof(char), revHistSize, revHistDownloader->m_stream );
				
			}
		} else {
         // go, let missing history dont stop update 
			// return false
		}
	}

	// displaying revision history dialog :
	if(Update::g_fn_StartRevHistoryDlg)	{
		revHistDlgRes = Update::g_fn_StartRevHistoryDlg(revHistory);
		if(revHistDlgRes == 0) {
			// alert
			userResult = UserUpdateResult_AbortedByUser;
			return false;
		}
	}
	return true;
}
