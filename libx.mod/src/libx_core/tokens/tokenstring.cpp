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
/*!	\file tokens/tokenstring.cpp
	\brief Implementation von x::TokenString
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::TokenString
*/
//-----------------------------------------------------------------------------------------------------------
#define XLIB_CORE
#include "../serializer.h"
#include "./tokenstring.h"

namespace x { namespace tokens {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
TokenString::TokenString()
:	Token(Token::TypeString),
	value_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
TokenString::TokenString(
	const uint_t line
)
:	Token(Token::TypeString, line),
	value_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
TokenString::TokenString(
	const string_t& value
)
:	Token(Token::TypeString),
	value_(value)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
TokenString::TokenString(
	const string_t& value,
	const uint_t line
)
:	Token(Token::TypeString, line),
	value_(value)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
TokenString::~TokenString()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Namen zurückgeben
	\return Name

	Gibt den Namensinhalt dieses Tokens zurück.
*/
const string_t& TokenString::GetValue() const
{
	return value_;
}

}} // namespace x::tokens

