#include <CppSystemRT.hpp>
#include <CppFastCGI.hpp>

using namespace CppFastCGI;

DaemonThread::DaemonThread(std::string const& address, std::string const& port): requests(address, {{"port", port}, {"bind", "true"}}) {
}

DaemonThread::~DaemonThread() { }

void DaemonThread::run() {
	if (!requests.hasConnection())
		return;

	requests.accept();
}
