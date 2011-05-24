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
/*!	\file templates/frame.cpp
	\brief Implementation von x::templates::Frame
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::Frame
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
Frame::Frame()
:	Template(TemplateTypeFrame),
	children_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
Frame::~Frame()
{
	for (
		children_container_t::iterator begin = children_.begin();
		begin != children_.end();
		++begin
	)
	{
		Template* current_template = (*begin);
		current_template->RemRef();
	}
	children_.clear();
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void Frame::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "Frame" << endl;
	for (
		children_container_t::const_iterator begin = children_.begin();
		begin != children_.end();
		++begin
	)
	{
		(*begin)->PrintFileStructure(level + 1);
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine Exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* Frame::Clone() const
{
	Frame* clone = new Frame;
	clone->name_ = name_;

	/*
	 *	Alle Children kopieren
	 */
	for (
		children_container_t::const_iterator begin = children_.begin();
		begin != children_.end();
		++begin
	)
	{
		clone->children_.push_back((*begin)->Clone());
	}

	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input.
*/
void Frame::ReadDefinition(
	TemporalInfo& info
) 
{
	// [...]
	{
		Token* obracket_token = info.scanner->ReadToken(info);
		Token* point1_token = info.scanner->ReadToken(info);
		Token* point2_token = info.scanner->ReadToken(info);
		Token* point3_token = info.scanner->ReadToken(info);
		Token* cbracket_token = info.scanner->ReadToken(info);

		if (obracket_token->GetType() != Token::TypeOBracket ||
			point1_token->GetType() != Token::TypeDot ||
			point2_token->GetType() != Token::TypeDot ||
			point3_token->GetType() != Token::TypeDot ||
			cbracket_token->GetType() != Token::TypeCBracket)
		{
			throw Exception(
				UNICODE_STRING("error in definition of '[...]'")
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
void Frame::ReadData(
	TokenName* name_token,
	TokenGuid* guid_token,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	if (!read_cbrace)
	{
		throw Exception(
			UNICODE_STRING("template 'Frame' cannot be included without reading the final CBRACKET-token!")
		);
	}

	/*
	 *	Namen übernehmen, falls der Name gegeben ist
	 */
	if (name_token != NULL)
		name_ = name_token->GetName();


	/*
	 *	[...] bedeutet "open-end", d.h. es kann eigentlich alles noch als Child-Element
	 *	dieses Templates kommen...
	 */
	bool_t do_continue = true;
	while (do_continue)
	{
		Token* next_token = info.scanner->ReadToken(info);

		switch (next_token->GetType())
		{
		case Token::TypeName:
			children_.push_back(
				info.serializer.ReadTemplateData(next_token, info)
			);
			break;

		case Token::TypeCBrace:
			do_continue = false;
			break;

		default:
			throw Exception(
				UNICODE_STRING("Unexpected Token")
			);
		}
	}
}



//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Frame-Namen zurückgeben
	\return Name

	Gibt den Namen dieses Frames zurück.
*/
const string_t& Frame::GetName() const
{
	return name_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Vektor mit den Children zurückgeben
	\return Referenz auf den Vektor

	Gibt eine Referenz auf den Vektor mit allen diesem Frame untergeordneten Templates
	zurück.
*/
const Frame::children_container_t& Frame::GetChildren() const
{
	return children_;
}

}} // namespace x::templates

