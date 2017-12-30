#ifndef _CPPFASTCGI_DAEMON_HPP
#define _CPPFASTCGI_DAEMON_HPP

namespace CppFastCGI {

class DaemonThread: public CppSystemRT::Thread {
public:
	DaemonThread(std::string const& address, std::string const& port);
	virtual ~DaemonThread();

	void run();
	
private:
	CppSystemRT::Socket socket;
	std::list<Thread*> requests;
};

}

#endif
