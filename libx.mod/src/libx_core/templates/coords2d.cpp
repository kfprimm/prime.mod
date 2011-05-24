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
/*!	\file templates/coords2d.cpp
	\brief Implementation von x::temlates::Coords2d
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::temlates::Coords2d
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
Coords2d::Coords2d()
:	Template(TemplateTypeCoords2D),
	v_()
{
	v_.x = 0.0f;
	v_.y = 0.0f;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
Coords2d::~Coords2d()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void Coords2d::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "Coords2d" << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine Exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* Coords2d::Clone() const
{
	Coords2d* clone = new Coords2d;
	clone->v_.x = v_.x;
	clone->v_.y = v_.y;
	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input.
*/
void Coords2d::ReadDefinition(
	TemporalInfo& info
)
{
	// float u;
	{
		Token::Type decl[] = { Token::TypeFloat, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("float u;"));
	}

	// float v_;
	{
		Token::Type decl[] = { Token::TypeFloat, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("float v_;"));
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
void Coords2d::ReadData(
	TokenName* name_token,
	TokenGuid* guid_token,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	v_.x = info.serializer.GetNextFloat32(info);
	v_.y = info.serializer.GetNextFloat32(info);

	if (read_cbrace)
	{
		info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Referenz auf den Vektor zurückgeben
	\return Vektor

	Gibt eine Referenz auf den kaos::math::Vector3f dieses Templates zurück.
*/
const math::Vector2f& Coords2d::GetVector() const
{
	return v_;
}

}} // namespace x::templates

