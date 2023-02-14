#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include "RgbImage.h"

#define _CRT_SECURE_NO_WARNINGS
#define PI 3.141592654

using namespace std;

const char* fFlamengo = "./flamengo.bmp";
const char* fMetal = "./metal.bmp";
const char* fEsfera = "./esfera.bmp";
const char* fboca = "./boca.bmp";
const char* fOlho = "./olho.bmp";
const char* fRosto = "./rosto.bmp";
const char* fBraco = "./braco.bmp";
const char* fDedo = "./dedo.bmp";
const char* fJean = "./jeans.bmp";

GLuint _jean;
GLuint _dedo;
GLuint _metal;
GLuint _braco;
GLuint _rosto;
GLuint _olho;
GLuint _torso;
GLuint _disco;
GLuint _esfera;
GLuint _boca;
GLUquadric* quadSphere;
GLUquadric* quadCylinder;

bool textureOn = true;

float centroZ = 0;
float centroY = 0;
float diameterCylinder = 0.3;
float diameterSphere = 0.4;
float sizeArm = 4.5;
float sizeKnee = 4.5;
float sizeForearm = 4.0;
float sizeLeg = 5.0;
float sizeHand = 2.0;
float sizeClampPart = 1.0;
float diameterBase = 2.0;
float heightBase = 0.5;
float torsoHeight = 12;
float diameterTorso = 5;
float diameterKneck = 0.8;
float heightKneck = 2;
float diameterHead = 2.5;
float heightHead = 4.5;
float diameterEye = 0.5;
float diameterNose = 1;
float heightNose = 1.5;
float diameterMouth = 0.4;
float heightMouth = diameterHead / 1.5;
float LLegArt = 180.0f;
float LkneeArt = 0.0f;
float RLegArt = 180.0f;
float RkneeArt = 0.0f;
float headArt = 0.0f;

float eyeDistance = 100.0;
float eyeX;
float eyeY;
float eyeZ;
float viewAngleX = 0.0;
float viewAngleZ = 15.0;
float zoom = 50;
float horizon = 0;
float vertic = 0;

float LangleArmZ = -90.0;
float LangleArmY = 0.0;
float LangleForearm = 0.0;
float LangleHand = 0.0;
float LangleClampZ = 0.0;
float LangleClampY = 0.0;
float LhandSpin = 90;

float RangleArmZ = 90.0;
float RangleArmY = 0.0;
float RangleForearm = 0.0;
float RangleHand = 0.0;
float RangleClampZ = 0.0;
float RangleClampY = 0.0;
float RhandSpin = 90;

GLfloat lightposition[4] = { 0.0f, 30.0f, 0.0f, 0.0f };
GLfloat luzAmbiente[4] = { 0.3, 0.3, 0.3, 0.0 };

//defines light source (LIGHT0)
void initLighting(void)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	// Especifica que a cor de fundo da janela será preta
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH);

	// Ativa o uso da luz ambiente 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_POSITION, lightposition);

	// Materials will follow current color
	glEnable(GL_COLOR_MATERIAL);
}

// makes the image into a texture, and returns the id of the texture
GLuint loadTexture(const char* filename) {
	GLuint textureId;

	RgbImage theTexMap(filename); // Image with texture

	glGenTextures(1, &textureId); // Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId);	// Tell OpenGL which texture to edit
	// Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,	// Always GL_TEXTURE_2D
		0,						// 0 for now
		GL_RGB,					// Format OpenGL uses for image
		theTexMap.GetNumCols(),	// Width 
		theTexMap.GetNumRows(),	// Height
		0,						// The border of the image
		GL_RGB,					// GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE,		// GL_UNSIGNED_BYTE, because pixels are stored as unsigned numbers
		theTexMap.ImageData());	// The actual pixel data
	return textureId; // Returns the id of the texture
}

