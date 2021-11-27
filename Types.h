#ifndef _TYPES_H_
#define _TYPES_H_

#ifdef __BEOS__
#include <SupportDefs.h>
#else

typedef	signed char				int8;
typedef unsigned char			uint8;
typedef volatile signed char   	vint8;
typedef volatile unsigned char	vuint8;

typedef	short					int16;
typedef unsigned short			uint16;
typedef volatile short			vint16;
typedef volatile unsigned short	vuint16;

typedef	long					int32;
typedef unsigned long			uint32;
typedef volatile long			vint32;
typedef volatile unsigned long	vuint32;

typedef	__int64						int64;
typedef unsigned __int64			uint64;
typedef volatile __int64			vint64;
typedef volatile unsigned __int64	vuint64;

typedef volatile long			vlong;
typedef volatile int			vint;
typedef volatile short			vshort;
typedef volatile char			vchar;

typedef volatile unsigned long	vulong;
typedef volatile unsigned int	vuint;
typedef volatile unsigned short	vushort;
typedef volatile unsigned char	vuchar;

typedef unsigned char			uchar;
typedef unsigned short          unichar;

#ifndef NULL
#define NULL 	(0)
#endif

#endif // __BEOS__
//------------------------------------------------------------------------------

#endif // _TYPES_H_
