#include <Quaternion.h>

namespace GameEngine
{
	namespace Math
	{
		void RotateVec3(Vector3f& vec, const Vector3f& axis, float angle)
		{
			RotQuaternionf quat(axis, angle);
			vec = quat * vec;
		}
	}
}