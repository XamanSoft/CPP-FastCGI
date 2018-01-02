#include <CppSystemRT.hpp>
#include <CppFastCGI.hpp>

using namespace CppFastCGI;

int main() {
	//return CppSystemRT::Daemon::exec<DaemonThread>("localhost", "9000");
	return DaemonThread{"localhost", "9000"}.exec(true);
}
