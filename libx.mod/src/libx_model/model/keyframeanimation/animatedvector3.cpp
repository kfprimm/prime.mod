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
/*!	\file model/keyframeanimation/animatedvector3.cpp
	\brief Implementation von x::model::AnimatedVector3
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::model::AnimatedVector3
*/
//-----------------------------------------------------------------------------------------------------------
#include "./animatedvector3.h"

namespace x { namespace model {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
AnimatedVector3::AnimatedVector3()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
AnimatedVector3::~AnimatedVector3()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
AnimatedVector3Instance::AnimatedVector3Instance()
:	v_(math::Vector3f::ZERO())
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
AnimatedVector3Instance::~AnimatedVector3Instance()
{
}




//-----------------------------------------------------------------------------------------------------------
void AnimatedVector3Instance::SetAnimatedVector3(
	const math::Vector3f& v
)
{
	v_ = v;
}




//-----------------------------------------------------------------------------------------------------------
const math::Vector3f& AnimatedVector3Instance::GetAnimatedVector3() const
{
	return v_;
}

}} // namespace x::model
