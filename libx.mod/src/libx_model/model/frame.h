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
/*!	\file model/frame.h
	\brief Definition von x::model::Frame
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::model::Frame
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef MODEL_FRAME_H
#define MODEL_FRAME_H

#include "../../libx_core/global/global.h"
#include "./keyframeanimation/animatedvector3.h"
#include "./keyframeanimation/animatedquaternion.h"
#include "./entity.h"
#include "./skeleton.h"

namespace x { namespace model {

//-----------------------------------------------------------------------------------------------------------
class Model;




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Model-Klasse
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	Es besteht die Problematik, dass einzelne Elemente eines Models in einer Hierachie 
	aufgebaut sein können. Dabei bilden Frames den Rahmen und enthalten jeweils 
	Transformationsmatrizen welche für die untergeorndeten Entities gelten. Eine Entity 
	benutzt die aufsummierte Transformationsmatrix aller übergeordneten Frames.
*/
class Frame : public AnimatedQuaternion, public AnimatedVector3
{
protected:
	typedef vector<Frame*> frames_t;
	typedef vector<Entity*> entities_t;

public:
	//-------------------------------------------------------------------------------------------------------
	// Konstruktur & Destruktor
	Frame();
	virtual ~Frame();

	virtual void SetModel(Model* model);
	virtual Model* GetModel();
	virtual const Model* GetModel() const;

	// parent frame, name and transform
	virtual void SetParentFrame(Frame* frame);
	virtual Frame* GetParentFrame();
	virtual const Frame* GetParentFrame() const;
	virtual void SetName(const string_t& name);
	virtual const string_t& GetName() const;

	virtual void ToIdentityTransform();
	virtual const bool_t IsIdentityTransform() const;

	virtual void SetTranslate(const math::Vector3f& translate);
	virtual void SetAnimatedVector3(const math::Vector3f& v);
	virtual const math::Vector3f& GetTranslate() const;
	
	virtual void SetRotate(const math::Quaternionf& rotate);
	virtual void SetAnimatedQuaternion(const math::Quaternionf& v);
	virtual const math::Quaternionf& GetRotate() const;
	
	virtual void SetScale(const math::Vector3f& scale);
	virtual const math::Vector3f& GetScale() const;

	virtual void InvalidateLocalTransformation();
	virtual const math::Matrix4x4f& GetLocalTransformation() const;

	// children
	virtual void AddFrame(Frame* frame);
	virtual const uint_t GetFrameCount() const;
	virtual Frame* GetFrame(const string_t& name);
	virtual const Frame* GetFrame(const string_t& name) const;
	virtual Frame* GetFrame(const uint_t index);
	virtual const Frame* GetFrame(const uint_t index) const;
	virtual void RemoveFrame(const string_t& name);
	virtual void RemoveFrame(Frame* frame);

	virtual void AddEntity(Entity* entity);
	virtual const uint_t GetEntityCount() const;
	virtual Entity* GetEntity(const string_t& name);
	virtual const Entity* GetEntity(const string_t& name) const;
	virtual Entity* GetEntity(const uint_t index);
	virtual const Entity* GetEntity(const uint_t index) const;
	virtual void RemoveEntity(const string_t& name);
	virtual void RemoveEntity(Entity* entity);

	// helper methods, propagate them down the model hierarchy until they are at the
	// elements they're needed for
	virtual void ComputeNormalVectors();
	virtual void ComputeTangentVectors();
	virtual void CreateBindPose();
	virtual void UpdateSkinnedModel(Skeleton* skeleton);

protected:
	/*!
		\var model_
		\brief Pointer auf den Model zu welchem dieses Frame gehört

		Enthält einen Pointer auf den Model zu welchem dieses Frame gehört.
	*//*!
		\var parent_frame_
		\brief Pointer auf das übergeordnete Frame

		Enthält einen Pointer auf das übergeordnete Frame
	*/

	Model* model_;
	Frame* parent_frame_;

	/*!
		\var name_
		\brief Name dieses Frames

		Enthält den Namen dieses Frames.
	*//*!
		\var translate_
		\brief Translationsvektor dieses Frames

		Alle Children des Frames werden um den angegebenen Vektor
		verschoben.
	*//*!
		\var rotate_
		\brief Transformationsmatrix dieses Frames

		Enthält die Transformationsmatrix d4ieses Frames
	*/

	string_t name_;
	math::Vector3f translate_;
	math::Quaternionf rotate_;
	math::Vector3f scale_;
	mutable bool_t local_transformation_dirty_;
	mutable math::Matrix4x4f local_transformation_;

	/*!
		\var frames_
		\brief Untergeordnete Frames

		Enthält alle untergeordneten Frames.
	*//*!
		\var entities_
		\brief Diesem Frame zugeordnete Entities

		Enthält alle zu diesem Frame gehörenden Entities
	*/

	frames_t frames_;
	entities_t entities_;
};

}} // namespace x::model

#endif // MODEL_FRAME_H
