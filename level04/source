#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <linux/ptrace.h>

int	main(void)
{
	int		pid = fork();
	char	buffer[32];
	int		ptrace_ret = 0;
	int		status = 0;

	bzero(buffer, 32);

	if (pid == 0)
	{
		prctl(PR_SET_PDEATHSIG, 1);
		ptrace(PTRACE_TRACEME, 0, 0, 0);
		puts("Give me some shellcode, k");
		gets(buffer);
	}
	else
	{
	    while (ptrace_ret != 11)
	    {
		    wait(&status);
		    if (WIFEXITED(status) || WIFSIGNALED(status))
		    {
			    puts("child is exiting...");
			    return (0);
		    }
		    ptrace_ret = ptrace(PTRACE_PEEKUSR, pid, 44, 0)
	    }
	    puts("no exec() for you");
	    kill(pid, 9);
	}
	return (0);
}
