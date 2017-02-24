#pragma once

#ifndef _WCONF_H_
#define _WCONF_H_

#ifdef UNICODE

#define _W(var) w##var

#ifndef _T
#define _T(x)	L##x
#endif

#else

#define _W(var) var

#ifndef _T
#define _T(x)	x
#endif

#endif


#endif