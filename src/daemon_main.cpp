#include <CppSystemRT.hpp>
#include <CppFastCGI.hpp>

using namespace CppFastCGI;

class DebugRequest: public Request {
public:
	DebugRequest(RequestInfo* requestInfo): Request(requestInfo) {}

protected:	
	void run() {
		print("Content-type: text/html; charset=utf-8\r\n\r\n");
		print("<html><head><title>Debug Request</title></head><body>");
		for (auto& param : requestInfo->params) {
			print(param.first + " = " + param.second + "<br>\n");
		}
		print("</body></html>");
		end(0);
	}
};

REQUESTMODULE(PythonModule, CppSystemRT::Path::SharedObject("$EWD/mod_python"));

int main(int argc, char* argv[]) {
	CppSystemRT::Daemon::config("pidfile", CppSystemRT::Path::Parse("$EWD/cppfcgi.pid"));

	if (argc > 1 && std::string(argv[1]) == "-q") {
		CppSystemRT::Daemon::stop();
		return 0;
	}

	RequestHandler requestHandler;
	requestHandler.registerPath<DebugRequest>("^/debug", RequestPath::RegexMatch);
	requestHandler.registerModulePath<PythonModule>("\\.py(/|\\?|$)", RequestPath::RegexSearch);
	
	return CppSystemRT::Daemon::exec<Server>("127.0.0.1", "9000", requestHandler);
}
