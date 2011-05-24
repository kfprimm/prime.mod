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
/*!	\file model/keyframeanimation/animatedquaternion.h
	\brief Definition von x::model::AnimatedQuaternion
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::model::AnimatedQuaternion
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef MODEL_KEYFRAMEANIMATION_ANIMATEDQUATERNION_H
#define MODEL_KEYFRAMEANIMATION_ANIMATEDQUATERNION_H

#include "../../../libx_core/global/global.h"

namespace x { namespace model {

//-----------------------------------------------------------------------------------------------------------
/*!
*/
class AnimatedQuaternion
{
public:
	// Konstruktur & Destruktor
	AnimatedQuaternion();
	virtual ~AnimatedQuaternion();
	virtual void SetAnimatedQuaternion(const math::Quaternionf& v) = 0;
};




//-----------------------------------------------------------------------------------------------------------
/*!
*/
class AnimatedQuaternionInstance : public AnimatedQuaternion
{
public:
	// Konstruktur & Destruktor
	AnimatedQuaternionInstance();
	virtual ~AnimatedQuaternionInstance();
	virtual void SetAnimatedQuaternion(const math::Quaternionf& v);
	virtual const math::Quaternionf& GetAnimatedQuaternion() const;

protected:
	math::Quaternionf q_;
};

}} // namespace x::model

#endif // MODEL_KEYFRAMEANIMATION_ANIMATEDQUATERNION_H
