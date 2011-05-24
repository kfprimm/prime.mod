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
/*!	\file scanner/binaryscanner.cpp
	\brief Implementation von x::scanner::BinaryScanner
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::scanner::BinaryScanner
*/
//-----------------------------------------------------------------------------------------------------------
#define XLIB_CORE
#include "./binaryscanner.h"

namespace x { namespace scanner {

using namespace x::tokens;

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
BinaryScanner::BinaryScanner()
:	lookahead_token_(NULL),
	token_pool_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
BinaryScanner::~BinaryScanner()
{
	while (token_pool_.size() > 0)
	{
		Token* token = token_pool_.back();
		token_pool_.pop_back();
		delete token;
	}
}




//-------------------------------------------------------------------------------------------------------
bool_t BinaryScanner::Eof(
	TemporalInfo& info
)
{
	return info.input.Eof();
}




//-------------------------------------------------------------------------------------------------------
Token* BinaryScanner::ReadToken(
	TemporalInfo& info
)
{
	Token* token = NULL;

	if (lookahead_token_ != NULL)
	{
		token = lookahead_token_;
		lookahead_token_ = NULL;
	}

	if (token == NULL)
	{
		token = ReadTokenFromFile(info);
		token_pool_.push_back(token);
	}

	bool_t accepted = false;
	while(!accepted && token != NULL)
	{
		if (token->GetType() != Token::TypeComma && token->GetType() != Token::TypeSemicolon)
		{
			accepted = true;
		}
		else
		{
			token = ReadTokenFromFile(info);
			if (token != NULL)
			{
				token_pool_.push_back(token);
			}
		}
	}

	info.last_token_index = 0;
	info.last_token = token;
	return token;
}




//-------------------------------------------------------------------------------------------------------
Token* BinaryScanner::ReadExpectedToken(
	TemporalInfo& info,
	const Token::Type type
)
{
	Token* pToken = ReadToken(info);
	if (pToken->GetType() != type)
	{
		string_t strType;
		switch (type)
		{
		case Token::TypeName: strType = UNICODE_STRING("Token::TypeName"); break;
		case Token::TypeString: strType = UNICODE_STRING("Token::TypeString"); break;
		case Token::TypeInteger: strType = UNICODE_STRING("Token::TypeInteger"); break;
		case Token::TypeGuid: strType = UNICODE_STRING("Token::TypeGuid"); break;
		case Token::TypeIntegerList: strType = UNICODE_STRING("Token::TypeIntegerList"); break;
		case Token::TypeFloatList: strType = UNICODE_STRING("Token::TypeFloatList"); break;
		case Token::TypeOBrace: strType = UNICODE_STRING("Token::TypeOBrace"); break;
		case Token::TypeCBrace: strType = UNICODE_STRING("Token::TypeCBrace"); break;
		case Token::TypeOParen: strType = UNICODE_STRING("Token::TypeOParen"); break;
		case Token::TypeCParen: strType = UNICODE_STRING("Token::TypeCParen"); break;
		case Token::TypeOBracket: strType = UNICODE_STRING("Token::TypeOBracket"); break;
		case Token::TypeCBracket: strType = UNICODE_STRING("Token::TypeCBracket"); break;
		case Token::TypeOAngle: strType = UNICODE_STRING("Token::TypeOAngle"); break;
		case Token::TypeCAngle: strType = UNICODE_STRING("Token::TypeCAngle"); break;
		case Token::TypeDot: strType = UNICODE_STRING("Token::TypeDot"); break;
		case Token::TypeComma: strType = UNICODE_STRING("Token::TypeComma"); break;
		case Token::TypeSemicolon: strType = UNICODE_STRING("Token::TypeSemicolon"); break;
		case Token::TypeTemplate: strType = UNICODE_STRING("Token::TypeTemplate"); break;
		case Token::TypeWord: strType = UNICODE_STRING("Token::TypeWord"); break;
		case Token::TypeDWord: strType = UNICODE_STRING("Token::TypeDWord"); break;
		case Token::TypeFloat: strType = UNICODE_STRING("Token::TypeFloat"); break;
		case Token::TypeDouble: strType = UNICODE_STRING("Token::TypeDouble"); break;
		case Token::TypeChar: strType = UNICODE_STRING("Token::TypeChar"); break;
		case Token::TypeUChar: strType = UNICODE_STRING("Token::TypeUChar"); break;
		case Token::TypeSWord: strType = UNICODE_STRING("Token::TypeSWord"); break;
		case Token::TypeSDWord: strType = UNICODE_STRING("Token::TypeSDWord"); break;
		case Token::TypeVoid: strType = UNICODE_STRING("Token::TypeVoid"); break;
		case Token::TypeLpStr: strType = UNICODE_STRING("Token::TypeLpStr"); break;
		case Token::TypeUnicode: strType = UNICODE_STRING("Token::TypeUnicode"); break;
		case Token::TypeCString: strType = UNICODE_STRING("Token::TypeCString"); break;
		case Token::TypeArray: strType = UNICODE_STRING("Token::TypeArray"); break;
		}

		throw Exception(
			strType + UNICODE_STRING(" expected!")
		);
	}
	return pToken;
}




//-------------------------------------------------------------------------------------------------------
Token* BinaryScanner::ReadOptionalToken(
	TemporalInfo& info,
	const Token::Type type
)
{
	if (lookahead_token_ == NULL)
	{
		lookahead_token_ = ReadToken(info);
	}

	if (lookahead_token_->GetType() == type)
	{
		Token* token = lookahead_token_;
		lookahead_token_ = NULL;
		return token;
	}
	else
	{
		info.last_token = NULL;
		info.last_token_index = 0;
		return NULL;
	}
}




//-------------------------------------------------------------------------------------------------------
void BinaryScanner::ReadDeclaration(
	TemporalInfo& info,
	const Token::Type decl[],
	const uint_t decl_size,
	const string_t& decl_string
)
{
	try
	{
		for (uint_t i = 0; i < decl_size; ++i)
		{
			ReadExpectedToken(info, decl[i]);
		}
	}
	catch (Exception& e) 
	{
		throw Exception(
			string_t(UNICODE_STRING("error in definition of '")) + 
			decl_string + 
			UNICODE_STRING("', description: '") + 
			e.GetDescription() + 
			UNICODE_STRING("'")
		);
	}
}




//-------------------------------------------------------------------------------------------------------
Token* BinaryScanner::ReadNameTokenFromFile(
	TemporalInfo& info
)
{
	udword_t length;
	info.input >> length;
	string_t name;

	if (length > 127)
	{
		char* buffer = new char[length + 1];
		info.input.Read(reinterpret_cast<ubyte_t*>(buffer), length);
		buffer[length] = 0;

		name = buffer;
		delete[] buffer;
	}
	else
	{
		char buffer[128];
		info.input.Read(reinterpret_cast<ubyte_t*>(buffer), length);
		buffer[length] = 0;

		name = buffer;
	}

	return new TokenName(name);
}




//-------------------------------------------------------------------------------------------------------
Token* BinaryScanner::ReadStringTokenFromFile(
	TemporalInfo& info
)
{
	udword_t length;
	info.input >> length;

	string_t value;
	if (length > 127)
	{
		char* buffer = new char[length + 1];
		info.input.Read(reinterpret_cast<ubyte_t*>(buffer), length);
		buffer[length] = 0;

		value = buffer;
		delete[] buffer;
	}
	else
	{
		char buffer[128];
		info.input.Read(reinterpret_cast<ubyte_t*>(buffer), length);
		buffer[length] = 0;

		value = buffer;
	}

	uword_t terminator;
	info.input >> terminator;
	if (terminator != Token::TypeSemicolon &&
		terminator != Token::TypeComma)
	{
		throw Exception(
			UNICODE_STRING("Token::TypeSemicolon or Token::TypeComma expected!")
		);
	}

	return new TokenString(value);
}




//-------------------------------------------------------------------------------------------------------
Token* BinaryScanner::ReadIntegerTokenFromFile(
	TemporalInfo& info
)
{
	udword_t value;
	info.input >> value;

	TokenInteger* token = new TokenInteger();
	token->SetDWord(value);
	return token;
}




//-----------------------------------------------------------------------------------------------------------
Token* BinaryScanner::ReadGuidTokenFromFile(
	TemporalInfo& info
)
{
	udword_t field1;
	uword_t field2;
	uword_t field3;
	ubyte_t field4_1;
	ubyte_t field4_2;
	ubyte_t field4_3;
	ubyte_t field4_4;
	ubyte_t field4_5;
	ubyte_t field4_6;
	ubyte_t field4_7;
	ubyte_t field4_8;

	info.input >> field1;
	info.input >> field2;
	info.input >> field3;
	info.input >> field4_1;
	info.input >> field4_2;
	info.input >> field4_3;
	info.input >> field4_4;
	info.input >> field4_5;
	info.input >> field4_6;
	info.input >> field4_7;
	info.input >> field4_8;

	return new TokenGuid(Guid(
		field1,
		field2, field3, 
		field4_1, field4_2, field4_3, field4_4,
		field4_5, field4_6, field4_7, field4_8
	));
}




//-------------------------------------------------------------------------------------------------------
Token* BinaryScanner::ReadIntegerListTokenFromFile(
	TemporalInfo& info
)
{
	udword_t count;
	info.input >> count;
	if (count == 0)
	{
		count = 5;
	}
	udword_t* array = new udword_t[count];
	info.input.Read((ubyte_t*)array, 4 * count);
	Token* token = new TokenIntegerList(array, count);
	delete[] array;
	return token;
}




//-------------------------------------------------------------------------------------------------------
Token* BinaryScanner::ReadFloatListTokenFromFile(
	TemporalInfo& info
)
{
	udword_t count;
	info.input >> count;
	if (info.header.UseFloat32())
	{
		float_t* array = new float_t[count];
		info.input.Read((ubyte_t*)array, 4 * count);
		Token* token = new TokenFloatList(array, count);
		delete[] array;
		return token;
	}
	else
	{
		double_t* array = new double_t[count];
		info.input.Read((ubyte_t*)array, 8 * count);
		Token* token =  new TokenFloatList(array, count);
		delete[] array;
		return token;
	}
}




//-----------------------------------------------------------------------------------------------------------
Token* BinaryScanner::ReadTokenFromFile(
	TemporalInfo& info
)
{
	uword_t token_type;
	info.input >> token_type;

	Token* token;
	switch (token_type)
	{
	case Token::TypeName: token = ReadNameTokenFromFile(info); break;
	case Token::TypeString: token = ReadStringTokenFromFile(info); break;
	case Token::TypeInteger: token = ReadIntegerTokenFromFile(info); break;
	case Token::TypeGuid: token = ReadGuidTokenFromFile(info); break;
	case Token::TypeIntegerList: token = ReadIntegerListTokenFromFile(info); break;
	case Token::TypeFloatList: token = ReadFloatListTokenFromFile(info); break;
	default: token = new StandaloneToken(static_cast<Token::Type>(token_type)); break;
	}

	// info nachführen
	return token;
}

}} // namespace x::scanner

