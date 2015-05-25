/*  レジスタブロックの物理アドレス */
#define PERI_BASE     0x20000000
#define GPIO_BASE     (PERI_BASE + 0x200000)
#define BLOCK_SIZE    4096

/*  ピン機能（BCM2835） */
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
 * configure each GPIO port
 *
 * @param pin GPIO port number 2,3,4,7,8,9,10,11,14,15,17,18,22,23,24,25,27,28,29,30,31
 * @param mode GPIO_INPUT  入力ポートとする
 *             GPIO_OUTPUT 出力ポートとする
 *             GPIO_ALT0
 *             GPIO_ALT1
 *             GPIO_ALT2
 *             GPIO_ALT3
 *             GPIO_ALT4
 *             GPIO_ALT5
 */
void gpio_configure(int pin, int mode);

void gpio_set(int pin);
void gpio_clear(int pin);

/*!
 * write value to output GPIO port.
 *
 * @param pin pin number
 * @param value value to write. 0 or 1
 */
void gpio_write(int pin, int value);

/*!
 * read value to input GPIO port.
 *
 * @param pin pin number
 * @return value that read.
 */
int gpio_read(int pin);

/*!
 * pullup resistance to input GPIO port
 *
 * @param pullmode  GPIO_PULLNONE / GPIO_PULLDOWN / GPIO_PULLUP
 */
void gpio_configure_pull(int pin, int pullmode);

#ifdef __cplusplus
}
#endif

