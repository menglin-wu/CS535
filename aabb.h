#pragma once

#include "v3.h"

class AABB {
public:
	V3 corners[2];
	AABB(V3 firstPoint);
	void AddPoint(V3 pt);
	int Clip(V3 minc, V3 maxc, int dimsN = 3);
};