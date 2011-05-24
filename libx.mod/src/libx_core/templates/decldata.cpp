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
/*!	\file templates/decldata.cpp
	\brief Implementation von x::templates::DeclData
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::DeclData
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
DeclData::DeclData()
:	Template(TemplateTypeDeclData),
	element_count_(0),
	elements_(),
	dword_count_(0),
	data_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
DeclData::~DeclData()
{
	for (
		element_container_t::iterator begin = elements_.begin();
		begin != elements_.end();
		++begin
	)
	{
		VertexElement* current_element = (*begin);
		current_element->RemRef();
	}
	elements_.clear();
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void DeclData::PrintFileStructure(const uint_t level) const
{
	PrintIndent(level);
	//cout << "DeclData" << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* DeclData::Clone() const
{
	DeclData* clone = new DeclData;
	clone->element_count_ = element_count_;
	for (
		vector<VertexElement*>::const_iterator begin = elements_.begin();
		begin != elements_.end();
		++begin
	)
	{
		clone->elements_.push_back(dynamic_cast<VertexElement*>((*begin)->Clone()));
	}

	clone->dword_count_ = dword_count_;
	for (
		vector<udword_t>::const_iterator begin = data_.begin();
		begin != data_.end();
		++begin
	)
	{
		clone->data_.push_back(*begin);
	}

	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input.
*/
void DeclData::ReadDefinition(
	TemporalInfo& info
)
{
	// DWORD nElements;
	{
		Token::Type decl[] = { Token::TypeDWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("DWORD nElements;"));
	}

	// array VertexElement vertices[nElements];
	{
		Token::Type decl[] = { Token::TypeArray, Token::TypeName, Token::TypeName, Token::TypeOBracket, Token::TypeName, Token::TypeCBracket };
		info.scanner->ReadDeclaration(info, decl, 6, UNICODE_STRING("array VertexElement vertices[nElements];"));
	}

	// DWORD nDWords;
	{
		Token::Type decl[] = { Token::TypeDWord, Token::TypeName };
		info.scanner->ReadDeclaration(info, decl, 2, UNICODE_STRING("DWORD nDWords;"));
	}

	// array DWORD faces[nDWords];
	{
		Token::Type decl[] = { Token::TypeArray, Token::TypeDWord, Token::TypeName, Token::TypeOBracket, Token::TypeName, Token::TypeCBracket };
		info.scanner->ReadDeclaration(info, decl, 6, UNICODE_STRING("array DWORD faces[nDWords];"));
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
void DeclData::ReadData(
	TokenName* name_token,
	TokenGuid* guid_token,
	TemporalInfo& info,
	bool_t read_cbrace
)
{
	elements_.clear();
	element_count_ = info.serializer.GetNextDword(info);
	for (udword_t i = 0; i < element_count_; ++i)
	{
		VertexElement* current_element = dynamic_cast<VertexElement*>(info.serializer.InstanciateTemplateByName(info, UNICODE_STRING("VertexElement")));
		current_element->ReadData(NULL, NULL, info, false);
		elements_.push_back(current_element);
	}

	dword_count_ = info.serializer.GetNextDword(info);
	for (udword_t i = 0; i < dword_count_; ++i)
		data_.push_back(info.serializer.GetNextDword(info));

	if (read_cbrace)
	{
		info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
	}
}

}} // namespace x::templates

