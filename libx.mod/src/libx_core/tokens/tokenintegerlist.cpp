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
/*!	\file tokens/tokenintegerlist.cpp
	\brief Implementation von x::TokenIntegerList
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::TokenIntegerList
*/
//-----------------------------------------------------------------------------------------------------------
#define XLIB_CORE
#include "./tokenintegerlist.h"

namespace x { namespace tokens {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
TokenIntegerList::TokenIntegerList()
:	Token(Token::TypeIntegerList),
	count_(0),
	array_(NULL)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
TokenIntegerList::TokenIntegerList(
	const uint_t line
)
:	Token(Token::TypeIntegerList, line),
	count_(0),
	array_(NULL)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
TokenIntegerList::TokenIntegerList(
	const udword_t* values,
	const uint_t value_count
)
:	Token(Token::TypeIntegerList),
	count_(0),
	array_(NULL)
{
	count_ = value_count;
	array_ = new udword_t[count_];
	memcpy(array_, values, sizeof(udword_t)*value_count);
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
TokenIntegerList::TokenIntegerList(
	const udword_t* values,
	const uint_t value_count,
	const uint_t line
)
:	Token(Token::TypeIntegerList, line),
	count_(0),
	array_(NULL)
{
	count_ = value_count;
	array_ = new udword_t[count_];
	memcpy(array_, values, sizeof(udword_t)*value_count);
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
TokenIntegerList::~TokenIntegerList()
{
	delete[] array_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Anzahl Elemente im Array zurückgeben
	\return Anzahl

	Gibt die Anzahl der Elemente im Integer-Array zurück.
*/
const udword_t TokenIntegerList::GetCount() const
{
	return count_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Array zurückgeben
	\return Pointer

	Gibt einen Pointer auf den Beginn des Arrays zurück.
*/
const udword_t* const TokenIntegerList::GetArray() const
{
	return array_;
}

}} // namespace x::tokens

