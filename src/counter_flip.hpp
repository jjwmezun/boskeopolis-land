#pragma once

template<int upper_limit_, int lower_limit_ = 0>
class CounterFlip
{
	public:
		constexpr CounterFlip( int init_value = 0 )
		:
			value_ ( init_value )
		{};

		constexpr int operator() () const
		{
			return value_;
		};

		constexpr int operator+ ( int n )
		{
			return testIncrease( value_ + n );
		};

		constexpr int operator-  ( int n )
		{
			return testDecrease( value_ - n );
		};

		constexpr CounterFlip<upper_limit_, lower_limit_>& operator+= ( int n )
		{
			value_ = testIncrease( value_ + n );
			return *this;
		};

		constexpr CounterFlip<upper_limit_, lower_limit_>& operator-= ( int n )
		{
			value_ = testDecrease( value_ - n );
			return *this;
		};

		constexpr CounterFlip<upper_limit_, lower_limit_>& operator= ( int n )
		{
			value_ = testIncrease( testDecrease( n ) );
			return *this;
		};

		constexpr CounterFlip<upper_limit_, lower_limit_>& operator++ () { return *this += 1; };
		constexpr CounterFlip<upper_limit_, lower_limit_>& operator-- () { return *this -= 1; };

		constexpr bool operator== ( int n ) const
		{
			return value_ == n;
		};

		constexpr bool operator!= ( int n ) const
		{
			return value_ != n;
		};

		constexpr bool operator<  ( int n ) const
		{
			return value_ < n;
		};

		constexpr bool operator>  ( int n ) const
		{
			return value_ > n;
		};

		constexpr bool operator<= ( int n ) const
		{
			return value_ <= n;
		};

		constexpr bool operator>= ( int n ) const
		{
			return value_ >= n;
		};

		constexpr int value() const { return value_; };

		constexpr int upperLimit() const
		{
			return upper_limit_;
		};

		constexpr int lowerLimit() const
		{
			return lower_limit_;
		};

		constexpr bool hitLimit() const
		{
			return value_ == lower_limit_ || value_ == upper_limit_;
		};


	private:
		int value_;

		constexpr int testDecrease( int n ) const
		{
			return ( n < lower_limit_ ) ? upper_limit_ : n;
		};

		constexpr int testIncrease( int n ) const
		{
			return ( n > upper_limit_ ) ? lower_limit_ : n;
		};
};
