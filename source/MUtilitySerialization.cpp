#include "interface/MUtilitySerialization.h"
#include "interface/MUtilityDataSizes.h"

using namespace DataSizes;
using MUtility::Byte;

//************************** Utility **************************

void MUtilitySerialization::CopyAndIncrementDestination( Byte*& destination, const void* const source, const size_t length )
{
	memcpy( destination, source, length );
	destination += length;
}

void MUtilitySerialization::CopyAndIncrementSource( void* const destination, const Byte*& source, const size_t length )
{
	memcpy( destination, source, length );
	source += length;
}

//************************** Writing **************************

void MUtilitySerialization::WriteInt16( int16_t value, Byte*& destination )
{
	CopyAndIncrementDestination( destination, &value, INT_16_SIZE );
}

void MUtilitySerialization::WriteInt32( int32_t value, Byte*& destination )
{
	CopyAndIncrementDestination( destination, &value, INT_32_SIZE );
}

void MUtilitySerialization::WriteInt64( int64_t value, Byte*& destination )
{
	CopyAndIncrementDestination( destination, &value, INT_64_SIZE );
}

void MUtilitySerialization::WriteUint16( uint16_t value, Byte*& destination )
{
	CopyAndIncrementDestination( destination, &value, INT_16_SIZE );
}

void MUtilitySerialization::WriteUint32( uint32_t value, Byte*& destination )
{
	CopyAndIncrementDestination( destination, &value, INT_32_SIZE );
}

void MUtilitySerialization::WriteUint64( uint64_t value, Byte*& destination )
{
	CopyAndIncrementDestination( destination, &value, INT_64_SIZE );
}

void MUtilitySerialization::WriteFloat( float value, Byte*& destination )
{
	CopyAndIncrementDestination( destination, &value, FLOAT_SIZE );
}

void MUtilitySerialization::WriteDouble( double value, Byte*& destination )
{
	CopyAndIncrementDestination( destination, &value, DOUBLE_SIZE );
}

void MUtilitySerialization::WriteBool( bool value, Byte*& destination )
{
	CopyAndIncrementDestination( destination, &value, BOOL_SIZE );
}

void MUtilitySerialization::WriteString( const std::string& value, Byte*& destination )
{
	uint32_t stringLength = static_cast<uint32_t>( value.size() );
	CopyAndIncrementDestination( destination, &stringLength, INT_32_SIZE );
	CopyAndIncrementDestination( destination, value.data(), stringLength );
}

//************************** Reading **************************

void MUtilitySerialization::ReadInt16( int16_t& value, const Byte*& source )
{
	CopyAndIncrementSource( &value, source, INT_16_SIZE );
}

void MUtilitySerialization::ReadInt32( int32_t& value, const Byte*& source )
{
	CopyAndIncrementSource( &value, source, INT_32_SIZE );
}

void MUtilitySerialization::ReadInt64( int64_t& value, const Byte*& source )
{
	CopyAndIncrementSource( &value, source, INT_64_SIZE );
}

void MUtilitySerialization::ReadUInt16( uint16_t& value, const Byte*& source )
{
	CopyAndIncrementSource( &value, source, INT_16_SIZE );
}

void MUtilitySerialization::ReadUint32( uint32_t& value, const Byte*& source )
{
	CopyAndIncrementSource( &value, source, INT_32_SIZE );
}

void MUtilitySerialization::ReadUint64( uint64_t& value, const Byte*& source )
{
	CopyAndIncrementSource( &value, source, INT_64_SIZE );
}

void MUtilitySerialization::ReadFloat( float& value, const Byte*& source )
{
	CopyAndIncrementSource( &value, source, FLOAT_SIZE );
}

void MUtilitySerialization::ReadDouble( double& value, const Byte*& source )
{
	CopyAndIncrementSource( &value, source, DOUBLE_SIZE );
}

void MUtilitySerialization::ReadBool( bool& value, const Byte*& source )
{
	CopyAndIncrementSource( &value, source, BOOL_SIZE );
}

void MUtilitySerialization::ReadString( std::string& value, const Byte*& source ) // TODODB: Optimize this one~ (Try to avoid unnecessary memory allocation)
{
	uint32_t stringLength;
	CopyAndIncrementSource( &stringLength, source, INT_32_SIZE );

	char* buffer = static_cast<char*>( malloc( stringLength + 1 ) ); // +1 for the null terminator //TODODB: See if we really need to place this null terminator or if it will be placed there by the std::string ctor
	CopyAndIncrementSource( buffer, source, stringLength );
	buffer[stringLength] = '\0';

	value = std::string( buffer );
	delete buffer;
}