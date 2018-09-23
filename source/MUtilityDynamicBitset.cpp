#include "Interface/MUtilityDynamicBitset.h"
#include "Interface/MUtilityIntrinsics.h"
#include <cassert>
#include <cstring>

using namespace MUtility;

constexpr uint32_t DYNAMIC_BITSET_DEFAULT_SIZE = 64;
constexpr int32_t EXAPNSION_FACTOR = 2;

// ---------- PUBLIC ----------

DynamicBitset::DynamicBitset()
{
	Construct(DYNAMIC_BITSET_DEFAULT_SIZE);
}

DynamicBitset::DynamicBitset(uint64_t initialBitSize)
{
	Construct(initialBitSize);
}

DynamicBitset::DynamicBitset(const DynamicBitset& other)
{
	Copy(other);
}

DynamicBitset::~DynamicBitset()
{
	delete[] m_Data;
}

void DynamicBitset::Reserve()
{
	Reserve(m_Capacity * EXAPNSION_FACTOR);
}

void DynamicBitset::Reserve(uint64_t newBitSize)
{
	newBitSize += (newBitSize % BITS_PER_BYTE); // Force align to byte size
	const uint32_t newByteSize = static_cast<uint32_t>(newBitSize / BITS_PER_BYTE);
	const uint32_t oldByteSize = static_cast<uint32_t>(m_Capacity / BITS_PER_BYTE);

	Byte* swap = m_Data;
	m_Data = new Byte[newByteSize];
	memcpy(m_Data, swap, oldByteSize);
	memset(m_Data + oldByteSize, 0, newByteSize - oldByteSize);
	m_Capacity = newBitSize;
	delete[] swap;
}

uint64_t DynamicBitset::CountSetBits() const
{
	uint64_t toReturn = 0;
	for (int i = 0; i <= static_cast<int>(m_Capacity / BITS_PER_BYTE / sizeof(unsigned long long)); ++i) // Capacity is aligned with byte size // TODODB: Do we risk not checking the last bytes here if they do not make up a full unsigned long long size?
	{
		toReturn += static_cast<uint64_t>(PopCount(*reinterpret_cast<unsigned long long*>(m_Data + (i * sizeof(unsigned long long)))));
	}
	return toReturn;
}

uint64_t DynamicBitset::Capacity() const
{
	return m_Capacity;
}

bool DynamicBitset::Test(uint64_t bitIndex) const
{
	assert(bitIndex < m_Capacity && "Accessing out of range bit index");
	return TestBit(*reinterpret_cast<int64_t*>(m_Data + (bitIndex / BITS_PER_BYTE)), bitIndex % sizeof(int64_t)); // Handle data as an int since intrinsics cannot handle arbitrary data sizes // TODODB: Make splits that are aligned to 64bits (Current version is unaligned)
}

bool DynamicBitset::Any() const
{
	return CountSetBits() > 0;
}

bool DynamicBitset::None() const
{
	return CountSetBits() == 0;
}

bool DynamicBitset::All() const
{
	return CountSetBits() == m_Capacity;
}

int64_t DynamicBitset::FindFirstSetBit() const
{
	unsigned long toReturn = -1;
	for (int i = 0; i <= static_cast<int>(m_Capacity / BITS_PER_BYTE / sizeof(unsigned long long)); ++i) // TODODB: Do we risk not checking the last bytes here if they do not make up a full unsigned long long size?
	{
		if (BitscanForward(*reinterpret_cast<unsigned long long*>(m_Data + (i * sizeof(unsigned long long))), &toReturn))
		{
			toReturn += i * sizeof(unsigned long long) * BITS_PER_BYTE;

			if (toReturn > m_Capacity)
				toReturn = -1;

			break;
		}
	}
	return static_cast<int64_t>(toReturn);
}

