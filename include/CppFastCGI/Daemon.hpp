#ifndef _CPPFASTCGI_DAEMON_HPP
#define _CPPFASTCGI_DAEMON_HPP

namespace CppFastCGI {

class DaemonThread: public CppSystemRT::Thread {
public:
	DaemonThread();
	virtual ~DaemonThread();

	void run();
	
private:
	CppSystemRT::Socket socket;
	//CppSystemRT::ConfigFile configFile;
};

}

#endif
