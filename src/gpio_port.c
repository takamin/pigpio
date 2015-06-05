#include "gpio.h"
#include "gpio_port.h"

static GPIO_PORT ports[32];

void gpio_port_init( void )
{
	int i = 0;
	for(i = 0; i < sizeof(ports)/sizeof(ports[0]); i++) {
		ports[i].portnum = -1;
		ports[i].dir = -1;
		ports[i].pullup = -1;
	}
}
GPIO_PORT* gpio_port(int portnum, int dir, int pullup)
{
	GPIO_PORT* port = &ports[portnum];
	port->portnum = portnum;
	port->dir = dir;
	port->pullup = pullup;
	gpio_configure(port->portnum, port->dir);
    gpio_configure_pull(port->portnum, port->pullup);
	return port;
}
GPIO_PORT* gpio_port_output(int portnum)
{
	return gpio_port(portnum, GPIO_OUTPUT, GPIO_PULLNONE);
}
GPIO_PORT* gpio_port_input(int portnum, int pullup)
{
	return gpio_port(portnum, GPIO_INPUT, pullup);
}
GPIO_PORT* gpio_port_input_pulldown(int portnum)
{
	return gpio_port_input(portnum, GPIO_PULLDOWN);
}
GPIO_PORT* gpio_port_input_pullup(int portnum)
{
	return gpio_port_input(portnum, GPIO_PULLUP);
}
void gpio_port_write(GPIO_PORT* port, int value)
{
	gpio_write(port->portnum, value);
}
int gpio_port_read(GPIO_PORT* port)
{
	return gpio_read(port->portnum);
}
