#ifndef _CPPFASTCGI_SERVER_HPP
#define _CPPFASTCGI_SERVER_HPP

namespace CppFastCGI {

class Server: public CppSystemRT::Thread {
public:
	Server(std::string const& address, std::string const& port);
	virtual ~Server();

	void run();
	
private:
	Requests requests;
};

}

#endif
