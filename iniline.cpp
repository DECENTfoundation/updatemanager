#include "stdafx.h"
#include "iniline.h"
#include "version.h"

using namespace Update;

CIniLine::CIniLine(void)
: m_lineStr("")
, m_updateVer(nullptr)
, m_acumulFrom(nullptr)
, m_acumulTo(nullptr)
{
}

CIniLine::CIniLine(std::string line)
: m_lineStr(line)
, m_updateVer(nullptr)
, m_acumulFrom(nullptr)
, m_acumulTo(nullptr)
{
	Init((char*)m_lineStr.data());
}
bool CIniLine::Init(char* line)
{
	char* curStr = nullptr;
	char seps[] = ";";
	m_lineStr = line;

	try
	{
		// update version
		curStr = strtok((char*)m_lineStr.data(), seps);
		if(curStr == nullptr)	{
         // alert
			return false; //exception
		}

		m_updateVer = new CVersion(curStr);
		if(m_updateVer == nullptr)	{
         // alert
			return false;// exception
		}
		
		// acumulate from
		curStr = strtok(nullptr, seps);
		if(curStr == nullptr)	{
         // alert
			return false; //exception
		}

		m_acumulFrom = new CVersion(curStr);
		if(m_acumulFrom == nullptr)	{
         // alert
			return false;// exception
		}
		
		// acumulate to
		curStr = strtok(nullptr, seps);
		if(curStr == nullptr)	{
         // alert
			return false; //exception
		}

		m_acumulTo = new CVersion(curStr);
		if(m_acumulTo == nullptr)	{
         // alert
			return false;// exception
		}
	
		// Check Dll filename on server
		curStr = strtok(nullptr, seps);
      if (curStr == nullptr) {
         return false;
      }
		// reserved1
		curStr = strtok(nullptr, seps);
		if(curStr == nullptr)	{
         // alert
			return false;
		}
		// reserved2
		curStr = strtok(nullptr, seps);
		if(curStr == nullptr)	{
         // alert
			return false;
		}
		// reserved3
		curStr = strtok(nullptr, seps);
		if(curStr == nullptr)	{
         // alert
			return false;
		}
		// reserved4
		curStr = strtok(nullptr, seps);
		if(curStr == nullptr)	{
         // alert
			return false;
		}
		// reserved5
		curStr = strtok(nullptr, seps);
		if(curStr == nullptr)	{
         // alert
			return false;
		}
		// reserved6
		curStr = strtok(nullptr, seps);
		if(curStr == nullptr)	{
         // alert
			return false;
		}
		// Setup filename on server
		curStr = strtok(nullptr, seps);
		if(curStr == nullptr)	{
         // alert
			return false;
		}
		
		m_serverSetupFileName = curStr;
		
		// First part of command line
		curStr = strtok(nullptr, seps);
		if(curStr == nullptr)	{
         // alert
			return false;
		}
		
		m_msiCmdLine1 = curStr;
		// Second part of command line
		curStr = &curStr[strlen(curStr)] + 1;
		//curStr = strtok(NULL, seps);
		if(curStr == nullptr)	{
         // alert
			return false;
		}

		m_msiCmdLine2 = curStr;
	
		if(m_updateVer->Parse() == false)	{
         // alert
			return false;
		}
		if(m_acumulFrom->Parse() == false)	{
         // alert
			return false;
		}
		if(m_acumulTo->Parse() == false)	{
         // alert
			return false;
		}
	} catch( std::exception e)	{
		if(m_updateVer)
			delete m_updateVer;
		m_updateVer = nullptr;
		if(m_acumulFrom)
			delete m_acumulFrom;
		m_acumulFrom = nullptr;
		if(m_acumulTo)
			delete m_acumulTo;
		m_acumulTo = nullptr;
		// alert
	} catch(...) {
		if(m_updateVer)
			delete m_updateVer;
		m_updateVer = nullptr;
		if(m_acumulFrom)
			delete m_acumulFrom;
		m_acumulFrom = nullptr;
		if(m_acumulTo)
			delete m_acumulTo;
		m_acumulTo = nullptr;
      // alert
	}

	return true;
}
CIniLine::CIniLine(const CIniLine& iniLine)
{
	m_lineStr = iniLine.m_lineStr;
	m_updateVer = nullptr;
	m_acumulFrom = nullptr;
	m_acumulTo = nullptr;

	if(iniLine.m_updateVer)	{
		m_updateVer = new CVersion(iniLine.m_updateVer->GetVersionStr());
		m_updateVer->Parse();
	}
	if(iniLine.m_acumulFrom)	{
		m_acumulFrom = new CVersion(iniLine.m_acumulFrom->GetVersionStr());
		m_acumulFrom->Parse();
	}
	if(iniLine.m_acumulTo)	{
		m_acumulTo = new CVersion(iniLine.m_acumulTo->GetVersionStr());
		m_acumulTo->Parse();
	}
	if(iniLine.m_msiCmdLine1.size())	{
		m_msiCmdLine1 = iniLine.m_msiCmdLine1;
	}
	if(iniLine.m_msiCmdLine2.size())	{
		m_msiCmdLine2 = iniLine.m_msiCmdLine2;
	}
	if(iniLine.m_serverSetupFileName.size())	{
		m_serverSetupFileName = iniLine.m_serverSetupFileName;
	}
}

CIniLine::~CIniLine(void)
{
	if(m_updateVer)
		delete m_updateVer;
	m_updateVer = nullptr;

	if(m_acumulFrom)
		delete m_acumulFrom;
	m_acumulFrom = nullptr;
	
	if(m_acumulTo)
		delete m_acumulTo;
	m_acumulTo = nullptr;

}
