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
/*!	\file model/keyframeanimation/vector3streamanimationtrack.cpp
	\brief Implementation von x::model::Vector3StreamAnimationTrack
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::model::Vector3StreamAnimationTrack
*/
//-----------------------------------------------------------------------------------------------------------
#include "./animation.h"
#include "./vector3streamanimationtrack.h"

namespace x { namespace model {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
Vector3StreamAnimationTrack::Vector3StreamAnimationTrack()
:	AnimationTrack(),
	animated_entity_(NULL),
	animated_stream_(NULL),
	keyframe_count_(0),
	keyframes_(NULL)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
Vector3StreamAnimationTrack::~Vector3StreamAnimationTrack()
{
	for (uint_t i = 0; i < keyframe_count_; ++i)
	{
		delete keyframes_[i];
	}
	delete[] keyframes_;
}




//-----------------------------------------------------------------------------------------------------------
/// Die animierte Entity setzen
void Vector3StreamAnimationTrack::SetAnimatedEntity(
	Entity* animated_entity
)
{
	animated_entity_ = animated_entity;
}




//-----------------------------------------------------------------------------------------------------------
/// Den animierten Stream setzen
void Vector3StreamAnimationTrack::SetAnimatedStream(
	Stream* animated_stream
)
{
	animated_stream_ = animated_stream;
}




//-----------------------------------------------------------------------------------------------------------
/// Die Keyframes setzen
void Vector3StreamAnimationTrack::SetKeyFrames(
	const uint_t keyframe_count,
	KeyFrameVector3** keyframes
)
{
	keyframe_count_ = keyframe_count;
	keyframes_ = keyframes;
}




//-----------------------------------------------------------------------------------------------------------
void Vector3StreamAnimationTrack::Apply(
	const uint_t frame1,
	const uint_t frame2,
	const float_t weight_frame1
)
{
	// computing the count and checking it is just for additional validation.
	// the element-count of the stream and all the keyframes should be 
	// the same anyway
	uint_t count = keyframes_[frame1]->GetElementCount();
	if (count > keyframes_[frame2]->GetElementCount())
	{
		count = keyframes_[frame2]->GetElementCount();
	}
	if (count > animated_stream_->GetElementCount())
	{
		count = animated_stream_->GetElementCount();
	}

	// interpolate or take one of the frames
	if (frame1 == frame2)
	{
		math::Vector3f* keyframe_data = keyframes_[frame1]->GetData();

		uint_t stride;
		animated_stream_->LockData(stride);
		for (uint_t i = 0; i < count; ++i)
		{
			animated_stream_->SetVector3(i, keyframe_data[i]);
		}
		animated_stream_->UnlockData();
	}
	else
	{
		math::Vector3f* keyframe1_data = keyframes_[frame1]->GetData();
		math::Vector3f* keyframe2_data = keyframes_[frame2]->GetData();
		float_t weight_frame2 = 1.0f - weight_frame1;

		uint_t stride;
		animated_stream_->LockData(stride);
		for (uint_t i = 0; i < count; ++i)
		{
			math::Vector3f result;
			result = keyframe1_data[i] * weight_frame1 + keyframe2_data[i] * weight_frame2;
			animated_stream_->SetVector3(i, result);
		}
		animated_stream_->UnlockData();
	}
}




//-----------------------------------------------------------------------------------------------------------
/// Die animierte Entity zurückgeben
Entity* Vector3StreamAnimationTrack::GetAnimatedEntity()
{
	return animated_entity_;
}




//-----------------------------------------------------------------------------------------------------------
/// Die animierte Entity zurückgeben
const Entity* Vector3StreamAnimationTrack::GetAnimatedEntity() const
{
	return animated_entity_;
}




//-----------------------------------------------------------------------------------------------------------
/// Den animierten Stream zurückgeben
Stream* Vector3StreamAnimationTrack::GetAnimatedStream()
{
	return animated_stream_;
}




//-----------------------------------------------------------------------------------------------------------
/// Den animierten Stream zurückgeben
const Stream* Vector3StreamAnimationTrack::GetAnimatedStream() const
{
	return animated_stream_;
}




//-----------------------------------------------------------------------------------------------------------
/// Den Typ des Tracks zurückgeben
const AnimationTrack::AnimationTrackType Vector3StreamAnimationTrack::GetAnimationTrackType() const
{
	return Vector3StreamAnimationTrackType;
}

}} // namespace x::model
