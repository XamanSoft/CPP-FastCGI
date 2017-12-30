#include <CppSystemRT.hpp>
#include <CppFastCGI.hpp>

using namespace CppFastCGI;

Thread::Thread(CppSystemRT::Socket* sock): socket(sock) {
	
}

Thread::~Thread() {
	
}

void Thread::run() {
	// Receive and dispatch messages
}
