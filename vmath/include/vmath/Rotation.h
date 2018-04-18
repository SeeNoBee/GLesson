#ifndef VMATH_ROTATION_H
#define VMATH_ROTATION_H

#include <vmath\Quaternion.h>
#include <vmath\Direction.h>
#include <vmath\Matrix.h>

namespace vmath
{

#include <vmath\PreprocessorTools.h>
FLAG_ENUM(INVERTED)

template <typename Type>
inline typename FloatPointTools<Type>::FloatPointType degree(const Type& deg)
{
	return (3.14159265358979323846 / 180) * deg;
}

template <typename Type>
inline typename FloatPointTools<Type>::FloatPointType deg(const Type& d)
{
	return degree(d);
}

template <bool isDouble>
class Rotation
{
public:
	typedef DoubleType<isDouble> BaseType;
	friend class Rotation<!isDouble>;

	Rotation():
		quaternion() {}

	template<bool anotherIsDouble, typename ScalarType>
	Rotation(const Direction<3, anotherIsDouble> axis, const ScalarType &angle):
		quaternion(axis * FloatPointTools<BaseType>::sin(BaseType(0.5f) * angle), FloatPointTools<BaseType>::cos(BaseType(0.5f) * angle)) {}

	template<bool isDouble1, bool isDouble2>
	Rotation(const Direction<3, isDouble1> &from, const Direction<3, isDouble2> &to)
	{
		Direction<3, isDouble1 || isDouble2> half = from + to;
		quaternion = { cross(from.toVector(), to.toVector()), cos(half, to) };
	}

	template<bool isDouble1, bool isDouble2>
	Rotation(const Direction<3, isDouble1> &to, const Direction<3, isDouble2> &from, Flag_INVERTED flag):
		Rotation(from, to) {}

	inline Quaternion<BaseType> toQuaternion() const
	{
		return quaternion;
	}

	template<typename VectorType>
	inline Vector<RESULT_TYPE(BaseType, *, VectorType), 3>
	operator*(const Vector<VectorType, 3> &vector) const
	{
		return (quaternion * Quaternion<VectorType>(vector) * quaternion.reverse()).vector;
	}

	template<typename VectorType>
	inline Vector<RESULT_TYPE(BaseType, *, VectorType), 4>
	operator*(const Vector<VectorType, 4> &vector) const
	{
		return Vector<RESULT_TYPE(BaseType, *, VectorType), 4>
		{ (*this) * v(vector[xyz]), vector.w };
	}

	template <bool anotherIsDouble>
	inline Rotation<isDouble || anotherIsDouble>
	operator*(const Rotation<anotherIsDouble> &rotation) const
	{
		return quaternion * rotation.quaternion;
	}

	inline Rotation operator-() const
	{
		return Quaternion<BaseType>(quaternion.vector, -quaternion.scalar);
	}

	inline Matrix<BaseType, 3, 3> toMatrix3() const
	{
		float xx = 2 * quaternion.base.x * quaternion.base.x;
		float yy = 2 * quaternion.base.y * quaternion.base.y;
		float zz = 2 * quaternion.base.z * quaternion.base.z;
		float xy = 2 * quaternion.base.x * quaternion.base.y;
		float yz = 2 * quaternion.base.y * quaternion.base.z;
		float xz = 2 * quaternion.base.x * quaternion.base.z;
		float xw = 2 * quaternion.base.x * quaternion.base.w;
		float yw = 2 * quaternion.base.y * quaternion.base.w;
		float zw = 2 * quaternion.base.z * quaternion.base.w;

		Matrix<BaseType, 3, 3> tmp;

		tmp.storage[0][0] = 1 - yy - zz;
		tmp.storage[0][1] = xy + zw;
		tmp.storage[0][2] = xz - yw;

		tmp.storage[1][0] = xy - zw;
		tmp.storage[1][1] = 1 - xx - zz;
		tmp.storage[1][2] = yz + xw;


		tmp.storage[2][0] = xz + yw;
		tmp.storage[2][1] = yz - xw;
		tmp.storage[2][2] = 1 - xx - yy;

		return tmp;
	}

	inline Matrix<BaseType, 4, 4> toMatrix4() const
	{
		return toMatrix3();
	}
private:
	Quaternion<BaseType> quaternion;

	template<typename QuatType>
	Rotation(const Quaternion<QuatType> &quat):
		quaternion(quat) {}
};

typedef Rotation<false> rot;
typedef Rotation<true> drot;

#include <vmath\PreprocessorTools.h>

}

#endif // !VMATH_ROTATION_H

