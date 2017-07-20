#pragma once



namespace Update
{

enum RequestMethod : unsigned int {
	ReqMethod_GET = 0,
	ReqMethod_HEAD,
	ReqMethod_POST,
};
class CHttpResponse;
enum Http_SecChannel : unsigned int;
class CHttpRequest
{
public:
	
	CHttpRequest(
      CURL* session,
      const char* address,
      const char* targetObj,
      Http_SecChannel secChannel
		);
	~CHttpRequest(void);

   CURL* m_curlSession;

	CHttpResponse* m_response;
	RequestMethod m_method;

	uint32_t Send(FILE* stream, bool createProgBar, const std::string& progBarTitle, uint32_t maxAcceptableSize, uint32_t* abort);

};

} //end namespace