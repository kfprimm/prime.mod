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
/*!	\file tokens/token.cpp
	\brief Implementation von x::Token
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::Token
*/
//-----------------------------------------------------------------------------------------------------------
#define XLIB_CORE
#include "./token.h"

namespace x { namespace tokens {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
Token::Token(
	const Type type
)
:	type_(type),
	line_(0)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
Token::Token(
	const Type type,
	const uint_t line
)
:	type_(type),
	line_(line)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
Token::~Token()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Typ des Tokens zurückgeben
	\return Typ

	Gibt den Typ dieses Tokens zurück.
*/
const Token::Type Token::GetType() const
{
	return type_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\return Zeile auf der das Token definiert worden ist.
*/
const uint_t Token::GetLine() const
{
	return line_;
}

}} // namespace x::tokens

