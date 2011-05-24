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
/*!	\file serializer.h
	\brief Definition von x::Serializer
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::Serializer
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "./global/global.h"
#include "./tokens/tokens.h"
#include "./templates/templates.h"
#include "./header.h"

namespace x {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief .x-Serializer-Klasse
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	Der Serializer wird verwendet um .x-Files importieren und exportieren zu können.

	\note
		Im Moment nehme ich an, dass es nicht darauf ankommt was für eine Version vorliegt, d.h. Templates
		dürften über verschiedene Versionen nicht ändern. Ich nehme an, dass in neueren Versionen einfach
		neue Templates hinzukommen. Deshalb überlesen wir die info einfach

*/
class Serializer
{
public:
	//
	// allow those tokens to use protected members as they are the only ones which need to
	// access read/write methodes.
	//
	friend class x::tokens::TokenName;
	friend class x::tokens::TokenGuid;
	friend class x::tokens::TokenString;

public:
	typedef vector<templates::Template*> instances_t;

public:
	//-------------------------------------------------------------------------------------------------------
	// Konstruktur & Destruktor
	Serializer();
	virtual ~Serializer();

	//-------------------------------------------------------------------------------------------------------
	virtual void Import(BufferInputStream& input, instances_t& instances);
	//virtual void Export(/*SharedPtr<Mesh> pMesh, */BufferOutputStream& Output);

	//-------------------------------------------------------------------------------------------------------
	virtual void ReadFile(TemporalInfo& info);
	templates::Template* InstanciateTemplateByGuid(TemporalInfo& info, const Guid& guid) const;
	templates::Template* InstanciateTemplateByName(TemporalInfo& info, const string_t& name) const;
	virtual templates::Template* ReadTemplateDefinition(string_t& template_name, TemporalInfo& info);
	virtual templates::Template* ReadTemplateData(tokens::Token* token, TemporalInfo& info);
	virtual void DeleteTemporalInfo(TemporalInfo& info);

	//-------------------------------------------------------------------------------------------------------
	virtual udword_t GetNextDword(TemporalInfo& info);
	virtual float_t GetNextFloat32(TemporalInfo& info);

	//-------------------------------------------------------------------------------------------------------
	virtual void PrintFileStructure(TemporalInfo& info);

};

} // namespace x

#endif // SERIALIZER_H
