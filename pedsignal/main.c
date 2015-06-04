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
#include "daemonize.h"
#include "gpio.h"
#include "gpio_port.h"

#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>

#ifndef __MINGW32__
#include <sys/wait.h>
#include <syslog.h>
#endif

static volatile int sigterm = 0;
static volatile int sigint = 0;
static volatile int sighup = 0;
static void handle_sigint(int sig)
{
    dlog(LOG_INFO, "SIGINT received.\n");
    sigint = 1;
}
static void handle_sigterm(int sig)
{
    dlog(LOG_INFO, "SIGTERM received.\n");
    sigterm = 1;
}
static void handle_sighup(int sig)
{
    dlog(LOG_INFO, "SIGHUP received.\n");
    sighup = 1;
}
static int pedsignal_main(int argc, char* argv[], char* envp[])
{
    dlog(LOG_INFO, "pedsignal started.\n");
    if (signal(SIGINT, handle_sigint) == SIG_ERR) {
        dlog(LOG_ERR, "fail to setup SIGINT.\n");
        return 2;
    } else
    if (signal(SIGTERM, handle_sigterm) == SIG_ERR) {
        dlog(LOG_ERR, "fail to setup SIGTERM.\n");
        return 2;
#ifndef __MINGW32__
    } else if (signal(SIGHUP, handle_sighup) == SIG_ERR) {
        dlog(LOG_ERR, "fail to ignore SIGHUP.\n");
        return 2;
#endif
    }
	gpio_init();

	GPIO_PORT* GPIO_LIGHT_CAR_BLUE = gpio_port_output(2);
	GPIO_PORT* GPIO_LIGHT_CAR_YELLOW = gpio_port_output(3);
	GPIO_PORT* GPIO_LIGHT_CAR_RED = gpio_port_output(4);
	GPIO_PORT* GPIO_LIGHT_PED_WALK = gpio_port_output(14);
	GPIO_PORT* GPIO_LIGHT_PED_STOP = gpio_port_output(15);
	GPIO_PORT* GPIO_DISP_PED_PUSH = gpio_port_output(17);
	GPIO_PORT* GPIO_DISP_PED_WAIT = gpio_port_output(18);
	GPIO_PORT* GPIO_PED_BUTTON = gpio_port_input_pullup(27);
	
	while(!sigterm && !sigint) {
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

    dlog(LOG_INFO, "pedsignal stopped.\n");
	return 0;
}

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
        daemonize(pidfilepath, "daemon", LOG_PID, LOG_DAEMON);
    }
    return pedsignal_main(argc, argv, envp);
}
