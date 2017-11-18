#include "aabb.h"

AABB::AABB(V3 firstPoint) {

	corners[0] = corners[1] = firstPoint;

}
void AABB::AddPoint(V3 pt) {

	for (int i = 0; i < 3; i++) {
		if (corners[0][i] > pt[i])
			corners[0][i] = pt[i];
		if (corners[1][i] < pt[i])
			corners[1][i] = pt[i];
	}

}

int AABB::Clip(V3 minc, V3 maxc, int dimsN) {

	for (int i = 0; i < dimsN; i++) {
		if (corners[0][i] > maxc[i])
			return 0;
		if (corners[1][i] < minc[i])
			return 0;
	}

	for (int i = 0; i < dimsN; i++) {
		if (corners[0][i] < minc[i])
			corners[0][i] = minc[i];
		if (corners[1][i] > maxc[i])
			corners[1][i] = maxc[i];
	}

	return 1;

}
