#pragma once

#include <Math/export.h>
#include <Vector.h>
#include <Matrix.h>

namespace GameEngine
{
	namespace Math
	{
		template <typename T> requires std::is_arithmetic_v<T>
		class Quaternion
		{
		protected:
			Vector3<T> vec{};
			T scalar{};			
		public:
			constexpr Quaternion() = default;
			explicit constexpr Quaternion(const Vector3<T>& vector, T scalar) : vec(vector), scalar(scalar) {};
			explicit constexpr Quaternion(T x, T y, T z, T scalar) : Quaternion(Vector3<T>(x, y, z), scalar) {};

			Quaternion(const Quaternion&) = default;
			Quaternion& operator=(const Quaternion&) = default;
			Quaternion(Quaternion&&) = default;
			Quaternion& operator=(Quaternion&&) = default;

			constexpr Vector3<T> GetVector() const noexcept { return vec; }
			constexpr T GetScalar() const noexcept { return scalar; }

			static constexpr Quaternion Identity() noexcept 
			{
				return Quaternion(Vector3<T>::Zero(), 1);
			}

			constexpr Quaternion Conjugated() const noexcept 
			{
				return Quaternion(-vec, scalar);
			}
			constexpr Quaternion Inversed() const noexcept
			{
				assert(GetLengthSquared() > std::numeric_limits<T>::epsilon());
				return Conjugated() / GetLengthSquared();
			}

			constexpr Quaternion& operator*=(const Quaternion& other) noexcept
			{
				T new_scalar = scalar * other.scalar - vec * other.vec;
				Vector3<T> new_vec = (other.vec * scalar) + (vec * other.scalar) + vec.CrossProduct(other.vec);
				*this = Quaternion(new_vec, new_scalar);
				return *this;
			}
			constexpr Quaternion operator*(const Quaternion& other) const noexcept
			{
				Quaternion result = *this;
				result *= other;
				return result;
			}

			constexpr Quaternion& operator+=(const Quaternion& other) noexcept
			{
				vec = vec + other.vec;
				scalar += other.scalar;
				return *this;
			}
			constexpr Quaternion operator+(const Quaternion& other) const noexcept
			{
				Quaternion result = *this;
				result += other;
				return result;
			}

			constexpr Quaternion& operator-=(const Quaternion& other) noexcept
			{
				vec = vec - other.vec;
				scalar -= other.scalar;
				return *this;
			}
			constexpr Quaternion operator-(const Quaternion& other) const noexcept
			{
				Quaternion result = *this;
				result -= other;
				return result;
			}

			constexpr Quaternion& operator*=(T s) noexcept
			{
				vec = vec * s;
				scalar *= s;
				return *this;
			}
			constexpr Quaternion operator*(T s) const noexcept
			{
				Quaternion result = *this;
				result *= s;
				return result;
			}
			
			constexpr Quaternion operator/(T s) const noexcept
			{
				return *this * (1 / s);
			}

			constexpr Quaternion operator-() const noexcept
			{
				Quaternion result;
				result.vec = -vec;
				result.scalar = -scalar;
				return result;
			}

			constexpr bool operator!=(const Quaternion& other) const noexcept
			{
				return (vec != other.vec || scalar != other.scalar);
			}
			constexpr bool operator==(const Quaternion& other) const noexcept
			{
				return !(*this != other);
			}

			constexpr T GetLengthSquared() const noexcept 
			{
				return dot(*this);
			}
			constexpr T GetLength() const noexcept 
			{
				return std::sqrt(GetLengthSquared());
			}

			constexpr Quaternion Normalized() const noexcept 
			{
				assert(GetLengthSquared() > std::numeric_limits<T>::epsilon());
				return *this / GetLength();
			}
			constexpr bool IsNormalized() const noexcept 
			{
				return std::abs(GetLength() - 1) < std::numeric_limits<T>::epsilon();
			}

			constexpr T dot(const Quaternion& other) const noexcept 
			{
				return vec * other.vec + scalar * other.scalar;
			}
		};

