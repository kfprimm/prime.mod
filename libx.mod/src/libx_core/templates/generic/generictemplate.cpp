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
/*!	\file templates/generic/generictemplate.cpp
	\brief Implementation von x::templates::generic::GenericTemplate
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::templates::generic::GenericTemplate
*/
//-----------------------------------------------------------------------------------------------------------
#define XLIB_CORE
#include "../../global/global.h"
#include "../../scanner/scanner.h"
#include "../../tokens/tokens.h"
#include "../../serializer.h"
#include "./generictemplate.h"

namespace x { namespace templates { namespace generic {

using namespace x::tokens;

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
GenericTemplate::GenericTemplate(
	TokenName* name_token,
	TokenGuid* guid_token
)
:	Template(TemplateTypeGeneric),
	name_(name_token->GetName()),
	guid_(guid_token->GetGuid()),
	has_children_(false),
	elements_(),
	values_(),
	values_map_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor
	\param name
	\param guid

	Konstruktor
*/
GenericTemplate::GenericTemplate(
	const string_t& name,
	const Guid& guid
)
:	Template(TemplateTypeGeneric),
	name_(name),
	guid_(guid),
	has_children_(false),
	elements_(),
	values_(),
	values_map_()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
GenericTemplate::~GenericTemplate()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Dateistruktur ausgeben
	\param level

