#include <CppSystemRT.hpp>
#include <CppFastCGI.hpp>

using namespace CppFastCGI;

bool RequestPath::RegexMatch(std::string const& basePath, std::string const& reqPath) {
	return std::regex_match(reqPath, std::regex(basePath));
}

bool RequestPath::RegexSearch(std::string const& basePath, std::string const& reqPath) {
	return std::regex_search(reqPath, std::regex(basePath));
}

RequestHandler::RequestHandler(std::vector<std::string> const& params): params(params) {
	
}

RequestHandler::~RequestHandler() {
	
}

void RequestHandler::registerRequestPath(std::string const& base, PathHandler::create_request_t handler) {
	handlers.push_back(PathHandler{base, nullptr, handler});
}

void RequestHandler::registerRequestPath(std::string const& base, PathHandler::request_check_t check, PathHandler::create_request_t handler) {
	handlers.push_back(PathHandler{base, check, handler});
}

Request* RequestHandler::create(RequestInfo* requestInfo) const {
	for (auto& param : params) {
		std::string path;

		if (requestInfo->params.count(param))
			path = requestInfo->params[param];

		if (!path.empty()) {
			for (auto& handler: handlers) {
				if (handler(path)) {
					Request* request = handler.request_handler(requestInfo);

					if (request)
						return request;
				}
			}
		}
	}
	
	return new Request(requestInfo);
}
