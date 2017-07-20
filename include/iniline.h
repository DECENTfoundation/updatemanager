#pragma once


namespace Update
{

class CVersion;
class CIniLine
{
public:
	CIniLine(void);
	CIniLine(std::string line);
	CIniLine(const CIniLine& iniLine);
	~CIniLine(void);

	bool Init(char* line);

	std::string m_lineStr;

	CVersion* m_updateVer;
	CVersion* m_acumulFrom;
	CVersion* m_acumulTo;

	std::string m_reserved1;
	std::string m_reserved2;
	std::string m_reserved3;
	std::string m_serverSetupFileName;
	std::string m_msiCmdLine1;
	std::string m_msiCmdLine2;

};

} //end namespace