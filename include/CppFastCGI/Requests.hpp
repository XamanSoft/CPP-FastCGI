#ifndef _CPPFASTCGI_REQUESTS_HPP
#define _CPPFASTCGI_REQUESTS_HPP

namespace CppFastCGI { // CppFastCGI Begin

class Requests: public CppSystemRT::Thread {
public:
	Requests(std::string const& address, std::map<std::string, std::string> const& params);
	virtual ~Requests();
	
	void run();
	
	bool hasConnection();
	void accept();

private:
	std::list<CppFastCGI::ReqPipe*> requests;
	std::mutex terminateMutex;
	CppSystemRT::Socket socket;
};

} // CppFastCGI End

#endif
