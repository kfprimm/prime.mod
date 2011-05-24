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
/*!	\file templates/effectparamfloats.cpp
	\brief Implementation von x::templates::EffectParamFloats
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::EffectParamFloats
*/
//-----------------------------------------------------------------------------------------------------------
#define XLIB_CORE
#include "../global/global.h"
#include "../tokens/tokens.h"
#include "../serializer.h"
#include "./templates.h"

namespace x { namespace templates {

using namespace x::tokens;

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
EffectParamFloats::EffectParamFloats()
:	Template(TemplateTypeEffectParamFloats),
	param_name_(),
	float_count_(0),
	floats_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
EffectParamFloats::~EffectParamFloats()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void EffectParamFloats::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "EffectParamFloats" << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* EffectParamFloats::Clone() const
{
	EffectParamFloats* clone = new EffectParamFloats;

	// Koordinaten klonen
	clone->param_name_ = param_name_;
	clone->float_count_ = float_count_;
	for (
		vector<float_t>::const_iterator begin = floats_.begin();
		begin != floats_.end();
		++begin
	)
	{
		clone->floats_.push_back(*begin);
	}

	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input.
*/
void EffectParamFloats::ReadDefinition(
	TemporalInfo& info
) 
{
	// STRING ParamName;
	{
		Token* string_token = info.scanner->ReadToken(info);
		Token* name_token = info.scanner->ReadToken(info);

		if (string_token->GetType() != Token::TypeLpStr ||
			name_token->GetType() != Token::TypeName)
		{
			throw Exception(
				UNICODE_STRING("error in definition of 'String ParamName;'")
			);
		}
	}

	// DWORD nFloats;
	{
		Token* dword_token = info.scanner->ReadToken(info);
		Token* name_token = info.scanner->ReadToken(info);

		if (dword_token->GetType() != Token::TypeDWord ||
			name_token->GetType() != Token::TypeName)
		{
			throw Exception(
				UNICODE_STRING("error in definition of 'DWORD nFloats;'")
			);
		}
	}

	// array float Floats[nFloats];
	{
		Token* array_token = info.scanner->ReadToken(info);
		Token* float_token = info.scanner->ReadToken(info);
		Token* name_token = info.scanner->ReadToken(info);;
		Token* obracket_token = info.scanner->ReadToken(info);
		Token* dimension_name_token = info.scanner->ReadToken(info);
		Token* cbracket_token = info.scanner->ReadToken(info);

		if (array_token->GetType() != Token::TypeArray ||
			float_token->GetType() != Token::TypeFloat ||
			name_token->GetType() != Token::TypeName ||
			obracket_token->GetType() != Token::TypeOBracket ||
			dimension_name_token->GetType() != Token::TypeName ||
			cbracket_token->GetType() != Token::TypeCBracket)
		{
			throw Exception(
				UNICODE_STRING("error in definition of 'array float Floats[nFloats];'")
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
void EffectParamFloats::ReadData(
	TokenName* name_token,
	TokenGuid* guid_token,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	Token* param_name_token = info.scanner->ReadToken(info);
	if (param_name_token->GetType() != Token::TypeString)
	{
		throw Exception(
			UNICODE_STRING("Token::TypeString expected!")
		);
	}
	param_name_ = dynamic_cast<TokenString*>(param_name_token)->GetValue();
	float_count_ = info.serializer.GetNextDword(info);
	for (udword_t i = 0; i < float_count_; ++i)
		floats_.push_back(info.serializer.GetNextFloat32(info));

	/*
	 *	Das nächste Token muss die Daten abschliessen, falls es überhaupt gelesen werden muss
	 */
	if (read_cbrace)
	{
		info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
	}
}

}} // namespace x::templates

