#include <CppSystemRT.hpp>
#include <CppFastCGI.hpp>

using namespace CppFastCGI;

Requests::Requests(std::string const& address, std::map<std::string, std::string> const& params): socket(address, params) {
	exec();
}

Requests::~Requests() {
	exit(0);
	wait(); // wait until all threads are finished
}

bool Requests::hasConnection() {
	return socket.select();
}

void Requests::accept() {
	CppFastCGI::ReqPipe* reqpipe = new CppFastCGI::ReqPipe(socket.accept());
	terminateMutex.lock();
	requests.push_back(reqpipe);
	terminateMutex.unlock();
	reqpipe->exec();
}

void Requests::run() {
	terminateMutex.lock();
	for (auto& reqpipe : std::list<CppFastCGI::ReqPipe*>(requests)) {
		if (reqpipe->checkStatus(ReqPipe::RPS_FINISHED)) {
			requests.remove(reqpipe);
			reqpipe->wait();
			delete reqpipe;
		}
	}
	terminateMutex.unlock();
}
