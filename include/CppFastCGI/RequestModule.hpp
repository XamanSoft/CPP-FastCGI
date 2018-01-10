#ifndef _CPPFASTCGI_REQUESTMODULE_HPP
#define _CPPFASTCGI_REQUESTMODULE_HPP

namespace CppFastCGI {
	
#define REQUESTMODULE(rm_name, path) struct rm_name: public RequestModule {rm_name(): RequestModule(path) {} virtual ~rm_name(){}}

namespace RequestPath {
	typedef bool (*request_check_t)(std::string const& basePath, std::string const& reqPath);
	typedef Request* (*create_request_t)(RequestInfo* requestInfo);
}

class RequestModule: private CppSystemRT::SharedLib {
public:
	RequestModule(std::string const& module);
	virtual ~RequestModule();

	Request* create(RequestInfo* requestInfo) const;
	
	template <typename Tp>
	static Request* Create(RequestInfo* requestInfo) {
		static Tp rmodule;
		return rmodule.create(requestInfo);
	}
	
private:
	RequestPath::create_request_t request_handler;
};

}

#endif
