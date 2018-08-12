#pragma once
#include "Rect.h"
#include "Graphics.h"
#include "Surface.h"
#include "Vec2.h"

class Block
{
public:

	enum class Displayed
	{
		OnePrime = 0,
		TwoPrime = 1,
		ThreePrime = 2,
		OneTwoPrime = 3,
		TwoThreePrime = 4,
		ThreeOnePrime = 5,
		all = 6,
		Nothing = 7,
	};

	enum class Contents
	{
		Stone = 0,
		Empty = 1
	};

	Block(const Contents contents_in,const Displayed display_in,Vei2 gridPos);
	void Draw(Graphics& gfx, Surface& surface, Surface& highlight, const Vei2& loc, const int drawHeight);
	int GetWidth() const;
	int GetHeight() const;
	Contents GetContent() const;
	Displayed GetDisplay() const;
	bool GetHasCalcNeighbors() const;
	Vei2 GetPos() const;
	bool GetSelected() const;
	void SetContent(Contents val);
	void SetDisplayed(Displayed val);
	void SetHasCalcNeighbors(const bool val);
	void SetSelected(const bool selected_in);
	RectI GetPrimeRect(const Displayed display, const Contents content) const;
private:
	bool hasCalcNeighbors = false;
	Color chroma = { 255,0,255 };
	Contents content;
	Displayed display;
	Vei2 pos;
	bool selected = false;
};