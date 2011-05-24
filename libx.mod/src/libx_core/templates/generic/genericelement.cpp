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
	\file templates/generic/genericelement.cpp
	\brief Implementation von x::templates::generic::GenericElement
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::generic::GenericElement
*/
//-----------------------------------------------------------------------------------------------------------
#define XLIB_CORE
#include "../../global/global.h"
#include "../../scanner/scanner.h"
#include "../../tokens/tokens.h"
#include "../../serializer.h"
#include "./genericelement.h"

namespace x { namespace templates { namespace generic {

using namespace x::tokens;

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
GenericElement::GenericElement()
:	type(ElementTypePrimitive),
	primitive_type(Token::TypeWord),
	primitive_name(),
	is_array_primitive_type(false),
	array_primitive_type(Token::TypeWord),
	array_name_type(),
	array_name(),
	array_dimension_list(),
	reference_type(),
	reference_name()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief dtor

	dtor
*/
GenericElement::~GenericElement()
{
	for (
		dimension_container_t::iterator begin = array_dimension_list.begin();
		begin != array_dimension_list.end();
		++begin
	)
	{
		delete (*begin);
	}
}

}}} // namespace x::templates::generic

