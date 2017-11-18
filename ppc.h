#pragma once

#include "v3.h"
class FrameBuffer;

class PPC {
public:
	V3 a, b, c, C;
	int w, h;
	PPC(int _w, int _h, float hfov);
	int Project(V3 P, V3 &projP);
	void Translate(V3 tv) {
		C = C + tv;
	};
	void PositionAndOrient(V3 newC, V3 lap, V3 up);
	float GetF();
	V3 GetVD();
	void SetInterpolated(PPC *ppc0, PPC *ppc1, float fracf);
	void Pan(float theta);
	void Tilt(float theta);
	void Roll(float theta);
	void Visualize(float vf, PPC *vppc, FrameBuffer *fb);
	V3 GetPoint(float uf, float vf, float z);
	V3 Unproject(V3 pp);
	void SaveToTextFile(char *fname);
	void LoadFromTextFile(char *fname);
};