void initRendering(void) {
	quadSphere = gluNewQuadric();
	quadCylinder = gluNewQuadric();
	_torso = loadTexture(fFlamengo);
	_disco = loadTexture(fMetal);
	_esfera = loadTexture(fEsfera);
	_boca = loadTexture(fboca);
	_olho = loadTexture(fOlho);
	_rosto = loadTexture(fRosto);
	_braco = loadTexture(fBraco);
	_dedo = loadTexture(fDedo);
	_jean = loadTexture(fJean);
}

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
	case 27: //Escape key
		exit(0);
	case 'w': //Increase view angle z axis
		if (viewAngleZ < 180) viewAngleZ += 3;
		glutPostRedisplay();
		break;
	case 's': //Decrease view angle z axis
		if (viewAngleZ > 0) viewAngleZ -= 3;
		glutPostRedisplay();
		break;
	case 'd': //Decrease view angle x axis
		if (viewAngleX > -180) viewAngleX -= 3;
		glutPostRedisplay();
		break;
	case 'a': //Increase view angle x axis
		if (viewAngleX < 180) viewAngleX += 3;
		glutPostRedisplay();
		break;
	case 't': //Use texture or not
		textureOn = !textureOn;
		glutPostRedisplay();
		break;
	case '1': //Increase arm angle
		if (LangleArmZ <= -90 + 45)
			LangleArmZ += 3;
		glutPostRedisplay();
		break;
	case '2': //Decrease arm angle
		if (LangleArmZ >= -180 + 45)
			LangleArmZ -= 3;
		glutPostRedisplay();
		break;
	case '3': //Decrease arm angle
		if (LangleArmY <= 90 - 15) {
			LangleArmY += 3;
		}
		glutPostRedisplay();
		break;
	case '4': //Decrease arm angle
		if (LangleArmY >= 0) {
			LangleArmY -= 3;
		}
		glutPostRedisplay();
		break;
	case '5': //Increase forearm angle
		if (LangleForearm < 90) LangleForearm += 3;
		glutPostRedisplay();
		break;
	case '6': //Decrease forearm angle
		if (LangleForearm > 0) LangleForearm -= 3;
		glutPostRedisplay();
		break;
	case '7': //Decrease forearm angle
		if (LhandSpin > -180) LhandSpin -= 3;
		glutPostRedisplay();
		break;
	case '8': //Decrease forearm angle
		if (LhandSpin < 180) LhandSpin += 3;
		glutPostRedisplay();
		break;
	case '9': //Increase clamp angle y axis
		if (LangleClampY < 60) LangleClampY += 3;
		glutPostRedisplay();
		break;
	case '0': //Decrease clamp angle y axis
		if (LangleClampY > 0) LangleClampY -= 3;
		glutPostRedisplay();
		break;
	case 92: //Increase arm angle
		if (RangleArmZ >= 90 - 45)
			RangleArmZ -= 3;
		glutPostRedisplay();
		break;
	case 'z': //Decrease arm angle
		if (RangleArmZ <= 90 + 45)
			RangleArmZ += 3;
		glutPostRedisplay();
		break;
	case 'x': //Decrease arm angle
		if (RangleArmY <= 90 - 15) {
			RangleArmY += 3;
		}
		glutPostRedisplay();
		break;
	case 'c': //Decrease arm angle
		if (RangleArmY >= 0) {
			RangleArmY -= 3;
		}
		glutPostRedisplay();
		break;
	case 'v': //Increase forearm angle
		if (RangleForearm < 90) RangleForearm += 3;
		glutPostRedisplay();
		break;
	case 'b': //Decrease forearm angle
		if (RangleForearm > 0) RangleForearm -= 3;
		glutPostRedisplay();
		break;
	case 'n': //Decrease forearm angle
		if (RhandSpin > -180) RhandSpin -= 3;
		glutPostRedisplay();
		break;
	case 'm': //Decrease forearm angle
		if (RhandSpin < 180) RhandSpin += 3;
		glutPostRedisplay();
		break;
	case ',': //Increase clamp angle y axis
		if (RangleClampY < 60) RangleClampY += 3;
		glutPostRedisplay();
		break;
	case '.': //Decrease clamp angle y axis
		if (RangleClampY > 0) RangleClampY -= 3;
		glutPostRedisplay();
		break;
	case 'y': //Increase Leg angle 
		if (LLegArt < 180 + 75) LLegArt += 3;
		glutPostRedisplay();
		break;
	case 'u': //Decrease Leg angle
		if (LLegArt > 180) LLegArt -= 3;
		glutPostRedisplay();
		break;
	case 'i': //Increase knee angle
		if (LkneeArt < 90) LkneeArt += 3;
		glutPostRedisplay();
		break;
	case 'o': //Decrease knee angle
		if (LkneeArt > 0) LkneeArt -= 3;
		glutPostRedisplay();
		break;
	case 'h': //Increase Leg angle 
		if (RLegArt < 180 + 75) RLegArt += 3;
		glutPostRedisplay();
		break;
	case 'j': //Decrease Leg angle
		if (RLegArt > 180) RLegArt -= 3;
		glutPostRedisplay();
		break;
	case 'k': //Increase knee angle
		if (RkneeArt < 90) RkneeArt += 3;
		glutPostRedisplay();
		break;
	case 'l': //Decrease knee angle
		if (RkneeArt > 0) RkneeArt -= 3;
		glutPostRedisplay();
		break;
	case 'q': //Increase head angle
		if (headArt < 90) headArt += 3;
		glutPostRedisplay();
		break;
	case 'e': //Decrease head angle
		if (headArt > -90) headArt -= 3;
		glutPostRedisplay();
		break;
	case '=': //
		if (zoom > 4) zoom -= 2;
		glutPostRedisplay();
		break;
	case '-': //
		if (zoom < 60) zoom += 2;
		glutPostRedisplay();
		break;
	}
}

