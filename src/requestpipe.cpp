#include <CppSystemRT.hpp>
#include <CppFastCGI.hpp>

using namespace CppFastCGI;

RequestPipe::RequestPipe(RequestHandler const& requestHandler, CppSystemRT::File* conn): requestHandler(requestHandler), conn(conn), status(0) {
	
}

RequestPipe::~RequestPipe() { conn->close(); }

void RequestPipe::run() {
	// Receive and dispatch messages
	if (conn->available() >= Record::HEADER_LEN) {
		Record rec(*conn);
		int id = rec.requestId();

		switch (rec.requestType()) {
			case Record::BEGIN_REQUEST: {
				int role = 0, flags = 0;
				requests[id] = new RequestInfo{ id, *this, nullptr };
				rec.readBeginRequestBody(role, flags);
				status &= ~RPS_KEEPALIVE;
				if (flags & Record::KEEP_CONN) // Last begin connection determine if the connection should stay alive
					status |= RPS_KEEPALIVE;
			}
			break;

			case Record::ABORT_REQUEST: // Request to abort a process
				if (requests.count(id) && requests[id]->request)
					requests[id]->request->exit(-1);
			break;

			case Record::PARAMS: // PARAMS of a process
				if (requests.count(id)) {
					if (rec.contentLength() > 0) {
						rec.readNameValuePair(requests[id]->params);
					}
				}
			break;
			
			case Record::STDIN: // STDIN of a process
				if (requests.count(id)) {
					if (rec.contentLength() > 0) {
						rec.readData(requests[id]->inStream);
					} else {
						Request* request = requestHandler.create(requests[id]);
						requests[id]->request = request;
						request->exec(); // Ready after STDIN contentLength == 0
					}
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
	for (auto& request : std::map<int,RequestInfo*>(requests)) {
		if (request.second->request && request.second->request->checkStatus(Request::RS_END)) {
			RequestInfo* requestInfo = request.second;
			requests.erase(request.first);
			if (requestInfo->request) {
				requestInfo->request->wait();
				delete requestInfo->request;
			}
			delete requestInfo;
		}
	}

	if (!(status & RPS_KEEPALIVE) && !requests.size()) {
		status |= RPS_FINISHED;
		exit(0);
	}
}

bool RequestPipe::checkStatus(int stat) {
	return status & stat;
}

void RequestPipe::send(Record& rec) {
	rec.write(*conn);
}
