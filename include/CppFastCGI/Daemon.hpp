#ifndef _CPPFASTCGI_DAEMON_HPP
#define _CPPFASTCGI_DAEMON_HPP

namespace CppFastCGI {
	
class DaemonWorker {
public:
	virtual ~DaemonWorker() {};

	virtual void run() =0;
};

int DaemonEntry();

}

#endif
