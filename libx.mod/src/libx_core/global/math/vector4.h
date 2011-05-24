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

#ifndef GLOBAL_MATH_VECTOR4_H
#define GLOBAL_MATH_VECTOR4_H

#include "../global.h"

namespace x { namespace math {

template <
	typename TypeT
>
class Vector4
{
public:
	inline Vector4()
	:	x(0), y(0), z(0), w(0)
	{
	}

	inline Vector4(const TypeT val_x, const TypeT& val_y, const TypeT& val_z, const TypeT& val_w)
	:	x(val_x), y(val_y), z(val_z), w(val_w)
	{
	}

	inline Vector4(const Vector4<TypeT>& rhs)
	:	x(rhs.x), y(rhs.y), z(rhs.z), w(rhs.w)
	{
	}

	inline const TypeT GetLenSqr() const
	{
		return x*x + y*y + z*z + w*w;
	}

	inline const TypeT GetLen() const
	{
		return sqrt(GetLenSqr());
	}

	inline void Normalize()
	{
		TypeT len = GetLen();
		x /= len;
		y /= len;
		z /= len;
		w /= len;
	}

	inline static Vector4<TypeT> ZERO()
	{
		return Vector4(0, 0, 0, 0);
	}

	inline Vector4<TypeT> operator  * (const Vector4<TypeT>& rhs) const
	{
		return Vector4<TypeT>(x * rhs.x, y * rhs.y, z * rhs.z, w * rhs.w);
	}

	inline Vector4<TypeT> operator  * (TypeT scalar) const
	{
		return Vector4<TypeT>(x * scalar, y * scalar, z * scalar, w * scalar);
	}

	inline Vector4<TypeT> operator - (const Vector4<TypeT>& rhs) const
	{
		return Vector4<TypeT>(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}

	inline Vector4<TypeT> operator + (const Vector4<TypeT>& rhs) const
	{
		return Vector4<TypeT>(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}

	inline const Vector4<TypeT>& operator += (const Vector4<TypeT>& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;
		return (*this);
	}

	inline const bool_t operator == (const Vector4<TypeT>& rhs) const
	{
		return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w);
	}

	inline const bool_t operator != (const Vector4<TypeT>& rhs) const
	{
		return (x != rhs.x || y != rhs.y || z != rhs.z || w != rhs.w);
	}

	inline TypeT& operator[] (const uint_t index)
	{
		switch (index) 
		{
		case 0:	return x;
		case 1: return y;
		case 2: return z;
		default: return w;
		}
	}

	inline const TypeT& operator[] (const uint_t index) const
	{
		switch (index) 
		{
		case 0:	return x;
		case 1: return y;
		case 2: return z;
		default: return w;
		}
	}

	TypeT x;
	TypeT y;
	TypeT z;
	TypeT w;
};

typedef Vector4<float_t> Vector4f;
typedef Vector4<double_t> Vector4d;

}} // namespace x::math

#endif // GLOBAL_MATH_VECTOR4_H
