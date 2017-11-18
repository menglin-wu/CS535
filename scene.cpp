#include "scene.h"
#include "v3.h"
#include "m33.h"
#include "ppc.h"

Scene *scene;

using namespace std;

#include <iostream>

Scene::Scene() {

	gui = new GUI();
	gui->show();

	L = V3(0.0f, 0.0f, 0.0f);
	specc = 180.0f;

	int u0 = 20;
	int v0 = 20;
	int sci = 2;
	int w = sci * 320;
	int h = sci * 240;

	fb = new FrameBuffer(u0, v0, w, h, 0);
	fb->label("First person");
	fb->show();

	fb3 = new FrameBuffer(u0, v0, w, h, 1);
	fb3->label("Third person");
	fb3->position(u0 + fb->w + 20, v0);
	fb3->show();

	gui->uiw->position(u0, v0 + fb->h + 20);

	float hfov = 55.0f;
	ppc = new PPC(fb->w, fb->h, hfov);
	ppc3 = new PPC(fb3->w, fb3->h, 90.0f);

	tmsN = 6;
	tms = new TM[tmsN];
	tms[0].id = 0;
	tms[1].id = 1;
	tms[0].SetToBox(V3(0.0f, 0.0f, -100.0f), V3(30.0f, 30.0f, 30.0f), 
		V3(1.0f, 0.5f, 0.0f));
	tms[0].enabled = 0;
	tms[1].LoadBin("geometry/teapot1K.bin");
	tms[1].enabled = 1;
	ppc->C = tms[1].GetCenterOfMass() + V3(0.0f, 0.0f, 110.0f);
	L = ppc->C;

	ppc3->PositionAndOrient(ppc->C + V3(50.0f, 100.0f, 50.0f), tms[1].GetCenterOfMass(), V3(0.0f, 1.0f, 0.0f));
	tms[2].SetToRectangle(V3(0.0f, 120.0f, -1000.0f), V3(1000.0f, 230.0f, 0.0f), V3(0.0f, 0.0f, 0.0f));
	tms[2].cols[0] = tms[2].cols[1] = V3(1.0f, 1.0f, 1.0f);
	tms[2].id = 2;
	tms[3].SetToRectangle(V3(0.0f, -120.0f, -1000.0f), V3(1000.0f, 230.0f, 0.0f), V3(0.0f, 0.0f, 0.0f));
	tms[3].cols[0] = tms[3].cols[1] = V3(1.0f, 1.0f, 1.0f);
	tms[3].id = 3;
	tms[2].enabled = tms[3].enabled = 0;

	texts = new FrameBuffer(0, 0, 128, 128, 2);
	texts->SetBWCheckerboard(16);
	texts->label("texture");
//	texts->show();
	tms[4].SetToRectangle(V3(0.0f, 0.0f, -100.0f), V3(100.0f, 100.0f, 0.0f), V3(1.0f, 1.0f, 1.0f));
	tms[4].tex = texts;
	tms[4].enabled = 0;
	tms[4].id = 4;
	Render(fb, ppc);
	Render(fb3, ppc3);

	AABB aabb = tms[1].ComputeAABB();
	V3 bC = tms[1].GetCenterOfMass();
	bC[1] = aabb.corners[0][1];
	tms[5].SetToBox(bC, V3(200.0f, 1.0f, 200.0f), V3(1.0f, 1.0f, 1.0f));
	tms[5].id = 5;
	L = L + V3(30.0f, 80.0f, -20.0f);

	smppc = 0;
	smfb = 0;
	gfb = 0;

}

