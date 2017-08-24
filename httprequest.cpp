#include "stdafx.h"
#include "httprequest.h"
#include "httpresponse.h"
#include "httpdefs.h"
#include "update.h"

using namespace Update;

char error_buffer[512];

CHttpRequest::CHttpRequest(
   CURL* session,
   const char* address,
   const char* targetObj,
   Http_SecChannel secChannel
	)
: m_method(ReqMethod_GET)
, m_curlSession(session)
{
   long curlProtocol = 0;
   curl_easy_setopt(session, CURLOPT_ERRORBUFFER, error_buffer);// debug purposes
   switch (secChannel) {
   case SEC_CHANNEL_SSL_WITHOUT_CLIENT_CERT:
   case SEC_CHANNEL_SSL_WITH_CLIENT_CERT:
      curlProtocol = CURLPROTO_HTTPS;
      //curl_easy_setopt(session, CURLOPT_USE_SSL, CURLUSESSL_ALL);
      break;
   case SEC_CHANNEL_NO_SSL:
      curlProtocol = CURLPROTO_HTTP;
      break;
   }

   //curl_easy_setopt(session, CURLOPT_PROTOCOLS, curlProtocol);
   //CURLcode res = curl_easy_perform(session);
   std::string url = address;
   url += targetObj;
   curl_easy_setopt(session, CURLOPT_URL, url.c_str());// update.decentgo.com/update.ini?&version=0.9.0.0&os=6232
}


CHttpRequest::~CHttpRequest(void)
{
}

uint32_t CHttpRequest::Send(FILE* stream, bool createProgBar, const std::string& progBarTitle, uint32_t maxAcceptableSize, uint32_t* abort)
{
   uint32_t err = 0;

   m_response = new CHttpResponse(m_curlSession, this, stream, createProgBar, progBarTitle, maxAcceptableSize, abort);
   curl_easy_setopt(m_curlSession, CURLOPT_WRITEFUNCTION, CHttpResponse::WriteMemoryCallback);
   curl_easy_setopt(m_curlSession, CURLOPT_WRITEDATA, m_response);
   curl_easy_setopt(m_curlSession, CURLOPT_HEADERFUNCTION, CHttpResponse::Header_callback);
   curl_easy_setopt(m_curlSession, CURLOPT_HEADERDATA, m_response);
#if !defined( VERIFY_SERVER_CERT)
   curl_easy_setopt(m_curlSession, CURLOPT_SSL_VERIFYPEER , 0);
#endif
   err = (uint32_t)curl_easy_perform(m_curlSession);

   if(createProgBar && g_fn_DestroyProgBar)
      g_fn_DestroyProgBar();

	return (uint32_t)err;
}
