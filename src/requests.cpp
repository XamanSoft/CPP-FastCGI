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
	CppFastCGI::Thread* thread = new CppFastCGI::Thread(socket.accept());
	terminateMutex.lock();
	requests.push_back(thread);
	terminateMutex.unlock();
	thread->exec();
}

void Requests::run() {
	terminateMutex.lock();
	for (auto& thread : std::list<CppFastCGI::Thread*>(requests)) {
		if (thread->finished) {
			requests.remove(thread);
			thread->wait();
			delete thread;
		}
	}
	terminateMutex.unlock();
}
