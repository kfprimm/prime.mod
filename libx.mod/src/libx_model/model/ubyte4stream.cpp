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
/*!	\file model/ubyte4stream.cpp
	\brief Implementation von x::model::UByte4Stream
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::model::UByte4Stream
*/
//-----------------------------------------------------------------------------------------------------------
#include "./ubyte4stream.h"

namespace x { namespace model {

//-----------------------------------------------------------------------------------------------------------
UByte4Stream::UByte4Stream()
:	semantic_type_(SemanticTypeUserDefined),
	semantic_info_(0),
	element_count_(0),
	array_(NULL),
	is_dirty_(false)
{
}




//-----------------------------------------------------------------------------------------------------------
UByte4Stream::~UByte4Stream()
{
	SAFE_DELETE_ARRAY(array_);
}




//-----------------------------------------------------------------------------------------------------------
void UByte4Stream::SetSemantics(
	const SemanticTypes semantics
)
{
	semantic_type_ = semantics;
}




//-----------------------------------------------------------------------------------------------------------
void UByte4Stream::SetSemanticsInfo(
	const uint_t semantics_info
)
{
	semantic_info_ = semantics_info;
}




//-----------------------------------------------------------------------------------------------------------
void UByte4Stream::SetData(
	const uint_t element_count,
	math::Vector4<ubyte_t>* data
)
{
	SAFE_DELETE_ARRAY(array_);
	element_count_ = element_count;
	array_ = data;
}




//-----------------------------------------------------------------------------------------------------------
const uint_t UByte4Stream::GetElementCount() const
{
	return element_count_;
}




//-----------------------------------------------------------------------------------------------------------
const ValueType UByte4Stream::GetValueType() const
{
	return ValueUByte4;
}




//-----------------------------------------------------------------------------------------------------------
const SemanticTypes UByte4Stream::GetSemantics() const
{
	return semantic_type_;
}




//-----------------------------------------------------------------------------------------------------------
const uint_t UByte4Stream::GetSemanticInfo() const
{
	return semantic_info_;
}




//-----------------------------------------------------------------------------------------------------------
/// Daten zum Zugriff bereit machen
ubyte_t* UByte4Stream::LockData(
	uint_t& stride
)
{
	is_dirty_ = true;
	return GetData(stride);
}




//-----------------------------------------------------------------------------------------------------------
/// Zugriff auf Daten. Nur Gültig innerhalb eins LockData/UnlockData-Paares
ubyte_t* UByte4Stream::GetData(
	uint_t& stride
)
{
	stride = sizeof(math::Vector4<ubyte_t>);
	return (ubyte_t*)array_;
}




//-----------------------------------------------------------------------------------------------------------
/// Daten fertig "bearbeitet". Wieder freigeben
void UByte4Stream::UnlockData()
{
}




//-----------------------------------------------------------------------------------------------------------
const bool_t UByte4Stream::IsDirty() const
{
	return is_dirty_;
}




//-----------------------------------------------------------------------------------------------------------
void UByte4Stream::ClearDirtyFlag()
{
	is_dirty_ = false;
}




//-----------------------------------------------------------------------------------------------------------
Stream* UByte4Stream::Clone() const
{
	UByte4Stream* clone(new UByte4Stream());
	clone->semantic_type_ = semantic_type_;
	clone->semantic_info_ = semantic_info_;
	clone->element_count_ = element_count_;
	clone->array_ = new math::Vector4<ubyte_t>[clone->element_count_];
	memcpy(clone->array_, array_, sizeof(math::Vector4<ubyte_t>) * element_count_);
	clone->is_dirty_ = is_dirty_;
	return clone;
}

}} // namespace x::model

