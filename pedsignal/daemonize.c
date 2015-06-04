#include <assert.h>
#include <signal.h>
#include <unistd.h>
#include "daemonize.h"
#include "gpio.h"
#include "gpio_port.h"

#include <stdio.h>
#include <stdarg.h>
#include <getopt.h>
#include <sys/types.h>

#ifndef __MINGW32__
#include <sys/wait.h>
#include <syslog.h>
#endif

int daemonized = 0;

#ifdef __MINGW32__
int openlog(const char *ident, int option, int facility)
{
    fprintf(stderr, "MinGW ignores : openlog(ident=\"%s\", option=0x%08X, facility=0x%08X)\n", ident, option, facility);
    return 0;
}
int daemon(int nochdir, int noclosehandles)
{
    fprintf(stderr, "MinGW ignores : daemon(nochdir=%d, noclosehandles=%d)", nochdir, noclosehandles);
    return 0;
}
#endif

void dlog(int priority, const char* format, ...)
{
    va_list vargs;
    

    va_start(vargs, format);
    if(daemonized) {
#ifdef __MINGW32__
        fprintf(stderr, "priority=%d : ", priority);
        vfprintf(stderr, format, vargs);
#else
        vsyslog(priority, format, vargs);
#endif
    } else {
        fprintf(stderr, "priority=%d : ", priority);
        vfprintf(stderr, format, vargs);
    }
    va_end(vargs);
}

int daemonize(const char* pidfilepath, const char *syslog_ident, int syslog_option, int syslog_facility)
{
    if (daemon(0, 0) == -1) {
        fprintf(stderr, "daemonize() : failed to daemonize.\n");
    } else {
        daemonized = 1;
        if(syslog_ident) {
            openlog(syslog_ident, syslog_option, syslog_facility);
        }
        if(pidfilepath) {
            FILE* pidfile = fopen(pidfilepath, "w+");
            if (pidfile) {
                int pid = getpid();
                fprintf(pidfile, "%d\n", pid);
                fclose(pidfile);
            } else {
                dlog(LOG_ERR, "daemonize() : failed to record process id to a file.\n");
            }
        }
    }
	return daemonized;
}