	Gibt die grobe Struktur der Datei zu welcher dieses Template gehört aus.
*/
void GenericTemplate::PrintFileStructure(
	const uint_t level
) const
{
	PrintIndent(level);
	//cout << name_ << " (GenericTemplate)" << endl;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Kopie anlegen
	\return Pointer auf die Kopie

	Erstellt eine Exakte Kopie dieses Templates und gibt sie zurück.
*/
Template* GenericTemplate::Clone() const
{
	GenericTemplate* clone = new GenericTemplate(name_, guid_);
	clone->has_children_ = has_children_;

	for (
		vector<GenericElement*>::const_iterator begin = elements_.begin();
		begin != elements_.end();
		++begin
	)
	{
		GenericElement* new_element = new GenericElement;
		GenericElement* element = (*begin);
		
		new_element->type = element->type;
		new_element->primitive_type = element->primitive_type;
		new_element->primitive_name = element->primitive_name;
		new_element->is_array_primitive_type = element->is_array_primitive_type;
		new_element->array_primitive_type = element->array_primitive_type;
		new_element->array_name_type = element->array_name_type;
		new_element->reference_type = element->reference_type;
		new_element->reference_name = element->reference_name;

		for (
			vector<ArrayDimension*>::const_iterator inner_begin = element->array_dimension_list.begin();
			inner_begin != element->array_dimension_list.end();
			++inner_begin
		)
		{
			ArrayDimension* new_dimension = new ArrayDimension;
			ArrayDimension* dimension = (*inner_begin);

			new_dimension->is_integer_dimension = dimension->is_integer_dimension;
			new_dimension->dimension = dimension->dimension;
			new_dimension->dimension_name = dimension->dimension_name;

			new_element->array_dimension_list.push_back(new_dimension);
		}

		clone->elements_.push_back(new_element);
	}

	return clone;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Template komplett auslesen
	\param info

	Liest das Template komplett aus dem über info gegebenen Input.
*/
void GenericTemplate::ReadDefinition(
	TemporalInfo& info
)
{
	bool_t do_continue = true;
	while (do_continue)
	{
		Token* token = info.scanner->ReadToken(info);
		switch (token->GetType())
		{
		// primitive_type
		case Token::TypeWord:
		case Token::TypeDWord:
		case Token::TypeFloat:
		case Token::TypeDouble:
		case Token::TypeChar:
		case Token::TypeUChar:
		case Token::TypeSWord:
		case Token::TypeSDWord:
		case Token::TypeLpStr:
		case Token::TypeUnicode:
		case Token::TypeCString:
			{
				GenericElement* element = new GenericElement;
				element->type = ElementTypePrimitive;
				element->primitive_type = token->GetType();
				TokenName* name_token = dynamic_cast<TokenName*>(info.scanner->ReadOptionalToken(info, Token::TypeName));
				if (name_token)
					element->primitive_name = name_token->GetName();
				elements_.push_back(element);
			}
			break;

		// array
		case Token::TypeArray:
			{
				GenericElement* element = new GenericElement;
				element->type = ElementTypeArray;

				// Typ
				token = info.scanner->ReadToken(info);
				if (token->GetType() != Token::TypeName)
				{
					element->is_array_primitive_type = true;
					element->array_primitive_type = token->GetType();
				}
				else
				{
					TokenName* name_token = dynamic_cast<TokenName*>(token);
					element->is_array_primitive_type = false;
					element->array_name_type = name_token->GetName();
				}

				// name 
				TokenName* array_name_token = dynamic_cast<TokenName*>(info.scanner->ReadExpectedToken(info, Token::TypeName));
				element->array_name = array_name_token->GetName();

				bool_t do_continue = true;
				while (do_continue)
				{
					token = info.scanner->ReadOptionalToken(info, Token::TypeOBracket);
					if (!token)
					{
						do_continue = false;
					}
					else
					{
						// dimension
						ArrayDimension* dimension = new ArrayDimension;
						token = info.scanner->ReadToken(info);
						if (token->GetType() == Token::TypeInteger)
						{
							TokenInteger* integer_token = dynamic_cast<TokenInteger*>(token);
							dimension->is_integer_dimension = true;
							dimension->dimension = (uint_t)integer_token->GetDword();
						}
						else if (token->GetType() == Token::TypeName)
						{
							TokenName* name_token = dynamic_cast<TokenName*>(token);
							dimension->is_integer_dimension = false;
							dimension->dimension_name = name_token->GetName();
						}
						else
						{
							throw Exception(
								UNICODE_STRING("Token::TypeInteger or Token::TypeName expected!")
							);
						}
						element->array_dimension_list.push_back(dimension);
						token = info.scanner->ReadExpectedToken(info, Token::TypeCBracket);
					}
				}
			}
			break;

		// template_reference
		case Token::TypeName:
			{
				GenericElement* element = new GenericElement;
				element->type = ElementTypeReference;

				TokenName* ref_type_token = dynamic_cast<TokenName*>(token);
				element->reference_type = ref_type_token->GetName();

				TokenName* name_token = dynamic_cast<TokenName*>(info.scanner->ReadOptionalToken(info, Token::TypeName));
				if (name_token)
					element->reference_name = name_token->GetName();
				elements_.push_back(element);
			}
			break;

		// children-definition
		case Token::TypeOBracket:
			has_children_ = true;
			while (token->GetType() != Token::TypeCBracket)
				token = info.scanner->ReadToken(info);
			break;

		case Token::TypeCBrace:
			do_continue = false;
			break;
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
void GenericTemplate::ReadData(
	TokenName* name_token,
	TokenGuid* guid_token,
	TemporalInfo& info, 
	bool_t read_cbrace
)
{
	for (
		vector<GenericElement*>::iterator begin = elements_.begin();
		begin != elements_.end();
		++begin
	)
	{
		GenericElement* element = (*begin);
		switch (element->type)
		{
		case ElementTypePrimitive:
			ReadPrimitiveElement(info, element);
			break;

		case ElementTypeArray:
			ReadArrayElement(info, element);
			break;

		case ElementTypeReference:
			ReadReferenceElement(info, element);
			break;
		}
	}

	if (read_cbrace)
	{
		info.scanner->ReadExpectedToken(info, Token::TypeCBrace);
	}
}




//-----------------------------------------------------------------------------------------------------------
void GenericTemplate::ReadPrimitiveElement(
	TemporalInfo& info,
	GenericElement* element
)
{
	switch(element->primitive_type)
	{
	case Token::TypeWord:
	case Token::TypeDWord:
	case Token::TypeChar:
	case Token::TypeUChar:
	case Token::TypeSWord:
	case Token::TypeSDWord:
		values_.push_back(
			new XGenericUDwordValue(
				info.serializer.GetNextDword(info)
			)
		);
		break;

	case Token::TypeFloat:
	case Token::TypeDouble:
		values_.push_back(
			new XGenericFloatValue(
				info.serializer.GetNextFloat32(info)
			)
		);
		break;

	case Token::TypeLpStr:
	case Token::TypeUnicode:
	case Token::TypeCString:
		values_.push_back(
			new XGenericStringValue(
				dynamic_cast<TokenString*>(
					info.scanner->ReadExpectedToken(
						info,
						Token::TypeString
					)
				)->GetValue()
			)
		);
		break;

	default:
		throw Exception(
			UNICODE_STRING("internal error, invalid element!")
		);
	}

	const string_t& name = element->primitive_name;
	values_map_.insert(pair<string_t, uint_t>(name, values_.size() - 1));
}




//-----------------------------------------------------------------------------------------------------------
void GenericTemplate::ReadArrayElement(
	TemporalInfo& info,
	GenericElement* element
)
{
	throw NotSupportedException(
		UNICODE_STRING("todo!")
	);
}




//-----------------------------------------------------------------------------------------------------------
void GenericTemplate::ReadReferenceElement(
	TemporalInfo& info,
	GenericElement* element
)
{
	throw NotSupportedException(
		UNICODE_STRING("todo!")
	);
}

}}} // namespace x::templates::generic

