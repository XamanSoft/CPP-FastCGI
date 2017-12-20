#ifndef _CPPFASTCGI_THREAD_HPP
#define _CPPFASTCGI_THREAD_HPP

namespace CppFastCGI {

class Thread: public CppSystemRT::Thread {
public:
	Thread(CppSystemRT::Socket* sock);
	virtual ~Thread();

	void run();
	
private:
	std::unique_ptr<CppSystemRT::Socket> socket;
};

}

#endif
