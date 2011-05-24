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
/*!	\file model/keyframeanimation/animation.cpp
	\brief Implementation von x::model::Animation
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::model::Animation
*/
//-----------------------------------------------------------------------------------------------------------
#include "./animation.h"

namespace x { namespace model {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
Animation::Animation()
:	name_(),
	frame_count_(0),
	frames_per_second_(0),
	tracks_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
Animation::~Animation()
{
	while (!tracks_.empty())
	{
		AnimationTrack* track = tracks_.back();
		tracks_.pop_back();
		delete track;
	}
}




//-----------------------------------------------------------------------------------------------------------
/// Den Namen der Animation setzen
void Animation::SetName(
	const string_t& name
)
{
	name_ = name;
}




//-----------------------------------------------------------------------------------------------------------
/// Anzahl der Frames der Animation setzen
void Animation::SetFrameCount(
	const uint_t frame_count
)
{
	frame_count_ = frame_count;
}




//-----------------------------------------------------------------------------------------------------------
/// Abspielgeschwindigkeit in Frames/s setzen
void Animation::SetFramesPerSecond(
	const uint_t frames_per_second
)
{
	frames_per_second_ = frames_per_second;
}




//-----------------------------------------------------------------------------------------------------------
/// Eine Animationsspur hinzufügen
void Animation::AddAnimationTrack(
	AnimationTrack* animation_track
)
{
	tracks_.push_back(animation_track);
}




//-----------------------------------------------------------------------------------------------------------
/// Name der Animation zurückgeben
const string_t& Animation::GetName() const
{
	return name_;
}




//-----------------------------------------------------------------------------------------------------------
/// Anzahl der Frames dieser Animation zurückgeben
const uint_t Animation::GetFrameCount() const
{
	return frame_count_;
}




//-----------------------------------------------------------------------------------------------------------
/// Geschwindigkeit der Animation in Frames/s zurückgeben
const uint_t Animation::GetFramesPerSecond() const
{
	return frames_per_second_;
}




//-----------------------------------------------------------------------------------------------------------
/// Zeit welche für einen Durchlauf der Animation benötigt wird. Entspricht framescount / fps.
const float_t Animation::GetRunningTime() const
{
	return ((float_t)frame_count_ / (float_t)frames_per_second_);
}




//-----------------------------------------------------------------------------------------------------------
/// Ein Animationsframe einer gegebenen Zeit und deren Gewichtung zurückgeben
void Animation::GetFrames(
	const float_t time,
	uint_t& frame1,
	uint_t& frame2,
	float_t& weight_frame1
)
{
	float_t running_time = GetRunningTime();
	float_t modified_time = time;
	while (modified_time > running_time)
	{
		modified_time -= running_time;
	}

	if (time == 0.0f)
	{
		// Das erste Frame ist aktiv
		frame1 = frame2 = 0;
		weight_frame1 = 1.0f;
	}
	else if (time == running_time)
	{
		// Das letzte Frame ist aktiv
		frame1 = frame2 = frame_count_ - 1;
		weight_frame1 = 0.0f;
	}
	else
	{
		// Zwei Frames sind aktiv => Gewichtung bestimmen
		uint_t frame_index = 0;
		float_t time_per_frame = 1.0f / (float_t)frames_per_second_;
		while (modified_time > time_per_frame)
		{
			modified_time -= time_per_frame;
			++frame_index;
		}
		frame1 = frame_index;
		frame2 = frame_index + 1;
		if (frame2 == frame_count_) // wrap
		{
			frame2 = 0;
		}
		weight_frame1 = 1.0f - modified_time / time_per_frame;
	}
}




//-----------------------------------------------------------------------------------------------------------
/// Animation auf einen gewissen Zeitpunkt setzen und anwenden
void Animation::Apply(
	const float_t time
)
{
	// Frames und Gewichtung berechnen
	uint_t frame1;
	uint_t frame2;
	float_t weight_frame1;
	GetFrames(time, frame1, frame2, weight_frame1);

	// An alle Tracks weitergeben
	typedef vector<AnimationTrack*>::iterator iterator;
	for (iterator begin = tracks_.begin(); begin != tracks_.end(); ++begin)
	{
		(*begin)->Apply(frame1, frame2, weight_frame1);
	}
}




//-----------------------------------------------------------------------------------------------------------
Vector3StreamAnimationTrack* Animation::CreateVector3StreamAnimationTrack()
{
	return new Vector3StreamAnimationTrack();
}




//-----------------------------------------------------------------------------------------------------------
VectorAnimationTrack* Animation::CreateVectorAnimationTrack()
{
	return new VectorAnimationTrack();
}




//-----------------------------------------------------------------------------------------------------------
QuaternionAnimationTrack* Animation::CreateQuaternionAnimationTrack()
{
	return new QuaternionAnimationTrack();
}




//-----------------------------------------------------------------------------------------------------------
void Animation::CreateNormalsAnimationTrack()
{
	// collect all position tracks animated through this animation
	vector<Vector3StreamAnimationTrack*> position_tracks;
	typedef vector<AnimationTrack*>::iterator iterator;
	for (iterator begin = tracks_.begin(); begin != tracks_.end(); ++begin)
	{
		if ((*begin)->GetAnimationTrackType() == AnimationTrack::Vector3StreamAnimationTrackType)
		{
			Vector3StreamAnimationTrack* track = (Vector3StreamAnimationTrack*)(*begin);
			if (track->GetAnimatedStream()->GetSemantics() == SemanticTypePosition)
			{
				position_tracks.push_back(track);
			}
		}
	}

	// keyframe-arrays erstellen
	KeyFrameVector3*** keyframes;
	keyframes = new KeyFrameVector3**[position_tracks.size()];
	for (uint_t i = 0; i < position_tracks.size(); ++i)
	{
		keyframes[i] = new KeyFrameVector3*[frame_count_];
	}

	// normals berechnen
	for (uint_t i = 0; i < frame_count_; ++i)
	{
		for (uint_t j = 0; j < position_tracks.size(); ++j)
		{
			// apply position tracks and let the entity compute the normal vetors
			position_tracks[j]->Apply(i, i, 1.0f);
			Entity* entity = position_tracks[j]->GetAnimatedEntity();
			entity->ComputeNormalVectors();

			// lock stream data
			uint_t stride = 0;
			Stream* normal_stream = entity->GetGeometry()->GetStream(SemanticTypeNormal, 0);
			ubyte_t* locked_data = normal_stream->LockData(stride);

			// copy data into a new keyframe-buffer
			math::Vector3f* keyframe_data = new math::Vector3f[entity->GetGeometry()->GetVertexCount()];
			if (stride == sizeof(math::Vector3f))
			{
				memcpy(keyframe_data, locked_data, sizeof(math::Vector3f) * entity->GetGeometry()->GetVertexCount());
			}
			else
			{
				for (uint_t k = 0; k < entity->GetGeometry()->GetVertexCount(); ++k)
				{
					keyframe_data[k] = normal_stream->GetVector3(k);
				}
			}

			// unlock stream data
			normal_stream->UnlockData();

			// create the new keyframe
			KeyFrameVector3* keyframe = new KeyFrameVector3(entity->GetGeometry()->GetVertexCount(), keyframe_data);
			keyframes[j][i] = keyframe;
		}
	}

	// normal-animation-tracks erstellen
	for (uint_t i = 0; i < position_tracks.size(); ++i)
	{
		Entity* animated_entity = position_tracks[i]->GetAnimatedEntity();
		Stream* animated_stream = animated_entity->GetGeometry()->GetStream(SemanticTypeNormal, 0);
		Vector3StreamAnimationTrack* normal_track(new Vector3StreamAnimationTrack());
		normal_track->SetAnimatedEntity(position_tracks[i]->GetAnimatedEntity());
		normal_track->SetAnimatedStream(animated_stream);
		normal_track->SetKeyFrames(GetFrameCount(), keyframes[i]);
		tracks_.push_back(normal_track);
	}

	delete[] keyframes;
}




//-----------------------------------------------------------------------------------------------------------
void Animation::CreateTangentsAnimationTrack()
{
}

}} // namespace x::model
