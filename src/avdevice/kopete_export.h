#ifndef KOPETEEXPORT_H
#define KOPETEEXPORT_H

#include <kdemacros.h>

#include <kdemacros.h>
#ifndef KOPETE_EXPORT
# ifdef MAKE_KOPETE_LIB
#  define KOPETE_EXPORT KDE_EXPORT
# else
#  define KOPETE_EXPORT KDE_IMPORT
# endif
#endif


#endif

