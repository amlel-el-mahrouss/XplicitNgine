/*
 * =====================================================================
 *
 *			XplicitNgin
 *			Copyright XPX, all rights reserved.
 *
 *			File: LocalPlayer.h
 *			Purpose: Client-side Player Instance
 *
 * =====================================================================
 */

#pragma once

#include "Instance.h"
#include "ApplicationContext.h"
#include "CameraInstance.h"
#include "NetworkInstance.h"

// FIXME: rework them when we will roll our own renderer.
namespace Xplicit::Client
{
	constexpr const int XPLICIT_NETWORK_DELAY = 100;
	constexpr const float XPLICIT_SPEED = 20.f;

	class LocalActor : public Instance
	{
	public:
		LocalActor(int64_t id);
		virtual ~LocalActor();

		LocalActor& operator=(const LocalActor&) = default;
		LocalActor(const LocalActor&) = default;

		virtual INSTANCE_TYPE type() noexcept override { return INSTANCE_ACTOR; }
		virtual const char* name() noexcept override { return ("LocalActor"); }

		virtual void update() override;

	private:
		IAnimatedMeshSceneNode* m_player_model_node;
		IAnimatedMesh* m_player_model;
		NetworkInstance* m_network;
		CameraInstance* m_camera;
		int64_t m_id;

	};

	class XPLICIT_API LocalActorMoveEvent : public Event
	{
	public:
		LocalActorMoveEvent();
		virtual ~LocalActorMoveEvent();

		LocalActorMoveEvent& operator=(const LocalActorMoveEvent&) = default;
		LocalActorMoveEvent(const LocalActorMoveEvent&) = default;

		virtual void operator()() override;
		const char* name() noexcept;

	private:
		NetworkPacket m_packet;

	};
}
