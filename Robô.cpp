#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include "RgbImage.h"

#define _CRT_SECURE_NO_WARNINGS
#define PI (float) 3.141592654

using namespace std;

const char* fAmarelo = "./amarelo.bmp";
const char* fMarrom = "./marrom.bmp";
const char* fMetal = "./metal.bmp";
const char* fOlho = "./olho.bmp";
const char* fVerde = "./verde.bmp";

GLuint _amarelo;
GLuint _marrom;
GLuint _metal;
GLuint _olho;
GLuint _verde;

GLUquadric* quadSphere;
GLUquadric* quadCylinder;

bool textureOn = true;

float centroZ = 0;
float centroY = 0;
float diameterCylinder = 0.3;
float sphereDiameter = 0.4;
float armSize = 4.5;
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

typedef struct Perna {
	float LegArt = 180.0f;
	float KneeArt = 0.0f;
	float DistZ = 4.0f;
};

Perna pernaEsquerda, pernaDireita;

typedef struct Braco {
	float AngleArmY = 0.0f;
	float AngleArmZ;
	float AngleClampY = 0.0f;
	float AngleClampZ = 0.0f;
	float AngleForearm = 0.0;
	float HandSpin = 90.0f;
	float DiameterTorso;
};

Braco bracoEsquerdo, bracoDireito;

const GLfloat lightposition[4] = { 0.0f, 30.0f, 0.0f, 0.0f };
const GLfloat luzAmbiente[4] = { 0.3, 0.3, 0.3, 0.0 };

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
	_amarelo = loadTexture(fAmarelo);
	_marrom = loadTexture(fMarrom);
	_metal = loadTexture(fMetal);
	_olho = loadTexture(fOlho);
	_verde = loadTexture(fVerde);
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
		if (bracoEsquerdo.AngleArmZ <= -90 + 45)
			bracoEsquerdo.AngleArmZ += 3;
		glutPostRedisplay();
		break;
	case '2': //Decrease arm angle
		if (bracoEsquerdo.AngleArmZ >= -180 + 45)
			bracoEsquerdo.AngleArmZ -= 3;
		glutPostRedisplay();
		break;
	case '3': //Decrease arm angle
		if (bracoEsquerdo.AngleArmY <= 90 - 15) {
			bracoEsquerdo.AngleArmY += 3;
		}
		glutPostRedisplay();
		break;
	case '4': //Decrease arm angle
		if (bracoEsquerdo.AngleArmY >= 0) {
			bracoEsquerdo.AngleArmY -= 3;
		}
		glutPostRedisplay();
		break;
	case '5': //Increase forearm angle
		if (bracoEsquerdo.AngleForearm < 90) bracoEsquerdo.AngleForearm += 3;
		glutPostRedisplay();
		break;
	case '6': //Decrease forearm angle
		if (bracoEsquerdo.AngleForearm > 0) bracoEsquerdo.AngleForearm -= 3;
		glutPostRedisplay();
		break;
	case '7': //Decrease forearm angle
		if (bracoEsquerdo.HandSpin > -180) bracoEsquerdo.HandSpin -= 3;
		glutPostRedisplay();
		break;
	case '8': //Decrease forearm angle
		if (bracoEsquerdo.HandSpin < 180) bracoEsquerdo.HandSpin += 3;
		glutPostRedisplay();
		break;
	case '9': //Increase clamp angle y axis
		if (bracoEsquerdo.AngleClampY < 60) bracoEsquerdo.AngleClampY += 3;
		glutPostRedisplay();
		break;
	case '0': //Decrease clamp angle y axis
		if (bracoEsquerdo.AngleClampY > 0) bracoEsquerdo.AngleClampY -= 3;
		glutPostRedisplay();
		break;
	case 92: //Increase arm angle
		if (bracoDireito.AngleArmZ >= 90 - 45)
			bracoDireito.AngleArmZ -= 3;
		glutPostRedisplay();
		break;
	case 'z': //Decrease arm angle
		if (bracoDireito.AngleArmZ <= 90 + 45)
			bracoDireito.AngleArmZ += 3;
		glutPostRedisplay();
		break;
	case 'x': //Decrease arm angle
		if (bracoDireito.AngleArmY <= 90 - 15) {
			bracoDireito.AngleArmY += 3;
		}
		glutPostRedisplay();
		break;
	case 'c': //Decrease arm angle
		if (bracoDireito.AngleArmY >= 0) {
			bracoDireito.AngleArmY -= 3;
		}
		glutPostRedisplay();
		break;
	case 'v': //Increase forearm angle
		if (bracoDireito.AngleForearm < 90) bracoDireito.AngleForearm += 3;
		glutPostRedisplay();
		break;
	case 'b': //Decrease forearm angle
		if (bracoDireito.AngleForearm > 0) bracoDireito.AngleForearm -= 3;
		glutPostRedisplay();
		break;
	case 'n': //Decrease forearm angle
		if (bracoDireito.HandSpin > -180) bracoDireito.HandSpin -= 3;
		glutPostRedisplay();
		break;
	case 'm': //Decrease forearm angle
		if (bracoDireito.HandSpin < 180) bracoDireito.HandSpin += 3;
		glutPostRedisplay();
		break;
	case ',': //Increase clamp angle y axis
		if (bracoDireito.AngleClampY < 60) bracoDireito.AngleClampY += 3;
		glutPostRedisplay();
		break;
	case '.': //Decrease clamp angle y axis
		if (bracoDireito.AngleClampY > 0) bracoDireito.AngleClampY -= 3;
		glutPostRedisplay();
		break;
	case 'y': //Increase Leg angle 
		if (pernaEsquerda.LegArt < 180 + 75) pernaEsquerda.LegArt += 3;
		glutPostRedisplay();
		break;
	case 'u': //Decrease Leg angle
		if (pernaEsquerda.LegArt > 180) pernaEsquerda.LegArt -= 3;
		glutPostRedisplay();
		break;
	case 'i': //Increase knee angle
		if (pernaEsquerda.KneeArt < 90) pernaEsquerda.KneeArt += 3;
		glutPostRedisplay();
		break;
	case 'o': //Decrease knee angle
		if (pernaEsquerda.KneeArt > 0) pernaEsquerda.KneeArt -= 3;
		glutPostRedisplay();
		break;
	case 'h': //Increase Leg angle 
		if (pernaDireita.LegArt < 180 + 75) pernaDireita.LegArt += 3;
		glutPostRedisplay();
		break;
	case 'j': //Decrease Leg angle
		if (pernaDireita.LegArt > 180) pernaDireita.LegArt -= 3;
		glutPostRedisplay();
		break;
	case 'k': //Increase knee angle
		if (pernaDireita.KneeArt < 90) pernaDireita.KneeArt += 3;
		glutPostRedisplay();
		break;
	case 'l': //Decrease knee angle
		if (pernaDireita.KneeArt > 0) pernaDireita.KneeArt -= 3;
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

