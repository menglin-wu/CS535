#include "framebuffer.h"
#include "math.h"
#include <iostream>
#include "scene.h"

#include <iostream>

using namespace std;

FrameBuffer::FrameBuffer(int u0, int v0,
	int _w, int _h) : Fl_Gl_Window(u0, v0, _w, _h, 0) {

	w = _w;
	h = _h;
	pix = new unsigned int[w*h];

}


void FrameBuffer::draw() {

	glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, pix);
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

	float tstep = 3.0f;
	float rstep = 3.0f;
	int key = Fl::event_key();
	switch (key) {
	case FL_Left: {
		cerr << "INFO: pressed left" << endl;
		break;
	}
	case FL_Right: {
		break;
	}
	case 'a': {
		cerr << "INFO: pressed a" << endl;
		break;
	}
	default:
		cerr << "INFO: do not understand keypress" << endl;
	}
}

