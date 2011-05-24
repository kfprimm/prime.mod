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
/*!	\file templates/meshmateriallist.h
	\brief Definition von x::templates::MeshMaterialList
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::templates::MeshMaterialList
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef TEMPLATES_MESHMATERIALLIST_H
#define TEMPLATES_MESHMATERIALLIST_H

#include "../global/global.h"
#include "../temporalinfo.h"
#include "./template.h"

namespace x { namespace templates {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief MeshMaterialList-Template
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	Implementiert das X-File-MeshMaterialList-Template.
*/
class MeshMaterialList : public Template
{
public:
	typedef vector<udword_t> faceindices_container_t;
	typedef vector<Material*> material_container_t;

public:
	//-------------------------------------------------------------------------------------------------------
	// Konstruktur & Destruktor
	MeshMaterialList();
protected:
	virtual ~MeshMaterialList();

public:
	//-------------------------------------------------------------------------------------------------------
	virtual void PrintFileStructure(const uint_t level) const;
	virtual Template* Clone() const;
	virtual void ReadDefinition(TemporalInfo& info);
	virtual void ReadData(tokens::TokenName* name_token, tokens::TokenGuid* guid_token, TemporalInfo& info, bool_t read_cbrace);

	//-------------------------------------------------------------------------------------------------------
	virtual const uint_t GetMaterialCount() const;
	virtual const uint_t GetFaceIndexCount() const;
	virtual const udword_t* GetFaceIndices() const;
	virtual const material_container_t& GetMaterials() const;

protected:
	udword_t material_count_;
	udword_t face_index_count_;
	udword_t* face_indexes_;
	material_container_t materials_;
};

}} // namespace x::templates

#endif // TEMPLATES_MESHMATERIALLIST_H
