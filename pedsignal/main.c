#include <stdio.h>
#include <getopt.h>
#ifndef __MINGW32__
#include <syslog.h>
#endif
#include "daemonize.h"

int daemon_main(int argc, char* argv[], char* envp[]);

int main(int argc, char* argv[], char* envp[])
{
    const struct option longopt[] = {
        {"daemon", no_argument, NULL, 'd'},
        {"pidfile", required_argument, NULL, 'p'},
        {NULL, 0, NULL, 0}
    };
    int opt;
    int opt_daemonize = 0;
    const char* pidfilepath = 0;
    while ((opt = getopt_long(argc, argv, "dp:", longopt, NULL)) != -1) {
        switch (opt) {
            case 'd':
                opt_daemonize = 1;
                break;
            case 'p':
                pidfilepath = optarg;
                break;
            default:
                break;
        }
    }
    if(opt_daemonize) {
        if(!daemonize(0, 0, pidfilepath, "daemon", LOG_PID, LOG_DAEMON)) {
			fprintf(stderr, "failed to daemonize.\n");
			return 2;
		}
    }
    return daemon_main(argc, argv, envp);
}