void glutInput(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		if (centroZ < 50) centroZ += 2;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:
		if (centroZ > -50) centroZ -= 2;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:
		if (centroY < 50) centroY += 2;
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT:
		if (centroY > -50) centroY -= 2;
		glutPostRedisplay();
		break;
	default:
		exit(0);
		break;
	}
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

void drawCylinder(float diameter, float lenght, GLuint texture) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else {
		gluQuadricTexture(quadCylinder, 0);
	}

	gluCylinder(quadCylinder, diameter, diameter, lenght, 40.0, lenght * 30.0);
}

void drawCone(float diameter, float lenght) {
	if (textureOn) {
		//glBindTexture(GL_TEXTURE_2D, _textureIdCylinder);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else
		gluQuadricTexture(quadCylinder, 0);
	gluCylinder(quadCylinder, diameter, 0, lenght, 40.0, lenght * 30.0);
}


void drawConeD(float diameter, float lenght) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _dedo);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else
		gluQuadricTexture(quadCylinder, 0);
	gluCylinder(quadCylinder, diameter, 0, lenght, 40.0, lenght * 30.0);
}

void drawDisk(float diameterInner, float diameterOuter, GLuint texture) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else
		gluQuadricTexture(quadCylinder, 0);
	gluDisk(quadCylinder, diameterInner, diameterOuter, 40.0, 30.0);
}

void drawSphere(float diameter, GLuint texture) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadSphere, 1);
	}
	else
		gluQuadricTexture(quadSphere, 0);
	gluSphere(quadSphere, diameter, 40.0, 40.0);
}

void drawScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(zoom, 1, 20, 200);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	eyeX = eyeDistance * cos(viewAngleZ * PI / 180) * cos(viewAngleX * PI / 180);
	eyeY = eyeDistance * cos(viewAngleZ * PI / 180) * sin(viewAngleX * PI / 180);
	eyeZ = eyeDistance * sin(viewAngleZ * PI / 180);

	if (viewAngleZ < 90) {
		gluLookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 0.0, 0.0, 0.0, 1);
	}
	else {
		gluLookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 0.0, 0.0, 0.0, -1);
	}

	glLightfv(GL_LIGHT0, GL_POSITION, lightposition);

	// drawing color
	glColor3f(1.0f, 1.0f, 1.0f);

	// Origem
	glPushMatrix();

	//Perna esquerda
	glTranslatef(0.0f, centroY + 4.0f, centroZ);
	glTranslatef(0.0f, 0.0f, heightBase);
	glTranslatef(0.0f, 0.0f, sizeKnee + diameterSphere / 5);
	glTranslatef(0.0f, 0.0f, diameterSphere / 5);
	glTranslatef(0.0f, 0.0f, sizeLeg);
	drawSphere(1.5 * diameterSphere, _esfera);
	glRotatef(LLegArt, 0.0f, -1.0f, 0.0f);

	//Coxa
	drawCylinder(1.5 * diameterCylinder, sizeLeg, _jean);
	glTranslatef(0.0f, 0.0f, sizeLeg);
	glTranslatef(0.0f, 0.0f, (diameterSphere / 5));
	drawSphere(1.5 * diameterSphere, _esfera);
	glRotatef(LkneeArt, 0.0f, 1.0f, 0.0f);
	drawCylinder(1.5 * diameterCylinder, sizeKnee, _jean);
	glTranslatef(0.0f, 0.0f, (sizeKnee + diameterSphere / 5));
	drawDisk(0, diameterBase, _disco);
	drawCylinder(diameterBase, heightBase, _disco);
	glTranslatef(0.0f, 0.0f, heightBase);
	drawDisk(0, diameterBase, _disco);

	glPopMatrix();
	glPushMatrix();

	//Perna direita
	glTranslatef(0.0f, centroY - 4.0f, centroZ);
	glTranslatef(0.0f, 0.0f, heightBase);
	glTranslatef(0.0f, 0.0f, sizeKnee + diameterSphere / 5);
	glTranslatef(0.0f, 0.0f, diameterSphere / 5);
	glTranslatef(0.0f, 0.0f, sizeLeg);
	drawSphere(1.5 * diameterSphere, _esfera);
	glRotatef(RLegArt, 0.0f, -1.0f, 0.0f);

	//Coxa
	drawCylinder(1.5 * diameterCylinder, sizeLeg, _jean);
	glTranslatef(0.0f, 0.0f, sizeLeg);
	glTranslatef(0.0f, 0.0f, (diameterSphere / 5));
	drawSphere(1.5 * diameterSphere, _esfera);
	glRotatef(RkneeArt, 0.0f, 1.0f, 0.0f);
	drawCylinder(1.5 * diameterCylinder, sizeKnee, _jean);
	glTranslatef(0.0f, 0.0f, (sizeKnee + diameterSphere / 5));
	drawDisk(0, diameterBase, _disco);
	drawCylinder(diameterBase, heightBase, _disco);
	glTranslatef(0.0f, 0.0f, heightBase);
	drawDisk(0, diameterBase, _disco);

	//Tronco
	glPopMatrix();
	glPushMatrix();		//Parte de baixo do tronco
	glTranslatef(0.0f, centroY, centroZ + sizeLeg + sizeKnee + heightBase + diameterSphere / 2);
	drawDisk(0, diameterTorso, _disco);
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	drawCylinder(diameterTorso, torsoHeight, _torso);
	glRotatef(-90, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.0f, 0.0f, torsoHeight);
	drawDisk(diameterCylinder, diameterTorso, _disco);

	//Pescoço e cabeça
	glPushMatrix();		//Base do pescoço
	drawCylinder(diameterKneck, heightKneck, _rosto);
	glTranslatef(0.0f, 0.0f, diameterKneck);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(headArt, 1.0f, 0.0f, 0.0f);
	glTranslatef(-diameterHead, 0.0f, 0.0f);
	glTranslatef(0.0, 0.0f, -heightHead / 2);
	drawDisk(0, diameterHead, _disco);
	drawCylinder(diameterHead, heightHead, _disco);
	glTranslatef(0.0f, 0.0f, heightHead);
	drawDisk(0, diameterHead, _rosto);

	//Olhos
	glPushMatrix();		//centro do rosto
	glTranslatef(-diameterHead / 2.2, diameterHead / 3, -diameterEye / 2);
	glRotatef(-86.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	drawSphere(diameterEye, _olho);
	glTranslatef(0.0f, 0.0f, diameterHead / 1.5);
	drawSphere(diameterEye, _olho);

	//Nariz
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -diameterHead / 3);
	drawCone(diameterNose, heightNose);

	//Boca
	glPopMatrix();
	glTranslatef(diameterHead / 2, diameterMouth / 1.5, 0.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glTranslatef(-diameterMouth / 1.5, 0.0f, -1.4 * diameterMouth);
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, heightMouth / 1.5);
	drawDisk(0, diameterMouth, _boca);
	glTranslatef(0.0f, 0.0f, -heightMouth);
	drawDisk(0, diameterMouth, _disco);
	glPopMatrix();
	glTranslatef(0.0f, 0.0f, -heightMouth / 3);
	glRotatef(40.0f, 0.0f, 0.0f, 1.0f);
	drawCylinder(diameterMouth, heightMouth, _boca);
	glPopMatrix();
	glPushMatrix();

	//Braço esquerdo
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(0.0f, diameterTorso, -torsoHeight / 3);
	drawSphere(2 * diameterSphere, _esfera);

	//move referencial do braço
	glRotatef(LangleArmZ, 1.0f, 0.0f, 0.0f);
	glRotatef(LangleArmY, 0.0f, 1.0f, 0.0f);

	//desenha braço
	drawCylinder(diameterCylinder, sizeArm, _braco);

	//move referencial do antebraço
	glTranslatef(0.0f, 0.0f, sizeArm + diameterSphere / 5);
	glRotatef(LangleForearm, 0.0f, 1.0f, 0.0f);

	//desenha antebraço
	drawSphere(diameterSphere, _esfera);
	glTranslatef(0.0f, 0.0f, diameterSphere / 5);
	drawCylinder(diameterCylinder, sizeForearm, _disco);

	//move para o referencial da garra
	glTranslatef(0.0f, 0.0f, sizeForearm + diameterSphere / 5);
	glRotatef(LangleClampZ, 0.0f, 0.0f, 1.0f);

	//desenha a esfera da garra
	drawSphere(diameterSphere, _esfera);
	glTranslatef(0.0f, 0.0f, diameterSphere / 2);
	glRotatef(LhandSpin, 0.0f, 0.0f, 1.0f);

	glPushMatrix();

	//desenha a parte de cima da garra
	glRotatef(LangleClampY + 60, 0.0f, 1.0f, 0.0f);

	drawCylinder(diameterCylinder / 3, sizeClampPart, _dedo);
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3, _esfera);

	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	glRotatef(-60, 0.0f, 1.0f, 0.0f);

	drawCylinder(diameterCylinder / 3, sizeClampPart, _dedo);
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3, _esfera);

	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	glRotatef(-60, 0.0f, 1.0f, 0.0f);
	drawConeD(diameterCylinder / 3, sizeClampPart);

	glPopMatrix();
	glPushMatrix();

	//desenha parte de baixo da garra
	glRotatef(-LangleClampY - 60, 0.0f, 1.0f, 0.0f);

	drawCylinder(diameterCylinder / 3, sizeClampPart, _dedo);
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3, _esfera);

	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	glRotatef(60, 0.0f, 1.0f, 0.0f);

	drawCylinder(diameterCylinder / 3, sizeClampPart, _dedo);
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3, _esfera);

	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	glRotatef(60, 0.0f, 1.0f, 0.0f);
	drawConeD(diameterCylinder / 3, sizeClampPart);

	glPopMatrix();

	//Braço direita
	glPopMatrix();
	//glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(0.0f, -diameterTorso, -torsoHeight / 3);
	drawSphere(2 * diameterSphere, _esfera);

	//move referencial do braço
	glRotatef(RangleArmZ, 1.0f, 0.0f, 0.0f);
	glRotatef(RangleArmY, 0.0f, 1.0f, 0.0f);

	//desenha braço
	drawCylinder(diameterCylinder, sizeArm, _braco);

	//move referencial do antebraço
	glTranslatef(0.0f, 0.0f, sizeArm + diameterSphere / 5);
	glRotatef(RangleForearm, 0.0f, 1.0f, 0.0f);

	//desenha antebraço
	drawSphere(diameterSphere, _esfera);
	glTranslatef(0.0f, 0.0f, diameterSphere / 5);
	drawCylinder(diameterCylinder, sizeForearm, _disco);

	//move para o referencial da garra
	glTranslatef(0.0f, 0.0f, sizeForearm + diameterSphere / 5);
	glRotatef(RangleClampZ, 0.0f, 0.0f, 1.0f);

	//desenha a esfera da garra
	drawSphere(diameterSphere, _esfera);
	glTranslatef(0.0f, 0.0f, diameterSphere / 2);
	glRotatef(RhandSpin, 0.0f, 0.0f, 1.0f);

	glPushMatrix();

	//desenha a parte de cima da garra
	glRotatef(RangleClampY + 60, 0.0f, 1.0f, 0.0f);

	drawCylinder(diameterCylinder / 3, sizeClampPart, _dedo);
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3, _esfera);

	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	glRotatef(-60, 0.0f, 1.0f, 0.0f);

	drawCylinder(diameterCylinder / 3, sizeClampPart, _dedo);
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3, _esfera);

	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	glRotatef(-60, 0.0f, 1.0f, 0.0f);
	drawConeD(diameterCylinder / 3, sizeClampPart);

	glPopMatrix();
	glPushMatrix();

	//desenha parte de baixo da garra
	glRotatef(-RangleClampY - 60, 0.0f, 1.0f, 0.0f);

	drawCylinder(diameterCylinder / 3, sizeClampPart, _dedo);
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3, _esfera);

	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	glRotatef(60, 0.0f, 1.0f, 0.0f);

	drawCylinder(diameterCylinder / 3, sizeClampPart, _dedo);
	glTranslatef(0.0f, 0.0f, sizeClampPart + diameterSphere / 15);
	drawSphere(diameterSphere / 3, _esfera);

	glTranslatef(0.0f, 0.0f, diameterSphere / 15);
	glRotatef(60, 0.0f, 1.0f, 0.0f);
	drawConeD(diameterCylinder / 3, sizeClampPart);

	glPopMatrix();
	glPopMatrix();
	glPopMatrix();

	glutSwapBuffers();
}



int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Robô");

	initLighting();
	initRendering();

	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(glutInput);
	glutReshapeFunc(handleResize);

	glutMainLoop();

	return 0;
}
