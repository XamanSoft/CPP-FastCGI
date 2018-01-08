#ifndef _CPPFASTCGI_REQPIPE_HPP
#define _CPPFASTCGI_REQPIPE_HPP

namespace CppFastCGI {

class ReqPipe: public CppSystemRT::Thread {
public:
	ReqPipe(CppSystemRT::File* conn);
	virtual ~ReqPipe();

	void run();
	
	void send(Record& rec);
	
	bool finished;
	
private:
	std::unique_ptr<CppSystemRT::File> conn;
	std::map<int,Request*> requests;
	bool keepAlive;
	std::mutex socketMutex;
};

}

#endif
