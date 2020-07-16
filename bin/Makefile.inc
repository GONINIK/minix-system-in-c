.include <bsd.own.mk>		# for MKDYNAMICROOT definition

WARNS?=		5
BINDIR?=	/bin

.if (${MKDYNAMICROOT} == "no")
LDSTATIC?=	-static
.endif