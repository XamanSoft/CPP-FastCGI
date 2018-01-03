#include <CppSystemRT.hpp>
#include <CppFastCGI.hpp>

using namespace CppFastCGI;

#ifdef _WIN32
#define getcwd		_getcwd
#endif

int main(int argc, char* argv[]) {
	char cwd[1024];
	if (getcwd(cwd, sizeof(cwd)) != NULL) {
		CppSystemRT::Daemon::config("pidfile", std::string(cwd)+"/cppfcgi.pid");
	}

	if (argc > 1 && std::string(argv[1]) == "-q") {
		CppSystemRT::Daemon::stop();
		return 0;
	}
	
	return CppSystemRT::Daemon::exec<DaemonThread>("127.0.0.1", "9000");
}
