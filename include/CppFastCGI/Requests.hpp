#ifndef _CPPFASTCGI_REQUESTS_HPP
#define _CPPFASTCGI_REQUESTS_HPP

namespace CppFastCGI { // CppFastCGI Begin

class Thread;

class Requests: public CppSystemRT::Thread {
public:
	Requests();
	virtual ~Requests();
	
	void run();
	
	void push(CppFastCGI::Thread* thread);

private:
	std::list<CppFastCGI::Thread*> requests;
	std::mutex terminateMutex;
};

} // CppFastCGI End

#endif