void Scene::Render(FrameBuffer *currfb, PPC *currppc) {

	unsigned int bgr = 0xFFFF0000;
	currfb->Clear(bgr, 0.0f);

	for (int tmi = 0; tmi < tmsN; tmi++) {
		if (currfb->id == 1) {
			continue;
		}
		if (!tms[tmi].enabled)
			continue;
//		tms[tmi].RenderPoints(ppc, fb);
//		tms[tmi].RenderWireframe(currppc, currfb);
		tms[tmi].RenderFilled(currppc, currfb);

	}

	if (currfb->id == 1) {
		float visz = 40.0f;
		ppc->Visualize(visz, currppc, currfb);
		fb->VisualizeImagePoints(visz, ppc, ppc3, fb3);
		fb->Visualize3DPoints(ppc, ppc3, fb3);
	}

	currfb->Draw3DSegment(L, L + V3(0.0f, 5.0f, 0.0f), V3(1.0f, 1.0f, 0.0f), V3(1.0f, 1.0f, 0.0f), currppc);
	currfb->Draw3DSegment(L, L + V3(5.0f, 0.0f, 0.0f), V3(1.0f, 1.0f, 0.0f), V3(1.0f, 1.0f, 0.0f), currppc);
	currfb->Draw3DSegment(L, L + V3(0.0f, 0.0f, 5.0f), V3(1.0f, 1.0f, 0.0f), V3(1.0f, 1.0f, 0.0f), currppc);

	currfb->redraw();
}

void Scene::ShadowMapSetup() {


	int smw = 128;
	smppc = new PPC(smw, smw, 55.0f);
	smppc->PositionAndOrient(L, tms[1].GetCenterOfMass(), V3(0.0f, 1.0f, 0.0f));
	FrameBuffer *tmpsmfb = new FrameBuffer(100, 100, smw, smw, 2);
	Render(tmpsmfb, smppc);
	tmpsmfb->label("Shadow Map");
	tmpsmfb->show();

	smfb = tmpsmfb;

}


