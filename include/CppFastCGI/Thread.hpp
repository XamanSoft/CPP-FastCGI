#ifndef _CPPFASTCGI_THREAD_HPP
#define _CPPFASTCGI_THREAD_HPP

namespace CppFastCGI {

class DaemonThread;

class Thread: public CppSystemRT::Thread {
public:
	Thread(CppSystemRT::Socket* sock);
	virtual ~Thread();

	void run();
	
	void send(Record& rec);
	
	bool finished;
	
private:
	std::unique_ptr<CppSystemRT::Socket> socket;
	std::map<int,Process*> processes;
	bool keepAlive;
	std::mutex socketMutex;
};

}

#endif
