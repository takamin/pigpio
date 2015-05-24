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
#include <unistd.h>
#include "gpio.h"

#define LIGHT_CAR_BLUE 2
#define LIGHT_CAR_YELLOW 3
#define LIGHT_CAR_RED 4
#define LIGHT_PED_WALK 14
#define LIGHT_PED_STOP 15
#define DISP_PED_PUSH 17
#define DISP_PED_WAIT 18
#define PED_BUTTON 27

int main(int argc, char* argv[])
{
	gpio_init();

	gpio_configure(LIGHT_CAR_BLUE, GPIO_OUTPUT);
	gpio_configure(LIGHT_CAR_YELLOW, GPIO_OUTPUT);
	gpio_configure(LIGHT_CAR_RED, GPIO_OUTPUT);
	gpio_configure(LIGHT_PED_WALK, GPIO_OUTPUT);
	gpio_configure(LIGHT_PED_STOP, GPIO_OUTPUT);
	gpio_configure(DISP_PED_PUSH, GPIO_OUTPUT);
	gpio_configure(DISP_PED_WAIT, GPIO_OUTPUT);
	gpio_configure(PED_BUTTON, GPIO_INPUT);
	gpio_configure_pull(PED_BUTTON, GPIO_PULLUP);
	
	while(1) {
		int button_state = gpio_read(PED_BUTTON);
		int test_output = (button_state == 0 ? 1 : 0);
		if(test_output) {
			gpio_set(LIGHT_CAR_BLUE);
			gpio_set(LIGHT_CAR_YELLOW);
			gpio_set(LIGHT_CAR_RED);
			gpio_set(LIGHT_PED_WALK);
			gpio_set(LIGHT_PED_STOP);
			gpio_set(DISP_PED_PUSH);
			gpio_set(DISP_PED_WAIT);
		} else {
			gpio_clear(LIGHT_CAR_BLUE);
			gpio_clear(LIGHT_CAR_YELLOW);
			gpio_clear(LIGHT_CAR_RED);
			gpio_clear(LIGHT_PED_WALK);
			gpio_clear(LIGHT_PED_STOP);
			gpio_clear(DISP_PED_PUSH);
			gpio_clear(DISP_PED_WAIT);
		}
		usleep(10000);
	}
	return 0;
}
