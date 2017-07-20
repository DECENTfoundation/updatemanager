#include "stdafx.h"
#include "httpsession.h"
#include "update.h"




namespace Update
{
const uint32_t UPDATE_OPTION_RECEIVE_TIMEOUT__VALUE = 20000;
const uint32_t UPDATE_OPTION_CONNECT_TIMEOUT__VALUE = 10000;
const uint32_t UPDATE_OPTION_SEND_TIMEOUT__VALUE = 10000;
}

using namespace Update;


CHttpSession::CHttpSession(void)
: m_hSession(nullptr)
{
   m_hSession = curl_easy_init();
}


CHttpSession::~CHttpSession(void)
{
   if(m_hSession)
      curl_easy_cleanup(m_hSession);
	m_hSession = nullptr;
}