int64_t DynamicBitset::FindFirstUnsetBit() const
{
	unsigned long toReturn = -1;
	for (int i = 0; i <= static_cast<int>(m_Capacity / BITS_PER_BYTE / sizeof(unsigned long long)); ++i) // TODODB: Do we risk not checking the last bytes here if they do not make up a full unsigned long long size?
	{
		if (InverseBitscanForward(*reinterpret_cast<unsigned long long*>(m_Data + (i * sizeof(unsigned long long))), &toReturn))
		{
			toReturn += i * sizeof(unsigned long long) * BITS_PER_BYTE;

			if (toReturn > m_Capacity)
				toReturn = -1;

			break;
		}
	}
	return toReturn;
}

int64_t DynamicBitset::FindFirstSetBitExpand()
{
	int64_t toReturn = FindFirstSetBit();
	if (toReturn == -1)
	{
		Reserve();
		toReturn = FindFirstSetBit();
	}
	return toReturn;
}

int64_t DynamicBitset::FindFirstUnsetBitExpand()
{
	int64_t toReturn = FindFirstUnsetBit();
	if (toReturn == -1)
	{
		Reserve();
		toReturn = FindFirstUnsetBit();
	}
	return toReturn;
}

void DynamicBitset::Set()
{
	memset(m_Data, ~static_cast<char>(0), m_Capacity / BITS_PER_BYTE);
}

void DynamicBitset::Set(uint64_t bitIndex, bool value)
{
	if(bitIndex >= m_Capacity)
		Reserve();

	if(value)
		SetBit(*reinterpret_cast<int64_t*>(m_Data + (bitIndex / BITS_PER_BYTE)), bitIndex % sizeof(int64_t));
	else
		ResetBit(*reinterpret_cast<int64_t*>(m_Data + (bitIndex / BITS_PER_BYTE)), bitIndex % sizeof(int64_t));
}

void DynamicBitset::Reset()
{
	memset(m_Data, 0, m_Capacity / BITS_PER_BYTE);
}

void DynamicBitset::Reset(uint64_t bitIndex)
{
	if(bitIndex >= m_Capacity)
		Reserve();

	ResetBit(*reinterpret_cast<int64_t*>(m_Data + (bitIndex / sizeof(int64_t))), bitIndex % sizeof(int64_t));
}

void DynamicBitset::Flip()
{
	for (int i = 0; i <= static_cast<int>(m_Capacity / BITS_PER_BYTE); ++i) // Capacity is aligned with byte size
	{
		Byte* startBytePos = m_Data + i * sizeof(uint64_t);
		*reinterpret_cast<uint64_t*>(startBytePos) = ~static_cast<uint64_t>(*reinterpret_cast<uint64_t*>(startBytePos));
	}
}

void DynamicBitset::Flip(uint64_t bitIndex)
{
	if (bitIndex >= m_Capacity)
		Reserve();

	*reinterpret_cast<uint32_t*>(m_Data + bitIndex / BITS_PER_BYTE) ^= (1ULL << (bitIndex % ((sizeof(uint32_t) * BITS_PER_BYTE))));
}

DynamicBitset& DynamicBitset::operator= (const DynamicBitset& other)
{
	if (&other == this)
		return *this;

	Copy(other);

	return *this;
}

bool DynamicBitset::operator[] (uint64_t bitIndex) const
{
	return Test(bitIndex);
}

// ---------- Private ----------

void DynamicBitset::Construct(uint64_t initialBitSize)
{
	initialBitSize += (initialBitSize % BITS_PER_BYTE); // Force align to byte size

	m_Data = new Byte[initialBitSize / BITS_PER_BYTE];
	memset(m_Data, 0, initialBitSize / BITS_PER_BYTE);
	m_Capacity = initialBitSize;
}

void DynamicBitset::Copy(const DynamicBitset& other)
{
	m_Data = new Byte[other.m_Capacity / BITS_PER_BYTE];
	memcpy(m_Data, other.m_Data, other.m_Capacity / BITS_PER_BYTE);
	m_Capacity = other.m_Capacity;
}