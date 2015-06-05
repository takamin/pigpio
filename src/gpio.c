#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#ifndef __MINGW32__
#include <sys/mman.h>
#else
#include <mingw-mman/mman.h>
#define O_SYNC 0
#endif
#include <unistd.h>
#include "gpio.h"

/* register block phisical address */
#define PERI_BASE     0x20000000
#define GPIO_BASE     (PERI_BASE + 0x200000)
#define BLOCK_SIZE    4096

#define GPSET0      7
#define GPCLR0      10
#define GPLEV0      13
#define GPPUD       37
#define GPPUDCLK0   38

/*  GPIO register */
static volatile unsigned int *gpio = (unsigned int*)MAP_FAILED;

void gpio_init()
{
    int fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1) {
        fprintf(stderr, "error: cannot open /dev/mem (gpio_setup)\n");
        exit(-1);
    }
    gpio = (unsigned int*)mmap(NULL, BLOCK_SIZE, 
		PROT_READ | PROT_WRITE, MAP_SHARED, fd, GPIO_BASE );
    close(fd);
    if (gpio == MAP_FAILED) {
        fprintf(stderr, 
			"error: cannot map /dev/mem on the memory (gpio_setup)\n");
        exit(-1);
    }
}

void gpio_configure(int port, int mode)
{
    /*
     * GPIO_BASE
     *              9   8   7   6   5   4   3   2   1   0
     * GPIO_0?: XX 000 000 000 XXX XXX 000 000 000 XXX XXX
     * GPIO_1?: XX XXX 000 000 XXX 000 000 XXX XXX 000 000
     * GPIO_2?: XX 000 000 000 XXX 000 000 000 000 XXX XXX
     * GPIO_3?: XX XXX XXX XXX XXX XXX XXX XXX XXX 000 000
     */
    int index = port / 10;
	int shift_bits = ((port % 10) * 3);
    unsigned int mask = ~(0x7 << shift_bits);

    gpio[index] = (gpio[index] & mask) | ((mode & 0x7) << shift_bits);
}
void gpio_configure_pull(int port, int pullmode)
{
    /* write pullup mode (2bits) NONE / DOWN / UP */
    gpio[GPPUD] = pullmode & 0x3;

    /* set clock status (wait needed) */
    usleep(1);
    gpio[GPPUDCLK0] = 0x1 << port;
    usleep(1);

    /* clear pull-up mode and clock status */
    gpio[37] = 0;
    gpio[38] = 0;
}

void gpio_set(int port)
{
    gpio[GPSET0] = 0x1 << port;
}
void gpio_clear(int port)
{
    gpio[GPCLR0] = 0x1 << port;
}


void gpio_write(int port, int value)
{
    if(value) {
        gpio_set(port);
    } else {
        gpio_clear(port);
    }
}
int gpio_read(int port)
{
    return (gpio[GPLEV0] & (0x1 << port)) != 0;
}
