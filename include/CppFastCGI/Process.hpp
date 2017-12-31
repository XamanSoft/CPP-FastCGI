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
	
protected:
	void print(std::string const& str);

private:
	int id;
	CppFastCGI::Thread& parent;
	std::stringstream stdin;
	std::map<std::string, std::string> params;
};

} // CppFastCGI End

#endif
