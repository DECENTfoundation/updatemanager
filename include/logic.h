#pragma once

#include "iniparser.h"
#include "version.h"
#include "common.h"


namespace Update
{

class CVersion;
class CLogic
{
public:
	CLogic(void);
	~CLogic(void);
private:
	void LoadInstalledVersion(void);
	
	CVersion m_installed;

	uint32_t 
	DownloadSetupVersion(
		const char* updateSetupFileName, 
		const CVersion* downloadVersion,
		uint32_t const maxDownloadSize, 
		uint32_t* abort,

      CURL** curlSession,

		uint32_t& httpStatus,
		bool& maxSizeExceeded,
		bool hideCertQuestions,
		int updateCounter,
		unsigned __int64 licenseUserID
		);

	bool 
	DisplayRevisionHistory(
		std::string targetObjRevHist, 
		::UserUpdateResult& userResult, 
		uint32_t& httpStatus, 
		uint32_t* abort, 
      CURL** curlSession,
		bool hideCertQuestions
		);

	bool CanBeApplied(const CVersion* update, const CVersion* from, const CVersion* to);
	std::string m_targetFilePath;
public:
	bool 
	InstallSuitableVersion(
		std::string targetObjRevHist, 
		IniLineMap& line, 
		::UserUpdateResult& userResult, 
		uint32_t* abort, 
      CURL** curlSession,
		bool hideCertQuestions, 
		int updateCounter,
		bool& foundUpdate,
		uint32_t& httpStatus,
		unsigned __int64 licenseUserID
		);

};


} //end namespace