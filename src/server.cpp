#include <CppSystemRT.hpp>
#include <CppFastCGI.hpp>

using namespace CppFastCGI;

Server::Server(std::string const& address, std::string const& port): requests(address, {{"port", port}, {"bind", "true"}}) {
}

Server::~Server() { }

void Server::run() {
	if (!requests.hasConnection())
		return;

	requests.accept();
}
