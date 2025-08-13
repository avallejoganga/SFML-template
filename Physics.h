#pragma once
#include "Entity.h"
#include <cmath>

namespace Physics
{
	inline Vec2f GetOverlap(Entity a, Entity b)
	{
		Vec2f aVec = a.get<CTransform>().pos;
		Vec2f aHalfBox = a.get<CBoundingBox>().halfSize;
		Vec2f bVec = b.get<CTransform>().pos;
		Vec2f bHalfBox = b.get<CBoundingBox>().halfSize;
		Vec2f overlap;

		float dx = std::abs(aVec.x - bVec.x);
		float dy = std::abs(aVec.y - bVec.y);
		float xValue = aHalfBox.x + bHalfBox.x;
		float yValue = aHalfBox.y + bHalfBox.y;

		//Si devuelve uno de los parámetros cómo negativo no hay overlap
		overlap = Vec2f(xValue - dx, yValue - dy);	
		return overlap;
	}

	inline Vec2f GetPreviousOverlap(Entity a, Entity b)
	{
		Vec2f aVec = a.get<CTransform>().prevPos;
		Vec2f aHalfBox = a.get<CBoundingBox>().halfSize;
		Vec2f bVec = b.get<CTransform>().prevPos;
		Vec2f bHalfBox = b.get<CBoundingBox>().halfSize;
		Vec2f overlap;

		float dx = std::abs(aVec.x - bVec.x);
		float dy = std::abs(aVec.y - bVec.y);
		float xValue = aHalfBox.x + bHalfBox.x;
		float yValue = aHalfBox.y + bHalfBox.y;

		//Si devuelve uno de los parámetros cómo negativo no hay overlap
		overlap = Vec2f(xValue - dx, yValue - dy);
		return overlap;
	}
}
