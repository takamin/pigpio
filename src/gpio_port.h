#ifdef __cplusplus
extern "C" { 
#endif

typedef struct gpio_port_t {
	int portnum;
	int dir;
	int pullup;
} GPIO_PORT;

void gpio_port_init( void );
GPIO_PORT* gpio_port(int portnum, int dir, int pullup);
GPIO_PORT* gpio_port_output(int portnum);
GPIO_PORT* gpio_port_input(int portnum, int pullup);
GPIO_PORT* gpio_port_input_pulldown(int portnum);
GPIO_PORT* gpio_port_input_pullup(int portnum);
void gpio_port_write(GPIO_PORT* port, int value);
int gpio_port_read(GPIO_PORT* port);

#ifdef __cplusplus
}
#endif
