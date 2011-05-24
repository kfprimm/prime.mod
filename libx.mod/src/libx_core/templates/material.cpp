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
/*!	\file templates/material.cpp
	\brief Implementation von x::templates::Material
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::Material
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
Material::Material()
:	Template(TemplateTypeMaterial),
	face_color_(NULL),
	power_(0.0f),
	specular_color_(NULL),
	emissive_color_(NULL),
	children_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
Material::~Material()
{
	SAFE_REMREF(face_color_);
	SAFE_REMREF(specular_color_);
	SAFE_REMREF(emissive_color_);

	for(
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
void Material::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "Material" << endl;
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
Template* Material::Clone() const
{
	Material* clone = new Material;

	/*
	 *	Zuerst werden alle Attribute sofern vorhanden kopiert
	 */
	if (face_color_ != NULL)
		clone->face_color_ = dynamic_cast<ColorRGBA*>(face_color_->Clone());
	clone->power_ = power_;
	if (specular_color_ != NULL)
		clone->specular_color_ = dynamic_cast<ColorRGB*>(specular_color_->Clone());
	if (emissive_color_ != NULL)
		clone->emissive_color_ = dynamic_cast<ColorRGB*>(emissive_color_->Clone());

	/*
	 *	Danach alle Children
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
void Material::ReadDefinition(
	TemporalInfo& info
)
{
	// ColorRGBA faceColor;
	{
		Token* type_token = info.scanner->ReadToken(info);
		Token* name_token = info.scanner->ReadToken(info);

		if (type_token->GetType() != Token::TypeName ||
			name_token->GetType() != Token::TypeName)
		{
			throw Exception(
				UNICODE_STRING("error in definition of 'ColorRGBA faceColor;'")
			);
		}
	}

	// FLOAT power;
	{
		Token* float_token = info.scanner->ReadToken(info);
		Token* name_token = info.scanner->ReadToken(info);

		if (float_token->GetType() != Token::TypeFloat ||
			name_token->GetType() != Token::TypeName)
		{
			throw Exception(
				UNICODE_STRING("error in definition of 'FLOAT power;'")
			);
		}
	}

	// ColorRGB specularColor;
	{
		Token* type_token = info.scanner->ReadToken(info);
		Token* name_token = info.scanner->ReadToken(info);

		if (type_token->GetType() != Token::TypeName ||
			name_token->GetType() != Token::TypeName)
		{
			throw Exception(
				UNICODE_STRING("error in definition of 'ColorRGB specularColor;'")
			);
		}
	}

	// ColorRGB emissiveColor;
	{
		Token* type_token = info.scanner->ReadToken(info);
		Token* name_token = info.scanner->ReadToken(info);

		if (type_token->GetType() != Token::TypeName ||
			name_token->GetType() != Token::TypeName)
		{
			throw Exception(
				UNICODE_STRING("error in definition of 'ColorRGB emissiveColor;'")
			);
		}
	}

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
void Material::ReadData(
	TokenName* name_token,
	TokenGuid* guid_token,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	if (!read_cbrace)
	{
		throw Exception(
			UNICODE_STRING("template 'Mesh' cannot be included without reading the final CBRACKET-token!")
		);
	}

	/*
	 *	Face-Color auslesen
	 */
	face_color_ = dynamic_cast<ColorRGBA*>(info.serializer.InstanciateTemplateByName(info, UNICODE_STRING("ColorRGBA")));
	face_color_->ReadData(NULL, NULL, info, false);

	/*
	 *	power auslesen
	 */
	power_ = info.serializer.GetNextFloat32(info);

	/*
	 *	Specular-Color und Emissive-Color auslesen
	 */
	specular_color_ = dynamic_cast<ColorRGB*>(info.serializer.InstanciateTemplateByName(info, UNICODE_STRING("ColorRGB")));
	specular_color_->ReadData(NULL, NULL, info, false);
	emissive_color_ = dynamic_cast<ColorRGB*>(info.serializer.InstanciateTemplateByName(info, UNICODE_STRING("ColorRGB")));
	emissive_color_->ReadData(NULL, NULL, info, false);

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
	\brief Farbe des Materials zurückgeben
	\return Farbe

	Gibt die Farbe des Materials unter AMbient und Diffuse-Licht zurück.
*/
const ColorRGBA* Material::GetFaceColor() const
{
	return face_color_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Reflektionsstärke zurückgeben
	\return Stärke

	Gibt die Reflektionsstärke des Materials unter Specular-Licht zurück.
*/
const float_t Material::GetPower() const
{
	return power_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Farbe der Specular-Highlights zurückgeben
	\return farbe

	Gibt die Farbe des Materials unter Specular-Licht zurück.
*/
const ColorRGB* Material::GetSpecularColor() const
{
	return specular_color_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Farbe des ausgestrahlten Lichts
	\return Farbe

	Gibt die Farbe des Lichts zurück welches dieses Material selbst ausstrahlt.
*/
const ColorRGB* Material::GetEmissiveColor() const
{
	return emissive_color_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Untergeordnete Templates zurückgeben
	\return Referenz auf den Vektor in welchem die Templates gespeichert sind

	Gibt eine Referenz auf den Vektor zurück in welchem die diesem Material untergeordneten Templates
	gespeichert sind. Dies könnten z.B. Texturen, etc. sein.
*/
const Material::children_container_t& Material::GetChildren() const
{
	return children_;
}

}} // namespace x::templates;

