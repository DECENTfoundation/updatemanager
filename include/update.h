#pragma once

#include <string>



namespace Update
{
extern "C"
{

#if defined( _MSC_VER )
#define DECENT_PRODUCT_VERSION_MAJOR 1
#define DECENT_PRODUCT_VERSION_MINOR 4
#define DECENT_PRODUCT_VERSION_BUILD 2
#define DECENT_PRODUCT_VERSION_SPECIAL 0

#define DECENTSETUP_MSI_PRODUCT_CODE "{11111111-1111-1111-1111-111111111111}"

#elif defined( __GNUC__ )
#if defined( __clang__ )
//#error "Define version numbers for your platform"
   // define version of current build, for example:
#define DECENT_PRODUCT_VERSION_MAJOR 1
#define DECENT_PRODUCT_VERSION_MINOR 4
#define DECENT_PRODUCT_VERSION_BUILD 2
#define DECENT_PRODUCT_VERSION_SPECIAL 0

#endif
#else
#error "Define version numbers for your platform"
#endif


typedef uint32_t		// 1 - install it, 0 - abort installation
	(__cdecl *fn_StartRevHistoryDlg)(		
	const std::string& revHistory	// revision history from text file
	);

typedef void 
	(__cdecl *fn_CreateProgBar)(
	int upperBorder, 
	uint32_t* abort
	);

typedef void 
	(__cdecl *fn_SetProgBarPos)(
	int pos
	);

typedef void 
	(__cdecl *fn_SetProgBarTitle)(
	const char* title
	);

typedef void 
	(__cdecl *fn_DestroyProgBar)(
	void
	);

extern fn_StartRevHistoryDlg g_fn_StartRevHistoryDlg;
extern fn_CreateProgBar g_fn_CreateProgBar;
extern fn_SetProgBarPos g_fn_SetProgBarPos;
extern fn_SetProgBarTitle g_fn_SetProgBarTitle;
extern fn_DestroyProgBar g_fn_DestroyProgBar;

bool TryUpdate(bool invokedByUser, uint32_t updateCounter, uint32_t* abort, bool hideCertQuestions, uint64_t licenseUserID, bool* iniDownloadFinished);
bool Update(bool invokedByUser, uint32_t updateCounter, uint32_t* abort, bool hideCertQuestions, uint64_t licenseUserID, bool* iniDownloadFinished);

void ReadInstalledVersion(std::string& prodVerMb);

void MakeRestOfURL(std::string& targetObj, int updateCounter, uint64_t licenseUserID);


} // extern "C"
} //end namespace



