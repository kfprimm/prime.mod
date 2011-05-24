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

#ifndef GLOBAL_MATH_MATRIX4X4_H
#define GLOBAL_MATH_MATRIX4X4_H

#include "../global.h"
#include "./vector3.h"

namespace x { namespace math {

template <
	typename TypeT
>
class Matrix4x4
{
public:
	Matrix4x4() 
	{ 
	}

	inline Matrix4x4(
		const TypeT _11, const TypeT _12, const TypeT _13, const TypeT _14,
		const TypeT _21, const TypeT _22, const TypeT _23, const TypeT _24,
		const TypeT _31, const TypeT _32, const TypeT _33, const TypeT _34,
		const TypeT _41, const TypeT _42, const TypeT _43, const TypeT _44
	)
	{
		this->_11 = _11;	this->_12 = _12;	this->_13 = _13;	this->_14 = _14;
		this->_21 = _21;	this->_22 = _22;	this->_23 = _23;	this->_24 = _24;
		this->_31 = _31;	this->_32 = _32;	this->_33 = _33;	this->_34 = _34;
		this->_41 = _41;	this->_42 = _42;	this->_43 = _43;	this->_44 = _44;
	}


	void Transpose()
	{
		Matrix4x4<TypeT> Copy;
		memcpy(&Copy, this, sizeof(TypeT)*16);

		_11 = Copy._11; _12 = Copy._21;  _13 = Copy._31; _14 = Copy._41;
		_21 = Copy._12; _22 = Copy._22;  _23 = Copy._32; _24 = Copy._42;
		_31 = Copy._13; _32 = Copy._23;  _33 = Copy._33; _34 = Copy._43;
		_41 = Copy._14; _42 = Copy._24;  _43 = Copy._34; _44 = Copy._44;
	}

	void ToIdentity()
	{
		_11 = 1.0; _12 = 0.0;  _13 = 0.0; _14 = 0.0;
		_21 = 0.0; _22 = 1.0;  _23 = 0.0; _24 = 0.0;
		_31 = 0.0; _32 = 0.0;  _33 = 1.0; _34 = 0.0;
		_41 = 0.0; _42 = 0.0;  _43 = 0.0; _44 = 1.0;
	}

	static const Matrix4x4<TypeT>& IDENTITY()
	{
		static Matrix4x4<TypeT> mat;
		mat.ToIdentity();
		return mat;
	}

	inline void ToTranslate(const Vector3<TypeT>& v)
	{
		_11 = 1.0f; _12 = 0.0f; _13 = 0.0f; _14 = v.x;
		_21 = 0.0f; _22 = 1.0f; _23 = 0.0f; _24 = v.y;
		_31 = 0.0f; _32 = 0.0f; _33 = 1.0f; _34 = v.z;
		_41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
	}

	inline void Mul(const Matrix4x4<TypeT>& mat1, const Matrix4x4<TypeT>& mat2)
	{
		_11 = (mat1._11*mat2._11 + mat1._12*mat2._21 + mat1._13*mat2._31 + mat1._14*mat2._41);
		_12 = (mat1._11*mat2._12 + mat1._12*mat2._22 + mat1._13*mat2._32 + mat1._14*mat2._42);
		_13 = (mat1._11*mat2._13 + mat1._12*mat2._23 + mat1._13*mat2._33 + mat1._14*mat2._43);
		_14 = (mat1._11*mat2._14 + mat1._12*mat2._24 + mat1._13*mat2._34 + mat1._14*mat2._44);

		_21 = (mat1._21*mat2._11 + mat1._22*mat2._21 + mat1._23*mat2._31 + mat1._24*mat2._41);
		_22 = (mat1._21*mat2._12 + mat1._22*mat2._22 + mat1._23*mat2._32 + mat1._24*mat2._42);
		_23 = (mat1._21*mat2._13 + mat1._22*mat2._23 + mat1._23*mat2._33 + mat1._24*mat2._43);
		_24 = (mat1._21*mat2._14 + mat1._22*mat2._24 + mat1._23*mat2._34 + mat1._24*mat2._44);

		_31 = (mat1._31*mat2._11 + mat1._32*mat2._21 + mat1._33*mat2._31 + mat1._34*mat2._41);
		_32 = (mat1._31*mat2._12 + mat1._32*mat2._22 + mat1._33*mat2._32 + mat1._34*mat2._42);
		_33 = (mat1._31*mat2._13 + mat1._32*mat2._23 + mat1._33*mat2._33 + mat1._34*mat2._43);
		_34 = (mat1._31*mat2._14 + mat1._32*mat2._24 + mat1._33*mat2._34 + mat1._34*mat2._44);

		_41 = (mat1._41*mat2._11 + mat1._42*mat2._21 + mat1._43*mat2._31 + mat1._44*mat2._41);
		_42 = (mat1._41*mat2._12 + mat1._42*mat2._22 + mat1._43*mat2._32 + mat1._44*mat2._42);
		_43 = (mat1._41*mat2._13 + mat1._42*mat2._23 + mat1._43*mat2._33 + mat1._44*mat2._43);
		_44 = (mat1._41*mat2._14 + mat1._42*mat2._24 + mat1._43*mat2._34 + mat1._44*mat2._44);
	}

	inline Matrix4x4<TypeT> operator * (const Matrix4x4<TypeT>& mat) const
	{
		Matrix4x4<TypeT> res;
		res.Mul(*this, mat);
		return res;
	}

