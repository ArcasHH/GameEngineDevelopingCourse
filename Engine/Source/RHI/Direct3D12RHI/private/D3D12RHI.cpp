#include <array.h>
#include <D3D12Mesh.h>
#include <D3D12RHI.h>
#include <D3D12RHIPrivate.h>
#include <D3D12Material.h>
#include <Math/Vector.h>

namespace GameEngine
{
	namespace Render::HAL
	{
		using namespace Core;

		D3D12RHI::D3D12RHI() :
			m_d3d12Private(std::make_shared<D3D12RHIPrivate>())
		{
			m_d3d12Private->EnableDebugLayer();
		}

		void D3D12RHI::Init()
		{
			m_d3d12Private->Init();
		}

		void D3D12RHI::Update(Mesh::Ptr mesh, Material::Ptr material, Core::Math::Vector3f position, size_t dt)
		{
			m_d3d12Private->Update(mesh, material, position, dt);
		}

		Mesh::Ptr D3D12RHI::CreateBoxMesh()
		{
			array<Vertex, 8> vertices =
			{
				Vertex({ Math::Vector3f(-1.0f, -1.0f, -1.0f), Math::Vector4f((float*)&DirectX::Colors::White) }),
				Vertex({ Math::Vector3f(-1.0f, +1.0f, -1.0f), Math::Vector4f((float*)&DirectX::Colors::Black) }),
				Vertex({ Math::Vector3f(+1.0f, +1.0f, -1.0f), Math::Vector4f((float*)&DirectX::Colors::Red) }),
				Vertex({ Math::Vector3f(+1.0f, -1.0f, -1.0f), Math::Vector4f((float*)&DirectX::Colors::Green) }),
				Vertex({ Math::Vector3f(-1.0f, -1.0f, +1.0f), Math::Vector4f((float*)&DirectX::Colors::Blue) }),
				Vertex({ Math::Vector3f(-1.0f, +1.0f, +1.0f), Math::Vector4f((float*)&DirectX::Colors::Yellow) }),
				Vertex({ Math::Vector3f(+1.0f, +1.0f, +1.0f), Math::Vector4f((float*)&DirectX::Colors::Cyan) }),
				Vertex({ Math::Vector3f(+1.0f, -1.0f, +1.0f), Math::Vector4f((float*)&DirectX::Colors::Magenta) }),
			};

			array<uint16_t, 36> indices =
			{
				// front face
				0, 1, 2,
				0, 2, 3,

				// back face
				4, 6, 5,
				4, 7, 6,

				// left face
				4, 5, 1,
				4, 1, 0,

				// right face
				3, 2, 6,
				3, 6, 7,

				// top face
				1, 5, 6,
				1, 6, 2,

				// bottom face
				4, 0, 3,
				4, 3, 7

			};

			return m_d3d12Private->CreateMesh(vertices.begin(), vertices.size(), sizeof(Vertex), indices.begin(), indices.size(), sizeof(uint16_t));
		}
		Mesh::Ptr D3D12RHI::CreateMyMesh()
		{
			array<Vertex, 14> vertices =
			{
				Vertex({ Math::Vector3f(-1.0f, -1.0f, -1.0f), Math::Vector4f((float*)&DirectX::Colors::Orange) }),
				Vertex({ Math::Vector3f(-1.0f, +1.0f, -1.0f), Math::Vector4f((float*)&DirectX::Colors::Purple) }),
				Vertex({ Math::Vector3f(+1.0f, +1.0f, -1.0f), Math::Vector4f((float*)&DirectX::Colors::Red) }),
				Vertex({ Math::Vector3f(+1.0f, -1.0f, -1.0f), Math::Vector4f((float*)&DirectX::Colors::LightGreen) }),
				Vertex({ Math::Vector3f(-1.0f, -1.0f, +1.0f), Math::Vector4f((float*)&DirectX::Colors::Blue) }),
				Vertex({ Math::Vector3f(-1.0f, +1.0f, +1.0f), Math::Vector4f((float*)&DirectX::Colors::Yellow) }),
				Vertex({ Math::Vector3f(+1.0f, +1.0f, +1.0f), Math::Vector4f((float*)&DirectX::Colors::Cyan) }),
				Vertex({ Math::Vector3f(+1.0f, -1.0f, +1.0f), Math::Vector4f((float*)&DirectX::Colors::Pink) }),

				Vertex({ Math::Vector3f(0.0f, 0.0f, -1.0f), Math::Vector4f((float*)&DirectX::Colors::Black) }),
				Vertex({ Math::Vector3f(0.0f, 0.0f, 1.0f), Math::Vector4f((float*)&DirectX::Colors::Black) }),
				Vertex({ Math::Vector3f(-1.0f, 0.0f, 0.0f), Math::Vector4f((float*)&DirectX::Colors::Black) }),
				Vertex({ Math::Vector3f(1.0f, 0.0f, 0.0f), Math::Vector4f((float*)&DirectX::Colors::Black) }),
				Vertex({ Math::Vector3f(0.0f, -1.0f, 0.0f), Math::Vector4f((float*)&DirectX::Colors::Black) }),
				Vertex({ Math::Vector3f(0.0f, 1.0f, 0.0f), Math::Vector4f((float*)&DirectX::Colors::Black) })
			};

			array<uint16_t, 72> indices =
			{
				0, 8, 12,
				0, 12, 10,
				0, 10, 8,

				1, 8, 10,
				1, 10, 13,
				1, 13, 8,

				2, 8, 13,
				2, 13, 11,
				2, 11, 8,

				3, 8, 11,
				3, 11, 12,
				3, 12, 8,

				4, 9, 10,
				4, 10, 12,
				4, 12, 9,

				5, 9, 13,
				5, 13, 10,
				5, 10, 9,

				6, 9, 11,
				6, 11, 13,
				6, 13, 9,

				7, 9, 12,
				7, 12, 11,
				7, 11, 9
			};

			return m_d3d12Private->CreateMesh(vertices.begin(), vertices.size(), sizeof(Vertex), indices.begin(), indices.size(), sizeof(uint16_t));
		}

		Material::Ptr D3D12RHI::GetMaterial(const std::string& name)
		{
			return m_d3d12Private->GetMaterial(name);
		}

		void D3D12RHI::ExecuteCommandLists()
		{
			m_d3d12Private->ExecuteCommandLists();
		}

		void D3D12RHI::Flush()
		{
			m_d3d12Private->FlushCommandQueue();
		}

		void D3D12RHI::OnResize()
		{
			m_d3d12Private->OnResize();
		}
	}
}