

// Name
//===================================
//
// Counter
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
            Counter
            (
                int init_value = 0,
                int upper_limit = 0,
                int lower_limit = 0,
                bool reset = false,
                bool flip = false
            );

            int  operator() () const;
            int  operator+  ( int n );
            int  operator+  ( const Counter& other );
            int  operator-  ( int n );
            int  operator-  ( const Counter& other );
            Counter& operator+= ( int n );
            Counter& operator-= ( int n );
            Counter& operator+= ( const Counter& other );
            Counter& operator-= ( const Counter& other );
            Counter& operator++ ();
            Counter& operator-- ();
            Counter& operator=  ( int n );
            Counter& operator= ( const Counter& other );
            bool operator== ( int n ) const;
            bool operator== ( const Counter& other ) const;
            bool operator!= ( int n ) const;
            bool operator!= ( const Counter& other ) const;
            bool operator<  ( int n ) const;
            bool operator<  ( const Counter& other ) const;
            bool operator>  ( int n ) const;
            bool operator>  ( const Counter& other ) const;
            bool operator<= ( int n ) const;
            bool operator<= ( const Counter& other ) const;
            bool operator>= ( int n ) const;
            bool operator>= ( const Counter& other ) const;

            int value() const;
			int upperLimit() const;
			int lowerLimit() const;
			bool hitLimit() const;

            void reset();


        private:
            static const int UPPER_LIMIT_NULL = 0;
            static const int LOWER_LIMIT_NULL = 0;

            const int init_value_ ;
            const int upper_limit_;
            const int lower_limit_;
            int value_            ;

            const bool reset_;
            const bool flip_;

            int testDecrease( int n ) const;
            int testIncrease( int n ) const;
    };

#endif // COUNTER_H
