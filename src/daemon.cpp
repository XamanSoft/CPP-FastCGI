#include <CppSystemRT.hpp>
#include <CppFastCGI.hpp>

using namespace CppFastCGI;

DaemonThread::DaemonThread(std::string const& address, std::string const& port): socket(address, {{"port", port}, {"bind", "true"}}) {
}

DaemonThread::~DaemonThread() { }

void DaemonThread::run() {
	if (!socket.select())
		return;

	CppFastCGI::Thread* thread = new CppFastCGI::Thread(*this, socket.accept());
	requests.push(thread);	
	thread->exec();
}
