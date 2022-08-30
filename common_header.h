
/*------------------------------------------------------------------------------
#ifndef __COMMON_HEADER__
#define __COMMON_HEADER__
------------------------------------------------------------------------------*/
#pragma once
#define LINUX_PORT
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
#define FALSE 0
#define TRUE  (!FALSE)
#ifndef __cplusplus
#define _GNU_SOURCE
#endif
/*------------------------------------------------------------------------------
	C99 standard (ISO/IEC 9899:1999):
	C11 standard (ISO/IEC 9899:2011):

	#include <stdatomic.h> // (since C11)	Atomic types
	#include <threads.h> // (since C11)	Thread library
------------------------------------------------------------------------------*/
#include <assert.h>	//	Conditionally compiled macro that compares its argument to zero
#include <complex.h>	// (since C99)	Complex number arithmetic
#include <ctype.h>	//	Functions to determine the type contained in character data
#include <errno.h>	//	Macros reporting error conditions
#include <fenv.h>	// (since C99)	Floating-point environment
#include <float.h>	//	Limits of float types
#include <inttypes.h>	// (since C99)	Format conversion of integer types
#include <iso646.h>	// (since C95)	Alternative operator spellings
#include <limits.h>	//	Sizes of basic types
#include <locale.h>	//	Localization utilities
#include <math.h>	//	Common mathematics functions
#include <setjmp.h>	//	Nonlocal jumps
#include <signal.h>	//	Signal handling
#include <stdalign.h>	// (since C11)	alignas and alignof convenience macros
#include <stdarg.h>	//	Variable arguments
#include <stdbool.h>	// (since C99)	Boolean type
#include <stddef.h>	//	Common macro definitions
#include <stdint.h>	// (since C99)	Fixed-width integer types
#include <stdio.h>	//	Input/output
#include <stdlib.h>	//	General utilities: memory management, program utilities, string conversions, random numbers
#include <stdnoreturn.h>	// (since C11)	noreturn convenience macros
#include <string.h>	//	String handling
#include <tgmath.h>	// (since C99)	Type-generic math (macros wrapping math.h and complex.h)
#include <time.h>	//	Time/date utilities
#include <uchar.h>	// (since C11)	UTF-16 and UTF-32 character utilities
#include <wchar.h>	// (since C95)	Extended multibyte and wide character utilities
#include <wctype.h>	// (since C95)	Wide character classification and mapping utilities
/*------------------------------------------------------------------------------
	C POSIX library -- excluding those already in the C standard library
------------------------------------------------------------------------------*/
#include <aio.h>	//	Asynchronous input and output
#include <arpa/inet.h>	//	Functions for manipulating numeric IP addresses (part of Berkeley sockets)
#include <cpio.h>	//	Magic numbers for the cpio archive format.
#include <dirent.h>	//	Allows the opening and listing of directories.
#include <dlfcn.h>	//	Dynamic linking
#include <fcntl.h>	//	File opening, locking and other operations.
#include <fmtmsg.h>	//	Message display structures
#include <fnmatch.h>	//	Filename matching
#include <ftw.h>	//	File tree traversal
#include <glob.h>	//	Pathname "globbing" (pattern-matching)
#include <grp.h>	//	User group information and control.
#include <iconv.h>	//	Codeset conversion facility
#include <inttypes.h>	//	Fixed Size Integer Types
#include <iso646.h>	//	Alternative Spellings, see C alternative tokens
#include <langinfo.h>	//	Language information constants. Build on C localization functions
#include <libgen.h>	//	Pathname manipulation
#include <monetary.h>	//	String formatting of monetary units
#include <mqueue.h>	//	Message queue
#include <net/if.h>	//	Listing of local network interfaces
#include <netdb.h>	//	Translating protocol and host names into numeric addresses. Part of Berkeley sockets
#include <netinet/in.h>	//	Defines Internet protocol and address family. Part of Berkeley sockets
#include <netinet/tcp.h>	//	Additional TCP control options. Part of Berkeley sockets
#include <nl_types.h>	//	Localization message catalog functions
#include <poll.h>	//	Asynchronous file descriptor multiplexing
#include <pthread.h>	//	Defines an API for creating and manipulating POSIX threads.
#include <pwd.h>	//	passwd (user information) access and control.
#include <regex.h>	//	Regular expression matching
#include <sched.h>	//	Execution scheduling
#include <search.h>	//	Search tables
#include <semaphore.h>	//	POSIX semaphores
#include <spawn.h>	//	Process spawning
#include <stdarg.h>	//	Handle Variable Argument List
#include <strings.h>	//	Case-insensitive string comparisons
//#include <stropts.h>	//	obsolete Stream manipulation, including ioctl
#include <sys/ioctl.h>	//	ioctl
#include <sys/ipc.h>	//	Inter-process communication (IPC).
#include <sys/mman.h>	//	Memory management, including POSIX Shared Memory and Memory mapped files
#include <sys/msg.h>	//	POSIX message queues.
#include <sys/resource.h>	//	Resource usage, priorities, and limiting
#include <sys/select.h>	//	Synchronous I/O multiplexing
#include <sys/sem.h>	//	XSI (SysV style) semaphores
#include <sys/shm.h>	//	XSI (SysV style) Shared Memory
#include <sys/socket.h>	//	Main Berkley sockets header
#include <sys/stat.h>	//	File information (stat et al.).
#include <sys/statvfs.h>	//	File System information
#include <sys/time.h>	//	Time and date functions and structures.
#include <sys/times.h>	//	File access and modification times
#include <sys/types.h>	//	Various data types used elsewhere.
#include <sys/uio.h>	//	Vectored I/O operations
#include <sys/un.h>	//	Unix domain sockets
#include <sys/utsname.h>	//	Operating system information, including uname
#include <sys/wait.h>	//	Status of terminated child processes (see wait)
#include <syslog.h>	//	System error logging
#include <tar.h>	//	Magic numbers for the tar archive format.
#include <termios.h>	//	Allows terminal I/O interfaces.
#include <tgmath.h>	//	Type-Generic Macros, see C mathematical functions
#include <unistd.h>	//	Various essential POSIX functions and constants.
#include <utime.h>	//	inode access and modification times.
#include <utmpx.h>	//	user accounting database functions
#include <wordexp.h>	//	Word-expansion like the shell would perform
/*------------------------------------------------------------------------------
//#include <assert.h>	//	Verify Assumptions
//#include <complex.h>	//	Complex Arithmetic, see C mathematical functions
//#include <errno.h>	//	Retrieving Error Number
//#include <fenv.h>	//	Floating-Point Environment, see C mathematical functions
//#include <float.h>	//	Floating Types
//#include <limits.h>	//	Implementation-Defined Constants
//#include <locale.h>	//	Category Macros, see C localization functions
//#include <math.h>	//	Mathematical Declarations, see C mathematical functions
//#include <setjmp.h>	//	Stack Environment Declarations
//#include <signal.h>	//	Signals, see C signal handling
//#include <stdbool.h>	//	Boolean Type and Values
//#include <stddef.h>	//	Standard Type Definitions
//#include <stdint.h>	//	Integer Types
//#include <stdio.h>	//	Standard Buffered Input/Output, see C file input/output
//#include <stdlib.h>	//	Standard Library Definitions, see C standard library
//#include <string.h>	//	Several String Operations, see C string handling
//#include <time.h>	//	Type-Generic Macros, see C date and time functions
//#include <wchar.h>	//	Wide-Character Handling, see C string handling
//#include <wctype.h>	//	Wide-Character Classification and Mapping Utilities, see C character classification
------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------
	system
------------------------------------------------------------------------------*/
#include <ifaddrs.h>
#include <malloc.h>
#include <sys/ioctl.h>
#include <sys/sysinfo.h>
/*------------------------------------------------------------------------------
	X11
	http://www.x.org/releases/X11R7.7/doc/libX11/libX11/libX11.html#Standard_Header_Files
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/extensions/XInput2.h>
#include <X11/extensions/XInput.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>
------------------------------------------------------------------------------*/
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xcms.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>

