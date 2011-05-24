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
/*!	\file xmeshgenerator/xmeshgenerator_judihui.cpp
	\brief Implementation von x::XMeshGenerator
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Implementation von x::XMeshGenerator
*/
//-----------------------------------------------------------------------------------------------------------
#include <sstream>
#include "./xmeshgenerator.h"

extern "C" {
#include "decompose.h"
}

namespace x {

//-----------------------------------------------------------------------------------------------------------
void decompose_matrix(
	const math::Matrix4x4f& matrix,
	math::Vector3f& translation,
	math::Quaternionf& rotation,
	math::Vector3f& scale
)
{
	HMatrix in_matrix;
	memcpy(in_matrix, &matrix, sizeof(float)*16);
	AffineParts parts;
	decomp_affine(in_matrix, &parts);

	// copy output
	scale = math::Vector3f(parts.k.x*parts.f, parts.k.y*parts.f, parts.k.z*parts.f);
	rotation = math::Quaternionf(parts.q.w, parts.q.x, parts.q.y, parts.q.z);
	translation = math::Vector3f(parts.t.x, parts.t.y, parts.t.z);

	// negative rotation angles result in a bug inside the SLERP code. until we find this bug
	// we keep this workaround which changes the rotation orientation.
	if (rotation.w < 0.0f)
	{
		float_t acos = acosf(rotation.w);
		float_t angle = acos * 2.0f;
		angle = 2 * 3.1415926535897932384626433832795f - angle;
		rotation = math::Quaternionf(cosf(angle/2.0f), -rotation.x, -rotation.y, -rotation.z);

		// normalize again
		float_t fNorm = rotation.GetNorm();
		if (fNorm > 0.0f)
		{
			float_t fInvNorm = 1.0f / fNorm;
			rotation = math::Quaternionf(rotation.w * fInvNorm, rotation.x * fInvNorm, rotation.y * fInvNorm, rotation.z * fInvNorm);
		}
		else
		{
			rotation = math::Quaternionf(0, 0, 0, 0);
		}
	}
}




//-----------------------------------------------------------------------------------------------------------
math::Quaternionf slerp_no_invert(
	const math::Quaternionf& from,
	const math::Quaternionf& to,
	const float_t weight_frame0
)
{
	float_t pi = 3.1415926535897932384626433832795f;

	float_t fAngle;
	float_t fCos = from.Dot(to);
	if (fCos <= -1.0f)
		fAngle = pi;
	else if (fCos >= 1.0f)
		fAngle = 0.0f;
	else
		fAngle = acos(fCos);

	if (fabs(fAngle) >= 2.0f * pi / 360.0f)
	{
		float_t fSin = sinf(fAngle);
		float_t fInvSin = ((float_t)1.0)/fSin;
		float_t fCoeff0 = sinf(weight_frame0*fAngle)*fInvSin;
		float_t fCoeff1 = sinf((1.0f - weight_frame0)*fAngle)*fInvSin;
		return from*fCoeff0 + to*fCoeff1;
	}
	else
	{
		return from;
	}
}




//-----------------------------------------------------------------------------------------------------------
math::Vector3f interpolate(
	const math::Vector3f& from,
	const math::Vector3f& to,
	const float_t weight_frame0,
	const float_t weight_frame1
)
{
	math::Vector3f res;
	res = (from * weight_frame0) + (to * weight_frame1);
	return res;
}




//-----------------------------------------------------------------------------------------------------------
void interpolate_frames(
	const dword_t frame_time,
	const dword_t frame0_time, const math::Vector3f& translation0, const math::Quaternionf& rotation0, const math::Vector3f& scale0,
	const dword_t frame1_time, const math::Vector3f& translation1, const math::Quaternionf& rotation1, const math::Vector3f& scale1,
	math::Vector3f& translation, math::Quaternionf& rotation, math::Vector3f& scale
)
{
	if (frame0_time == frame1_time)
	{
		translation = translation0;
		rotation = rotation0;
		scale = scale0;
	}
	else
	{
		float_t time_diff = (float_t)frame1_time - (float_t)frame0_time;
		float_t weight_frame0 = 1.0f - ((float_t)frame_time / time_diff);
		float_t weight_frame1 = 1.0f - weight_frame0;

		translation = interpolate(translation0, translation1, weight_frame0, weight_frame1);
		rotation = slerp_no_invert(rotation0, rotation1, weight_frame0);
		scale = interpolate(scale0, scale1, weight_frame0, weight_frame1);
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Konstruktor

	Konstruktor
*/
XMeshGenerator::XMeshGenerator()
{
}





//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Destruktor

	Destruktor
*/
XMeshGenerator::~XMeshGenerator()
{
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Mesh generieren
	\param mesh
	\param instances

	Generiert aus der in Info unter dem Member instances gespeicherten x_template-Hierarchie einen 
	Mesh welcher in den ebenfalls als mesh gespeicherten Member abgelegt wird.
*/
void XMeshGenerator::Generate(
	model::Model* mesh, 
	x::Interface::instances_t& instances
)
{
	XMeshGenerator Generator; 
	Generator.GenerateInternal(mesh, instances);
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Mesh generieren (interne Methode)
	\param mesh
	\param instances

	Macht genau was XMeshGenerator::Generate() erledigen sollte, nur nicht static.
*/
void XMeshGenerator::GenerateInternal(
	model::Model* mesh, 
	x::Interface::instances_t& instances
)
{

	// build the skeleton first so we can use it later when parsing the vertex weights
	for (
		x::Interface::instances_t::iterator begin = instances.begin();
		begin != instances.end();
		++begin
	)
	{
		x::templates::Template* x_template = (*begin);
		switch (x_template->GetType())
		{
		case x::templates::TemplateTypeAnimationSet: {
			vector<string_t> referenced_frames;
			CollectFramesReferencedFromAnimation(dynamic_cast<x::templates::AnimationSet*>(x_template), referenced_frames);

			if (mesh->GetSkeleton() == NULL)
			{
				LoadSkeleton(mesh, instances, referenced_frames);
			}

			if (mesh->GetAnimationGroupCount() == 0)
			{
				model::AnimationGroup* animation_group = mesh->CreateAnimationGroup();
				animation_group->SetName("main_animations");
				mesh->AddAnimationGroup(animation_group);
			}

			model::AnimationGroup* animation_group = mesh->GetAnimationGroup(0);
			model::Animation* animation = animation_group->CreateAnimation();
			animation->SetName(dynamic_cast<x::templates::AnimationSet*>(x_template)->GetName());
			animation->SetFramesPerSecond(0);
			animation->SetFrameCount(0);
			animation_group->AddAnimation(animation);

			ApplyAnimationSetTemplate(dynamic_cast<x::templates::AnimationSet*>(x_template), mesh->GetSkeleton(), animation);
			break;
		}
		}
	}

	// Loop durch alle Elemente der obersten Ebene. Wir können hier spezielle Elemente haben
	// welche dann eine eigene Hierachie besitzen können. Beispiele: Frames, Transformationen, etc. 
	for (
		x::Interface::instances_t::iterator begin = instances.begin();
		begin != instances.end();
		++begin
	)
	{
		x::templates::Template* x_template = (*begin);
		switch (x_template->GetType())
		{
		case x::templates::TemplateTypeFrame: {
			model::Frame* frame = mesh->GetRootFrame();
			this->ApplyFrameTemplate(dynamic_cast<x::templates::Frame*>(x_template), frame);
			break;
		}
		case x::templates::TemplateTypeMesh: {
			model::Frame* frame = mesh->GetRootFrame();
			this->ApplyMeshTemplate(dynamic_cast<x::templates::Mesh*>(x_template), frame);
			break;
		}
		}
	}

	// If the model owns a skeleton we need to create a bind-pose. the bind-pose contains the vertex-data
	// in a space which can then be transformed to bone and again to world-space. the bind-pose is the
	// one which is actually loaded in the model. but since we will modify all the vertices when animating
	// the skeleton we need to clone all the data.
	mesh->CreateBindPose();

	// ok we've loaded the static model. if the model contains a skeleton then we might
	// have many unused frames, so eliminate them.
	DeleteEmptyChildren(mesh->GetRootFrame());
	
	// try to convert indices from 32bit to 16bit.
	TryIndexConversion(mesh->GetRootFrame());
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Frame-x_template anwenden
	\param x_frame
	\param parent_frame

	Wendet das übergebene Frame-XTemplate an und generiert daraus ein kaos::Frame.
3*/
void XMeshGenerator::ApplyFrameTemplate(
	x::templates::Frame* x_frame, 
	model::Frame* parent_frame
)
{
	// Neues Frame konstruieren und initialisieren
	model::Frame* frame = parent_frame->GetModel()->CreateFrame();
	const x::string_t& name = x_frame->GetName();
	if (name.length() > 0)
		frame->SetName(name);
	frame->SetRotate(math::Quaternionf::IDENTITY());
	frame->SetTranslate(math::Vector3f::ZERO());
	parent_frame->AddFrame(frame);

	const x::templates::Frame::children_container_t& children = x_frame->GetChildren();
	for (
		x::templates::Frame::children_container_t::const_iterator begin = children.begin();
		begin != children.end();
		++begin
	)
	{
		x::templates::Template* x_template = (*begin);
		switch (x_template->GetType())
		{
		case x::templates::TemplateTypeFrameTransformMatrix:
			this->ApplyFrameTransformMatrix(dynamic_cast<x::templates::FrameTransformMatrix*>(x_template), frame);
			break;

		case x::templates::TemplateTypeFrame:
			this->ApplyFrameTemplate(dynamic_cast<x::templates::Frame*>(x_template), frame);
			break;

		case x::templates::TemplateTypeMesh:
			this->ApplyMeshTemplate(dynamic_cast<x::templates::Mesh*>(x_template), frame);
			break;
		}
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Frame-Transform-x_template anwenden
	\param x_transform
	\param parent_frame

	Wendet das übergebene x_template an das übergeordnete Frame an und übergibt ihm die Transformationsmatrix
*/
void XMeshGenerator::ApplyFrameTransformMatrix(
	x::templates::FrameTransformMatrix* x_transform,
	model::Frame* parent_frame
)
{
	const x::templates::Matrix4x4* x_matrix = x_transform->GetMatrix();
	math::Matrix4x4f mat_transform = x_matrix->GetMatrix();
	mat_transform.Transpose();

	// decompose using 'decomp_affine' by Ken Shoemake
	math::Vector3f scale;
	math::Quaternionf rotation;
	math::Vector3f translation;
	decompose_matrix(mat_transform, translation, rotation, scale);

	parent_frame->SetScale(scale);
	parent_frame->SetRotate(rotation);
	parent_frame->SetTranslate(translation);
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Mesh-x_template anwenden
	\param x_mesh
	\param parent_frame

	Wendet das Mesh x_template an und kreiert aus diesem eine neue Entity unter dem übergebenen
	Parent-Frame.
*/
void XMeshGenerator::ApplyMeshTemplate(
	x::templates::Mesh* x_mesh,
	model::Frame* parent_frame
)
{
	// Entity instanzieren und mit dem Default-List PRIMITIVE_TYPES initialisieren
	model::Entity* entity = parent_frame->GetModel()->CreateEntity();
	const x::string_t& name = x_mesh->GetName();
	if (name.length() > 0)
		entity->SetName(name);
	entity->SetPrimitiveType(model::PrimitiveTriangleList);
	parent_frame->AddEntity(entity);

	// Positionsdaten. Zuerst die Anzahl Vertices holen, dann den Vektor auf die Vertices und als
	// zweites das ganze in ein binäres Array schreiben.
	uint_t vertex_count = x_mesh->GetVertexCount();
	const ubyte_t* positions = x_mesh->GetVertices();
	math::Vector3f* position_data = new math::Vector3f[vertex_count];
	memcpy(position_data, positions, sizeof(float_t) * vertex_count * 3);

	// Indexdaten...
	uint_t face_count = x_mesh->GetFaceCount();
	ubyte_t* binary_faces = this->Triangulate(x_mesh->GetFaces(), face_count); 

	// Die Positionsdaten können gesetzt werden und Indexdaten können jetzt gesetzt werden. Auf
	// die Indexdaten muss eventuell später nochmals zugegriffen werden.
	entity->GetGeometry()->SetVertexCount(vertex_count);
	entity->GetGeometry()->AddVector3Stream(model::SemanticTypePosition, 0, vertex_count, position_data);
	entity->SetIndexCount(face_count * 3);
	entity->SetIndexSize(32);
	entity->SetIndexData(binary_faces);

	// Alle untergeordneten Elemente laden. Es können sich noch Normalvektoren, Materialien,
	// Farbdaten, Texturkoordinaten, etc. darunter befinden. We need to do this process in 3 passes:
	//   - first load all substreams. collect SkinWeights since they are split accross multiple 
	//      templates but must be merged into a single stream.
	//   - merge the SkinWeights into streams
	//   - from all streams loaded eliminate mutiple references (description follows)
	//   - load normals (separatly indexed)
	//   - then load materials. the material-list-tempalte may split the entity so it would become
	//     invalid for use with another stream!
	x::vector<x::templates::SkinWeights*> skinweights;
	uint_t texture_unit = 0;
	const x::templates::Mesh::children_container_t& children = x_mesh->GetChildren();
	for (
		x::templates::Mesh::children_container_t::const_iterator begin = children.begin();
		begin != children.end();
		++begin
	)
	{
		x::templates::Template* x_template = (*begin);
		switch (x_template->GetType())
		{
		case x::templates::TemplateTypeMeshTextureCoords: {
			this->ApplyMeshTextureCoords(dynamic_cast<x::templates::MeshTextureCoords*>(x_template), entity, texture_unit);
			++texture_unit;
			break;
		}
		case x::templates::TemplateTypeMeshVertexColors: {
			;
			break;
		}
		case x::templates::TemplateTypeSkinWeights: {
			skinweights.push_back(dynamic_cast<x::templates::SkinWeights*>(x_template));
			break;
		}
		}
	}

	if (skinweights.size() > 0)
	{
		CreateSkinningStreams(entity, skinweights);
	}

	// Aktuelles Problem: Wenn eine Position mehrfach referenziert wird, aber verschiedene Normalen
	// hat dann wird nur eine der Normalen verwendet. Daher werden als Quick-Fix einfach alle mehrfachen
	// Referenzen eliminiert.
	EliminateMultipleReferences(entity);

	// this loop loads normals if available
	for (
		x::templates::Mesh::children_container_t::const_iterator begin = children.begin();
		begin != children.end();
		++begin
	)
	{
		x::templates::Template* x_template = (*begin);
		if(x_template->GetType() == x::templates::TemplateTypeMeshNormals)
		{
			this->ApplyMeshNormalsTemplate(dynamic_cast<x::templates::MeshNormals*>(x_template), entity);
		}
	}

	// this loop may split the entity into multiple smaller entities depending on the material
	// used.
	for (
		x::templates::Mesh::children_container_t::const_iterator begin = children.begin();
		begin != children.end();
		++begin
	)
	{
		x::templates::Template* x_template = (*begin);
		if(x_template->GetType() == x::templates::TemplateTypeMeshMaterialList)
		{
			this->ApplyMeshMaterialListTemplate(dynamic_cast<x::templates::MeshMaterialList*>(x_template), parent_frame, entity);
		}
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief MeshNormals-x_template anwenden
	\param x_normals
	\param entity

	Wendet das MeshNormals-x_template an, um der übergebenen Entity Normalvektoren hinzufügen zu 
	können.
*/
void XMeshGenerator::ApplyMeshNormalsTemplate(
	x::templates::MeshNormals* x_normals, 
	model::Entity* entity
)
{
	// Normaldaten. Zuerst die Anzahl Normalvektoren holen, dann den Vektor auf die Normalen und als
	// zweites das ganze in ein binäres Array schreiben.
	uint_t normal_count = x_normals->GetNormalCount();
	const ubyte_t* normals = x_normals->GetNormals();
	math::Vector3f* normal_data = new math::Vector3f[normal_count];
	memcpy(normal_data, normals, normal_count * sizeof(float_t) * 3);

	// Indexdaten...
	uint_t face_count = x_normals->GetFaceCount();
	ubyte_t* binary_faces = this->Triangulate(x_normals->GetFaces(), face_count); 

	// Nun folgt der schwierigste Teil der ganzen Übung. Es sind zwei Index-Arrays vorhanden, jede
	// Entity hat aber nur EIN Array. Das heisst wir müssen gegebenenfalls das Normalenarray umsortieren,
	// so dass die BinaryFaces des MeshNormals-Templates mit den Faces der Entity übereinstimmen. 
	// Gelingt dies nicht so wird alles abgebrochen und das Laden ist fehlgelaufen.
	if(entity->GetIndexSize() != 32)
		throw Exception(UNICODE_STRING("not yet supported"));

	// the reorder-function will change the following vars:
	//   - normal_data
	//   - normal_count
	//   - binary_faces
	this->ReorderVectors(
		normal_data, normal_count,
		(udword_t*)entity->GetIndexData(), entity->GetIndexCount(), 
		(udword_t*)binary_faces, face_count * 3
	);
	SAFE_DELETE_ARRAY(binary_faces);

	// Wenn das umordnen geglückt ist, dann können die Normaldaten an die Geometry der Entity übergeben 
	// werden
	entity->GetGeometry()->AddVector3Stream(model::SemanticTypeNormal, 0, normal_count, normal_data);
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Material-List-x_template anwenden
	\param x_materiallist
	\param parent_frame
	\param entity

	Wendet das MaterialList-x_template auf die übergebene Entity an.
*/
void XMeshGenerator::ApplyMeshMaterialListTemplate(
	x::templates::MeshMaterialList* x_materiallist, 
	model::Frame* parent_frame,
	model::Entity* entity
)
{
	// if having more than one material inside the materiallist we need 
	// to split the entity since one entity can have only one material!
	if (x_materiallist->GetMaterialCount() > 1)
	{
		uint_t face_index_count = x_materiallist->GetFaceIndexCount();
		const udword_t* face_indices = x_materiallist->GetFaceIndices();

		// remove the old entity from the frame. we'll be creating new ones
		parent_frame->RemoveEntity(entity);

		uint_t material_index = 0;
		const x::templates::MeshMaterialList::material_container_t& materials = x_materiallist->GetMaterials();
		for (
			x::templates::MeshMaterialList::material_container_t::const_iterator begin = materials.begin();
			begin != materials.end();
			++begin
		)
		{
			// Für jedes Material die ursprüngliche Entity splitten in die Polygone des
			// aktuellen materials und die übrigen
			std::stringstream str;
			str << entity->GetName() << " " << material_index;
			model::Entity* new_entity = parent_frame->GetModel()->CreateEntity();
			new_entity->SetName(str.str());

			// if there are no vertices using this material SplitEntityByFaceIndices will return false. 
			// this means that the new_entity isn't valid and should be discarded. It doesn't make sense
			// to have entities with zero vertices anyway...
			if (SplitEntityByFaceIndices(new_entity, entity, face_index_count, &face_indices[0], material_index))
			{
				parent_frame->AddEntity(new_entity);

				// momentan gibt es jeweils nur einen Schleifendurchlauf, da oben geprüft wird, dass
				// nicht mehr als ein Material vorhanden ist.
				this->ApplyMaterialTemplate((*begin), new_entity);
			}
			else
			{
				delete new_entity;
			}

			// increment material index
			++material_index;
		}

		delete entity;
	}
	else
	{
		this->ApplyMaterialTemplate(*x_materiallist->GetMaterials().begin(), entity);
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Materialtemplate auf eine Entity anwenden
	\param x_material
	\param entity

	Wendet das gegebene Material auf die übergebene Entity an.
*/
void XMeshGenerator::ApplyMaterialTemplate(
	x::templates::Material* x_material, 
	model::Entity* entity
)
{
	model::Material* material = entity->GetModel()->CreateMaterial();

	// Farbe des materials holen
	const x::templates::ColorRGBA* x_diffuse_color = x_material->GetFaceColor();
	const x::templates::ColorRGB* x_specular_color = x_material->GetSpecularColor();
	const x::templates::ColorRGB* x_emissive_color = x_material->GetEmissiveColor();
	float_t x_power = x_material->GetPower();

	material->SetAmbientColor(x_diffuse_color->GetColor());
	material->SetDiffuseColor(x_diffuse_color->GetColor());
	material->SetSpecularColor(x_specular_color->GetColor());
	material->SetEmissiveColor(x_emissive_color->GetColor());
	material->SetPower(x_power);

	// children des materials beachten.
	uint_t texture_unit_index = 0;
	const x::templates::Material::children_container_t& children = x_material->GetChildren();
	for (
		x::templates::Material::children_container_t::const_iterator begin = children.begin();
		begin != children.end();
		++begin
	)
	{
		x::templates::Template* x_template = (*begin);
		switch (x_template->GetType())
		{
		case x::templates::TemplateTypeTextureFilename:
			this->ApplyTextureFilenameTemplate(dynamic_cast<x::templates::TextureFilename*>(x_template), material);
			break;
		}
	}

	entity->SetMaterial(material);
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief TextureFilename-x_template auf Texture-Unit-State anwenden
	\param x_texture_filename
	\param material

	Wendet das übergebene TextureFilename-x_template auf den übergebenen Texture-Unit-State
	an.
*/
void XMeshGenerator::ApplyTextureFilenameTemplate(
	x::templates::TextureFilename* x_texture_filename,
	model::Material* material
)
{
	model::TextureSampler sampler;
	sampler.texture = x_texture_filename->GetFilename();
	material->AddTextureSampler(sampler);
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Textur-Koordinaten-x_template auf Entity anwenden
	\param x_texture_coords
	\param entity
	\param texture_unit

	Wendet das Textur-Koordinaten-x_template auf die übergebene Entity an.
*/
void XMeshGenerator::ApplyMeshTextureCoords(
	x::templates::MeshTextureCoords* x_texture_coords, 
	model::Entity* entity,
	const uint_t texture_unit
)
{
	uint_t texture_coords_count = x_texture_coords->GetTextureCoordsCount();
	const x::vector<x::templates::Coords2d*>& tex_coords = x_texture_coords->GetTextureCoords();
	math::Vector2f* tex_coords_data = new math::Vector2f[texture_coords_count];
	for (uint_t i = 0; i < texture_coords_count; ++i)
	{
		tex_coords_data[i] = tex_coords[i]->GetVector();
	}
	
	entity->GetGeometry()->AddVector2Stream(model::SemanticTypeTextureCoordinate, texture_unit, texture_coords_count, tex_coords_data);
}




//-----------------------------------------------------------------------------------------------------------
void XMeshGenerator::CreateSkinningStreams(
	model::Entity* entity,
	x::vector<x::templates::SkinWeights*>& skinweights
)
{
	uint_t vertex_count = entity->GetGeometry()->GetVertexCount();
	ubyte_t* num_bones_per_vertex_array = new ubyte_t[vertex_count];
	memset(num_bones_per_vertex_array, 0, sizeof(ubyte_t) * vertex_count);

	// collect count of bones / vertex
	for (uint_t i = 0; i < skinweights.size(); ++i)
	{
		x::templates::SkinWeights* weights = skinweights[i];
		const dword_t* vertex_indices = weights->GetVertexIndices();
		for (dword_t j = 0; j < weights->GetNumWeights(); ++j)
		{
			++num_bones_per_vertex_array[vertex_indices[j]];
		}
	}

	// save max bones_per_vertex value
	uint_t bones_per_vertex = 0;
	for (uint_t i = 0; i < vertex_count; ++i)
		if (num_bones_per_vertex_array[i] > bones_per_vertex)
			bones_per_vertex = num_bones_per_vertex_array[i];
		

	// allocate arrays with the information given
	model::Stream* bone_index_stream;
	model::Stream* bone_weight_stream;
	math::Vector4<ubyte_t>* bone_index_data = new math::Vector4<ubyte_t>[vertex_count];
	memset(bone_index_data, 0, sizeof(math::Vector4<ubyte_t>) * vertex_count);
	bone_index_stream = entity->GetGeometry()->AddUByte4Stream(model::SemanticTypeBoneIndices, 0, vertex_count, bone_index_data);
	switch (bones_per_vertex)
	{
	case 2: {
		math::Vector2f* bone_weights_data = new math::Vector2f[vertex_count];
		memset(bone_weights_data, 0, sizeof(math::Vector2f) * vertex_count);
		bone_weight_stream = entity->GetGeometry()->AddVector2Stream(model::SemanticTypeBoneWeights, 0, vertex_count, bone_weights_data);
		break;
	}
	case 3: {
		math::Vector3f* bone_weights_data = new math::Vector3f[vertex_count];
		memset(bone_weights_data, 0, sizeof(math::Vector3f) * vertex_count);
		bone_weight_stream = entity->GetGeometry()->AddVector3Stream(model::SemanticTypeBoneWeights, 0, vertex_count, bone_weights_data);
		break;
	}
	default: {
		throw Exception("given number of bones/vertex is not supported!");
	}
	}

	// reuse the 'num_bones_per_vertex_array'-array to index the bone weights
	memset(num_bones_per_vertex_array, 0, sizeof(ubyte_t) * vertex_count);
	for (uint_t i = 0; i < skinweights.size(); ++i)
	{
		// get information
		x::templates::SkinWeights* weights = skinweights[i];
		string_t bone_name = weights->GetTransformNodeName();
		dword_t num_weights = weights->GetNumWeights();
		const dword_t* vertex_indices = weights->GetVertexIndices();
		const float_t* vertex_weights = weights->GetWeights();
		math::Matrix4x4f matrix_to_bone_space = weights->GetMatrixOffset()->GetMatrix();
		matrix_to_bone_space.Transpose();

		// modify bone
		model::Bone* bone = entity->GetModel()->GetSkeleton()->GetBone(bone_name);
		bone->SetToBoneSpaceMatrix(matrix_to_bone_space);
		ubyte_t bone_index = entity->GetModel()->GetSkeleton()->GetBoneIndex(bone_name);

		// now write weights
		for (dword_t j = 0; j < num_weights; ++j)
		{
			dword_t vertex_index = vertex_indices[j];

			// set bone index
			math::Vector4<ubyte_t> bone_indices = bone_index_stream->GetUByte4(vertex_index);
			switch (num_bones_per_vertex_array[vertex_index])
			{
			case 0: bone_indices.x = bone_index; break;
			case 1: bone_indices.y = bone_index; break;
			case 2: bone_indices.z = bone_index; break;
			case 3: bone_indices.w = bone_index; break;
			}
			bone_index_stream->SetUByte4(vertex_index, bone_indices);

			// set weights
			switch (bones_per_vertex)
			{
			case 2: {
				math::Vector2f v = bone_weight_stream->GetVector2(vertex_index);
				switch (num_bones_per_vertex_array[vertex_index])
				{
				case 0: v.x = vertex_weights[j]; break;
				case 1: v.y = vertex_weights[j]; break;
				}
				bone_weight_stream->SetVector2(vertex_index, v);
				break;
			}
			case 3: {
				math::Vector3f v = bone_weight_stream->GetVector3(vertex_index);
				switch (num_bones_per_vertex_array[vertex_index])
				{
				case 0: v.x = vertex_weights[j]; break;
				case 1: v.y = vertex_weights[j]; break;
				case 2: v.z = vertex_weights[j]; break;
				}
				bone_weight_stream->SetVector3(vertex_index, v);
				break;
			}
			}

			// increment index for this vertex's bone
			++num_bones_per_vertex_array[vertex_index];
		}
	}

	// it's done. things have been merged into the bone_indices_stream and the bone_weights_stream.
	delete[] num_bones_per_vertex_array;
}




//-----------------------------------------------------------------------------------------------------------
void XMeshGenerator::LoadSkeleton(
	model::Model* mesh,
	x::Interface::instances_t &instances,
	vector<string_t>& referenced_frames
)
{
	for (
		x::Interface::instances_t::iterator begin = instances.begin();
		begin != instances.end();
		++begin
	)
	{
		x::templates::Template* x_template = (*begin);
		switch (x_template->GetType())
		{
		// only load FIRST Frame Hierarchy as skeleton.
		case x::templates::TemplateTypeFrame: {
			x::templates::Frame* x_frame = dynamic_cast<x::templates::Frame*>(x_template);
			for (uint_t i = 0; i < referenced_frames.size(); ++i)
			{
				if (referenced_frames[i] == x_frame->GetName())
				{
					model::Skeleton* skeleton = mesh->CreateSkeleton();
					LoadBone(x_frame, skeleton, NULL);
					mesh->SetSkeleton(skeleton);

					// execute UpdateCombinedTransformationMatrix()
					model::Bone* root_bone = skeleton->GetRootBone();
					root_bone->UpdateCombinedTransformationMatrix(NULL);
					return;
				}
			}
		}
		}
	}
}




//-----------------------------------------------------------------------------------------------------------
void XMeshGenerator::LoadBone(
	x::templates::Frame *x_frame,
	model::Skeleton* skeleton,
	model::Bone* parent_bone
)
{
	// create the new bone
	string_t name;
	if (x_frame->GetName().length() == 0)
		return;
	
	name = x_frame->GetName();
	model::Bone* new_bone;
	if (parent_bone != NULL)
		new_bone = skeleton->AddBone(parent_bone, name, math::Vector3f::ZERO(), math::Quaternionf::IDENTITY(), math::Vector3f(1.0f, 1.0f, 1.0f));
	else
		new_bone = skeleton->CreateRootBone(name, math::Vector3f::ZERO(), math::Quaternionf::IDENTITY(), math::Vector3f(1.0f, 1.0f, 1.0f));

	// build hierarchy and bone transformation
	const x::templates::Frame::children_container_t& children = x_frame->GetChildren();
	for (
		x::templates::Frame::children_container_t::const_iterator begin = children.begin();
		begin != children.end();
		++begin
	)
	{
		x::templates::Template* x_template = (*begin);
		switch (x_template->GetType())
		{
		case x::templates::TemplateTypeFrameTransformMatrix: {
			this->ApplyBoneTransformMatrix(dynamic_cast<x::templates::FrameTransformMatrix*>(x_template), new_bone);
			break;
		}
		case x::templates::TemplateTypeFrame: {
			this->LoadBone(dynamic_cast<x::templates::Frame*>(x_template), skeleton, new_bone);
			break;
		}
		}
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Frame-Transform-x_template anwenden
	\param x_transform
	\param bone

	Wendet das übergebene x_template an das übergeordnete Frame an und übergibt ihm die Transformationsmatrix
*/
void XMeshGenerator::ApplyBoneTransformMatrix(
	x::templates::FrameTransformMatrix* x_transform,
	model::Bone* bone
)
{
	const x::templates::Matrix4x4* x_matrix = x_transform->GetMatrix();
	math::Matrix4x4f mat_transform = x_matrix->GetMatrix();
	mat_transform.Transpose();

	// decompose using 'decomp_affine' by Ken Shoemake
	math::Vector3f translation;
	math::Quaternionf rotation;
	math::Vector3f scale;
	decompose_matrix(mat_transform, translation, rotation, scale);

	// set transformations
	bone->SetScale(scale);
	bone->SetRotate(rotation);
	bone->SetTranslate(translation);
}




//-----------------------------------------------------------------------------------------------------------
void XMeshGenerator::CollectFramesReferencedFromAnimation(
	x::templates::AnimationSet* x_set,
	vector<string_t>& referenced_frames
)
{
	const x::templates::AnimationSet::animation_container_t& animations = x_set->GetAnimations();
	for (
		x::templates::AnimationSet::animation_container_t::const_iterator begin = animations.begin();
		begin != animations.end();
		++begin
	)
	{
		CollectFramesReferencedFromAnimation((*begin), referenced_frames);
	}
}




//-----------------------------------------------------------------------------------------------------------
void XMeshGenerator::CollectFramesReferencedFromAnimation(
	x::templates::Animation* x_animation,
	vector<string_t>& referenced_frames
)
{
	for (uint_t i = 0; i < referenced_frames.size(); ++i)
		if (referenced_frames[i] == x_animation->GetReference())
			return;
	referenced_frames.push_back(x_animation->GetReference());
}




//-----------------------------------------------------------------------------------------------------------
void XMeshGenerator::ApplyAnimationSetTemplate(
	x::templates::AnimationSet* x_set,
	model::Skeleton* skeleton,
	model::Animation* animation
)
{
	const x::templates::AnimationSet::animation_container_t& animations = x_set->GetAnimations();
	// get minimum diff between two frames and time of last frame
	dword_t min_diff = INT_MAX;
	dword_t last_frame_time = 0;
	for (
		x::templates::AnimationSet::animation_container_t::const_iterator begin = animations.begin();
		begin != animations.end();
		++begin
	)
	{
		GetMinDiffAndLastFrameTime((*begin), min_diff, last_frame_time);
	}

	if (last_frame_time % min_diff != 0)
	{
		throw Exception(
			"The last frames key time must be a multiple of the minimum difference between "
			"two frames inside the animation!"
		);
	}

	for (
		x::templates::AnimationSet::animation_container_t::const_iterator begin = animations.begin();
		begin != animations.end();
		++begin
	)
	{
		ApplyAnimationTemplate((*begin), skeleton, animation, min_diff, last_frame_time);
	}
}




//-----------------------------------------------------------------------------------------------------------
void XMeshGenerator::GetMinDiffAndLastFrameTime(
	x::templates::Animation* x_animation,
	dword_t& min_diff,
	dword_t& last_frame_time
)
{
	const x::templates::Animation::children_container_t& children = x_animation->GetChildren();
	for (
		x::templates::Animation::children_container_t::const_iterator begin = children.begin();
		begin != children.end();
		++begin
	)
	{
		x::templates::Template* x_template = (*begin);
		switch (x_template->GetType())
		{
		case x::templates::TemplateTypeAnimationKey: {
			GetMinDiffAndLastFrameTime(dynamic_cast<x::templates::AnimationKey*>(x_template), min_diff, last_frame_time);
			break;
		}
		}
	}
}




//-----------------------------------------------------------------------------------------------------------
void XMeshGenerator::GetMinDiffAndLastFrameTime(
	x::templates::AnimationKey* x_animation_key,
	dword_t& min_diff,
	dword_t& last_frame_time
)
{
	const x::templates::AnimationKey::keys_container_t& keys = x_animation_key->GetKeys();
	uint_t key_count = x_animation_key->GetNumKeys();

	// find min_diff
	for (uint_t i = 0; i < (key_count-1); ++i)
	{
		const x::templates::TimedFloatKeys* key1 = keys[i];
		const x::templates::TimedFloatKeys* key2 = keys[i+1];
		dword_t diff = key2->GetTime() - key1->GetTime();
		if (diff < min_diff)
		{
			min_diff = diff;
		}
	}

	// last frame time
	const x::templates::TimedFloatKeys* last_key = keys[key_count-1];
	if (last_key->GetTime() > last_frame_time)
		last_frame_time = last_key->GetTime();
}




//-----------------------------------------------------------------------------------------------------------
void XMeshGenerator::ApplyAnimationTemplate(
	x::templates::Animation* x_animation,
	model::Skeleton* skeleton,
	model::Animation* animation,
	const dword_t frame_diff,
	const dword_t last_frame_time
)
{
	const x::templates::Animation::children_container_t& children = x_animation->GetChildren();
	for (
		x::templates::Animation::children_container_t::const_iterator begin = children.begin();
		begin != children.end();
		++begin
	)
	{
		x::templates::Template* x_template = (*begin);
		switch (x_template->GetType())
		{
		case x::templates::TemplateTypeAnimationKey: {
			ApplyAnimationKeyTemplate(dynamic_cast<x::templates::AnimationKey*>(x_template), x_animation->GetReference(), skeleton, animation, frame_diff, last_frame_time);
			break;
		}
		}
	}
}




//-----------------------------------------------------------------------------------------------------------
void XMeshGenerator::ApplyAnimationKeyTemplate(
	x::templates::AnimationKey* x_animation_key,
	const string_t& referenced_bone,
	model::Skeleton* skeleton,
	model::Animation* animation,
	const dword_t frame_diff,
	const dword_t last_frame_time
)
{
	switch (x_animation_key->GetKeyType())
	{
	case 4: {
		ApplyMatrix4x4AnimationKeyTemplate(x_animation_key, referenced_bone, skeleton, animation, frame_diff, last_frame_time);
		break;
	}
	default:
		throw Exception("given key type inside template AnimationKey is not supported!");
	}
}




//-----------------------------------------------------------------------------------------------------------
void XMeshGenerator::ApplyMatrix4x4AnimationKeyTemplate(
	x::templates::AnimationKey* x_animation_key,
	const string_t& referenced_bone,
	model::Skeleton* skeleton,
	model::Animation* animation,
	const dword_t frame_diff,
	const dword_t last_frame_time
)
{
	// matrices
	const x::templates::AnimationKey::keys_container_t& keys = x_animation_key->GetKeys();
	uint_t key_count = x_animation_key->GetNumKeys();
	uint_t frame_count = last_frame_time / frame_diff;

	// we need three tracks. one for positions, one for rotations and one for scale
	math::Vector3f* translations = new math::Vector3f[frame_count];
	math::Quaternionf* rotations = new math::Quaternionf[frame_count];
	//math::Vector3f* scales = new math::Vector3f[frame_count];

	// validate keys
	for (uint_t i = 0; i < key_count; ++i)
	{
		if (keys[i]->GetTime() % frame_diff != 0)
		{
			throw Exception("All frames must be aligned to 'frame_diff'.");
		}
	}

	// fill tracks
	for (uint_t i = 0; i < frame_count; ++i)
	{
		dword_t frame_time = i * frame_diff;

		uint_t frame0 = 0;
		uint_t frame1 = key_count-1;
		for (uint_t j = 0; j < key_count; ++j)
		{
			if (
				keys[j]->GetTime() > keys[frame0]->GetTime() &&
				keys[j]->GetTime() <= frame_time
				)
			{
				frame0 = j;
			}

			if (
				keys[j]->GetTime() < keys[frame1]->GetTime() &&
				keys[j]->GetTime() >= frame_time
			)
			{
				frame1 = j;
			}
		}

		// get frame0 data
		const x::templates::TimedFloatKeys* key0 = keys[frame0];
		const x::templates::FloatKeys* float_keys0 = key0->GetKeys();
		const float_t* values0 = float_keys0->GetValues();
		math::Matrix4x4f mat_transform0(
			values0[ 0], values0[ 4], values0[ 8], values0[12],
			values0[ 1], values0[ 5], values0[ 9], values0[13],
			values0[ 2], values0[ 6], values0[10], values0[14],
			values0[ 3], values0[ 7], values0[11], values0[15]
		);
		// get frame1 data
		const x::templates::TimedFloatKeys* key1 = keys[frame1];
		const x::templates::FloatKeys* float_keys1 = key1->GetKeys();
		const float_t* values1 = float_keys1->GetValues();
		math::Matrix4x4f mat_transform1(
			values1[ 0], values1[ 4], values1[ 8], values1[12],
			values1[ 1], values1[ 5], values1[ 9], values1[13],
			values1[ 2], values1[ 6], values1[10], values1[14],
			values1[ 3], values1[ 7], values1[11], values1[15]
		);

		// decompose framedata
		math::Vector3f translation0, translation1;
		math::Quaternionf rotation0, rotation1;
		math::Vector3f scale0, scale1;
		decompose_matrix(mat_transform0, translation0, rotation0, scale0);
		decompose_matrix(mat_transform1, translation1, rotation1, scale1);

		// interpolate and save data
		math::Vector3f translation;
		math::Quaternionf rotation;
		math::Vector3f scale;
		interpolate_frames(
			frame_time,
			key0->GetTime(), translation0, rotation0, scale0,
			key1->GetTime(), translation1, rotation1, scale1,
			translation, rotation, scale
		);
		translations[i] = translation;
		rotations[i] = rotation;
		//scales[i] = scale;
	}
	/*
	for (uint_t i = 0; i < key_count; ++i)
	{
		const x::TimedFloatKeys* key = keys[i];
		const x::FloatKeys* float_keys = key->GetKeys();
		const float_t* values = float_keys->GetValues();

		Matrix4x4f mat_transform(
			values[ 0], values[ 4], values[ 8], values[12],
			values[ 1], values[ 5], values[ 9], values[13],
			values[ 2], values[ 6], values[10], values[14],
			values[ 3], values[ 7], values[11], values[15]
		);

		// decompose matrix
		math::Vector3f translation;
		math::Quaternionf rotation;
		math::Vector3f scale;
		decompose_matrix(mat_transform, translation, rotation, scale);

		translations[i] = translation;
		rotations[i] = rotation;
		scales[i] = scale;
	}*/

	// search referenced bone
	model::Bone* bone = skeleton->GetBone(referenced_bone);

	// create translation track for this bone
	model::VectorAnimationTrack* translate_track = animation->CreateVectorAnimationTrack();
	translate_track->SetAnimatedVector(bone);
	translate_track->SetKeyFrames(translations);
	animation->AddAnimationTrack(translate_track);
	// create rotation track for this bone
	model::QuaternionAnimationTrack* rotate_track = animation->CreateQuaternionAnimationTrack();
	rotate_track->SetAnimatedQuaternion(bone);
	rotate_track->SetKeyFrames(rotations);
	animation->AddAnimationTrack(rotate_track);

	// update animation with the frame count which can't be evaulated beforehand
	if (animation->GetFrameCount() == 0)
	{
		animation->SetFrameCount(frame_count);
	}
	if (animation->GetFrameCount() != frame_count)
	{
		throw Exception(
			"this XAnimationKey doesn't contain the same amount of keyframes as the other "
			"templates inside this AnimationSet. There must be something wronge!"
		);
	}
	if (animation->GetFramesPerSecond() == 0)
	{
		//dword_t time = keys[1]->GetTime();
		animation->SetFramesPerSecond(1000 / frame_diff);
	}
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Binary-Faces in Dreieckspolygone umwandeln
	\param data
	\param face_count
	\return Array mit Dreieckspolygonen. 
	\note
		Es wird angenommen dass im Input-Array keine 2-Eck-Polygone vorhanden sind
	\note
		Das Input-Array besitzt folgendes Format: Für jedes Polygon: Anzahl Eckpunkte, Eckpunkt-Index 1, 
		Eckpunkt-Index 2, ... . Im Output-Array wird die Eckpunktezahl nicht mehr angegeben, da dies
		nun überflüssig wäre und so auch besser geeignet ist für weitere Verwendungen (z.B. in DX)!

	Die übergebenen Faces/Polygone können jeweils n-Eck-Polygone sein. Die Aufgabe dieser Methode ist
	es daraus Dreieckspolygone zu berechnen.
*/
ubyte_t* XMeshGenerator::Triangulate(
	const ubyte_t* data,
	uint_t& face_count
)
{
	uint_t new_face_count = 0;
	udword_t* old_ptr = NULL;
	udword_t* new_ptr = NULL;

	/*
	 *	In einem ersten Schritt wird berechnet wieviele Polygone aus den jetzigen Polygonen
	 *	entstehen würden.
	 */
	new_face_count = 0;
	old_ptr = (udword_t*)data;
	for (uint_t i = 0; i < face_count; ++i)
	{
		uint_t vertex_count = (uint_t)(*old_ptr);
		new_face_count += (vertex_count - 2); // Anzahl Indices - 2, für 3 Indices = 1 Polygon für 4 = 2 Polygone, etc.
		old_ptr += (vertex_count + 1); // Grössenangabe + Anzahl Indices
	}

	/*
	 *	Im zweiten Schritt werden die Polygone umgerechnet. Beispiel wie umgerechnet wird:
	 *
	 *	       2		
	 *         /'-.  
	 *        /    '-.
	 *       /        '-. 3
	 *      /         .'
	 *     /        .'
	 *    --------- 4
	 *   1
	 *
	 *	Aufgabe dieser Methode ist es dieses Viereckspolygon in zwei Dreieckspolygone
	 *	umzuwandeln. Dabei nimmt die Methode immer den ersten Vertex und fügt dazu die zwei
	 *	nächsten Vertices des alten Polygons hinzu. Aus dem Polygon:
	 *		1, 2, 3, 4
	 *	entstünden also zwei Polygone:
	 *		1, 2, 3
	 *		1, 3, 4
	 *	Es ist die gleiche Technik wie sie beim TRIANGLE_FAN-Primitive in DirectX eingesetzt
	 *	wird z.B.
	 */
	ubyte_t* new_data = new ubyte_t[new_face_count * 3 * sizeof(udword_t)];
	old_ptr = (udword_t*)data;
	new_ptr = (udword_t*)new_data;
	for (uint_t i = 0; i < face_count; ++i)
	{
		uint_t vertex_count = (uint_t)(*old_ptr);
		++old_ptr;

		udword_t first_vertex = *old_ptr;
		++old_ptr;

		for (uint_t i = 0; i < (vertex_count - 2); ++i)
		{
			new_ptr[0] = first_vertex;
			new_ptr[1] = old_ptr[0];
			new_ptr[2] = old_ptr[1];

			new_ptr += 3;
			++old_ptr;
		}

		// nachinkrementieren, da er immer noch den letzten Index des Polygons zeigt, er 
		// muss aber neu auf den Count des nächsten Polygons zeigen
		++old_ptr; 
	}

	face_count = new_face_count;
	return new_data;
}




//-----------------------------------------------------------------------------------------------------------
void XMeshGenerator::EliminateMultipleReferences(
	model::Entity* entity
)
{
	uint_t vertex_count = entity->GetGeometry()->GetVertexCount();
	uint_t face_count = entity->GetIndexCount() / 3;
	udword_t* faces = (udword_t*)entity->GetIndexData();

	// ref-count
	uint_t* ref_count = new uint_t[vertex_count];
	memset(ref_count, 0, sizeof(uint_t)*vertex_count);
	for (uint_t i = 0; i < face_count; ++i)
	{
		++ref_count[faces[i*3+0]];
		++ref_count[faces[i*3+1]];
		++ref_count[faces[i*3+2]];
	}

	// berechnen um wieviel die streams wachsen müssen
	uint_t grow_size = 0;
	for (uint_t i = 0; i < vertex_count; ++i)
	{
		if (ref_count[i] > 1)
		{
			grow_size += (ref_count[i] - 1);
		}
	}

	// eventuell vergrössern und umsortieren
	if (grow_size > 0)
	{
		uint_t new_vertex_count = vertex_count + grow_size;

		// neue streams allozieren und alte daten kopieren
		vector<model::Stream*> new_streams;
		for (uint_t i = 0; i < entity->GetGeometry()->GetStreamCount(); ++i)
		{
			model::Stream* new_stream;
			model::Stream* stream = entity->GetGeometry()->GetStream(i);
			switch (stream->GetValueType())
			{
			case model::ValueUByte4: {
				math::Vector4<ubyte_t>* new_data = new math::Vector4<ubyte_t>[new_vertex_count];
				model::UByte4Stream* new_stream_inst = entity->GetGeometry()->CreateDefaultUByte4Stream();
				new_stream_inst->SetData(new_vertex_count, new_data);
				for (uint_t j = 0; j < stream->GetElementCount(); ++j)
					new_stream_inst->SetUByte4(j, stream->GetUByte4(j));
				new_stream_inst->SetSemantics(stream->GetSemantics());
				new_stream_inst->SetSemanticsInfo(stream->GetSemanticInfo());
				new_stream = new_stream_inst;
				break;
			}
			case model::ValueColor: {
				throw Exception("TODO");
				break;
			}
			case model::ValueFloat: {
				throw Exception("TODO");
			}
			case model::ValueVector2: {
				math::Vector2f* new_data = new math::Vector2f[new_vertex_count];
				model::Vector2Stream* new_stream_inst = entity->GetGeometry()->CreateDefaultVector2Stream();
				new_stream_inst->SetData(new_vertex_count, new_data);
				for (uint_t j = 0; j < stream->GetElementCount(); ++j)
					new_stream_inst->SetVector2(j, stream->GetVector2(j));
				new_stream_inst->SetSemantics(stream->GetSemantics());
				new_stream_inst->SetSemanticsInfo(stream->GetSemanticInfo());
				new_stream = new_stream_inst;
				break;
			}
			case model::ValueVector3:{
				math::Vector3f* new_data(new math::Vector3f[new_vertex_count]);
				model::Vector3Stream* new_stream_inst = entity->GetGeometry()->CreateDefaultVector3Stream();
				new_stream_inst->SetData(new_vertex_count, new_data);
				for (uint_t j = 0; j < stream->GetElementCount(); ++j)
					new_stream_inst->SetVector3(j, stream->GetVector3(j));
				new_stream_inst->SetSemantics(stream->GetSemantics());
				new_stream_inst->SetSemanticsInfo(stream->GetSemanticInfo());
				new_stream = new_stream_inst;
				break;
			}
			case model::ValueQuaternion: {
				throw Exception("TODO");
			}
			}

			// copy semantics
			new_streams.push_back(new_stream);
		}
		
		// mehrfachreferenzen lösen
		udword_t index = vertex_count;
		for (uint_t i = 0; i < face_count*3; ++i)
		{
			udword_t old_index = faces[i];

			if (ref_count[old_index] > 1)
			{
				--ref_count[old_index];
				udword_t new_index = index;
				faces[i] = new_index;
				++index;

				// copy data to current end of stream
				for (uint_t j = 0; j < entity->GetGeometry()->GetStreamCount(); ++j)
				{
					model::Stream* old_stream = entity->GetGeometry()->GetStream(j);
					model::Stream* new_stream = new_streams[j];

					switch (old_stream->GetValueType())
					{
					case model::ValueUByte4: {
						new_stream->SetUByte4(new_index, old_stream->GetUByte4(old_index));
						break;
					}
					case model::ValueColor: {
						new_stream->SetColor(new_index, old_stream->GetColor(old_index));
						break;
					}
					case model::ValueFloat: {
						new_stream->SetFloat(new_index, old_stream->GetFloat(old_index));
						break;
					}
					case model::ValueVector2: {
						new_stream->SetVector2(new_index, old_stream->GetVector2(old_index));
						break;
					}
					case model::ValueVector3:{
						new_stream->SetVector3(new_index, old_stream->GetVector3(old_index));
						break;
					}
					case model::ValueQuaternion: {
						new_stream->SetQuaternion(new_index, old_stream->GetQuaternion(old_index));
						break;
					}
					}
				}
			}
		}

		// neue streams einfügen
		uint_t old_stream_count = entity->GetGeometry()->GetStreamCount();
		entity->GetGeometry()->RemoveAllStreams();
		for (uint_t i = 0; i < new_streams.size(); ++i)
			entity->GetGeometry()->AddStream(new_streams[i]);
		entity->GetGeometry()->SetVertexCount(new_vertex_count);
	}

	delete[] ref_count;
}




//-----------------------------------------------------------------------------------------------------------
/*!
	\brief Vektoren umsortieren, so dass zwei Index-Arrays übereinstimmen
	\param vectors
		Array von Vektoren welches eventuell umgeordnet werden muss
	\param vector_count
		Anzahl Vektoren in vectors
	\param target_indices
		Array in welchem die Indices gespeichert sind, so wie vector_indices einmal aussehen soll
	\param target_index_count
		Anzahl Indices in target_indices
	\param vector_indices
		Das Array von Indices dessen Indices zusammen mit den Vektoren umgeändert werden soll
	\param vector_index_count
		Anzahl Indices in vector_indices

	Diese Methode ändert die Sortierung von vectors so, dass nachher jeder Index in vector_indices
	noch auf denselben Vektor (NICHT Index) in vectors zeigt und die beiden Arrays target_indices
	und vector_indices identisch sind.
*/
void XMeshGenerator::ReorderVectors(
	math::Vector3f*& vectors, 
	uint_t& vector_count, 
	const udword_t* target_indices, 
	const uint_t target_index_count, 
	udword_t* vector_indices, 
	const uint_t vector_index_count
)
{
	// wenn beide Index-Arrays nicht dieselbe Anzahl Einträge haben, dann kanns schon mal nicht
	// funktionieren!
	if (target_index_count != vector_index_count)
	{
		throw Exception(
			UNICODE_STRING("target_index_count != vector_index_count!")
		);
	}


	// Ein Früher Ausstieg ist an dieser Stelle möglich, falls die beiden Arrays bereits jetzt
	// übereinstimmen kann man sich alle Arbeit sparen!
	if (memcmp(target_indices, vector_indices, target_index_count * sizeof(udword_t)) == 0)
		return;

	// first search the maximum target index. this determines how large the target
	// array must be. it 'should' be >= the old array else we'll merge some 
	// vectors into one...
	uint_t max_target_index = 0;
	for (uint_t i = 0; i < target_index_count; ++i)
	{
		if (target_indices[i] > max_target_index)
			max_target_index = target_indices[i];
	}

	// allocate the new array and copy things
	vector_count = max_target_index + 1;
	math::Vector3f* temp_array = new math::Vector3f[vector_count];
	for (uint_t i = 0; i < target_index_count; ++i)
	{
		temp_array[target_indices[i]] = vectors[vector_indices[i]];
	}
	delete[] vectors;
	vectors = temp_array;

	// copy indices
	memcpy(vector_indices, target_indices, target_index_count * sizeof(udword_t));
}




//-----------------------------------------------------------------------------------------------------------
bool_t XMeshGenerator::SplitEntityByFaceIndices(
	model::Entity* new_entity,
	model::Entity* old_entity,
	const uint_t face_index_count,
	const udword_t* face_indices,
	const udword_t material_index
)
{
	if (old_entity->GetIndexSize() != 32)
		throw Exception("32bit-indices expected!");

	// note: face_index_count should match the face count of the old_entity...
	uint_t old_index_count = old_entity->GetIndexCount();
	udword_t* old_indices = (udword_t*)old_entity->GetIndexData(); // we've got always 32 bit indices
	uint_t old_vertex_count = old_entity->GetGeometry()->GetVertexCount();
	udword_t* index_mappings = new udword_t[old_vertex_count];
	udword_t invalid_mark = 0xffffffff;
	for (uint_t i = 0; i < old_vertex_count; ++i)
		index_mappings[i] = invalid_mark; // initialize with invalid value we can recognise later

	// evaluate face count
	uint_t old_face_count = 0;
	switch (old_entity->GetPrimitiveType())
	{
	case model::PrimitivePointList:		old_face_count = old_index_count; break;
	case model::PrimitiveLineList:		old_face_count = old_index_count / 2; break;
	case model::PrimitiveLineStrip:		old_face_count = old_index_count - 1; break;
	case model::PrimitiveTriangleList:	old_face_count = old_index_count / 3; break;
	case model::PrimitiveTriangleStrip:	old_face_count = old_index_count - 2; break;
	case model::PrimitiveTriangleFan:	old_face_count = old_index_count - 2; break;
	}

	// check
	if (old_face_count != face_index_count)
	{
		throw Exception(
			UNICODE_STRING(
				"expecting that the given face count from the material-list-template and the face count "
				"of the given entity match. this isn't the case so there is either a bug inside this "
				"library or the model isn't a valid one..."
			)
		);
	}

	// calculate the new face count
	uint_t new_face_count = 0;
	for (uint_t i = 0; i < old_face_count; ++i)
	{
		// if given face is not part of the new entity...
		if (face_indices[i] != material_index)
			continue;

		++new_face_count; // a new face found
	}

	// allocate based on the new_face_count the new index-array
	uint_t new_index_count = 0;
	switch (old_entity->GetPrimitiveType())
	{
	case model::PrimitivePointList:		new_index_count = new_face_count; break;
	case model::PrimitiveLineList:		new_index_count = new_face_count * 2; break;
	case model::PrimitiveLineStrip:		new_index_count = new_face_count + 1; break;
	case model::PrimitiveTriangleList:	new_index_count = new_face_count * 3; break;
	case model::PrimitiveTriangleStrip:	new_index_count = new_face_count + 2; break;
	case model::PrimitiveTriangleFan:	new_index_count = new_face_count + 2; break;
	}
	ubyte_t* new_indexdata = new ubyte_t[sizeof(udword_t)*new_index_count];
	udword_t* new_indices = (udword_t*)new_indexdata;
	memset(new_indices, 0, sizeof(udword_t)*new_index_count);

	// for each face calculate the new index mappings and fill index array
	uint_t new_vertex_count = 0;
	uint_t new_face_index = 0;
	for (uint_t old_face_index = 0; old_face_index < old_face_count; ++old_face_index)
	{
		// if given face is not part of the new entity...
		if (face_indices[old_face_index] != material_index)
			continue;

		bool_t indices_used[3] = { false, false, false };
		uint_t old_indices_index[3] = { 0, 0, 0 };
		uint_t new_indices_index[3] = { 0, 0, 0 };

		switch (old_entity->GetPrimitiveType())
		{
		case model::PrimitivePointList: {
			indices_used[0] = true;
			old_indices_index[0] = old_face_index;
			new_indices_index[0] = new_face_index;
			break;
		}
		case model::PrimitiveLineList: {
			for (uint_t j = 0; j < 2; ++j)
			{
				indices_used[j] = true;
				old_indices_index[j] = old_face_index*2+j;
				new_indices_index[j] = new_face_index*2+j;
			}
			break;
		}
		case model::PrimitiveLineStrip: {
			for (uint_t j = 0; j < 2; ++j)
			{
				indices_used[j] = true;
				old_indices_index[j] = old_face_index+j;
				new_indices_index[j] = new_face_index+j;
			}
			break;
		}
		case model::PrimitiveTriangleList: {
			for (uint_t j = 0; j < 3; ++j)
			{
				indices_used[j] = true;
				old_indices_index[j] = old_face_index*3+j;
				new_indices_index[j] = new_face_index*3+j;
			}
			break;
		}
		case model::PrimitiveTriangleStrip: {
			for (uint_t j = 0; j < 3; ++j)
			{
				indices_used[j] = true;
				old_indices_index[j] = old_face_index+j;
				new_indices_index[j] = new_face_index+j;
			}
			break;
		}
		case model::PrimitiveTriangleFan: {
			for (uint_t j = 0; j < 3; ++j)
			{
				indices_used[j] = true;
				old_indices_index[j] = (j == 0 ? 0 : old_face_index+j);
				new_indices_index[j] = (j == 0 ? 0 : new_face_index+j);
			}
			break;
		}
		}

		for (uint_t j = 0; j < 3; ++j)
		{
			if (indices_used[j])
			{
				// try to map the given index
				udword_t old_index = old_indices[old_indices_index[j]];
				// index which hasn't been mapped yet
				if (index_mappings[old_index] == invalid_mark)
				{
					index_mappings[old_index] = new_vertex_count;
					++new_vertex_count;
				}
				// copy index
				new_indices[new_indices_index[j]] = index_mappings[old_index];
			}
		}

		++new_face_index;
	}

	// check if there are vertices using this group...
	if (new_vertex_count == 0)
	{
		delete[] index_mappings;
		delete[] new_indexdata;
		return false;
	}

	// fill the new entity
	new_entity->SetPrimitiveType(old_entity->GetPrimitiveType());
	new_entity->SetIndexSize(sizeof(udword_t)*8);
	new_entity->SetIndexCount(new_index_count);
	new_entity->SetIndexData(new_indexdata);
	
	// now finally copy all the streams
	new_entity->GetGeometry()->SetVertexCount(new_vertex_count);
	for (uint_t i = 0; i < old_entity->GetGeometry()->GetStreamCount(); ++i)
	{
		model::Stream* old_stream = old_entity->GetGeometry()->GetStream(i);
		model::Stream* new_stream;
		
		uint_t element_size = 0;
		switch (old_stream->GetValueType())
		{
		case model::ValueUByte4: {
			element_size = sizeof(math::Vector4<ubyte_t>);
			model::UByte4Stream* stream = new_entity->GetGeometry()->CreateDefaultUByte4Stream();
			stream->SetSemantics(old_stream->GetSemantics());
			stream->SetSemanticsInfo(old_stream->GetSemanticInfo());
			math::Vector4<ubyte_t>* data = new math::Vector4<ubyte_t>[new_vertex_count];
			stream->SetData(new_vertex_count, data);
			new_stream = stream;
			break;
		}
		case model::ValueColor: {
			element_size = sizeof(udword_t); 
			break;
		}
		case model::ValueFloat: {
			element_size = sizeof(float); 
			break;
		}
		case model::ValueVector2: {
			element_size = sizeof(float)*2; 
			model::Vector2Stream* stream = new_entity->GetGeometry()->CreateDefaultVector2Stream();
			stream->SetSemantics(old_stream->GetSemantics());
			stream->SetSemanticsInfo(old_stream->GetSemanticInfo());
			math::Vector2f* data = new math::Vector2f[new_vertex_count];
			stream->SetData(new_vertex_count, data);
			new_stream = stream;
			break;
		}
		case model::ValueVector3: {
			element_size = sizeof(float)*3;
			model::Vector3Stream* stream = new_entity->GetGeometry()->CreateDefaultVector3Stream();
			stream->SetSemantics(old_stream->GetSemantics());
			stream->SetSemanticsInfo(old_stream->GetSemanticInfo());
			math::Vector3f* data = new math::Vector3f[new_vertex_count];
			stream->SetData(new_vertex_count, data);
			new_stream = stream;
			break;
		}
		case model::ValueQuaternion: {
			element_size = sizeof(float)*4; 
			break;
		}
		}

		// copy data
		for (uint_t i = 0; i < old_vertex_count; ++i)
		{
			if (index_mappings[i] != invalid_mark)
			{
				switch (old_stream->GetValueType())
				{
				case model::ValueUByte4: {
					const math::Vector4<ubyte_t>& v = old_stream->GetUByte4(i);
					new_stream->SetUByte4(index_mappings[i], v);
					break;
				}
				case model::ValueColor: {
					throw Exception("todo copy color info");
					break;
				}
				case model::ValueFloat: {
					throw Exception("todo copy float info");
					break;
				}
				case model::ValueVector2: {
					const math::Vector2f& v = old_stream->GetVector2(i);
					new_stream->SetVector2(index_mappings[i], v);
					break;
				}
				case model::ValueVector3: {
					const math::Vector3f& v = old_stream->GetVector3(i);
					new_stream->SetVector3(index_mappings[i], v);
					break;
				}
				case model::ValueQuaternion: {
					throw Exception("todo copy quaternion info");
					break;
				}
				}
			}
		}

		new_entity->GetGeometry()->AddStream(new_stream);
	}

	// DONE!!
	delete[] index_mappings;
	return true;
}




//-----------------------------------------------------------------------------------------------------------
void XMeshGenerator::DeleteEmptyChildren(
	model::Frame* frame
)
{
	// recursive for all frames
	for (uint_t i = 0; i < frame->GetFrameCount(); ++i)
		DeleteEmptyChildren(frame->GetFrame(i));

	// delete frame if it hasn't got any children
	if (frame->GetFrameCount() == 0 && frame->GetEntityCount() == 0 && frame->GetParentFrame() != NULL)
	{
		model::Frame* parent_frame = frame->GetParentFrame();
		parent_frame->RemoveFrame(frame);
		delete frame;
	}
}




//-----------------------------------------------------------------------------------------------------------
void XMeshGenerator::TryIndexConversion(
	model::Frame* frame
)
{
	for (uint_t i = 0; i < frame->GetFrameCount(); ++i)
		TryIndexConversion(frame->GetFrame(i));
	for (uint_t i = 0; i < frame->GetEntityCount(); ++i)
		TryIndexConversion(frame->GetEntity(i));
}




//-----------------------------------------------------------------------------------------------------------
void XMeshGenerator::TryIndexConversion(
	model::Entity* entity
)
{
	if (entity->GetIndexSize() != 32)
		return;

	// check old indices
	bool_t conversion_possible = true;
	uint_t index_count = entity->GetIndexCount();
	udword_t* old_indices = (udword_t*)entity->GetIndexData(); // we've got always 32 bit indices
	for (uint_t i = 0; i < index_count; ++i)
	{
		if (old_indices[i] >= 0xffff)
		{
			conversion_possible = false;
			break;
		}
	}
	
	// if conversion not possible return
	if (!conversion_possible)
		return;
			
	// if conversion possible convert
	ubyte_t* new_indexdata = new ubyte_t[sizeof(uword_t)*index_count];
	uword_t* new_indices = (uword_t*)new_indexdata;
	memset(new_indices, 0, sizeof(uword_t)*index_count);
	for (uint_t i = 0; i < index_count; ++i)
		new_indices[i] = old_indices[i];
		
	// set index data
	entity->SetIndexSize(16);
	entity->SetIndexData(new_indexdata);
	SAFE_DELETE_ARRAY(old_indices);
}

} // namespace x

