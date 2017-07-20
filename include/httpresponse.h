#pragma once



namespace Update
{

struct SProgressControl;
class CHttpRequest;
//enum ProgBarState : unsigned int;

class CHttpResponse
{
public:
	CHttpResponse(CURL* curlSession, CHttpRequest* request, FILE* stream, bool useProgBar, const std::string& progBarTitle, uint32_t maxAcceptableSize, uint32_t* abort);
	~CHttpResponse(void);

   CURL* m_curlSession;
	
   static size_t CHttpResponse::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
   static size_t CHttpResponse::Header_callback(char *buffer, size_t size, size_t nitems, void *userdata);

	void 
	GetStatus(
		uint32_t& httpStatus,						// HTTP response status code
		bool& maxSizeExceeded,
		uint32_t* recvBytes,
		uint32_t* originalLenHeaderField,
		uint32_t* abortedByUser
		);
	
private:
   uint32_t m_httpStatus;
   uint32_t m_contentLength;
   uint32_t m_receivedBytes;
   uint32_t m_maxAcceptableSize;
   bool m_useProgBar;
   bool m_maxSizeExceeded;
   bool m_abortedByUser;
   std::string m_progBarTitle;
   FILE* m_stream;
   uint32_t* m_abort;
};

} //end namespace