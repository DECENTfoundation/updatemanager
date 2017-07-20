#pragma once 


typedef void* MPHANDLE;// multiplatform thread "handle"
typedef uint32_t(__stdcall *MP_THREAD_FUNCTION)(void* params);


class CDetectUpdateThreadParams
{
public:

	CDetectUpdateThreadParams()
	: m_invokedByUser(FALSE)
	, m_abort(0)
	, m_licenseUserID(0ULL) 
	, m_iniDownloadFinished(false)
   , m_curlSession(nullptr)
   , m_runUpdateFlag(false)
   , m_stopUpdateThreadFlag(false)
	{
	};

	bool m_invokedByUser;			// invoked by user (for example click on update button)
   bool m_runUpdateFlag;         // set to true to run update 
   bool m_stopUpdateThreadFlag;  // set to true to stop update
	uint32_t m_abort;					// set to 1 for aborting update
   void* m_curlSession;
	uint64_t m_licenseUserID;     
	bool m_iniDownloadFinished;	// it is set to true after update.ini is downloaded
};

uint32_t __stdcall DetectUpdateThread(void* param);
void AbortUpdate(uint32_t* abort, void* session);


