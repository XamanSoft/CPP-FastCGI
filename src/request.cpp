#include <CppSystemRT.hpp>
#include <CppFastCGI.hpp>

using namespace CppFastCGI;

Request::Request(RequestInfo* rInfo): requestInfo(rInfo), status(0) {
	
}

Request::~Request() {
	
}

bool Request::checkStatus(int stat) {
	return status & stat;
}

void Request::run() {
	print("Status: 404 Not Found\r\n\r\n");
	end(0);
}

void Request::end(int code) {
	if (status & RS_STDOUT) {
		Record outRec{Record::STDOUT, requestInfo->id};
		requestInfo->parent.send(outRec);
	}
	if (status & RS_STDERR) {
		Record errRec{Record::STDERR, requestInfo->id};
		requestInfo->parent.send(errRec);
	}
	Record endRec(Record::END_REQUEST, requestInfo->id);
	endRec.writeEndRequestBody(code, Record::REQUEST_COMPLETE);
	requestInfo->parent.send(endRec);
	status |= RS_END;
	exit(code);
}

void Request::error(std::string const& str) {
	status |= RS_STDERR;
	Record errRec(Record::STDERR, requestInfo->id);
	std::stringstream output(str);
	errRec.writeData(output);
	requestInfo->parent.send(errRec);
}

void Request::print(std::string const& str) {
	status |= RS_STDOUT;
	Record outRec(Record::STDOUT, requestInfo->id);
	std::stringstream output(str);
	outRec.writeData(output);
	requestInfo->parent.send(outRec);
}
