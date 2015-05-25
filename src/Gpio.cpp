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
#include "Gpio.hpp"
GpioPin::GpioPin()
: pin_no(-1)
{
}
GpioPin::~GpioPin() {
}
void GpioPin::setPinNo(int pin_no) {
	this->pin_no = pin_no;
	setMode(GPIO_INPUT);
}
void GpioPin::setMode(int mode, int pullMode/* =GPIO_PULLNONE */) {
	if(pin_no != -1) {
		gpio_configure(pin_no, mode);
        gpio_configure_pull(pin_no, pullMode);
	}
}
GpioPin::operator bool() {
	return (pin_no != -1) && ::gpio_read(pin_no) != 0;
}
GpioPin& GpioPin::operator = (bool value) {
	if(pin_no != -1) {
		if(value) {
			gpio_set(pin_no);
		} else {
			gpio_clear(pin_no);
		}
	}
	return *this;
}
#define P(n) (1 << n)
#define N(n) (0 << n)
static const unsigned port_bits = 
	N( 0)|N( 1)|P( 2)|P( 3)|
	P( 4)|N( 5)|N( 6)|P( 7)|
	P( 8)|P( 9)|P(10)|P(11)|
	N(12)|N(13)|P(14)|P(15)|
	N(16)|P(17)|P(18)|N(19)|
	N(20)|N(21)|P(22)|P(23)|
	P(24)|P(25)|N(26)|P(27)|
	N(28)|N(29)|N(30)|N(31);
bool RaspiGpio::exists_pin(int pin_no) {
	return  (port_bits & (1 << pin_no)) != 0;
}
RaspiGpio::RaspiGpio() {
	gpio_init();
	for(int i = 0; i < 32; i++) {
		if(RaspiGpio::exists_pin(i)) {
			gpio_pin[i].setPinNo(i);
		}
	}
}

RaspiGpio::~RaspiGpio() {
}
const GpioPin& RaspiGpio::operator[](int pin_no) const {
	return gpio_pin[pin_no];
}
GpioPin& RaspiGpio::operator[](int pin_no) {
	return gpio_pin[pin_no];
}

