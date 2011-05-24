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
/*!	\file templates/animation.cpp
	\brief Implementation von x::templates::Animation
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::Animation
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
Animation::Animation()
:	Template(TemplateTypeAnimation),
	name_(),
	reference_(),
	children_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
Animation::~Animation()
{
	typedef children_container_t::iterator iterator;
	for (iterator begin = children_.begin(); begin != children_.end(); ++begin)
	{
		Template* t = (*begin);
		t->RemRef();
	}
	children_.clear();
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void Animation::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "Animation" << endl;
	typedef children_container_t::const_iterator iterator;
	for (iterator begin = children_.begin(); begin != children_.end(); ++begin)
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
Template* Animation::Clone() const
{
	Animation* clone = new Animation;
	clone->name_ = name_;
	clone->reference_ = reference_;

	// Alle Children kopieren
	typedef children_container_t::const_iterator iterator;
	for (iterator begin = children_.begin(); begin != children_.end(); ++begin)
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
void Animation::ReadDefinition(
	TemporalInfo& info
)
{
	// [...]
	{
		Token::Type decl[] = { Token::TypeOBracket, Token::TypeDot, Token::TypeDot, Token::TypeDot, Token::TypeCBracket };
		info.scanner->ReadDeclaration(info, decl, 5, UNICODE_STRING("[...]"));
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
void Animation::ReadData(
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
	bool_t continue_flag = true;
	while (continue_flag)
	{
		Token* next_token = info.scanner->ReadToken(info);

		switch (next_token->GetType())
		{
		case Token::TypeName: {
			children_.push_back(
				info.serializer.ReadTemplateData(next_token, info)
			);
			break;
		}
		case Token::TypeOBrace: {
			// we've got a reference!
			Token* reference_name = info.scanner->ReadToken(info);
			if (reference_name->GetType() == Token::TypeName)
			{
				reference_ = dynamic_cast<TokenName*>(reference_name)->GetName();
			}
			Token* cbrace = info.scanner->ReadToken(info);
			if (cbrace->GetType() != Token::TypeCBrace)
			{
				cbrace = info.scanner->ReadToken(info);
			}
			if (cbrace->GetType() != Token::TypeCBrace)
			{
				throw Exception(
					UNICODE_STRING("Unexpected Token")
				);
			}
			break;
		}
		case Token::TypeCBrace: {
			continue_flag = false;
			break;
		}
		default:
			throw Exception(
				UNICODE_STRING("Unexpected Token")
			);
		}
	}
}




//-----------------------------------------------------------------------------------------------------------
const string_t& Animation::GetName() const
{
	return name_;
}




//-----------------------------------------------------------------------------------------------------------
const string_t& Animation::GetReference() const
{
	return reference_;
}




//-----------------------------------------------------------------------------------------------------------
const Animation::children_container_t& Animation::GetChildren() const
{
	return children_;
}

}} // namespace x::templates

