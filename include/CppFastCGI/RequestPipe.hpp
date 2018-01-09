#ifndef _CPPFASTCGI_REQPIPE_HPP
#define _CPPFASTCGI_REQPIPE_HPP

namespace CppFastCGI {

class RequestPipe: public CppSystemRT::Thread {
public:
	enum RequestPipeStatus {
		RPS_FINISHED	= 1,
		RPS_KEEPALIVE	= 2
	};

	RequestPipe(RequestHandler const& requestHandler, CppSystemRT::File* conn);
	virtual ~RequestPipe();

	void run();
	
	bool checkStatus(int stat);
	void send(Record& rec);
	
private:
	RequestHandler const& requestHandler;
	std::unique_ptr<CppSystemRT::File> conn;
	std::map<int,RequestInfo*> requests;
	int status;
};

}

#endif
