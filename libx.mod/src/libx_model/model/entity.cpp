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
/*!	\file model/entity.cpp
	\brief Implementation von x::model::Entity
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::model::Entity
*/
//-----------------------------------------------------------------------------------------------------------
#include "./model.h"
#include "./frame.h"
#include "./entity.h"
#include "./geometry.h"

namespace x { namespace model {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
Entity::Entity()
:	model_(NULL),
	frame_(NULL),
	name_(),
	primitive_type_(PrimitiveTriangleList),
	index_count_(0),
	index_size_(0),
	indices_(NULL),
	geometry_(NULL),
	material_(NULL)
{
	geometry_ = new Geometry;
}





//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
Entity::~Entity()
{
	if (indices_)
	{
		delete[] indices_;
		indices_ = NULL;
	}
	if (geometry_)
	{
		delete geometry_;
		geometry_ = NULL;
	}
	if (material_)
	{
		delete material_;
		material_ = NULL;
	}
}




//-----------------------------------------------------------------------------------------------------------
void Entity::SetModel(
	Model* model
)
{
	model_ = model;
}




//-----------------------------------------------------------------------------------------------------------
Model* Entity::GetModel()
{
	return model_;
}




//-----------------------------------------------------------------------------------------------------------
const Model* Entity::GetModel() const
{
	return model_;
}




//-----------------------------------------------------------------------------------------------------------
void Entity::SetParentFrame(
	Frame* frame
)
{
	frame_ = frame;
}




//-----------------------------------------------------------------------------------------------------------
Frame* Entity::GetParentFrame()
{
	return frame_;
}




//-----------------------------------------------------------------------------------------------------------
const Frame* Entity::GetParentFrame() const
{
	return frame_;
}




//-----------------------------------------------------------------------------------------------------------
void Entity::SetName(
	const string_t& name
)
{
	name_ = name;
}




//-----------------------------------------------------------------------------------------------------------
const string_t& Entity::GetName() const
{
	return name_;
}




//-----------------------------------------------------------------------------------------------------------
void Entity::SetPrimitiveType(
	const PrimitiveType primitive_type
)
{
	primitive_type_ = primitive_type;
}




//-----------------------------------------------------------------------------------------------------------
void Entity::SetIndexCount(
	const uint_t index_count
)
{
	index_count_ = index_count;
}




//-----------------------------------------------------------------------------------------------------------
void Entity::SetIndexSize(
	const uint_t size
)
{
	index_size_ = size;
}




//-----------------------------------------------------------------------------------------------------------
void Entity::SetIndexData(
	ubyte_t* data
)
{
	indices_ = data;
}




//-----------------------------------------------------------------------------------------------------------
const PrimitiveType Entity::GetPrimitiveType() const
{
	return primitive_type_;
}




//-----------------------------------------------------------------------------------------------------------
const uint_t Entity::GetPrimitiveCount() const
{
	switch (primitive_type_)
	{
	case PrimitivePointList:	 return index_count_;
	case PrimitiveLineList:		 return index_count_ / 2;
	case PrimitiveLineStrip:	 return index_count_ - 1;
	case PrimitiveTriangleList:	 return index_count_ / 3;
	case PrimitiveTriangleStrip: return index_count_ - 2;
	case PrimitiveTriangleFan:	 return index_count_ - 2;
	default: throw Exception("invalid primitive type set!");
	}
}




//-----------------------------------------------------------------------------------------------------------
void Entity::GetIndicesFromFace(
	const uint_t index,
	uint_t& indexcount,
	udword_t* indices
) const
{
	switch (primitive_type_)
	{
	case PrimitivePointList: {
		indexcount = 1;
		indices[0] = index;
		break;
	}
	case PrimitiveLineList: {
		indexcount = 2;
		for (uint_t i = 0; i < 2; ++i)
			indices[i] = index*2+i;
		break;
	}
	case PrimitiveLineStrip: {
		indexcount = 2;
		for (uint_t i = 0; i < 2; ++i)
			indices[i] = index+i;
		break;
	}
	case PrimitiveTriangleList: {
		indexcount = 3;
		for (uint_t i = 0; i < 3; ++i)
			indices[i] = index*3+i;
		break;
	}
	case PrimitiveTriangleStrip: {
		indexcount = 3;
		for (uint_t i = 0; i < 3; ++i)
			indices[i] = index+i;
		break;
	}
	case PrimitiveTriangleFan: {
		indexcount = 3;
		for (uint_t i = 0; i < 3; ++i)
			indices[i] = (i == 0 ? 0 : index+i);
		break;
	}
	default: {
		throw Exception("invalid primitive type set!");
	}
	}
}




//-----------------------------------------------------------------------------------------------------------
const uint_t Entity::GetIndexCount() const
{
	return index_count_;
}




//-----------------------------------------------------------------------------------------------------------
const uint_t Entity::GetIndexSize() const
{
	return index_size_;
}




//-----------------------------------------------------------------------------------------------------------
const ubyte_t* Entity::GetIndexData() const
{
	return indices_;
}




//-----------------------------------------------------------------------------------------------------------
void Entity::UseSharedGeometry()
{
	geometry_ = model_->GetGeometry();
}




//-----------------------------------------------------------------------------------------------------------
void Entity::SetGeometry(
	Geometry* geometry
)
{
	geometry_ = geometry;
}




//-----------------------------------------------------------------------------------------------------------
Geometry* Entity::GetGeometry()
{
	return geometry_;
}




//-----------------------------------------------------------------------------------------------------------
const Geometry* Entity::GetGeometry() const
{
	return geometry_;
}




//-----------------------------------------------------------------------------------------------------------
void Entity::SetMaterial(
	Material* material
)
{
	material_ = material;
}




//-----------------------------------------------------------------------------------------------------------
Material* Entity::GetMaterial()
{
	return material_;
}




//-----------------------------------------------------------------------------------------------------------
const Material* Entity::GetMaterial() const
{
	return material_;
}





//-----------------------------------------------------------------------------------------------------------
void Entity::ComputeNormalVectors()
{
	Stream* positions = geometry_->GetStream(SemanticTypePosition, 0);

	math::Vector3f* normals = new math::Vector3f[geometry_->GetVertexCount()];
	for (uint_t i = 0; i < geometry_->GetVertexCount(); ++i)
		normals[i] = math::Vector3f(0.0f, 0.0f, 0.0f);

	switch (primitive_type_)
	{
	case PrimitiveTriangleStrip: {
		for (uint_t i = 0; i < (index_count_ - 2); ++i)
			ComputeTriangleNormal(i, i+1, i+2, positions, normals);
		break;
	}
	case PrimitiveTriangleList: {
		for (uint_t i = 0; i < (index_count_ / 3); ++i)
			ComputeTriangleNormal(i*3+0, i*3+1, i*3+2, positions, normals);
		break;
	}
	case PrimitiveTriangleFan: {
		for (uint_t i = 0; i < (index_count_ - 2); ++i)
			ComputeTriangleNormal(0, i+1, i+2, positions, normals);
		break;
	}
	default:
		throw Exception("invalid primitive type, can't generate normal for those!");
	}

	for (uint_t i = 0; i < geometry_->GetVertexCount(); ++i)
		normals[i].Normalize();

	// put component
	geometry_->AddVector3Stream(SemanticTypeNormal, 0, geometry_->GetVertexCount(), normals);
}




//-----------------------------------------------------------------------------------------------------------
void Entity::ComputeTangentVectors()
{
	Stream* positions = geometry_->GetStream(SemanticTypePosition, 0);
	Stream* normals = geometry_->GetStream(SemanticTypeNormal, 0);
	Stream* texcoords = geometry_->GetStream(SemanticTypeTextureCoordinate, 0);

	math::Vector3f* tangents = new math::Vector3f[geometry_->GetVertexCount()];
	for (uint_t i = 0; i < geometry_->GetVertexCount(); ++i)
		tangents[i] = math::Vector3f(0.0f, 0.0f, 0.0f);

	switch (primitive_type_)
	{
	case PrimitiveTriangleStrip: {
		for (uint_t i = 0; i < (index_count_ - 2); ++i)
			ComputeTriangleTangent(i, i+1, i+2, positions, normals, texcoords, tangents);
		break;
	}
	case PrimitiveTriangleList: {
		for (uint_t i = 0; i < (index_count_ / 3); ++i)
			ComputeTriangleTangent(i*3+0, i*3+1, i*3+2, positions, normals, texcoords, tangents);
		break;
	}
	case PrimitiveTriangleFan: {
		for (uint_t i = 0; i < (index_count_ - 2); ++i)
			ComputeTriangleTangent(0, i+1, i+2, positions, normals, texcoords, tangents);
		break;
	}
	default:
		throw Exception("invalid primitive type, can't generate normal for those!");
	}

	for (uint_t i = 0; i < geometry_->GetVertexCount(); ++i)
	{
		const math::Vector3f& n = normals->GetVector3(i);
		const math::Vector3f& t = tangents[i];

		// Gram-Schmidt orthogonalize
		tangents[i] = (t - n * (n * t));
		tangents[i].Normalize();
	}

	// put component
	geometry_->AddVector3Stream(SemanticTypeTangent, 0, geometry_->GetVertexCount(), tangents);
}




//-----------------------------------------------------------------------------------------------------------
void Entity::CreateBindPose()
{
	geometry_->CreateBindPose();
}




//-----------------------------------------------------------------------------------------------------------
void Entity::UpdateSkinnedModel(
	Skeleton* skeleton
)
{
	geometry_->UpdateSkinnedModel(this, skeleton);
}




//-----------------------------------------------------------------------------------------------------------
void Entity::ComputeTriangleNormal(
	const uint_t i0, 
	const uint_t i1,
	const uint_t i2, 
	Stream* positions,
	math::Vector3f* normals
) const
{
	udword_t index0 = 0;
	udword_t index1 = 0;
	udword_t index2 = 0;
	if (index_size_ == sizeof(uword_t)*8)
	{
		uword_t* indices = reinterpret_cast<uword_t*>(indices_);
		index0 = indices[i0];
		index1 = indices[i1];
		index2 = indices[i2];
	}
	else if (index_size_ == sizeof(udword_t)*8)
	{
		udword_t* indices = reinterpret_cast<udword_t*>(indices_);
		index0 = indices[i0];
		index1 = indices[i1];
		index2 = indices[i2];
	}
	else
	{
		throw Exception("invalid index size!");
	}

	// check for degenerate triangles
	if (index0 == index1 || index1 == index2 || index0 == index2)
		return;

	const math::Vector3f& v0 = positions->GetVector3(index0);
	const math::Vector3f& v1 = positions->GetVector3(index1);
	const math::Vector3f& v2 = positions->GetVector3(index2);

	// compute the normal k(length provides the weighted sum)
	math::Vector3f edge1, edge2, normal;
	edge1 = v1 - v0;
	edge2 = v2 - v0;
	normal = edge1.Cross(edge2);

	normals[index0] += normal;
	normals[index1] += normal;
	normals[index2] += normal;
}




//-----------------------------------------------------------------------------------------------------------
/*
 * LINKS zum Thema:
 *  - http://www.terathon.com/code/tangent.html
 *	- http://nehe.gamedev.net/data/articles/article.asp?article=20
 *	- http://www.blacksmith-studios.dk/projects/downloads/tangent_matrix_derivation.php
 */
void Entity::ComputeTriangleTangent(
	const uint_t i0,
	const uint_t i1,
	const uint_t i2, 
	Stream* positions,
	Stream* normals, 
	Stream* texcoords, 
	math::Vector3f* tangents
) const
{
	/*
	Vec1 = Vertex3 – Vertex2
	Vec2 = Vertex1 – Vertex2
	DeltaU1 = Vertex3.u - Vertex2.u
	DeltaU2 = Vertex1.u - Vertex2.u
	DirectionV = |DeltaU2*Vec1-DeltaU1*Vec2|
	DirectionU = |DirectionV x Vertex.N|
	DirectionW = |DirectionU x DirectionV|
	*/

	udword_t index0 = 0;
	udword_t index1 = 0;
	udword_t index2 = 0;
	if (index_size_ == sizeof(uword_t)*8)
	{
		uword_t* indices = reinterpret_cast<uword_t*>(indices_);
		index0 = indices[i0];
		index1 = indices[i1];
		index2 = indices[i2];
	}
	else if (index_size_ == sizeof(udword_t)*8)
	{
		udword_t* indices = reinterpret_cast<udword_t*>(indices_);
		index0 = indices[i0];
		index1 = indices[i1];
		index2 = indices[i2];
	}
	else
	{
		throw Exception("invalid index size!");
	}

	// check for degenerate triangles
	if (index0 == index1 || index1 == index2 || index0 == index2)
		return;

	const math::Vector3f& v1 = positions->GetVector3(index0);
	const math::Vector3f& v2 = positions->GetVector3(index1);
	const math::Vector3f& v3 = positions->GetVector3(index2);
	const math::Vector2f& w1 = texcoords->GetVector2(index0);
	const math::Vector2f& w2 = texcoords->GetVector2(index1);
	const math::Vector2f& w3 = texcoords->GetVector2(index2);

	// compute the normal (length provides the weighted sum)
	float x1 = v2.x - v1.x;
	float x2 = v3.x - v1.x;
	float y1 = v2.y - v1.y;
	float y2 = v3.y - v1.y;
	float z1 = v2.z - v1.z;
	float z2 = v3.z - v1.z;

	float s1 = w2.x - w1.x;
	float s2 = w3.x - w1.x;
	float t1 = w2.y - w1.y;
	float t2 = w3.y - w1.y;

	float r = 1.0f / (s1 * t2 - s2 * t1);
	math::Vector3f sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r, (t2 * z1 - t1 * z2) * r);
	//math::Vector3f tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,	(s1 * z2 - s2 * z1) * r);

	tangents[index0] += sdir;
	tangents[index1] += sdir;
	tangents[index2] += sdir;

/*	math::Vector3f edge1, edge2, normal;
	edge1 = v1 - v0;
	edge2 = v2 - v0;
	normal = edge1.Cross(edge2);

	float_t delta_u1 = t1.x - t0.x;
	float_t delta_u2 = t2.x - t0.x;
	
	math::Vector3f binormal, tangent;
	binormal = v2*delta_u2 - v0*delta_u1;
	binormal.Normalize();
	tangent = binormal.Cross(normal);
	tangent.Normalize();

	tangents[index0] += tangent;
	tangents[index1] += tangent;
	tangents[index2] += tangent;*/
}

}} // namespace x::model

