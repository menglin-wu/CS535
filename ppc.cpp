#include "ppc.h"
#include "m33.h"
#include "framebuffer.h"

#include <fstream>

PPC::PPC(int _w, int _h, float hfov) : w(_w), h(_h) {

	C = V3(0.0f, 0.0f, 0.0f);
	a = V3(1.0f, 0.0f, 0.0f);
	b = V3(0.0f, -1.0f, 0.0f);
	float h2 = hfov*3.1415926f / 180.0f / 2.0f;
	c = V3(-0.5f*(float)w, 0.5f*(float)h, -(float)w / (2.0f*tanf(h2)));

}

int PPC::Project(V3 P, V3& projP) {

	M33 cam;
	cam.SetColumn(0, a);
	cam.SetColumn(1, b);
	cam.SetColumn(2, c);

	V3 q = cam.Inverted()*(P - C);

	if (q[2] <= 0.0f)
		return 0;

	projP = q / q[2];
	projP[2] = 1.0f / q[2];

	return 1;

}

float PPC::GetF() {

	float f = GetVD()*c;
	return f;

}

void PPC::PositionAndOrient(V3 newC, V3 lap, V3 up) {

	V3 newa, newb, newc;
	V3 newvd = (lap - newC).UnitVector();
	float f = GetF();
	newa = (newvd ^ up).UnitVector()*a.Length();
	newb = (newvd ^ newa).UnitVector()*b.Length();
	newc = newvd*f - newb * (float)h/2.0f - newa * (float)w/2.0f;

	// commit the new values
	C = newC;
	a = newa;
	b = newb;
	c = newc;

}

V3 PPC::GetVD() {

	return (a^b).UnitVector();


}

void PPC::SetInterpolated(PPC *ppc0, PPC *ppc1, float fracf) {

	*this = *ppc0;

	V3 newC = ppc0->C + (ppc1->C - ppc0->C)*fracf;
	V3 newvd = ppc0->GetVD() + (ppc1->GetVD() - ppc0->GetVD())*fracf;
	V3 up = (ppc0->b + (ppc1->b- ppc0->b)*fracf)*-1.0f;
	PositionAndOrient(newC, newC + newvd, up);

}



void PPC::Pan(float theta) {

	a = a.RotateDirectionAboutDirection(b.UnitVector()*-1.0f, theta);
	c = c.RotateDirectionAboutDirection(b.UnitVector()*-1.0f, theta);

}

void PPC::Tilt(float theta) {

	b = b.RotateDirectionAboutDirection(a.UnitVector(), theta);
	c = c.RotateDirectionAboutDirection(a.UnitVector(), theta);

}


void PPC::Roll(float theta) {

	a = a.RotateDirectionAboutDirection(GetVD(), theta);
	b = b.RotateDirectionAboutDirection(GetVD(), theta);
	c = c.RotateDirectionAboutDirection(GetVD(), theta);

}

void PPC::Visualize(float vf, PPC *vppc, FrameBuffer *fb) {

	float pointSize = 7.0f;
	V3 red(1.0f, 0.0f, 0.0f);
	V3 black(0.0f, 0.0f, 0.0f);
	fb->Draw3DPoint(C, red, pointSize, vppc);
	float scf = vf / GetF();

	fb->Draw3DSegment(C, C + c*scf, red, black, vppc);

	V3 cs[4];
	cs[0] = GetPoint(0.0f, 0.0f, vf);
	cs[1] = GetPoint((float)w, 0.0f, vf);
	cs[2] = GetPoint((float)w, (float)h, vf);
	cs[3] = GetPoint(0.0f, (float)h, vf);

	for (int i = 0; i < 4; i++) {
		int _i = (i + 1) % 4;
		fb->Draw3DSegment(cs[i], cs[_i], black, black, vppc);
	}

}


V3 PPC::GetPoint(float uf, float vf, float z) {

	float f = GetF();
	V3 ret = C + (c + a*uf + b*vf)*z / f;
	return ret;

}

V3 PPC::Unproject(V3 pp) {

	V3 ret = C + (c + a*pp[0] + b*pp[1]) / pp[2];
	return ret;

}

void PPC::SaveToTextFile(char *fname) {

	ofstream ofs(fname);
	ofs << a << endl << b << endl << c << endl << C << endl;
	ofs.close();

}

void PPC::LoadFromTextFile(char *fname) {

	ifstream ifs(fname);
	if(ifs.fail()) {
		cerr << "INFO: cannot open file: " << fname << endl;
		return;
	}
	ifs >> a >> b >> c >> C;
	ifs.close();

}

void PPC::SetIntrinsicsHW(float zNear, float zFar) {

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float left, right, bottom, top;
	float scf = zNear / GetF();
	left = -a.Length()*(float)w / 2.0f * scf;
	right = +a.Length()*(float)w / 2.0f * scf;
	bottom = -b.Length()*(float)h / 2.0f*scf;
	top = +b.Length()*(float)h / 2.0f*scf;
	glFrustum(left, right, bottom, top, zNear, zFar);

	glMatrixMode(GL_MODELVIEW);

}

void PPC::SetExtrinsicsHW() {

	V3 LAP = C + GetVD()*100.0f;
	glLoadIdentity();
	gluLookAt(C[0], C[1], C[2], LAP[0], LAP[1], LAP[2], -b[0], -b[1], -b[2]);

}

