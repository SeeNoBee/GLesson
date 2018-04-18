#ifndef VMATH_QUATERNION_H
#define VMATH_QUATERNION_H

#include <vmath\Vector.h>
#include <vmath\PreprocessorTools.h>

namespace vmath
{

template <typename Type>
struct Quaternion
{
	union
	{
		struct
		{
			Type i, j, k, w;
		};

		struct
		{
			Vector<Type, 3> vector;
			Type scalar;
		};

		Vector<Type, 4> base;
	};

	typedef Type BaseType;

	Quaternion():
		base(Type(0), Type(0), Type(0), Type(1)) {}

	template <typename AnotherType>
	Quaternion(const Vector<AnotherType, 4> &vec):
		base(vec) {}

	template <typename AnotherType>
	Quaternion(const Quaternion<AnotherType> &quaternion) :
		base(quaternion.base) {}

	template <typename VectorBaseType, typename ScalarType = int>
	Quaternion(const Vector<VectorBaseType, 3> &vec, const ScalarType &scl = CAST(ScalarType, 0)):
		vector(vec), scalar(scl) {}

	inline Vector<Type, 3> toVector() const
	{
		return vector;
	}

	template <typename AnotherType>
	inline friend Quaternion<RESULT_TYPE(Type, +, AnotherType)>
	operator*(const Quaternion &quaternion, const AnotherType &scl)
	{
		return quaternion.base * scl;
	}

	template <typename AnotherType>
	inline friend Quaternion<RESULT_TYPE(Type, +, AnotherType)>
	operator*(const AnotherType &scl, const Quaternion &quaternion)
	{
		return scl * quaternion.base;
	}

	#define BINARY_OPERATOR(op)\
	template<typename AnotherType>\
	inline Quaternion<RESULT_TYPE(Type, op, AnotherType)>\
	operator op(Quaternion<AnotherType> &quaternion){\
	return base op quaternion.base;}\
	template<typename AnotherType>\
	inline Quaternion&\
	operator op=(Quaternion<AnotherType> &quaternion){\
	base op= quaternion.base;\
	return *this;}

	BINARY_OPERATOR(+)
	BINARY_OPERATOR(-)

	inline Quaternion<SignedType<Type>> conjugate() const
	{
		return { -vector, scalar };
	}

	inline friend Quaternion<SignedType<Type>> conjugate(const Quaternion &quaternion)
	{
		return quaternion.conjugate();
	}

	inline Quaternion<typename FloatPointTools<Type>::FloatPointType> reverse() const
	{
		typedef FloatPointTools<Type>::FloatPointType FloatPointType;
		if (base.qlength() <= std::numeric_limits<FloatPointType>::epsilon())
			return {};
		else
		{
			return (FloatPointType(1) / base.qlength()) * conjugate();
		}
	}

	inline friend Quaternion<typename FloatPointTools<Type>::FloatPointType>
	reverse(const Quaternion &quaternion)
	{
		return quaternion.reverse();
	}

	inline typename FloatPointTools<Type>::FloatPointType length() const
	{
		return base.length();
	}

	inline friend typename FloatPointTools<Type>::FloatPointType
	length(const Quaternion &quaternion)
	{
		return quaternion.length();
	}
};

template <typename Type>
Vector<Type, 3> v(const Quaternion<Type> &quaternion)
{
	return quaternion.vector;
}

template <typename Type1, typename Type2>
Quaternion<SignedType<RESULT_TYPE(Type1, *, Type2)>>
operator*(const Quaternion<Type1> &left, const Quaternion<Type2> &right)
{
	return {
		left.w * right.vector + right.w * left.vector + cross(left.vector, right.vector),
		left.w * right.w - dot(left.vector, right.vector)
	};
}

}

#undef BINARY_OPERATOR
#include <vmath\PreprocessorTools.h>

#endif
