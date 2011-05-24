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
/*!	\file templates/floatkeys.cpp
	\brief Implementation von x::templates::FloatKeys
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::FloatKeys
*/
//-----------------------------------------------------------------------------------------------------------
#define XLIB_CORE
#include "../global/global.h"
#include "../tokens/tokens.h"
#include "../serializer.h"
#include "./templates.h"

#include <string.h> // Kevin was here

namespace x { namespace templates {

using namespace x::tokens;

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
FloatKeys::FloatKeys()
:	Template(TemplateTypeFloatKeys),
	num_values_(0),
	values_(NULL)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
FloatKeys::~FloatKeys()
{
	SAFE_DELETE_ARRAY(values_);
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void FloatKeys::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "FloatKeys" << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* FloatKeys::Clone() const
{
	FloatKeys* clone = new FloatKeys;
	clone->num_values_ = num_values_;
	clone->values_ = new float_t[num_values_];
	memcpy(clone->values_, values_, sizeof(float_t) * num_values_);
	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input. Definition 
	des Templates gemäss DirectX-Dokumentation:

	\verbatim
		template SkinWeights
		{
			< 10DD46A9-775B-11cf-8F52-0040333594A3 >
			DWORD nValues;
			array float values[nValues];
		}
	\endverbatim
*/
void FloatKeys::ReadDefinition(
	TemporalInfo& info
)
{
	// DWORD nValues;
	{
		Token* dword_token = info.scanner->ReadToken(info);
		Token* name_token = info.scanner->ReadToken(info);

		if (
			dword_token->GetType() != Token::TypeDWord ||
			name_token->GetType() != Token::TypeName
		)
		{
			throw Exception(
				UNICODE_STRING("error in definition of 'DWORD nValues;'")
			);
		}
	}

	// array float values[nValues];
	{
		Token* array_token = info.scanner->ReadToken(info);
		Token* float_token = info.scanner->ReadToken(info);
		Token* name_token = info.scanner->ReadToken(info);;
		Token* obracket_token = info.scanner->ReadToken(info);
		Token* dimension_name_token = info.scanner->ReadToken(info);
		Token* cbracket_token = info.scanner->ReadToken(info);

		if (
			array_token->GetType() != Token::TypeArray ||
			float_token->GetType() != Token::TypeFloat ||
			name_token->GetType() != Token::TypeName ||
			obracket_token->GetType() != Token::TypeOBracket ||
			dimension_name_token->GetType() != Token::TypeName ||
			cbracket_token->GetType() != Token::TypeCBracket
		)
		{
			throw Exception(
				UNICODE_STRING("error in definition of 'array float values[nValues];'")
			);
		}
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template-Daten lesen
	\param name_token
	\param guid_token
	\param info
	\param read_cbrace

	Liest Daten dieses Templates.
*/
void FloatKeys::ReadData(
	TokenName* name_token,
	TokenGuid* guid_token,
	TemporalInfo& info,
	bool_t read_cbrace
)
{
	// Ev. Name auslesen falls vorhanden
	num_values_ = info.serializer.GetNextDword(info);
	SAFE_DELETE_ARRAY(values_);
	values_ = new float_t[num_values_];
	for (dword_t i = 0; i < num_values_; ++i)
		values_[i] = info.serializer.GetNextFloat32(info);

	if (read_cbrace)
	{
		info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
	}
}




//-----------------------------------------------------------------------------------------------------------
const dword_t FloatKeys::GetNumValues() const
{
	return num_values_;
}




//-----------------------------------------------------------------------------------------------------------
const float_t* FloatKeys::GetValues() const
{
	return values_;
}

}} // namespace x::templates

