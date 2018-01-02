#ifndef _CPPFASTCGI_PROCESS_HPP
#define _CPPFASTCGI_PROCESS_HPP

namespace CppFastCGI { // CppFastCGI Begin

class Thread;

class Process: public CppSystemRT::Thread {
public:
	Process(CppFastCGI::Thread& parent, int const& id);
	virtual ~Process();
	
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
	CppFastCGI::Thread& parent;
	std::stringstream inStream;
	std::map<std::string, std::string> params;
};

} // CppFastCGI End

#endif
