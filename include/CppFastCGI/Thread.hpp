#ifndef _CPPFASTCGI_THREAD_HPP
#define _CPPFASTCGI_THREAD_HPP

namespace CppFastCGI {

class Thread: public CppSystemRT::Thread {
public:
	Thread(CppSystemRT::Socket* sock);
	virtual ~Thread();

	void run();
	
	void send(Record& rec);
	
private:
	std::unique_ptr<CppSystemRT::Socket> socket;
	std::map<int,Process*> processes;
};

}

#endif
