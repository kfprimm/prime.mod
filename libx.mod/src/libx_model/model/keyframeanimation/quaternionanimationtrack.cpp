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

//-----------------------------------------------------------------------------------------------------------
/*!	\file model/keyframeanimation/quaternionanimationtrack.cpp
	\brief Implementation von x::model::QuaternionAnimationTrack
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::model::QuaternionAnimationTrack
*/
//-----------------------------------------------------------------------------------------------------------
#include "./quaternionanimationtrack.h"

namespace x { namespace model {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
QuaternionAnimationTrack::QuaternionAnimationTrack()
:	AnimationTrack(),
	animated_quaternion_(NULL),
	keyframes_(NULL)
{
}





//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
QuaternionAnimationTrack::~QuaternionAnimationTrack()
{
	delete[] keyframes_;
}




math::Quaternionf lerp_quat(
	const math::Quaternionf& from, 
	const math::Quaternionf& to,
	const float_t weight_frame1
)
{
	math::Quaternionf ret = from * weight_frame1 + to * (1.0f - weight_frame1);
	//ret.Normalize();
	float_t norm = ret.GetNorm();
	if (norm != 0.0f)
	{
		ret.w /= norm;
		ret.x /= norm;
		ret.y /= norm;
		ret.z /= norm;
	}
	return ret;
}
/*float_t sfquat angleBetween(sfvec3f v1,sfvec3f v2) { 
	float d = sfvec3f.dot(v1,v2); 
	sfvec3f axis = v1;
	axis.cross(v2);
    float qw = (float)Math.sqrt(v1.len_squared()*v2.len_squared()) + d;
	if (qw < 0.0001) { // vectors are 180 degrees apart
		return (new sfquat(0,-v1.z,v1.y,v1.x)).norm;
	} 
	sfquat q= new sfquat(qw,axis.x,axis.y,axis.z); 
    return q.norm();
}*/
math::Quaternionf slerp_no_invert_quat(
	const math::Quaternionf& from,
	const math::Quaternionf& to,
	const float_t weight_frame1
)
{
	float_t pi = 3.1415926535897932384626433832795f;

	float_t fAngle;
	float_t fCos = from.Dot(to);
	if (fCos <= -1.0f)
		fAngle = pi;
	else if (fCos >= 1.0f)
		fAngle = 0.0f;
	else
		fAngle = acos(fCos);

	if (fabs(fAngle) >= 2.0f * pi / 360.0f)
	{
		float_t fSin = sinf(fAngle);
		float_t fInvSin = ((float_t)1.0)/fSin;
		float_t fCoeff0 = sinf(weight_frame1*fAngle)*fInvSin;
		float_t fCoeff1 = sinf((1.0f - weight_frame1)*fAngle)*fInvSin;
		return from*fCoeff0 + to*fCoeff1;
	}
	else
	{
		return from;
	}
}


float_t Quat_dotProduct (const math::Quaternionf& qa, const math::Quaternionf& qb)
{
  return ((qa.x * qb.x) + (qa.y * qb.y) + (qa.z * qb.z) + (qa.w * qb.w));
}

math::Quaternionf Quat_slerp (const math::Quaternionf& qa, const math::Quaternionf& qb, float t)
{
  /* Check for out-of range parameter and return edge points if so */
  if (t <= 0.0)
    {
      return qa;
    }

  if (t >= 1.0)
    {
      return qb;
    }

  /* Compute "cosine of angle between quaternions" using dot product */
  float_t cosOmega = Quat_dotProduct (qa, qb);

  /* If negative dot, use -q1.  Two quaternions q and -q
     represent the same rotation, but may produce
     different slerp.  We chose q or -q to rotate using
     the acute angle. */
  float q1w = qb.w;
  float q1x = qb.x;
  float q1y = qb.y;
  float q1z = qb.z;

  if (cosOmega < 0.0f)
    {
      q1w = -q1w;
      q1x = -q1x;
      q1y = -q1y;
      q1z = -q1z;
      cosOmega = -cosOmega;
    }

  /* We should have two unit quaternions, so dot should be <= 1.0 */
  //kaos_assert (cosOmega < 1.1f, "");

  /* Compute interpolation fraction, checking for quaternions
     almost exactly the same */
  float k0, k1;

  if (cosOmega > 0.9999f)
    {
      /* Very close - just use linear interpolation,
	 which will protect againt a divide by zero */

      k0 = 1.0f - t;
      k1 = t;
    }
  else
    {
      /* Compute the sin of the angle using the
	 trig identity sin^2(omega) + cos^2(omega) = 1 */
      float sinOmega = sqrt (1.0f - (cosOmega * cosOmega));

      /* Compute the angle from its sin and cosine */
      float omega = atan2 (sinOmega, cosOmega);

      /* Compute inverse of denominator, so we only have
	 to divide once */
      float oneOverSinOmega = 1.0f / sinOmega;

      /* Compute interpolation parameters */
      k0 = sin ((1.0f - t) * omega) * oneOverSinOmega;
      k1 = sin (t * omega) * oneOverSinOmega;
    }

  /* Interpolate and return new quaternion */
  return math::Quaternionf(
	  (k0 * qa.w) + (k1 * q1w),
	  (k0 * qa.x) + (k1 * q1x),
	  (k0 * qa.y) + (k1 * q1y),
	  (k0 * qa.z) + (k1 * q1z)
	  );
}




//-----------------------------------------------------------------------------------------------------------
void QuaternionAnimationTrack::Apply(
	const uint_t frame1,
	const uint_t frame2,
	const float_t weight_frame1
)
{
	if (frame1 == frame2)
	{
		animated_quaternion_->SetAnimatedQuaternion(keyframes_[frame1]);
	}
	else
	{
		math::Quaternionf interpolated;
		const math::Quaternionf& q1 = keyframes_[frame1];
		const math::Quaternionf& q2 = keyframes_[frame2];
		float_t weight_frame2 = 1.0f - weight_frame1;
		//interpolated = slerp_no_invert(q1, q2, weight_frame1);
		interpolated = Quat_slerp(q1, q2, weight_frame2);
		animated_quaternion_->SetAnimatedQuaternion(interpolated);
	}
}




//-----------------------------------------------------------------------------------------------------------
/// Das animierte Quaternion setzen
void QuaternionAnimationTrack::SetAnimatedQuaternion(
	AnimatedQuaternion* q
)
{
	animated_quaternion_ = q;
}




//-----------------------------------------------------------------------------------------------------------
/// Keyframes setzen
void QuaternionAnimationTrack::SetKeyFrames(
	math::Quaternionf* keyframes
)
{
	keyframes_ = keyframes;
}




//-----------------------------------------------------------------------------------------------------------
/// Den Typ des Tracks zurückgeben
const AnimationTrack::AnimationTrackType QuaternionAnimationTrack::GetAnimationTrackType() const
{
	return QuaternionAnimationTrackType;
}

}} // namespace x::model
