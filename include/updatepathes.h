#pragma once

#include <string>

namespace Update
{

class CUpdatePathes
{
public:
	CUpdatePathes(void);
	~CUpdatePathes(void);

	std::string m_updateDir;// directory for downloaded files
	std::string m_updateIniFilePath;
	std::string m_updateSetupFilePath;

};

} //end namespace
