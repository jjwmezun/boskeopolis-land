#pragma once

class TextObj;

class TextComponent
{
	public:
		TextComponent();
		virtual ~TextComponent();
		virtual void update( TextObj& text ) = 0;
};