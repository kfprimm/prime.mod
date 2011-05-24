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
/*!	\file templates/animationkey.cpp
	\brief Implementation von x::templates::AnimationKey
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::AnimationKey
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
AnimationKey::AnimationKey()
:	Template(TemplateTypeAnimationKey),
	key_type_(0),
	num_keys_(0),
	keys_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
AnimationKey::~AnimationKey()
{
	typedef keys_container_t::iterator iterator;
	for (iterator begin = keys_.begin(); begin != keys_.end(); ++begin)
	{
		TimedFloatKeys* key = (*begin);
		key->RemRef();
	}
	keys_.clear();
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void AnimationKey::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "AnimationKey" << endl;
	typedef keys_container_t::const_iterator iterator;
	for (iterator begin = keys_.begin(); begin != keys_.end(); ++begin)
	{
		(*begin)->PrintFileStructure(level + 1);
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* AnimationKey::Clone() const
{
	AnimationKey* clone = new AnimationKey;
	clone->key_type_ = key_type_;
	clone->num_keys_ = num_keys_;
	typedef keys_container_t::const_iterator iterator;
	for (iterator begin = keys_.begin(); begin != keys_.end(); ++begin)
	{
		clone->keys_.push_back(dynamic_cast<TimedFloatKeys*>((*begin)->Clone()));
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
		template SkinWeights
		{
			< 10DD46A8-775B-11CF-8F52-0040333594A3 >
			DWORD keyType
			DWORD nKeys;
			array TimedFloatKeys keys[nKeys];
		}
	\endverbatim
*/
void AnimationKey::ReadDefinition(
	TemporalInfo& info
)
{
	// DWORD keyType;
	{
		Token* dword_token = info.scanner->ReadToken(info);
		Token* name_token = info.scanner->ReadToken(info);

		if (
			dword_token->GetType() != Token::TypeDWord ||
			name_token->GetType() != Token::TypeName
		)
		{
			throw Exception(
				UNICODE_STRING("error in definition of 'DWORD keyType;'")
			);
		}
	}

	// DWORD nKeys;
	{
		Token* dword_token = info.scanner->ReadToken(info);
		Token* name_token = info.scanner->ReadToken(info);

		if (
			dword_token->GetType() != Token::TypeDWord ||
			name_token->GetType() != Token::TypeName
		)
		{
			throw Exception(
				UNICODE_STRING("error in definition of 'DWORD nKeys;'")
			);
		}
	}

	// array TimedFloatKeys keys[nKeys];
	{
		Token* array_token = info.scanner->ReadToken(info);
		Token* type_token = info.scanner->ReadToken(info);
		Token* name_token = info.scanner->ReadToken(info);;
		Token* obracket_token = info.scanner->ReadToken(info);
		Token* dimension_name_token = info.scanner->ReadToken(info);
		Token* cbracket_token = info.scanner->ReadToken(info);

		if (
			array_token->GetType() != Token::TypeArray ||
			type_token->GetType() != Token::TypeName ||
			name_token->GetType() != Token::TypeName ||
			obracket_token->GetType() != Token::TypeOBracket ||
			dimension_name_token->GetType() != Token::TypeName ||
			cbracket_token->GetType() != Token::TypeCBracket
		)
		{
			throw Exception(
				UNICODE_STRING("error in definition of 'array TimedFloatKeys keys[nKeys];'")
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
void AnimationKey::ReadData(
	TokenName* name_token,
	TokenGuid* guid_token,
	TemporalInfo& info,
	bool_t read_cbrace
)
{
	// Ev. Name auslesen falls vorhanden
	key_type_ = info.serializer.GetNextDword(info);
	num_keys_ = info.serializer.GetNextDword(info);

	typedef keys_container_t::iterator iterator;
	for (iterator begin = keys_.begin(); begin != keys_.end(); ++begin)
	{
		TimedFloatKeys* key = (*begin);
		key->RemRef();
	}
	keys_.clear();

	for (dword_t i = 0; i < num_keys_; ++i)
	{
		TimedFloatKeys* key = dynamic_cast<TimedFloatKeys*>(info.serializer.InstanciateTemplateByName(info, UNICODE_STRING("TimedFloatKeys")));
		key->ReadData(NULL, NULL, info, false);
		keys_.push_back(key);
	}

	if (read_cbrace)
	{
		info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
	}
}




//-----------------------------------------------------------------------------------------------------------
const dword_t AnimationKey::GetKeyType() const
{
	return key_type_;
}




//-----------------------------------------------------------------------------------------------------------
const dword_t AnimationKey::GetNumKeys() const
{
	return num_keys_;
}




//-----------------------------------------------------------------------------------------------------------
const AnimationKey::keys_container_t& AnimationKey::GetKeys() const
{
	return keys_;
}

}} // namespace x::templates

