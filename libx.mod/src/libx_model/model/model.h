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
/*!	\file model/model.h
	\brief Definition von x::model::Model
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::model::Model
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef MODEL_MODEL_H
#define MODEL_MODEL_H

#include "../../libx_core/global/global.h"
#include "./keyframeanimation/animationgroup.h"
#include "./frame.h"
#include "./entity.h"
#include "./geometry.h"
#include "./material.h"
#include "./skeleton.h"

namespace x { namespace model {

//-----------------------------------------------------------------------------------------------------------
class ModelManager;




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Model-Klasse
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	Model-Interface. Definiert das Interface über welches Kaos-Meshes implementiert werden. Je nach 
	der verwendeten Rendering-API und der verwendeten Rendering-Technik sind verschiedene Model-Strukturen
	mehr oder weniger optimal. Deshalb kann dieses Interface verschieden implementiert werden.
*/
class Model
{
public:
	//-------------------------------------------------------------------------------------------------------
	// Konstruktur & Destruktor
	Model();
	virtual ~Model();

	virtual void Copy(Model* rhs, const string_t& new_identifier);
	virtual void SetIdentifier(const string_t& identifier);
	virtual const string_t& GetIdentifier();
	virtual Frame* GetRootFrame();
	virtual const Frame* GetRootFrame() const;
	virtual Geometry* GetGeometry();
	virtual const Geometry* GetGeometry() const;
	virtual const uint_t GetMaterialCount() const;
	virtual Material* GetMaterial(const uint_t index);
	virtual const Material* GetMaterial(const uint_t index) const;

	// helper methods, propagate them down the model hierarchy until they are at the
	// elements they're needed for
	virtual void ComputeNormalVectors();
	virtual void ComputeTangentVectors();
	virtual void CreateBindPose();
	virtual void UpdateSkinnedModel();

	// animation groups
	virtual void AddAnimationGroup(AnimationGroup* group);
	virtual const uint_t GetAnimationGroupCount() const;
	virtual AnimationGroup* GetAnimationGroup(const uint_t index);
	virtual const AnimationGroup* GetAnimationGroup(const uint_t index) const;
	virtual AnimationGroup* GetAnimationGroup(const string_t& name);
	virtual const AnimationGroup* GetAnimationGroup(const string_t& name) const;
	virtual void RemoveAnimationGroup(const uint_t index);

	// skeleton
	virtual void SetSkeleton(Skeleton* skeleton);
	virtual Skeleton* GetSkeleton();
	virtual const Skeleton* GetSkeleton() const;

	// factor some needed parts
	virtual Frame* CreateFrame();
	virtual Frame* CreateFrame(const string_t& name);
	virtual Entity* CreateEntity();
	virtual Entity* CreateEntity(const string_t& name);
	virtual Geometry* CreateGeometry();
	virtual Material* CreateMaterial();
	virtual AnimationGroup* CreateAnimationGroup();
	virtual Skeleton* CreateSkeleton();

protected:
	/*!
		\var identifier_
		\brief Identifier dieses Meshes

		Speichert den Identifier dieses Meshes
	*/

	string_t identifier_;

	/*!
		\var root_
		\brief Pointer auf das Root-Frame

		Zeigt auf das Root-Frame dieses Meshes.
	*//*!
		\var geometry_
		\brief Geometrie

		Die Shared-Geometriedaten dieses Meshes
	*/

	Frame* root_;
	Geometry* geometry_;
	vector<Material*> materials_;
	vector<AnimationGroup*> animation_groups_;

	/*!
		\var skeleton_
		\brief this models skeleton
	*/
	Skeleton* skeleton_;
};

}} // namespace x::model

#endif // MODEL_MODEL_H
