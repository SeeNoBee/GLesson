#ifndef SWIZZLING_TYPES_H
#define SWIZZLING_TYPES_H

#define CONCAT__(a, b) a ## b
#define CONCAT(a, b) CONCAT__(a, b)


struct SwizzlingTools
{
	static constexpr unsigned int size(unsigned int swizzling, unsigned int currentSize = 0)
	{
		return (swizzling == 0) ? currentSize : size(swizzling / 10, currentSize + 1);
	}

	static constexpr unsigned int mask(unsigned int index, unsigned int currentMask = 1)
	{
		return (index == 0) ? currentMask : mask(index - 1, currentMask * 10);
	}

	static constexpr unsigned int elementMult(unsigned int swizzling, unsigned int currentMult = 1)
	{
		return (swizzling == 0) ? currentMult : elementMult(swizzling / 10, currentMult * (swizzling % 10));
	}

	static constexpr bool hasRepeates(unsigned int swizzling)
	{
		unsigned int mult = elementMult(swizzling);
		return (mult % (4 * 4) == 0) || (mult % (5 * 5) == 0) || (mult % (6 * 6) == 0) || (mult % (7 * 7) == 0);
	}

	static constexpr unsigned int index(unsigned int swizzling, unsigned int index)
	{
		return ((swizzling / mask(index % size(swizzling))) % 10) - 4;
	}
};

#define SWIZZLING_TYPE(name, var1, var2, var3, conv)\
struct name {\
static constexpr unsigned int convolution = conv;\
};\
const name var1;\
const name var2;\
const name var3;

#define COMBINATIONS1(name, var1, var2, var3, number)\
SWIZZLING_TYPE(CONCAT(X, name), CONCAT(x, var1), CONCAT(r, var2), CONCAT(s, var3), CONCAT(number, 4))\
SWIZZLING_TYPE(CONCAT(Y, name), CONCAT(y, var1), CONCAT(g, var2), CONCAT(t, var3), CONCAT(number, 5))\
SWIZZLING_TYPE(CONCAT(Z, name), CONCAT(z, var1), CONCAT(b, var2), CONCAT(p, var3), CONCAT(number, 6))\
SWIZZLING_TYPE(CONCAT(W, name), CONCAT(w, var1), CONCAT(a, var2), CONCAT(q, var3), CONCAT(number, 7))

#define COMBINATIONS2(name, var1, var2, var3, number)\
COMBINATIONS1(CONCAT(X, name), CONCAT(x, var1), CONCAT(r, var2), CONCAT(s, var3), CONCAT(number, 4))\
COMBINATIONS1(CONCAT(Y, name), CONCAT(y, var1), CONCAT(g, var2), CONCAT(t, var3), CONCAT(number, 5))\
COMBINATIONS1(CONCAT(Z, name), CONCAT(z, var1), CONCAT(b, var2), CONCAT(p, var3), CONCAT(number, 6))\
COMBINATIONS1(CONCAT(W, name), CONCAT(w, var1), CONCAT(a, var2), CONCAT(q, var3), CONCAT(number, 7))

#define COMBINATIONS3(name, var1, var2, var3, number)\
COMBINATIONS2(CONCAT(X, name), CONCAT(x, var1), CONCAT(r, var2), CONCAT(s, var3), CONCAT(number, 4))\
COMBINATIONS2(CONCAT(Y, name), CONCAT(y, var1), CONCAT(g, var2), CONCAT(t, var3), CONCAT(number, 5))\
COMBINATIONS2(CONCAT(Z, name), CONCAT(z, var1), CONCAT(b, var2), CONCAT(p, var3), CONCAT(number, 6))\
COMBINATIONS2(CONCAT(W, name), CONCAT(w, var1), CONCAT(a, var2), CONCAT(q, var3), CONCAT(number, 7))

#define COMBINATIONS4(name, var1, var2, var3, number)\
COMBINATIONS3(CONCAT(X, name), CONCAT(x, var1), CONCAT(r, var2), CONCAT(s, var3), CONCAT(number, 4))\
COMBINATIONS3(CONCAT(Y, name), CONCAT(y, var1), CONCAT(g, var2), CONCAT(t, var3), CONCAT(number, 5))\
COMBINATIONS3(CONCAT(Z, name), CONCAT(z, var1), CONCAT(b, var2), CONCAT(p, var3), CONCAT(number, 6))\
COMBINATIONS3(CONCAT(W, name), CONCAT(w, var1), CONCAT(a, var2), CONCAT(q, var3), CONCAT(number, 7))

COMBINATIONS1( , , , , )
COMBINATIONS2( , , , , )
COMBINATIONS3( , , , , )
COMBINATIONS4( , , , , )

#undef CONCAT__
#undef CONCAT
#undef SWIZZLING_TYPE
#undef COMBINATIONS1
#undef COMBINATIONS2
#undef COMBINATIONS3
#undef COMBINATIONS4

#endif // SWIZZLING_TYPES_H
