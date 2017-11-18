#include "framebuffer.h"
#include "math.h"
#include <iostream>
#include "scene.h"
#include "v3.h"
#include <iostream>

using namespace std;

FrameBuffer::FrameBuffer(int u0, int v0,
	int _w, int _h, unsigned int _id) : Fl_Gl_Window(u0, v0, _w, _h, 0) {

	isHW = 0;
	id = _id;
	w = _w;
	h = _h;
	pix = new unsigned int[w*h];
	zb = new float[w*h];

}


void FrameBuffer::draw() {

	if (!isHW)
		glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, pix);
	else if (isHW == 1) // fixed pipeline
		scene->RenderHW();
	else
		scene->RenderGPU(); // programmable pipeline
}

int FrameBuffer::handle(int event) {

	switch (event)
	{
	case FL_KEYBOARD: {
		KeyboardHandle();
		return 0;
	}
	default:
		break;
	}
	return 0;
}

void FrameBuffer::KeyboardHandle() {

	PPC *currppc;
	FrameBuffer *currfb = this;
	if (id == 0) {
		currppc = scene->ppc;
	}
	else {
		currppc = scene->ppc3;
	}

	float tstep = 3.0f;
	float rstep = 3.0f;
	int key = Fl::event_key();
	switch (key) {
	case ',': {
		currppc->Translate(currppc->a.UnitVector()*(-tstep));
		scene->RenderAll();
		break;
	}
	case '.': {
		currppc->Translate(currppc->a.UnitVector()*tstep);
		scene->RenderAll();
		break;
	}
	case FL_Up: {
		currppc->Translate(currppc->GetVD()*tstep);
		scene->RenderAll();
		break;
	}
	case FL_Down: {
		currppc->Translate(currppc->GetVD()*-tstep);
		scene->RenderAll();
		break;
	}
	case 'q': {
		currppc->Translate(currppc->b.UnitVector()*(-tstep));
		scene->RenderAll();
		break;
	}
	case 'a': {
		currppc->Translate(currppc->b.UnitVector()*tstep);
		scene->RenderAll();
		break;
	}
	case FL_Right: {
		currppc->Pan(rstep);
		scene->RenderAll();
		break;
	}
	case FL_Left: {
		currppc->Pan(-rstep);
		scene->RenderAll();
		break;
	}
	case 'w': {
		currppc->Tilt(-rstep);
		scene->RenderAll();
		break;
	}
	case 's': {
		currppc->Tilt(rstep);
		scene->RenderAll();
		break;
	}
	case 'o': {
		currppc->Roll(rstep);
		scene->RenderAll();
		break;
	}
	case 'p': {
		currppc->Roll(-rstep);
		scene->RenderAll();
		break;
	}
	default:
		cerr << "INFO: do not understand keypress" << endl;
	}
}

void FrameBuffer::Set(V3 uv, unsigned int color) {

	SetGuarded((int)uv[0], (int)uv[1], color);

}

void FrameBuffer::Set(int u, int v, unsigned int color) {

	pix[(h - 1 - v)*w + u] = color;

}

void FrameBuffer::SetGuarded(int u, int v, unsigned int color) {

	if (u < 0 || u >= w || v < 0 || v >= h)
		return;
	Set(u, v, color);

}

void FrameBuffer::SetBGR(unsigned int bgr) {

	for (int uv = 0; uv < w*h; uv++)
		pix[uv] = bgr;

}

void FrameBuffer::Draw2DRectangle(int u0, int v0, int u1, int v1, unsigned int color) {

	for (int v = v0; v <= v1; v++) {
		for (int u = u0; u <= u1; u++) {
			SetGuarded(u, v, color);
		}
	}



}

void FrameBuffer::DrawCircle(int u0, int v0, float r, unsigned int color) {

	int left = (int)(u0 - r);
	if (left < 0)
		left = 0;
	if (left > w - 1)
		return;
	int right = (int)(u0 + r);
	if (right < 0)
		return;
	if (right > w - 1)
		right = w-1;

	int top = (int)(v0 - r);
	if (top < 0)
		top = 0;
	if (top > h - 1)
		return;
	int bottom = (int)(v0 + r);
	if (bottom < 0)
		return;
	if (bottom > h - 1)
		bottom = h - 1;

	V3 cc(.5f + (float)u0, .5f + (float)v0, 0.0f);
	for (int v = top; v <= bottom; v++) {
		for (int u = left; u <= right; u++) {
			V3 cpc(.5f + (float)u, .5f + (float)v, 0.0f);
			if ((cpc - cc).Length() <= r)
				Set(u, v, color);
		}
	}


}

void FrameBuffer::Draw2DSegment(V3 p0, V3 p1, V3 c0, V3 c1) {

	float du = fabsf(p0[0] - p1[0]);
	float dv = fabsf(p0[1] - p1[1]);
	int stepsN;
	if (du > dv) {
		stepsN = (int)(du + 2);
	}
	else {
		stepsN = (int)(dv + 2);
	}

	V3 duv = (p1 - p0) / (float) (stepsN-1);
	V3 dc = (c1 - c0) / (float) (stepsN-1);
	for (int stepi = 0; stepi < stepsN; stepi++) {
		V3 currp = p0 + duv * (float)stepi;
		V3 currc = c0 + dc * (float)stepi;
		SetZB(currp, currc.GetColor());
	}

}

