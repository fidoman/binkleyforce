/*
 *	binkleyforce -- unix FTN mailer project
 *	
 *	Copyright (c) 1998-2000 Alexander Belkin, 2:5020/1398.11
 *	
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *	
 *	$Id$
 */

#include "includes.h"
#include "confread.h"
#include "version.h"
#include "logger.h"
#include "util.h"

/*
 * Local variables for logger
 */
static FILE *log_fp = NULL;
static char  log_name[BF_MAXPATH+1] = BFORCE_LOGFILE;
static char  log_extension[32] = "";
static char  log_ident[32] = "";
static char  log_ttyname[32] = "";

#ifdef DEBUG
/*
 * Local variables needed to make debug work
 */
static FILE *debug_fp = NULL;
static long  debug_current_debuglevel = 0L;
static char  debug_name[BF_MAXPATH+1] = BFORCE_DEBFILE;
static bool  debug_invalid_name = FALSE;

/*
 * Debug level names
 */
struct debuglevel {
	const char *keystr;
	unsigned long value;
} debuglevels[] = {
	{ "Config",   D_CONFIG   },
	{ "Override", D_OVERRIDE },
	{ "Event",    D_EVENT    },
	{ "Nodelist", D_NODELIST },
	{ "Outbound", D_OUTBOUND },
	{ "Info",     D_INFO     },
	{ "HShake",   D_HSHAKE   },
	{ "TtyIO",    D_TTYIO    },
	{ "Modem",    D_MODEM    },
	{ "Prot",     D_PROT     },
	{ "Freq",     D_FREQ     },
	{ "Statem",   D_STATEM   },
	{ "Daemon",   D_DAEMON   },
	{ "Full",     D_FULL     },
	{ NULL,       0          }
};
#endif /* DEBUG */

#ifdef USE_SYSLOG

#include <syslog.h>

#endif /* ifdef USE_SYSLOG */

#ifndef USE_SYSLOG
const char *log_getfilename(int whatfor)
{
	const char *p = NULL;
	
	switch(whatfor) {
	case LOG_FILE_DAEMON:
		if( (p = conf_string(cf_log_file_daemon)) == NULL )
			p = DAEMON_LOGFILE;
		break;
	case LOG_FILE_SESSION:
		if( (p = conf_string(cf_log_file)) == NULL )
			p = BFORCE_LOGFILE;
		break;
#ifdef DEBUG
	case LOG_FILE_DEBUG:
		if( (p = conf_string(cf_debug_file)) == NULL )
			p = BFORCE_DEBFILE;
		break;
#endif
	case LOG_FILE_HISTORY:
		break;
	}
	
	return p;
}

/*
 *  Logging part
 */
bool log_isopened(void)
{
	return log_fp ? TRUE : FALSE;
}

int log_open(const char *logname, const char *ext, const char *tty)
{
	if( log_fp )
		log("warning: opening new log file, but previous was not closed");
	
	if( logname )
	{
		/* Reset previous settings */
		*log_name = '\0';
		*log_extension = '\0';
		*log_ttyname = '\0';
		
		strnxcpy(log_name, logname, sizeof(log_name));
		
		if( tty && *tty )
			strnxcpy(log_ttyname, tty, sizeof(log_ttyname));
		
		if( ext && *ext )
		{
			strnxcpy(log_extension, ext, sizeof(log_extension));
			strnxcat(log_name, ".", sizeof(log_name));
			strnxcat(log_name, ext, sizeof(log_name));
		}
		
		if( (log_fp = fopen(log_name, "a")) == NULL )
		{
			logerr("can't open log file \"%s\"", log_name);
			return -1;
		}
	}
	else if( log_name )
	{
		/* Open previously set log file */
			
		if( (log_fp = fopen(log_name, "a")) == NULL )
		{
			logerr("can't open log file \"%s\"", log_name);
			return -1;
		}
	} else
		return -1;
	
	return 0;
}

int log_close(void)
{
	int rc = 0;

	ASSERT(log_fp);
	
	if( log_fp )
	{
		rc = fclose(log_fp); log_fp = NULL;
	}
	
	return rc;	
}

int log_reopen(const char *logname, const char *ext, const char *tty)
{
	ASSERT(logname != NULL);
	
	if( log_isopened() )
	{
		if( !strcmp(logname, log_name)
		 && !strcmp(log_extension ? log_extension : "", ext ? ext : "") )
		{
			return 0;
		}
		log_close();
	}
	
	return log_open(logname, ext, tty);
}

/* void log_setident(const char *ident)
{
	if( ident )
		strnxcpy(log_ident, ident, sizeof(log_ident));
	else
		*log_ident = '\0';
} */

int log(const char *s, ...)

