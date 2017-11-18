#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <GL/glut.h>

#include "v3.h"
#include "ppc.h"

class FrameBuffer : public Fl_Gl_Window {
public:
	int isHW;
	unsigned int id;
	unsigned int *pix;
	float *zb;
	int w, h;
	FrameBuffer(int u0, int v0, int _w, int _h, unsigned int _id);
	void draw();
	void KeyboardHandle();
	int handle(int guievent);
	void SetBGR(unsigned int bgr);
	void Set(int u, int v, unsigned int color);
	unsigned int Get(int u, int v);
	void Set(V3 uv, unsigned int color);
	void SetZB(V3 uvz, unsigned int color);
	void SetGuarded(int u, int v, unsigned int color);
	void Draw2DRectangle(int u0, int v0, int u1, int v1, unsigned int color);
	void DrawCircle(int u0, int v0, float r, unsigned int color);
	void DrawCircle(V3 c, float r, V3 cv);
	void Draw2DSegment(V3 p0, V3 p1, V3 c0, V3 c1);
	void Draw3DSegment(V3 p0, V3 p1, V3 c0, V3 c1, PPC *ppc);
	void Draw3DPoint(V3 p, V3 c, float psize, PPC *ppc);
	void Clear(unsigned int bgr, float z0);
	void VisualizeImagePoints(float visz, PPC *ppc, PPC *visppc, FrameBuffer *visfb);
	void Visualize3DPoints(PPC *ppc, PPC *visppc, FrameBuffer *visfb);
	void SetBWCheckerboard(int csize);
	unsigned int LookUpNN(float s, float t);
	unsigned int LookUpBilinear(float s, float t);
	int IsVisible(V3 PP);
};