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
/*!	\file templates/headertemplate.cpp
	\brief Implementation von x::templates::HeaderTemplate
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::HeaderTemplate
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
HeaderTemplate::HeaderTemplate()
:	Template(TemplateTypeHeader),
	major_(0),
	minor_(0),
	flags_(0)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
HeaderTemplate::~HeaderTemplate()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void HeaderTemplate::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "Header" << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine Exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* HeaderTemplate::Clone() const
{
	HeaderTemplate* clone = new HeaderTemplate;
	clone->major_ = major_;
	clone->minor_ = minor_;
	clone->flags_ = flags_;
	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input.
*/
void HeaderTemplate::ReadDefinition(
	TemporalInfo& info
) 
{
	// WORD major;
	{
		Token* word_token = info.scanner->ReadToken(info);
		Token* name_token = info.scanner->ReadToken(info);

		if (word_token->GetType() != Token::TypeWord ||
			name_token->GetType() != Token::TypeName)
		{
			throw Exception(
				UNICODE_STRING("error in definition of 'WORD major;'")
			);
		}
	}

	// WORD minor;
	{
		Token* word_token = info.scanner->ReadToken(info);
		Token* name_token = info.scanner->ReadToken(info);

		if (word_token->GetType() != Token::TypeWord ||
			name_token->GetType() != Token::TypeName)
		{
			throw Exception(
				UNICODE_STRING("error in definition of 'WORD minor;'")
			);
		}
	}

	// DWORD flags;
	{
		Token* dword_token = info.scanner->ReadToken(info);
		Token* name_token = info.scanner->ReadToken(info);

		if (dword_token->GetType() != Token::TypeDWord ||
			name_token->GetType() != Token::TypeName)
		{
			throw Exception(
				UNICODE_STRING("error in definition of 'DWORD flags;'")
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
void HeaderTemplate::ReadData(
	TokenName* name_token,
	TokenGuid* guid_token,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	major_ = info.serializer.GetNextDword(info);
	minor_ = info.serializer.GetNextDword(info);
	flags_ = info.serializer.GetNextDword(info);

	if (read_cbrace)
	{
		info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
	}
}

}} // namespace x::templates

