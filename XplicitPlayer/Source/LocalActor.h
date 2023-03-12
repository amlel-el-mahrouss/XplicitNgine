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

#include "ApplicationContext.h"
#include "NetworkInstance.h"
#include "LocalInstance.h"

// FIXME: rework them when we will roll our own renderer.
namespace Xplicit::Client
{
	constexpr const int XPLICIT_NETWORK_DELAY = 100;
	constexpr const float XPLICIT_SPEED = 20.f;

	class LocalActor : public Instance
	{
	public:
		LocalActor() = delete;

	public:
		LocalActor(int64_t id);
		virtual ~LocalActor();

		LocalActor& operator=(const LocalActor&) = default;
		LocalActor(const LocalActor&) = default;

		virtual INSTANCE_TYPE type() noexcept override { return INSTANCE_ACTOR; }
		virtual const char* name() noexcept override { return ("LocalActor"); }

		IAnimatedMeshSceneNode* operator->() const;
		virtual void update() override;


	private:
		IAnimatedMeshSceneNode* m_node;
		NetworkInstance* m_network;
		NetworkPacket m_packet;
		IAnimatedMesh* m_model;
		int64_t m_id;

	};

	class XPLICIT_API LocalMoveEvent : public Event
	{
	public:
		LocalMoveEvent();
		virtual ~LocalMoveEvent();

		LocalMoveEvent& operator=(const LocalMoveEvent&) = default;
		LocalMoveEvent(const LocalMoveEvent&) = default;

		virtual void operator()() override;
		const char* name() noexcept;

	private:
		NetworkInstance* m_network;
		NetworkPacket m_packet;

	};
}
