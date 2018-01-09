#ifndef _CPPFASTCGI_REQUEST_HPP
#define _CPPFASTCGI_REQUEST_HPP

namespace CppFastCGI { // CppFastCGI Begin

class RequestPipe;

class Request: public CppSystemRT::Thread {
public:
	enum ReqStatus {
		RS_NONE		= 0,
		RS_STDERR	= 1,
		RS_STDOUT	= 2,
		RS_READY	= 4,
		RS_END		= 8
	};

	Request(RequestInfo* rInfo);
	virtual ~Request();

	bool checkStatus(int stat);

protected:
	virtual void run();
	void end(int code);
	void error(std::string const& str);
	void print(std::string const& str);

	RequestInfo* requestInfo;
	int status;
};

} // CppFastCGI End

#endif
