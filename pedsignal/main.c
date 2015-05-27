/*
 * GPIO ports definition
 *
 * Signal for cars
 *
 * GPIO_2 - Blue
 * GPIO_3 - Yellow
 * GPIO_4 - Red
 *
 * Signal for pedestrian
 *
 * GPIO_14 - Walk
 * GPIO_15 - Stop
 *
 * Message LED for pedestrian
 *
 * GPIO_17 - Push the button to abroad
 * GPIO_18 - Wait till the pedestrian signal turn to 'Walk'
 */
#include <assert.h>
#include <signal.h>
#include <unistd.h>
#include "gpio.h"
#include "gpio_port.h"

static volatile int signaled = 0;
static void handler(int sig)
{
	assert(sig == SIGINT
#ifndef __MINGW32__
            || sig == SIGHUP
#endif
            || sig == SIGTERM);
	signaled = sig;
}

#include <stdio.h>
#include <getopt.h>
#include <sys/types.h>
#ifndef __MINGW32__
#include <sys/wait.h>
#include <syslog.h>
#endif

#define FLAG_DAEMONIZE     1
#define FLAG_KILL          2

#ifdef __MINGW32__
#define LOG_PID 0
#define LOG_DAEMON 0
#define LOG_ERR 0
#define LOG_INFO 0
int openlog(const char* param0, int param1, int param2) {
    return 0;
}
int syslog(int logid, const char* msg) {
    return 0;
}
int daemon(int chdir, int closehandles) {
    return 0;
}
#endif

