#ifndef RANGED_FLOAT_H
#define RANGED_FLOAT_H

namespace BSL
{
    class RangedFloat
    {
        public:
            constexpr RangedFloat( float max, float min = 0.0f, float init = 0.0f, bool loop = false )
            :
                loop_ ( loop ),
                max_ ( max ),
                min_ ( min ),
                n_ ( init )
            {};

            constexpr float get() const { return n_; };

            constexpr RangedFloat& operator+= ( float n )
            {
                n_ = testIncrease( n_ + n );
                return *this;
            };

            constexpr RangedFloat& operator++ () { return *this += 1; };

        private:
            bool loop_;
            float max_;
            float min_;
            float n_;

            constexpr float testIncrease( float n ) const
            {
                return n > max_ ? ( loop_ ? min_ : max_ ) : n;
            };
    };
}

#endif // RANGED_FLOAT_H