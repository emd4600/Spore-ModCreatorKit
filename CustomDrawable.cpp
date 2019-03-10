/****************************************************************************
* Copyright (C) 2019 Eric Mor
*
* This file is part of Spore ModCreatorKit.
*
* Spore ModAPI is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/
#include "stdafx.h"
#include "CustomDrawable.h"


CustomDrawable::CustomDrawable()
	: mpImage(nullptr)
{
	Image* pImage = nullptr;
	if (Image::GetImage(ResourceKey(0x5F4C1EA3, Image::kTypePNG, 0x02231C8B), pImage))
	{
		mpImage = pImage;
	}
}


CustomDrawable::~CustomDrawable()
{
}

inline float moveEdge(int edge, const Math::Rectangle& area, float fValue)
{
	switch (edge)
	{
	case ILayoutElement::kLeft: return area.left + fValue;
	case ILayoutElement::kTop: return area.top + fValue;
	case ILayoutElement::kRight: return area.right - fValue;
	case ILayoutElement::kBottom: return area.bottom - fValue;
	}
}

void CustomDrawable::Paint(UIRenderer* pRenderer, const Math::Rectangle& area, const RenderParams& params)
{
	Graphics2D& g2d = pRenderer->GetGraphics2D(0);

	float fWidth = 4.0f;

	float fQuadParam = 15.0f;

	g2d.SetColor(Color(0xFF00FF00));
	g2d.FillRectangle(
		/* left */	area.x1 + fWidth,
		/* top */	area.y1 + fWidth,
		/* right */	area.x2 - fWidth,
		/* bot */	area.y2 - fWidth
	);

	g2d.SetColor(Color::BLACK);
	g2d.DrawImageScaled(area, mpImage.get(), Math::Rectangle(0.25f, 0, 1.0f, 1.0f));

	g2d.SetColor(Color(0xFF5487FD));
	g2d.DrawLine(area.x1, area.y1, area.x2, area.y2);
}
