#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "gpio.h"
int main (int argc, char* argv[])
{
	int port;
	int value;

	gpio_init();
	if(argc < 2) {
		fprintf(stderr, "write_gpio <port> <value 0/1>\n");
		exit(-1);
	}
	port = atoi(argv[1]);
	if(port < 0 || 31 < port) {
		fprintf(stderr, "error: port range 0 .. 31\n");
		exit(-1);
	}
	value = atoi(argv[2]);
	if(value != 0 && value != 1) {
		value = (value ? 1:0);
		fprintf(stderr, "WARNING: reset value to %d\n", value);
	}
	gpio_configure(port, GPIO_OUTPUT);
	fprintf(stderr, "output %d to port %d\n", value, port);
	if(value != 0) {
		gpio_set(port);
	} else {
		gpio_clear(port);
	}
    return 0;
}
