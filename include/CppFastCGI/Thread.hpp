#ifndef _CPPFASTCGI_THREAD_HPP
#define _CPPFASTCGI_THREAD_HPP

namespace CppFastCGI {

class DaemonThread;

class Thread: public CppSystemRT::Thread {
public:
	Thread(CppSystemRT::File* conn);
	virtual ~Thread();

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
