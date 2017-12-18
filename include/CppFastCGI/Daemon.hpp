#ifndef _CPPFASTCGI_DAEMON_HPP
#define _CPPFASTCGI_DAEMON_HPP

namespace CppFastCGI {

class DaemonThread: public CppSystemRT::Thread {
public:
	void run();
};

}

#endif
