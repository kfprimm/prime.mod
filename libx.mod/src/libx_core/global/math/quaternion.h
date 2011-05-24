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

#ifndef GLOBAL_MATH_QUATERNION_H
#define GLOBAL_MATH_QUATERNION_H

#include "../global.h"

namespace x { namespace math {

template <
	typename TypeT
>
class Quaternion
{
public:
	inline Quaternion()
	:	w(0), x(0), y(0), z(0)
	{
	}

	inline Quaternion(const TypeT val_w, const TypeT val_x, const TypeT val_y, const TypeT val_z)
	:	w(val_w), x(val_x), y(val_y), z(val_z)
	{
	}

	inline Quaternion(const Quaternion<TypeT>& rhs)
	:	w(rhs.w), x(rhs.x), y(rhs.y), z(rhs.z)
	{
	}

	inline static const Quaternion<TypeT> IDENTITY()
	{
		return Quaternion<TypeT>(1, 0, 0, 0);
	}

	inline const bool_t IsIdentity() const
	{
		return (x == 0 && y == 0 && z == 0 && w == 1);
	}

	inline const bool_t operator == (const Quaternion<TypeT>& rhs) const
	{
		return (w == rhs.w && x == rhs.x && y == rhs.y && z == rhs.z);
	}

	inline const bool_t operator != (const Quaternion<TypeT>& rhs) const
	{
		return (w != rhs.w || x != rhs.x || y != rhs.y || z != rhs.z);
	}

	inline void FromRotationMatrix(const Matrix3x3<TypeT>& mat)
	{
		// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
		// article "Quaternion Calculus and Fast Animation".

		TypeT fTrace = mat._11 + mat._22 + mat._33;
		TypeT fRoot;

		if (fTrace > 0.0f)
		{
			// |w| > 1/2, may as well choose w > 1/2
			fRoot = sqrtf(fTrace + 1.0);
			this->w = 0.5f * fRoot;
			fRoot = 0.5f / fRoot;
			this->x = (mat._32 - mat._23) * fRoot;
			this->y = (mat._13 - mat._31) * fRoot;
			this->z = (mat._21 - mat._12) * fRoot;
		}
		else
		{
			// |w| <= 1/2
			static int s_iNext[3] = { 1, 2, 0 };
			int i = 0;
			if (mat._22 > mat._11)
				i = 1;
			if (mat._33 > mat.m_fData2[i][i])
				i = 2;
			int j = s_iNext[i];
			int k = s_iNext[j];

			fRoot = sqrtf(mat.m_fData2[i][i] - mat.m_fData2[j][j] - mat.m_fData2[k][k] + 1.0f);
			TypeT* apkQuat[3] = { &this->x, &this->y, &this->z };
			*apkQuat[i] = 0.5f * fRoot;
			fRoot = 0.5f / fRoot;
			this->w = (mat.m_fData2[k][j] - mat.m_fData2[j][k]) * fRoot;
			*apkQuat[j] = (mat.m_fData2[j][i] + mat.m_fData2[i][j]) * fRoot;
			*apkQuat[k] = (mat.m_fData2[k][i] + mat.m_fData2[i][k]) * fRoot;
		}
	}

	inline void ToRotationMatrix(Matrix3x3<TypeT>& mat) const
	{
		TypeT f2x = 2.0f * this->x;
		TypeT f2y = 2.0f * this->y;
		TypeT f2z = 2.0f * this->z;

		TypeT f2wx = this->w * f2x;
		TypeT f2wy = this->w * f2y;
		TypeT f2wz = this->w * f2z;

		TypeT f2xx = f2x * this->x;
		TypeT f2yy = f2y * this->y;
		TypeT f2zz = f2z * this->z;

		TypeT f2xy = f2x * this->y;
		TypeT f2xz = f2x * this->z;
		TypeT f2yz = f2y * this->z;

		mat._11 = 1.0f - (f2yy + f2zz);
		mat._12 = f2xy - f2wz;
		mat._13 = f2xz + f2wy;
		
		mat._21 = f2xy + f2wz;
		mat._22 = 1.0f - (f2xx + f2zz);
		mat._23 = f2yz - f2wx;
		
		mat._31 = f2xz - f2wy;
		mat._32 = f2yz + f2wx;
		mat._33 = 1.0f - (f2xx + f2yy);
	}

