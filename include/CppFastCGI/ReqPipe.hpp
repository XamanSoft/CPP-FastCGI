#ifndef _CPPFASTCGI_REQPIPE_HPP
#define _CPPFASTCGI_REQPIPE_HPP

namespace CppFastCGI {

class ReqPipe: public CppSystemRT::Thread {
public:
	enum ReqPipeStatus {
		RPS_FINISHED	= 1,
		RPS_KEEPALIVE	= 2
	};

	ReqPipe(CppSystemRT::File* conn);
	virtual ~ReqPipe();

	void run();
	
	bool checkStatus(int stat);
	void send(Record& rec);
	
	
private:
	std::unique_ptr<CppSystemRT::File> conn;
	std::map<int,Request*> requests;
	bool keepAlive;
	std::mutex socketMutex;
	int status;
};

}

#endif
