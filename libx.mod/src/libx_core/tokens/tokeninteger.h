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
/*!	\file tokens/tokeninteger.h
	\brief Definition von x::TokenInteger
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::TokenInteger
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef TOKENS_TOKENINTEGER_H
#define TOKENS_TOKENINTEGER_H

#include "../global/global.h"
#include "../temporalinfo.h"
#include "./token.h"

namespace x { namespace tokens {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Standalone-TokenInteger
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	TokenInteger welches als einzigen Wert seine ID besitzt
*/
class TokenInteger : public Token
{
public:
	//-------------------------------------------------------------------------------------------------------
	// Konstruktur & Destruktor
	TokenInteger();
	TokenInteger(const uint_t line);
	virtual ~TokenInteger();

	virtual void SetDWord(const udword_t value);
	virtual const udword_t GetDword() const;

protected:
	udword_t value_;
};

}} // namespace x::tokens

#endif // TOKENS_TOKENINTEGER_H
