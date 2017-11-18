#include "m33.h"

V3& M33::operator[](int rowi) {

	return rows[rowi];

}

ostream& operator<<(ostream& ostr, M33 m) {

	return ostr << m[0] << endl << m[1] << endl << m[2] << endl;

}

V3 M33::operator*(V3 v) {

	M33 &m = *this;
	return V3(m[0] * v, m[1] * v, m[2] * v);

}

M33 M33::operator*(M33 m1) {

	M33 ret, &m = *this;
	ret.SetColumn(0, m*m1.GetColumn(0));
	ret.SetColumn(1, m*m1.GetColumn(1));
	ret.SetColumn(2, m*m1.GetColumn(2));
	return ret;

}

M33 M33::Inverted() {

	M33 ret;
	V3 a = GetColumn(0), b = GetColumn(1), c = GetColumn(2);
	V3 _a = b ^ c; _a = _a / (a * _a);
	V3 _b = c ^ a; _b = _b / (b * _b);
	V3 _c = a ^ b; _c = _c / (c * _c);
	ret[0] = _a;
	ret[1] = _b;
	ret[2] = _c;

	return ret;

}

V3 M33::GetColumn(int i) {

	M33 &m = *this;
	return V3(m[0][i], m[1][i], m[2][i]);

}

void M33::SetColumn(int i, V3 v) {

	M33 &m = *this;
	m[0][i] = v[0];
	m[1][i] = v[1];
	m[2][i] = v[2];

}


void M33::SetRotationAboutY(float theta) {

	float thetar = theta / 180.0f * 3.1415f;
	M33 &m = *this;
	m[0] = V3(cosf(thetar), 0.0f, sinf(thetar));
	m[1] = V3(0.0f, 1.0f, 0.0f);
	m[2] = V3(-sinf(thetar), 0.0f, cosf(thetar));


}

M33 M33::Transposed() {

	M33 ret;
	ret[0] = GetColumn(0);
	ret[1] = GetColumn(1);
	ret[2] = GetColumn(2);
	return ret;
}