	inline void Slerp(
		const Quaternion& lhs,
		const Quaternion& rhs,
		const TypeT t
	)
	{
		// This function is the milk and honey of our quaternion code, the rest of
		// the functions are an appendage to what is done here.  Everyone understands
		// the terms, "matrix to quaternion", "quaternion to matrix", "create quaternion matrix",
		// "quaternion multiplication", etc.. but "SLERP" is the stumbling block, even a little 
		// bit after hearing what it stands for, "Spherical Linear Interpolation".  What that
		// means is that we have 2 quaternions (or rotations) and we want to interpolate between 
		// them.  The reason what it's called "spherical" is that quaternions deal with a sphere.  
		// Linear interpolation just deals with 2 points primarily, where when dealing with angles
		// and rotations, we need to use sin() and cos() for interpolation.  If we wanted to use
		// quaternions for camera rotations, which have much more instant and jerky changes in 
		// rotations, we would use Spherical-Cubic Interpolation.  The equation for SLERP is this:
		//
		// q = (((b.a)^-1)^t)a
		//
		// Go here for an a detailed explanation and proofs of this equation:
		//
		// http://www.magic-software.com/Documentation/quat.pdf
		//
		// Now, Let's code it

		// Here we do a check to make sure the 2 quaternions aren't the same, return q1 if they are
		if(lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w) 
		{
			(*this) = lhs;
			return;
		}

		// Following the (b.a) part of the equation, we do a dot product between lhs and rhs.
		// We can do a dot product because the same math applied for a 3D vector as a 4D vector.
		TypeT result = (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z) + (lhs.w * rhs.w);

		// If the dot product is less than 0, the angle is greater than 90 degrees
		Quaternion rhscopy = rhs;
		if(result < 0.0f)
		{
			// Negate the second quaternion and the result of the dot product
			rhscopy = Quaternion(-rhs.x, -rhs.y, -rhs.z, -rhs.w);
			result = -result;
		}

		// Set the first and second scale for the interpolation
		TypeT scale0 = 1 - t, scale1 = t;

		// Next, we want to actually calculate the spherical interpolation.  Since this
		// calculation is quite computationally expensive, we want to only perform it
		// if the angle between the 2 quaternions is large enough to warrant it.  If the
		// angle is fairly small, we can actually just do a simpler linear interpolation
		// of the 2 quaternions, and skip all the complex math.  We create a "delta" value
		// of 0.1 to say that if the cosine of the angle (result of the dot product) between
		// the 2 quaternions is smaller than 0.1, then we do NOT want to perform the full on 
		// interpolation using.  This is because you won't really notice the difference.

		// Check if the angle between the 2 quaternions was big enough to warrant such calculations
		if(1 - result > 0.1f)
		{
			// Get the angle between the 2 quaternions, and then store the sin() of that angle
			TypeT theta = (TypeT)acos(result);
			TypeT sin_theta = (TypeT)sin(theta);

			// Calculate the scale for lhs and rhs, according to the angle and it's sine value
			scale0 = (TypeT)sin( ( 1 - t ) * theta) / sin_theta;
			scale1 = (TypeT)sin( ( t * theta) ) / sin_theta;
		}	

		// Calculate the x, y, z and w values for the quaternion by using a special
		// form of linear interpolation for quaternions.
		x = (scale0 * lhs.x) + (scale1 * rhscopy.x);
		y = (scale0 * lhs.y) + (scale1 * rhscopy.y);
		z = (scale0 * lhs.z) + (scale1 * rhscopy.z);
		w = (scale0 * lhs.w) + (scale1 * rhscopy.w);
	}

	inline TypeT Dot(const Quaternion<TypeT>& rhs) const
	{
		return (this->w * rhs.w + this->x * rhs.x + this->y * rhs.y + this->z * rhs.z);
	}

	inline Quaternion<TypeT> operator * (const Quaternion<TypeT>& Q1) const
	{
		const Quaternion<TypeT>& Q0 = (*this);
		return Quaternion<TypeT>(
			Q0.w*Q1.w - Q0.x*Q1.x - Q0.y*Q1.y - Q0.z*Q1.z,
			Q0.w*Q1.x + Q0.x*Q1.w + Q0.y*Q1.z - Q0.z*Q1.y,
			Q0.w*Q1.y - Q0.x*Q1.z + Q0.y*Q1.w + Q0.z*Q1.x,
			Q0.w*Q1.z + Q0.x*Q1.y - Q0.y*Q1.x + Q0.z*Q1.w
		);
	}

	inline TypeT GetNorm() const
	{
		return this->Dot(*this);
	}

	inline void Normalize()
	{
		TypeT norm = GetNorm();
		if (norm != 0.0f)
		{
			w /= norm;
			x /= norm;
			y /= norm;
			z /= norm;
		}
	}

	inline Quaternion<TypeT> operator * (const TypeT s) const
	{
		return Quaternion<TypeT>(w * s, x * s, y * s, z * s);
	}

	inline Quaternion<TypeT> operator + (const Quaternion<TypeT>& Q1) const
	{
		const Quaternion<TypeT>& Q0 = (*this);
		return Quaternion<TypeT>(Q0.w + Q1.w, Q0.x + Q1.x, Q0.y + Q1.y, Q0.z + Q1.z);
	}


	TypeT w;
	TypeT x;
	TypeT y;
	TypeT z;
};

typedef Quaternion<float_t> Quaternionf;
typedef Quaternion<double_t> Quaterniond;

}} // namespace x::math

#endif // GLOBAL_MATH_QUATERNION_H
