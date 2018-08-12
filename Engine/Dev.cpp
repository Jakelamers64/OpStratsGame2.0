#include "Dev.h"

void Dev::DrawStats(const Vei2 pos, const Vei2 mousePos, const float dt, Graphics& gfx,World& world)
{
	FrameTime(dt,pos,gfx);
	MousePos(mousePos, pos, gfx);
	PlayerGridPos(mousePos, pos, gfx, world);
}

void Dev::FrameTime(const float dt, const Vei2 pos, Graphics& gfx)
{
	//get the frame time to be output
	std::string frameTime = std::to_string(dt);
	//output frame time 
	font.DrawText(frameTime, pos, gfx);
}

void Dev::MousePos(const Vei2 mousePos, const Vei2 pos, Graphics & gfx)
{
	//convet mouse pos to string
	std::string mousePosX = "X=" + std::to_string(mousePos.x);
	std::string mousePosY = "Y=" + std::to_string(mousePos.y);
	//out put pos
	font.DrawText(mousePosX, { pos.x,pos.y + font.GetGlyphHeight() }, gfx);
	font.DrawText(mousePosY, { pos.x,pos.y + font.GetGlyphHeight() * 2 }, gfx);
}

void Dev::PlayerGridPos(const Vei2 mousePos, const Vei2 pos, Graphics & gfx, World & world)
{
	//convert mousePos into gridpos then output
	Vei2 playerGridPos = world.IsoToGrid(mousePos, { world.GetOrigin().x,world.GetOrigin().y - 32 });
	//x
	std::string playerGridPosXStr = "X=" + std::to_string(playerGridPos.x);
	font.DrawText(playerGridPosXStr, { pos.x,pos.y + font.GetGlyphHeight() * 3 }, gfx);
	//y
	std::string playerGridPosYStr = "Y=" + std::to_string(playerGridPos.y);
	font.DrawText(playerGridPosYStr, { pos.x,pos.y + font.GetGlyphHeight() * 4 }, gfx);
}
