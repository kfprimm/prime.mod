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
/*!	\file templates/meshface.cpp
	\brief Implementation von x::templates::MeshFace
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::MeshFace
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
MeshFace::MeshFace()
:	Template(TemplateTypeMeshFace),
	index_count_(0),
	indices_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
MeshFace::~MeshFace()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void MeshFace::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "MeshFace" << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine Exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* MeshFace::Clone() const
{
	MeshFace* clone = new MeshFace;
	clone->index_count_ = index_count_;
	for (
		face_indices_t::const_iterator begin = indices_.begin();
		begin != indices_.end();
		++begin
	)
	{
		clone->indices_.push_back(*begin);
	}
	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input.
*/
void MeshFace::ReadDefinition(
	TemporalInfo& info
) 
{
	// DWORD nFaceVertexIndices;
	{
		Token::Type decl[] = { Token::TypeDWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("DWORD nFaceVertexIndices;"));
	}

	// array DWORD faceVertexIndices[nFaceVertexIndices];
	{
		Token::Type decl[] = { Token::TypeArray, Token::TypeDWord, Token::TypeName, Token::TypeOBracket, Token::TypeName, Token::TypeCBracket };
		info.scanner->ReadDeclaration(info, decl, 6, UNICODE_STRING("array DWORD faceVertexIndices[nFaceVertexIndices];"));
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
void MeshFace::ReadData(
	TokenName* name_token,
	TokenGuid* guid_token,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	index_count_ = info.serializer.GetNextDword(info);
	for (udword_t i = 0; i < index_count_; ++i)
		indices_.push_back(info.serializer.GetNextDword(info));

	if (read_cbrace)
	{
		info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Anzahl Indices dieses Polygons zurückgeben
	\return Anzahl

	Gibt die Anzahl der Indices dieses Polygons zurück (= Anzahl Ecken).
*/
const udword_t MeshFace::GetIndexCount() const
{
	return index_count_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Indices zurückgeben
	\return Referenz auf den Vektor mit den Indices

	Gibt eine Referenz auf den Vektor mit den Indices zurück welche die Eckpunkte dieses Polygons 
	referenzieren.
*/
const MeshFace::face_indices_t& MeshFace::GetIndices() const
{
	return indices_;
}

}} // namespace x::templates

