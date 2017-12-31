#include <CppSystemRT.hpp>
#include <CppFastCGI.hpp>

using namespace CppFastCGI;

Thread::Thread(CppSystemRT::Socket* sock): socket(sock) {
	
}

Thread::~Thread() {
	
}

void Thread::run() {
	// Receive and dispatch messages
	if (socket->available() >= Record::HEADER_LEN) {
		Record rec(*socket);
		int id = rec.requestId();
		
		switch (rec.requestType()) {
			case Record::BEGIN_REQUEST:
				processes[id] = new Process(*this, id);
			break;

			case Record::ABORT_REQUEST: // Request to abort a process
				if (processes.count(id))
					processes[id]->exit(-1);
			break;

			case Record::PARAMS: // PARAMS of a process
			case Record::STDIN: // STDIN of a process
				if (processes.count(id)) {
					processes[id]->writeData(rec);
					if (processes[id]->ready)
						processes[id]->exec();
				}
			break;

			case Record::GET_VALUES: // Server request values from FastCGI (MAX_CONNS/MAX_REQS/MPXS_CONNS)
			{
				Record response(Record::GET_VALUES_RESULT);
				response.writeNameValuePair({{Record::MAX_CONNS, "10"}, {Record::MAX_REQS, "50"}, {Record::MPXS_CONNS, "1"}});
				response.write(*socket);
			}
			break;
		}
	}
}

void Thread::send(Record& rec) {
	rec.write(*socket);
}
