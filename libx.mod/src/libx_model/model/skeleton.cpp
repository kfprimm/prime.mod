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
/*!	\file model/skeleton.cpp
	\brief Implementation von x::model::Skeleton
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::model::Skeleton
*/
//-----------------------------------------------------------------------------------------------------------
#include "./skeleton.h"

namespace x { namespace model {

//-----------------------------------------------------------------------------------------------------------
Skeleton::Skeleton()
:	root_bone_(NULL),
	bones_(),
	matrices_(NULL),
	inv_transpose_matrices_(NULL)
{
}




//-----------------------------------------------------------------------------------------------------------
Skeleton::~Skeleton()
{
	while (bones_.size() > 0)
	{
		Bone* bone = bones_.back();
		bones_.pop_back();
		delete bone;
	}
	delete[] matrices_;
	delete[] inv_transpose_matrices_;
}




//-----------------------------------------------------------------------------------------------------------
Bone* Skeleton::CreateRootBone(
	const string_t& name,
	const math::Vector3f& translate,
	const math::Quaternionf& rotate,
	const math::Vector3f& scale
)
{
	Bone* bone(new Bone());
	bone->SetName(name);
	bone->SetTranslate(translate);
	bone->SetRotate(rotate);
	bone->SetScale(scale);
	root_bone_ = bone;
	bones_.push_back(bone);
	return bone;
}




//-----------------------------------------------------------------------------------------------------------
Bone* Skeleton::AddBone(
	Bone* parent,
	const string_t& name,
	const math::Vector3f& translate,
	const math::Quaternionf& rotate,
	const math::Vector3f& scale
)
{
	for (uint_t i = 0; i < bones_.size(); ++i)
		if (bones_[i]->GetName() == name)
			throw Exception("Bone with given name already registered!");

	Bone* bone(new Bone());
	bone->SetName(name);
	bone->SetTranslate(translate);
	bone->SetRotate(rotate);
	bone->SetScale(scale);
	bones_.push_back(bone);

	Bone* prev = parent->GetFirstChildBone();
	if (prev != NULL)
	{
		Bone* next = prev->GetNextBone();
		while (next != NULL)
		{
			prev = next;
			next = prev->GetNextBone();
		}
		prev->SetNextBone(bone);
	}
	else
	{
		parent->SetFirstChildBone(bone);
	}

	return bone;
}




//-----------------------------------------------------------------------------------------------------------
const uint_t Skeleton::GetBoneCount() const
{
	return (uint_t)bones_.size();
}




//-----------------------------------------------------------------------------------------------------------
Bone* Skeleton::GetBone(
	const string_t& bone_name
)
{
	for (uint_t i = 0;  i < bones_.size(); ++i)
	{
		if (bones_[i]->GetName() == bone_name)
			return bones_[i];
	}
	return NULL;
}




//-----------------------------------------------------------------------------------------------------------
Bone* Skeleton::GetBone(
	const uint_t index
)
{
	return bones_[index];
}




//-----------------------------------------------------------------------------------------------------------
const uint_t Skeleton::GetBoneIndex(
	const string_t& bone_name
) const
{
	for (uint_t i = 0; i < bones_.size(); ++i)
		if (bones_[i]->GetName() == bone_name)
			return i;
	throw Exception(string_t("Bone with name'") + bone_name + "' can't be found!");
}




//-----------------------------------------------------------------------------------------------------------
Bone* Skeleton::GetRootBone()
{
	return root_bone_;
}




//-----------------------------------------------------------------------------------------------------------
const Bone* Skeleton::GetRootBone() const
{
	return root_bone_;
}




//-----------------------------------------------------------------------------------------------------------
void Skeleton::BuildMatrixArray()
{
	// if not already done create matrix array
	if (matrices_ == NULL)
		matrices_ = new math::Matrix4x4f[bones_.size()];
	if (inv_transpose_matrices_ == NULL)
		inv_transpose_matrices_ = new math::Matrix4x4f[bones_.size()];

	for (uint_t i = 0; i < bones_.size(); ++i)
	{
		// compute world matrix
		matrices_[i] = bones_[i]->GetCombinedTransformationMatrix() * bones_[i]->GetToBoneSpaceMatrix();
		// compute inv-transpose-world-matrix
		inv_transpose_matrices_[i] = matrices_[i].GetInverse();
		inv_transpose_matrices_[i].Transpose();
	}
}




//-----------------------------------------------------------------------------------------------------------
const math::Matrix4x4f& Skeleton::GetMatrix(
	const uint_t index
) const
{
	if (index > bones_.size())
		throw Exception("index out of bounds!");

	return matrices_[index];
}




//-----------------------------------------------------------------------------------------------------------
const math::Matrix4x4f& Skeleton::GetInvTransposeMatrix(
	const uint_t index
) const
{
	if (index > bones_.size())
		throw Exception("index out of bounds!");

	return inv_transpose_matrices_[index];
}

}} // namespace x::model

