#include <CppSystemRT.hpp>
#include <CppFastCGI.hpp>

using namespace CppFastCGI;

Requests::Requests() {
	exec();
}

Requests::~Requests() {
	wait(); // wait until all threads are finished
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

void Requests::push(CppFastCGI::Thread* thread) {
	terminateMutex.lock();
	requests.push_back(thread);
	terminateMutex.unlock();
}