{
	char timestr[40];
	va_list args;

	time_string_log(timestr, sizeof(timestr), 0);
	
	va_start(args, s);

	/*
	 * If log file is not opened print messages to the stderr
	 */ 
	if( log_fp == NULL )
	{
		fprintf(stderr, "%s ", timestr);
		vfprintf(stderr, s, args);
		putc('\n', stderr);
		fflush(stderr);
	}
	else
	{
		int mypid = getpid();
		
		if( log_ident[0] && log_ttyname[0] )
			fprintf(log_fp, "%s %s.%s[%d] ", timestr, log_ident, log_ttyname, mypid);
		else if( log_ident[0] )
			fprintf(log_fp, "%s %s[%d] ", timestr, log_ident, mypid);
		else if( log_ttyname[0] )
			fprintf(log_fp, "%s %s[%d] ", timestr, log_ttyname, mypid);
		else
			fprintf(log_fp, "%s [%d] ", timestr, mypid);
		
		vfprintf(log_fp, s, args);
		putc('\n', log_fp);
		fflush(log_fp);
	}
	
	va_end(args);

	return 0;
}

int logerr(const char *s, ...)
{
	char timestr[40];
	va_list args;

	time_string_log(timestr, sizeof(timestr), 0);
	
	va_start(args, s);
	
	/*
	 * If log file is not opened print messages to the stderr
	 */ 
	if( log_fp == NULL )
	{
		fprintf(stderr, "%s ", timestr);
		vfprintf(stderr, s, args);
		putc('\n', stderr);
		
		if( errno == EACCES )
		{
			fprintf(stderr, "\tmessage: \"%s\" : errno = %d (euid = %d, egid = %d)\n",
				strerror(errno), errno, geteuid(), getegid());
		}
		else
		{
			fprintf(stderr, "\tmessage: \"%s\" : errno = %d\n",
				strerror(errno), errno);
		}
		
		fflush(stderr);
	}
	else
	{
		int mypid = getpid();
		
		if( *log_ident && *log_ttyname )
			fprintf(log_fp, "%s %s.%s[%d] ", timestr, log_ident, log_ttyname, mypid);
		else if( *log_ident )
			fprintf(log_fp, "%s %s[%d] ", timestr, log_ident, mypid);
		else if( *log_ttyname )
			fprintf(log_fp, "%s %s[%d] ", timestr, log_ttyname, mypid);
		else
			fprintf(log_fp, "%s [%d] ", timestr, mypid);
		
		vfprintf(log_fp, s, args);
		putc('\n', log_fp);

		/*
		 * Print error message according to the errno value
		 */
		if( errno == EACCES )
		{
			fprintf(log_fp, "\tmessage: \"%s\" : errno = %d (euid = %d, egid = %d)\n",
				strerror(errno), errno, geteuid(), getegid());
		}
		else if( errno )
		{
			fprintf(log_fp, "\tmessage: \"%s\" : errno = %d\n",
				strerror(errno), errno);
		}
		
		fflush(log_fp);
	}
	
	va_end(args);

	return 0;
}

#endif /* ifndef USE_SYSLOG */


#ifdef USE_SYSLOG 

const char *log_getfilename(int whatfor)
{
     const char *p = "/dev/null";
     return p;
}

bool log_isopened(void)
{
     return TRUE;
}

int log_open(const char *logname, const char *ext, const char *tty)
{
     char *p = "bforce";
     int fac = conf_number(cf_syslog_facility);
     if( tty && *tty )
                strncpy(p, tty, sizeof(p));
     openlog(p, LOG_PID, fac);
     return 0;
}

int log_reopen(const char *logname, const char *ext, const char *tty)
{
     return 0;
}

int log_close(void)
{
     
     closelog();
     return 0;
     
}

int log(const char *s, ...)

{
	char timestr[40];
	va_list args;
	
	int fac = conf_number(cf_syslog_facility);
	const int LP_TTY = fac || LOG_INFO;
	const int LP_A = fac || LOG_NOTICE;
	const int LP_O = fac || LOG_DEBUG;	
	const int log_priority = fac || LOG_ERR;
	
	time_string_log(timestr, sizeof(timestr), 0);
	
	va_start(args, s);

	int mypid = getpid();
		
	if( log_ident[0] && log_ttyname[0] )
	     syslog(LP_TTY, "%s %s.%s[%d] ", timestr, log_ident, log_ttyname, mypid);
	else if( log_ident[0] )
	     syslog(LP_A, "%s %s[%d] ", timestr, log_ident, mypid);
	else if( log_ttyname[0] )
	     syslog(LP_O, "%s %s[%d] ", timestr, log_ttyname, mypid);
	else
	fprintf(log_priority, "%s [%d] ", timestr, mypid);

			
/*		vsyslog(log_priority, s, args);*/
/*		putc('\n', log_fp); 
		fflush(log_fp);

		} */
	
	va_end(args);

	return 0;
}

int logerr(const char *s, ...)

