// project_abyss
// LittleSpace Studio 2012

#include "GameCamera.h"

GameCamera::GameCamera()
{
	Reset();
	debugFont = (mk::Font*)mk::RessourceManager::getInstance()->LoadRessource("fonts/fontface.fnt");
}

void GameCamera::Reset()
{
	camX = camY = 0.0f;
}

void GameCamera::Set()
{
	lowCamLookAt(camX, camY, camZ, camX, camY, 0.0f, 0.0f, -1.0f, 0.0f);
}

void GameCamera::DebugMove()
{
	mk::Input *input = mk::InputManager::GetInput(0, CNT_KEYBOARD);

	/*if(input->buttons[mk::ButtonNames::Up].held)
		camY -= 50.0f*lowGetFrameTime();
	if(input->buttons[mk::ButtonNames::Down].held)
		camY += 50.0f*lowGetFrameTime();
	if(input->buttons[mk::ButtonNames::Left].held)
		camX -= 50.0f*lowGetFrameTime();
	if(input->buttons[mk::ButtonNames::Right].held)
		camX += 50.0f*lowGetFrameTime();*/
	if(input->buttons[mk::ButtonNames::A].held)
		camZ += 0.5f;
	if(input->buttons[mk::ButtonNames::B].held)
		camZ -= 0.5f;

	static float mouseX = 0;
	static float mouseY = 0;

	if(input->buttons[mk::ButtonNames::RightClick].pressed)
	{
		mouseX = input->pointer.x;
		mouseY = input->pointer.y;
	}

	if(input->buttons[mk::ButtonNames::RightClick].held)
	{
		camX -= (mouseX - input->pointer.x)/2.0f*lowGetFrameTime();
		camY -= (mouseY - input->pointer.y)/2.0f*lowGetFrameTime();
	}

	if(camZ >= -10)
		camZ = -10;
}

void GameCamera::ShowDebugInfo(float x, float y)
{
	char text[64];
	sprintf(text, "Camera X:%d, Y:%d, Z:%d", (int)camX, (int)camY, (int)camZ);
	lowDisplayText(debugFont, x, y, text, 0xAAAAAAFF, 0.8f);
}

NVector GameCamera::Get3DPointScreenCoordinates(float x, float y, float z)
{
	// Méthode opengl
	// TODO : pas directement portable
	lowPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	lowCamLookAt(camX, camY, camZ, camX, camY, 0.0f, 0.0f, -1.0f, 0.0f);

	GLdouble model_view[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, model_view);
	
	GLdouble projection[16];
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	GLdouble pos3D_x, pos3D_y, pos3D_z;
	gluProject(x, y, z,
		model_view, projection, viewport,
		&pos3D_x, &pos3D_y, &pos3D_z);

	// Correction des coordonnées
	//pos3D_x -= mk::Core::getBaseWidth()/2.0f;
	pos3D_y = mk::Core::getBaseHeight() - pos3D_y;

	lowPopMatrix();

	return NVector(pos3D_x, pos3D_y);
}

NVector GameCamera::GetUnProject(float x, float y, float z)
{
	// Méthode opengl
	// TODO : pas directement portable
	lowPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	lowCamLookAt(camX, camY, camZ, camX, camY, 0.0f, 0.0f, -1.0f, 0.0f);

	GLdouble model_view[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, model_view);
	
	GLdouble projection[16];
	glGetDoublev(GL_PROJECTION_MATRIX, projection);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	GLdouble pos3D_x, pos3D_y, pos3D_z;
	gluUnProject(x, y, z,
		model_view, projection, viewport,
		&pos3D_x, &pos3D_y, &pos3D_z);

	// Correction des coordonnées
	//pos3D_x -= mk::Core::getBaseWidth()/2.0f;
	//pos3D_y = mk::Core::getBaseHeight() - pos3D_y;

	lowPopMatrix();

	return NVector(pos3D_x, pos3D_y);
}