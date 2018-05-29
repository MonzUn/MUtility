#pragma once
#include "MUtilityByte.h"
#include <string>
#include <stdint.h>

// Use in header files after bitflag declaration
#define CREATE_BITFLAG_OPERATOR_SIGNATURES(BitFlagType) \
BitFlagType operator|(const BitFlagType& lhs, const BitFlagType& rhs);\
BitFlagType operator&(const BitFlagType& lhs, const BitFlagType& rhs);\
BitFlagType& operator|=(BitFlagType& lhs, const BitFlagType& rhs);\
BitFlagType& operator&=(BitFlagType& lhs, const BitFlagType& rhs);\
bool operator==(const BitFlagType& lhs, const std::underlying_type_t<BitFlagType>& rhs);\
bool operator!=(const BitFlagType& lhs, const std::underlying_type_t<BitFlagType>& rhs);\
bool operator==(const std::underlying_type_t<BitFlagType>& lhs, const BitFlagType& rhs);\
bool operator!=(const std::underlying_type_t<BitFlagType>& lhs, const BitFlagType& rhs)

// Use in .cpp files if CREATE_BITFLAG_OPERATOR_SIGNATURES has been used in the header file and the BitFlag definition is located in a namespace
#define CREATE_NAMESPACED_BITFLAG_OPERATOR_DEFINITIONS(Namespace, BitFlagType) \
Namespace::BitFlagType Namespace::operator|(const Namespace::BitFlagType& lhs, const Namespace::BitFlagType& rhs) { return static_cast<const Namespace::BitFlagType>(static_cast<const std::underlying_type_t<Namespace::BitFlagType>>(lhs) | static_cast<const std::underlying_type_t<Namespace::BitFlagType>>(rhs)); }\
Namespace::BitFlagType Namespace::operator&(const Namespace::BitFlagType& lhs, const Namespace::BitFlagType& rhs) { return static_cast<const Namespace::BitFlagType>(static_cast<const std::underlying_type_t<Namespace::BitFlagType>>(lhs) & static_cast<const std::underlying_type_t<Namespace::BitFlagType>>(rhs)); }\
Namespace::BitFlagType& Namespace::operator|=(Namespace::BitFlagType& lhs, const Namespace::BitFlagType& rhs) { return lhs = (lhs | rhs); }\
Namespace::BitFlagType& Namespace::operator&=(Namespace::BitFlagType& lhs, const Namespace::BitFlagType& rhs) { return lhs = (lhs & rhs); }\
bool Namespace::operator==(const Namespace::BitFlagType& lhs, const std::underlying_type_t<Namespace::BitFlagType>& rhs) { return static_cast<const std::underlying_type_t<Namespace::BitFlagType>>(lhs) == rhs; };\
bool Namespace::operator!=(const Namespace::BitFlagType& lhs, const std::underlying_type_t<Namespace::BitFlagType>& rhs) { return static_cast<const std::underlying_type_t<Namespace::BitFlagType>>(lhs) != rhs; };\
bool Namespace::operator==(const std::underlying_type_t<Namespace::BitFlagType>& lhs, const Namespace::BitFlagType& rhs) { return lhs == static_cast<const std::underlying_type_t<Namespace::BitFlagType>>(rhs); };\
bool Namespace::operator!=(const std::underlying_type_t<Namespace::BitFlagType>& lhs, const Namespace::BitFlagType& rhs) { return lhs != static_cast<const std::underlying_type_t<Namespace::BitFlagType>>(rhs); }

