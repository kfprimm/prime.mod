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
/*!	\file tokens/token.h
	\brief Definition von x::Token
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::Token
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef TOKENS_TOKEN_H
#define TOKENS_TOKEN_H

#include "../global/global.h"
#include "../temporalinfo.h"

namespace x { namespace tokens {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief X-File-Token
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	Klasse fürs Handling von Tokens
*/
class Token
{
public:
	enum Type
	{
		TypeName = 1,
		TypeString = 2,
		TypeInteger = 3,
		TypeGuid = 5,
		TypeIntegerList = 6,
		TypeFloatList = 7,

		// The stand-alone tokens are defined as follows.
		TypeOBrace = 10,
		TypeCBrace = 11,
		TypeOParen = 12,
		TypeCParen = 13,
		TypeOBracket = 14,
		TypeCBracket = 15,
		TypeOAngle = 16,
		TypeCAngle = 17,
		TypeDot = 18,
		TypeComma = 19,
		TypeSemicolon = 20,
		TypeTemplate = 31,
		TypeWord = 40,
		TypeDWord = 41,
		TypeFloat = 42,
		TypeDouble = 43,
		TypeChar = 44,
		TypeUChar = 45,
		TypeSWord = 46,
		TypeSDWord = 47,
		TypeVoid = 48,
		TypeLpStr = 49,
		TypeUnicode = 50,
		TypeCString = 51,
		TypeArray = 52
	};

public:
	//-------------------------------------------------------------------------------------------------------
	// Konstruktur & Destruktor
	Token(const Type type);
	Token(const Type type, const uint_t line);
	virtual ~Token();

	//-------------------------------------------------------------------------------------------------------
	virtual const Type GetType() const;
	virtual const uint_t GetLine() const;

protected:
	/*!
		\var type_
		\brief Token-Typ

		Definiert den Typ des Tokens.
	*//*!
		\var line_
		\brief Linie

		Die Zeile der Input-Datei auf welcher dieses Token definiert worden ist.
	*/
	
	Type type_;
	uint_t line_;
};

}} // namespace x::tokens

#endif // TOKENS_TOKEN_H
