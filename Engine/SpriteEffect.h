#pragma once
#include "Graphics.h"
#include "Colors.h"

namespace SpriteEffect
{
	class Chroma
	{
	public:
		Chroma( Color c )
			:
			chroma( c )
		{}
		void operator()(Color cSrc, int xDest, int yDest, Graphics& gfx) const
		{
			if (cSrc != chroma)
			{
				gfx.PutPixel(xDest, yDest, cSrc);
			}
		}
	private:
		Color chroma;
	};
	class Substitution
	{
	public:
		Substitution(Color chroma_in, Color sub_in)
			:
			chroma(chroma_in),
			sub(sub_in)
		{}
		void operator()(Color cSrc, int xDest, int yDest, Graphics& gfx) const
		{
			if (cSrc != chroma)
			{
				gfx.PutPixel(xDest, yDest, sub);
			}
		}
	private:
		Color chroma = Colors::Magenta;
		Color sub;
	};
	class Copy
	{
	public:
		void operator()(Color cSrc, int xDest, int yDest, Graphics& gfx) const
		{
			gfx.PutPixel(xDest, yDest, cSrc);
		}
	};
	class Ghost
	{
	public:
		Ghost(Color chroma_in)
			:
			chroma(chroma_in)
		{}
		void operator()(Color cSrc, int xDest, int yDest, Graphics& gfx) const
		{
			if (cSrc != chroma)
			{
				const Color destPixel = gfx.GetPixel(xDest, yDest);
				const Color blendedPix = {
					unsigned char((destPixel.GetR() + cSrc.GetR()) / 2),
					unsigned char((destPixel.GetG() + cSrc.GetG()) / 2),
					unsigned char((destPixel.GetB() + cSrc.GetB()) / 2),
				};
				gfx.PutPixel(xDest, yDest, blendedPix);
			}
		}
	private:
		Color chroma;
	};
}