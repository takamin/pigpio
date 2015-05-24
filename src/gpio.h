//  レジスタブロックの物理アドレス
#define PERI_BASE     0x20000000
#define GPIO_BASE     (PERI_BASE + 0x200000)
#define BLOCK_SIZE    4096
//  ピン機能（BCM2835）
#define GPIO_INPUT    0x0       //  入力
#define GPIO_OUTPUT   0x1       //  出力
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

void gpio_init();

//  gpio_configure: ピン機能を設定する（ピンを使用する前に必ず設定）
//      pin : (P1) 2,3,4,7,8,9,10,11,14,15,17,18,22,23,24,25,27
//            (P5) 28,29,30,31
//      mode: GPIO_INPUT, _OUTPUT, _ALT0, _ALT1, _ALT2, _ALT3, _ALT4, _ALT5
void gpio_configure(int pin, int mode);

//  gpio_set/clear: ピンをセット (3.3V)，クリア (0V)
void gpio_set(int pin);
void gpio_clear(int pin);
int gpio_read(int pin);

//  gpio_configure_pull: プルアップ/ダウン抵抗の設定
//      pullmode: GPIO_PULLNONE/PULLDOWN/PULLUP
void gpio_configure_pull(int pin, int pullmode);

#ifdef __cplusplus
}
#endif

