#pragma once

#include <string>

class HexString final
{
	public:
		HexString( int n, int min_digits = 1 );
		const std::string& getHexString() const;
		int getDecimal() const;
		static std::string generateHexStringFromDecimal( int decimal, int min_digits = 1 );

	private:
		const std::string hex_;
		const int decimal_;
};
