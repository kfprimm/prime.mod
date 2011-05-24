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
/*!	\file model/keyframeanimation/animatedquaternion.cpp
	\brief Implementation von x::model::AnimatedQuaternion
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::model::AnimatedQuaternion
*/
//-----------------------------------------------------------------------------------------------------------
#include "./animatedquaternion.h"

namespace x { namespace model {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
AnimatedQuaternion::AnimatedQuaternion()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
AnimatedQuaternion::~AnimatedQuaternion()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
AnimatedQuaternionInstance::AnimatedQuaternionInstance()
:	q_(math::Quaternionf::IDENTITY())
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
AnimatedQuaternionInstance::~AnimatedQuaternionInstance()
{
}




//-----------------------------------------------------------------------------------------------------------
void AnimatedQuaternionInstance::SetAnimatedQuaternion(
	const math::Quaternionf& q
)
{
	q_ = q;
}




//-----------------------------------------------------------------------------------------------------------
const math::Quaternionf& AnimatedQuaternionInstance::GetAnimatedQuaternion() const
{
	return q_;
}

}} // namespace x::model
