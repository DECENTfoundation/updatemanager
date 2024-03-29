#include "stdafx.h"
#include "iniparser.h"
#include "version.h"

#include <strstream>
#include <iostream>
#include <sstream>

using namespace Update;

CIniParser::CIniParser(FILE* file)
: m_file(file)
{
	if(m_file == (FILE*)(-1) || m_file == NULL)
		assert(0);
}

CIniParser::~CIniParser(void)
{
}

bool CIniParser::Parse(void)
{
	uint32_t err = 0;
   std::vector<char> buffer;// ini file buffer
	std::string line;

	try
	{
      buffer.resize(4096);
      

		fseek(m_file, 0, SEEK_END);
		buffer.resize(ftell(m_file) + 1);
      memset(&buffer[0], 0, buffer.size());

		fseek(m_file, 0, SEEK_SET);

		if(fread((void*)&buffer[0], 1, buffer.size() - 1, m_file) != (buffer.size() - 1))
			return false; //exception

		std::istringstream mystream;
		//mystream.str((char*)&buffer[0], (int)buffer.size(), std::ios::binary);
		mystream.str((char*)&buffer[0]);
		//std::string test = stream.str();
		//stream.clear();
		//stream.sync();
		//stream.ignore();
		//stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		//stream.seekg(0,std::ios::beg);

		std::getline(mystream, line, '\n');
		//line += '\0';
		std::strstream::pos_type p;
		while(1)	{

			if(line.size() >= 32)	{// aby tam bolo aspon jedno cislo verzie, inak je neplatny riadok
				if(line.at(line.size() - 1) == '\r')
					line.at(line.size() - 1) = 0;
				// spracuj
				try
				{	
					CIniLine curIniLine(line);//currently parsed line
				

					m_lines.insert(std::make_pair(curIniLine.m_updateVer->GetVersionNum64(), curIniLine));

					IniLineMap::const_iterator iter = m_lines.begin();
					while(iter != m_lines.end())
					{
						iter++;
					}
					
				} catch(std::exception e) {
					err = err;
				} catch(...)	{
					err = err;
				}
				//m_lines.push_back(curIniLine);
			}
			
			getline(mystream, line, '\n');
			//if(mystream.tellg() == (std::strstream::pos_type)(-1))
				//break;
			if(line.size() == 0)
				break;
		}

		IniLineMap::const_iterator iter = m_lines.begin();
		while(iter != m_lines.end())
		{
			iter++;
		}
	} catch(std::exception e)	{

		m_lines.clear();
		// alert

	} catch(...)	{
		m_lines.clear();
		//alert
	}

	return true;
}

