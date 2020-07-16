#ifndef _WCTYPE_H_
#define	_WCTYPE_H_

#include <sys/cdefs.h>
#include <sys/featuretest.h>
#include <sys/ansi.h>

#ifdef	_BSD_WINT_T_
typedef	_BSD_WINT_T_    wint_t;
#undef	_BSD_WINT_T_
#endif

#ifdef	_BSD_WCTRANS_T_
typedef	_BSD_WCTRANS_T_	wctrans_t;
#undef	_BSD_WCTRANS_T_
#endif

#ifdef	_BSD_WCTYPE_T_
typedef	_BSD_WCTYPE_T_	wctype_t;
#undef	_BSD_WCTYPE_T_
#endif

#ifndef WEOF
#define	WEOF	((wint_t)-1)
#endif

__BEGIN_DECLS
int	iswalnum(wint_t);
int	iswalpha(wint_t);
int	iswblank(wint_t);
int	iswcntrl(wint_t);
int	iswdigit(wint_t);
int	iswgraph(wint_t);
int	iswlower(wint_t);
int	iswprint(wint_t);
int	iswpunct(wint_t);
int	iswspace(wint_t);
int	iswupper(wint_t);
int	iswxdigit(wint_t);
int	iswctype(wint_t, wctype_t);
wint_t	towctrans(wint_t, wctrans_t);
wint_t	towlower(wint_t);
wint_t	towupper(wint_t);
wctrans_t wctrans(const char *);
wctype_t wctype(const char *);

#if (_POSIX_C_SOURCE - 0) >= 200809L || defined(_NETBSD_SOURCE)
#  ifndef __LOCALE_T_DECLARED
typedef struct _locale		*locale_t;
#  define __LOCALE_T_DECLARED
#  endif
int	iswalnum_l(wint_t, locale_t);
int	iswalpha_l(wint_t, locale_t);
int	iswblank_l(wint_t, locale_t);
int	iswcntrl_l(wint_t, locale_t);
int	iswdigit_l(wint_t, locale_t);
int	iswgraph_l(wint_t, locale_t);
int	iswlower_l(wint_t, locale_t);
int	iswprint_l(wint_t, locale_t);
int	iswpunct_l(wint_t, locale_t);
int	iswspace_l(wint_t, locale_t);
int	iswupper_l(wint_t, locale_t);
int	iswxdigit_l(wint_t, locale_t);
int	iswctype_l(wint_t, wctype_t, locale_t);
wint_t	towctrans_l(wint_t, wctrans_t, locale_t);
wint_t	towlower_l(wint_t, locale_t);
wint_t	towupper_l(wint_t, locale_t);
wctrans_t wctrans_l(const char *, locale_t);
wctype_t wctype_l(const char *, locale_t);
#endif
__END_DECLS

#endif		/* _WCTYPE_H_ */