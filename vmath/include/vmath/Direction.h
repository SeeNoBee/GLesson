#ifndef VMATH_DIRECTION_H
#define VMATH_DIRECTION_H

#include <vmath\Vector.h>
#include <vmath\PreprocessorTools.h>

namespace vmath
{

template <unsigned int size, bool isDouble = false>
class Direction
{
public:
	typedef DoubleType<isDouble> BaseType;
	typedef Vector<BaseType, size> BaseVectorType;

	friend class Direction<size, !isDouble>;

	template <typename AnotherType>
	Direction(const Vector<AnotherType, size> &vector) :
		base(BaseVectorType(vector).normalize()) {}

	Direction():
		base(0) {}

	inline operator BaseVectorType() const
	{
		return base;
	}

	inline const BaseVectorType& toVector() const
	{
		return base;
	}

	inline operator bool() const
	{
		return base.qlength() != BaseType(0);
	}

	inline Direction operator-() const
	{
		return -base;
	}

	#define BINARY_OPERATOR(op)\
	template <bool anotherIsDouble>\
	inline Direction<size, isDouble || anotherIsDouble>\
	operator op(const Direction<size, anotherIsDouble> &direction) const{\
	return base op direction.base;}\
	template <bool anotherIsDouble>\
	inline Direction&\
	operator op=(const Direction<size, anotherIsDouble> &direction){\
	base op=direction.base;\
	return *this;}

	BINARY_OPERATOR(+)
	BINARY_OPERATOR(-)

	template <bool anotherIsDouble>
	bool operator==(const Direction<anotherIsDouble> &direction) const
	{
		return (DoubleType<isDouble || anotherIsDouble>(1) - cos(*this, direction)) <=
			std::numeric_limits<DoubleType<isDouble || anotherIsDouble>>::epsilon();
	}

	template <bool anotherIsDouble>
	bool operator!=(const Direction<anotherIsDouble> &direction) const
	{
		return !((*this) == direction);
	}

	#define VECTOR_BINARY_OPERATOR(op)\
	template<typename Type>\
	inline friend Vector<RESULT_TYPE(Type, op, BaseType), size>\
	operator op(const Vector<Type, size> &vector, const Direction &direction){\
	return vector op direction.base;}\
	template<typename Type>\
	inline friend Vector<RESULT_TYPE(Type, op, BaseType), size>\
	operator op(const Direction &direction, const Vector<Type, size> &vector){\
	return direction.base op vector;}

	VECTOR_BINARY_OPERATOR(+)
	VECTOR_BINARY_OPERATOR(-)
	VECTOR_BINARY_OPERATOR(*)
	VECTOR_BINARY_OPERATOR(/)

	template <typename Type>
	inline friend Vector<RESULT_TYPE(Type, *, BaseType), size>
	operator*(const Type &scalar, const Direction &direction)
	{
		return scalar * direction.base;
	}

	template <typename Type>
	inline friend Vector<RESULT_TYPE(Type, *, BaseType), size>
	operator*(const Direction &direction, const Type &scalar)
	{
		return direction.base * scalar;
	}

private:
	Vector<BaseType, size> base;
};

template <unsigned int size, bool isDouble1, bool isDouble2>
inline DoubleType<isDouble1 || isDouble2>
cos(const Direction<size, isDouble1> &direction1, const Direction<size, isDouble2> &direction2)
{
	return dot(direction1.toVector(), direction2.toVector());
}

template <bool isDouble1, bool isDouble2>
inline Direction<3, isDouble1 || isDouble2>
cross(const Direction<3, isDouble1> &direction1, const Direction<3, isDouble2> &direction2)
{
	return cross(direction1.toVector(), direction2.toVector());
}

template <bool isDouble1, bool isDouble2>
inline DoubleType<isDouble1 || isDouble2>
sin(const Direction<3, isDouble1> &direction1, const Direction<3, isDouble2> &direction2)
{
	return cross(direction1.toVector(), direction2.toVector()).length();
}

template<unsigned int size, bool isDouble>
inline typename Direction<size, isDouble>::BaseVectorType v(const Direction<size, isDouble> &direction)
{
	return direction.toVector();
}

typedef Direction<2> dir2;
typedef Direction<3> dir3;
typedef Direction<4> dir4;

typedef Direction<2, true> ddir2;
typedef Direction<3, true> ddir3;
typedef Direction<4, true> ddir4;

}

#undef VECTOR_BINARY_OPERATOR
#undef BINARY_OPERATOR
#include <vmath\PreprocessorTools.h>

#endif // !VMATH_DIRECTION_H