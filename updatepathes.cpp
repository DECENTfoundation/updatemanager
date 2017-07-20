#include "stdafx.h"
#include "updatepathes.h"
#include "constants.h"



using namespace Update;


CUpdatePathes::CUpdatePathes(void)
{
	DWORD numOfChars = 0;
   char updateDir[1024];
	std::string updateDirShort;
	char* tempDir = getenv("TMP");
	if(tempDir == NULL)
		tempDir = getenv("TEMP");

	if(tempDir == NULL)
		tempDir = getenv("SystemDrive");

	updateDirShort = tempDir;
	
	memset(updateDir, 0, sizeof(updateDir));
#if defined( _MSC_VER )
   updateDirShort += "\\";
	numOfChars = GetLongPathName(updateDirShort.c_str(), (LPSTR)updateDir, sizeof(updateDir) - 1);
   m_updateDir = updateDir;
#elif
   m_updateDir = updateDirShort;// Is this ok for directory with downloaded files ?
#error "Is this ok ?"
#endif
	
	m_updateIniFilePath += m_updateDir;
	m_updateIniFilePath += UPDATE_INI_FILENAME;
	m_updateSetupFilePath = m_updateDir;
	
}

CUpdatePathes::~CUpdatePathes(void)
{
}
