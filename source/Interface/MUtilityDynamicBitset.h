#pragma once
#include "MUtilityByte.h"
#include <stdint.h>

// TODODB: Unit test

namespace MUtility
{
	class DynamicBitset
	{
	public:
		DynamicBitset();
		DynamicBitset(uint64_t initialBitSize);
		DynamicBitset(const DynamicBitset& other);
		~DynamicBitset();

		void Reserve(); // Increases the size of the container by a predefined factor
		void Reserve(uint64_t newBitSize); // Will truncate if a parameter is smaller than m_Capacity

		uint64_t CountSetBits() const;
		uint64_t Capacity() const;
		bool Test(uint64_t bitIndex) const;
		bool Any() const;
		bool None() const;
		bool All() const;

		int64_t FindFirstSetBit() const;
		int64_t FindFirstUnsetBit() const;

		int64_t FindFirstSetBitExpand();
		int64_t FindFirstUnsetBitExpand();

		void Set();
		void Set(uint64_t bitIndex, bool value = true);
		void Reset();
		void Reset(uint64_t bitIndex);
		void Flip();
		void Flip(uint64_t bitIndex);

		DynamicBitset& operator= (const DynamicBitset& other);
		bool operator[] (uint64_t bitIndex) const;

	private:
		void Construct(uint64_t initialBitSize);
		void Copy(const DynamicBitset& other);

		Byte* m_Data = nullptr;
		uint64_t m_Capacity = 0;
	};
}