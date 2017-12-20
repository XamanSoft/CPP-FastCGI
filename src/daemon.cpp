#include <CppSystemRT.hpp>
#include <CppFastCGI.hpp>

using namespace CppFastCGI;

DaemonThread::DaemonThread(): socket("localhost", "9000") {
	
}

DaemonThread::~DaemonThread() {
	
}

void DaemonThread::run() {
	exit(0);
}
