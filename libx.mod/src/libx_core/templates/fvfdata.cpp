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
/*!	\file templates/fvfdata.cpp
	\brief Implementation von x::templates::FVFData
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::FVFData
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
FVFData::FVFData()
:	Template(TemplateTypeFVFData)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
FVFData::~FVFData()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void FVFData::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "FVFData" << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine Exakte Kopie dieses TemplateTypes und gibt sie zurück.
*/
Template* FVFData::Clone() const
{
	FVFData* clone = new FVFData;
	clone->fvf_ = fvf_;
	clone->dword_count_ = dword_count_;
	for (
		vector<udword_t>::const_iterator begin = data_.begin();
		begin != data_.end();
		++begin
	)
	{
		clone->data_.push_back(*begin);
	}
	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input.
*/
void FVFData::ReadDefinition(
	TemporalInfo& info
) 
{
	// DWORD dwFVF;
	{
		Token* dword_token = info.scanner->ReadToken(info);
		Token* name_token = info.scanner->ReadToken(info);

		if (dword_token->GetType() != Token::TypeDWord ||
			name_token->GetType() != Token::TypeName)
		{
			throw Exception(
				UNICODE_STRING("error in definition of 'DWORD dwFVF;'")
			);
		}
	}

	// DWORD nDWords;
	{
		Token* dword_token = info.scanner->ReadToken(info);
		Token* name_token = info.scanner->ReadToken(info);

		if (dword_token->GetType() != Token::TypeDWord ||
			name_token->GetType() != Token::TypeName)
		{
			throw Exception(
				UNICODE_STRING("error in definition of 'DWORD nDWords;'")
			);
		}
	}

	// array DWORD data[nDWords];
	{
		Token* array_token = info.scanner->ReadToken(info);
		Token* dword_token = info.scanner->ReadToken(info);
		Token* name_token = info.scanner->ReadToken(info);;
		Token* obracket_token = info.scanner->ReadToken(info);
		Token* dimension_name_token = info.scanner->ReadToken(info);
		Token* cbracket_token = info.scanner->ReadToken(info);

		if (array_token->GetType() != Token::TypeArray ||
			dword_token->GetType() != Token::TypeDWord ||
			name_token->GetType() != Token::TypeName ||
			obracket_token->GetType() != Token::TypeOBracket ||
			dimension_name_token->GetType() != Token::TypeName ||
			cbracket_token->GetType() != Token::TypeCBracket)
		{
			throw Exception(
				UNICODE_STRING("error in definition of 'array DWORD data[nDWords];'")
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

	Liest Daten dieses TemplateTypes.
*/
void FVFData::ReadData(
	TokenName* name_token,
	TokenGuid* guid_token,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	fvf_ = info.serializer.GetNextDword(info);
	dword_count_ = info.serializer.GetNextDword(info);
	data_.clear();
	for (udword_t i = 0; i < dword_count_; ++i)
		data_.push_back(info.serializer.GetNextDword(info));

	if (read_cbrace)
	{
		info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
	}
}

}} // namespace x::templates

