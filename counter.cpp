
// Name
//===================================
//
// Counter
//


// DEPENDENCIES
//===================================

    #include "counter.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================


    Counter::Counter( int init_value, int upper_limit, int lower_limit, bool reset, bool flip )
    :
        init_value_ ( init_value ),
        upper_limit_ ( upper_limit ),
        lower_limit_ ( lower_limit ),
        value_ ( init_value ),
        reset_ ( reset ),
        flip_ ( flip )
    {};

    int Counter::operator() () const
    {
        return value_;
    };

    int Counter::operator+  ( int n )
    {
        return testIncrease( value_ + n );
    };

    int Counter::operator+  ( const Counter& other )
    {
        return *this + other.value();
    };

    int Counter::operator-  ( int n )
    {
        return testDecrease( value_ - n );
    };

    int Counter::operator-  ( const Counter& other )
    {
        return *this - other.value();
    };

    Counter& Counter::operator+=  ( int n )
    {
        value_ = testIncrease( value_ + n );
        return *this;
    };

    Counter& Counter::operator-=  ( int n )
    {
        value_ = testDecrease( value_ - n );
        return *this;
    };

    Counter& Counter::operator+=  ( const Counter& other )
    {
        *this += other.value();
        return *this;
    };

    Counter& Counter::operator-=  ( const Counter& other )
    {
        *this -= other.value();
        return *this;
    };

    Counter& Counter::operator= ( int n )
    {
        value_ = testIncrease( testDecrease( n ) );
        return *this;
    };

    Counter& Counter::operator= ( const Counter& other )
    {
        *this = other.value();
        return *this;
    };

    Counter& Counter::operator++ () { *this += 1; };

    Counter& Counter::operator-- () { *this -= 1; };

    bool Counter::operator== ( int n ) const
    {
        return value_ == n;
    };

    bool Counter::operator== ( const Counter& other ) const
    {
        return *this == other.value();
    };

    bool Counter::operator!= ( int n ) const
    {
        return value_ != n;
    };

    bool Counter::operator!= ( const Counter& other ) const
    {
        return *this != other.value();
    };

    bool Counter::operator<  ( int n ) const
    {
        return value_ < n;
    };

    bool Counter::operator<  ( const Counter& other ) const
    {
        return *this < other.value();
    };

    bool Counter::operator>  ( int n ) const
    {
        return value_ > n;
    };

    bool Counter::operator>  ( const Counter& other ) const
    {
        return *this > other.value();
    };

    bool Counter::operator<= ( int n ) const
    {
        return value_ <= n;
    };

    bool Counter::operator<= ( const Counter& other ) const
    {
        return *this <= other.value();
    };

    bool Counter::operator>= ( int n ) const
    {
        return value_ >= n;
    };

    bool Counter::operator>= ( const Counter& other ) const
    {
        return *this >= other.value();
    };

    int Counter::value() const { return value_; };

    int Counter::testDecrease( int n ) const
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

    int Counter::testIncrease( int n ) const
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

    void Counter::reset()
    {
        value_ = init_value_;
    };
