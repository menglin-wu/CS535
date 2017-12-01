#include "stdafx.h"

#include "gfb.h"
#include "ppc.h"
#include "framebuffer.h"

GFB::GFB(int _w, int _h) {

	w = _w;
	h = _h;
	gpix = new GPix[w*h];
	visfb = new FrameBuffer(10, 10, w, h, 0);
	visfb->label("heat map vis");
	visfb->Clear(0xFF000000, 0.0f);
	visfb->show();
}

void GFB::AddSL(V3 PP, int uv) {
	float uf = PP[0];
	float vf = PP[1];
	if (uf < 0.0f || uf >(float)w || vf < 0.0f || vf >(float)h)
		return;
	int u = (int)uf;
	int v = (int)vf;
	gpix[(h - 1 - v)*w + u].AddSL(PP, uv);

}

void GPix::AddSL(V3 PP, int uv) {

	if (slsN == maxN) {
		maxN = (maxN + 1) * 2;
		SL *tmp = new SL[maxN];
		for (int sli = 0; sli < slsN; sli++) {
			tmp[sli] = sls[sli];
		}
		delete[]sls;
		sls = tmp;
	}

	sls[slsN].uv1 = V3(PP[0], PP[1], 1.0f);
	sls[slsN].z = PP[2];
	sls[slsN].inShadow = 0;
	sls[slsN].uv = uv;
	slsN++;

}

void GPix::Clear() {

	delete[]sls;
	slsN = 0;
	maxN = 0;
}

void GFB::Clear() {

	for (int v = 0; v < h; v++) {
		for (int u = 0; u < w; u++) {
			gpix[(h - 1 - v)*w + u].Clear();
		}
	}

}

void GFB::SLStats() {

	int slsN = 0;
	int maxN = gpix[0].slsN;
	for (int v = 0; v < h; v++) {
		for (int u = 0; u < w; u++) {
			int uv = (h - 1 - v)*w + u;
			slsN += gpix[uv].slsN;
			if (gpix[uv].slsN > maxN)
				maxN = gpix[uv].slsN;
		}
	}

	for (int v = 0; v < h; v++) {
		for (int u = 0; u < w; u++) {
			int uv = (h - 1 - v)*w + u;
			if (gpix[uv].slsN == 0) {
				visfb->Set(u, v, 0xFF000000);
				continue;
			}
			V3 hotColor(1.0f, 0.0f, 0.0f);
			V3 coldColor(0.0f, 0.0f, 1.0f);
			V3 currColor = coldColor + (hotColor - coldColor)*(float)gpix[uv].slsN / (float)10;
			visfb->Set(u, v, currColor.GetColor());
		}
	}

	cerr << "INFO: sampling locations N: " << slsN << endl;
	cerr << "INFO: max: " << maxN << endl;

}

void GFB::AddSLs(PPC *ppc, FrameBuffer *fb, PPC *smppc) {

	for (int v = 0; v < fb->h; v++) {
		for (int u = 0; u < fb->w; u++) {
			int uv = (fb->h - 1 - v)*fb->w + u;
			if (fb->zb[uv] == 0.0f)
				continue;
			V3 fbPP((float)u + 0.5f, (float)v + 0.5f, fb->zb[uv]);
			V3 P = ppc->Unproject(fbPP);
			V3 PP;
			if (!smppc->Project(P, PP))
				continue;
			AddSL(PP, uv);
		}
	}

}

void GPix::ApplyShadow(FrameBuffer *fb) {

	for (int sli = 0; sli < slsN; sli++) {
		if (sls[sli].inShadow) {
			V3 oldColor;
			oldColor.SetColor(fb->pix[sls[sli].uv]);
			V3 shadowColor(0.0f, 1.0f, 0.0f);
			V3 newColor = (oldColor + shadowColor) / 2.0f;
			fb->pix[sls[sli].uv] = newColor.GetColor();
		}
	}

}

void GFB::ApplyShadow(FrameBuffer *fb) {

	for (int v = 0; v < h; v++) {
		for (int u = 0; u < w; u++) {
			int uv = (h - 1 - v)*w + u;
			gpix[uv].ApplyShadow(fb);
		}
	}

}