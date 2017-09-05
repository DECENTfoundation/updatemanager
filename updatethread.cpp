#include "stdafx.h"

#include "srvsettings.h"
#include "updatethread.h"
#include "update.h"
#include "constants.h"



using namespace Update;


bool 
TryUpdateIfNeeded(
	uint32_t updateCounterLoops,	// counter of parent cycles
   uint32_t period,					// period of parent cycle (calculating of minute of first update)
   uint32_t random,					// for not starting at the same minute
	bool invokedByUser,			   // user started update
   CURL** curlSession,           // session handle
	uint32_t* abort,						// update was aborted - it is filled in update thread function								
	uint64_t licenseUserID,	      // license or user identification
	bool* iniDownloadFinished			// when *.ini file is downloaded it is set to true
	);

void MpSleep(uint32_t milisec)
{
#if		defined( _MSC_VER )
   Sleep(milisec);
#elif	defined( __GNUC__ )
#if defined( __clang__ )
   struct timespec req;
   struct timespec rem;
   req.tv_sec = milisec / 1000;
   req.tv_nsec = (milisec % 1000) * 1000000;
   int ret = 0;
   while (1) {
      ret = nanosleep(&req, &rem);
      if (ret == 0)
         break;
      if (errno == EINTR) {
         memcpy(&req, &rem, sizeof(rem));
      }
      else
         break;
   }
#elif
#error "Implement Sleep(miliseconds) code"
#endif
#else
#error "Undefined Compiler platform"
#endif
}

// MpGetCurrentSystemTime()
// It returns 64-bit value representing the number of 100-nanosecond intervals since January 1, 1601 (UTC).
const int64_t FILETIME_1970 = 0x019db1ded53e8000LL;
void MpGetCurrentSystemTime(int64_t* fileTime)
{
#if defined( _MSC_VER )
   SYSTEMTIME st;
   GetSystemTime(&st);
   SystemTimeToFileTime(&st, (LPFILETIME)fileTime);
#elif defined( __GNUC__ )
   struct timeb the_time;

   ftime(&the_time);
   *fileTime = (int64_t)the_time.time * (int64_t)1000; // sec -> milisec
   *fileTime += the_time.millitm;     // +milisec
   *fileTime = (*fileTime) * (int64_t)10000;  // 100 nanosecond intervals
   *fileTime += FILETIME_1970;

#else
#error "Undefined Compiler platform"
#endif
}




uint32_t __stdcall DetectUpdateThread(void* param)
{
	CDetectUpdateThreadParams* updateParams = (CDetectUpdateThreadParams*)param;
   uint64_t lastUpdateTimeFt = (uint64_t)(-1);
	uint32_t err = 0;	

   curl_global_init(CURL_GLOBAL_ALL);
   
   SrvSettings::Get_uint64_t_Value(&lastUpdateTimeFt, SrvSettings::LAST_UPDATE_FILETIME_VAL_NAME);

	uint32_t updateCounterLoops = 0;
   uint32_t updateCounter = 0;
	
	// unknown time of last update
	if(lastUpdateTimeFt == (uint64_t)(-1))	{
	   // reset counter
		SrvSettings::Set_uint32_t_Value(0, SrvSettings::DETECT_UPDATE_COUNTER_VAL_NAME);
	} else {
	}

   uint32_t waitRes = 0;
	//HANDLE hArray[8];
   uint32_t random = 0;
   uint32_t period = 60000;// miliseconds - how often to repeat following cycle
   const uint32_t SLEEP_MILISEC = 200;
   const uint32_t COUNTER_FOR_TRYING_MAX = 60000 / SLEEP_MILISEC;// 1 minute
   uint32_t counterForTrying = 0;

	srand( GetTickCount() );
	random =  rand() / (RAND_MAX / 3);// 0 - 4

	//hArray[0] = updateParams->m_stopUpdateThread;
	//hArray[1] = updateParams->m_runUpdateEvent;

	while(1)	{
		//waitRes = WaitForMultipleObjects(2, hArray, FALSE, period);
      MpSleep(SLEEP_MILISEC);
      counterForTrying++;
		//if(waitRes == 0)	// end
      if(updateParams->m_stopUpdateThreadFlag)
		{
			break;
		} else
		//if(waitRes == WAIT_TIMEOUT)	{
      if(counterForTrying > COUNTER_FOR_TRYING_MAX) {// One per minute checks if it is scheduled update request
         
			if(TryUpdateIfNeeded(
				updateCounterLoops,
				period,
				random,
				false,
            &updateParams->m_curlSession,
				&updateParams->m_abort,
				updateParams->m_licenseUserID,
				&updateParams->m_iniDownloadFinished
				) == true)	
			{
				updateCounterLoops = 0;
			} else {
				updateCounterLoops++;
			}
         counterForTrying = 0;
		} else
		//if(waitRes == 1) {// invoked by user
      if(updateParams->m_runUpdateFlag) {
         
			updateParams->m_iniDownloadFinished = false;
			if(TryUpdateIfNeeded(
				updateCounterLoops,
				period,
				random,
				true,
            &updateParams->m_curlSession,
				&updateParams->m_abort,
				updateParams->m_licenseUserID,
				&updateParams->m_iniDownloadFinished
				) == true)	
			{
				updateCounterLoops = 0;
			} else {
				updateCounterLoops++;
			}
         updateParams->m_runUpdateFlag = false;
		} /*else 
		if(waitRes == WAIT_FAILED) {
			err = GetLastError();
			break;
		}*/
	}

	return 0;
}
void AbortUpdate(uint32_t* abort, CURL* hRequest);

