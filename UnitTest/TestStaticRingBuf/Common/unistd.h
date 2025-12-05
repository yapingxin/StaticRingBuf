#ifndef _INC_COMMON_unistd_H
#define _INC_COMMON_unistd_H

/* /////////////////////////////////////////////////////////////////////////
 * ISO C and C++ conformant named API functions
 */

#include <io.h> /* _access() */
#ifndef access
#   define access _access
#endif

#ifndef strncasecmp
#   define strncasecmp _strnicmp
#endif

#ifndef strcasecmp
#   define strcasecmp _stricmp
#endif

#ifndef strnicmp
#   define strnicmp _strnicmp
#endif

#ifndef strtok_r
#   define strtok_r strtok_s
#endif

#ifndef getcwd
#   define getcwd _getcwd
#endif

#ifndef stricmp
#   define stricmp _stricmp
#endif

#ifndef unlink
#   define unlink _unlink
#endif

#ifndef strdup
#   define strdup _strdup
#endif

#endif // !_INC_COMMON_unistd_H
