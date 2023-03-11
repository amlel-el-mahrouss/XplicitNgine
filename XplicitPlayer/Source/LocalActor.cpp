/*
 * =====================================================================
 *
 *				XplicitNgin C++ Game Engine
 *			Copyright XPX, all rights reserved.
 *
 *			File: LocalPlayer.cpp
 *			Purpose: Client-side Player Instance
 *
 * =====================================================================
 */

#include "LocalActor.h"

#include "App.h"
#include "XUI.h"

namespace Xplicit::Client
{
	LocalActor::LocalActor()
		: Instance(), m_player_model(nullptr), m_player_model_node(nullptr)
	{

		XPLICIT_GET_DATA_DIR(data_dir);
		std::string mesh_path = data_dir;
		mesh_path += "\\Models\\player_model.obj";

		m_player_model = IRR->getSceneManager()->getMesh(mesh_path.c_str());
		m_player_model_node = IRR->getSceneManager()->addAnimatedMeshSceneNode(m_player_model);

		if (m_player_model && m_player_model_node)
		{
			m_player_model_node->setPosition(vector3df(0, 0, 0));
			m_player_model_node->setMaterialFlag(EMF_LIGHTING, false);
		}

		XPLICIT_INFO(mesh_path);

		assert(m_player_model);
		assert(m_player_model_node);

#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("LocalActor::LocalActor");
#endif
	}

	LocalActor::~LocalActor()
	{
#ifdef XPLICIT_DEBUG
		XPLICIT_INFO("LocalActor::~LocalActor");
#endif

		if (m_player_model)
			m_player_model->drop();

	}

	void LocalActor::update()
	{
		
	}
}