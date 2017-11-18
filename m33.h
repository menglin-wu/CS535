#pragma once
#include "v3.h"

class M33 {
public:
	V3 rows[3];
	M33() {};
	V3& operator[](int rowi);
	friend ostream& operator<<(ostream& ostr, M33 m);
	V3 operator*(V3 v);
	M33 operator*(M33 m);
	M33 Inverted();
	V3 GetColumn(int i);
	void SetColumn(int i, V3 v);
	void SetRotationAboutY(float theta);
	M33 Transposed();
};