void drawCone(float diameter, float lenght, GLuint texture = INFINITE) {
	if (textureOn) {
		if (texture != INFINITE)
		{
			glBindTexture(GL_TEXTURE_2D, texture);
		}
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else {
		gluQuadricTexture(quadCylinder, 0);
	}

	gluCylinder(quadCylinder, diameter, 0, lenght, 40.0, lenght * 30.0);
}

void drawDisk(float diameterInner, float diameterOuter, GLuint texture) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else {
		gluQuadricTexture(quadCylinder, 0);
	}

	gluDisk(quadCylinder, diameterInner, diameterOuter, 40.0, 30.0);
}

void drawSphere(float diameter, GLuint texture) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadSphere, 1);
	}
	else {
		gluQuadricTexture(quadSphere, 0);
	}

	gluSphere(quadSphere, diameter, 40.0, 40.0);
}

void drawPerna(Perna perna) {
	glTranslatef(0.0f, centroY + perna.DistZ, centroZ + heightBase + sizeKnee + (sphereDiameter / 2.5f) + sizeLeg);
	drawSphere(1.5f * sphereDiameter, _amarelo);

	glRotatef(perna.LegArt, 0.0f, -1.0f, 0.0f);
	drawCylinder(1.5f * diameterCylinder, sizeLeg, _marrom);

	glTranslatef(0.0f, 0.0f, sizeLeg);
	glTranslatef(0.0f, 0.0f, (sphereDiameter / 5));
	drawSphere(1.5f * sphereDiameter, _amarelo);

	glRotatef(perna.KneeArt, 0.0f, 1.0f, 0.0f);
	drawCylinder(1.5f * diameterCylinder, sizeKnee, _marrom);

	glTranslatef(0.0f, 0.0f, (sizeKnee + sphereDiameter / 5));
	drawDisk(0, diameterBase, _metal);
	drawCylinder(diameterBase, heightBase, _metal);
	glTranslatef(0.0f, 0.0f, heightBase);
	drawDisk(0, diameterBase, _metal);

	glPopMatrix();
	glPushMatrix();
}

