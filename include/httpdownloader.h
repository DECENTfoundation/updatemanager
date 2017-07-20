#pragma once



namespace Update
{
//enum Http_SecChannel;
class CHttpDownloader
{
public:
	CHttpDownloader(
		const char* address,				// base of URL 
		const char* objName,				// rest of url
									         // example: full url: update.decent.ch/update.ini
									         // address is:	"update.decent.ch"
									         // objName is:	"/update.ini"
		const char* targetPath,			// directory path to downloaded file, ended with slash/backslah
		const char* targetFileName,	// filename of downloaded file, if you donw want to store result in file pass NULL

		enum Http_SecChannel secChannel,	// See UpdateMgr_SecChannel enumeration
											   // for possible values
		uint16_t port,
		uint32_t maxDownloadedSize,	// if in downloading is exceeded, download is aborted
		bool useTemporaryFile,
		bool useProgressBar
		);

	~CHttpDownloader(void);
	
	bool Download(uint32_t& httpStatus, bool& maxSizeExceeded, uint32_t& err, uint32_t* abort, CURL** curlSession, bool hideCertQuestions, char* progBarTitle);
	bool Unix2Dos(void);

	void GetDownloadedFilePath(std::string& filePath);
	void CloseDownloadedFile(void);
	
private:

	std::string m_objName;
	std::string m_address;
	std::string m_addresAndObject;
	std::string m_targDir;
	std::string m_targFileName;
	
	Http_SecChannel  m_secChannel;
	uint16_t m_port;
	uint32_t m_maxDownloadedSize;
	bool m_useTemporary;
	bool m_useProgressBar;
public:
	FILE* m_stream;
};

} //end namespace