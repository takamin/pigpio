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

static volatile int signaled = 0;
static void handler(int sig)
{
	assert(sig == SIGINT
#ifndef __MINGW32__
            || sig == SIGHUP
#endif
            || sig == SIGTERM);
	signaled = sig;
}

int main(int argc, char* argv[])
{
#ifndef __MINGW32__
	if (signal(SIGHUP, handler) == SIG_ERR) {
		return 1;
	}
#endif
	if (signal(SIGINT, handler) == SIG_ERR) {
		return 2;
	}
	if (signal(SIGTERM, handler) == SIG_ERR) {
		return 3;
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