	inline Vector3<TypeT> operator * (const Vector3<TypeT>& v) const
	{
		Vector3f res;
		res.x = _11 * v.x + _12 * v.y + _13 * v.z + _14;
		res.y = _21 * v.x + _22 * v.y + _23 * v.z + _24;
		res.z = _31 * v.x + _32 * v.y + _33 * v.z + _34;
		return res;
	}

	Matrix4x4<TypeT> GetInverse() const
	{
		TypeT fA0 = m_fData1[ 0]*m_fData1[ 5] - m_fData1[ 1]*m_fData1[ 4];
		TypeT fA1 = m_fData1[ 0]*m_fData1[ 6] - m_fData1[ 2]*m_fData1[ 4];
		TypeT fA2 = m_fData1[ 0]*m_fData1[ 7] - m_fData1[ 3]*m_fData1[ 4];
		TypeT fA3 = m_fData1[ 1]*m_fData1[ 6] - m_fData1[ 2]*m_fData1[ 5];
		TypeT fA4 = m_fData1[ 1]*m_fData1[ 7] - m_fData1[ 3]*m_fData1[ 5];
		TypeT fA5 = m_fData1[ 2]*m_fData1[ 7] - m_fData1[ 3]*m_fData1[ 6];
		TypeT fB0 = m_fData1[ 8]*m_fData1[13] - m_fData1[ 9]*m_fData1[12];
		TypeT fB1 = m_fData1[ 8]*m_fData1[14] - m_fData1[10]*m_fData1[12];
		TypeT fB2 = m_fData1[ 8]*m_fData1[15] - m_fData1[11]*m_fData1[12];
		TypeT fB3 = m_fData1[ 9]*m_fData1[14] - m_fData1[10]*m_fData1[13];
		TypeT fB4 = m_fData1[ 9]*m_fData1[15] - m_fData1[11]*m_fData1[13];
		TypeT fB5 = m_fData1[10]*m_fData1[15] - m_fData1[11]*m_fData1[14];

		TypeT fDet = fA0*fB5-fA1*fB4+fA2*fB3+fA3*fB2-fA4*fB1+fA5*fB0;
		if ((TypeT)fabs(fDet) <= 0.00000001)
		{
			Matrix4x4<TypeT> zero;
			memset(&zero, 0, sizeof(TypeT)*16);
			return zero;
		}

		Matrix4x4<TypeT> matInv;
		matInv.m_fData2[0][0] = + m_fData1[ 5]*fB5 - m_fData1[ 6]*fB4 + m_fData1[ 7]*fB3;
		matInv.m_fData2[1][0] = - m_fData1[ 4]*fB5 + m_fData1[ 6]*fB2 - m_fData1[ 7]*fB1;
		matInv.m_fData2[2][0] = + m_fData1[ 4]*fB4 - m_fData1[ 5]*fB2 + m_fData1[ 7]*fB0;
		matInv.m_fData2[3][0] = - m_fData1[ 4]*fB3 + m_fData1[ 5]*fB1 - m_fData1[ 6]*fB0;
		matInv.m_fData2[0][1] = - m_fData1[ 1]*fB5 + m_fData1[ 2]*fB4 - m_fData1[ 3]*fB3;
		matInv.m_fData2[1][1] = + m_fData1[ 0]*fB5 - m_fData1[ 2]*fB2 + m_fData1[ 3]*fB1;
		matInv.m_fData2[2][1] = - m_fData1[ 0]*fB4 + m_fData1[ 1]*fB2 - m_fData1[ 3]*fB0;
		matInv.m_fData2[3][1] = + m_fData1[ 0]*fB3 - m_fData1[ 1]*fB1 + m_fData1[ 2]*fB0;
		matInv.m_fData2[0][2] = + m_fData1[13]*fA5 - m_fData1[14]*fA4 + m_fData1[15]*fA3;
		matInv.m_fData2[1][2] = - m_fData1[12]*fA5 + m_fData1[14]*fA2 - m_fData1[15]*fA1;
		matInv.m_fData2[2][2] = + m_fData1[12]*fA4 - m_fData1[13]*fA2 + m_fData1[15]*fA0;
		matInv.m_fData2[3][2] = - m_fData1[12]*fA3 + m_fData1[13]*fA1 - m_fData1[14]*fA0;
		matInv.m_fData2[0][3] = - m_fData1[ 9]*fA5 + m_fData1[10]*fA4 - m_fData1[11]*fA3;
		matInv.m_fData2[1][3] = + m_fData1[ 8]*fA5 - m_fData1[10]*fA2 + m_fData1[11]*fA1;
		matInv.m_fData2[2][3] = - m_fData1[ 8]*fA4 + m_fData1[ 9]*fA2 - m_fData1[11]*fA0;
		matInv.m_fData2[3][3] = + m_fData1[ 8]*fA3 - m_fData1[ 9]*fA1 + m_fData1[10]*fA0;

		TypeT fInvDet = ((TypeT)1.0)/fDet;
		for (uint_t iRow = 0; iRow < 4; iRow++)
		{
			for (uint_t iCol = 0; iCol < 4; iCol++)
				matInv.m_fData2[iRow][iCol] *= fInvDet;
		}

		return matInv;
	}

public:
	union 
	{
		TypeT m_fData1[16];
		TypeT m_fData2[4][4];
		struct
		{
			TypeT _11, _12, _13, _14;
			TypeT _21, _22, _23, _24;
			TypeT _31, _32, _33, _34;
			TypeT _41, _42, _43, _44;
		};
	};
};

typedef Matrix4x4<float_t> Matrix4x4f;
typedef Matrix4x4<double_t> Matrix4x4d;

}} // namespace x::math

#endif // GLOBAL_MATH_MATRIX4X4_H
