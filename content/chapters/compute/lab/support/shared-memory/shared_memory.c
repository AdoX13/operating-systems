#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#include "utils/utils.h"

int main(void)
{
	pid_t ret_pid;
	pid_t pid;
	int *p;

	/* TODO 1: Change the flags to disable copy-on-write. */
	p = mmap(NULL, getpagesize(), PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	DIE(p == MAP_FAILED, "mmap");

	*p = 42;

	/**
	 * TODO 2.1: Create a semaphore in the shared page and use it to signal
	 * the child process to end.
	 */

	pid = fork();
	switch (pid) {
	case -1:
		/* `fork()` has encountered an error. */
		DIE(1, "fork");
		break;

	case 0:
		/* Child process */
		*p = 69;
		printf("[child] Wrote value %d at address %p\n", *p, p);

		/**
		* TODO 2.2: Wait for the semaphore to be signalled by the
		* parent.
		*/

		break;

	default:
		/* Parent process */
		/**
		 * TODO 2.3: Sleep for a few seconds before signalling the child
		 * process to end.
		 */

		ret_pid = waitpid(pid, NULL, 0);
		DIE(ret_pid < 0, "waitpid parent");

		/**
		 * TODO 2.4: Make sure the value read by the parent is still the
		 * one written by the child.
		 */
		printf("[parent] Child process exited. Data at address %p = %d\n",
			p, *p);
		break;
	}

	return 0;
}
