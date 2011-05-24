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
/*!	\file model/keyframeanimation/animationcontroller.cpp
	\brief Implementation von x::model::AnimationController
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::model::AnimationController
*/
//-----------------------------------------------------------------------------------------------------------
#include "./animationcontroller.h"

namespace x { namespace model {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
AnimationController::AnimationController(
	AnimationGroup* animation_group
)
:	animation_group_(animation_group),
	running_starttime_(0.0f),
	running_animation_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
AnimationController::~AnimationController()
{
}




//-----------------------------------------------------------------------------------------------------------
/// Aktuell laufende Animation setzen
void AnimationController::SwitchToAnimation(
	const uint_t index
)
{
	SwitchToAnimation(animation_group_->GetAnimation(index));
}




//-----------------------------------------------------------------------------------------------------------
/// Aktuell laufende Animation setzen
void AnimationController::SwitchToAnimation(
	const string_t& animation_name
)
{
	for (uint_t i = 0; i < animation_group_->GetAnimationCount(); ++i)
	{
		Animation* animation = animation_group_->GetAnimation(i);
		if (animation->GetName() == animation_name)
		{
			SwitchToAnimationInternal(animation);
			return;
		}
	}
	throw Exception(string_t("animation with name '") + animation_name + "' doesn't exist!");
}




//-----------------------------------------------------------------------------------------------------------
/// Aktuell laufende Animation setzen
void AnimationController::SwitchToAnimation(
	const Animation* animation
)
{
	for (uint_t i = 0; i < animation_group_->GetAnimationCount(); ++i)
	{
		Animation* current = animation_group_->GetAnimation(i);
		if (current == animation)
		{
			SwitchToAnimationInternal(current);
			return;
		}
	}
	throw Exception("given parameter 'animation' is not part of this animation-group");
}




//-----------------------------------------------------------------------------------------------------------
/// Aktuell laufende Animation zurückgeben
const Animation* AnimationController::GetRunningAnimation() const
{
	return running_animation_;
}




//-----------------------------------------------------------------------------------------------------------
/// Aktuell laufende Animation auf einen gewissen Zeitpunkt setzen und anwenden
void AnimationController::Apply(
	const float_t time
)
{
	if (running_starttime_ < 0.0f)
	{
		running_starttime_ = time;
	}

	if (running_animation_ != NULL)
	{
		running_animation_->Apply(time - running_starttime_);
	}
}




//-----------------------------------------------------------------------------------------------------------
/// Aktuell laufende Animation setzen
void AnimationController::SwitchToAnimationInternal(
	const Animation* animation
)
{
	Animation* anim = const_cast<Animation*>(animation);
	running_starttime_ = -1.0f;
	running_animation_ = anim;
}

}} // namespace x::model
