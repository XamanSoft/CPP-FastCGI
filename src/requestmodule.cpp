#include <CppSystemRT.hpp>
#include <CppFastCGI.hpp>

using namespace CppFastCGI;

RequestModule::RequestModule(std::string const& module): CppSystemRT::SharedLib(module) {
	request_handler = symbol<RequestPath::create_request_t>("request_handler");
}

RequestModule::~RequestModule() {
	
}

Request* RequestModule::create(RequestInfo* requestInfo) const {
	if (request_handler)
		return request_handler(requestInfo);
	
	return nullptr;
}
