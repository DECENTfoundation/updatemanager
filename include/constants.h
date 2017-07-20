#pragma once


namespace Update
{
   char const* const SERVER_ADDRESS = "https://update2x.decent.ch";
   char const* const UPDATE_ROOT_DIR_ON_SERVER = "/";
#if defined(_MSC_VER)	
	char const* const UPDATE_INI_FILENAME = "update.ini";
	char const* const REV_HISTORY_FILENAME = "revhistory.txt";
#elif defined( __GNUC__ )
#if defined( __clang__ )
   char const* const UPDATE_INI_FILENAME = "update_mac.ini";
   char const* const REV_HISTORY_FILENAME = "revhistory_mac.txt";
#endif
#elif
#error "Undefined compiler platform"
#endif

DWORD const WAIT_FOR_FIRST_UPDATE_MINUTES = 1;

// use SSL or not ?
#define USE_SSL

#ifdef USE_SSL
#define SSL_SEC_CHANNEL SEC_CHANNEL_SSL_WITHOUT_CLIENT_CERT
#define USED_PORT 443
//#define VERIFY_SERVER_CERT
#else
#define SSL_SEC_CHANNEL SEC_CHANNEL_NO_SSL
#define USED_PORT 80
#endif


} //end namespace