int main(int argc, char* argv[])
{
    /* http://pukiwiki.tuntunkun.com/index.php?Linux%2Fdaemon%20%A5%D7%A5%ED%A5%B0%A5%E9%A5%E0%A4%CE%BA%EE%C0%AE */
    /**
     * opt:
     *   コマンドラインオプションのコード
     *   command -h としたとき、'h' のコードが入る
     *   command --help としたときには、struct longopt 構造体の、
     *   第４メンバーが代入される
     * flag:
     *   フラグ
     * nochdir:
     *   フラグが 0 でなければカレントディレクトリを "/" (ルート) にしない
     * noclose:
     *   フラグが 0 でなければ 標準入力, 標準出力, 標準エラー を閉じない
     * pid_file_path:
     *   プロセス ID を記録するファイルへの絶対パス
     * pid_file:
     *  プロセス ID を記録するための、ファイルディスクリプタ
     * pid:
     *   プロセス ID を格納する変数
     */
    int opt;
    int flag = 0;
    int nochdir = 1;
    int noclose = 0;
    char *pid_file_path = NULL;
    FILE *pid_file;
    pid_t pid;

    /**
     * daemon[PID]: ログメッセージ
     * というように /var/log/messages に出力するようログを開く
     */
    openlog("daemon", LOG_PID, LOG_DAEMON);

    /**
     * コマンドライン引数のテーブル
     * 第一メンバ:
     *   オプション名 (--help の場合 "help"）
     * 第２メンバ:
     *   no_argument | required_argument | optional_argument
     *   それぞれ、値を必要としない、必要とする、オプションとなる
     * 第３メンバ:
     *   フラグセット用のポインタ
     *   基本的に使わないので、NULL にしておけばいい。
     * 第４引数:
     *   opt に代入されるコード (int 型)
     *   ショートオプション（-h のような形式）とおなじ 'h' としておくと楽
     */
    const struct option longopt[] = {
        {"help", no_argument, NULL, '?'},
        {"daemonize", no_argument, NULL, 'd'},
        {"kill", no_argument, NULL, 'k'},
        {"pidfile", required_argument, NULL, 'p'},
        {NULL, 0, NULL, 0}
    };

    /**
     * デーモン起動時のオプションをパース
     */
    while ((opt = getopt_long(argc, argv, "p:dk", longopt, NULL)) != -1) {
        switch (opt) {
            case 'p':
                pid_file_path = optarg;
                break;
            case 'd':
                flag = FLAG_DAEMONIZE;
                break;
            case 'k':
                flag = FLAG_KILL;
                break;
            default:
                break;
        }
    }

    /**
     * オプションが指定されなかった場合にエラーを表示し終了
     */
    if (pid_file_path == NULL) {
        printf("--pid-file required\n");
        return -1;
    }

    if (flag == FLAG_KILL) {
        pid_file = fopen(pid_file_path, "r");
        if (pid_file != NULL) {
            fscanf(pid_file, "%d\n", &pid);
            fclose(pid_file);
#ifndef __MINGW32__
            if (kill(pid, SIGKILL) == 0) {
                syslog(LOG_INFO, "daemon stopped.\n");
            }
#endif
        } else {
            syslog(LOG_ERR, "no daemon started.\n");
            return -1;
        }
        return 0;
    }

    /**
     * デーモンプロセスの立ち上げ
     */
    if (flag == FLAG_DAEMONIZE) {
        if (daemon(nochdir, noclose) == -1) {
            syslog(LOG_ERR, "failed to launch daemon.\n");
            return -1;
        }
    }

    syslog(LOG_INFO, "daemon started.\n");

    /**
     * getppid() を実行すると、1 が帰ってくるはず
     * daemon() の実行によって、fork() されたプロセスが、
     * いまここにいて、親プロセスは先に _exit() を呼び出して
     * 死んでしまっているので、init プロセスの養子になっている。
     * init プロセスは、Linux 起動時に最初に起動されるため PID は必ず 1
     */

    /*
     * このプロセス（デーモン）の PID を取得し、
     * ファイルに PID を記録
     */
    pid = getpid();
    pid_file = fopen(pid_file_path, "w+");
    if (pid_file != NULL) {
        fprintf(pid_file, "%d\n", pid);
        fclose(pid_file);
    } else {
        syslog(LOG_ERR, "failed to record process id to file.\n");
        return -1;
    }

#ifndef __MINGW32__
	if (signal(SIGHUP, handler) == SIG_ERR) {
		return 1;
	}
#endif
	if (signal(SIGINT, handler) == SIG_ERR) {
		return 2;
	}
	if (signal(SIGTERM, handler) == SIG_ERR) {
		return 3;
	}

	gpio_init();

	GPIO_PORT* GPIO_LIGHT_CAR_BLUE = gpio_port_output(2);
	GPIO_PORT* GPIO_LIGHT_CAR_YELLOW = gpio_port_output(3);
	GPIO_PORT* GPIO_LIGHT_CAR_RED = gpio_port_output(4);
	GPIO_PORT* GPIO_LIGHT_PED_WALK = gpio_port_output(14);
	GPIO_PORT* GPIO_LIGHT_PED_STOP = gpio_port_output(15);
	GPIO_PORT* GPIO_DISP_PED_PUSH = gpio_port_output(17);
	GPIO_PORT* GPIO_DISP_PED_WAIT = gpio_port_output(18);
	GPIO_PORT* GPIO_PED_BUTTON = gpio_port_input_pullup(27);
	
	while(!signaled) {
		int button_state = gpio_port_read(GPIO_PED_BUTTON);
		int test_output = (button_state == 0 ? 1 : 0);
		gpio_port_write(GPIO_LIGHT_CAR_BLUE, test_output);
		gpio_port_write(GPIO_LIGHT_CAR_YELLOW, test_output);
		gpio_port_write(GPIO_LIGHT_CAR_RED, test_output);
		gpio_port_write(GPIO_LIGHT_PED_WALK, test_output);
		gpio_port_write(GPIO_LIGHT_PED_STOP, test_output);
		gpio_port_write(GPIO_DISP_PED_PUSH, test_output);
		gpio_port_write(GPIO_DISP_PED_WAIT, test_output);
		usleep(10000);
	}
	gpio_port_write(GPIO_LIGHT_CAR_BLUE, 0);
	gpio_port_write(GPIO_LIGHT_CAR_YELLOW, 0);
	gpio_port_write(GPIO_LIGHT_CAR_RED, 0);
	gpio_port_write(GPIO_LIGHT_PED_WALK, 0);
	gpio_port_write(GPIO_LIGHT_PED_STOP, 0);
	gpio_port_write(GPIO_DISP_PED_PUSH, 0);
	gpio_port_write(GPIO_DISP_PED_WAIT, 0);
	return 0;
}
