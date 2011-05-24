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
/*!	\file templates/skinweights.h
	\brief Definition von x::templates::SkinWeights
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::templates::SkinWeights
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef TEMPLATES_SKINWEIGHTS_H
#define TEMPLATES_SKINWEIGHTS_H

#include "../global/global.h"
#include "../temporalinfo.h"
#include "./template.h"

namespace x { namespace templates {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief SkinWeights-Template
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	Implementiert das X-File-SkinWeights-Template.
*/
class SkinWeights : public Template
{
public:
	//-------------------------------------------------------------------------------------------------------
	// Konstruktur & Destruktor
	SkinWeights();
protected:
	virtual ~SkinWeights();

public:
	//-------------------------------------------------------------------------------------------------------
	virtual void PrintFileStructure(const uint_t level) const;
	virtual Template* Clone() const;
	virtual void ReadDefinition(TemporalInfo& info);
	virtual void ReadData(tokens::TokenName* name_token, tokens::TokenGuid* guid_token, TemporalInfo& info, bool_t read_cbrace);

	//-------------------------------------------------------------------------------------------------------
	virtual const string_t& GetTransformNodeName() const;
	virtual const dword_t GetNumWeights() const;
	virtual const dword_t* GetVertexIndices() const;
	virtual const float_t* GetWeights() const;
	virtual const Matrix4x4* GetMatrixOffset() const;

protected:
	string_t transform_node_name_;
	dword_t num_weights_;
	dword_t* vertex_indices_;
	float_t* weights_;
	Matrix4x4* matrix_offset_;
};

}} // namespace x::templates

#endif // TEMPLATES_SKINWEIGHTS_H
