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
/*!	\file model/keyframeanimation/quaternionanimationtrack.h
	\brief Definition von x::model::QuaternionAnimationTrack
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::model::QuaternionAnimationTrack
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef MODEL_KEYFRAMEANIMATION_QUATERNIONANIMATIONTRACK_H
#define MODEL_KEYFRAMEANIMATION_QUATERNIONANIMATIONTRACK_H

#include "../../../libx_core/global/global.h"
#include "./animationtrack.h"
#include "./animatedquaternion.h"

namespace x { namespace model {

//-----------------------------------------------------------------------------------------------------------
/*!
*/
class QuaternionAnimationTrack : public AnimationTrack
{
public:
	// Konstruktur & Destruktor
	QuaternionAnimationTrack();
	virtual ~QuaternionAnimationTrack();

	/// Das animierte Quaternion setzen
	virtual void SetAnimatedQuaternion(AnimatedQuaternion* q);
	/// Keyframes setzen
	virtual void SetKeyFrames(math::Quaternionf* keyframes);

	/// Animation anwenden
	virtual void Apply(const uint_t frame1, const uint_t frame2, const float_t weight_frame1);

	/// Den Typ des Tracks zurückgeben
	virtual const AnimationTrackType GetAnimationTrackType() const;

protected:
	AnimatedQuaternion* animated_quaternion_;
	math::Quaternionf* keyframes_;
};

}} // namespace x::model

#endif // MODEL_KEYFRAMEANIMATION_QUATERNIONANIMATIONTRACK_H
