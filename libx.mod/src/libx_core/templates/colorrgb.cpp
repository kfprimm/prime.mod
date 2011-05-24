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
/*!	\file templates/colorrgb.cpp
	\brief Implementation von x::templates::ColorRGB
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::ColorRGB
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
ColorRGB::ColorRGB()
:	Template(TemplateTypeColorRGB),
	c_()
{
	c_.a = 1.0f;
	c_.r = 1.0f;
	c_.g = 1.0f;
	c_.b = 1.0f;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
ColorRGB::~ColorRGB()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void ColorRGB::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "ColorRGB" << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine Exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* ColorRGB::Clone() const
{
	ColorRGB* clone = new ColorRGB;
	clone->c_.a = c_.a;
	clone->c_.r = c_.r;
	clone->c_.g = c_.g;
	clone->c_.b = c_.b;
	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input.
*/
void ColorRGB::ReadDefinition(
	TemporalInfo& info
) 
{
	// float red;
	{
		Token::Type decl[] = { Token::TypeFloat, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("float red;"));
	}

	// float green;
	{
		Token::Type decl[] = { Token::TypeFloat, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("float green;"));
	}

	// float blue;
	{
		Token::Type decl[] = { Token::TypeFloat, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("float blue;"));
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
void ColorRGB::ReadData(
	TokenName* name_token,
	TokenGuid* guid_token,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	c_.r = info.serializer.GetNextFloat32(info);
	c_.g = info.serializer.GetNextFloat32(info);
	c_.b = info.serializer.GetNextFloat32(info);

	if (read_cbrace)
	{
		info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Farbe zurückgeben
	\return Farbe

	Gibt die Farbe dieses Templates zurück.
*/
const math::Colorf& ColorRGB::GetColor() const
{
	return c_;
}

}} // namespace x::templates

