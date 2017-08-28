#include "scene.h"

Scene *scene;

using namespace std;

#include <iostream>

Scene::Scene() {

	gui = new GUI();
	gui->show();

	int u0 = 20;
	int v0 = 50;
	int sci = 2;
	int w = sci * 320;
	int h = sci * 240;

	fb = new FrameBuffer(u0, v0, w, h);
	fb->label("SW framebuffer");
	fb->show();
	gui->uiw->position(fb->w + 20 + 20, 50);

	Render();
}

void Scene::Render() {

	fb->redraw();
}

void Scene::DBG() {
	cerr << "INFO: pressed DBG Button" << endl;
}
