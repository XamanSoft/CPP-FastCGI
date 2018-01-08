#include <CppSystemRT.hpp>
#include <CppFastCGI.hpp>

using namespace CppFastCGI;

Request::Request(CppFastCGI::ReqPipe& parent, int const& id): status(0), id(id), parent(parent) {
	
}

Request::~Request() {
	
}

bool Request::checkStatus(int stat) {
	return status & stat;
}

void Request::run() {
	// Request to be executed
	print("Content-type: text/html\r\n\r\n<html><head><title>Test</title></head><body>");
	for (auto& param : params) {
		print(param.first + " = " + param.second + "<br />\n");
	}
	print("</body></html>");
	end(0);
}

void Request::writeData(Record& rec) {
	switch (rec.requestType()) {
		case Record::PARAMS: // PARAMS of a Request
			if (rec.contentLength() > 0) {
				rec.readNameValuePair(params);
			}
		break;

		case Record::STDIN: // STDIN of a Request
			if (rec.contentLength() > 0) {
				rec.readData(inStream);
			} else {
				status |= RS_READY; // Request can be executed
			}
		break;
	}
}

void Request::end(int code) {
	if (status & RS_STDOUT) {
		Record outRec{Record::STDOUT, id};
		parent.send(outRec);
	}
	if (status & RS_STDERR) {
		Record errRec{Record::STDERR, id};
		parent.send(errRec);
	}
	Record endRec(Record::END_REQUEST, id);
	endRec.writeEndRequestBody(code, Record::REQUEST_COMPLETE);
	parent.send(endRec);
	status |= RS_END;
	exit(code);
}

void Request::error(std::string const& str) {
	status |= RS_STDERR;
	Record errRec(Record::STDERR, id);
	std::stringstream output(str);
	errRec.writeData(output);
	parent.send(errRec);
}

void Request::print(std::string const& str) {
	status |= RS_STDOUT;
	Record outRec(Record::STDOUT, id);
	std::stringstream output(str);
	outRec.writeData(output);
	parent.send(outRec);
}
