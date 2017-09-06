#include "stdafx.h"
#include "updatepathes.h"
#include "constants.h"



using namespace Update;


CUpdatePathes::CUpdatePathes(void)
{
	uint32_t numOfChars = 0;
   char updateDir[1024];
	std::string updateDirShort;
#if defined( _MSC_VER )
	char* tempDir = getenv("TMP");
	if(tempDir == NULL)
		tempDir = getenv("TEMP");

	if(tempDir == NULL)
		tempDir = getenv("SystemDrive");
#else
   char* tempDir = getenv("TMPDIR");
#endif
	updateDirShort = tempDir;
	
	memset(updateDir, 0, sizeof(updateDir));
#if defined( _MSC_VER )
   updateDirShort += "\\";
	numOfChars = GetLongPathName(updateDirShort.c_str(), (LPSTR)updateDir, sizeof(updateDir) - 1);
   m_updateDir = updateDir;
#else
   m_updateDir = updateDirShort;// Is this ok for directory with downloaded files ?
#endif
	
	m_updateIniFilePath += m_updateDir;
	m_updateIniFilePath += UPDATE_INI_FILENAME;
	m_updateSetupFilePath = m_updateDir;
	
}

CUpdatePathes::~CUpdatePathes(void)
{
}
