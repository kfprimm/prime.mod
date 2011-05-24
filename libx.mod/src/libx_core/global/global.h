//-----------------------------------------------------------------------------------------------------------
/*
 *	LibX - A Library to export/import .x-models
 *
 *	This file is
 *		Copyright (C) 2008 Christian Oberholzer <coberholzer@gmx.ch>
 *
 *	It is part of LibX, which is
 *		Copyright (C) 2008 Christian Oberholzer <coberholzer@gmx.ch>
 *		Copyright (C) 2008 Basil Fierz <basil@gmx.ch>
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License version 2.1 as published by the Free Software Foundation.
 *	
 *	This library is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *	Lesser General Public License for more details.
 *	
 *	You should have received a copy of the GNU Lesser General Public
 *	License along with this library; if not, write to the Free Software
 *	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */
//-----------------------------------------------------------------------------------------------------------

#ifndef GLOBAL_GLOBAL_H
#define GLOBAL_GLOBAL_H

#include <string>
#include <vector>
#include <map>
#include <cmath> 

// Kevin was here
#include <string.h>
#include <climits>

#define UNICODE_STRING(x) x

namespace x {

using std::string;
using std::map;
using std::vector;
using std::pair;

typedef signed char byte_t;
typedef unsigned char ubyte_t;
typedef signed short word_t;
typedef unsigned short uword_t;
typedef signed int dword_t;
typedef unsigned int udword_t;
typedef float float_t;
typedef double double_t;

typedef bool bool_t;
typedef char schar_t;
typedef wchar_t swchar_t;
typedef signed char signed_char_t;
typedef unsigned char unsigned_char_t;
typedef signed short signed_short_t;
typedef unsigned short unsigned_short_t;
typedef signed int signed_int_t;
typedef unsigned int unsigned_int_t;
typedef unsigned int uint_t;
typedef signed long signed_long_t;
typedef unsigned long unsigned_long_t;
typedef float sfloat_t;
typedef double sdouble_t;
typedef string string_t;

} // namespace x

#define XLIB_EXPORT __declspec(dllexport)
#define XLIB_IMPORT __declspec(dllimport)

#include "./memory/memmanager.h"
#include "./exception.h"
#include "./notsupportedexception.h"
#include "./io/io.h"
#include "./math/color.h"
#include "./math/matrix3x3.h"
#include "./math/matrix4x4.h"
#include "./math/vector2.h"
#include "./math/vector3.h"
#include "./math/vector4.h"
#include "./math/quaternion.h"

#endif // GLOBAL_GLOBAL_H
