#pragma once

class ConsoleArguments final
{
	public:
		ConsoleArguments( int argc, char** argv );
		bool windowed() const;
		bool noaudio() const;
		bool nosave() const;
		bool noharm() const;
		bool changepalette() const;
		int magnification() const;

	private:
		bool windowed_;
		bool noaudio_;
		bool nosave_;
		bool noharm_;
		bool changepalette_;
		int magnification_;
};
