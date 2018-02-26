#pragma once
#include <stdint.h>

namespace MUtility
{
	namespace DataSizes
	{
		const short INT_16_SIZE = sizeof(int16_t);
		const short	INT_32_SIZE = sizeof(int32_t);
		const short	INT_64_SIZE = sizeof(int64_t);

		const short CHAR_SIZE = sizeof(char);
		static_assert(CHAR_SIZE == 1, "Sizeof char must be 1");

		const short SHORT_SIZE = sizeof(short);
		static_assert(SHORT_SIZE == 2, "Sizeof short must be 2");

		const short	INT_SIZE = sizeof(int);
		static_assert(INT_SIZE == 4, "Sizeof int must be 4");

		const short FLOAT_SIZE = sizeof(float);
		static_assert(FLOAT_SIZE == 4, "Sizeof float must be 4");

		const short DOUBLE_SIZE = sizeof(double);
		static_assert(DOUBLE_SIZE == 8, "Sizeof double must be 8");

		const short	BOOL_SIZE = sizeof(bool);
		static_assert(BOOL_SIZE == 1, "Sizeof bool must be 1");
	}
}