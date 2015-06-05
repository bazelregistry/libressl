/*
 * Public domain
 * stdio.h compatibility shim
 */

#include_next <stdio.h>

#ifndef LIBCRYPTOCOMPAT_STDIO_H
#define LIBCRYPTOCOMPAT_STDIO_H

#ifndef HAVE_ASPRINTF
#include <stdarg.h>
int vasprintf(char **str, const char *fmt, va_list ap);
int asprintf(char **str, const char *fmt, ...);
#endif

#ifdef _WIN32
#include <errno.h>
#include <string.h>

static inline void
posix_perror(const char *s)
{
	fprintf(stderr, "%s: %s\n", s, strerror(errno));
}

#define perror(errnum) posix_perror(errnum)

static inline FILE *
posix_fopen(const char *path, const char *mode)
{
	char *bin_mode = mode;
	if (strchr(mode, 'b') == NULL) {
		bin_mode = NULL;
		if (asprintf(&bin_mode, "%sb", mode) == -1)
			return NULL;
		fprintf(stderr, "opening bin file %s\n", bin_mode);
	}

	FILE *f = fopen(path, bin_mode);
	if (bin_mode != mode)
		free(bin_mode);
	return f;
}

#define fopen(path, mode) posix_fopen(path, mode)

#endif

#endif