{
	char timestr[40];
	va_list args;

	int fac = conf_number(cf_syslog_facility);
	const int log_priority = fac || LOG_ERR;
	
	time_string_log(timestr, sizeof(timestr), 0);
	
	va_start(args, s);

	int mypid = getpid();
		
/*	if( log_ident[0] && log_ttyname[0] )
	     syslog(log_priority, "%s %s.%s[%d] ", timestr, log_ident, log_ttyname, mypid);
	else if( log_ident[0] )
	     syslog(log_priority, "%s %s[%d] ", timestr, log_ident, mypid);
	else if( log_ttyname[0] )
	     fprintf(log_priority, "%s %s[%d] ", timestr, log_ttyname, mypid);
	else
	fprintf(log_priority, "%s [%d] ", timestr, mypid);
*/
			
		vsyslog(log_priority, s, args);
/*		putc('\n', log_fp); 
		fflush(log_fp);

		} */
	
	va_end(args);

	return 0;
}

#endif /* ifdef USE_SYSLOG */


/*
 *  Debugging part
 */


#ifdef DEBUG
void debug_setlevel(long newlevel, bool logit)
{
	if( logit && newlevel != debug_current_debuglevel )
	{
		log("changing debug level from 0x%07x to 0x%07x",
			debug_current_debuglevel, newlevel);
		debug_current_debuglevel = newlevel;
	}
}

bool debug_isopened(void)
{
	return debug_fp ? TRUE : FALSE;
}

int debug_parsestring(char *str, unsigned long *deblevel)
{
	int i, rc = 0;
	char *n;
	char *p_str = NULL;

	ASSERT(str != NULL);
	
	*deblevel = 0L;
	
	for( p_str = string_token(str, &n, NULL, 0); p_str;
	     p_str = string_token(NULL, &n, NULL, 0) )
	{
		for( i = 0; debuglevels[i].keystr; i++ )
		{
			if( strcasecmp(p_str, debuglevels[i].keystr) == 0 )
			{
				*deblevel |= debuglevels[i].value;
				break;
			}
		}
		if( debuglevels[i].keystr == NULL )
		{
			log("unknown debug level \"%s\"", p_str);
			rc = 1;
		}
	}
	return rc;	
}

void debug_setfilename(const char *debugname)
{
	ASSERT(debugname != NULL);
	
	if( !strcmp(debug_name, debugname) ) return;
	
	if( debug_isopened() ) debug_close();
	
	/* Reset ignore flag */
	if( debug_invalid_name )
		debug_invalid_name = FALSE;
	
	strnxcpy(debug_name, debugname, sizeof(debug_name));
}

int debug_open(const char *debugname)
{
	char buf[40];

	ASSERT(debug_fp == NULL);

	if( debugname )
	{
		if( (debug_fp = fopen(debugname, "a")) == NULL )
		{
			logerr("can't open debug file \"%s\"", debugname);
			return -1;
		}
		strnxcpy(debug_name, debugname, sizeof(debug_name));
	}
	else if( debug_name )
	{
		if( (debug_fp = fopen(debug_name, "a")) == NULL )
		{
			/* Don't try to open it next time */
			debug_invalid_name = TRUE;
			
			logerr("can't open debug file \"%s\"", debug_name);
			return -1;
		}
	} else
		return -1;
	
	/* Put banner */
	fprintf(debug_fp, "****************************************************\n");
	fprintf(debug_fp, " Starting binkleyforce version %s\n", BF_VERSION);
	fprintf(debug_fp, " Time: %s, PID: %d, UID: %d, GID: %d, EUID: %d, EGID %d\n",
		time_string_long(buf, sizeof(buf), 0), getpid(),
		getuid(), getgid(), geteuid(), getegid());
	fprintf(debug_fp, "****************************************************\n");
	fflush(debug_fp);
	
	return 0;
}

int debug_close(void)
{
	char buf[40];
	int rc = 0;
	
	ASSERT(debug_fp != NULL);
	
	if( debug_fp )
	{
		fprintf(debug_fp, "****************************************************\n");
		fprintf(debug_fp, " Closing Binkley-Force debug file at %s\n",
			time_string_long(buf, sizeof(buf), 0));
		fprintf(debug_fp, "****************************************************\n");
		
		rc = fclose(debug_fp) ? 0 : -1;	debug_fp = NULL;
	}
	
	return rc;	
}

int debug(unsigned long what, const char *str, ...)
{
	char buf[40];
	va_list args;
	
	if( (what == 0 && debug_current_debuglevel)
	 || (debug_current_debuglevel & what) )
	{
		if( debug_fp == NULL && debug_invalid_name == FALSE )
		{
			debug_open(NULL);
		}
		
		if( debug_fp )
		{
			fprintf(debug_fp, "%s ", time_string_log(buf, sizeof(buf), 0));
			va_start(args, str);
			vfprintf(debug_fp, str, args);
			va_end(args);
			putc('\n', debug_fp);
			fflush(debug_fp);
		}
	}
	
	return 0;
}

#endif /* DEBUG */