bool 
TryUpdateIfNeeded(
	uint32_t updateCounterLoops,
   uint32_t period,
   uint32_t  random,
	bool invokedByUser,
   CURL** curlSession,
	uint32_t* abort,
	uint64_t licenseUserID,
	bool* iniDownloadFinished
	)
{
	uint64_t lastUpdateFt = 0ULL;// kept in runtime, to avoid loading from registry/file
	int64_t currentFt = 0ULL;
	int64_t lastStartOfUpdateFt = 0ULL;
	uint32_t updatePeriodHours = 0;
	uint32_t updateCounter = 0;
	bool hideCertQuestions = false;
	uint64_t updatePeriod100Nanos = 0;
	uint32_t updateTimeoutSeconds = 600;// timeout, when expires then it is checked if previous update is running, if yes then previous update will be aborted

	srand(GetTickCount());

   SrvSettings::Get_uint64_t_Value(&lastUpdateFt, SrvSettings::LAST_UPDATE_FILETIME_VAL_NAME);

   MpGetCurrentSystemTime(&currentFt);

	if( lastUpdateFt > currentFt )	{	// if changed time, let last update is equal to current time
		lastUpdateFt = currentFt;
      
      SrvSettings::Set_uint64_t_Value((uint64_t)lastUpdateFt, SrvSettings::LAST_UPDATE_FILETIME_VAL_NAME);
	}
	SrvSettings::Get_uint32_t_ValueOrInit(&updateCounter, 0, SrvSettings::DETECT_UPDATE_COUNTER_VAL_NAME);	
	SrvSettings::Get_uint32_t_ValueOrInit(&updatePeriodHours, SrvSettings::UpdatePeriods[SrvSettings::UpdatePeriodDefaultIndex], SrvSettings::UPDATE_PERIOD_HOURS_VAL_NAME);
	// premena hours na nanoseconds:
	updatePeriod100Nanos = (uint64_t)updatePeriodHours * 60 * 60 * 10000000;
		
   if (SrvSettings::Get_uint64_t_Value((uint64_t*)&lastStartOfUpdateFt, SrvSettings::START_UPDATE_FILETIME_VAL_NAME) != 0) {
      // Setting "Start of update time" was not found 
		//uint64_t currentFtMinuslastUpdateFt = *(uint64_t*)&currentFt - *(uint64_t*)&lastUpdateFt;// debug purposes - to see value
		
		// update if:
		if( (invokedByUser) )	
		{
			// ok
		} else 	
		if(	updateCounter == 0 &&																				
			(Update::WAIT_FOR_FIRST_UPDATE_MINUTES + random) <= ( (updateCounterLoops * (period / 1000))/60 )	
			)	
		{
			// ok
		} else	
		if( (updateCounter != 0) &&																	
			( (currentFt - lastUpdateFt) >  updatePeriod100Nanos) &&	
			(updatePeriodHours != 0) )																
		{
			// ok
		} else {
			return false;// update not needed
		}
		
      SrvSettings::Set_uint64_t_Value(currentFt, SrvSettings::START_UPDATE_FILETIME_VAL_NAME);
		hideCertQuestions = true;
       
		if(TryUpdate(invokedByUser, updateCounter, abort, hideCertQuestions, licenseUserID, iniDownloadFinished))	{
			updateCounter++;
			SrvSettings::DelValue(SrvSettings::START_UPDATE_FILETIME_VAL_NAME);// disable lock
			return true;
		}
		SrvSettings::DelValue(SrvSettings::START_UPDATE_FILETIME_VAL_NAME);// disable lock
		return false;
		
	// Settings "Start of update time" was found
	} else {
		// check if last update is not taking too much time

		if( (updatePeriodHours != 0) && (lastStartOfUpdateFt != 0ULL) )
		{
			if(lastStartOfUpdateFt > currentFt)	// nonsense -but it can exist if user changes system time during updating 
			{
				// update it
			} else 
			if( (currentFt - lastStartOfUpdateFt) > (updateTimeoutSeconds * 1000 * 1000 * 10) )  // ak je od posledneho startu nepodareneho update viac ako timeout, tak updatuj 
			{
				// update it
			} else {
				SrvSettings::DelValue(SrvSettings::START_UPDATE_FILETIME_VAL_NAME);// disable lock
				return false;
			}

		} else {

			if( invokedByUser ) 
			{
				// update it
			} else {

				SrvSettings::DelValue(SrvSettings::START_UPDATE_FILETIME_VAL_NAME);// remove lock
				return false;
			}
		}
		// abort !
 		AbortUpdate(abort, *curlSession);// abort previous
		
      SrvSettings::Set_uint64_t_Value(currentFt, SrvSettings::START_UPDATE_FILETIME_VAL_NAME);
		hideCertQuestions = true;
		if(TryUpdate(invokedByUser, updateCounter, abort, hideCertQuestions, licenseUserID, iniDownloadFinished) == true)	{
			updateCounter++;
			SrvSettings::DelValue(SrvSettings::START_UPDATE_FILETIME_VAL_NAME);// remove lock
			return true;
		}
		SrvSettings::DelValue(SrvSettings::START_UPDATE_FILETIME_VAL_NAME);// remove lock
		return false;
	}
}

void AbortUpdate(uint32_t* abort, CURL* session)
{
	if(abort)
		*abort = 1;
	if(session)	{
      curl_easy_cleanup(session);
	}
	SrvSettings::DelValue(SrvSettings::START_UPDATE_FILETIME_VAL_NAME);
	MpSleep(200);
	if(abort)
		*abort = 0;

}
