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
/*!	\file templates/meshnormals.h
	\brief Definition von x::templates::MeshNormals
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::templates::MeshNormals
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef TEMPLATES_MESHNORMALS_H
#define TEMPLATES_MESHNORMALS_H

#include "../global/global.h"
#include "../temporalinfo.h"
#include "./template.h"

namespace x { namespace templates {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief MeshNormals-Template
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	Implementiert das X-File-MeshNormals-Template.
*/
class MeshNormals : public Template
{
public:
	//-------------------------------------------------------------------------------------------------------
	// Konstruktur & Destruktor
	MeshNormals();
protected:
	virtual ~MeshNormals();

public:
	//-------------------------------------------------------------------------------------------------------
	virtual void PrintFileStructure(const uint_t level) const;
	virtual Template* Clone() const;
	virtual void ReadDefinition(TemporalInfo& info);
	virtual void ReadData(tokens::TokenName* name_token, tokens::TokenGuid* guid_token, TemporalInfo& info, bool_t read_cbrace);

	//-------------------------------------------------------------------------------------------------------
	virtual const uint_t GetNormalCount() const;
	virtual const ubyte_t* GetNormals() const;
	virtual const uint_t GetFaceCount() const;
	virtual const ubyte_t* GetFaces() const;

protected:
	udword_t normal_count_;
	ubyte_t* normals_;
	udword_t face_count_;
	udword_t faces_size_;
	ubyte_t* faces_;
};

}} // namespace x::templates

#endif // TEMPLATES_MESHNORMALS_H
