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
/*!	\file model/model.cpp
	\brief Implementation von x::model::Model
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::model::Model
*/
//-----------------------------------------------------------------------------------------------------------
#include "./model.h"

namespace x { namespace model {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
Model::Model()
:	identifier_(),
	root_(NULL),
	geometry_(NULL),
	materials_(),
	animation_groups_(),
	skeleton_(NULL)
{
	geometry_ = new Geometry();
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
Model::~Model()
{
	delete root_;
	delete geometry_;

	while (!animation_groups_.empty())
	{
		AnimationGroup* group = animation_groups_.back();
		animation_groups_.pop_back();
		delete group;
	}

	delete skeleton_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Das übergebene Model kopieren
	\param rhs
		Das zu kopierende Model
	\param new_identifier
		Neuer Identifier

	Kopiert das übergebene Model in diese Meshstruktur und ordnet diesem Model den neuen übergebenen 
	Identifier zu.
*/
void Model::Copy(
	Model* rhs, 
	const string_t& new_identifier
)
{
	throw NotSupportedException(UNICODE_STRING("todo"));
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Diesem Model seinen Identifier zuordnen
	\param identifier

	Der neue Identifier muss unter allen FVFMeshes eindeutig sein!
*/
void Model::SetIdentifier(
	const string_t& identifier
)
{
	identifier_ = identifier;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Identifier dieses Models zurückgeben
	\return Identifier

	Gibt den Identifier dieses Models zurück.
*/
const string_t& Model::GetIdentifier()
{
	return identifier_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Root-Frame zurückgeben
	\return Pointer auf das Frame

	Gibt einen Pointer auf das Root-Frame zurück. D.h. das Frame mit der Identity-Transformation
	welches die gesamte Frame-Hierachie enthält.
*/
Frame* Model::GetRootFrame()
{
	if (root_ == NULL)
	{
		root_ = new Frame;
		root_->SetModel(this);
		root_->SetParentFrame(NULL);
		root_->SetName(UNICODE_STRING("root"));
		root_->SetRotate(math::Quaternionf::IDENTITY());
	}

	return root_;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Root-Frame zurückgeben
	\return Pointer auf das Frame

	Gibt einen Pointer auf das Root-Frame zurück. D.h. das Frame mit der Identity-Transformation
	welches die gesamte Frame-Hierachie enthält.
*/
const Frame* Model::GetRootFrame() const
{
	return root_;
}




//-----------------------------------------------------------------------------------------------------------
Geometry* Model::GetGeometry()
{
	return geometry_;
}




//-----------------------------------------------------------------------------------------------------------
const Geometry* Model::GetGeometry() const
{
	return geometry_;
}




//-----------------------------------------------------------------------------------------------------------
const uint_t Model::GetMaterialCount() const
{
	return (uint_t)materials_.size();
}




//-----------------------------------------------------------------------------------------------------------
Material* Model::GetMaterial(
	const uint_t index
)
{
	return materials_[index];
}




//-----------------------------------------------------------------------------------------------------------
const Material* Model::GetMaterial(
	const uint_t index
) const
{
	return materials_[index];
}




//-----------------------------------------------------------------------------------------------------------
void Model::ComputeNormalVectors()
{
	root_->ComputeNormalVectors();

	// bei allen Animationen die Normal-Animationtracks erstellen
	for (uint_t i = 0; i < animation_groups_.size(); ++i)
	{
		for (uint_t j = 0; j < animation_groups_[i]->GetAnimationCount(); ++j)
		{
			Animation* animation = animation_groups_[i]->GetAnimation(j);
			animation->CreateNormalsAnimationTrack();
		}
	}
}




//-----------------------------------------------------------------------------------------------------------
void Model::ComputeTangentVectors()
{
	root_->ComputeTangentVectors();

	// bei allen Animationen die Tangent-Animationtracks erstellen
	for (uint_t i = 0; i < animation_groups_.size(); ++i)
	{
		for (uint_t j = 0; j < animation_groups_[i]->GetAnimationCount(); ++j)
		{
			Animation* animation = animation_groups_[i]->GetAnimation(j);
			animation->CreateTangentsAnimationTrack();
		}
	}
}




//-----------------------------------------------------------------------------------------------------------
void Model::CreateBindPose()
{
	root_->CreateBindPose();
}




//-----------------------------------------------------------------------------------------------------------
void Model::UpdateSkinnedModel()
{
	skeleton_->BuildMatrixArray();
	root_->UpdateSkinnedModel(skeleton_);
}




//-----------------------------------------------------------------------------------------------------------
void Model::AddAnimationGroup(
	AnimationGroup* group
)
{
	animation_groups_.push_back(group);
}




//-----------------------------------------------------------------------------------------------------------
const uint_t Model::GetAnimationGroupCount() const
{
	return (uint_t)animation_groups_.size();
}




//-----------------------------------------------------------------------------------------------------------
AnimationGroup* Model::GetAnimationGroup(
	const uint_t index
)
{
	return animation_groups_[index];
}




//-----------------------------------------------------------------------------------------------------------
const AnimationGroup* Model::GetAnimationGroup(
	const uint_t index
) const
{
	return const_cast<Model*>(this)->GetAnimationGroup(index);
}




//-----------------------------------------------------------------------------------------------------------
AnimationGroup* Model::GetAnimationGroup(
	const string_t& name
)
{
	typedef vector<AnimationGroup*>::iterator iterator;
	for (iterator begin = animation_groups_.begin(); begin != animation_groups_.end(); ++begin)
	{
		if ((*begin)->GetName() == name)
		{
			return (*begin);
		}
	}
	return NULL;
}




//-----------------------------------------------------------------------------------------------------------
const AnimationGroup* Model::GetAnimationGroup(
	const string_t& name
) const
{
	return const_cast<Model*>(this)->GetAnimationGroup(name);
}




//-----------------------------------------------------------------------------------------------------------
void Model::RemoveAnimationGroup(
	const uint_t index
)
{
	animation_groups_.erase(animation_groups_.begin()+index);
}




//-----------------------------------------------------------------------------------------------------------
void Model::SetSkeleton(
	Skeleton* skeleton
)
{
	skeleton_ = skeleton;
}




//-----------------------------------------------------------------------------------------------------------
Skeleton* Model::GetSkeleton()
{
	return skeleton_;
}




//-----------------------------------------------------------------------------------------------------------
const Skeleton* Model::GetSkeleton() const
{
	return skeleton_;
}




//-----------------------------------------------------------------------------------------------------------
Frame* Model::CreateFrame() 
{
	Frame* frame = new Frame();
	frame->SetModel(this);
	frame->SetName(UNICODE_STRING("unnamed"));
	return frame;
}




//-----------------------------------------------------------------------------------------------------------
Frame* Model::CreateFrame(
	const string_t& name
) 
{
	Frame* frame = new Frame();
	frame->SetModel(this);
	frame->SetName(name);
	return frame;
}




//-----------------------------------------------------------------------------------------------------------
Entity* Model::CreateEntity()
{
	Entity* entity = new Entity();
	entity->SetModel(this);
	entity->SetName(UNICODE_STRING("unnamed"));
	return entity;
}




//-----------------------------------------------------------------------------------------------------------
Entity* Model::CreateEntity(
	const string_t& name
)
{
	Entity* entity = new Entity();
	entity->SetModel(this);
	entity->SetName(name);
	return entity;
}




//-----------------------------------------------------------------------------------------------------------
Geometry* Model::CreateGeometry()
{
	return new Geometry();
}




//-----------------------------------------------------------------------------------------------------------
Material* Model::CreateMaterial()
{
	Material* material = new Material();
	materials_.push_back(material);
	return material;
}




//-----------------------------------------------------------------------------------------------------------
AnimationGroup* Model::CreateAnimationGroup()
{
	return new AnimationGroup();
}




//-----------------------------------------------------------------------------------------------------------
Skeleton* Model::CreateSkeleton()
{
	return new Skeleton();
}

}} // namespace x::model

