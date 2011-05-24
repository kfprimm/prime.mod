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
/*!	\file tokens/tokenintegerlist.h
	\brief Definition von x::TokenIntegerList
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::TokenIntegerList
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef TOKENS_TOKENINTEGERLIST_H
#define TOKENS_TOKENINTEGERLIST_H

#include "../global/global.h"
#include "../temporalinfo.h"
#include "./token.h"

namespace x { namespace tokens {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Standalone-TokenIntegerList
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	TokenIntegerList welches als einzigen Wert seine ID besitzt
*/
class TokenIntegerList : public Token
{
public:
	//-------------------------------------------------------------------------------------------------------
	// Konstruktur & Destruktor
	TokenIntegerList();
	TokenIntegerList(const uint_t line);
	TokenIntegerList(const udword_t* values, const uint_t value_count);
	TokenIntegerList(const udword_t* values, const uint_t value_count, const uint_t line);
	virtual ~TokenIntegerList();

	const udword_t GetCount() const;
	const udword_t* const GetArray() const;

protected:
	udword_t count_;
	udword_t* array_;
};

}} // namespace x::tokens

#endif // TOKENS_TOKENINTEGERLIST_H
