#pragma once

#include "iniline.h"


namespace Update
{


typedef std::map<uint64_t, CIniLine> IniLineMap;

class CIniParser
{
public:
	CIniParser(FILE* file);
	~CIniParser(void);

	FILE* m_file;

	IniLineMap m_lines;
	bool Parse(void);

};

} //end namespace