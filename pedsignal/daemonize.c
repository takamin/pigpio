#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <signal.h>
#ifndef __MINGW32__
#include <syslog.h>
#else
int openlog(const char *ident, int option, int facility) { return 0; }
void vsyslog(int, const char* f, ...) { va_list a; va_start(a, f); va_end(a); }
int daemon(int nochdir, int noclosehandles) { return 0; }
#endif
#include "daemonize.h"

/*!
 * flag that the process running as daemon.
 * This is same to damonize's return value.
 */
static int daemonized = 0;

void dlog(int priority, const char* format, ...)
{
    va_list vargs;
    va_start(vargs, format);
    if(daemonized) {
        vsyslog(priority, format, vargs);
    } else {
        fprintf(stderr, "priority=%d : ", priority);
        vfprintf(stderr, format, vargs);
    }
    va_end(vargs);
}

int daemonize(
	const char* pidfilepath,
	const char *syslog_ident,
	int syslog_option,
	int syslog_facility)
{
	daemonized = 0; /* initialize */

    if (daemon(0, 0) != -1) {

	   	/* success to daemonize. */
		daemonized = 1;

		/* open syslog */
		if(syslog_ident) {
			openlog(syslog_ident, syslog_option, syslog_facility);
		}

		/* write daemon pid to the file */
		if(pidfilepath) {
			FILE* pidfile = fopen(pidfilepath, "w+");
			if (pidfile) {
				int pid = getpid();
				fprintf(pidfile, "%d\n", pid);
				fclose(pidfile);
			} else {
				dlog(LOG_ERR,
			   		"daemonize() : failed to write pid.\n");
			}
		}
	}
	return daemonized;
}
