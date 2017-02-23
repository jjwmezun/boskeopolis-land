
// Name
//===================================
//
// TimerSwitch
//


// DEPENDENCIES
//===================================

    #include "timer_switch.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

	TimerSwitch::TimerSwitch
	(
		int switch_duration,
		int trans_duration,
		bool starts_on
	)
    :
		on_ ( starts_on ),
		switch_ ( false ),
		trans_ ( false ),
		switch_duration_ ( switch_duration ),
		trans_duration_ ( trans_duration ),
		switch_counter_ ( 0 ),
		trans_counter_ ( 0 )
    {};

    TimerSwitch::~TimerSwitch() {};

	void TimerSwitch::pause() { on_ = false; };
	void TimerSwitch::resume() { on_ = true; };

    void TimerSwitch::start()
    {
        on_ = true;
        restartCounters();
        switch_ = false;
		trans_ = false;
    };

	void TimerSwitch::restart() { start(); };

    void TimerSwitch::stop()
    {
        on_ = false;
        restartCounters();
        switch_ = false;
		trans_ = false;
    };

	void TimerSwitch::update()
	{
		if ( on() )
		{
			if ( inTrans() )
			{
				++trans_counter_;

				if ( trans_counter_ >= trans_duration_ )
				{
					trans_ = false;
					restartCounters();
					changeSwitch();
				}
			}
			else
			{
				++switch_counter_;
				
				if ( switch_counter_ >= switch_duration_ )
				{
					trans_ = true;
				}
			}
		}
	};

	bool TimerSwitch::on() const { return on_; };

    bool TimerSwitch::switchStatus() const { return switch_; };

	bool TimerSwitch::inTrans() const { return trans_; };

	double TimerSwitch::transPercent() const
	{
		return ( ( double ) trans_counter_ / ( double ) trans_duration_ );
	};

    void TimerSwitch::changeSwitch()
    {
        switch_ = !switch_;
    };

	void TimerSwitch::restartCounters()
	{
		switch_counter_ = 0;
		trans_counter_ = 0;
	};

	void TimerSwitch::changeDurations( int switch_duration, int trans_duration )
	{
		if ( switch_duration > -1 )
		{
			switch_duration_ = switch_duration;
		}
		
		if ( trans_duration > -1 )
		{
			trans_duration_ = trans_duration;
		}
	};

	void TimerSwitch::speedUp()
	{
		changeDurations( ceil( ( double )switch_duration_ / 2 ), ceil( ( double )trans_duration_ / 2 ) );
	};

	void TimerSwitch::slowDown()
	{
		changeDurations( switch_duration_ * 2, trans_duration_ * 2 );
	};
