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
/*!	\file model/keyframeanimation/animationgroup.cpp
	\brief Implementation von x::model::AnimationGroup
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::model::AnimationGroup
*/
//-----------------------------------------------------------------------------------------------------------
#include "./animationgroup.h"
#include "./animationcontroller.h"

namespace x { namespace model {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
AnimationGroup::AnimationGroup()
:	name_(),
	animations_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
AnimationGroup::~AnimationGroup()
{
	while(!animations_.empty())
	{
		Animation* animation = animations_.back();
		animations_.pop_back();
		delete animation;
	}
}




//-----------------------------------------------------------------------------------------------------------
void AnimationGroup::SetName(
	const string_t& name
)
{
	name_ = name;
}




//-----------------------------------------------------------------------------------------------------------
void AnimationGroup::AddAnimation(
	Animation* animation
)
{
	animations_.push_back(animation);
}




//-----------------------------------------------------------------------------------------------------------
/// Name der AnimationGroup zurückgeben
const string_t& AnimationGroup::GetName() const
{
	return name_;
}




//-----------------------------------------------------------------------------------------------------------
/// Anzahl der verschiedenen Animationen in der Gruppe zurückgeben
const uint_t AnimationGroup::GetAnimationCount() const
{
	return (uint_t)animations_.size();
}




//-----------------------------------------------------------------------------------------------------------
/// Eine Animation der AnimationGroup zurückgeben
Animation* AnimationGroup::GetAnimation(
	const uint_t index
)
{
	return animations_[index];
}




//-----------------------------------------------------------------------------------------------------------
/// Eine Animation der AnimationGroup zurückgeben
const Animation* AnimationGroup::GetAnimation(
	const uint_t index
) const
{
	return animations_[index];
}




//-----------------------------------------------------------------------------------------------------------
Animation* AnimationGroup::CreateAnimation()
{
	return new Animation();
}




//-----------------------------------------------------------------------------------------------------------
AnimationController* AnimationGroup::CreateAnimationController()
{
	return new AnimationController(this);
}

}} // namespace x::model
