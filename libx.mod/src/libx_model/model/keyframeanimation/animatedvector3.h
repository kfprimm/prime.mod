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
/*!	\file model/keyframeanimation/animatedvector3.h
	\brief Definition von x::model::AnimatedVector3
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::model::AnimatedVector3
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef MODEL_KEYFRAMEANIMATION_ANIMATEDVECTOR3_H
#define MODEL_KEYFRAMEANIMATION_ANIMATEDVECTOR3_H

#include "../../../libx_core/global/global.h"

namespace x { namespace model {

//-----------------------------------------------------------------------------------------------------------
/*!
*/
class AnimatedVector3
{
public:
	// Konstruktur & Destruktor
	AnimatedVector3();
	virtual ~AnimatedVector3();
	virtual void SetAnimatedVector3(const math::Vector3f& v) = 0;
};




//-----------------------------------------------------------------------------------------------------------
/*!
*/
class AnimatedVector3Instance : public AnimatedVector3
{
public:
	// Konstruktur & Destruktor
	AnimatedVector3Instance();
	virtual ~AnimatedVector3Instance();
	virtual void SetAnimatedVector3(const math::Vector3f& v);
	virtual const math::Vector3f& GetAnimatedVector3() const;

protected:
	math::Vector3f v_;
};

}} // namespace x::model

#endif // MODEL_KEYFRAMEANIMATION_ANIMATEDVECTOR3_H
