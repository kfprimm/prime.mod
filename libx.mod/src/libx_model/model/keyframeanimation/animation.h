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
/*!	\file model/keyframeanimation/animation.h
	\brief Definition von x::model::Animation
	\version 0.1
	\date 21.9.2003
	\author Christian Oberholzer

	Definition von x::model::Animation
*/
//-----------------------------------------------------------------------------------------------------------
#ifndef MODEL_KEYFRAMEANIMATION_ANIMATION_H
#define MODEL_KEYFRAMEANIMATION_ANIMATION_H

#include "../../../libx_core/global/global.h"
#include "./animationtrack.h"
#include "./vector3streamanimationtrack.h"
#include "./vectoranimationtrack.h"
#include "./quaternionanimationtrack.h"

namespace x { namespace model {

//-----------------------------------------------------------------------------------------------------------
/*!
*/
class Animation
{
public:
	// Konstruktur & Destruktor
	Animation();
	virtual ~Animation();

	/// Den Namen der Animation setzen
	virtual void SetName(const string_t& name);
	/// Anzahl der Frames der Animation setzen
	virtual void SetFrameCount(const uint_t frame_count);
	/// Abspielgeschwindigkeit in Frames/s setzen
	virtual void SetFramesPerSecond(const uint_t frames_per_second);
	/// Eine Animationsspur hinzufügen
	virtual void AddAnimationTrack(AnimationTrack* animation_track);

	/// Name der Animation zurückgeben
	virtual const string_t& GetName() const;
	/// Anzahl der Frames dieser Animation zurückgeben
	virtual const uint_t GetFrameCount() const;
	/// Geschwindigkeit der Animation in Frames/s zurückgeben
	virtual const uint_t GetFramesPerSecond() const;
	/// Zeit welche für einen Durchlauf der Animation benötigt wird. Entspricht framescount / fps.
	virtual const float_t GetRunningTime() const;

	/// Ein Animationsframe einer gegebenen Zeit und deren Gewichtung zurückgeben
	virtual void GetFrames(const float_t time, uint_t& frame1, uint_t& frame2, float_t& weight_frame1);
	/// Animation auf einen gewissen Zeitpunkt setzen und anwenden
	virtual void Apply(const float_t time);

	/// Vector3StreamAnimationTrack erstellen
	virtual Vector3StreamAnimationTrack* CreateVector3StreamAnimationTrack();
	/// VectorAnimationTrack erstellen
	virtual VectorAnimationTrack* CreateVectorAnimationTrack();
	/// QuaternionAnimationTrack erstellen
	virtual QuaternionAnimationTrack* CreateQuaternionAnimationTrack();
	/// Normals-Animationtrack erstellen
	virtual void CreateNormalsAnimationTrack();
	/// Tangentvector-Animationtrack erstellen
	virtual void CreateTangentsAnimationTrack();

protected:
	/// Name der Animation
	string_t name_;
	/// Anzahl der Frames die für diese Animation existieren
	uint_t frame_count_;
	/// Geschwindigkeit in Frames/s in der die Animation laufen soll
	uint_t frames_per_second_;
	/// Animationsspuren
	vector<AnimationTrack*> tracks_;
};

}} // namespace x::model

#endif // MODEL_KEYFRAMEANIMATION_ANIMATION_H
