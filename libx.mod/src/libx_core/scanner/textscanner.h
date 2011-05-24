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
/*!	\file scanner/textscanner.h
	\brief Definition von x::scanner::TextScanner
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::scanner::TextScanner
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef SCANNER_TEXTSCANNER_H
#define SCANNER_TEXTSCANNER_H

#include "../global/global.h"
#include "../tokens/tokens.h"
#include "./scanner.h"

namespace x { namespace scanner {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief X-File-TextScanner
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	Klasse fürs Handling von Tokens
*/
class TextScanner : public Scanner
{
public:
	//-------------------------------------------------------------------------------------------------------
	// Konstruktur & Destruktor
	TextScanner();
	virtual ~TextScanner();

	//-------------------------------------------------------------------------------------------------------
	virtual bool_t Eof(TemporalInfo& info);
	virtual tokens::Token* ReadToken(TemporalInfo& info);
	virtual tokens::Token* ReadExpectedToken(TemporalInfo& info, const tokens::Token::Type type);
	virtual tokens::Token* ReadOptionalToken(TemporalInfo& info, const tokens::Token::Type type);
	virtual void ReadDeclaration(TemporalInfo& info, const tokens::Token::Type decl[], const uint_t decl_size, const string_t& decl_string);

protected:
	schar_t ReadCharacter(TemporalInfo& info);
	void SkipWhitespaces(TemporalInfo& info);
	void EatComment(TemporalInfo& info);
	void ReadGuidField(TemporalInfo& info, schar_t* field, const uint_t length);
	void ReadGuidSeparator(TemporalInfo& info);
	udword_t HexStringToInt(const schar_t* field, const uint_t length);
	void ReadGuidTokenFromFile(TemporalInfo& info);
	void ReadNumber(TemporalInfo& info, string_t& number_string, bool_t& with_sign, bool_t& integral);
	void ReadNumberTokenFromFile(TemporalInfo& info);
	void ReadStringTokenFromFile(TemporalInfo& info);
	void ReadTokenFromFile(TemporalInfo& info);

protected:
	vector<tokens::Token*> lookahead_pool_;
	vector<tokens::Token*> token_pool_;
};

}} // namespace x::scanner

#endif // SCANNER_TEXTSCANNER_H
