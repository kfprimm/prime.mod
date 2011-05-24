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
/*!	\file model/ubyte4stream.h
	\brief Definition von x::model::UByte4Stream
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::model::UByte4Stream
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef MODEL_UBYTE4STREAM_H
#define MODEL_UBYTE4STREAM_H

#include "../../libx_core/global/global.h"
#include "./stream.h"
#include "./valuetype.h"
#include "./semantictypes.h"
#include "./semanticinfo.h"

namespace x { namespace model {

//-------------------------------------------------------------------------------------------------------
/*!
*/
class UByte4Stream : public Stream
{
public:
	UByte4Stream();
	virtual ~UByte4Stream();

	// Daten setzen
	virtual void SetSemantics(const SemanticTypes semantics);
	virtual void SetSemanticsInfo(const uint_t semantics_info);
	virtual void SetData(const uint_t element_count, math::Vector4<ubyte_t>* data);

	// Informationen über den UByte4Stream
	virtual const uint_t GetElementCount() const;
	virtual const ValueType GetValueType() const;
	virtual const SemanticTypes GetSemantics() const;
	virtual const uint_t GetSemanticInfo() const;

	// Bytedaten
	/// Daten zum Zugriff bereit machen
	virtual ubyte_t* LockData(uint_t& stride);
	/// Zugriff auf Daten. Nur Gültig innerhalb eins LockData/UnlockData-Paares
	virtual ubyte_t* GetData(uint_t& stride);
	/// Daten fertig "bearbeitet". Wieder freigeben
	virtual void UnlockData();

	virtual const bool_t IsDirty() const;
	virtual void ClearDirtyFlag();

	virtual Stream* Clone() const;

protected:
	SemanticTypes semantic_type_;
	uint_t semantic_info_;
	uint_t element_count_;
	math::Vector4<ubyte_t>* array_;
	bool_t is_dirty_;
};

}} // namespace x::model

#endif // MODEL_UBYTE4STREAM_H
