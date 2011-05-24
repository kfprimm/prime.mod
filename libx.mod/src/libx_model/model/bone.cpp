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
/*!	\file model/bone.cpp
	\brief Implementation von x::model::Bone
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::model::Bone
*/
//-----------------------------------------------------------------------------------------------------------
#include "./bone.h"

namespace x { namespace model {

//-----------------------------------------------------------------------------------------------------------
Bone::Bone()
:	name_(),
	translate_(math::Vector3f::ZERO()),
	rotate_(math::Quaternionf::IDENTITY()),
	scale_(math::Vector3f(1.0f, 1.0f, 1.0f)),
	next_(NULL),
	first_child_(NULL)
{
	combined_transformation_matrix_.ToIdentity();
	to_bone_space_matrix_.ToIdentity();
}




//-----------------------------------------------------------------------------------------------------------
Bone::~Bone()
{
}




//-----------------------------------------------------------------------------------------------------------
void Bone::SetName(
	const string_t& name
)
{
	name_ = name;
}




//-----------------------------------------------------------------------------------------------------------
void Bone::SetAnimatedVector3(
	const math::Vector3f& v
)
{
	translate_ = v;
}




//-----------------------------------------------------------------------------------------------------------
void Bone::SetTranslate(
	const math::Vector3f& translate
)
{
	translate_ = translate;
}




//-----------------------------------------------------------------------------------------------------------
void Bone::SetAnimatedQuaternion(
	const math::Quaternionf& v
)
{
	rotate_ = v;
}




//-----------------------------------------------------------------------------------------------------------
void Bone::SetRotate(
	const math::Quaternionf& rotate
)
{
	rotate_ = rotate;
}




//-----------------------------------------------------------------------------------------------------------
void Bone::SetScale(
	const math::Vector3f& scale
)
{
	scale_ = scale;
}




//-----------------------------------------------------------------------------------------------------------
void Bone::SetToBoneSpaceMatrix(
	const math::Matrix4x4f& matrix
)
{
	to_bone_space_matrix_ = matrix;
}




//-----------------------------------------------------------------------------------------------------------
void Bone::SetNextBone(
	Bone* next
)
{
	next_ = next;
}




//-----------------------------------------------------------------------------------------------------------
void Bone::SetFirstChildBone(
	Bone* child
)
{
	first_child_ = child;
}




//-----------------------------------------------------------------------------------------------------------
void Bone::UpdateCombinedTransformationMatrix(
	Bone* parent
)
{
	// calculate this bones transformation matrix
	math::Matrix3x3f scale;
	scale.ToScale(scale_);
	
	math::Matrix3x3f rotation;
	rotate_.ToRotationMatrix(rotation);

	math::Matrix4x4f translate;
	translate.ToTranslate(translate_);

	math::Matrix4x4f transformation_matrix = translate * rotation * scale;
	if (parent != NULL)
	{
		combined_transformation_matrix_ = parent->GetCombinedTransformationMatrix() * transformation_matrix;
	}
	else
	{
		combined_transformation_matrix_ = transformation_matrix;
	}

	if (next_ != NULL)
		next_->UpdateCombinedTransformationMatrix(parent);
	if (first_child_ != NULL)
		first_child_->UpdateCombinedTransformationMatrix(this);
}




//-----------------------------------------------------------------------------------------------------------
const string_t& Bone::GetName() const
{
	return name_;
}




//-----------------------------------------------------------------------------------------------------------
const math::Vector3f& Bone::GetTranslate() const
{
	return translate_;
}




//-----------------------------------------------------------------------------------------------------------
const math::Quaternionf& Bone::GetRotate() const
{
	return rotate_;
}




//-----------------------------------------------------------------------------------------------------------
const math::Vector3f& Bone::GetScale() const
{
	return scale_;
}




//-----------------------------------------------------------------------------------------------------------
const math::Matrix4x4f& Bone::GetCombinedTransformationMatrix() const
{
	return combined_transformation_matrix_;
}




//-----------------------------------------------------------------------------------------------------------
const math::Matrix4x4f& Bone::GetToBoneSpaceMatrix() const
{
	return to_bone_space_matrix_;
}




//-----------------------------------------------------------------------------------------------------------
Bone* Bone::GetNextBone()
{
	return next_;
}




//-----------------------------------------------------------------------------------------------------------
const Bone* Bone::GetNextBone() const
{
	return next_;
}




//-----------------------------------------------------------------------------------------------------------
Bone* Bone::GetFirstChildBone()
{
	return first_child_;
}




//-----------------------------------------------------------------------------------------------------------
const Bone* Bone::GetFirstChildBone() const
{
	return first_child_;
}

}} // namespace x::model

