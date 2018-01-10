#include <CppSystemRT.hpp>
#include <CppFastCGI.hpp>

#ifdef _WIN32
#if PYTHON_MODULE
#   define MODULE_EXPORT extern "C" __declspec(dllexport)
#else
#   define MODULE_EXPORT __declspec(dllimport)
#endif
#else
#   define MODULE_EXPORT extern "C" 
#endif

using namespace CppFastCGI;

class PythonRequest: public Request {
public:
	PythonRequest(RequestInfo* requestInfo): Request(requestInfo) {}

protected:	
	void run() {
		print("Content-type: text/html; charset=utf-8\r\n\r\n");
		print("<html><head><title>Python Request</title></head><body>");
		for (auto& param : requestInfo->params) {
			print(param.first + " = " + param.second + "<br>\n");
		}
		print("</body></html>");
		end(0);
	}
};

MODULE_EXPORT Request* request_handler(RequestInfo* requestInfo) {
	return new PythonRequest(requestInfo);
}
