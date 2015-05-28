/*
 * GPIO ports definition
 *
 * Signal for cars
 *
 * GPIO_2 - Blue
 * GPIO_3 - Yellow
 * GPIO_4 - Red
 *
 * Signal for pedestrian
 *
 * GPIO_14 - Walk
 * GPIO_15 - Stop
 *
 * Message LED for pedestrian
 *
 * GPIO_17 - Push the button to abroad
 * GPIO_18 - Wait till the pedestrian signal turn to 'Walk'
 */
#include <assert.h>
#include <signal.h>
#include <unistd.h>
#include "gpio.h"
#include "gpio_port.h"

#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>

#ifndef __MINGW32__
#include <sys/wait.h>
#include <syslog.h>
#endif

#define FLAG_DAEMONIZE     1
#define FLAG_KILL          2

#ifdef __MINGW32__
/**
 * stub for MinGW
 */
#define LOG_PID 0
#define LOG_DAEMON 0
#define LOG_ERR 0
#define LOG_INFO 0
int openlog(const char* param0, int param1, int param2) {
    return 0;
}
int syslog(int logid, const char* msg) {
    return 0;
}
int daemon(int chdir, int closehandles) {
    return 0;
}
#endif

static volatile int signaled = 0;

static int do_daemon( void )
{
	gpio_init();

	GPIO_PORT* GPIO_LIGHT_CAR_BLUE = gpio_port_output(2);
	GPIO_PORT* GPIO_LIGHT_CAR_YELLOW = gpio_port_output(3);
	GPIO_PORT* GPIO_LIGHT_CAR_RED = gpio_port_output(4);
	GPIO_PORT* GPIO_LIGHT_PED_WALK = gpio_port_output(14);
	GPIO_PORT* GPIO_LIGHT_PED_STOP = gpio_port_output(15);
	GPIO_PORT* GPIO_DISP_PED_PUSH = gpio_port_output(17);
	GPIO_PORT* GPIO_DISP_PED_WAIT = gpio_port_output(18);
	GPIO_PORT* GPIO_PED_BUTTON = gpio_port_input_pullup(27);
	
	while(!signaled) {
		int button_state = gpio_port_read(GPIO_PED_BUTTON);
		int test_output = (button_state == 0 ? 1 : 0);
		gpio_port_write(GPIO_LIGHT_CAR_BLUE, test_output);
		gpio_port_write(GPIO_LIGHT_CAR_YELLOW, test_output);
		gpio_port_write(GPIO_LIGHT_CAR_RED, test_output);
		gpio_port_write(GPIO_LIGHT_PED_WALK, test_output);
		gpio_port_write(GPIO_LIGHT_PED_STOP, test_output);
		gpio_port_write(GPIO_DISP_PED_PUSH, test_output);
		gpio_port_write(GPIO_DISP_PED_WAIT, test_output);
		usleep(10000);
	}
	gpio_port_write(GPIO_LIGHT_CAR_BLUE, 0);
	gpio_port_write(GPIO_LIGHT_CAR_YELLOW, 0);
	gpio_port_write(GPIO_LIGHT_CAR_RED, 0);
	gpio_port_write(GPIO_LIGHT_PED_WALK, 0);
	gpio_port_write(GPIO_LIGHT_PED_STOP, 0);
	gpio_port_write(GPIO_DISP_PED_PUSH, 0);
	gpio_port_write(GPIO_DISP_PED_WAIT, 0);

	return 0;
}

static void handler(int sig)
{
	assert(sig == SIGINT || sig == SIGTERM);
	switch(sig) {
		case SIGINT:
			syslog(LOG_INFO, "SIGINT received.\n");
			signaled = sig;
			break;
		case SIGTERM:
			syslog(LOG_INFO, "SIGTERM received.\n");
			signaled = sig;
			break;
		default:
			syslog(LOG_ERR, "unknown signal received.\n");
			break;
	}
}

int main(int argc, char* argv[])
{
    const struct option longopt[] = {
        {"daemon", no_argument, NULL, 'd'},
        {NULL, 0, NULL, 0}
    };
    int opt;
	int exit_code = 0;

    openlog("daemon", LOG_PID, LOG_DAEMON);

    while ((opt = getopt_long(argc, argv, "d", longopt, NULL)) != -1) {
        switch (opt) {
            case 'd':
				if (daemon(0, 0) == -1) {
					syslog(LOG_ERR, "failed to launch pedsignal.\n");
					return 2;
				}
                break;
            default:
                break;
        }
    }

    syslog(LOG_INFO, "pedsignal started.\n");

#ifndef __MINGW32__
	if (signal(SIGHUP, SIG_IGN) == SIG_ERR) {
		syslog(LOG_ERR, "fail to ignore SIGHUP.\n");
		exit_code = 1;
	} else
#endif
	if (signal(SIGINT, handler) == SIG_ERR) {
		syslog(LOG_ERR, "fail to setup SIGINT.\n");
		exit_code = 2;
	} else
	if (signal(SIGTERM, handler) == SIG_ERR) {
		syslog(LOG_ERR, "fail to setup SIGTERM.\n");
		exit_code = 3;
	} else {
		exit_code = do_daemon();
	}

	syslog(LOG_INFO, "pedsignal stopped.\n");
	return exit_code;
}
