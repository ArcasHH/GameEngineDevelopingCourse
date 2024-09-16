#include <RenderEngine.h>
#include <RHIHelper.h>
#include <Mesh.h>

namespace GameEngine::Render
{
	RenderEngine::RenderEngine()
	{
		m_rhi = HAL::RHIHelper::CreateRHI("D3D12");
		m_rhi->Init();
#if 0
		RenderObject::Ptr box = std::make_shared<RenderObject>();
		m_RenderObjects.push_back(box);		
		box->m_mesh = m_rhi->CreateBoxMesh();
		box->m_material = m_rhi->GetMaterial(box->m_mesh->GetName());
#endif
		RenderObject::Ptr myMesh = std::make_shared<RenderObject>();
		m_RenderObjects.push_back(myMesh);
		myMesh->m_mesh = m_rhi->CreateMyMesh();
		myMesh->m_material = m_rhi->GetMaterial(myMesh->m_mesh->GetName());
		myMesh->m_position = Core::Math::Vector3f(1.f, 0.f, 0.f);
		
		m_rhi->ExecuteCommandLists();
		m_rhi->Flush();
	}

	void RenderEngine::Update(float dt)
	{
		for (int i = 0; i < m_RenderObjects.size(); ++i)
		{
			//If there are several objects in m_RenderObjects, it draws to different buffers. fix it
			Move(m_RenderObjects[i], dt);
			m_rhi->Update(m_RenderObjects[i]->m_mesh, m_RenderObjects[i]->m_material, m_RenderObjects[i]->m_position, dt);
			m_rhi->Flush();
		}
	}

	void RenderEngine::Move(RenderObject::Ptr rObject, float dt)
	{
		Core::Math::Vector3f pos = rObject->GetPosition();
		Core::Math::Vector3f new_pos = pos;
		float phi = dt * 5.f;

		float cos = cosf(phi);
		float sin = sinf(phi);
		// do it with matrix
		new_pos.x = pos.x * cos - pos.y * sin;
		new_pos.y = pos.x * sin + pos.y * cos;

		rObject->SetPosition(new_pos);
	}

	void RenderEngine::OnResize(uint16_t width, uint16_t height)
	{
		m_swapChainWidth = width;
		m_swapChainHeight = height;
		m_rhi->OnResize();
	}
}