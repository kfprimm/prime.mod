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
/*!	\file model/keyframeanimation/vector3streamanimationtrack.h
	\brief Definition von x::model::Vector3StreamAnimationTrack
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::model::Vector3StreamAnimationTrack
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef MODEL_KEYFRAMEANIMATION_VECTOR3STREAMANIMATIONTRACK_H
#define MODEL_KEYFRAMEANIMATION_VECTOR3STREAMANIMATIONTRACK_H

#include "../../../libx_core/global/global.h"
#include "../stream.h"
#include "../entity.h"
#include "./animationtrack.h"
#include "./keyframe.h"

namespace x { namespace model {

//-----------------------------------------------------------------------------------------------------------
/*!
*/
class Vector3StreamAnimationTrack : public AnimationTrack
{
public:
	// Konstruktur & Destruktor
	Vector3StreamAnimationTrack();
	virtual ~Vector3StreamAnimationTrack();

	/// Die animierte Entity setzen
	virtual void SetAnimatedEntity(Entity* animated_entity);
	/// Den animierten Stream setzen
	virtual void SetAnimatedStream(Stream* animated_stream);
	/// Die Keyframes setzen
	virtual void SetKeyFrames(const uint_t keyframe_count, KeyFrameVector3** keyframes);

	/// Animation anwenden
	virtual void Apply(const uint_t frame1, const uint_t frame2, const float_t weight_frame1);

	/// Den Typ des Tracks zurückgeben
	virtual const AnimationTrackType GetAnimationTrackType() const;
	/// Die animierte Entity zurückgeben
	virtual Entity* GetAnimatedEntity();
	/// Die animierte Entity zurückgeben
	virtual const Entity* GetAnimatedEntity() const;
	/// Den animierten Stream zurückgeben
	virtual Stream* GetAnimatedStream();
	/// Den animierten Stream zurückgeben
	virtual const Stream* GetAnimatedStream() const;

protected:
	Entity* animated_entity_;
	Stream* animated_stream_;
	uint_t keyframe_count_;
	KeyFrameVector3** keyframes_;
};

}} // namespace x::model

#endif // MODEL_KEYFRAMEANIMATION_VECTOR3STREAMANIMATIONTRACK_H
