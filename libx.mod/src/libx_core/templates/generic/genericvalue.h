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
	\file templates/generic/genericvalue.h
	\brief Definition von x::templates::generic::GenericValue
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::templates::generic::GenericValue
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef TEMPLATES_GENERIC_GENERICVALUE_H
#define TEMPLATES_GENERIC_GENERICVALUE_H

#include "../../global/global.h"
#include "../../temporalinfo.h"

namespace x { namespace templates { namespace generic {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief generic value
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	super-class for all value-classes
*/
class GenericValue
{
public:
	enum ValueTypes
	{
		ValueTypeUDWord,
		ValueTypeFloat,
		ValueTypeString
	};

public:
	//-------------------------------------------------------------------------------------------------------
	// Konstruktur & Destruktor
	GenericValue(const ValueTypes type);
	virtual ~GenericValue();

	//-------------------------------------------------------------------------------------------------------
	virtual const ValueTypes GetType() const;

protected:
	ValueTypes type_;
};




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief generic-value implementation
	\version 0.1
	\date 06.05.2004
	\author Christian Oberholzer

	implementation of GenericValue for a specific value type.
*/
template <
	typename TypeT,
	GenericValue::ValueTypes ValueType
>
class GenericValueImpl : public GenericValue
{
public:
	//-------------------------------------------------------------------------------------------------------
	// Konstruktur & Destruktor
	GenericValueImpl(const TypeT& Value);
	virtual ~GenericValueImpl();

	//-------------------------------------------------------------------------------------------------------
	virtual const TypeT& GetValue() const;

protected:
	TypeT value_;
};

typedef GenericValueImpl<udword_t, GenericValue::ValueTypeUDWord> XGenericUDwordValue;
typedef GenericValueImpl<float_t, GenericValue::ValueTypeFloat> XGenericFloatValue;
typedef GenericValueImpl<string_t, GenericValue::ValueTypeString> XGenericStringValue;

}}} // namespace x::templates::generic

#endif // TEMPLATES_GENERIC_GENERICVALUE_H
