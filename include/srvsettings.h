#pragma once 



namespace SrvSettings {



uint32_t DelValue(const char* name);

uint32_t
Get_uint32_t_Value(
	uint32_t* buff,	
	const char* name
	);
uint32_t
Get_uint32_t_ValueOrInit(
	uint32_t* value,	
	uint32_t initValue,
	const char* name
	);

uint32_t 
Set_uint32_t_Value(
	uint32_t value,	
	const char* name
	);

uint32_t
Get_uint64_t_Value(
   uint64_t* valBuff,
   const char* name
   );

uint32_t
Set_uint64_t_Value(
   uint64_t value,
   const char* name
   );

extern const char* DETECT_UPDATE_COUNTER_VAL_NAME;// Detect update counter
extern const char* LAST_UPDATE_FILETIME_VAL_NAME;// Last Detect time
extern const char* START_UPDATE_FILETIME_VAL_NAME;// Start of update
extern const char* UPDATE_PERIOD_HOURS_VAL_NAME;// How often to update

extern const uint32_t UpdatePeriodDefaultIndex;
extern const uint32_t UpdatepPeriodHighestIndex;
extern const uint32_t UpdatePeriodsCount;
extern const uint32_t UpdatePeriods[8];


} // end namespace SrvSettings

