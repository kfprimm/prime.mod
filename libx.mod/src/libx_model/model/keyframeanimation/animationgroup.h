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
/*!	\file model/keyframeanimation/animationgroup.h
	\brief Definition von x::model::AnimationGroup
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::model::AnimationGroup
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef MODEL_KEYFRAMEANIMATION_ANIMATIONGROUP_HH
#define MODEL_KEYFRAMEANIMATION_ANIMATIONGROUP_HH

#include "../../../libx_core/global/global.h"
#include "./animation.h"

namespace x { namespace model {

//-----------------------------------------------------------------------------------------------------------
class AnimationController;




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief
		Eine Gruppe von Animationen
	\version
		0.1
	\date
		18.10.2006, Christian Oberholzer, Programmierung

	Eine AnimationGroup enhält mehrere Animationen. Alle Animationen der AnimationGroup
	beziehen sich auf dieselben Objekte. Es kann jeweils eine der Animationen der AnimationGroup
	ausgeführt werden.
*/
class AnimationGroup
{
public:
	// Konstruktur & Destruktor
	AnimationGroup();
	virtual ~AnimationGroup();

	/// Den Namen der Animationsgruppe setzen
	virtual void SetName(const string_t& name);
	/// Eine Animation zur Gruppe hinzufügen
	virtual void AddAnimation(Animation* animation);

	/// Name der AnimationGroup zurückgeben
	virtual const string_t& GetName() const;
	/// Anzahl der verschiedenen Animationen in der Gruppe zurückgeben
	virtual const uint_t GetAnimationCount() const;
	/// Eine Animation der AnimationGroup zurückgeben
	virtual Animation* GetAnimation(const uint_t index);
	/// Eine Animation der AnimationGroup zurückgeben
	virtual const Animation* GetAnimation(const uint_t index) const;

	/// Animation erstellen
	virtual Animation* CreateAnimation();
	/// AnimationController für diese Group erstellen
	virtual AnimationController* CreateAnimationController();

protected:
	/// Name der Animationsgruppe
	string_t name_;
	/// Animationen in der Animationsgruppe
	vector<Animation*> animations_;
};

}} // namespace x::model

#endif // MODEL_KEYFRAMEANIMATION_ANIMATIONGROUP_HH
