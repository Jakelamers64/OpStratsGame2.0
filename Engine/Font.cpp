#include "Font.h"
#include <assert.h>
#include "SpriteEffect.h"

Font::Font(const std::string & filename, Color chroma)
	:
	surface(filename),
	//calc dimensions from bitmap dimensions
	glyphWidth( surface.GetWidth() / nColumns ),
	glyphHeight(surface.GetHeight() / nRows ),
	chroma(chroma)
{
	//verify the bitmap had valid dimensions 
	assert( glyphWidth * nColumns == surface.GetWidth() );
	assert( glyphHeight * nRows == surface.GetHeight() );
}

void Font::DrawText(const std::string & text, const Vei2 pos, Graphics & gfx,Color color) const
{
	//curPos is the pos that we are drawing to on the screen
	Vei2 curPos = pos;
	for (auto c : text)
	{ 
		//on new line char reset x and jump a glyph height down
		if (c == '\n')
		{
			//carrige return
			curPos.x = pos.x;
			//line feed
			curPos.y += glyphHeight;
			continue;
		}
		//only draw chars that are on the font sheet
		//first char + 1 becuase might as well skip ' '
		else if ( c >= firstChar + 1 && c <= lastChar )
		{
			gfx.DrawSprite(curPos.x, curPos.y, MapGlyphRect(c), gfx.GetScreenRect(), surface, SpriteEffect::Substitution{ chroma,color });
		}
		//advances screen pos to next char
		curPos.x += glyphWidth;
	}
}

int Font::GetGlyphHeight() const
{
	return glyphHeight;
}

RectI Font::MapGlyphRect(char c) const
{
	assert(c >= firstChar && c <= lastChar);
	//font sheet starts at ' ', calc index into sheet
	const int glyphIndex = c - ' ';
	//map 1 glyph index to 2d coordinates
	const int yGlyph = glyphIndex / nColumns;
	const int xGlyph = glyphIndex % nColumns;
	//convert sheet grid coords to pixel coords in sheet
	return RectI(
		{xGlyph * glyphWidth,yGlyph * glyphHeight},
		glyphWidth,glyphHeight
	);
}
