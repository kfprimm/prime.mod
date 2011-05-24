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
/*!	\file model/skeleton.h
	\brief Definition von x::model::Skeleton
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::model::Skeleton
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef MODEL_SKELETON_H
#define MODEL_SKELETON_H

#include "../../libx_core/global/global.h"
#include "./bone.h"

namespace x { namespace model {

//-------------------------------------------------------------------------------------------------------
/*!
*/
class Skeleton
{
public:
	Skeleton();
	virtual ~Skeleton();

	// creation 
	virtual Bone* CreateRootBone(const string_t& name, const math::Vector3f& translate, const math::Quaternionf& rotate, const math::Vector3f& scale);
	virtual Bone* AddBone(Bone* parent, const string_t& name, const math::Vector3f& translate, const math::Quaternionf& rotate, const math::Vector3f& scale);

	// get-methods
	virtual const uint_t GetBoneCount() const;
	virtual Bone* GetBone(const string_t& bone_name);
	virtual Bone* GetBone(const uint_t index);
	virtual const uint_t GetBoneIndex(const string_t& bone_name) const;
	virtual Bone* GetRootBone();
	virtual const Bone* GetRootBone() const;

	// matrix array (built from all the bones)
	virtual void BuildMatrixArray();
	virtual const math::Matrix4x4f& GetMatrix(const uint_t index) const;
	virtual const math::Matrix4x4f& GetInvTransposeMatrix(const uint_t index) const;

protected:
	Bone* root_bone_;
	vector<Bone*> bones_;
	math::Matrix4x4f* matrices_;
	math::Matrix4x4f* inv_transpose_matrices_;
};

}} // namespace x::model

#endif // MODEL_SKELETON_H
