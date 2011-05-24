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
/*!	\file model/vector2stream.cpp
	\brief Implementation von x::model::Vector2Stream
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::model::Vector2Stream
*/
//-----------------------------------------------------------------------------------------------------------
#include "./vector2stream.h"

namespace x { namespace model {

//-----------------------------------------------------------------------------------------------------------
Vector2Stream::Vector2Stream()
:	semantic_type_(SemanticTypeUserDefined),
	semantic_info_(0),
	element_count_(0),
	array_(NULL),
	is_dirty_(false)
{
}




//-----------------------------------------------------------------------------------------------------------
Vector2Stream::~Vector2Stream()
{
	SAFE_DELETE_ARRAY(array_);
}




//-----------------------------------------------------------------------------------------------------------
void Vector2Stream::SetSemantics(
	const SemanticTypes semantics
)
{
	semantic_type_ = semantics;
}




//-----------------------------------------------------------------------------------------------------------
void Vector2Stream::SetSemanticsInfo(
	const uint_t semantics_info
)
{
	semantic_info_ = semantics_info;
}




//-----------------------------------------------------------------------------------------------------------
void Vector2Stream::SetData(
	const uint_t element_count,
	math::Vector2f* data
)
{
	SAFE_DELETE_ARRAY(array_);
	element_count_ = element_count;
	array_ = data;
}




//-----------------------------------------------------------------------------------------------------------
const uint_t Vector2Stream::GetElementCount() const
{
	return element_count_;
}




//-----------------------------------------------------------------------------------------------------------
const ValueType Vector2Stream::GetValueType() const
{
	return ValueVector2;
}




//-----------------------------------------------------------------------------------------------------------
const SemanticTypes Vector2Stream::GetSemantics() const
{
	return semantic_type_;
}




//-----------------------------------------------------------------------------------------------------------
const uint_t Vector2Stream::GetSemanticInfo() const
{
	return semantic_info_;
}




//-----------------------------------------------------------------------------------------------------------
/// Daten zum Zugriff bereit machen
ubyte_t* Vector2Stream::LockData(
	uint_t& stride
)
{
	is_dirty_ = true;
	return GetData(stride);
}




//-----------------------------------------------------------------------------------------------------------
/// Zugriff auf Daten. Nur Gültig innerhalb eins LockData/UnlockData-Paares
ubyte_t* Vector2Stream::GetData(
	uint_t& stride
)
{
	stride = sizeof(math::Vector2f);
	return (ubyte_t*)array_;
}




//-----------------------------------------------------------------------------------------------------------
/// Daten fertig "bearbeitet". Wieder freigeben
void Vector2Stream::UnlockData()
{
}




//-----------------------------------------------------------------------------------------------------------
const bool_t Vector2Stream::IsDirty() const
{
	return is_dirty_;
}




//-----------------------------------------------------------------------------------------------------------
void Vector2Stream::ClearDirtyFlag()
{
	is_dirty_ = false;
}




//-----------------------------------------------------------------------------------------------------------
Stream* Vector2Stream::Clone() const
{
	Vector2Stream* clone(new Vector2Stream());
	clone->semantic_type_ = semantic_type_;
	clone->semantic_info_ = semantic_info_;
	clone->element_count_ = element_count_;
	clone->array_ = new math::Vector2f[clone->element_count_];
	memcpy(clone->array_, array_, sizeof(math::Vector2f) * element_count_);
	clone->is_dirty_ = is_dirty_;
	return clone;
}

}} // namespace x::model

