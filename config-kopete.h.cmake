
/* Define if you have valgrind.h installed */
#cmakedefine HAVE_VALGRIND_H 1

/* Define if you have xslt libraries and header files */
#cmakedefine HAVE_XSLT 1

/* Define if you have xmms libraries and header files. */
#cmakedefine HAVE_XMMS 1

/* Define if you have the XScreenSaver extension */
#cmakedefine HAVE_XSCREENSAVER 1

/* Define if MSN webcam support can be enabled */
#define MSN_WEBCAM 1

/* Define to compile with GSM SMS support */
/* #undef INCLUDE_SMSGSM */

/* Define to 1 if you want IDN support. */
#define LIBIDN 1

/* Jingle support is enabled */
/* #undef SUPPORT_JINGLE */

/* enable support for the smpppd */
/* #undef USE_SMPPPD */

/* Define if you have a tm_gmtoff member in struct tm */
#define HAVE_TM_GMTOFF 1

/* Defined when we have ilbc codec lib */
#define HAVE_ILBC 1

/* Defines if you have GL (Mesa, OpenGL, ...) */
#define HAVE_GL 1

/* Glib is required for oRTP code and libmimic code */
#define HAVE_GLIB 1

/* TODO */
/* #undef HAVE_XSHM */

/* Define to 1 if you have the <inttypes.h> header file. */
#cmakedefine HAVE_INTTYPES_H 1

/* Define to 1 if you have the <stdint.h> header file. */
#cmakedefine HAVE_STDINT_H 1

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Define to 1 if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
#cmakedefine WORDS_BIGENDIAN ${CMAKE_WORDS_BIGENDIAN}

/* The size of a `long', as computed by sizeof. */
#define SIZEOF_LONG ${SIZEOF_LONG}

/* The size of a `unsigned long', as computed by sizeof. */
#define SIZEOF_UNSIGNED_LONG ${SIZEOF_UNSIGNED_LONG}

/* Define to 1 if you want libv4l support */
#cmakedefine HAVE_LIBV4L2 1
