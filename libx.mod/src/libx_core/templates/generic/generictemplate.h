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
/*!	\file templates/generic/generictemplate.h
	\brief Definition von x::templates::generic::GenericTemplate
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::templates::generic::GenericTemplate
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef TEMPLATES_GENERIC_GENERICTEMPLATE_H
#define TEMPLATES_GENERIC_GENERICTEMPLATE_H

#include "../../global/global.h"
#include "../../temporalinfo.h"
#include "../template.h"
#include "./genericelement.h"
#include "./genericvalue.h"

namespace x { namespace templates { namespace generic {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief GenericTemplate-Template
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	Implementiert das X-File-GenericTemplate-Template.
*/
class GenericTemplate : public Template
{
public:
	//-------------------------------------------------------------------------------------------------------
	// Konstruktur & Destruktor
	GenericTemplate(tokens::TokenName* name_token, tokens::TokenGuid* guid_token);
	GenericTemplate(const string_t& name, const Guid& guid);
	virtual ~GenericTemplate();

	//-------------------------------------------------------------------------------------------------------
	virtual void PrintFileStructure(const uint_t level) const;
	virtual Template* Clone() const;
	virtual void ReadDefinition(TemporalInfo& info);
	virtual void ReadData(tokens::TokenName* name_token, tokens::TokenGuid* guid_token, TemporalInfo& info, bool_t read_cbrace);

protected:
	virtual void ReadPrimitiveElement(TemporalInfo& info, GenericElement* element);
	virtual void ReadArrayElement(TemporalInfo& info, GenericElement* element);
	virtual void ReadReferenceElement(TemporalInfo& info, GenericElement* element);

protected:
	string_t name_;
	Guid guid_;
	bool_t has_children_;
	vector<GenericElement*> elements_;
	vector<GenericValue*> values_;
	map<string_t, uint_t> values_map_;
};

}}} // namespace x::templates::generic

#endif // TEMPLATES_GENERIC_GENERICTEMPLATE_H
