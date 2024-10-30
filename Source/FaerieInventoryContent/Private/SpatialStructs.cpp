﻿// Copyright Guy (Drakynfly) Lundvall. All Rights Reserved.

#include "SpatialStructs.h"

FFaerieGridShape FFaerieGridShape::MakeRect(const int32 Height, const int32 Width)
{
	FFaerieGridShape OutShape;
	OutShape.Points.Reserve(Height * Width);
	for (int32 x = 0; x < Height; ++x)
	{
		for (int32 y = 0; y < Width; ++y)
		{
			OutShape.Points.Add(FIntPoint(x, y));
		}
	}
	return OutShape;
}

void FFaerieGridShape::Translate(const FIntPoint& Position)
{
	NormalizeShape();
	for (FIntPoint& Coord : Points)
	{
		Coord += Position;
	}
}

void FFaerieGridShape::NormalizeShape()
{
	if (Points.IsEmpty())
	{
		return;
	}
    
	int32 MinX = Points[0].X;
	int32 MinY = Points[0].Y;
    
	for (const FIntPoint& Point : Points)
	{
		MinX = FMath::Min(MinX, Point.X);
		MinY = FMath::Min(MinY, Point.Y);
	}
    
	for (FIntPoint& Point : Points)
	{
		Point.X -= MinX;
		Point.Y -= MinY;
	}
}