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
/*!	\file templates/animationset.cpp
	\brief Implementation von x::templates::AnimationSet
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::AnimationSet
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
AnimationSet::AnimationSet()
:	Template(TemplateTypeAnimationSet),
	animations_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
AnimationSet::~AnimationSet()
{
	typedef animation_container_t::iterator iterator;
	for (iterator begin = animations_.begin(); begin != animations_.end(); ++begin)
	{
		Animation* animation = (*begin);
		animation->RemRef();
	}
	animations_.clear();
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine Exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* AnimationSet::Clone() const
{
	AnimationSet* clone = new AnimationSet;

	typedef animation_container_t::const_iterator iterator;
	for (iterator begin = animations_.begin(); begin != animations_.end(); ++begin)
	{
		clone->animations_.push_back(dynamic_cast<Animation*>((*begin)->Clone()));
	}

	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input. Definition 
	des Templates gemäss DirectX-Dokumentation:

	\verbatim
		template AnimationSet
		{
			< 3D82AB50-62DA-11cf-AB39-0020AF71E433 >
			[ Animation < 3D82AB4F-62DA-11cf-AB39-0020AF71E433 > ]
		}
	\endverbatim
*/
void AnimationSet::ReadDefinition(
	TemporalInfo& info
)
{
	// [ Animation < 3D82AB4F-62DA-11cf-AB39-0020AF71E433 > ]
	{
		Token* obracket_token = info.scanner->ReadToken(info);
		Token* name_token = info.scanner->ReadToken(info);
		Token* guid_token = NULL;
		Token* cbracket_token = info.scanner->ReadToken(info);
		if (cbracket_token->GetType() == Token::TypeGuid)
		{
			guid_token = cbracket_token;
			cbracket_token = info.scanner->ReadToken(info);
		}

		if (obracket_token->GetType() != Token::TypeOBracket ||
			name_token->GetType() != Token::TypeName ||
			(guid_token != NULL && guid_token->GetType() != Token::TypeGuid) ||
			cbracket_token->GetType() != Token::TypeCBracket)
		{
			throw Exception(
				UNICODE_STRING("error in definition of '[ Animation < 3D82AB4F-62DA-11cf-AB39-0020AF71E433 > ]'")
			);
		}
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void AnimationSet::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "AnimationSet" << endl;
	typedef animation_container_t::const_iterator iterator;
	for (iterator begin = animations_.begin(); begin != animations_.end(); ++begin)
	{
		(*begin)->PrintFileStructure(level + 1);
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
void AnimationSet::ReadData(
	TokenName* name_token,
	TokenGuid* guid_token,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	if (!read_cbrace)
	{
		throw Exception(
			UNICODE_STRING("template 'AnimationSet' cannot be included without reading the final CBRACKET-token!")
		);
	}

	if (name_token)
	{
		name_ = name_token->GetName();
	}
	
	/*
	 *	[ Animation < 3D82AB4F-62DA-11cf-AB39-0020AF71E433 > ]
	 */
	bool_t coninue_flag = true;
	while (coninue_flag)
	{
		Token* name_token = info.scanner->ReadToken(info);
		switch (name_token->GetType())
		{
		case Token::TypeName: {
			Template* aimation = info.serializer.ReadTemplateData(name_token, info);
			if (aimation->GetType() != TemplateTypeAnimation)
			{
				throw Exception(
					UNICODE_STRING("'Animation'-template expected!")
				);
			}
			animations_.push_back(dynamic_cast<Animation*>(aimation));
			break;
		}
		case Token::TypeCBrace:
			coninue_flag = false;
			break;

		default:
			throw Exception(
				UNICODE_STRING("unexpected token occured!")
			);
		}
	}
}




//-----------------------------------------------------------------------------------------------------------
const string_t& AnimationSet::GetName() const
{
	return name_;
}





//-----------------------------------------------------------------------------------------------------------
const AnimationSet::animation_container_t& AnimationSet::GetAnimations() const
{
	return animations_;
}

}} // namespace x::templates

