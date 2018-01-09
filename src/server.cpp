#include <CppSystemRT.hpp>
#include <CppFastCGI.hpp>

using namespace CppFastCGI;

Server::Server(std::string const& address, std::string const& port, RequestHandler const& rqHnd):
	requestHandler(rqHnd), socket(address, {{"port", port}, {"bind", "true"}}) {
}

Server::~Server() { }

void Server::run() {
	if (socket.select()) {
		RequestPipe* reqpipe = new RequestPipe(requestHandler, socket.accept());
		requests.push_back(reqpipe);
		reqpipe->exec();
	}
	
	for (auto& reqpipe : std::list<RequestPipe*>(requests)) {
		if (reqpipe->checkStatus(RequestPipe::RPS_FINISHED)) {
			requests.remove(reqpipe);
			reqpipe->wait();
			delete reqpipe;
		}
	}
}
