#pragma once

class Collision final
{
	public:

		constexpr Collision
		(
			int overlap_x_left,
			int overlap_x_right,
			int overlap_y_top,
			int overlap_y_bottom
		)
		:
			overlap_x_left_   ( overlap_x_left ),
			overlap_x_right_  ( overlap_x_right ),
			overlap_y_top_    ( overlap_y_top ),
			overlap_y_bottom_ ( overlap_y_bottom )
		{};

		constexpr int overlapXLeft() const
		{
			return overlap_x_left_;
		};

		constexpr int overlapXRight() const
		{
			return overlap_x_right_;
		};

		constexpr int overlapYTop() const
		{
			return overlap_y_top_;
		};

		constexpr int overlapYBottom() const
		{
			return overlap_y_bottom_;
		};

		constexpr bool collideLeft() const
		{
			return overlapXLeft() > 0;
		};

		constexpr bool collideRight() const
		{
			return overlapXRight() > 0;
		};

		constexpr bool collideTop() const
		{
			return overlapYTop() > 0;
		};

		constexpr bool collideBottom() const
		{
			return overlapYBottom() > 0;
		};

		constexpr bool collideAny() const
		{
			return ( collideLeft() || collideRight() || collideTop() || collideBottom() );
		};

	private:
		const int overlap_x_left_;
		const int overlap_x_right_;
		const int overlap_y_top_;
		const int overlap_y_bottom_;
};