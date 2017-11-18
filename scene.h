#pragma once

#include "gui.h"
#include "framebuffer.h"
#include "tm.h"
#include "gfb.h"

class Scene {
public:

	GUI *gui;
	FrameBuffer *fb, *fb3, *smfb;
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
};

extern Scene *scene;