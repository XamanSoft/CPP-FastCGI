# CPP-FastCGI

The aim of this project is to create a powerful C++ FastCGI library, where each request is run in a thread.

## Daemon Example

```cpp
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

int main(int argc, char* argv[]) {
	RequestHandler requestHandler;
	requestHandler.registerPath<DebugRequest>("^/debug", RequestPath::RegexMatch);
	
	return CppSystemRT::Daemon::exec<Server>("127.0.0.1", "9000", requestHandler);
}
```