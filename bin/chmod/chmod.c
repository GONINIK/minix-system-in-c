#include <sys/cdefs.h>
#ifndef lint
__COPYRIGHT(
"@(#) Copyright (c) 1989, 1993, 1994\
 The Regents of the University of California.  All rights reserved.");
#endif /* not lint */

#ifndef lint
#if 0
static char sccsid[] = "@(#)chmod.c	8.8 (Berkeley) 4/1/94";
#else
__RCSID("$NetBSD: chmod.c,v 1.38 2012/10/22 18:00:46 christos Exp $");
#endif
#endif /* not lint */

#include <sys/param.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <err.h>
#include <errno.h>
#include <fts.h>
#include <limits.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

__dead static void	usage(void);

struct option chmod_longopts[] = {
	{ "reference",		required_argument,	0,
						1 },
	{ NULL,			0,			0,
						0 },
};

int
main(int argc, char *argv[])
{
	FTS *ftsp;
	FTSENT *p;
	void *set;
	mode_t mval;
	int Hflag, Lflag, Rflag, ch, fflag, fts_options, hflag, rval;
	char *mode, *reference;
	int (*change_mode)(const char *, mode_t);

	setprogname(argv[0]);
	(void)setlocale(LC_ALL, "");

	Hflag = Lflag = Rflag = fflag = hflag = 0;
	reference = NULL;
	while ((ch = getopt_long(argc, argv, "HLPRXfghorstuwx",
	    chmod_longopts, NULL)) != -1)
		switch (ch) {
		case 1:
			reference = optarg;
			break;
		case 'H':
			Hflag = 1;
			Lflag = 0;
			break;
		case 'L':
			Lflag = 1;
			Hflag = 0;
			break;
		case 'P':
			Hflag = Lflag = 0;
			break;
		case 'R':
			Rflag = 1;
			break;
		case 'f':
			fflag = 1;
			break;
		case 'h':
			/*
			 * In System V the -h option causes chmod to
			 * change the mode of the symbolic link.
			 * 4.4BSD's symbolic links didn't have modes,
			 * so it was an undocumented noop.  In NetBSD
			 * 1.3, lchmod(2) is introduced and this
			 * option does real work.
			 */
			hflag = 1;
			break;
		/*
		 * XXX
		 * "-[rwx]" are valid mode commands.  If they are the entire
		 * argument, getopt has moved past them, so decrement optind.
		 * Regardless, we're done argument processing.
		 */
		case 'g': case 'o': case 'r': case 's':
		case 't': case 'u': case 'w': case 'X': case 'x':
			if (argv[optind - 1][0] == '-' &&
			    argv[optind - 1][1] == ch &&
			    argv[optind - 1][2] == '\0')
				--optind;
			goto done;
		case '?':
		default:
			usage();
		}
done:	argv += optind;
	argc -= optind;

	if (argc == 0 || (argc == 1 && reference == NULL))
		usage();

	fts_options = FTS_PHYSICAL;
	if (Rflag) {
		if (hflag) {
			errx(EXIT_FAILURE,
		"the -R and -h options may not be specified together.");
			/* NOTREACHED */
		}
		if (Hflag)
			fts_options |= FTS_COMFOLLOW;
		if (Lflag) {
			fts_options &= ~FTS_PHYSICAL;
			fts_options |= FTS_LOGICAL;
		}
	} else if (!hflag)
		fts_options |= FTS_COMFOLLOW;
	if (hflag)
		change_mode = lchmod;
	else
		change_mode = chmod;

	if (reference == NULL) {
		mode = *argv++;
		if ((set = setmode(mode)) == NULL) {
			err(EXIT_FAILURE, "Cannot set file mode `%s'", mode);
			/* NOTREACHED */
		}
		mval = 0;
	} else {
		struct stat st;

		if (stat(reference, &st) == -1)
			err(EXIT_FAILURE, "Cannot stat `%s'", reference);
		mval = st.st_mode;
		set = NULL;
	}

	if ((ftsp = fts_open(argv, fts_options, 0)) == NULL) {
		err(EXIT_FAILURE, "fts_open");
		/* NOTREACHED */
	}
	for (rval = 0; (p = fts_read(ftsp)) != NULL;) {
		switch (p->fts_info) {
		case FTS_D:
			if (!Rflag)
				(void)fts_set(ftsp, p, FTS_SKIP);
			break;
		case FTS_DNR:			/* Warn, chmod, continue. */
			warnx("%s: %s", p->fts_path, strerror(p->fts_errno));
			rval = 1;
			break;
		case FTS_DP:			/* Already changed at FTS_D. */
			continue;
		case FTS_ERR:			/* Warn, continue. */
		case FTS_NS:
			warnx("%s: %s", p->fts_path, strerror(p->fts_errno));
			rval = 1;
			continue;
		case FTS_SL:			/* Ignore. */
		case FTS_SLNONE:
			/*
			 * The only symlinks that end up here are ones that
			 * don't point to anything and ones that we found
			 * doing a physical walk.
			 */
			if (!hflag)
				continue;
			/* else */
			/* FALLTHROUGH */
		default:
			break;
		}
		if ((*change_mode)(p->fts_accpath,
		    set ? getmode(set, p->fts_statp->st_mode) : mval)
		    && !fflag) {
			warn("%s", p->fts_path);
			rval = 1;
		}
	}
	if (errno) {
		err(EXIT_FAILURE, "fts_read");
		/* NOTREACHED */
	}
	exit(rval);
	/* NOTREACHED */
}

static void
usage(void)
{
	(void)fprintf(stderr,
	    "Usage: %s [-R [-H | -L | -P]] [-fh] mode file ...\n"
	    "\t%s [-R [-H | -L | -P]] [-fh] --reference=rfile file ...\n",
	    getprogname(), getprogname());
	exit(1);
	/* NOTREACHED */
}