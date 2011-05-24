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
/*!	\file templates/frametransformmatrix.cpp
	\brief Implementation von x::templates::FrameTransformMatrix
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::FrameTransformMatrix
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
FrameTransformMatrix::FrameTransformMatrix()
:	Template(TemplateTypeFrameTransformMatrix),
	matrix_(NULL)
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
FrameTransformMatrix::~FrameTransformMatrix()
{
	if (matrix_)
	{
		matrix_->RemRef();
		matrix_ = NULL;
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void FrameTransformMatrix::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << "FrameTransformMatrix" << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine Exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* FrameTransformMatrix::Clone() const
{
	FrameTransformMatrix* clone = new FrameTransformMatrix;

	/*
	 *	Das Attribut kopieren, wenn vorhanden
	 */
	if (matrix_ != NULL)
		clone->matrix_ = dynamic_cast<Matrix4x4*>(matrix_->Clone());

	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input.
*/
void FrameTransformMatrix::ReadDefinition(
	TemporalInfo& info
) 
{
	// Matrix4x4 frameMatrix;
	{
		Token* type_token = info.scanner->ReadToken(info);
		Token* name_token = info.scanner->ReadToken(info);

		if (type_token->GetType() != Token::TypeName ||
			name_token->GetType() != Token::TypeName)
		{
			throw Exception(
				UNICODE_STRING("error in definition of 'Matrix4x4 frameMatrix'")
			);
		}
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
void FrameTransformMatrix::ReadData(
	TokenName* name_token,
	TokenGuid* guid_token,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	/*
	 *	Face-Color auslesen
	 */
	matrix_ = dynamic_cast<Matrix4x4*>(info.serializer.InstanciateTemplateByName(info, UNICODE_STRING("Matrix4x4")));
	matrix_->ReadData(NULL, NULL, info, false);

	if (read_cbrace)
	{
		info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Pointer auf die Transformationsmatrix zurückgeben
	\return Pointer

	Gibt einen Pointer auf das Matrix4x4-Template zurück welches diese Transformation
	speichert.
*/
const Matrix4x4* FrameTransformMatrix::GetMatrix() const
{
	return matrix_;
}

}} // namespace x::templates

