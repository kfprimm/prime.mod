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
/*!	\file templates/vector.cpp
	\brief Implementation von x::templates::Vector
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::Vector
*/
//-----------------------------------------------------------------------------------------------------------
#define XLIB_CORE
#include "../global/global.h"
#include "../scanner/scanner.h"
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
Vector::Vector()
:	Template(TemplateTypeVector),
	v_()
{
	v_.x = 0.0f;
	v_.y = 0.0f;
	v_.z = 0.0f;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
Vector::~Vector()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void Vector::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "Vector" << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine Exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* Vector::Clone() const
{
	Vector* clone = new Vector;
	clone->v_.x = v_.x;
	clone->v_.y = v_.y;
	clone->v_.z = v_.z;
	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input.
*/
void Vector::ReadDefinition(
	TemporalInfo& info
)
{
	// float x;
	{
		Token::Type decl[] = { Token::TypeFloat, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("float x;"));
	}

	// float y;
	{
		Token::Type decl[] = { Token::TypeFloat, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("float y;"));
	}

	// float z;
	{
		Token::Type decl[] = { Token::TypeFloat, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("float z;"));
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
void Vector::ReadData(
	TokenName* name_token,
	TokenGuid* guid_token,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	v_.x = info.serializer.GetNextFloat32(info);
	v_.y = info.serializer.GetNextFloat32(info);
	v_.z = info.serializer.GetNextFloat32(info);

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
const math::Vector3f& Vector::GetVector() const
{
	return v_;
}

}} // namespace x::templates

