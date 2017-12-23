#pragma once
#include "MUtilityPlatformDefinitions.h"
#include "MUtilityByte.h"
#include <stdint.h>
#include <string>

#if PLATFORM == PLATFORM_LINUX
#include <stdlib.h> // For size_t
#endif

namespace MUtilitySerialization
{
	void CopyAndIncrementDestination( Byte*& destination, const void* const source, const size_t length );
	void CopyAndIncrementSource( void* const destination, const Byte*& source, const size_t length );

	void WriteInt16(				int16_t			value,		Byte*& destination );
	void WriteInt32(				int32_t			value,		Byte*& destination );
	void WriteInt64(				int64_t			value,		Byte*& destination );
	void WriteUint16(				uint16_t		value,		Byte*& destination );
	void WriteUint32(				uint32_t		value,		Byte*& destination );
	void WriteUint64(				uint64_t		value,		Byte*& destination );
	void WriteFloat(				float			value,		Byte*& destination );
	void WriteDouble(				double			value,		Byte*& destination );
	void WriteBool(					bool			value,		Byte*& destination );
	void WriteString(		const	std::string&	value,		Byte*& destination );

	void ReadInt16(					int16_t&		value,		const Byte*& source );
	void ReadInt32(					int32_t&		value,		const Byte*& source );
	void ReadInt64(					int64_t&		value,		const Byte*& source );
	void ReadUInt16(				uint16_t&		value,		const Byte*& source );
	void ReadUint32(				uint32_t&		value,		const Byte*& source );
	void ReadUint64(				uint64_t&		value,		const Byte*& source );
	void ReadFloat(					float&			value,		const Byte*& source );
	void ReadDouble(				double&			value,		const Byte*& source );
	void ReadBool(					bool&			value,		const Byte*& source );
	void ReadString(				std::string&	value,		const Byte*& source );
}