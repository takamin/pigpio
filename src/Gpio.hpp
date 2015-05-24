#include "gpio.h"

class GpioPin {
public:
	GpioPin();
	virtual ~GpioPin();
	void setPinNo(int pin_no);
	void setMode(int mode, int pullMode = GPIO_PULLNONE);
	operator bool();
	GpioPin& operator =(bool value);
private:
	int pin_no;
};
class RaspiGpio {
public:
	RaspiGpio();
	virtual ~RaspiGpio();
	const GpioPin& operator[](int pin_no) const;
	GpioPin& operator[](int pin_no);
private:
	static bool exists_pin(int pin_no);
private:
	GpioPin gpio_pin[32];
};
