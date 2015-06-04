#ifdef __cplusplus__
extern "C" {
#endif

/*!
 * create a daemon process and write its pid to the file.
 *
 * @param [in] pidfilepath      The pid file pathname.
 *                              If NULL, it will never record.
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

#ifdef __cplusplus__
}
#endif
