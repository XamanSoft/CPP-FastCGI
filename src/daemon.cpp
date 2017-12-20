#include <CppSystemRT.hpp>
#include <CppFastCGI.hpp>

using namespace CppFastCGI;

DaemonThread::DaemonThread() {
	socket->bind("localhost", "9000");
}

DaemonThread::~DaemonThread() {
	
}

void DaemonThread::run() {
	socket->accept();
	exit(0);
}
