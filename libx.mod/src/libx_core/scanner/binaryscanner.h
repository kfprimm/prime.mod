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
/*!	\file scanner/binaryscanner.h
	\brief Definition von x::scanner::BinaryScanner
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::scanner::BinaryScanner
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef SCANNER_BINARYSCANNER_H
#define SCANNER_BINARYSCANNER_H

#include "../global/global.h"
#include "../tokens/tokens.h"
#include "./scanner.h"

namespace x { namespace scanner {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief X-File-BinaryScanner
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	Klasse fürs Handling von Tokens
*/
class BinaryScanner : public Scanner
{
public:
	//-------------------------------------------------------------------------------------------------------
	// Konstruktur & Destruktor
	BinaryScanner();
	virtual ~BinaryScanner();

	//-------------------------------------------------------------------------------------------------------
	virtual bool_t Eof(TemporalInfo& info);
	virtual tokens::Token* ReadToken(TemporalInfo& info);
	virtual tokens::Token* ReadExpectedToken(TemporalInfo& info, const tokens::Token::Type type);
	virtual tokens::Token* ReadOptionalToken(TemporalInfo& info, const tokens::Token::Type type);
	virtual void ReadDeclaration(TemporalInfo& info, const tokens::Token::Type decl[], const uint_t decl_size, const string_t& decl_string);

protected:
	tokens::Token* ReadNameTokenFromFile(TemporalInfo& info);
	tokens::Token* ReadStringTokenFromFile(TemporalInfo& info);
	tokens::Token* ReadIntegerTokenFromFile(TemporalInfo& info);
	tokens::Token* ReadGuidTokenFromFile(TemporalInfo& info);
	tokens::Token* ReadIntegerListTokenFromFile(TemporalInfo& info);
	tokens::Token* ReadFloatListTokenFromFile(TemporalInfo& info);
	tokens::Token* ReadTokenFromFile(TemporalInfo& info);

protected:
	tokens::Token* lookahead_token_;
	vector<tokens::Token*> token_pool_;
};

}} // namespace x::scanner

#endif // SCANNER_BINARYSCANNER_H
