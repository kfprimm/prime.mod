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
/*!	\file tokens/tokenname.cpp
	\brief Implementation von x::TokenName
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::TokenName
*/
//-----------------------------------------------------------------------------------------------------------
#define XLIB_CORE
#include "../serializer.h"
#include "./tokenname.h"

namespace x { namespace tokens {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
TokenName::TokenName()
:	Token(Token::TypeName),
	name_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
TokenName::TokenName(
	const uint_t line
)
:	Token(Token::TypeName, line),
	name_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
TokenName::TokenName(
	const string_t& name
)
:	Token(Token::TypeName),
	name_(name)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
TokenName::TokenName(
	const string_t& name,
	const uint_t line
)
:	Token(Token::TypeName, line),
	name_(name)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
TokenName::~TokenName()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Namen zurückgeben
	\return Name

	Gibt den Namensinhalt dieses Tokens zurück.
*/
const string_t& TokenName::GetName() const
{
	return name_;
}

}} // namespace x::tokens

