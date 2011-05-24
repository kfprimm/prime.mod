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
/*!	\file model/keyframeanimation/vectoranimationtrack.cpp
	\brief Implementation von x::model::VectorAnimationTrack
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::model::VectorAnimationTrack
*/
//-----------------------------------------------------------------------------------------------------------
#include "./vectoranimationtrack.h"

namespace x { namespace model {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
VectorAnimationTrack::VectorAnimationTrack()
:	AnimationTrack(),
	animated_vector_(NULL),
	keyframes_(NULL)
{
}





//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
VectorAnimationTrack::~VectorAnimationTrack()
{
	delete[] keyframes_;
}




//-----------------------------------------------------------------------------------------------------------
void VectorAnimationTrack::Apply(
	const uint_t frame1,
	const uint_t frame2,
	const float_t weight_frame1
)
{
	if (frame1 == frame2)
	{
		animated_vector_->SetAnimatedVector3(keyframes_[frame1]);
	}
	else
	{
		math::Vector3f res;
		const math::Vector3f& v1 = keyframes_[frame1];
		const math::Vector3f& v2 = keyframes_[frame2];
		float_t weight_frame2 = 1.0f - weight_frame1;
		res = (v1 * weight_frame1) + (v2 * weight_frame2);
		animated_vector_->SetAnimatedVector3(res);
	}
}





//-----------------------------------------------------------------------------------------------------------
/// Das animierte Quaternion setzen
void VectorAnimationTrack::SetAnimatedVector(
	AnimatedVector3* v
)
{
	animated_vector_ = v;
}




//-----------------------------------------------------------------------------------------------------------
/// Keyframes setzen
void VectorAnimationTrack::SetKeyFrames(
	math::Vector3f* keyframes
)
{
	keyframes_ = keyframes;
}




//-----------------------------------------------------------------------------------------------------------
/// Den Typ des Tracks zurückgeben
const AnimationTrack::AnimationTrackType VectorAnimationTrack::GetAnimationTrackType() const
{
	return Vector3AnimationTrackType;
}

}} // namespace x::model
