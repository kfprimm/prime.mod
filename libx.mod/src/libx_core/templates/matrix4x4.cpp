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
/*!	\file templates/matrix4x4.cpp
	\brief Implementation von x::templates::Matrix4x4
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::Matrix4x4
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
Matrix4x4::Matrix4x4()
:	Template(TemplateTypeMatrix4x4),
	matrix_()
{
	matrix_.ToIdentity();
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
Matrix4x4::~Matrix4x4()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void Matrix4x4::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "Matrix4x4" << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* Matrix4x4::Clone() const
{
	Matrix4x4* clone = new Matrix4x4;
	clone->matrix_ = matrix_;
	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input.
*/
void Matrix4x4::ReadDefinition(
	TemporalInfo& info
) 
{
	// array float matrix[16];
	{
		Token::Type decl[] = { Token::TypeArray, Token::TypeFloat, Token::TypeName, Token::TypeOBracket, Token::TypeInteger, Token::TypeCBracket };
		info.scanner->ReadDeclaration(info, decl, 6, UNICODE_STRING("array float matrix[16];"));
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
void Matrix4x4::ReadData(
	TokenName* name_token,
	TokenGuid* guid_token,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	for (uint_t i = 0; i < 4; ++i)
	{
		for (uint_t j = 0; j < 4; ++j)
		{
			matrix_.m_fData2[i][j] = info.serializer.GetNextFloat32(info);
		}
	}

	/*
	 *	Das nächste Token muss die Daten abschliessen, falls es überhaupt gelesen werden muss
	 */
	if (read_cbrace)
	{
		info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Matrix-Inhalt zurückgeben
	\return Matrix

	Gibt die math::Matrix4x4f-Struktur zurück, in welche der Inhalt dieses Templates 
	gespeichert worden ist.
*/
const math::Matrix4x4f& Matrix4x4::GetMatrix() const
{
	return matrix_;
}

}} // namespace x::templates

