#pragma once



#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <map>

#ifdef _MSC_VER
#include <WinSock2.h> 
#include <WS2tcpip.h>
#include <windows.h>
//#include <ShlObj.h>
#endif

#include <stdint.h>
#include <assert.h>


#include "curl/curl.h"
#include "curl/easy.h"
