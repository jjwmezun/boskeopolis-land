#ifndef COUNTER_H
#define COUNTER_H

namespace BSL
{
    template<typename T, T max, T min = 0, bool loop = false>
    class Counter
    {
        public:
            constexpr Counter( T init = min )
            :
                value_ ( init )
            {};

            constexpr T get() const { return value_; };

            constexpr Counter& operator+= ( T n )
            {
                value_ = testIncrease( value_ + n );
                return *this;
            };

            constexpr Counter& operator++ () { return *this += 1; };

        private:
            T value_;

            constexpr int testIncrease( T n ) const
            {
                return n > max ? ( loop ? min : max ) : n;
            };
    };
}

#endif // COUNTER_H