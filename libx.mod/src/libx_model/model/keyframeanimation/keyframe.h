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
/*!	\file model/keyframeanimation/keyframe.h
	\brief Definition von x::model::KeyFrame
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::model::KeyFrame
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef MODEL_KEYFRAMEANIMATION_KEYFRAME_H
#define MODEL_KEYFRAMEANIMATION_KEYFRAME_H

#include "../../../libx_core/global/global.h"

namespace x {

//-----------------------------------------------------------------------------------------------------------
/*!
*/
template <
	typename TypeT
>
class KeyFrame
{
public:
	typedef KeyFrame<math::Vector3f> KeyFrameVector3;

public:
	// Konstruktur & Destruktor
	KeyFrame(const uint_t element_count, TypeT* array)
	:	element_count_(element_count),
		array_(array)
	{
	}

	virtual ~KeyFrame()
	{
		delete[] array_;
	}

	const uint_t GetElementCount() const
	{
		return element_count_;
	}

	TypeT* GetData()
	{
		return array_;
	}

	const TypeT* GetData() const
	{
		return array_;
	}

protected:
	uint_t element_count_;
	TypeT* array_;
};

typedef KeyFrame<math::Vector3f> KeyFrameVector3;




} // namespace x

#endif // MODEL_KEYFRAMEANIMATION_KEYFRAME_H
