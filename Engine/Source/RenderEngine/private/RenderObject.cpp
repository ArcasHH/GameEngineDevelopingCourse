#include <RenderObject.h>

namespace GameEngine::Render
{
	RenderObject::RenderObject()
	{
		
	}
	void RenderObject::SetPosition(Core::Math::Vector3f position)
	{
		m_position = position;

	}
	Core::Math::Vector3f RenderObject::GetPosition()
	{
		return m_position;
	}
}