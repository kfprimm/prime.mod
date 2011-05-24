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
/*!	\file templates/timedfloatkeys.cpp
	\brief Implementation von x::templates::TimedFloatKeys
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::TimedFloatKeys
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
TimedFloatKeys::TimedFloatKeys()
:	Template(TemplateTypeTimedFloatKeys),
	time_(0),
	keys_(NULL)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
TimedFloatKeys::~TimedFloatKeys()
{
	SAFE_REMREF(keys_);
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void TimedFloatKeys::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "TimedFloatKeys" << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* TimedFloatKeys::Clone() const
{
	TimedFloatKeys* clone = new TimedFloatKeys;
	clone->time_ = time_;
	if (keys_ != NULL)
		clone->keys_ = dynamic_cast<FloatKeys*>(keys_->Clone());
	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input. Definition 
	des Templates gemäss DirectX-Dokumentation:

	\verbatim
		template TimedFloatKeys
		{
			< F406B180-7B3B-11CF-8F52-0040333594A3 >
			DWORD time;
			FloatKeys tfkeys;
		}
	\endverbatim
*/
void TimedFloatKeys::ReadDefinition(
	TemporalInfo& info
)
{
	// DWORD time;
	{
		Token::Type decl[] = { Token::TypeDWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("DWORD time;"));
	}

	// FloatKeys tfkeys;
	{
		Token::Type decl[] = { Token::TypeName, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("FloatKeys tfkeys;"));
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
void TimedFloatKeys::ReadData(
	TokenName* name_token,
	TokenGuid* guid_token,
	TemporalInfo& info,
	bool_t read_cbrace
)
{
	// Ev. Name auslesen falls vorhanden
	time_ = info.serializer.GetNextDword(info);
	keys_ = dynamic_cast<FloatKeys*>(info.serializer.InstanciateTemplateByName(info, UNICODE_STRING("FloatKeys")));
	keys_->ReadData(NULL, NULL, info, false);

	if (read_cbrace)
	{
		info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
	}
}




//-----------------------------------------------------------------------------------------------------------
const dword_t TimedFloatKeys::GetTime() const
{
	return time_;
}




//-----------------------------------------------------------------------------------------------------------
const FloatKeys* TimedFloatKeys::GetKeys() const
{
	return keys_;
}

}} // namespace x::templates

