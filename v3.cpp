#include "stdafx.h"

#include "v3.h"
#include "m33.h"

V3::V3(float x, float y, float z) {

	xyz[0] = x;
	xyz[1] = y;
	xyz[2] = z;

}

float& V3::operator[](int i) {

	return xyz[i];

}


V3 V3::operator-(V3 v1) {
	V3 ret;
	ret[0] = xyz[0] - v1[0];
	ret[1] = xyz[1] - v1[1];
	ret[2] = xyz[2] - v1[2];
	return ret;
}

V3 V3::operator+(V3 v1) {
	V3 ret;
	ret[0] = xyz[0] + v1[0];
	ret[1] = xyz[1] + v1[1];
	ret[2] = xyz[2] + v1[2];
	return ret;
}

ostream& operator<<(ostream &os, V3 v) {

	os << v[0] << " " << v[1] << " " << v[2];
	return os;

}

istream& operator>>(istream &is, V3& v) {

	is >> v[0] >> v[1] >> v[2];
	return is;

}

float V3::operator*(V3 v1) {

	V3 &v0 = *this;
	return v0[0] * v1[0] + v0[1] * v1[1] + v0[2] * v1[2];

}

float V3::Length() {

	V3 &v = *this;
	return sqrtf(v*v);


}

V3 V3::operator/(float scf) {

	V3 &v = *this;
	return V3(v[0] / scf, v[1] / scf, v[2] / scf);

}

V3 V3::operator*(float scf) {

	V3 &v = *this;
	return V3(v[0] * scf, v[1] * scf, v[2] * scf);

}

V3 V3::operator^(V3 v2) {

	V3 &v1 = *this;
	V3 ret(v1[1] * v2[2] - v1[2] * v2[1],
		v1[2] * v2[0] - v1[0] * v2[2],
		v1[0] * v2[1] - v1[1] * v2[0]);
	return ret;
}

unsigned int V3::GetColor() {

	V3 &v = *this;
	unsigned int ret = 0xFF000000;
	unsigned char *rgba = (unsigned char*)&ret;
	for (int i = 0; i < 3; i++) {
		int ichan = (int)(255.0f*v[i]);
		ichan = (ichan < 0) ? 0 : ichan;
		ichan = (ichan > 255) ? 255 : ichan;
		rgba[i] = ichan;
	}

	return ret;

}


void V3::SetColor(unsigned int color) {

	V3 &v = *this;
	unsigned char*rgba = (unsigned char*)&color;
	v[0] = (float)rgba[0] / 255.0f;
	v[1] = (float)rgba[1] / 255.0f;
	v[2] = (float)rgba[2] / 255.0f;

}

V3 V3::UnitVector() {

	return (*this) / Length();

}


V3 V3::RotatePointAboutAxis(V3 O, V3 a, float theta) {

	V3 aux;
	a = a.UnitVector();
	if (fabsf(V3(1.0f, 0.0f, 0.0f)*a) < fabsf(V3(0.0f, 1.0f, 0.0f)*a))
		aux = V3(1.0f, 0.0f, 0.0f);
	else
		aux = V3(0.0f, 1.0f, 0.0f);
	V3 c = (a^aux).UnitVector();
	V3 b = (a^c).UnitVector();

	// (c, a, b) local coordinate system
	V3 &P = *this;
	M33 cs;
	cs[0] = c;
	cs[1] = a;
	cs[2] = b;
	V3 PL = cs * (P - O);
	M33 rot;
	rot.SetRotationAboutY(theta);
	V3 PLR = rot * PL;
	V3 PR = cs.Inverted()*PLR + O;

	return PR;

}


V3 V3::RotateDirectionAboutDirection(V3 a, float theta) {

	V3 aux;
	a = a.UnitVector();
	if (fabsf(V3(1.0f, 0.0f, 0.0f)*a) < fabsf(V3(0.0f, 1.0f, 0.0f)*a))
		aux = V3(1.0f, 0.0f, 0.0f);
	else
		aux = V3(0.0f, 1.0f, 0.0f);
	V3 c = (a^aux).UnitVector();
	V3 b = (a^c).UnitVector();

	// (c, a, b) local coordinate system
	V3 &P = *this;
	M33 cs;
	cs[0] = c;
	cs[1] = a;
	cs[2] = b;
	V3 PL = cs * P;
	M33 rot;
	rot.SetRotationAboutY(theta);
	V3 PLR = rot * PL;
	V3 PR = cs.Inverted()*PLR;

	return PR;

}