// Use in .cpp files if CREATE_BITFLAG_OPERATOR_SIGNATURES has been used in the header file and the BitFlag definition is not located in a namespace
#define CREATE_BITFLAG_OPERATOR_DEFINITIONS(BitFlagType) \
BitFlagType operator|(const BitFlagType& lhs, const BitFlagType& rhs) { return static_cast<const BitFlagType>(static_cast<const std::underlying_type_t<BitFlagType>>(lhs) | static_cast<const std::underlying_type_t<BitFlagType>>(rhs)); }\
BitFlagType operator&(const BitFlagType& lhs, const BitFlagType& rhs) { return static_cast<const BitFlagType>(static_cast<const std::underlying_type_t<BitFlagType>>(lhs) & static_cast<const std::underlying_type_t<BitFlagType>>(rhs)); }\
BitFlagType& operator|=(BitFlagType& lhs, const BitFlagType& rhs) { return lhs = (lhs | rhs); }\
BitFlagType& operator&=(BitFlagType& lhs, const BitFlagType& rhs) { return lhs = (lhs & rhs); }\
bool operator==(const BitFlagType& lhs, const std::underlying_type_t<BitFlagType>& rhs) { return static_cast<const std::underlying_type_t<BitFlagType>>(lhs) == rhs; };\
bool operator!=(const BitFlagType& lhs, const std::underlying_type_t<BitFlagType>& rhs) { return static_cast<const std::underlying_type_t<BitFlagType>>(lhs) != rhs; };\
bool operator==(const std::underlying_type_t<BitFlagType>& lhs, const BitFlagType& rhs) { return lhs == static_cast<const std::underlying_type_t<BitFlagType>>(rhs); };\
bool operator!=(const std::underlying_type_t<BitFlagType>& lhs, const BitFlagType& rhs) { return lhs != static_cast<const std::underlying_type_t<BitFlagType>>(rhs); }

// Use in header file if mutliple definitions is not a problem
#define CREATE_BITFLAG_OPERATOR_SIGNATURES_AND_DEFINITIONS(BitFlagType) \
BitFlagType operator|(const BitFlagType& lhs, const BitFlagType& rhs) { return static_cast<const BitFlagType>(static_cast<const std::underlying_type_t<BitFlagType>>(lhs) | static_cast<const std::underlying_type_t<BitFlagType>>(rhs)); }\
BitFlagType operator&(const BitFlagType& lhs, const BitFlagType& rhs) { return static_cast<const BitFlagType>(static_cast<const std::underlying_type_t<BitFlagType>>(lhs) & static_cast<const std::underlying_type_t<BitFlagType>>(rhs)); }\
BitFlagType& operator|=(BitFlagType& lhs, const BitFlagType& rhs) { return lhs = (lhs | rhs); }\
BitFlagType& operator&=(BitFlagType& lhs, const BitFlagType& rhs) { return lhs = (lhs & rhs); }\
bool operator==(const BitFlagType& lhs, const std::underlying_type_t<BitFlagType>& rhs) { return static_cast<const std::underlying_type_t<BitFlagType>>(lhs) == rhs; }\
bool operator!=(const BitFlagType& lhs, const std::underlying_type_t<BitFlagType>& rhs) { return static_cast<const std::underlying_type_t<BitFlagType>>(lhs) != rhs; }\
bool operator==(const std::underlying_type_t<BitFlagType>& lhs, const BitFlagType& rhs) { return lhs == static_cast<const std::underlying_type_t<BitFlagType>>(rhs); }\
bool operator!=(const std::underlying_type_t<BitFlagType>& lhs, const BitFlagType& rhs) { return lhs != static_cast<const std::underlying_type_t<BitFlagType>>(rhs); }

namespace MUtility
{
	typedef uint64_t BitSet;
	constexpr BitSet EMPTY_BITSET = 0;

	constexpr int32_t BITSET_SIZE_BYTES = sizeof(BitSet);
	constexpr int32_t BITSET_SIZE_BITS = BITSET_SIZE_BYTES * BITS_PER_BYTE;

	uint64_t	GetSetBitCount(const BitSet& bitset);
	BitSet		GetLowestSetBit(const BitSet& bitset);
	BitSet		GetHighestSetBit(const BitSet& bitset);
	int32_t		GetLowestSetBitIndex(const BitSet& bitset);
	int32_t		GetHighestSetBitIndex(const BitSet& bitset);

	bool		IsBitSet(const BitSet& bitset, uint32_t indexToTest);

	void		BitSetToString(const BitSet& bitset, char* buffer, int32_t bufferSize);
	std::string BitSetToString(const BitSet& bitset);
}