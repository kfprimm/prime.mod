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
/*!	\file templates/meshmateriallist.cpp
	\brief Implementation von x::templates::MeshMaterialList
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::MeshMaterialList
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
MeshMaterialList::MeshMaterialList()
:	Template(TemplateTypeMeshMaterialList),
	material_count_(0),
	face_index_count_(0),
	face_indexes_(NULL),
	materials_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
MeshMaterialList::~MeshMaterialList()
{
	SAFE_DELETE_ARRAY(face_indexes_);

	for (
		material_container_t::iterator begin = materials_.begin();
		begin != materials_.end();
		++begin
	)
	{
		Material* material = (*begin);
		material->RemRef();
	}
	materials_.clear();
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine Exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* MeshMaterialList::Clone() const
{
	MeshMaterialList* clone = new MeshMaterialList;
	clone->material_count_ = material_count_;
	clone->face_index_count_ = face_index_count_;
	
	clone->face_indexes_ = new udword_t[clone->face_index_count_];
	for (uint_t i = 0; i < face_index_count_; ++i)
		clone->face_indexes_[i] = face_indexes_[i];

	for (
		material_container_t::const_iterator begin = materials_.begin();
		begin != materials_.end();
		++begin
	)
	{
		clone->materials_.push_back(dynamic_cast<Material*>((*begin)->Clone()));
	}

	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input.
*/
void MeshMaterialList::ReadDefinition(
	TemporalInfo& info
)
{
	// DWORD nMaterials;
	{
		Token::Type decl[] = { Token::TypeDWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("DWORD nMaterials;"));
	}

	// DWORD nFaceIndexes;
	{
		Token::Type decl[] = { Token::TypeDWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("DWORD nFaceIndexes;"));
	}

	// array DWORD faceIndexes[nFaceIndexes];
	{
		Token::Type decl[] = { Token::TypeArray, Token::TypeDWord, Token::TypeName, Token::TypeOBracket, Token::TypeName, Token::TypeCBracket };
		info.scanner->ReadDeclaration(info, decl, 6, UNICODE_STRING("array DWORD faceIndexes[nFaceIndexes];"));
	}

	// [Material <3D82AB4D-62DA-11CF-AB39-0020AF71E433>]
	{
		Token* obracket_token = info.scanner->ReadToken(info);
		Token* name_token = info.scanner->ReadToken(info);
		Token* guid_token = NULL;
		Token* cbracket_token = info.scanner->ReadToken(info);
		if (cbracket_token->GetType() == Token::TypeGuid)
		{
			guid_token = cbracket_token;
			cbracket_token = info.scanner->ReadToken(info);
		}

		if (obracket_token->GetType() != Token::TypeOBracket ||
			name_token->GetType() != Token::TypeName ||
			(guid_token != NULL && guid_token->GetType() != Token::TypeGuid) ||
			cbracket_token->GetType() != Token::TypeCBracket)
		{
			throw Exception(
				UNICODE_STRING("error in definition of '[Material <3D82AB4D-62DA-11CF-AB39-0020AF71E433>]'")
			);
		}
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void MeshMaterialList::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "MeshMaterialList" << endl;
	for (
		material_container_t::const_iterator begin = materials_.begin();
		begin != materials_.end();
		++begin
	)
	{
		(*begin)->PrintFileStructure(level + 1);
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
void MeshMaterialList::ReadData(
	TokenName* name_token,
	TokenGuid* guid_token,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	material_count_ = info.serializer.GetNextDword(info);
	face_index_count_ = info.serializer.GetNextDword(info);
	SAFE_DELETE_ARRAY(face_indexes_);
	face_indexes_ = new udword_t[face_index_count_];
	for (udword_t i = 0; i < face_index_count_; ++i)
		face_indexes_[i] = info.serializer.GetNextDword(info);

	for (udword_t i = 0; i < material_count_; ++i)
	{
		Token* name_token = info.scanner->ReadToken(info);
		Template* material = info.serializer.ReadTemplateData(name_token, info);
		if (material == NULL || material->GetType() != TemplateTypeMaterial)
		{
			throw Exception(
				UNICODE_STRING("'Material'-template expected!")
			);
		}
		materials_.push_back(dynamic_cast<Material*>(material));
	}

	if (read_cbrace)
	{
		info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Anzahl Materialien zurückgeben
	\return Anzahl
	
	Gibt die Anzahl Materialien dieser Liste zurück.
*/
const uint_t MeshMaterialList::GetMaterialCount() const
{
	return (uint_t)material_count_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Anzahl Face-Indices zurückgeben
	\return Anzahl

	Gibt die Anzahl der Face-Indices zurück.
*/
const uint_t MeshMaterialList::GetFaceIndexCount() const
{
	return (uint_t)face_index_count_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Face-Indices zurückgeben
	\return Indices

	Gibt die Face-Indices zurück.
*/
const udword_t* MeshMaterialList::GetFaceIndices() const
{
	return face_indexes_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Materialien zurückgeben
	\return Referenz

	Gibt eine Referenz auf den Vektor zurück in welchem alle Materialien dieser Material-Liste
	gespeichert sind.
*/
const MeshMaterialList::material_container_t& MeshMaterialList::GetMaterials() const
{
	return materials_;
}

}} // namespace x::templates

