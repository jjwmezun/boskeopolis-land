

// Name
//===================================
//
// Counter
//
// Integer with automatic bounds-checking.
//


#ifndef COUNTER_H
#define COUNTER_H

// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================


// CLASS
//===================================

    class Counter
    {
        public:
            constexpr Counter
            (
                int init_value = 0,
                int upper_limit = 0,
                int lower_limit = 0,
                bool reset = false
			)
			:
				init_value_ ( init_value ),
				upper_limit_ ( upper_limit ),
				lower_limit_ ( lower_limit ),
				value_ ( init_value ),
				reset_ ( reset )
			{};

			constexpr int operator() () const
			{
				return value_;
			};
	
			constexpr int operator+  ( int n )
			{
				return testIncrease( value_ + n );
			};

			constexpr int operator+  ( const Counter& other )
			{
				return *this + other.value();
			};

			constexpr int operator-  ( int n )
			{
				return testDecrease( value_ - n );
			};

			constexpr int operator-  ( const Counter& other )
			{
				return *this - other.value();
			};

			constexpr Counter& operator+=  ( int n )
			{
				value_ = testIncrease( value_ + n );
				return *this;
			};

			constexpr Counter& operator-=  ( int n )
			{
				value_ = testDecrease( value_ - n );
				return *this;
			};

			constexpr Counter& operator+=  ( const Counter& other )
			{
				*this += other.value();
				return *this;
			};

			constexpr Counter& operator-=  ( const Counter& other )
			{
				*this -= other.value();
				return *this;
			};

			constexpr Counter& operator= ( int n )
			{
				value_ = testIncrease( testDecrease( n ) );
				return *this;
			};

			constexpr Counter& operator= ( const Counter& other )
			{
				*this = other.value();
				return *this;
			};

			constexpr Counter& operator++ () { *this += 1; };

			constexpr Counter& operator-- () { *this -= 1; };

			constexpr bool operator== ( int n ) const
			{
				return value_ == n;
			};

			constexpr bool operator== ( const Counter& other ) const
			{
				return *this == other.value();
			};

			constexpr bool operator!= ( int n ) const
			{
				return value_ != n;
			};

			constexpr bool operator!= ( const Counter& other ) const
			{
				return *this != other.value();
			};

			constexpr bool operator<  ( int n ) const
			{
				return value_ < n;
			};

			constexpr bool operator<  ( const Counter& other ) const
			{
				return *this < other.value();
			};

			constexpr bool operator>  ( int n ) const
			{
				return value_ > n;
			};

			constexpr bool operator>  ( const Counter& other ) const
			{
				return *this > other.value();
			};

			constexpr bool operator<= ( int n ) const
			{
				return value_ <= n;
			};

			constexpr bool operator<= ( const Counter& other ) const
			{
				return *this <= other.value();
			};

			constexpr bool operator>= ( int n ) const
			{
				return value_ >= n;
			};

			constexpr bool operator>= ( const Counter& other ) const
			{
				return *this >= other.value();
			};

			constexpr int value() const { return value_; };

			constexpr void reset()
			{
				value_ = init_value_;
			};

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
            const int init_value_ ;
            const int upper_limit_;
            const int lower_limit_;
            int value_            ;
            const bool reset_;

			constexpr int testDecrease( int n ) const
			{
				if ( n < lower_limit_ )
				{
					if ( reset_ )
						return init_value_;
					else
						return lower_limit_;
				}

				return n;
			};

			constexpr int testIncrease( int n ) const
			{
				if ( n > upper_limit_ )
				{
					if ( reset_ )
						return init_value_;
					else
						return upper_limit_;
				}

				return n;
			};
    };

#endif // COUNTER_H
