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
/*!	\file scanner/scanner.h
	\brief Definition von x::scanner::Scanner
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::scanner::Scanner
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef SCANNER_SCANNER_H
#define SCANNER_SCANNER_H

#include "../global/global.h"
#include "../tokens/tokens.h"

namespace x { namespace scanner {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief X-File-Scanner
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	Klasse fürs Handling von Tokens
*/
class Scanner
{
public:
	//-------------------------------------------------------------------------------------------------------
	// Konstruktur & Destruktor
	Scanner();
	virtual ~Scanner();

	//-------------------------------------------------------------------------------------------------------
	virtual bool_t Eof(TemporalInfo& info) = 0;
	virtual tokens::Token* ReadToken(TemporalInfo& info) = 0;
	virtual tokens::Token* ReadExpectedToken(TemporalInfo& info, const tokens::Token::Type type) = 0;
	virtual tokens::Token* ReadOptionalToken(TemporalInfo& info, const tokens::Token::Type type) = 0;
	virtual void ReadDeclaration(TemporalInfo& info, const tokens::Token::Type decl[], const uint_t decl_size, const string_t& decl_string) = 0;
};

}} // namespace x::scanner

#endif // SCANNER_SCANNER_H
