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
	\file templates/generic/genericelement.h
	\brief Definition von x::templates::generic::GenericElement
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::templates::generic::GenericElement
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef TEMPLATES_GENERIC_GENERICELEMENT_H
#define TEMPLATES_GENERIC_GENERICELEMENT_H

#include "../../global/global.h"
#include "../../temporalinfo.h"
#include "./arraydimension.h"

namespace x { namespace templates { namespace generic {

enum ElementTypes
{
	ElementTypePrimitive,
	ElementTypeArray,
	ElementTypeReference
};

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief saves the description of an element 
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	saves the description for an element for a generic-element.
*/
struct GenericElement
{
public:
	typedef vector<ArrayDimension*> dimension_container_t;

public:
	GenericElement();
	~GenericElement();

public:
	ElementTypes type;

	tokens::Token::Type primitive_type;
	string_t primitive_name;

	bool_t is_array_primitive_type;
	tokens::Token::Type array_primitive_type;
	string_t array_name_type;
	string_t array_name;
	dimension_container_t array_dimension_list;

	string_t reference_type;
	string_t reference_name;
};

}}} // namespace x::templates::generic

#endif // TEMPLATES_GENERIC_GENERICELEMENT_H
