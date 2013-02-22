// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef MK_MACROS
#define MK_MACROS

#include "gctypes.h"

// Pour manipuler des couleurs
static inline u32 MK_MAKE_RGBA(u8 r, u8 g, u8 b, u8 a) {
	return (u32)((u32)r << 24 | ((u32)g << 16) | ((u32)b << 8) | (u32)(a) );
}

#endif