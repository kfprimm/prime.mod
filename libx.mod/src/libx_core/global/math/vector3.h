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

#ifndef GLOBAL_MATH_VECTOR3_H
#define GLOBAL_MATH_VECTOR3_H

#include "../global.h"

namespace x { namespace math {

template <
	typename TypeT
>
class Vector3
{
public:
	inline Vector3()
	:	x(0), y(0), z(0)
	{
	}

	inline Vector3(const TypeT val_x, const TypeT& val_y, const TypeT& val_z)
	:	x(val_x), y(val_y), z(val_z)
	{
	}

	inline Vector3(const Vector3<TypeT>& rhs)
	:	x(rhs.x), y(rhs.y), z(rhs.z)
	{
	}

	inline const TypeT GetLenSqr() const
	{
		return x*x + y*y + z*z;
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
	}

	inline static Vector3<TypeT> ZERO()
	{
		return Vector3(0, 0, 0);
	}

	inline Vector3<TypeT> operator  * (const Vector3<TypeT>& rhs) const
	{
		return Vector3<TypeT>(x * rhs.x, y * rhs.y, z * rhs.z);
	}

	inline Vector3<TypeT> operator  * (TypeT scalar) const
	{
		return Vector3<TypeT>(x * scalar, y * scalar, z * scalar);
	}

	inline Vector3<TypeT> operator - (const Vector3<TypeT>& rhs) const
	{
		return Vector3<TypeT>(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	inline Vector3<TypeT> operator + (const Vector3<TypeT>& rhs) const
	{
		return Vector3<TypeT>(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	inline Vector3<TypeT> Cross(const Vector3<TypeT>& rhs) const
	{
		return Vector3<TypeT>(
			this->y * rhs.z - this->z * rhs.y, 
			this->z * rhs.x - this->x * rhs.z, 
			this->x * rhs.y - this->y * rhs.x
		);
	}

	inline const Vector3<TypeT>& operator += (const Vector3<TypeT>& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return (*this);
	}

	inline const bool_t operator == (const Vector3<TypeT>& rhs) const
	{
		return (x == rhs.x && y == rhs.y && z == rhs.z);
	}

	inline const bool_t operator != (const Vector3<TypeT>& rhs) const
	{
		return (x != rhs.x || y != rhs.y || z != rhs.z);
	}

	TypeT x;
	TypeT y;
	TypeT z;
};

typedef Vector3<float_t> Vector3f;
typedef Vector3<double_t> Vector3d;

}} // namespace x::math

#endif // GLOBAL_MATH_VECTOR3_H
