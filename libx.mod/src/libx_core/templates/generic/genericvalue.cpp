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
/*!	
	\file templates/generic/genericvalue.cpp
	\brief Implementation von x::templates::generic::GenericValue
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::generic::GenericValue
*/
//-----------------------------------------------------------------------------------------------------------
#define XLIB_CORE
#include "../../global/global.h"
#include "../../scanner/scanner.h"
#include "../../tokens/tokens.h"
#include "../../serializer.h"
#include "./genericvalue.h"

namespace x { namespace templates { namespace generic {

using namespace std;

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
GenericValue::GenericValue(
	const ValueTypes type
)
:	type_(type)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
GenericValue::~GenericValue()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief return value-type
	\return type

	returns the value-type of this class
*/
const GenericValue::ValueTypes GenericValue::GetType() const
{
	return type_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
template <
	typename TypeT,
	GenericValue::ValueTypes ValueType
>
GenericValueImpl<TypeT, ValueType>::GenericValueImpl(
	const TypeT& Value
)
:	GenericValue(ValueType),
	value_(Value)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
template <
	typename TypeT,
	GenericValue::ValueTypes ValueType
>
GenericValueImpl<TypeT, ValueType>::~GenericValueImpl()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief returns the string value
	\return value
*/
template <
	typename TypeT,
	GenericValue::ValueTypes ValueType
>
const TypeT& GenericValueImpl<TypeT, ValueType>::GetValue() const
{
	return value_;
}

template class GenericValueImpl<udword_t, GenericValue::ValueTypeUDWord>;
template class GenericValueImpl<float_t, GenericValue::ValueTypeFloat>;
template class GenericValueImpl<string_t, GenericValue::ValueTypeString>;

}}} // namespace x::templates::generic

