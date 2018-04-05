#pragma once

namespace MUtility
{
	template<class TagStruct, class UnderlyingType, UnderlyingType invalidValue>
	class StrongID
	{
	public:
		StrongID() : m_Value(invalidValue) {}
		explicit StrongID(UnderlyingType value) : m_Value(value) {}

		static const StrongID Invalid() { return StrongID(); };
		void Invalidate() { m_Value = invalidValue; }
		bool IsValid() const { return m_Value != invalidValue; }

		operator UnderlyingType() const { return m_Value; }

	private:
		UnderlyingType m_Value;
	};
}