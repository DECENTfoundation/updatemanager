#include "stdafx.h"

#include "httpresponse.h"
#include "httprequest.h"
#include "update.h"


using namespace Update;



CHttpResponse::CHttpResponse(CURL* session, CHttpRequest* request, FILE* stream, bool useProgBar, const std::string& progBarTitle, uint32_t maxAcceptableSize, uint32_t* abort)
: m_stream(stream)
, m_curlSession(session)
, m_httpStatus(0)
, m_contentLength((uint32_t)(-1))
, m_receivedBytes(0)
, m_useProgBar(useProgBar)
, m_progBarTitle(progBarTitle)
, m_maxSizeExceeded(false)
, m_maxAcceptableSize(maxAcceptableSize)
, m_abort(abort)
, m_abortedByUser(false)
{
	if(!request) 
		assert(0);
}

CHttpResponse::~CHttpResponse(void)
{
}


void 
CHttpResponse::GetStatus(
	uint32_t& httpStatus,
	bool& maxSizeExceeded,
	uint32_t* recvBytes, 
	uint32_t* originalLen, 
	uint32_t* abortedByUser
	)
{
   httpStatus = m_httpStatus;
   *recvBytes = m_receivedBytes;
   *originalLen = m_contentLength;
   *abortedByUser = m_abortedByUser ? 1 : 0;
   maxSizeExceeded = m_maxSizeExceeded;
}

char* stristr(char* str1, const char* str2)
{
   char* p1 = str1;
   const char* p2 = str2;
   char* r = *p2 == 0 ? str1 : 0;

   while (*p1 != 0 && *p2 != 0)
   {
      if (tolower(*p1) == tolower(*p2))
      {
         if (r == 0)
         {
            r = p1;
         }

         p2++;
      }
      else
      {
         p2 = str2;
         if (tolower(*p1) == tolower(*p2))
         {
            r = p1;
            p2++;
         }
         else
         {
            r = 0;
         }
      }

      p1++;
   }

   return *p2 == 0 ? r : 0;
}

size_t CHttpResponse::Header_callback(char *buffer, size_t size, size_t nitems, void *userdata)
{
   // received header is nitems * size long in 'buffer' NOT ZERO TERMINATED 
   // 'userdata' is set with CURLOPT_HEADERDATA 
   CHttpResponse* resp = (CHttpResponse*)userdata;
   if (resp->m_httpStatus == 0) {
      while (*buffer == ' ' || *buffer == '\t') {
         buffer++;
      }
      std::string httpStatus(buffer, size * nitems);
      size_t begin = httpStatus.find_first_of(" \t", 0);
      begin = httpStatus.find_first_of("0123456789", begin);
      size_t end = httpStatus.find_first_of(" \t", 9);
      std::string statusNum = httpStatus.substr(begin, end - begin);
      resp->m_httpStatus = atoi(statusNum.c_str());
   }
   else {
      if (resp->m_contentLength == (uint32_t)-1) {
         char* b = stristr(buffer, "Content-Length");
         if (b) {
            char* begin = strchr(b, ':');
            if (begin) {
               begin++;
               resp->m_contentLength = atoi(begin);
               if (resp->m_useProgBar && g_fn_CreateProgBar) {
                  g_fn_CreateProgBar(resp->m_contentLength, (uint32_t*)resp->m_abort);
                  g_fn_SetProgBarTitle(resp->m_progBarTitle.c_str());
               }
            }
         }
      }
   }
   return nitems * size;
}


size_t CHttpResponse::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
   CHttpResponse* resp = (CHttpResponse*)userp;
   size_t realsize = size * nmemb;

   if (resp->m_useProgBar)
      g_fn_SetProgBarPos(resp->m_receivedBytes);

   if ((resp->m_receivedBytes + (uint32_t)realsize) > resp->m_maxAcceptableSize) {
      return (size_t)(-1);
   }

   if (resp->m_abort && *resp->m_abort) {
      resp->m_abortedByUser = true;
      return (size_t)(-1);
   }
   
   resp->m_receivedBytes += (uint32_t)realsize;

   fwrite(contents, size, nmemb, resp->m_stream);

   return realsize;
}