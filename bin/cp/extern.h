#ifndef _EXTERN_H_
#define _EXTERN_H_

typedef struct {
	char *p_end;			/* pointer to NULL at end of path */
	char *target_end;		/* pointer to end of target base */
	char p_path[MAXPATHLEN + 1];	/* pointer to the start of a path */
} PATH_T;

extern PATH_T to;
extern uid_t myuid;
extern int Rflag, rflag, Hflag, Lflag, Pflag, fflag, iflag, lflag, pflag, Nflag;
extern mode_t myumask;
extern sig_atomic_t pinfo;

#include <sys/cdefs.h>

__BEGIN_DECLS
int	copy_fifo(struct stat *, int);
int	copy_file(FTSENT *, int);
int	copy_link(FTSENT *, int);
int	copy_special(struct stat *, int);
int	set_utimes(const char *, struct stat *);
int	setfile(struct stat *, int);
void	usage(void) __attribute__((__noreturn__));
__END_DECLS

#endif /* !_EXTERN_H_ */