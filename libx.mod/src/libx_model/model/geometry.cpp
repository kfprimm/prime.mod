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
/*!	\file model/geometry.cpp
	\brief Implementation von x::model::Geometry
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::model::Geometry
*/
//-----------------------------------------------------------------------------------------------------------
#include "./geometry.h"
#include "./entity.h"
#include "./frame.h"

namespace x { namespace model {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
Geometry::Geometry()
:	vertex_count_(0),
	streams_(),
	bind_pose_()
{
}





//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
Geometry::~Geometry()
{
	while (streams_.size() > 0)
	{
		Stream* stream = streams_.back();
		streams_.pop_back();
		delete stream;
	}
	while (bind_pose_.size() > 0)
	{
		Stream* stream = bind_pose_.back();
		bind_pose_.pop_back();
		delete stream;
	}
}




//-----------------------------------------------------------------------------------------------------------
UByte4Stream* Geometry::CreateDefaultUByte4Stream() const
{
	return new UByte4Stream();
}




//-----------------------------------------------------------------------------------------------------------
Vector2Stream* Geometry::CreateDefaultVector2Stream() const
{
	return new Vector2Stream();
}




//-----------------------------------------------------------------------------------------------------------
Vector3Stream* Geometry::CreateDefaultVector3Stream() const
{
	return new Vector3Stream();
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Anzahl Vertices setzen
	\param vertex_count

	Setzt die Anzahl Vertices
*/
void Geometry::SetVertexCount(
	const uint_t vertex_count
)
{
	if (vertex_count == 0)
	{
		throw Exception("Vertex Count of 0 is not valid!");
	}
	vertex_count_ = vertex_count;
}




//-----------------------------------------------------------------------------------------------------------
void Geometry::AddStream(
	Stream* stream
)
{
	if (stream != NULL)
	{
		streams_.push_back(stream);
	}
	else
	{
		//LOG_WRITE_L1_1(LOGSEVERITY_ERROR, "kkaos", "Geometry::AddStream", "Tried to add a NULL-Stream!");
	}
}




//-----------------------------------------------------------------------------------------------------------
void Geometry::RemoveStream(
	Stream* stream
)
{
	if (stream)
	{
		for (uint_t i = 0; i < streams_.size(); ++i)
		{
			if (streams_[i] == stream)
			{
				delete streams_[i];
				streams_.erase(streams_.begin()+i);
				return;
			}
		}
	}
	else
	{
		//LOG_WRITE_L1_1(LOGSEVERITY_ERROR, "kkaos", "Geometry::AddStream", "Tried to remove a NULL-Stream!");
	}
}




//-----------------------------------------------------------------------------------------------------------
void Geometry::RemoveAllStreams()
{
	while (!streams_.empty())
	{
		delete streams_[streams_.size()-1];
		streams_.pop_back();
	}
}




//-----------------------------------------------------------------------------------------------------------
Stream* Geometry::AddUByte4Stream(
	const SemanticTypes semantics,
	const uint_t semantics_info,
	const uint_t element_count,
	math::Vector4<ubyte_t>* data
)
{
	UByte4Stream* stream = CreateDefaultUByte4Stream();
	stream->SetSemantics(semantics);
	stream->SetSemanticsInfo(semantics_info);
	stream->SetData(element_count, data);
	AddStream(stream);
	return stream;
}




//-----------------------------------------------------------------------------------------------------------
Stream* Geometry::AddVector2Stream(
	const SemanticTypes semantics,
	const uint_t semantics_info,
	const uint_t element_count,
	math::Vector2f* data
)
{
	Vector2Stream* stream = CreateDefaultVector2Stream();
	stream->SetSemantics(semantics);
	stream->SetSemanticsInfo(semantics_info);
	stream->SetData(element_count, data);
	AddStream(stream);
	return stream;
}




//-----------------------------------------------------------------------------------------------------------
Stream* Geometry::AddVector3Stream(
	const SemanticTypes semantics,
	const uint_t semantics_info,
	const uint_t element_count,
	math::Vector3f* data
)
{
	Vector3Stream* stream = CreateDefaultVector3Stream();
	stream->SetSemantics(semantics);
	stream->SetSemanticsInfo(semantics_info);
	stream->SetData(element_count, data);
	AddStream(stream);
	return stream;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Anzahl der Vertices zurückgeben
	\return Anzahl

	Gibt die Anzahl der Vertices welche dieses Geometry-Objekt besitzt zurück.
*/
const uint_t Geometry::GetVertexCount() const
{
	return vertex_count_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Anzahl der Vertex-Komponenten zurückgeben
	\return Anzahl

	Gibt die Anzahl der Komponenten zurück welche jeder der Vertices besitzt.
*/
const uint_t Geometry::GetStreamCount() const
{
	return (uint_t)streams_.size();
}




//-----------------------------------------------------------------------------------------------------------
Stream* Geometry::GetStream(
	const uint_t index
)
{
	return streams_[index];
}




//-----------------------------------------------------------------------------------------------------------
const Stream* Geometry::GetStream(
	const uint_t index
) const
{
	return streams_[index];
}




//-----------------------------------------------------------------------------------------------------------
Stream* Geometry::GetStream(
	const SemanticTypes semantics, 
	const uint_t info
)
{
	typedef vector<Stream*>::iterator iterator;
	for (iterator begin = streams_.begin(); begin != streams_.end(); ++begin)
	{
		if ((*begin)->GetSemantics() == semantics && (*begin)->GetSemanticInfo() == info)
		{
			return (*begin);
		}
	}
	return NULL;
}




//-----------------------------------------------------------------------------------------------------------
const Stream* Geometry::GetStream(
	const SemanticTypes semantics, 
	const uint_t info
) const
{
	return const_cast<Geometry*>(this)->GetStream(semantics, info);
}




//-----------------------------------------------------------------------------------------------------------
void Geometry::CreateBindPose()
{
	for (uint_t i = 0; i < streams_.size(); ++i)
	{
		switch (streams_[i]->GetSemantics())
		{
		case SemanticTypePosition:
		case SemanticTypeNormal:
		case SemanticTypeTangent:
			bind_pose_.push_back(streams_[i]->Clone());
		}
	}
}




//-----------------------------------------------------------------------------------------------------------
void Geometry::UpdateSkinnedModel(
	Entity* entity,
	Skeleton* skeleton
)
{
 	Stream* bone_indices_stream = GetStream(SemanticTypeBoneIndices, 0);
	Stream* bone_weights = GetStream(SemanticTypeBoneWeights, 0);

	// calculate bone count
	uint_t bone_count = 0;
	switch (bone_weights->GetValueType())
	{
	case ValueVector2: bone_count = 2; break;
	case ValueVector3: bone_count = 3; break;
	}

	// avoid calls to new
	const uint_t bind_pose_max_size = 12;
	if (bind_pose_.size() > bind_pose_max_size)
	{
		throw Exception("size of bind_pose_-Vector not supported!");
	}

	Stream* targets[bind_pose_max_size];
	for (uint_t i = 0; i < bind_pose_.size(); ++i)
	{
		if (bind_pose_[i]->GetValueType() != ValueVector3)
		{
			throw Exception("invalid valuetype inside bind_pose_");
		}
		targets[i] = GetStream(bind_pose_[i]->GetSemantics(), bind_pose_[i]->GetSemanticInfo());
	}

	// flags indicating if computation needs to be done using the world-matrix or
	// the inv-transpose-world-matrix (for normals, tangents, etc.)
	bool_t matrix_flags[bind_pose_max_size];
	for (uint_t i = 0; i < bind_pose_.size(); ++i)
	{
		matrix_flags[i] = (
			bind_pose_[i]->GetSemantics() != SemanticTypeNormal && 
			bind_pose_[i]->GetSemantics() != SemanticTypeTangent
		);
	}

	// Lock streams
	for (uint_t i = 0; i < bind_pose_.size(); ++i)
	{
		uint_t stride_dummy = 0;
		targets[i]->LockData(stride_dummy);
	}

	// for each vertex
	for (uint_t i = 0; i < vertex_count_; ++i)
	{
		// get weights for this bone
		float_t weights[4] = { 0, 0, 0, 0 };
		switch (bone_count)
		{
		case 2: {
			const math::Vector2f& v = bone_weights->GetVector2(i);
			weights[0] = v.x;
			weights[1] = v.y;
			break;
		}
		case 3: {
			const math::Vector3f& v = bone_weights->GetVector3(i);
			weights[0] = v.x;
			weights[1] = v.y;
			weights[2] = v.z;
			break;
		}
		}

		// get bone indices
		const math::Vector4<ubyte_t>& indices = bone_indices_stream->GetUByte4(i);

		// for each stream
		for (uint_t j = 0; j < bind_pose_.size(); ++j)
		{
			Stream* bind_pose = bind_pose_[j];
			Stream* target = targets[j];

			// bind_pose_ may only contain vector3-streams so it's possible to skip
			// the type check
			math::Vector3f v(0.0f, 0.0f, 0.0f);
			const math::Vector3f& source = bind_pose->GetVector3(i);
			for (uint_t k = 0; k < bone_count; ++k)
			{
				// skip bones with weight 0
				if (weights[k] == 0) 
					continue;
				// get bone matrix
				ubyte_t index = indices[k];

				math::Vector3f transformed;
				if (matrix_flags[j])
				{
					const math::Matrix4x4f& bone_matrix = skeleton->GetMatrix(index);
					transformed = bone_matrix * source;
				}
				else
				{
					const math::Matrix4x4f& bone_matrix = skeleton->GetInvTransposeMatrix(index);
					transformed = bone_matrix * source;
				}
				// add weighted value
				v = v + transformed * weights[k];
			}
			target->SetVector3(i, v);
		}
	}

	for (uint_t i = 0; i < bind_pose_.size(); ++i)
	{
		targets[i]->UnlockData();
	}
}




//-----------------------------------------------------------------------------------------------------------
const uint_t Geometry::GetBindPoseStreamCount() const
{
	return (uint_t)bind_pose_.size();
}




//-----------------------------------------------------------------------------------------------------------
Stream* Geometry::GetBindPoseStream(
	const uint_t index
)
{
	return bind_pose_[index];
}




//-----------------------------------------------------------------------------------------------------------
const Stream* Geometry::GetBindPoseStream(
	const uint_t index
) const
{
	return bind_pose_[index];
}




//-----------------------------------------------------------------------------------------------------------
Stream* Geometry::GetBindPoseStream(
	const SemanticTypes semantics, 
	const uint_t info
)
{
	typedef vector<Stream*>::iterator iterator;
	for (iterator begin = bind_pose_.begin(); begin != bind_pose_.end(); ++begin)
	{
		if ((*begin)->GetSemantics() == semantics && (*begin)->GetSemanticInfo() == info)
		{
			return (*begin);
		}
	}
	return NULL;
}




//-----------------------------------------------------------------------------------------------------------
const Stream* Geometry::GetBindPoseStream(
	const SemanticTypes semantics, 
	const uint_t info
) const
{
	return const_cast<Geometry*>(this)->GetBindPoseStream(semantics, info);
}

}} // namespace x::model
