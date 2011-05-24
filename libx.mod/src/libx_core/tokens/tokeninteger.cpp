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
/*!	\file tokens/tokeninteger.cpp
	\brief Implementation von x::TokenInteger
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::TokenInteger
*/
//-----------------------------------------------------------------------------------------------------------
#define XLIB_CORE
#include "./tokeninteger.h"

namespace x { namespace tokens {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
TokenInteger::TokenInteger()
:	Token(Token::TypeInteger)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
TokenInteger::TokenInteger(
	const uint_t line
)
:	Token(Token::TypeInteger, line)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
TokenInteger::~TokenInteger()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief set token value
	\param value

	sets this token's value.
*/
void TokenInteger::SetDWord(
	const udword_t value
)
{
	value_ = value;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Integer-Wert zurückgeben
	\return Wert

	Gibt den Wert dieses Tokens zurück.
*/
const udword_t TokenInteger::GetDword() const
{
	return value_;
}

}} // namespace x::tokens

