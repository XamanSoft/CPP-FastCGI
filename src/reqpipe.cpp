#include <CppSystemRT.hpp>
#include <CppFastCGI.hpp>

using namespace CppFastCGI;

ReqPipe::ReqPipe(CppSystemRT::File* conn): finished(false), conn(conn), keepAlive(true) {
	
}

ReqPipe::~ReqPipe() { conn->close(); }

void ReqPipe::run() {
	// Receive and dispatch messages
	if (conn->available() >= Record::HEADER_LEN) {
		Record rec(*conn);
		int id = rec.requestId();
		
		switch (rec.requestType()) {
			case Record::BEGIN_REQUEST: {
				int role = 0, flags = 0;
				requests[id] = new Request(*this, id);
				rec.readBeginRequestBody(role, flags);
				keepAlive = flags & Record::KEEP_CONN; // Last begin connection determine if the connection should stay alive
			}
			break;

			case Record::ABORT_REQUEST: // Request to abort a process
				if (requests.count(id))
					requests[id]->exit(-1);
			break;

			case Record::PARAMS: // PARAMS of a process
			case Record::STDIN: // STDIN of a process
				if (requests.count(id)) {
					requests[id]->writeData(rec);
					if (requests[id]->ready) // Ready after STDIN contentLength == 0
						requests[id]->exec();
				}
			break;

			case Record::GET_VALUES: { // Server request values from FastCGI (MAX_CONNS/MAX_REQS/MPXS_CONNS)
				if (rec.requestId() == Record::NULL_REQUEST_ID) {
					Record response(Record::GET_VALUES_RESULT);
					response.writeNameValuePair({{Record::MAX_CONNS, "10"}, {Record::MAX_REQS, "50"}, {Record::MPXS_CONNS, "1"}});
					response.write(*conn);
				} else {
					// ?
				}
			}
			break;
		}
	}

	// check if last requests have finished and exit the thread
	for (auto& request : std::map<int,Request*>(requests)) {
		if (request.second->finished) {
			Request* proc = request.second;
			requests.erase(request.first);
			proc->wait();
			delete proc;
		}
	}

	if (!keepAlive && !requests.size()) {
		finished = true;
		exit(0);
	}
}

void ReqPipe::send(Record& rec) {
	socketMutex.lock();
	rec.write(*conn);
	socketMutex.unlock();
}