void FrameBuffer::Draw3DSegment(V3 p0, V3 p1, V3 c0, V3 c1, PPC *ppc) {

	V3 pp0, pp1;
	if (!ppc->Project(p0, pp0))
		return;
	if (!ppc->Project(p1, pp1))
		return;

	Draw2DSegment(pp0, pp1, c0, c1);

}

void FrameBuffer::Clear(unsigned int bgr, float z0) {

	SetBGR(bgr);
	for (int i = 0; i < w*h; i++)
		zb[i] = z0;

}

int FrameBuffer::IsVisible(V3 uvz) {

	int u = (int)uvz[0];
	int v = (int)uvz[1];
	if (u < 0 || v < 0 || u > w - 1 || v > h - 1)
		return 1;

	int uv = (h - 1 - v)*w + u;
	if (zb[uv] > 0.1f + uvz[2])
		return 0;

	return 1;

}

void FrameBuffer::SetZB(V3 uvz, unsigned int color) {

	int u = (int)uvz[0];
	int v = (int)uvz[1];
	if (u < 0 || v < 0 || u > w - 1 || v > h - 1)
		return;

	int uv = (h - 1 - v)*w + u;
	if (zb[uv] > uvz[2])
		return;
	zb[uv] = uvz[2];

	Set(uvz, color);

}

void FrameBuffer::Draw3DPoint(V3 p, V3 c, float psize, PPC *ppc) {

	V3 pp;
	if (!ppc->Project(p, pp))
		return;
	DrawCircle(pp, psize, c);

}

void FrameBuffer::DrawCircle(V3 c, float r, V3 cv) {

	int left = (int)(c[0] - r);
	int right = (int)(c[0] + r);
	int top = (int)(c[1] - r);
	int bottom = (int)(c[1] + r);
	unsigned int color = cv.GetColor();
	for (int v = top; v <= bottom; v++) {
		for (int u = left; u <= right; u++) {
			V3 currp(.5f+(float)u, .5f + (float) v, c[2]);
			if ((currp - c).Length() < r) {
				SetZB(currp, color);
			}
		}
	}
}

unsigned int FrameBuffer::Get(int u, int v) {

	return pix[(h - v - 1)*w + u];

}

void FrameBuffer::VisualizeImagePoints(float visz, PPC *ppc, PPC *visppc, 
	FrameBuffer *visfb) {

	for (int v = 0; v < h; v++) {
		for (int u = 0; u < w; u++) {
			int uv = (h - 1 - v)*w + u;
			if (zb[uv] == 0.0f)
				continue;
			V3 P = ppc->GetPoint(.5f + (float)u, .5f + (float)v, visz);
			V3 c; c.SetColor(Get(u, v));
			visfb->Draw3DPoint(P, c, 1.0f, visppc);
		}
	}

}

void FrameBuffer::Visualize3DPoints(PPC *ppc, PPC *visppc, FrameBuffer *visfb) {

	for (int v = 0; v < h; v++) {
		for (int u = 0; u < w; u++) {
			int uv = (h - 1 - v)*w + u;
			if (zb[uv] == 0.0f)
				continue;
			V3 pp = V3(.5f + (float)u, .5f + (float)v, zb[uv]);
			V3 P = ppc->Unproject(pp);
			V3 c; c.SetColor(Get(u, v));
			visfb->Draw3DPoint(P, c, 1.0f, visppc);
		}
	}

}



void FrameBuffer::SetBWCheckerboard(int csize) {

	for (int v = 0; v < h; v++) {
		for (int u = 0; u < w; u++) {
			int cu = u / csize;
			int cv = v / csize;
			if ((cu + cv) % 2)
				Set(u, v, 0xFFFFFFFF);
			else
				Set(u, v, 0xFF000000);
		}
	}


}

unsigned int FrameBuffer::LookUpNN(float s, float t) {

	int u = (int)((float)w*s);
	int v = (int)((float)h*t);
	if (u < 0 || u > w - 1 || v < 0 || v > h - 1)
		return 0xFFFFFFFF;
	return Get(u, v);

}

unsigned int FrameBuffer::LookUpBilinear(float s, float t) {

	float uf = (float)w*s;
	float vf = (float)h*t;
	if (uf < 0.5f || uf >(float) w - 0.5f || vf < 0.5f || vf >(float)h - 0.5f)
		return 0xFFFFFFFF;
	int u0 = (int)(uf - 0.5f);
	int v0 = (int)(vf - 0.5f);
	float du = uf - ((float)u0 + 0.5f);
	float dv = vf - ((float)v0 + 0.5f);

	V3 c00; c00.SetColor(Get(u0, v0));
	V3 c10; c10.SetColor(Get(u0+1, v0));
	V3 c11; c11.SetColor(Get(u0+1, v0+1));
	V3 c01; c01.SetColor(Get(u0, v0+1));

	V3 ret =
		c00*(1.0f - du)*(1.0f - dv) +
		c10*du*(1.0f - dv) +
		c11*du*dv +
		c01*(1.0f - du)*dv;

	return ret.GetColor();

}

