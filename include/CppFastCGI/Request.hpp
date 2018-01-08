#ifndef _CPPFASTCGI_REQUEST_HPP
#define _CPPFASTCGI_REQUEST_HPP

namespace CppFastCGI { // CppFastCGI Begin

class ReqPipe;

class Request: public CppSystemRT::Thread {
public:
	Request(CppFastCGI::ReqPipe& parent, int const& id);
	virtual ~Request();
	
	int exec(bool waitFinish = false);
	void run();
	
	void writeData(Record& rec);
	
	bool ready;
	bool finished;
	
	int status;
	
	enum ProcStatus {
		PS_NONE		= 0,
		PS_STDERR	= 1,
		PS_STDOUT	= 2,
		PS_END		= 4
	};

protected:
	void end(int code);
	void error(std::string const& str);
	void print(std::string const& str);

private:
	int id;
	CppFastCGI::ReqPipe& parent;
	std::stringstream inStream;
	std::map<std::string, std::string> params;
};

} // CppFastCGI End

#endif
