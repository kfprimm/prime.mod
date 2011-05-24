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
/*!	\file templates/meshface.h
	\brief Definition von x::templates::MeshFace
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::templates::MeshFace
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef TEMPLATES_MESHFACE_H
#define TEMPLATES_MESHFACE_H

#include "../global/global.h"
#include "../temporalinfo.h"
#include "./template.h"

namespace x { namespace templates {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief MeshFace-Template
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	Implementiert das X-File-MeshFace-Template.
*/
class MeshFace : public Template
{
public:
	typedef vector<udword_t> face_indices_t;

public:
	//-------------------------------------------------------------------------------------------------------
	// Konstruktur & Destruktor
	MeshFace();
protected:
	virtual ~MeshFace();

public:
	//-------------------------------------------------------------------------------------------------------
	virtual void PrintFileStructure(const uint_t level) const;
	virtual Template* Clone() const;
	virtual void ReadDefinition(TemporalInfo& info);
	virtual void ReadData(tokens::TokenName* name_token, tokens::TokenGuid* guid_token, TemporalInfo& info, bool_t read_cbrace);

	//-------------------------------------------------------------------------------------------------------
	virtual const udword_t GetIndexCount() const;
	virtual const face_indices_t& GetIndices() const;

protected:
	udword_t index_count_;
	face_indices_t indices_;
};

}} // namespace x::templates

#endif // TEMPLATES_MESHFACE_H
