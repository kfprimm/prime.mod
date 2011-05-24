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
/*!	\file model/bone.h
	\brief Definition von x::model::Bone
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::model::Bone
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef MODEL_BONE_H
#define MODEL_BONE_H

#include "../../libx_core/global/global.h"
#include "./keyframeanimation/animatedvector3.h"
#include "./keyframeanimation/animatedquaternion.h"

namespace x { namespace model {

//-------------------------------------------------------------------------------------------------------
/*!
*/
class Bone : public AnimatedQuaternion, public AnimatedVector3
{
public:
	Bone();
	virtual ~Bone();

	// set values
	virtual void SetName(const string_t& name);
	virtual void SetAnimatedVector3(const math::Vector3f& v);
	virtual void SetTranslate(const math::Vector3f& translate);
	virtual void SetAnimatedQuaternion(const math::Quaternionf& v);
	virtual void SetRotate(const math::Quaternionf& rotate);
	virtual void SetScale(const math::Vector3f& scale);
	virtual void SetToBoneSpaceMatrix(const math::Matrix4x4f& matrix);
	virtual void SetNextBone(Bone* next);
	virtual void SetFirstChildBone(Bone* child);

	// recursive update
	virtual void UpdateCombinedTransformationMatrix(Bone* parent);

	// get methods
	virtual const string_t& GetName() const;
	virtual const math::Vector3f& GetTranslate() const;
	virtual const math::Quaternionf& GetRotate() const;
	virtual const math::Vector3f& GetScale() const;
	virtual const math::Matrix4x4f& GetCombinedTransformationMatrix() const;
	virtual const math::Matrix4x4f& GetToBoneSpaceMatrix() const;
	virtual Bone* GetNextBone();
	virtual const Bone* GetNextBone() const;
	virtual Bone* GetFirstChildBone();
	virtual const Bone* GetFirstChildBone() const;

protected:
	string_t name_;
	math::Vector3f translate_;
	math::Quaternionf rotate_;
	math::Vector3f scale_;
	math::Matrix4x4f combined_transformation_matrix_;
	math::Matrix4x4f to_bone_space_matrix_;
	Bone* next_;
	Bone* first_child_;
};

}} // namespace x::model

#endif // MODEL_BONE_H
