/*!
 * flag that the process running as daemon.
 * This is same to damonize's return value.
 */
extern int daemonized;

/*!
 * daemonize a current process.
 * this calls daemon, initialize syslog and write daemon's pid
 * to the file specified by pidfilepath.
 *
 * @param [in] pidfilepath      A file pathname to be written
 *                              the daemon's process id.
 *                              If NULL is specified, the file
 *                              not created.
 * @param [in] syslog_ident     syslog ident.
 * @param [in] syslog_option    syslog option.
 * @param [in] syslog_facility  syslog facility.
 * @return this function        This returns 1 when daemonizing
 *                              is completed.
 *                              If this returns 0, it's in parent
 *                              process, because a child process
 *                              was not created.
 */
int daemonize(
        const char* pidfilepath,
        const char *syslog_ident,
        int syslog_option,
        int syslog_facility);

/*!
 * output log.
 * The log is write to syslog from the daemonized process or to
 * stderr from normal process.
 * 
 * @param [in] priority syslog priority values.
 * @param [in] format format string for syslog.
 * @arama [in] ... data values for the format.
 */
void dlog(int priority, const char* format, ...);

#ifdef __MINGW32__
/**
 * stub for MinGW
 */
#define LOG_PID 0
#define LOG_DAEMON 0
#define LOG_ERR 0
#define LOG_INFO 0
#endif