/*------------------------------------------------------------------------------
	GNU and BSD
#include <bsd/string.h>
------------------------------------------------------------------------------*/
#include <mcheck.h>
#define str$cpy(dest, srce, len)			\
	__builtin_strncpy(dest, srce, len - 1);	\
	*(dest + len) = '\0';
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/

#ifndef _REENTRANT
#error "pthread needs re-entrancy -- compile with -D_REENTRANT option"
#endif
#define MKDIR(path) mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)
#define __FUNC__ __func__
#define RADIANS_TO_DEGREES(angle) (angle * 180.0 / M_PI)
#define GIGABYTE (1024L * 1024L * 1024L)
#define MEGABYTE (1024L * 1024L)
#define KILOBYTE (1024)
#define SQUARE(x) ((x) * (x))
#define GENERAL_STRING_LENGTH 2048

#define SGN(a)		(((a)<0) ? -1 : 1)
#define MIN(a,b)	(((a)<(b))?(a):(b))
#define MAX(a,b)	(((a)>(b))?(a):(b))
#define MAX3(a,b,c)	(a>b)?((a>c)?a:c):((b>c)?b:c)
#define CLEAR(x)	memset(&(x), 0, sizeof(x))

/*------------------------------------------------------------------------------

	https://en.wikipedia.org/wiki/ANSI_escape_code

	Control Sequence Introducer = \x1b[

	https://en.wikipedia.org/wiki/ANSI_escape_code

	CSI = "\x1b["

	CSI n K	EL – Erase in Line	Erases part of the line.
	If n is 0 (or missing), clear from cursor to the end of the line.
	If n is 1, clear from cursor to beginning of the line.
	If n is 2, clear entire line. Cursor position does not change.

	CSI n G	CHA – Cursor Horizontal Absolute
	Moves the cursor to column n (default 1).

#define ANSI_CLEAR_LINE "\x01B[J"

------------------------------------------------------------------------------*/
#define COLOR_ORANGE		"\x1b[38;2;255;127;0m"

