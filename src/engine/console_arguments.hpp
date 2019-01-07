#pragma once

class ConsoleArguments
{
	public:
		ConsoleArguments( int argc, char** argv );
		bool windowed() const;
		bool noaudio() const;
		bool nosave() const;
		int magnification() const;

	private:
		bool windowed_;
		bool noaudio_;
		bool nosave_;
		int magnification_;
};
