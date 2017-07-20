#include "stdafx.h"
#include "srvsettings.h"
#include "common.h"



namespace SrvSettings {

#define TEMPORARY_WIN_REG_PATH "Software\\DECENT" // temporary till we have not QSettings

const char* DETECT_UPDATE_COUNTER_VAL_NAME = "DetectUpdateCounter";
const char* LAST_UPDATE_FILETIME_VAL_NAME = "LastDetectedFTime";
const char* START_UPDATE_FILETIME_VAL_NAME = "StartOfUpdateFTime";
const char* UPDATE_PERIOD_HOURS_VAL_NAME = "UpdatePeriodHours";

	

const uint32_t UpdatePeriodDefaultIndex = 1;
const uint32_t UpdatepPeriodHighestIndex = 4;
const uint32_t UpdatePeriodsCount = 8;
const uint32_t UpdatePeriods[8] = {0,24,72,168,336,0,0,0};


uint32_t
Set_uint32_t_Value(
	uint32_t value,	
	const char* name
	)
{
#if	defined( _MSC_VER )
	DWORD err = 0;
	HKEY hKey = NULL;
	char serviceRegPath[MAX_PATH];

   lstrcpy(serviceRegPath, TEMPORARY_WIN_REG_PATH);

	err = RegOpenKeyEx(HKEY_CURRENT_USER, serviceRegPath, 0, KEY_ALL_ACCESS, &hKey);
	if(err == ERROR_FILE_NOT_FOUND)	{
		DWORD disp = 0;
		err = RegCreateKeyEx(HKEY_CURRENT_USER, serviceRegPath, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, &disp);
	}
	if(err == 0)	{
		err = RegSetValueEx(hKey, name, 0, REG_DWORD, (BYTE*)&value, sizeof(DWORD));
		RegCloseKey(hKey);
	}

	return err;
#elif	defined( __GNUC__ )
	return 1;//TODO:
#else
	#error "Undefined Compiler platform"
#endif
}

uint32_t
Get_uint32_t_ValueOrInit(
	uint32_t* value,	
	uint32_t initValue,
	const char* name
	)
{
#if	defined( _MSC_VER )
	DWORD err = 0;
	HKEY hKey = NULL;
	char serviceRegPath[MAX_PATH];
	DWORD* valBuff = NULL;
	DWORD defaultVal = 0;

   lstrcpy(serviceRegPath, TEMPORARY_WIN_REG_PATH);

	err = RegOpenKeyEx(HKEY_CURRENT_USER, serviceRegPath, 0, KEY_ALL_ACCESS, &hKey);
	if(err == ERROR_FILE_NOT_FOUND)	{
		DWORD disp = 0;
		err = RegCreateKeyEx(HKEY_CURRENT_USER, serviceRegPath, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, &disp);
	}
	if(err == 0)	{

		DWORD buffSizeLocal = sizeof(DWORD);
		DWORD type = 0;
		if(value)
			err = RegQueryValueEx(hKey, name, NULL, &type, (LPBYTE)value, &buffSizeLocal);
		if(err == ERROR_FILE_NOT_FOUND)	{
			err = RegSetValueEx(hKey, name, 0, REG_DWORD, (BYTE*)&initValue, sizeof(DWORD));
			if(value)
				*value = initValue;
		}
		RegCloseKey(hKey);
	}

	return err;
#elif	defined( __GNUC__ )
   *value = initValue;// initial value
	return 1;//TODO:
#else
	#error "Undefined Compiler platform"
#endif
}

uint32_t
Get_uint32_t_Value(
	uint32_t* buff,
	const char* name
	)
{
#if	defined( _MSC_VER )
	DWORD err = 0;
	HKEY hKey = NULL;
	TCHAR serviceRegPath[MAX_PATH];
	DWORD* valBuff = NULL;
	DWORD defaultVal = 0;

   lstrcpy(serviceRegPath, TEMPORARY_WIN_REG_PATH);

	err = RegOpenKeyEx(HKEY_CURRENT_USER, serviceRegPath, 0, KEY_READ | KEY_QUERY_VALUE, &hKey);
	if(err == ERROR_FILE_NOT_FOUND)	{
		DWORD disp = 0;
		err = RegCreateKeyEx(HKEY_CURRENT_USER, serviceRegPath, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, &disp);
	}
	if(err == 0)	{

		DWORD buffSizeLocal = sizeof(DWORD);
		DWORD type = 0;
		err = RegQueryValueEx(hKey, name, NULL, &type, (LPBYTE)buff, &buffSizeLocal);
		RegCloseKey(hKey);
	}

	return err;
#elif	defined( __GNUC__ )
	return 1;//TODO:
#else
	#error "Undefined Compiler platform"
#endif
}

uint32_t
Get_uint64_t_Value(
   uint64_t* buff,
   const char* name
)
{
#if defined( _MSC_VER )
   DWORD err = 0;
   HKEY hKey = NULL;
   TCHAR serviceRegPath[MAX_PATH];
   DWORD defaultVal = 0;

   lstrcpy(serviceRegPath, TEMPORARY_WIN_REG_PATH);

   err = RegOpenKeyEx(HKEY_CURRENT_USER, serviceRegPath, 0,
      KEY_READ | KEY_QUERY_VALUE, &hKey);
   if (err == ERROR_FILE_NOT_FOUND) {
      DWORD disp = 0;
      err = RegCreateKeyEx(HKEY_CURRENT_USER, serviceRegPath, 0, NULL, 0,
         KEY_ALL_ACCESS, NULL, &hKey, &disp);
   }
   if (err == 0) {

      DWORD buffSizeLocal = sizeof(uint64_t);
      DWORD type = 0;
      err = RegQueryValueEx(hKey, name, NULL, &type, (LPBYTE)buff, &buffSizeLocal);
      RegCloseKey(hKey);
   }

   return err;
#elif defined( __GNUC__ )
   return 1;//TODO:
#else
#error "Undefined Compiler platform"
#endif
}

uint32_t
Set_uint64_t_Value(
   uint64_t value,
   const char* name
)
{
#if defined( _MSC_VER )
   DWORD err = 0;
   HKEY hKey = NULL;

   char serviceRegPath[MAX_PATH];
   lstrcpy(serviceRegPath, TEMPORARY_WIN_REG_PATH);

   err = RegOpenKeyEx(HKEY_CURRENT_USER, serviceRegPath, 0, KEY_ALL_ACCESS, &hKey);
   if (err == ERROR_FILE_NOT_FOUND) {
      DWORD disp = 0;
      err = RegCreateKeyEx(HKEY_CURRENT_USER, serviceRegPath, 0, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, &disp);
   }
   if (err == 0) {
      err = RegSetValueEx(hKey, name, 0, REG_QWORD, (LPBYTE)&value, sizeof(uint64_t));
   }
   return err;
#elif defined( __GNUC__ )
   return 1;//TODO
#else
#error "Undefined compiler platform"
#endif
}
uint32_t DelValue(const char* name)
{

#if	defined( _MSC_VER )
	DWORD err = 0;
	char serviceRegPath[MAX_PATH];
	HKEY hkey = NULL;

   lstrcpy(serviceRegPath, TEMPORARY_WIN_REG_PATH);

	err = RegOpenKey(HKEY_CURRENT_USER, serviceRegPath, &hkey);
	if(err)
		return err;

	if(hkey == NULL)
		return ERROR_INVALID_HANDLE_STATE;

	err = RegDeleteValue(hkey, name);
	RegCloseKey(hkey);
	return err;
#elif	defined( __GNUC__ )
	return 1;//TODO:
#else
	#error "Undefined Compiler platform"
#endif
}



}

