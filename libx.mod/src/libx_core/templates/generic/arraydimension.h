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
/*!	\file templates/generic/arraydimension.h
	\brief Definition von x::templates::generic::GenericTemplate
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::templates::generic::GenericTemplate
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef TEMPLATES_GENERIC_ARRAYDIMENSION_H
#define TEMPLATES_GENERIC_ARRAYDIMENSION_H

#include "../../global/global.h"
#include "../../temporalinfo.h"

namespace x { namespace templates { namespace generic {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief array dimensions class
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	saves information about the dimension of a generic array.
*/
struct ArrayDimension
{
	ArrayDimension();
	~ArrayDimension();

	/*!
		\var is_integer_dimension
		\brief flag to decide between dimension and dimension_name

		this flag signals if the dimension is given by an absolute integer value or
		by another variable in the same class. the first one will be stored in dimension
		for a reference the name of the referenced variable is stored in dimension_name.
	*//*!
		\var dimension
		\brief fixed dimension

		fixed dimension. this is only valid if is_integer_dimension = true
	*//*!
		\var dimension_name
		\brief name of a referenced variable

		stores the name of the variable with the array dimension if is_integer_dimension
		equals false.
	*/

	bool_t is_integer_dimension;
	uint_t dimension;
	string_t dimension_name;
};

}}} // namespace x::templates::generic

#endif // TEMPLATES_GENERIC_ARRAYDIMENSION_H
