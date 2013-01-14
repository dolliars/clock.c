#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <signal.h>

void print_time(int signum)
{
	struct timeval now;
	char time[21];
	gettimeofday(&now, NULL);
	strftime(time, sizeof time, "%a %Y-%m-%d %H:%M", localtime(&now.tv_sec));
	printf("%s\n", time);
	fflush(stdout);
}

int main()
{
	print_time(0);

	/* set up SIGALRM-handling callback */
	struct sigaction alarm;
	alarm.sa_handler = print_time;
	sigemptyset(&alarm.sa_mask);
	alarm.sa_flags = 0;
	sigaction(SIGALRM, &alarm, NULL);

	/* set up timer. Sends a SIGALRM when countdown reaches 0,
	   then starts counting down again from 60 seconds. */
	struct timeval interval = {60, 0};
	struct timeval timeleft;
	struct timeval now;
	gettimeofday(&now, NULL);
	struct tm *now_tm = localtime(&now.tv_sec);
	if(now.tv_usec == 0) {
		timeleft.tv_sec = 60 - now_tm->tm_sec;
	} else {
		timeleft.tv_sec = 59 - now_tm->tm_sec;
		timeleft.tv_usec = 1000000 - now.tv_usec;
	}
	struct itimerval timer = {interval, timeleft};
	setitimer(ITIMER_REAL, &timer, NULL);

	/* do nothing until a signal is received, then do nothing again */
	while(pause()) {
	}

	/* this should never be reached */
	return 0;
}
