#ifndef _CPPFASTCGI_THREAD_HPP
#define _CPPFASTCGI_THREAD_HPP

namespace CppFastCGI {

class DaemonThread;

class Thread: public CppSystemRT::Thread {
public:
	Thread(DaemonThread& parent, CppSystemRT::Socket* sock);
	virtual ~Thread();

	void run();
	
	void send(Record& rec);
	
	bool finished;
	
private:
	std::unique_ptr<CppSystemRT::Socket> socket;
	std::map<int,Process*> processes;
	DaemonThread& parent;
	bool keepAlive;
	std::mutex socketMutex;
};

}

#endif
