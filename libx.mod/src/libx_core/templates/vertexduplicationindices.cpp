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
/*!	\file templates/vertexduplicationindices.cpp
	\brief Implementation von x::templates::VertexDuplicationIndices
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::VertexDuplicationIndices
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
VertexDuplicationIndices::VertexDuplicationIndices()
:	Template(TemplateTypeVertexDuplicationIndices),
	index_count_(0),
	original_vertices_(0),
	indices_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
VertexDuplicationIndices::~VertexDuplicationIndices()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void VertexDuplicationIndices::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "VertexDuplicationIndices" << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Kopie

	Gibt eine Kopie dieses Templates zurück.
*/
Template* VertexDuplicationIndices::Clone() const
{
	VertexDuplicationIndices* clone(new VertexDuplicationIndices);

	clone->index_count_ = index_count_;
	clone->original_vertices_ = original_vertices_;
	for (
		vector<udword_t>::const_iterator begin = indices_.begin();
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
void VertexDuplicationIndices::ReadDefinition(
	TemporalInfo& info
)
{
	// DWORD nIndices;
	{
		Token::Type decl[] = { Token::TypeDWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("DWORD nIndices;"));
	}

	// DWORD nOriginalVertices
	{
		Token::Type decl[] = { Token::TypeDWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("DWORD nOriginalVertices;"));
	}

	// array DWORD indices[nIndices];
	{
		Token::Type decl[] = { Token::TypeArray, Token::TypeDWord, Token::TypeName, Token::TypeOBracket, Token::TypeName, Token::TypeCBracket };
		info.scanner->ReadDeclaration(info, decl, 6, UNICODE_STRING("array DWORD indices[nIndices];"));
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
void VertexDuplicationIndices::ReadData(
	TokenName* name_token,
	TokenGuid* guid_token,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	index_count_ = info.serializer.GetNextDword(info);
	original_vertices_ = info.serializer.GetNextDword(info);
	for (udword_t i = 0; i < index_count_; ++i)
		indices_.push_back(info.serializer.GetNextDword(info));

	/*
	 *	Das nächste Token muss die Daten abschliessen, falls es überhaupt gelesen werden muss
	 */
	if (read_cbrace)
	{
		info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
	}
}

}} // namespace x::templates

