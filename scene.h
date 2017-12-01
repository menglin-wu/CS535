#pragma once

#include "gui.h"
#include "framebuffer.h"
#include "tm.h"
#include "gfb.h"
#include "CGInterface.h"


class Scene {
public:

	CGInterface *cgi;
	ShaderOneInterface *soi;

	GUI *gui;
	FrameBuffer *fb, *fb3, *smfb, *hwfb, *gpufb;
	GFB *gfb;
	V3 L;
	float specc;
	TM *tms;
	int tmsN;
	FrameBuffer *texts;
	PPC *ppc, *ppc3, *smppc;
	Scene();
	void DBG();
	void NewButton();
	void Render(FrameBuffer *fb, PPC *ppc);
	void RenderAll();
	void ShadowMapSetup();
	void RenderHW();
	void RenderGPU();

	float morphFraction;
};

extern Scene *scene;