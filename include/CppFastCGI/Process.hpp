#ifndef _CPPFASTCGI_PROCESS_HPP
#define _CPPFASTCGI_PROCESS_HPP

namespace CppFastCGI { // CppFastCGI Begin

class Process: public CppSystemRT::Thread {
public:
	Process(int const& id);
	virtual ~Process();
	
	void run();

private:
	int id;
};

} // CppFastCGI End

#endif
