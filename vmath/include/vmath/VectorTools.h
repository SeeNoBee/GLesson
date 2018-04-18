#ifndef VECTOR_TOOLS
#define VECTOR_TOOLS

#include <utility>
#include <type_traits>
#include <limits>

namespace vmath
{ 

template <typename Type>
struct FloatPointTools
{
	typedef float FloatPointType;

	static inline FloatPointType sqrt(FloatPointType value)
	{
		return sqrtf(value);
	}

	static inline FloatPointType sin(FloatPointType value)
	{
		return sinf(value);
	}

	static inline FloatPointType cos(FloatPointType value)
	{
		return cosf(value);
	}

	#ifdef VMATH_NO_FAST_NORMALIZE
	static inline FloatPointType normCoeff(FloatPointType qlength)
	{
		if (qlength <= std::numeric_limits<FloatPointType>::epsilon())
			return 0.f;
		else
			return 1.f / FloatPointTools<Type>::sqrt(qlength);
	}
	#else
	static inline FloatPointType normCoeff(FloatPointType qlength)
	{
		if (qlength <= std::numeric_limits<FloatPointType>::epsilon())
			return 0.f;
		else
		{
			float half = 0.5f * qlength;
			int i = *(int*)&qlength;
			i = 0x5f375a86 - (i >> 1);
			qlength = *(float*)&i;
			qlength *= (1.5f - half * qlength * qlength);
			qlength *= (1.5f - half * qlength * qlength);
			return qlength;
		}
	}
	#endif //VMATH_NO_FAST_NORMALIZE
};

template <>
struct FloatPointTools<double>
{
	typedef double FloatPointType;

	static inline FloatPointType sqrt(FloatPointType value)
	{
		return std::sqrt(value);
	}

	static inline FloatPointType sin(FloatPointType value)
	{
		return std::sin(value);
	}

	static inline FloatPointType cos(FloatPointType value)
	{
		return std::cos(value);
	}

	static inline FloatPointType normCoeff(FloatPointType qlength)
	{
		if (qlength <= std::numeric_limits<FloatPointType>::epsilon())
			return 0.f;
		else
			return 1.0 / FloatPointTools<double>::sqrt(qlength);
	}
};

template <typename Type1, typename Type2>
inline typename std::common_type<Type1, Type2>::type max(const Type1 &left, const Type2 &right)
{
	return (left > right) ? left : right;
}

template <typename Type1, typename Type2>
inline typename std::common_type<Type1, Type2>::type min(const Type1 &left, const Type2 &right)
{
	return (left < right) ? left : right;
}

template <typename Type1, typename Type2>
using DistanceType = decltype(std::declval<Type1>() - std::declval<Type2>());

template <typename Type1, typename Type2>
using QuadDistanceType = decltype(std::declval<DistanceType<Type1, Type2>>() * std::declval<DistanceType<Type1, Type2>>());

template <bool isSigned, typename Type1, typename Type2>
struct DistanceTool
{
	template <typename Type1, typename Type2>
	inline static DistanceType<Type1, Type2> distance(const Type1 &left, const Type2 &right)
	{
		return max(left, right) - min(left, right);
	}

	inline static QuadDistanceType<Type1, Type2> qdistance(const Type1& left, const Type2 &right)
	{
		auto tmp = distance(left, right);
		return tmp * tmp;
	}
};

template <typename Type1, typename Type2>
struct DistanceTool<true, Type1, Type2>
{
	inline static QuadDistanceType<Type1, Type2> qdistance(const Type1& left, const Type2 &right)
	{
		return (left - right) * (left - right);
	}

	inline static DistanceType<Type1, Type2> distance(const Type1& left, const Type2 &right)
	{
		auto tmp = left - right;
		return (tmp < 0) ? -tmp : tmp;
	}
};

template <typename Type1, typename Type2>
inline DistanceType<Type1, Type2> distance(const Type1 &left, const Type2 &right)
{
	return DistanceTool<std::is_signed<DistanceType<Type1, Type2>>::value, Type1, Type2>::distance(left, right);
}

template <typename Type1, typename Type2>
inline QuadDistanceType<Type1, Type2> qdistance(const Type1 &left, const Type2 &right)
{
	return DistanceTool<std::is_signed<DistanceType<Type1, Type2>>::value, Type1, Type2>::qdistance(left, right);
}

template <bool isUnsigned, typename Type>
struct SignedTypeTool
{
	typedef Type SignedType;
};

template <typename Type>
struct SignedTypeTool<true, Type>
{
	typedef typename std::make_signed<Type>::type SignedType;
};

template <typename Type>
using SignedType = typename SignedTypeTool<std::is_unsigned<Type>::value, Type>::SignedType;

template <bool isDouble>
struct DoubleTypeTool
{
	typedef float BaseType;
};

template<>
struct DoubleTypeTool<true>
{
	typedef double BaseType;
};

template <bool isDouble>
using DoubleType = typename DoubleTypeTool<isDouble>::BaseType;


}

#endif VECTOR_TOOLS