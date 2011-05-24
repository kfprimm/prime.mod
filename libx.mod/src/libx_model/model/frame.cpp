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
/*!	\file model/frame.cpp
	\brief Implementation von x::model::Frame
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::model::Frame
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
Frame::Frame()
:	model_(NULL),
	parent_frame_(NULL),
	name_(),
	translate_(math::Vector3f::ZERO()),
	rotate_(math::Quaternionf::IDENTITY()),
	scale_(math::Vector3f(1.0f, 1.0f, 1.0f)),
	frames_(),
	entities_()
{
}





//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
Frame::~Frame()
{
	while (frames_.size() > 0)
	{
		Frame* frame = frames_.back();
		frames_.pop_back();
		delete frame;
	}
	while (entities_.size() > 0)
	{
		Entity* entity = entities_.back();
		entities_.pop_back();
		delete entity;
	}
}




//-----------------------------------------------------------------------------------------------------------
void Frame::SetModel(
	Model* model
)
{
	model_ = model;
}




//-----------------------------------------------------------------------------------------------------------
Model* Frame::GetModel()
{
	return model_;
}




//-----------------------------------------------------------------------------------------------------------
const Model* Frame::GetModel() const
{
	return model_;
}




//-----------------------------------------------------------------------------------------------------------
void Frame::SetParentFrame(Frame* frame)
{
	parent_frame_ = frame;
}




//-----------------------------------------------------------------------------------------------------------
Frame* Frame::GetParentFrame()
{
	return parent_frame_;
}




//-----------------------------------------------------------------------------------------------------------
const Frame* Frame::GetParentFrame() const
{
	return parent_frame_;
}




//-----------------------------------------------------------------------------------------------------------
void Frame::SetName(
	const string_t& name
)
{
	name_ = name;
}




//-----------------------------------------------------------------------------------------------------------
const string_t& Frame::GetName() const
{
	return name_;
}




//-----------------------------------------------------------------------------------------------------------
void Frame::ToIdentityTransform()
{
	if (!IsIdentityTransform())
	{
		rotate_ = math::Quaternionf::IDENTITY();
		translate_ = math::Vector3f::ZERO();
		InvalidateLocalTransformation();
	}
}




//-----------------------------------------------------------------------------------------------------------
const bool_t Frame::IsIdentityTransform() const
{
	return (rotate_.IsIdentity() && translate_ == math::Vector3f(0.0f, 0.0f, 0.0f));
}




//-----------------------------------------------------------------------------------------------------------
void Frame::SetTranslate(
	const math::Vector3f& translate
)
{
	if (translate_ != translate)
	{
		translate_ = translate;
		InvalidateLocalTransformation();
	}
}




//-----------------------------------------------------------------------------------------------------------
void Frame::SetAnimatedVector3(
	const math::Vector3f& v
)
{
	SetTranslate(v);
}




//-----------------------------------------------------------------------------------------------------------
const math::Vector3f& Frame::GetTranslate() const
{
	return translate_;
}




//-----------------------------------------------------------------------------------------------------------
void Frame::SetRotate(
	const math::Quaternionf& rotate
)
{
	if (rotate_ != rotate)
	{
		rotate_ = rotate;
		InvalidateLocalTransformation();
	}
}




//-----------------------------------------------------------------------------------------------------------
void Frame::SetAnimatedQuaternion(
	const math::Quaternionf& v
)
{
	SetRotate(v);
}




//-----------------------------------------------------------------------------------------------------------
const math::Quaternionf& Frame::GetRotate() const
{
	return rotate_;
}




//-----------------------------------------------------------------------------------------------------------
void Frame::SetScale(
	const math::Vector3f& scale
)
{
	if (scale_ != scale)
	{
		scale_ = scale;
		InvalidateLocalTransformation();
	}
}




//-----------------------------------------------------------------------------------------------------------
const math::Vector3f& Frame::GetScale() const
{
	return scale_;
}




//-----------------------------------------------------------------------------------------------------------
void Frame::InvalidateLocalTransformation()
{
	local_transformation_dirty_ = true;
	for (uint_t i = 0; i < frames_.size(); ++i)
	{
		frames_[i]->InvalidateLocalTransformation();
	}
}




//-----------------------------------------------------------------------------------------------------------
const math::Matrix4x4f& Frame::GetLocalTransformation() const
{
	if (local_transformation_dirty_)
	{
		math::Matrix3x3f scale;
		scale.ToScale(scale_);

		math::Matrix3x3f rotation;
		GetRotate().ToRotationMatrix(rotation);
		//rotation.Transpose(); // dieses transpose sollte nicht nötig sein!

		math::Matrix4x4f translate;
		translate.ToTranslate(GetTranslate());
		math::Matrix4x4f transform = translate * rotation * scale;

		if (parent_frame_ != NULL)
		{
			local_transformation_ = parent_frame_->GetLocalTransformation() * transform;
		}
		else
		{
			local_transformation_ = transform;
		}
		local_transformation_dirty_ = false;
	}

	return local_transformation_;
}




//-----------------------------------------------------------------------------------------------------------
void Frame::AddFrame(
	Frame* frame
)
{
	frame->SetParentFrame(this);
	frames_.push_back(frame);
}




//-----------------------------------------------------------------------------------------------------------
const uint_t Frame::GetFrameCount() const
{
	return (uint_t)frames_.size();
}




//-----------------------------------------------------------------------------------------------------------
Frame* Frame::GetFrame(
	const string_t& name
)
{
	for (uint_t i = 0; i < frames_.size(); ++i)
	{
		if (frames_[i]->GetName() == name)
			return frames_[i];
	}
	return NULL;
}




//-----------------------------------------------------------------------------------------------------------
const Frame* Frame::GetFrame(
	const string_t& name
) const
{
	for (uint_t i = 0; i < frames_.size(); ++i)
	{
		if (frames_[i]->GetName() == name)
			return frames_[i];
	}
	return NULL;
}




//-----------------------------------------------------------------------------------------------------------
Frame* Frame::GetFrame(
	const uint_t index
)
{
	return frames_[index];
}




//-----------------------------------------------------------------------------------------------------------
const Frame* Frame::GetFrame(
	const uint_t index
) const
{
	return frames_[index];
}




//-----------------------------------------------------------------------------------------------------------
void Frame::RemoveFrame(
	const string_t& name
)
{
	frames_t::iterator begin = frames_.begin();
	frames_t::iterator end = frames_.end();
	for (; begin != end; ++begin)
	{
		if ((*begin)->GetName() == name)
		{
			frames_.erase(begin);
			return;
		}
	}
}




//-----------------------------------------------------------------------------------------------------------
void Frame::RemoveFrame(
	Frame* frame
)
{
	RemoveFrame(frame->GetName());
}




//-----------------------------------------------------------------------------------------------------------
void Frame::AddEntity(
	Entity* entity
)
{
	entity->SetParentFrame(this);
	entities_.push_back(entity);
}




//-----------------------------------------------------------------------------------------------------------
const uint_t Frame::GetEntityCount() const
{
	return (uint_t)entities_.size();
}




//-----------------------------------------------------------------------------------------------------------
Entity* Frame::GetEntity(
	const string_t& name
)
{
	for (uint_t i = 0; i < entities_.size(); ++i)
	{
		if (entities_[i]->GetName() == name)
			return entities_[i];
	}
	return NULL;
}




//-----------------------------------------------------------------------------------------------------------
const Entity* Frame::GetEntity(
	const string_t& name
) const
{
	for (uint_t i = 0; i < entities_.size(); ++i)
	{
		if (entities_[i]->GetName() == name)
			return entities_[i];
	}
	return NULL;
}




//-----------------------------------------------------------------------------------------------------------
Entity* Frame::GetEntity(
	const uint_t index
)
{
	return entities_[index];
}




//-----------------------------------------------------------------------------------------------------------
const Entity* Frame::GetEntity(
	const uint_t index
) const
{
	return entities_[index];
}




//-----------------------------------------------------------------------------------------------------------
void Frame::RemoveEntity(
	const string_t& name
)
{
	entities_t::iterator begin = entities_.begin();
	entities_t::iterator end = entities_.end();
	for (; begin != end; ++begin)
	{
		if ((*begin)->GetName() == name)
		{
			entities_.erase(begin);
			return;
		}
	}
}




//-----------------------------------------------------------------------------------------------------------
void Frame::RemoveEntity(
	Entity* entity
)
{
	RemoveEntity(entity->GetName());
}




//-----------------------------------------------------------------------------------------------------------
void Frame::ComputeNormalVectors()
{
	for (uint_t i = 0; i < frames_.size(); ++i)
	{
		frames_[i]->ComputeNormalVectors();
	}
	for (uint_t i = 0; i < entities_.size(); ++i)
	{
		entities_[i]->ComputeNormalVectors();
	}
}




//-----------------------------------------------------------------------------------------------------------
void Frame::ComputeTangentVectors()
{
	for (uint_t i = 0; i < frames_.size(); ++i)
	{
		frames_[i]->ComputeTangentVectors();
	}
	for (uint_t i = 0; i < entities_.size(); ++i)
	{
		entities_[i]->ComputeTangentVectors();
	}
}




//-----------------------------------------------------------------------------------------------------------
void Frame::CreateBindPose()
{
	for (uint_t i = 0; i < frames_.size(); ++i)
	{
		frames_[i]->CreateBindPose();
	}
	for (uint_t i = 0; i < entities_.size(); ++i)
	{
		entities_[i]->CreateBindPose();
	}
}




//-----------------------------------------------------------------------------------------------------------
void Frame::UpdateSkinnedModel(
	Skeleton* skeleton
)
{
	for (uint_t i = 0; i < frames_.size(); ++i)
	{
		frames_[i]->UpdateSkinnedModel(skeleton);
	}
	for (uint_t i = 0; i < entities_.size(); ++i)
	{
		entities_[i]->UpdateSkinnedModel(skeleton);
	}
}

}} // namespace x::model

