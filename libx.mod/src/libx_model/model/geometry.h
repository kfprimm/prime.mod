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
/*!	\file model/geometry.h
	\brief Definition von x::model::Geometry
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::model::Geometry
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef MODEL_GEOMETRY_H
#define MODEL_GEOMETRY_H

#include "../../libx_core/global/global.h"
#include "./stream.h"
#include "./ubyte4stream.h"
#include "./vector2stream.h"
#include "./vector3stream.h"
#include "./skeleton.h"

namespace x { namespace model {

//-----------------------------------------------------------------------------------------------------------
class Entity;




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Geometry-Klasse
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	Geometry-Interface. Definiert das Interface über welches Kaos-Geometryes implementiert 
	werden.

	Geometriedaten sind in dieser Klasse gekapselt damit sogenannte Shared-Geometry möglich wird.
	Dies sind Geometriedaten welche von mehreren Entities verwendet und daher beim Modell gespeichert 
	sind.

	TODO: Erweiterungen nötig: Die Klasse Geometry benötigt eine Möglichkeit zu Erfahren welche
	Semantik die lieferbaren Streams enthalten. Umänderung, dass Streams verwendet werden.
*/
class Geometry
{
public:
	//-------------------------------------------------------------------------------------------------------
	// Konstruktur & Destruktor
	Geometry();
	virtual ~Geometry();

	// Factory-Methoden
	virtual UByte4Stream* CreateDefaultUByte4Stream() const;
	virtual Vector2Stream* CreateDefaultVector2Stream() const;
	virtual Vector3Stream* CreateDefaultVector3Stream() const;

	// Set-Methoden
	virtual void SetVertexCount(const uint_t vertex_count);
	virtual void AddStream(Stream* stream);
	virtual void RemoveStream(Stream* stream);
	virtual void RemoveAllStreams();
	virtual Stream* AddUByte4Stream(const SemanticTypes semantics, const uint_t semantics_info, const uint_t element_count, math::Vector4<ubyte_t>* data);
	virtual Stream* AddVector2Stream(const SemanticTypes semantics, const uint_t semantics_info, const uint_t element_count, math::Vector2f* data);
	virtual Stream* AddVector3Stream(const SemanticTypes semantics, const uint_t semantics_info, const uint_t element_count, math::Vector3f* data);

	// Get-Methoden
	virtual const uint_t GetVertexCount() const;
	virtual const uint_t GetStreamCount() const;
	virtual Stream* GetStream(const uint_t index);
	virtual const Stream* GetStream(const uint_t index) const;
	virtual Stream* GetStream(const SemanticTypes semantics, const uint_t info);
	virtual const Stream* GetStream(const SemanticTypes semantics, const uint_t info) const;

	// bind pose
	virtual void CreateBindPose();
	virtual void UpdateSkinnedModel(Entity* entity, Skeleton* skeleton);
	virtual const uint_t GetBindPoseStreamCount() const;
	virtual Stream* GetBindPoseStream(const uint_t index);
	virtual const Stream* GetBindPoseStream(const uint_t index) const;
	virtual Stream* GetBindPoseStream(const SemanticTypes semantics, const uint_t info);
	virtual const Stream* GetBindPoseStream(const SemanticTypes semantics, const uint_t info) const;

protected:
	/*!
		\var vertex_count_
		\brief Anzahl Vertices

		Setzt die Anzahl Vertices dieses Meshes
	*/

	uint_t vertex_count_;
	vector<Stream*> streams_;
	vector<Stream*> bind_pose_;
};

}} // namespace x::model

#endif // MODEL_GEOMETRY_H
