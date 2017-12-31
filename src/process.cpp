#include <CppSystemRT.hpp>
#include <CppFastCGI.hpp>

using namespace CppFastCGI;

Process::Process(CppFastCGI::Thread& parent, int const& id): id(id), ready(false), parent(parent) {
	
}

Process::~Process() {
	
}

int Process::exec(bool waitFinish) {
	if (ready)
		return CppSystemRT::Thread::exec(waitFinish);

	return 0;
}

void Process::run() {
	// Process to be executed
}

void Process::writeData(Record& rec) {
	switch (rec.requestType()) {
		case Record::PARAMS: // PARAMS of a process
			if (rec.contentLength() > 0) {
				rec.readNameValuePair(params);
			}
		break;

		case Record::STDIN: // STDIN of a process
			if (rec.contentLength() > 0) {
				rec.readData(stdin);
			} else {
				ready = true; // process can be executed
			}
		break;
	}
}

void Process::print(std::string const& str) {
	Record outRec(Record::STDOUT, id);
	std::stringstream output(str);
	outRec.writeData(output);
	parent.send(outRec);
}
