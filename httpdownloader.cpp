#include "stdafx.h"

#include "httpdownloader.h"
#include "httpsession.h"
#include "httprequest.h"
#include "httpresponse.h"
#include "iniparser.h"
#include "logic.h"


using namespace Update;


CHttpDownloader::CHttpDownloader(
	const char* address, 
   const char* objName,
   const char* targetDir,
   const char* targetFileName,
	Http_SecChannel secChannel,
	uint16_t port,
	uint32_t maxDownloadedSize,
	bool useTemporaryFile,
	bool useProgressBar
	)
: m_secChannel(secChannel)
, m_port(port)
, m_stream(nullptr)
, m_maxDownloadedSize(maxDownloadedSize)
, m_useTemporary(useTemporaryFile)
, m_useProgressBar(useProgressBar)
{
	m_objName = objName;
	m_address = address;
	m_targDir = targetDir;
	m_targFileName = targetFileName;
	m_addresAndObject = m_address;
	m_addresAndObject += m_objName;
}

CHttpDownloader::~CHttpDownloader(void)
{
	CloseDownloadedFile();
}
void CHttpDownloader::CloseDownloadedFile(void)
{
	if(m_stream != NULL)
		fclose(m_stream);
	m_stream = NULL;
}

bool CHttpDownloader::Download(uint32_t& httpStatus, bool& maxSizeExceeded,  uint32_t& err, uint32_t* abort, CURL** curlSession, bool hideCertQuestions, char* progBarTitle)
{
	CHttpSession* sess = NULL;
	CHttpRequest* req = NULL;
	uint32_t recvBytes = 0;
	uint32_t abortedByUser = 0;
	std::string downloadedFilePath;
	bool result = false;
	
   try {

      std::unique_ptr<CHttpSession>
         sess(
            new CHttpSession()
         );
      if (sess.get() == NULL)
         return false;

      std::unique_ptr<CHttpRequest>
         req(
            new
            CHttpRequest(
               sess->m_hSession,
               m_address.c_str(),
               m_objName.c_str(),
               m_secChannel
            )
         );
      if (req.get() == NULL)
         return false;

      downloadedFilePath = m_targDir;
      downloadedFilePath += m_targFileName;
      if (m_useTemporary)
         m_stream = fopen(downloadedFilePath.c_str(), "w+bD");
      else
         m_stream = fopen(downloadedFilePath.c_str(), "w+b");

      if (m_stream) {
         err = req->Send(m_stream, m_useProgressBar, progBarTitle ? std::string(progBarTitle) : std::string(""), m_maxDownloadedSize, abort);
         
         if (req->m_response) {
            uint32_t originalLen = 0;

               req->m_response->GetStatus(
                  httpStatus,
                  maxSizeExceeded,
                  &recvBytes,
                  &originalLen,
                  &abortedByUser
               );

            if (m_useTemporary == false) {
               fclose(m_stream);
               m_stream = NULL;
            }

            if (httpStatus == 200 && maxSizeExceeded == false && err == 0 && abortedByUser == 0)
               result = true;
            else               
               remove(downloadedFilePath.c_str());
         }
         else {
            httpStatus = 0;
         }
      }
      else
         return false;

	} catch(std::exception e) {
      if (m_stream != NULL)
      	fclose(m_stream);
      m_stream = NULL;
		remove(downloadedFilePath.c_str());
		// alert
	} catch(...) {
      if (m_stream != NULL)
         fclose(m_stream);
      m_stream = NULL;
		remove(downloadedFilePath.c_str());
		// alert
	}

	return result;
}
void CHttpDownloader::GetDownloadedFilePath(std::string& filePath)
{
	filePath = m_targDir;
	filePath += m_targFileName;
}
bool CHttpDownloader::Unix2Dos(void)
{
#ifdef _MSC_VER
	char curChar = 0;
	char prevChar = 0;
	const char r = '\r';
	const char c = '_';

	if(m_stream == NULL)
		return false;

	FILE* dos = fopen("winText", "w+bD");
	if(dos == NULL)
		return NULL;

	fseek(m_stream, 0, SEEK_SET);
	while(1)	{
		if(feof(m_stream))
			break;
		fread(&curChar, 1, 1, m_stream);
		// it changes LF for CRLF
		if(curChar == '\n' && prevChar != '\r')	{
			fwrite(&r, 1, 1, dos);
			fwrite(&curChar, 1, 1, dos);
		} else 
		if((curChar == '-' && prevChar == '-') || (curChar == '-' && prevChar == '\n') )
		{
			fwrite(&c, 1, 1, dos);
		} else {
			fwrite(&curChar, 1, 1, dos);
		}
		prevChar = curChar;
	}
	fclose(m_stream);
	m_stream = dos;
#endif
	return true;
}



