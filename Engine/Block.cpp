#include "Block.h"
#include "SpriteEffect.h"
#include <assert.h>

Block::Block(const Contents contents_in, const Displayed display_in, Vei2 gridPos)
	:
	content(contents_in),
	display(display_in),
	pos(gridPos)
{
}

void Block::Draw(Graphics& gfx, Surface& surface, Surface& highlight, const Vei2& loc_in, const int drawHeight)
{
	//shift sprite so the middle is the seed loc for drawing instead of top left corner
	Vei2 loc = { loc_in.x - GetWidth() / 2,loc_in.y - GetHeight() / 4 };

	//see what should be drawn
	if (Block::Contents::Empty != content && Block::Displayed::Nothing != display)
	{
		if (Block::Contents::Stone == content)
		{
			gfx.DrawSprite(loc.x, loc.y - 32 * drawHeight, GetPrimeRect(display, content), gfx.GetScreenRect(), surface, SpriteEffect::Chroma{ chroma });
		}
	}
	//draw border if selected
	if (selected)
	{
		if (Block::Contents::Empty != content && Block::Displayed::Nothing != display)
		{
			gfx.DrawSprite(loc.x, loc.y - 32 * drawHeight, GetPrimeRect(display, content), gfx.GetScreenRect(), highlight, SpriteEffect::Chroma{ chroma });
		}
		else
		{
			gfx.DrawSprite(loc.x, loc.y - 32 * drawHeight, RectI(6 * 64,7 * 64,0,64), gfx.GetScreenRect(), highlight, SpriteEffect::Chroma{ chroma });
		}
	}
}

int Block::GetWidth() const
{
	return GetPrimeRect(display,content).right - GetPrimeRect(display, content).left;
}

int Block::GetHeight() const
{
	return GetPrimeRect(display, content).bottom - GetPrimeRect(display, content).top;
}

Block::Contents Block::GetContent() const
{
	return content;
}

Block::Displayed Block::GetDisplay() const
{
	return display;
}

bool Block::GetHasCalcNeighbors() const
{
	return hasCalcNeighbors;
}

Vei2 Block::GetPos() const
{
	return pos;
}

bool Block::GetSelected() const
{
	return selected;
}

void Block::SetContent(Contents val)
{
	content = val;
}

void Block::SetDisplayed(Displayed val)
{
	display = val;
	hasCalcNeighbors = true;

}

void Block::SetHasCalcNeighbors(const bool val)
{
	hasCalcNeighbors = val;
}

void Block::SetSelected(const bool selected_in)
{
	selected = selected_in;
}

RectI Block::GetPrimeRect(const Displayed display,const Contents content) const
{
	assert(int(display) >= 0);
	assert(int(display) < 8);
	assert(int(content) >= 0);
	assert(int(content) <= 1);
	//returns rect of the sprite based on primes
	return RectI( 64 * int(display),64 + 64 * int(display),64 * int(content),64 + 64 * int(content));
}
