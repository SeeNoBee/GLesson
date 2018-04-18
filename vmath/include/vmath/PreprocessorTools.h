#ifndef PREPROCESSOR_TOOLS
#define PREPROCESSOR_TOOLS

#define CONCAT__(x, y) x ## y
#define CONCAT(x, y) CONCAT__(x, y)
#define STR__(x) #x
#define STR(x) STR__(x)
#define RESULT_TYPE(Type1, operator, Type2) decltype(std::declval<Type1>() operator std::declval<Type2>())
#define COMMA ,
#define CAST(Type, value) Type(value)
#define FLAG_ENUM(name) enum class CONCAT(Flag_, name){name};\
const CONCAT(Flag_, name) name = CONCAT(Flag_, name)::name;

#ifdef EXPAND_SIZE

#if (EXPAND_SIZE == 0)
	//TODO:
	#error "Size of 'Vector' can't be less than zero"
#elif (EXPAND_SIZE == 1)
	#define EXPAND(x, y, z, w) x
#elif (EXPAND_SIZE == 2)
	#define EXPAND(x, y, z, w) x y
#elif (EXPAND_SIZE == 3)
	#define EXPAND(x, y, z, w) x y z
#elif (EXPAND_SIZE == 4)
	#define EXPAND(x, y, z, w) x y z w
#elif (EXPAND_SIZE > 4)
	#error "EXPAND_SIZE can't be greather than 4"
#elif
	#error "EXPAND_SIZE must be a number"
#endif

#define STATEMENTS(x, y, z, w) EXPAND(x;, y;, z;, w;)
#define FIELDS(prefix, x, y, z, w) STATEMENTS(prefix x, prefix y, prefix z, prefix w)
#define OPERANDS(prefix, x, y, z, w) EXPAND(prefix x, COMMA prefix y, COMMA prefix z, COMMA prefix w)

#endif // EXPAND_SIZE

#else

#undef PREPROCESSOR_TOOLS

#undef CONCAT__
#undef CONCAT
#undef STR__
#undef STR
#undef RESULT_TYPE
#undef COMMA
#undef CAST
#undef FLAG_ENUM

#ifdef EXPAND_SIZE
#undef EXPAND_SIZE
#undef EXPAND

#undef STATEMENTS
#undef FIELDS
#undef OPERANDS

#endif //EXPAND_SIZE

#endif // VECTOR_PREPROCESSOR_TOOLS
