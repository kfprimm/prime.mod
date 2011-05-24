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
/*!	\file templates/meshnormals.cpp
	\brief Implementation von x::templates::MeshNormals
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::MeshNormals
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
MeshNormals::MeshNormals()
:	Template(TemplateTypeMeshNormals),
	normal_count_(0),
	normals_(NULL),
	face_count_(0),
	faces_size_(0),
	faces_(NULL)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
MeshNormals::~MeshNormals()
{
	SAFE_DELETE_ARRAY(faces_);
	SAFE_DELETE_ARRAY(normals_);
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void MeshNormals::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "MeshNormals" << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* MeshNormals::Clone() const
{
	MeshNormals* clone = new MeshNormals;

	// normalvektoren klonen
	clone->normal_count_ = normal_count_;
	clone->normals_ = new ubyte_t[sizeof(float_t) * normal_count_ * 3];
	memcpy(clone->normals_, normals_, sizeof(float_t) * normal_count_ * 3);

	// indices klonen
	clone->face_count_ = face_count_;
	clone->faces_size_ = faces_size_;
	clone->faces_ = new ubyte_t[faces_size_];
	memcpy(clone->faces_, faces_, faces_size_);

	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input.
*/
void MeshNormals::ReadDefinition(
	TemporalInfo& info
)
{
	// DWORD nNormals;
	{
		Token::Type decl[] = { Token::TypeDWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("DWORD nNormals;"));
	}

	// array Vector normals[nNormals];
	{
		Token::Type decl[] = { Token::TypeArray, Token::TypeName, Token::TypeName, Token::TypeOBracket, Token::TypeName, Token::TypeCBracket };
		info.scanner->ReadDeclaration(info, decl, 6, UNICODE_STRING("array Vector normals[nNormals];"));
	}

	// DWORD nFaceNormals;
	{
		Token::Type decl[] = { Token::TypeDWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("DWORD nFaceNormals;"));
	}

	// array MeshFace meshFaces[nFaceNormals];
	{
		Token::Type decl[] = { Token::TypeArray, Token::TypeName, Token::TypeName, Token::TypeOBracket, Token::TypeName, Token::TypeCBracket };
		info.scanner->ReadDeclaration(info, decl, 6, UNICODE_STRING("array MeshFace meshFaces[nFaceNormals];"));
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template-Daten lesen
	\param name_token
	\param guid_token
	\param info
	\param read_cbrace

	Liest Daten dieses Templates. Die Normalvektoren und faces werden direkt ausgelesen und nicht
	als Array von Templates gespeichert, da ansonsten tausende von zusätzlichen Objekten kreiert würden.
*/
void MeshNormals::ReadData(
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
	normal_count_ = info.serializer.GetNextDword(info);
	SAFE_DELETE_ARRAY(normals_);
	normals_ = new ubyte_t[sizeof(float_t) * normal_count_ * 3];
	float_t* normals = (float_t*)normals_;
	for (udword_t i = 0; i < normal_count_; ++i)
	{
		normals[i*3+0] = info.serializer.GetNextFloat32(info);
		normals[i*3+1] = info.serializer.GetNextFloat32(info);
		normals[i*3+2] = info.serializer.GetNextFloat32(info);
	}

	/*
	 *	Anzahl faces auslesen. Danach gleich alle faces direkt auch auslesen
	 */
	face_count_ = info.serializer.GetNextDword(info);
	vector<udword_t> faces;
	for (udword_t i = 0; i < face_count_; ++i)
	{
		udword_t index_count = info.serializer.GetNextDword(info);
		faces.push_back(index_count);
		for (udword_t j = 0; j < index_count; ++j)
			faces.push_back(info.serializer.GetNextDword(info));
	}
	faces_size_ = sizeof(udword_t) * (udword_t)faces.size();
	SAFE_DELETE_ARRAY(faces_);
	faces_ = new ubyte_t[faces_size_];
	memcpy(faces_, &faces[0], faces_size_);

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
	\brief Anzahl Normalvektoren zurückgeben
	\return Anzahl

	Gibt die Anzahl Normalvektoren dieses MeshNormals-Templates zurück.
*/
const uint_t MeshNormals::GetNormalCount() const
{
	return (uint_t)normal_count_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Normalvektoren zurückgeben
	\return Referenz auf den Vektor mit den Normalvektoren

	Gibt die Normalvektoren dieses Templates zurück. 
*/
const ubyte_t* MeshNormals::GetNormals() const
{
	return normals_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Anzahl faces zurückgeben
	\return Anzahl

	Gibt die Anzahl faces dieses meshes zurück.
*/
const uint_t MeshNormals::GetFaceCount() const
{
	return (uint_t)face_count_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief faces zurückgeben
	\return Referenz auf den Vektor mit den Flächen

	Gibt die Oberflächenangaben dieses Meshes zurück.
*/
const ubyte_t* MeshNormals::GetFaces() const
{
	return faces_;
}

}} // namespace x::templates

