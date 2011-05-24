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
/*!	\file templates/template.h
	\brief Definition von x::templates::Template
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::templates::Template
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef TEMPLATES_TEMPLATE_H
#define TEMPLATES_TEMPLATE_H

#include "../global/global.h"
#include "../tokens/tokens.h"
#include "../temporalinfo.h"

namespace x { namespace templates {

/*!
	\brief Basisklasse für X-File-Templates
	\version 0.1
	\date 15.4.2004
	\author Christian Oberholzer

	Basisklasse für alle X-File-Templates. 
*/
class Template
{
public:
	//-------------------------------------------------------------------------------------------------------
	// Konstruktur & Destruktor
	Template(TemplateTypes Type);
protected:
	virtual ~Template();

public:
	//-------------------------------------------------------------------------------------------------------
	virtual void AddRef();
	virtual void RemRef();
	virtual const uint_t GetRefCount() const;
	virtual void OnZeroRef();
	virtual const TemplateTypes GetType() const;




	//-------------------------------------------------------------------------------------------------------
	virtual void PrintFileStructure(const uint_t level) const = 0;
	virtual Template* Clone() const = 0;
	virtual void ReadDefinition(TemporalInfo& info) = 0;
	virtual void ReadData(tokens::TokenName* name_token, tokens::TokenGuid* guid_token, TemporalInfo& info, bool_t read_cbrace) = 0;

protected:
	virtual void PrintIndent(const uint_t level) const;

protected:
	/*!
		\var type_
		\brief Template-Typ
		Definiert den Typ des Templates.
	*//*!
		\var ref_count_
		\brief reference count of template
		implements a reference count for each tempalte
	*/

	TemplateTypes type_;
	uint_t ref_count_;
};

}} // namespace x::templates

#endif // TEMPLATES_TEMPLATE_H
