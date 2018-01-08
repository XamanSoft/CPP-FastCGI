#ifndef _CPPFASTCGI_REQUEST_HPP
#define _CPPFASTCGI_REQUEST_HPP

namespace CppFastCGI { // CppFastCGI Begin

class ReqPipe;

class Request: public CppSystemRT::Thread {
public:
	enum ReqStatus {
		RS_NONE		= 0,
		RS_STDERR	= 1,
		RS_STDOUT	= 2,
		RS_READY	= 4,
		RS_END		= 8
	};

	Request(CppFastCGI::ReqPipe& parent, int const& id);
	virtual ~Request();
	
	void run();
	
	void writeData(Record& rec);
	bool checkStatus(int stat);

protected:
	void end(int code);
	void error(std::string const& str);
	void print(std::string const& str);

private:
	int id;
	int status;
	CppFastCGI::ReqPipe& parent;
	std::stringstream inStream;
	std::map<std::string, std::string> params;
};

} // CppFastCGI End

#endif
