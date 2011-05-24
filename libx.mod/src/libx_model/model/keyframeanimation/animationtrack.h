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
/*!	\file model/keyframeanimation/animationtrack.h
	\brief Definition von x::model::AnimationTrack
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::model::AnimationTrack
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef MODEL_KEYFRAMEANIMATION_ANIMATIONTRACK_H
#define MODEL_KEYFRAMEANIMATION_ANIMATIONTRACK_H

#include "../../../libx_core/global/global.h"

namespace x { namespace model {

class Animation;




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief
		Eine Animationsspur
	\version 
		0.1
	\date
		17.10.2006, Christian Oberholzer, Implementierung

	Dieses Interface definiert eine Spur einer Animation. Mit einer Animationsspur kann ein einzelnes
	Element der Animation animiert werden wie beispielsweise:
		- Ein numerischer Wert (float, Vector, Quaternion)
		- Ein Stream von geometriedaten
		- Ein Element eines Skelettes
*/
class AnimationTrack
{
public:
	enum AnimationTrackType
	{
		Vector3AnimationTrackType,
		QuaternionAnimationTrackType,
		Vector3StreamAnimationTrackType
	};

public:
	// Konstruktur & Destruktor
	AnimationTrack();
	virtual ~AnimationTrack();

	/// Den Typ des Tracks zurückgeben
	virtual const AnimationTrackType GetAnimationTrackType() const = 0;

	/// Animation anwenden
	virtual void Apply(const uint_t frame1, const uint_t frame2, const float_t weight_frame1) = 0;

protected:
	Animation* animation_;
};

}} // namespace x::model

#endif // MODEL_KEYFRAMEANIMATION_ANIMATIONTRACK_H
