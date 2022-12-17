#include <signal.h>
#include <stdio.h>
/* first, here is the signal handler */
void catch_int(int sig_num) {
}
int main() {
/* set the INT (Ctrl-C) signal handler to 'catch_int' */
	/* set the INT (Ctrl-C) signal handler to 'catch_int' */
	struct sigaction act1;
	sigfillset(&act1.sa_mask);
	act1.sa_flags = SA_RESTART;
	act1.sa_handler = &catch_int;
	if (sigaction(SIGTERM, &act1, NULL) == -1) {//sigaction failed
	perror("smash error: sigaction failed");
	};  
/* now, lets get into an infinite loop of doing nothing. */
while(1){}
}
