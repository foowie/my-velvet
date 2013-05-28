/***************************************************************************}
{																			}
{    cGlobalDef.h                                     		      			}
{																			}
{																			}
{                 Copyright (c) 1998, 2004				Vaclav Snasel		}
{																			}
{    VERSION: 3.0										DATE 10/11/1998		}
{																			}
{             following functionality:										}
{																			}
{																			}
{    UPDATE HISTORY:														}
{																			}
{																			}
{***************************************************************************/

#ifndef __CGLOBALDEF_H__
#define __CGLOBALDEF_H__

/** prevent VS from babblign nonsense warnings about 'unsecure' sprintf etc. */
#define _CRT_SECURE_NO_WARNINGS 1
#define _CRT_SECURE_NO_DEPRECATE 1

#define DIM(x)	 (sizeof(x) / sizeof(x[0]))

#define CodeTable(Name, Type)	 {(unsigned int)Name, "" #Name "", Type}

typedef struct
{
	unsigned int Id;
	const char* Name;
	int Type;
} t_CodeTable, * pt_CodeTable;

typedef struct
{
	unsigned int Id;
	char* Name;
	unsigned int Value;
} t_CodeTableIdType, * pt_CodeTableIdType;

#ifdef __GNUC__
	#define strnicmp strncasecmp
	#define _stricmp strcasecmp
#endif

#define str(s) #s
#define joinstr(x,y) str(x ## y)

#if ((__GNUC__ * 100) + (__GNUC_MINOR__ * 10) + __GNUC_PATCHLEVEL__ ) >= 420
	# define GCC_DIAG_DO_PRAGMA(x) _Pragma (#x)
	# define GCC_DIAG_PRAGMA(x) GCC_DIAG_DO_PRAGMA(GCC diagnostic x)
	# if ((__GNUC__ * 100) + (__GNUC_MINOR__ * 10) + __GNUC_PATCHLEVEL__) >= 460
		#define WARNING_OFF(x,y) GCC_DIAG_PRAGMA(push) \
				GCC_DIAG_PRAGMA(ignored joinstr(-W,x))
		#define WARNING_ON(x) GCC_DIAG_PRAGMA(pop)
	#else
		#define WARNING_OFF(x,y) GCC_DIAG_PRAGMA(ignored joinstr(-W,x))
		#define WARNING_ON(x) GCC_DIAG_PRAGMA(warning joinstr(-W,x))
	#endif
#elif defined _MSC_VER
	#define MSVC_DO_PRAGMA(x) __pragma( x )
	#define WARNING_OFF(x,y) MSVC_DO_PRAGMA(warning(push)) \
							 MSVC_DO_PRAGMA(warning(disable: y))
	#define WARNING_ON(x) MSVC_DO_PRAGMA(warning(pop))
#else
	#define WARNING_OFF(x,y)
	#define WARNING_ON(x)
#endif

#endif	//		__CGLOBALDEF_H__
