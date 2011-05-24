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
/*!	\file templates/skinweights.cpp
	\brief Implementation von x::templates::SkinWeights
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::SkinWeights
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
SkinWeights::SkinWeights()
:	Template(TemplateTypeSkinWeights),
	transform_node_name_(),
	num_weights_(0),
	vertex_indices_(NULL),
	weights_(NULL),
	matrix_offset_(NULL)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
SkinWeights::~SkinWeights()
{
	SAFE_DELETE_ARRAY(vertex_indices_);
	SAFE_DELETE_ARRAY(weights_);
	if (matrix_offset_)
	{
		matrix_offset_->RemRef();
		matrix_offset_ = NULL;
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void SkinWeights::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "SkinWeights" << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* SkinWeights::Clone() const
{
	SkinWeights* clone = new SkinWeights;
	clone->transform_node_name_ = transform_node_name_;
	clone->num_weights_ = num_weights_;
	clone->vertex_indices_ = new dword_t[num_weights_];
	memcpy(clone->vertex_indices_, vertex_indices_, sizeof(dword_t) * num_weights_);
	clone->weights_ = new float_t[num_weights_];
	memcpy(clone->weights_, weights_, sizeof(float_t) * num_weights_);
	if (matrix_offset_ != NULL)
		clone->matrix_offset_ = dynamic_cast<Matrix4x4*>(matrix_offset_->Clone());
	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input. Definition 
	des Templates gemäss DirectX-Dokumentation:

	\verbatim
		template SkinWeights
		{
			< 6F0D123B-BAD2-4167-A0D0-80224F25FABB >
			STRING transformNodeName;
			DWORD nWeights;
			array DWORD vertexIndices[nWeights];
			array float weights[nWeights];
			Matrix4x4 matrixOffset;
		}
	\endverbatim
*/
void SkinWeights::ReadDefinition(
	TemporalInfo& info
)
{
	// STRING transformNodeName;
	{
		Token::Type decl[] = { Token::TypeLpStr, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("STRING transformNodeName;"));
	}

	// DWORD nWeights;
	{
		Token::Type decl[] = { Token::TypeDWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("DWORD nWeights;"));
	}

	// array DWORD vertexIndices[nWeights];
	{
		Token::Type decl[] = { Token::TypeArray, Token::TypeDWord, Token::TypeName, Token::TypeOBracket, Token::TypeName, Token::TypeCBracket };
		info.scanner->ReadDeclaration(info, decl, 6, UNICODE_STRING("array DWORD vertexIndices[nWeights];"));
	}

	// array float weights[nWeights];
	{
		Token::Type decl[] = { Token::TypeArray, Token::TypeFloat, Token::TypeName, Token::TypeOBracket, Token::TypeName, Token::TypeCBracket };
		info.scanner->ReadDeclaration(info, decl, 6, UNICODE_STRING("array float weights[nWeights];"));
	}

	// Matrix4x4 matrixOffset;
	{
		Token::Type decl[] = { Token::TypeName, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("Matrix4x4 matrixOffset;"));
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
void SkinWeights::ReadData(
	TokenName* name_token,
	TokenGuid* guid_token,
	TemporalInfo& info,
	bool_t read_cbrace
)
{
	// Ev. Name auslesen falls vorhanden
	Token* value = info.scanner->ReadToken(info);
	if (value->GetType() != Token::TypeString)
	{
		throw Exception(
			UNICODE_STRING("Token::TypeString expected!")
		);
	}
	transform_node_name_ = dynamic_cast<TokenString*>(value)->GetValue();

	num_weights_ = info.serializer.GetNextDword(info);
	SAFE_DELETE_ARRAY(vertex_indices_);
	vertex_indices_ = new dword_t[num_weights_];
	for (dword_t i = 0; i < num_weights_; ++i)
		vertex_indices_[i] = info.serializer.GetNextDword(info);
	SAFE_DELETE_ARRAY(weights_);
	weights_ = new float_t[num_weights_];
	for (dword_t i = 0; i < num_weights_; ++i)
		weights_[i] = info.serializer.GetNextFloat32(info);

	matrix_offset_ = dynamic_cast<Matrix4x4*>(info.serializer.InstanciateTemplateByName(info, UNICODE_STRING("Matrix4x4")));
	matrix_offset_->ReadData(NULL, NULL, info, false);

	if (read_cbrace)
	{
		info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
	}
}




//-----------------------------------------------------------------------------------------------------------
const string_t& SkinWeights::GetTransformNodeName() const
{
	return transform_node_name_;
}




//-----------------------------------------------------------------------------------------------------------
const dword_t SkinWeights::GetNumWeights() const
{
	return num_weights_;
}




//-----------------------------------------------------------------------------------------------------------
const dword_t* SkinWeights::GetVertexIndices() const
{
	return vertex_indices_;
}




//-----------------------------------------------------------------------------------------------------------
const float_t* SkinWeights::GetWeights() const
{
	return weights_;
}




//-----------------------------------------------------------------------------------------------------------
const Matrix4x4* SkinWeights::GetMatrixOffset() const
{
	return matrix_offset_;
}

}} // namespace x::templates

