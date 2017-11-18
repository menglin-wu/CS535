#pragma once

#include "v3.h"
#include "ppc.h"
#include "framebuffer.h"

class SL{
public:
	V3 uv1;
	int uv; // pixel coordinates in output image
	float z; // z value of the output image sample, as reprojected onto light image plane
	int inShadow;
};

class GPix {
public:
	SL *sls;
	int slsN;
	int maxN;
	GPix() : sls(0), slsN(0), maxN(0) {};
	void AddSL(V3 PP, int uv);
	void ApplyShadow(FrameBuffer *fb);
	void Clear();
};

class GFB{
public:
	int w, h;
	GPix *gpix;
	FrameBuffer *visfb;
	void AddSL(V3 PP, int uv);
	void AddSLs(PPC *ppc, FrameBuffer *fb, PPC *smppc);
	GFB(int _w, int _h);
	void Clear();
	void SLStats();
	void ApplyShadow(FrameBuffer *fb);
};

