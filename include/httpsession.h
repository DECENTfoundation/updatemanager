#pragma once


namespace Update
{

class CHttpSession
{
public:

	CHttpSession(void);

	~CHttpSession(void);

   CURL *m_hSession;
};

} //end namespace