void Scene::DBG() {

	{
		ppc->LoadFromTextFile("view.txt");
		Fl::check();
		ShadowMapSetup();
		Fl::check();
		RenderAll();
		Fl::check();
		int gfbw = smfb->w, gfbh = smfb->h;
		gfb = new GFB(gfbw, gfbh);
		// add sampling locations from output image
		gfb->AddSLs(ppc, fb, smppc);
		gfb->SLStats();
		// render scene from light viewpoint on generalized framebuffer
		for (int tmi = 0; tmi < tmsN; tmi++) {
			if (!tms[tmi].enabled)
				continue;
			tms[tmi].RenderGFB(smppc, gfb);
		}
		// collecting shadow information
		gfb->ApplyShadow(fb);
		fb->redraw();
		Fl::check();
		return;

	}

	{

		V3 L0 = L;
		V3 L1 = L + V3(50.0f, -20.0f, -30.0f);

		ppc->LoadFromTextFile("view.txt");
		int fsN = 30;
		for (int fi = 0; fi < fsN; fi++) {
			L = L0 + (L1 - L0)*(float)fi / (float) fsN;
			RenderAll();
			ShadowMapSetup();
			RenderAll();
			Fl::check();
		}
		L = L0;
		return;

	}

	{
		float s0 = 10000.0f;
		float s1 = 10.0f;
		for (int fi = 0; fi < 100; fi++) {
			specc = s0 + (s1 - s0)*(float)fi / 99.0f;
			RenderAll();
			Fl::check();
		}
		return;

	}

	{
		V3 L0 = tms[1].GetCenterOfMass() + V3(0.0f, 0.0f, 100.0f);
		V3 L1 = tms[1].GetCenterOfMass() + V3(0.0f, 100.0f, 0.0f);
		for (int fi = 0; fi < 100; fi++) {
			L = L0 + (L1 - L0)*(float)fi / 99.0f;
			RenderAll();
			Fl::check();
		}
		return;

	}

	{

		float len = 10.0f;
		tms[1].VisualizeNormals(ppc, fb, len);
		fb->redraw();
		return;

	}

	{

		fb->Clear(0xFFFFFFFF, 0.0f);
		fb->Draw3DPoint(ppc->C+V3(0.0f, 0.0f, -100.0f), V3(0.0f, 0.0f, 1.0f),
			11.2f, ppc);
		fb->redraw();
		return;

	}

	{

		V3 a(1.0f, 2.0f, 0.25f);
		a = a.UnitVector();
		V3 C = tms[1].GetCenterOfMass();
		for (int i = 0; i < 360; i++) {
			tms[1].RotateAboutAxis(C, a, 1.0f);
			Render(fb, ppc);
			Fl::check();
		}
		return;

	}


	{

		PPC ppc0 = *ppc;
		V3 O = tms[0].GetCenterOfMass();
		V3 newC = ppc->C + V3(50.0f, 40.0f, 30.0f);
		ppc->PositionAndOrient(newC, O, V3(0.0f, 1.0f, 0.0f));
		PPC ppc1 = *ppc;
		int framesN = 300;
		for (int fi = 0; fi < framesN; fi++) {
			float fracf = (float)fi / (float)(framesN - 1);
			ppc->SetInterpolated(&ppc0, &ppc1, fracf);
			Render(fb, ppc);
			Fl::check();
		}
		*ppc = ppc0;
		return;

	}

	{

		float hfov = 55.0f;
		PPC ppc(fb->w, fb->h, hfov);

		V3 P(0.0f, 0.0f, -100.0f);
		V3 projP;
		if (ppc.Project(P, projP)) {
			cerr << projP << endl;
			fb->DrawCircle((int) projP[0], (int) projP[1], 5.0f, 0xFF00FF00);
			fb->redraw();
		}
		else {
			cerr << "point behind head" << endl;
		}


		return;

	}

	{

		M33 m;
		m[0] = V3(1.0f, -3.0f, 7.0f);
		m[1] = V3(2.0f, 10.0f, 4.3f);
		m[2] = V3(-8.0f, -50.0f, 1.3f);

		M33 m1 = m.Inverted();
		cerr << m1*m << endl << m*m1 << endl;
		return;


	}


	{
		V3 p0(23.5f, 100.1f, 0.0f);
		V3 p1 = p0 + V3(300.0f, 0.0f, 0.0f);
		V3 p2(23.5f, 300.1f, 0.0f);
		V3 p3 = p2 + V3(300.0f, 0.0f, 0.0f);
		int stepsN = 1000;
		for (int i = 0; i < stepsN; i++) {
			float frac = (float)i / (float)stepsN;
			V3 p01 = p0 + (p1 - p0)*frac;
			V3 p32 = p3 + (p2 - p3)*frac;
			V3 p02 = p0 + (p2 - p0)*frac;
			V3 p31 = p3 + (p1 - p3)*frac;
			fb->SetBGR(0xFFFFFFFF);
			V3 c1(0.0f, 1.0f, 0.0f);
			V3 c0(1.0f, 0.0f, 0.0f);
			fb->Draw2DSegment(p01, p32, c0, c0);
			fb->Draw2DSegment(p02, p31, c1, c1);
			fb->redraw();
			Fl::check();
		}
		return;

	}


	{

		M33 m;
		m[0] = V3(1.0f, 0.0f, 0.0f);
		m[1] = V3(0.0f, 1.0f, 0.0f);
		m[2] = V3(0.0f, 0.0f, 1.0f);
		cerr << m << endl;
		V3 v(2.0f, 10.0f, -1.0f);
		cerr << m*v << endl;
		return;

	}

	{
		int u0 = 100;
		int v0 = 200;
		int u1 = 500;
		int v1 = 400;
		float r = 34.3f;
		int stepsN = 100;
		unsigned int color = 0xFF00FFFF;
		for (int stepi = 0; stepi < stepsN; stepi++) {
			int curru = u0 + (u1 - u0)*stepi / stepsN;
			int currv = v0 + (v1 - v0)*stepi / stepsN;
			fb->SetBGR(0xFFFFFFFF);
			fb->DrawCircle(curru, currv, r, color);
			fb->redraw();
			Fl::check();
		}
		return;
	}

	{
		V3 v0(4.0f, 3.0f, 0.0f);
		V3 v1(5.0f, 2.0f, 1.0f);
		cerr << v0.Length() << endl;
		return;
		cerr << v0 << endl << v1 << endl << v0-v1 << endl;
		return;
		v0[0] = 3.0f;
		cerr << "v0[0]= " << v0[0] << endl;
		return;
	}

	{
		int u0 = 20;
		int v0 = 40;
		int u1 = 400;
		int v1 = 200;
		unsigned int color = 0xFFFF0000;
		fb->Draw2DRectangle(u0, v0, u1, v1, color);
	}

	fb->redraw();
	return;

	for (int u = 0; u < fb->w; u++) {
		fb->Set(u, fb->h / 2, 0xFF000000);
	}

	fb->redraw();

}


void Scene::NewButton() {
	cerr << "INFO: pressed New Button" << endl;
	ppc->SaveToTextFile("view.txt");
}

void Scene::RenderAll() {

	Render(fb, ppc);
//	Render(fb3, ppc3);

}