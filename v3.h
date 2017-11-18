#pragma once

#include <iostream>

using namespace std;


class V3 {
public:
	float xyz[3];
	V3() {};
	V3(float x, float y, float z);
	V3 operator-(V3 v1);
	V3 operator+(V3 v1);
	V3 operator/(float scf);
	V3 operator*(float scf);
	V3 operator^(V3 v1);
	float& operator[](int i);
	float operator*(V3 v1);
	float Length();
	friend ostream& operator<<(ostream &os, V3 v);
	friend istream& operator>>(istream &os, V3 &v);
	unsigned int GetColor();
	void SetColor(unsigned int color);
	V3 UnitVector();
	V3 RotatePointAboutAxis(V3 O, V3 a, float theta);
	V3 RotateDirectionAboutDirection(V3 a, float theta);
};