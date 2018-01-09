#ifndef _CPPFASTCGI_SERVER_HPP
#define _CPPFASTCGI_SERVER_HPP

namespace CppFastCGI {
	
class RequestHandler;

class Server: public CppSystemRT::Thread {
public:
	Server(std::string const& address, std::string const& port, RequestHandler const& rqHnd = RequestHandler());
	virtual ~Server();
	
	void run();
	
private:
	std::list<RequestPipe*> requests;
	RequestHandler const& requestHandler;
	CppSystemRT::Socket socket;
};

}

#endif
