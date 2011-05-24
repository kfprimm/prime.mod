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
/*!	\file model/keyframeanimation/animationcontroller.h
	\brief Definition von x::model::AnimationController
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::model::AnimationController
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef MODEL_KEYFRAMEANIMATION_ANIMATIONCONTROLLER_H
#define MODEL_KEYFRAMEANIMATION_ANIMATIONCONTROLLER_H

#include "../../../libx_core/global/global.h"
#include "./animationgroup.h"
#include "./animation.h"

namespace x { namespace model {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief
		Kontrolliert die aktuell laufende Animation
	\version
		0.1
	\date
		08.02.2007, Christian Oberholzer, Programmierung

	Der AnimationController enhält einen Verweis auf eine AnimationGroup. Er kontrolliert die
	Animation dieser Animationsgruppe. 
*/
class AnimationController
{
public:
	// Konstruktur & Destruktor
	AnimationController(AnimationGroup* animation_group);
	virtual ~AnimationController();

	/// Aktuell laufende Animation setzen
	virtual void SwitchToAnimation(const uint_t index);
	/// Aktuell laufende Animation setzen
	virtual void SwitchToAnimation(const string_t& animation_name);
	/// Aktuell laufende Animation setzen
	virtual void SwitchToAnimation(const Animation* animation);
	/// Aktuell laufende Animation zurückgeben
	virtual const Animation* GetRunningAnimation() const;

	/// Aktuell laufende Animation auf einen gewissen Zeitpunkt setzen und anwenden
	virtual void Apply(const float_t time);

protected:
	/// Aktuell laufende Animation setzen
	virtual void SwitchToAnimationInternal(const Animation* animation);

protected:
	/// Zeiger auf die kontrollierte AnimationGroup
	AnimationGroup* animation_group_;
	/// Zeitpunkt zu welchem die laufende Animation gestartet wurde
	float_t running_starttime_;
	/// Laufende Animation
	Animation* running_animation_;
};

}} // namespace x::model

#endif // MODEL_KEYFRAMEANIMATION_ANIMATIONCONTROLLER_H
