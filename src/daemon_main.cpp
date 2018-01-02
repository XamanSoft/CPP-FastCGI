#include <CppSystemRT.hpp>
#include <CppFastCGI.hpp>

using namespace CppFastCGI;

int main(int argc, char* argv[]) {
	return CppSystemRT::Daemon::exec<DaemonThread>("127.0.0.1", "9000");
}