#define ANSI_COLOR_RED		"\x1b[31m"
#define ANSI_COLOR_GREEN	"\x1b[32m"
#define ANSI_COLOR_YELLOW	"\x1b[33m"
#define ANSI_COLOR_BLUE		"\x1b[34m"
#define ANSI_COLOR_MAGENTA	"\x1b[35m"
#define ANSI_COLOR_CYAN		"\x1b[36m"

#define ANSI_COLOR_LIGHT_RED		"\x1b[91m"
#define ANSI_COLOR_LIGHT_GREEN		"\x1b[92m"
#define ANSI_COLOR_LIGHT_YELLOW		"\x1b[93m"
#define ANSI_COLOR_LIGHT_BLUE		"\x1b[94m"
#define ANSI_COLOR_LIGHT_MAGENTA	"\x1b[95m"
#define ANSI_COLOR_LIGHT_CYAN		"\x1b[96m"

#define ANSI_COLOR_RESET	"\x1b[0m"
#define ANSI_BLINK_ON		"\x1b[5m"
#define ANSI_BLINK_OFF		"\x1b[25m"
#define ANSI_BOLD_ON		"\x1b[1m"
#define ANSI_BOLD_OFF		"\x1b[21m"

#define ANSI_CURSOR_UP		"\x1b[1A"
#define ANSI_CURSOR_DOWN	"\x1b[1B"
#define ANSI_CURSOR_SAVE	"\x1b[1s"
#define ANSI_CURSOR_RESTORE	"\x1b[1u"
#define ANSI_CURSOR_RESET	"\x1b[1G"
/*------------------------------------------------------------------------------
Clears part of the screen.
If n is 0 (or missing), clear from cursor to end of screen.
If n is 1, clear from cursor to beginning of the screen.
If n is 2, clear entire screen.
If n is 3, clear entire screen and delete all lines saved in the scrollback buffer
------------------------------------------------------------------------------*/

#define ANSI_CLEAR_LINE		"\x1b[2K"
#define ANSI_CLEAR_SCREEN	"\x1b[2J"
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
#define free_null(pointer) if(pointer != NULL) free(((void *)pointer)); pointer = NULL;
/*------------------------------------------------------------------------------
// a terminated strncpy macro
#define strn$cpy(dest, srce, size)			\
	__builtin_strncpy(dest, srce, size);	\
	*(dest + size - 1) = '\0';

https://yarchive.net/comp/linux/strncpy.html
From: Linus Torvalds <torvalds@transmeta.com>
Subject: Re: Resend [PATCH] Make KOBJ_NAME_LEN match BUS_ID_SIZE
Original-Message-ID: <Pine.LNX.4.44.0305242045050.1666-100000@home.transmeta.com>
Date: Sun, 25 May 2003 03:54:06 GMT
I hate using snprintf() for this kind of mindless string copy opertation.

Yeah, "strncpy()" is a frigging disaster when it comes to '\0', in many
ways. We should probably disallow using strncpy(), and aim for a _sane_
implementation that does what we actually want (none of that zero-padding
crap, and _always_ put a NUL at the end). I bet that is what most current
strncpy() users actually would want.

static inline int strn$cpy(char *dest, const char *src, int len)
{
	int size;

	if(!len) return 0;
	size = strlen(src);
	if(size >= len) size = len-1;
	memcpy(dest, src, size);
	dest[size] = '\0';
	return size;
}

which is what pretty much everybody really _wants_ to have anyway? We
should deprecate "strncpy()" within the kernel entirely.

On Ubuntu, Mint, and relatives, strlcpy and strlcat are available in the
libbsd-dev package. Run
sudo apt-get install libbsd-dev
This will install the libraries, header files, and man pages.
To use the functions from C code, add the line
#include <bsd/string.h>
to your files, and
add -lbsd , or the more portable $(pkg-config --libs libbsd) ,
to your gcc command line to link the library.

------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------

https://man7.org/linux/man-pages/man3/strcpy.3.html

------------------------------------------------------------------------------*/
static inline size_t strn$cpy(char *dest, const char *src, size_t len)
{
	size_t count = 0;

	while(*src != '\0' && count < len)
	{
		*dest++ = *src++;
		count++;
	}

	*dest = '\0';
	return count;
}
static inline bool str$equ(char *string1, char *string2)
{
	return !strcmp(string1, string2);
}
/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
