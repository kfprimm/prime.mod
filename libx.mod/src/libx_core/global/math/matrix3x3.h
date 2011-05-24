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

#ifndef GLOBAL_MATH_MATRIX3X3_H
#define GLOBAL_MATH_MATRIX3X3_H

#include "../global.h"
#include "./matrix4x4.h"

namespace x { namespace math {

template <
	typename TypeT
>
class Matrix3x3
{
public:
	Matrix3x3() 
	{ 
	}

	void Transpose()
	{
		Matrix3x3<TypeT> copy;
		memcpy(&copy, this, sizeof(TypeT)*9);

		_11 = copy._11; _12 = copy._21;  _13 = copy._31;
		_21 = copy._12; _22 = copy._22;  _23 = copy._32;
		_31 = copy._13; _32 = copy._23;  _33 = copy._33;
	}

	void ToIdentity()
	{
		_11 = 1.0; _12 = 0.0;  _13 = 0.0;
		_21 = 0.0; _22 = 1.0;  _23 = 0.0;
		_31 = 0.0; _32 = 0.0;  _33 = 1.0;
	}

	inline Matrix3x3<TypeT> operator * (const Matrix3x3<TypeT>& rhs)
	{
		Matrix3x3<TypeT> retval;
		retval.Mul(*this, rhs);
		return retval;
	}

	inline void Mul(
		const Matrix3x3<TypeT>& lhs, 
		const Matrix3x3<TypeT>& rhs
	)
	{
		_11 = lhs._11 * rhs._11 + lhs._12 * rhs._21 + lhs._13 * rhs._31;
		_12 = lhs._11 * rhs._12 + lhs._12 * rhs._22 + lhs._13 * rhs._32;
		_13 = lhs._11 * rhs._13 + lhs._12 * rhs._23 + lhs._13 * rhs._33;

		_21 = lhs._21 * rhs._11 + lhs._22 * rhs._21 + lhs._23 * rhs._31;
		_22 = lhs._21 * rhs._12 + lhs._22 * rhs._22 + lhs._23 * rhs._32;
		_23 = lhs._21 * rhs._13 + lhs._22 * rhs._23 + lhs._23 * rhs._33;

		_31 = lhs._31 * rhs._11 + lhs._32 * rhs._21 + lhs._33 * rhs._31;
		_32 = lhs._31 * rhs._12 + lhs._32 * rhs._22 + lhs._33 * rhs._32;
		_33 = lhs._31 * rhs._13 + lhs._32 * rhs._23 + lhs._33 * rhs._33;
	}

	static const Matrix3x3<TypeT>& IDENTITY()
	{
		static Matrix3x3<TypeT> mat;
		mat.ToIdentity();
		return mat;
	}

	inline void ToZero()
	{
		_11 = _12 = _13 = 0.0f;
		_21 = _22 = _23 = 0.0f;
		_31 = _32 = _33 = 0.0f;
	}

	inline void ToScale(
		TypeT factor
	)
	{
		ToZero();

		_11 = factor;
		_22 = factor;
		_33 = factor;
	}

	inline void ToScale(
		const Vector3<TypeT>& v
	)
	{
		ToZero();

		_11 = v.x;
		_22 = v.y;
		_33 = v.z;
	}


	inline operator Matrix4x4<TypeT>()
	{
		Matrix4x4<TypeT> mat;
		mat._11 = _11;	mat._12 = _12;	mat._13 = _13;	mat._14 = 0.0;
		mat._21 = _21;	mat._22 = _22;	mat._23 = _23;	mat._24 = 0.0;
		mat._31 = _31;	mat._32 = _32;	mat._33 = _33;	mat._34 = 0.0;
		mat._41 = 0.0;	mat._42 = 0.0;	mat._43 = 0.0;	mat._44 = 1.0;
		return mat;
	}

public:
	union 
	{
		TypeT m_fData1[9];
		TypeT m_fData2[3][3];
		struct
		{
			TypeT _11, _12, _13;
			TypeT _21, _22, _23;
			TypeT _31, _32, _33;
		};
	};
};

typedef Matrix3x3<float_t> Matrix3x3f;
typedef Matrix3x3<double_t> Matrix3x3d;

}} // namespace x::math

#endif // GLOBAL_MATH_MATRIX3X3_H