		template <typename T> requires std::is_arithmetic_v<T>
		class RotQuaternion final : private Quaternion<T>
		{		
		public:
			constexpr RotQuaternion() = default;
			explicit constexpr RotQuaternion(const Vector3<T>& vector, T angle)
				: Quaternion<T>(vector.Normalized() * std::sin(angle * 0.5), std::cos(angle * 0.5))
			{}
			explicit constexpr RotQuaternion(Quaternion<T> other) : Quaternion<T>{ std::move(other) } {
				assert(this->IsNormalized());
			}
			explicit constexpr RotQuaternion(const Vector3<T>& vector) : Quaternion<T>::Quaternion(vector.Normalized(), 0) {};
			explicit constexpr RotQuaternion(T x, T y, T z, T angle) : RotQuaternion(Vector3<T>(x, y, z), angle) {};
			explicit constexpr RotQuaternion(T roll, T pitch, T yaw)
			{
				T cr = std::cos(roll * 0.5);
				T sr = std::sin(roll * 0.5);
				T cp = std::cos(pitch * 0.5);
				T sp = std::sin(pitch * 0.5);
				T cy = std::cos(yaw * 0.5);
				T sy = std::sin(yaw * 0.5);

				this->vec.x = sr * cp * cy - cr * sp * sy;
				this->vec.y = cr * sp * cy + sr * cp * sy;
				this->vec.z = cr * cp * sy - sr * sp * cy;
				this->scalar = cr * cp * cy + sr * sp * sy;
			}

			constexpr RotQuaternion Inversed() const noexcept 
			{
				return RotQuaternion(Quaternion<T>::Conjugated());
			}

			constexpr Vector3<T> operator*(const Vector3<T>& vector) const noexcept
			{
				Quaternion<T> v(vector, 0);
				const Quaternion<T>& this_quat = *this;
				return (this_quat * v * Inversed()).GetVector();
			}
			constexpr RotQuaternion& operator*=(const RotQuaternion& other) noexcept 
			{
				Quaternion<T>::operator*=(other);
				return *this;
			}
			constexpr RotQuaternion operator*(const RotQuaternion& other) const noexcept 
			{
				return RotQuaternion(Quaternion<T>::operator*(other));
			}

			[[nodiscard]] constexpr Matrix<T, 4, 4> CreateRotationMatrix() const noexcept
			{
				Vector3f vec = this->vec;
				T scalar = this->scalar;

				T xx2 = 2 * vec.x * vec.x;
				T yy2 = 2 * vec.y * vec.y;
				T zz2 = 2 * vec.z * vec.z;
				T sx2 = 2 * scalar * vec.x;
				T sy2 = 2 * scalar * vec.y;
				T sz2 = 2 * scalar * vec.z;
				T xy2 = 2 * vec.x * vec.y;
				T xz2 = 2 * vec.x * vec.z;
				T yz2 = 2 * vec.y * vec.z;

				Matrix<T, 4, 4> rotationMatrix = Math::Matrix<T,4,4>:: Identity();
			
				rotationMatrix.SetElement(1 - (yy2 + zz2), 0, 0);
				rotationMatrix.SetElement(xy2 - sz2, 1, 0);
				rotationMatrix.SetElement(xz2 + sy2, 2, 0);

				rotationMatrix.SetElement(xy2 + sz2, 0, 1);
				rotationMatrix.SetElement(1 - (xx2 + zz2), 1, 1);
				rotationMatrix.SetElement(yz2 - sx2, 2, 1);

				rotationMatrix.SetElement(xz2 - sy2, 0, 2);
				rotationMatrix.SetElement(yz2 + sx2, 1, 2);
				rotationMatrix.SetElement(1 - (xx2 + yy2), 2, 2);

				return rotationMatrix;
			}
		};

		using Quaternionf = Quaternion<float>;
		using RotQuaternionf = RotQuaternion<float>;

		MATH_API void RotateVec3(Vector3f& vec, const Vector3f& axis, float angle);
	}
}