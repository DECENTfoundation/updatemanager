#include "stdafx.h"
#include "version.h"

using namespace Update;

CVersion::CVersion(std::string verStr)
: m_ver0(-1)
, m_ver1(-1)
, m_ver2(-1)
, m_ver3(-1)
, m_version(0LL)
{
	memset(m_versionStr, 0, sizeof(m_versionStr));
	strncpy(m_versionStr, verStr.c_str(), sizeof(m_versionStr) - 1);
}
CVersion::CVersion(void)
: m_ver0(-1)
, m_ver1(-1)
, m_ver2(-1)
, m_ver3(-1)
, m_version(0LL)
{
	memset(m_versionStr, 0, sizeof(m_versionStr));
}
CVersion::CVersion(const CVersion& version)
{
	m_ver0 = version.m_ver0;
	m_ver1 = version.m_ver1;
	m_ver2 = version.m_ver2;
	m_ver3 = version.m_ver3;
	strcpy(m_versionStr, version.m_versionStr);
	m_version = version.m_version;
}
CVersion::~CVersion(void)
{
}
bool CVersion::Parse(void)
{
	bool result = false;

	result = ParseInternal();
	if(result == false)
		return false;

	m_version = 0ULL;
	m_version = m_ver0;
	m_version = m_version << 16;
	m_version = m_version + m_ver1;
	m_version = m_version << 16;
	m_version = m_version + m_ver2;
	m_version = m_version << 16;
	if(m_ver3 != 0xffff)	{
		m_version = m_version + m_ver3;
	}

	return result;
}
bool CVersion::ParseInternal(void)
{
	std::string::size_type strIndex = 0;
	std::string::size_type endIndex = 0;
	char versionStr[64];
	memset(versionStr, 0, sizeof(versionStr));
	strcpy(versionStr, m_versionStr);
	
	char seps[] = ".";
	char* versionDigit = nullptr;
	char* tmp = nullptr;

	if(strlen(versionStr) < 5)
		return false ;// exception

	// ver0
	versionDigit = strtok(versionStr, seps);
	if(versionDigit == nullptr)
		return false;// exception

	m_ver0 = atoi(versionDigit);
	//if(m_ver0 == 0)
	//	return FALSE;//exception

	// ver1
	//versionDigit = strtok(NULL, seps);
	versionDigit = strtok(NULL, seps);

	if(versionDigit == nullptr)
		return false;// exception

	m_ver1 = atoi(versionDigit);

	// ver2
	versionDigit = strtok(NULL, seps);

	if(versionDigit == NULL) // na konci nie je bodka
		return false;// posledny digit ma nulovu velkost , exception
	
	m_ver2 = atoi(versionDigit);
	
	// ver3
	versionDigit = strtok(NULL, seps);

	if(versionDigit == NULL) { // na konci nie je bodka
		m_ver3 = 0;
		return true;// normal - nemusi byt stvrty digit, ale ak tam nieje, tak sa nastavi nula
	}
	
	m_ver3 = atoi(versionDigit);

	return true;

}

const char* CVersion::GetVersionStr(void) const
{
   return m_versionStr;
}

uint64_t CVersion::GetVersionNum64(void) const
{
   return m_version;
}
