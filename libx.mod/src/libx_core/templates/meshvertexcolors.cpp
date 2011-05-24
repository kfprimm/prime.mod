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
/*!	\file templates/meshvertexcolors.cpp
	\brief Implementation von x::templates::MeshVertexColors
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::MeshVertexColors
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
MeshVertexColors::MeshVertexColors()
:	Template(TemplateTypeMeshVertexColors),
	vertex_colors_count_(0),
	vertex_colors_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
MeshVertexColors::~MeshVertexColors()
{
	for (
		colors_container_t::iterator begin = vertex_colors_.begin();
		begin != vertex_colors_.end();
		++begin
	)
	{
		IndexedColor* indexed_color = (*begin);
		indexed_color->RemRef();
	}
	vertex_colors_.clear();
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void MeshVertexColors::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "MeshVertexColors" << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* MeshVertexColors::Clone() const
{
	MeshVertexColors* clone = new MeshVertexColors;

	// Koordinaten klonen
	clone->vertex_colors_count_ = vertex_colors_count_;
	for (
		vector<IndexedColor*>::const_iterator begin = vertex_colors_.begin();
		begin != vertex_colors_.end();
		++begin
	)
	{
		clone->vertex_colors_.push_back(dynamic_cast<IndexedColor*>((*begin)->Clone()));
	}

	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input.
*/
void MeshVertexColors::ReadDefinition(
	TemporalInfo& info
)
{
	// DWORD nVertexColors;
	{
		Token::Type decl[] = { Token::TypeDWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("DWORD nVertexColors;"));
	}

	// array IndexColor vertexColors[nVertexColors];
	{
		Token::Type decl[] = { Token::TypeArray, Token::TypeName, Token::TypeName, Token::TypeOBracket, Token::TypeName, Token::TypeCBracket };
		info.scanner->ReadDeclaration(info, decl, 6, UNICODE_STRING("array IndexColor vertexColors[nVertexColors];"));
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
void MeshVertexColors::ReadData(
	TokenName* name_token,
	TokenGuid* guid_token,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	/*
	 *	Anzahl Normalvektoren auslesen. Danach alle Normalvektoren direkt
	 *	auslesen
	 */
	vertex_colors_.clear();
	vertex_colors_count_ = info.serializer.GetNextDword(info);
	for (udword_t i = 0; i < vertex_colors_count_; ++i)
	{
		IndexedColor* indexed_color = dynamic_cast<IndexedColor*>(info.serializer.InstanciateTemplateByName(info, UNICODE_STRING("IndexedColor")));
		indexed_color->ReadData(NULL, NULL, info, false);
		vertex_colors_.push_back(indexed_color);
	}

	/*
	 *	Das nächste Token muss die Daten abschliessen, falls es überhaupt gelesen werden muss
	 */
	if (read_cbrace)
	{
		info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
	}
}

}} // namespace x::templates

