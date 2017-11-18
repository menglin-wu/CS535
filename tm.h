#pragma once

#include "v3.h"
#include "aabb.h"
#include "framebuffer.h"
#include "ppc.h"
#include "m33.h"
#include "gfb.h"

class TM {

public:
	V3 *verts, *cols, *tcs, *normals;
	FrameBuffer *tex;
	int vertsN;
	unsigned int *tris;
	int trisN;
	int enabled;
	int id;

	TM() : normals(0), tcs(0), tex(0), id (0), verts(0), cols(0), vertsN(0), tris(0), trisN(0), enabled(1) {};
	void SetToBox(V3 O, V3 dims, V3 color);
	void SetToRectangle(V3 O, V3 dims, V3 color);
	void Allocate(int vsN, int tsN);
	void RenderPoints(PPC *ppc, FrameBuffer *fb);
	void RenderWireframe(PPC *ppc, FrameBuffer *fb);
	void RenderFilled(PPC *ppc, FrameBuffer *fb);
	void RenderGFB(PPC *ppc, GFB *gfb);
	V3 GetCenterOfMass();
	void LoadBin(char *fname);
	void RotateAboutAxis(V3 O, V3 a, float theta);
	M33 ComputeSSIM(V3 *pvs);
	M33 ComputeMSIM(M33 vs, PPC *ppc);
	void VisualizeNormals(PPC *ppc, FrameBuffer *fb, float len);
	AABB ComputeAABB();
	void RenderHW();
};