/* register block phisical address */
#define PERI_BASE     0x20000000
#define GPIO_BASE     (PERI_BASE + 0x200000)
#define BLOCK_SIZE    4096

/* port function (BCM2835) */
#define GPIO_INPUT    0x0
#define GPIO_OUTPUT   0x1
#define GPIO_ALT0     0x4
#define GPIO_ALT1     0x5
#define GPIO_ALT2     0x6
#define GPIO_ALT3     0x7
#define GPIO_ALT4     0x3
#define GPIO_ALT5     0x2

#define GPIO_PULLNONE 0x0
#define GPIO_PULLDOWN 0x1
#define GPIO_PULLUP   0x2

#ifdef __cplusplus
extern "C" { 
#endif

/*!
 * initialize GPIO
 */
void gpio_init();

/*!
 * configure GPIO port
 *
 * @param [in] port GPIO port number:
 *                  2,3,4,
 *                  7,8,9,10,11,
 *                  14,15,17,18,
 *                  22,23,24,25,
 *                  27,28,29,30,31
 * @param [in] mode port mode:
 *                  GPIO_INPUT, GPIO_OUTPUT,
 *                  GPIO_ALT0, GPIO_ALT1, GPIO_ALT2,
 *                  GPIO_ALT3, GPIO_ALT4, GPIO_ALT5
 */
void gpio_configure(int port, int mode);

/*!
 * set pullup resistance state for input GPIO port
 *
 * @param [in] pullmode  GPIO_PULLNONE / GPIO_PULLDOWN / GPIO_PULLUP
 */
void gpio_configure_pull(int port, int pullmode);

/*!
 * set GPIO output port.
 *
 * @param [in] port port number
 */
void gpio_set(int port);

/*!
 * clear GPIO output port.
 *
 * @param [in] port port number
 */
void gpio_clear(int port);

/*!
 * write value to output GPIO port.
 *
 * @param [in] port port number
 * @param [in] value value to write. 0 or 1
 */
void gpio_write(int port, int value);

/*!
 * read value to input GPIO port.
 *
 * @param [in] port port number
 * @return [in] value that read.
 */
int gpio_read(int port);

#ifdef __cplusplus
}
#endif
