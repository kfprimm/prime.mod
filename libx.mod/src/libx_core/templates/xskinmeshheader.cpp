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
/*!	\file templates/xskinmeshheader.cpp
	\brief Implementation von x::templates::XSkinMeshHeader
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::XSkinMeshHeader
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
XSkinMeshHeader::XSkinMeshHeader()
:	Template(TemplateTypeXSkinMeshHeader),
	max_skin_weights_per_vertex_(0),
	max_skin_weights_per_face_(0),
	num_bones_(0)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
XSkinMeshHeader::~XSkinMeshHeader()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void XSkinMeshHeader::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << UNICODE_STRING("XSkinMeshHeader") << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine Exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* XSkinMeshHeader::Clone() const
{
	XSkinMeshHeader* clone = new XSkinMeshHeader();
	clone->max_skin_weights_per_vertex_ = max_skin_weights_per_vertex_;
	clone->max_skin_weights_per_face_ = max_skin_weights_per_face_;
	clone->num_bones_ = num_bones_;
	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input. Definition 
	des Templates gemäss DirectX-Dokumentation:

	\verbatim
		template XSkinMeshHeader
		{
			< 3CF169CE-FF7C-44ab-93C0-F78F62D172E2 >
			WORD nMaxSkinWeightsPerVertex;
			WORD nMaxSkinWeightsPerFace;
			WORD nBones;
		}
	\endverbatim
*/
void XSkinMeshHeader::ReadDefinition(
	TemporalInfo& info
)
{
	// WORD nMaxSkinWeightsPerVertex;
	{
		Token::Type decl[] = { Token::TypeWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("WORD nMaxSkinWeightsPerVertex;"));
	}

	// WORD nMaxSkinWeightsPerFace;
	{
		Token::Type decl[] = { Token::TypeWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("WORD nMaxSkinWeightsPerFace;"));
	}

	// WORD nBones;
	{
		Token::Type decl[] = { Token::TypeWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("WORD nBones;"));
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
void XSkinMeshHeader::ReadData(
	TokenName* name_token,
	TokenGuid* guid_token,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	max_skin_weights_per_vertex_ = info.serializer.GetNextDword(info);
	max_skin_weights_per_face_ = info.serializer.GetNextDword(info);
	num_bones_ = info.serializer.GetNextDword(info);

	if (read_cbrace)
	{
		info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
	}
}

}} // namespace x::templates

