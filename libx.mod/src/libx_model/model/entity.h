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
/*!	\file model/entity.h
	\brief Definition von x::model::Entity
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::model::Entity
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef MODEL_ENTITY_H
#define MODEL_ENTITY_H

#include "../../libx_core/global/global.h"
#include "./geometry.h"
#include "./material.h"
#include "./primitivetype.h"
#include "./skeleton.h"

namespace x { namespace model {

//-----------------------------------------------------------------------------------------------------------
class Model;
class Frame;




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Entity-Klasse
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	Eine Entity stellt einen einzelnen Teil eines Meshes dar. Sie ist immer einem Frame 
	untergeordnet, welches seine Transformation festlegt.

	Zusäthliche ist wichtig zu bemerken, dass die Geometriedaten einer Entity in einem
	Objekt der Klasse Geometry gespeichert sind. => Geometry
*/
class Entity
{
public:
	// Konstruktor / Destruktor und This-Pointer
	Entity();
	virtual ~Entity();

	// Model Structuring
	virtual void SetModel(Model* model);
	virtual Model* GetModel();
	virtual const Model* GetModel() const;
	virtual void SetParentFrame(Frame* frame);
	virtual Frame* GetParentFrame();
	virtual const Frame* GetParentFrame() const;
	virtual void SetName(const string_t& name);
	virtual const string_t& GetName() const;

	// Primitive Information
	virtual void SetPrimitiveType(const PrimitiveType primitive_type);
	virtual const PrimitiveType GetPrimitiveType() const;
	virtual const uint_t GetPrimitiveCount() const;
	virtual void GetIndicesFromFace(const uint_t index, uint_t& indexcount, udword_t* indices) const;

	// Indices 
	virtual void SetIndexCount(const uint_t index_count);
	virtual void SetIndexSize(const uint_t size);
	virtual void SetIndexData(ubyte_t* data);
	virtual const uint_t GetIndexCount() const;
	virtual const uint_t GetIndexSize() const;
	virtual const ubyte_t* GetIndexData() const;

	// Geometric Information
	virtual void UseSharedGeometry();
	virtual void SetGeometry(Geometry* geometry);
	virtual Geometry* GetGeometry();
	virtual const Geometry* GetGeometry() const;

	// Material used
	virtual void SetMaterial(Material* material); 
	virtual Material* GetMaterial();
	virtual const Material* GetMaterial() const;

	// generate geometry components => Move to Geometry!
	virtual void ComputeNormalVectors(); // needs position vectors
	virtual void ComputeTangentVectors(); // needs normal vectors and texture coordinates
	virtual void CreateBindPose();
	virtual void UpdateSkinnedModel(Skeleton* skeleton);

protected:
	void ComputeTriangleNormal(const uint_t i0, const uint_t i1, const uint_t i2, Stream* positions, math::Vector3f* normals) const;
	void ComputeTriangleTangent(const uint_t i0, const uint_t i1, const uint_t i2, Stream* positions, Stream* normals, Stream* texcoords, math::Vector3f* tangents) const;

protected:
	/*!
		\var model_
		\brief Pointer auf den Model

		Pointer auf den Model zu welchem diese Entity gehört.
	*//*!
		\var frame_
		\brief Pointer auf das übergeordnete Frame

		Pointer auf das übergeordnete Frame, dessen Transformation diese Entity
		erbt.
	*/

	Model* model_;
	Frame* frame_;

	/*!
		\var name_
		\brief Name dieser Entity

		Speichert den Namen dieser Entity.
	*/

	string_t name_;

	/*!
		\var primitive_type_
		\brief Primitive-Typ

		Definiert den Primitive-Typ welchen die Geometriedaten dieser Entity haben.
	*//*!
		\var index_count_
		\brief Anzahl Indices

		Anzahl in dieser Entity gespeicherte Indices auf Vertices der Geometrie.
	*//*!
		\var index_size_
		\brief Grösse eines Index

		Definiert die Grösse eines einzelnen Index in Bytes.
	*//*!
		\var indices_
		\brief Index-Array

		Pointer auf das Array welches die Indices dieser Entity enthält.
	*//*!
		\var geometry_
		\brief Geometriedaten

		Pointer auf die Geometriedaten dieser Entity.
	*/

	PrimitiveType primitive_type_;
	uint_t index_count_;
	uint_t index_size_;
	ubyte_t* indices_;
	Geometry* geometry_;
	Material* material_;
};

}} // namespace x::model

#endif // MODEL_ENTITY_H
