#pragma once

namespace Update 
{

class CVersion
{
public:
	CVersion(std::string verStr);
	CVersion(void);
	CVersion(const CVersion& version);
	~CVersion(void);

	bool Parse(void);
	bool ParseInternal(void);
   const char* GetVersionStr(void) const;
   uint64_t GetVersionNum64(void) const;

	std::string::iterator m_strTokIter;
	std::string m_searchStr;// retazec ktory sa prehladava

	//std::string m_versionStr;
private:
	char m_versionStr[64];

	uint16_t m_ver0;// navyssie cislo verzie
   uint16_t m_ver1;
   uint16_t m_ver2;
   uint16_t m_ver3;// najnizsie, stale 0

	uint64_t m_version;
};

} //end namespace