void drawBraco(Braco braco) {
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(0.0f, braco.DiameterTorso, -torsoHeight / 3);
	drawSphere(2 * sphereDiameter, _amarelo);

	glRotatef(braco.AngleArmZ, 1.0f, 0.0f, 0.0f);
	glRotatef(braco.AngleArmY, 0.0f, 1.0f, 0.0f);

	drawCylinder(diameterCylinder, armSize, _verde);

	glTranslatef(0.0f, 0.0f, armSize + sphereDiameter / 5);
	glRotatef(braco.AngleForearm, 0.0f, 1.0f, 0.0f);

	drawSphere(sphereDiameter, _amarelo);
	glTranslatef(0.0f, 0.0f, sphereDiameter / 5);
	drawCylinder(diameterCylinder, sizeForearm, _metal);

	glTranslatef(0.0f, 0.0f, sizeForearm + sphereDiameter / 5);
	glRotatef(braco.AngleClampZ, 0.0f, 0.0f, 1.0f);

	drawSphere(sphereDiameter, _amarelo);
	glTranslatef(0.0f, 0.0f, sphereDiameter / 2);
	glRotatef(braco.HandSpin, 0.0f, 0.0f, 1.0f);

	glPushMatrix();

	glRotatef(braco.AngleClampY + 60, 0.0f, 1.0f, 0.0f);

	drawCylinder(diameterCylinder / 3, sizeClampPart, _amarelo);
	glTranslatef(0.0f, 0.0f, sizeClampPart + sphereDiameter / 15);
	drawSphere(sphereDiameter / 3, _amarelo);

	glTranslatef(0.0f, 0.0f, sphereDiameter / 15);
	glRotatef(-60, 0.0f, 1.0f, 0.0f);

	drawCylinder(diameterCylinder / 3, sizeClampPart, _amarelo);
	glTranslatef(0.0f, 0.0f, sizeClampPart + sphereDiameter / 15);
	drawSphere(sphereDiameter / 3, _amarelo);

	glTranslatef(0.0f, 0.0f, sphereDiameter / 15);
	glRotatef(-60, 0.0f, 1.0f, 0.0f);
	drawCone(diameterCylinder / 3, sizeClampPart, _amarelo);

	glPopMatrix();
	glPushMatrix();

	glRotatef(-braco.AngleClampY - 60, 0.0f, 1.0f, 0.0f);

	drawCylinder(diameterCylinder / 3, sizeClampPart, _amarelo);
	glTranslatef(0.0f, 0.0f, sizeClampPart + sphereDiameter / 15);
	drawSphere(sphereDiameter / 3, _amarelo);

	glTranslatef(0.0f, 0.0f, sphereDiameter / 15);
	glRotatef(60, 0.0f, 1.0f, 0.0f);

	drawCylinder(diameterCylinder / 3, sizeClampPart, _amarelo);
	glTranslatef(0.0f, 0.0f, sizeClampPart + sphereDiameter / 15);
	drawSphere(sphereDiameter / 3, _amarelo);

	glTranslatef(0.0f, 0.0f, sphereDiameter / 15);
	glRotatef(60, 0.0f, 1.0f, 0.0f);
	drawCone(diameterCylinder / 3, sizeClampPart, _amarelo);

	glPopMatrix();
	glPopMatrix();
}

void drawScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(zoom, 1, 20, 200);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	eyeX = eyeDistance * cosf(viewAngleZ * PI / 180.0f) * cosf(viewAngleX * PI / 180.0f);
	eyeY = eyeDistance * cosf(viewAngleZ * PI / 180.0f) * sinf(viewAngleX * PI / 180.0f);
	eyeZ = eyeDistance * sinf(viewAngleZ * PI / 180.0f);

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

	drawPerna(pernaEsquerda);
	drawPerna(pernaDireita);

	//Tronco
	glTranslatef(0.0f, centroY, centroZ + sizeLeg + sizeKnee + heightBase + sphereDiameter / 2);
	drawDisk(0, diameterTorso, _metal);
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	drawCylinder(diameterTorso, torsoHeight, _verde);
	glRotatef(-90, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.0f, 0.0f, torsoHeight);
	drawDisk(diameterCylinder, diameterTorso, _metal);

	glPushMatrix();

	drawSphere(diameterTorso, _metal);

	//Olhos
	glPushMatrix();
	glTranslatef(-diameterTorso / 2.2, diameterTorso / 3, -diameterEye / 2);
	glRotatef(-86.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	drawSphere(diameterEye, _olho);
	glTranslatef(0.0f, 0.0f, diameterTorso / 1.5);
	drawSphere(diameterEye, _olho);

	////Nariz
	//glPopMatrix();
	//glPushMatrix();
	//glTranslatef(0.0f, 0.0f, -diameterHead / 3);
	//drawCone(diameterNose, heightNose);

	////Boca
	//glPopMatrix();
	//glTranslatef(diameterHead / 2, diameterMouth / 1.5, 0.0f);
	//glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	//glTranslatef(-diameterMouth / 1.5, 0.0f, -1.4 * diameterMouth);
	//glPushMatrix();
	//glTranslatef(0.0f, 0.0f, heightMouth / 1.5);
	//drawDisk(0, diameterMouth, _boca);
	//glTranslatef(0.0f, 0.0f, -heightMouth);
	//drawDisk(0, diameterMouth, _metal);
	//glPopMatrix();
	//glTranslatef(0.0f, 0.0f, -heightMouth / 3);
	//glRotatef(40.0f, 0.0f, 0.0f, 1.0f);
	//drawCylinder(diameterMouth, heightMouth, _boca);
	glPopMatrix();
	glPushMatrix();

	drawBraco(bracoEsquerdo);
	drawBraco(bracoDireito);

	glPopMatrix();

	glutSwapBuffers();
}

int main(int argc, char** argv) {
	pernaEsquerda.DistZ = 4.0f;
	pernaDireita.DistZ = -4.0f;

	bracoEsquerdo.AngleArmZ = -90.0f;
	bracoEsquerdo.DiameterTorso = 5.0f;

	bracoDireito.AngleArmZ = 90.0f;
	bracoDireito.DiameterTorso = -5.0f;

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
