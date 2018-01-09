#ifndef _CPPFASTCGI_REQUESTHANDLER_HPP
#define _CPPFASTCGI_REQUESTHANDLER_HPP

namespace CppFastCGI {

namespace RequestPath {

	template <typename Tp>
	Request* Create(RequestInfo* requestInfo) {
		return new Tp(requestInfo);
	}

	bool RegexMatch(std::string const& basePath, std::string const& reqPath);
	bool RegexSearch(std::string const& basePath, std::string const& reqPath);

}

class RequestHandler {
public:
	struct PathHandler {
		typedef bool (*request_check_t)(std::string const& basePath, std::string const& reqPath);
		typedef Request* (*create_request_t)(RequestInfo* requestInfo);
	
		std::string path;
		request_check_t request_check;
		create_request_t request_handler;
		
		bool operator()(std::string const& reqPath) const {
			if (request_check)
				return request_check(path, reqPath);

			return reqPath.find(path) == 0;
		}
		
		PathHandler(): request_check(nullptr), request_handler(nullptr) {}
		PathHandler(std::string path, request_check_t request_check, create_request_t request_handler):
			path(path), request_check(request_check), request_handler(request_handler) {}
	};

	RequestHandler(std::vector<std::string> const& params = { "REQUEST_URI" });
	virtual ~RequestHandler();
	
	template <typename Tp>
	inline void registerPath(std::string const& base) {
		registerRequestPath(base, RequestPath::Create<Tp>);
	}

	template <typename Tp>
	inline void registerPath(std::string const& base, PathHandler::request_check_t check) {
		registerRequestPath(base, check, RequestPath::Create<Tp>);
	}

	Request* create(RequestInfo* requestInfo) const;

private:
	void registerRequestPath(std::string const& base, PathHandler::create_request_t handler);
	void registerRequestPath(std::string const& base, PathHandler::request_check_t check, PathHandler::create_request_t handler);

	std::vector<std::string> params;
	std::vector<PathHandler> handlers;
};
	
}

#endif
