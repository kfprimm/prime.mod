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
/*!	\file templates/meshfacewraps.cpp
	\brief Implementation von x::templates::MeshFaceWraps
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::MeshFaceWraps
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
MeshFaceWraps::MeshFaceWraps()
:	Template(TemplateTypeMeshFaceWraps),
	face_wrap_values_count_(0),
	face_wrap_values_(NULL)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
MeshFaceWraps::~MeshFaceWraps()
{
	SAFE_REMREF(face_wrap_values_);
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void MeshFaceWraps::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "MeshFaceWrapValues" << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine Exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* MeshFaceWraps::Clone() const
{
	MeshFaceWraps* clone = new MeshFaceWraps;
	clone->face_wrap_values_count_ = face_wrap_values_count_;
	if (face_wrap_values_ != NULL)
		clone->face_wrap_values_ = dynamic_cast<Boolean2d*>(face_wrap_values_->Clone());
	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input.
*/
void MeshFaceWraps::ReadDefinition(
	TemporalInfo& info
) 
{
	// DWORD nFaceWrapValues;
	{
		Token::Type decl[] = { Token::TypeDWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("DWORD nFaceWrapValues;"));
	}

	// Boolean2d faceWrapValues;
	{
		Token::Type decl[] = { Token::TypeName, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("Boolean2d faceWrapValues;"));
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
void MeshFaceWraps::ReadData(
	TokenName* name_token,
	TokenGuid* guid_token,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	face_wrap_values_count_ = info.serializer.GetNextDword(info);
	face_wrap_values_ = dynamic_cast<Boolean2d*>(info.serializer.InstanciateTemplateByName(info, UNICODE_STRING("Boolean2d")));
	face_wrap_values_->ReadData(NULL, NULL, info, false);

	if (read_cbrace)
	{
		info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
	}
}

}} // namespace x::templates
