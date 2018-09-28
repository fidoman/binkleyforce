/* include/config.h.  Generated from config.h.in by configure.  */
/* include/config.h.in.  Generated automatically from configure.in by autoheader.  */

/* Define to empty if the keyword does not work.  */
/* #undef const */

/* Define if the `getpgrp' function takes no argument.  */
#define GETPGRP_VOID 1

/* Define if you don't have vprintf but do have _doprnt.  */
/* #undef HAVE_DOPRNT */

/* Define if you have <sys/wait.h> that is POSIX.1 compatible.  */
#define HAVE_SYS_WAIT_H 1

/* Define if you have the vprintf function.  */
#define HAVE_VPRINTF 1

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef mode_t */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef pid_t */

/* Define as the return type of signal handlers (int or void).  */
#define RETSIGTYPE void

/* Define if the setvbuf function takes the buffering type as its second
   argument and the buffer pointer as the third, as on System V
   before release 3.  */
/* #undef SETVBUF_REVERSED */

/* Define to `unsigned' if <sys/types.h> doesn't define.  */
/* #undef size_t */

/* Define if you have the ANSI C header files.  */
#define STDC_HEADERS 1

/* Define if you can safely include both <sys/time.h> and <time.h>.  */
#define TIME_WITH_SYS_TIME 1

/* Define if your <sys/time.h> declares struct tm.  */
/* #undef TM_IN_SYS_TIME */

/* Do you want debug code to be compiled? */
#define DEBUG 1

/* Do you want use DCD line control? */
#define MODEM_WATCH_CARRIER 1

/* Do you want hangup to watch for modem DCD line? */
#define MODEM_HANGUP_WATCH_CARRIER 1

/* Version string */
#define RELEASE_VERSION "0.23"

/* Disable passwords logging? */
#define BFORCE_LOG_PASSWD 1

/* Path to the UUCP lock files directory */
#define BFORCE_LOCK_DIR "/var/lock/"

/* Do you want to use syslog? */
/* #undef USE_SYSLOG */

/* Netspool */
#define NETSPOOL 1

/* Do you want use .csy locks? */
#define BFORCE_USE_CSY 1

/* Define if you have the mkdir function.  */
#define HAVE_MKDIR 1

/* Define if you have the mktime function.  */
#define HAVE_MKTIME 1

/* Define if you have the rename function.  */
#define HAVE_RENAME 1

/* Define if you have the select function.  */
#define HAVE_SELECT 1

/* Define if you have the setproctitle function.  */
/* #undef HAVE_SETPROCTITLE */

/* Define if you have the socket function.  */
#define HAVE_SOCKET 1

/* Define if you have the statfs function.  */
#define HAVE_STATFS 1

/* Define if you have the statvfs function.  */
#define HAVE_STATVFS 1

/* Define if you have the strcasecmp function.  */
#define HAVE_STRCASECMP 1

/* Define if you have the strspn function.  */
#define HAVE_STRSPN 1

/* Define if you have the <dirent.h> header file.  */
#define HAVE_DIRENT_H 1

/* Define if you have the <fcntl.h> header file.  */
#define HAVE_FCNTL_H 1

/* Define if you have the <ndir.h> header file.  */
/* #undef HAVE_NDIR_H */

/* Define if you have the <sys/dir.h> header file.  */
/* #undef HAVE_SYS_DIR_H */

/* Define if you have the <sys/file.h> header file.  */
#define HAVE_SYS_FILE_H 1

/* Define if you have the <sys/ioctl.h> header file.  */
#define HAVE_SYS_IOCTL_H 1

/* Define if you have the <sys/mount.h> header file.  */
#define HAVE_SYS_MOUNT_H 1

/* Define if you have the <sys/ndir.h> header file.  */
/* #undef HAVE_SYS_NDIR_H */

/* Define if you have the <sys/param.h> header file.  */
#define HAVE_SYS_PARAM_H 1

/* Define if you have the <sys/select.h> header file.  */
#define HAVE_SYS_SELECT_H 1

/* Define if you have the <sys/statfs.h> header file.  */
#define HAVE_SYS_STATFS_H 1

/* Define if you have the <sys/statvfs.h> header file.  */
#define HAVE_SYS_STATVFS_H 1

/* Define if you have the <sys/termiox.h> header file.  */
/* #undef HAVE_SYS_TERMIOX_H */

/* Define if you have the <sys/time.h> header file.  */
#define HAVE_SYS_TIME_H 1

/* Define if you have the <sys/vfs.h> header file.  */
#define HAVE_SYS_VFS_H 1

/* Define if you have the <termios.h> header file.  */
#define HAVE_TERMIOS_H 1

/* Define if you have the <unistd.h> header file.  */
#define HAVE_UNISTD_H 1

/* Define if you want buggy emsi :) */
#define BUGGY_EMSI 0
