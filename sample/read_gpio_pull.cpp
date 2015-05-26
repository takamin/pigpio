#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#include "gpio.h"
int main (int argc, char* argv[])
{
	int port;
	int pullup;
	int value;

    gpio_init();
	if(argc < 1) {
		fprintf(stderr, "read_gpio <port> [<pullup>=1]\n");
		exit(-1);
	}
	port = atoi(argv[1]);
	if(port < 0 || 31 < port) {
		fprintf(stderr, "error: port range 0 .. 31\n");
		exit(-1);
	}
	if(argc > 2) {
		pullup = atoi(argv[2]);
		if(pullup != 0 && pullup != 1) {
			fprintf(stderr,
				"ERROR: invalid pullup (%d), 0 or 1 is available.\n", pullup);
			exit(-1);
		}
	} else {
		pullup = 1;
	}
	gpio_configure(port, GPIO_INPUT);
	gpio_configure_pull(port, (pullup ? GPIO_PULLUP : GPIO_PULLDOWN));
    while (1) {
        value = gpio_read(port);
        printf("input(%d): %d\n", port, value);
        usleep(500000); // 0.5[s]
    }
    return 0;
}

