#include <CppSystemRT.hpp>
#include <CppFastCGI.hpp>

using namespace CppFastCGI;

DaemonThread::DaemonThread(std::string const& address, std::string const& port): socket(address, {{"port", port}, {"bind", "true"}}) {
}

DaemonThread::~DaemonThread() {
	// wait/stop all threads
	for (auto& thread : requests) {
		thread->wait();
		delete thread;
	}
}

void DaemonThread::run() {
	Thread* thread = new CppFastCGI::Thread(socket.accept());
	requests.push_back(thread);	
	thread->exec();
}
