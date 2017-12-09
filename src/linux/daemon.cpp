#include "../stdafx.h"
#include <CppFastCGI.hpp>

namespace CppFastCGI {
	
bool running{false};
void DaemonProcess();

int DaemonEntry() {
	// Process ID and Session ID
	pid_t pid, sid;
	int fd;

	// Forking the parent process
	// fork return the process id of the child process
	pid = fork();
	if (pid < 0) {
		return EXIT_FAILURE;
	}

	// setting the umask to 0 in order to have full access to the files generated by the daemon
	umask(0);

	// Creating a new SID for the child process
	// The child process must get a unique SID from the kernel in order to operate
	sid = setsid();
	if (sid < 0) {
		return EXIT_FAILURE;
	}

	// Changing the current working directory
	if ((chdir("/")) < 0) {
		return EXIT_FAILURE;
	}
	
	// Close all open file descriptors
	for (fd = sysconf(_SC_OPEN_MAX); fd > 0; fd--) {
		close(fd);
	}

	// Reopen stdin (fd = 0), stdout (fd = 1), stderr (fd = 2)
	stdin = fopen("/dev/null", "r");
	stdout = fopen("/dev/null", "w+");
	stderr = fopen("/dev/null", "w+");

	// calling process function
	DaemonProcess();

	return EXIT_SUCCESS;
}

void DaemonProcess() {
	//DaemonWorker worker;

	while (running) {
		//worker.run();
		sleep(1);
	}

}

}
