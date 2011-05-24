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
/*!	\file xmeshgenerator.h
	\brief Definition von x::XMeshGenerator
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::XMeshGenerator
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef XMESHGENERATOR_H
#define XMESHGENERATOR_H

#include "../libx_core/global/global.h"
#include "../libx_core/interface.h"
#include "./model/model.h"

namespace x {

//-----------------------------------------------------------------------------------------------------------
/*!
	\brief .x-Mesh-Generator-Klasse
	\version 0.1
	\date 2.4.2004
	\author Christian Oberholzer

	Der Generator kann aus einer Template-Hierarchie wie sie in einem XTemporalInfo-Objekt
	gespeichert ist einen kaos::Mesh generieren.
*/
class XMeshGenerator
{
public:
	//-------------------------------------------------------------------------------------------------------
	// Konstruktur & Destruktor
	XMeshGenerator();
	~XMeshGenerator();

	//-------------------------------------------------------------------------------------------------------
	static void Generate(model::Model* mesh, x::Interface::instances_t& instances);

protected:
	// internal methods to control mesh loading
	void GenerateInternal(model::Model* mesh, x::Interface::instances_t& instances);

	// loading base-mesh
	void ApplyFrameTemplate(x::templates::Frame* x_frame, model::Frame* parent_frame);
	void ApplyFrameTransformMatrix(x::templates::FrameTransformMatrix* x_transform, model::Frame* parent_frame);
	void ApplyMeshTemplate(x::templates::Mesh* x_mesh, model::Frame* parent_frame);
	void ApplyMeshNormalsTemplate(x::templates::MeshNormals* x_normals, model::Entity* entity);
	void ApplyMeshMaterialListTemplate(x::templates::MeshMaterialList* x_materiallist, model::Frame* parent_frame, model::Entity* entity);
	void ApplyMaterialTemplate(x::templates::Material* x_material, model::Entity* entity);
	void ApplyTextureFilenameTemplate(x::templates::TextureFilename* x_texture_filename, model::Material* material);
	void ApplyMeshTextureCoords(x::templates::MeshTextureCoords* x_texture_coords, model::Entity* entity, const uint_t texture_unit);
	void CreateSkinningStreams(model::Entity* entity, x::vector<x::templates::SkinWeights*>& skinweights);

	// skeleton loading
	void LoadSkeleton(model::Model* mesh, x::Interface::instances_t& instances, vector<string_t>& referenced_frames);
	void LoadBone(x::templates::Frame* x_frame, model::Skeleton* skeleton, model::Bone* parent_bone);
	void ApplyBoneTransformMatrix(x::templates::FrameTransformMatrix* x_transform, model::Bone* bone);

	// animation
	void CollectFramesReferencedFromAnimation(x::templates::AnimationSet* x_set, vector<string_t>& referenced_frames);
	void CollectFramesReferencedFromAnimation(x::templates::Animation* x_animation, vector<string_t>& referenced_frames);
	void ApplyAnimationSetTemplate(x::templates::AnimationSet* x_set, model::Skeleton* skeleton, model::Animation* animation);
	void GetMinDiffAndLastFrameTime(x::templates::Animation* x_animation, dword_t& min_diff, dword_t& last_frame_time);
	void GetMinDiffAndLastFrameTime(x::templates::AnimationKey* x_animation_key, dword_t& min_diff, dword_t& last_frame_time);
	void ApplyAnimationTemplate(x::templates::Animation* x_animation, model::Skeleton* skeleton, model::Animation* animation, const dword_t frame_diff, const dword_t last_frame_time);
	void ApplyAnimationKeyTemplate(x::templates::AnimationKey* x_animation_key, const string_t& referenced_bone, model::Skeleton* skeleton, model::Animation* animation, const dword_t frame_diff, const dword_t last_frame_time);
	void ApplyMatrix4x4AnimationKeyTemplate(x::templates::AnimationKey* x_animation_key, const string_t& referenced_bone, model::Skeleton* skeleton, model::Animation* animation, const dword_t frame_diff, const dword_t last_frame_time);

	// some useful methods for manipulation
	ubyte_t* Triangulate(const ubyte_t* data, uint_t& face_count);
	void EliminateMultipleReferences(model::Entity* entity);
	void ReorderVectors(math::Vector3f*& vectors, uint_t& vector_count, const udword_t* target_indices, const uint_t target_index_count, udword_t* vector_indices, const uint_t vector_index_count);
	bool_t SplitEntityByFaceIndices(model::Entity* new_entity, model::Entity* old_entity, const uint_t face_index_count, const udword_t* face_indices, const udword_t material_index);
	void DeleteEmptyChildren(model::Frame* frame);
	void TryIndexConversion(model::Frame* frame);
	void TryIndexConversion(model::Entity* entity);
};

} // namespace x

#endif // XMESHGENERATOR_H
