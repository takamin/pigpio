#ifdef __MINGW32__
/**
 * stub for MinGW
 */
#define LOG_PID 0
#define LOG_DAEMON 0
#define LOG_ERR 0
#define LOG_INFO 0
#endif
typedef int (*DAEMON_MAIN)(int argc, char* argv[], char* envp[]);
extern int daemonized;
void dlog(int priority, const char* format, ...);
int daemonize(const char* pidfilepath, const char *syslog_ident, int syslog_option, int syslog_facility);

