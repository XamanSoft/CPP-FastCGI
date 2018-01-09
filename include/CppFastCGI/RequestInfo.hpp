#ifndef _CPPFASTCGI_REQUESTINFO_HPP
#define _CPPFASTCGI_REQUESTINFO_HPP

namespace CppFastCGI {
	
class Request;
class RequestPipe;
	
struct RequestInfo {
	int id;
	RequestPipe& parent;
	Request* request;
	std::stringstream inStream;
	std::map<std::string, std::string> params;
};
	
}

#endif
