#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include "RgbImage.h"

#define _CRT_SECURE_NO_WARNINGS
#define PI (float) 3.141592654

using namespace std;

GLuint _amarelo;
GLuint _marrom;
GLuint _metal;
GLuint _olho;
GLuint _verde;

GLUquadric* quadSphere;
GLUquadric* quadCylinder;

bool texturaLigado = true;

float centroZ = 0;
float centroY = 0;

float diametroCilindro = 0.3f;
float diametroEsfera = 0.4f;
float diametroTorso = 5;
float diametroOlho = 0.5;

float tamanhoBraco = 4.5;
float tamanhoJoelho = 4.5;
float tamanhoAntebraco = 4.0;
float tamanhoPerna = 5.0;
float tamanhoParteDaGarra = 1.0;

float alturaBase = 0.5;
float alturaTorso = 12;

float rotacaoCabeca = 0.0f;

float distanciaOlho = 100.0;
float olhoX;
float olhoY;
float olhoZ;
float anguloVisaoX = 0.0;
float anguloVisaoZ = 15.0;
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
	float AnguloBracoY = 0.0f;
	float AnguloBracoZ;
	float AnguloGarraY = 0.0f;
	float AnguloGarraZ = 0.0f;
	float AnguloAntebraco = 0.0;
	float GiroDaMao = 90.0f;
	float DiameterTorso;
};
Braco bracoEsquerdo, bracoDireito;

GLfloat posicaoLuz[4] = { 0.0f, 30.0f, 0.0f, 0.0f };
GLfloat luzAmbiente[4] = { 0.3f, 0.3f, 0.3f, 0.0f };

void initLighting(void)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glShadeModel(GL_SMOOTH);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

	glEnable(GL_COLOR_MATERIAL);
}

GLuint carregaTextura(const char* nomeArquivo) {
	GLuint textura;

	RgbImage image(nomeArquivo);

	glGenTextures(1, &textura);
	glBindTexture(GL_TEXTURE_2D, textura);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image.GetNumCols(), image.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE, image.ImageData());

	return textura;
}

void initRendering(void) {
	quadSphere = gluNewQuadric();
	quadCylinder = gluNewQuadric();

	_amarelo = carregaTextura("./amarelo.bmp");
	_marrom = carregaTextura("./marrom.bmp");
	_metal = carregaTextura("./metal.bmp");
	_olho = carregaTextura("./olho.bmp");
	_verde = carregaTextura("./verde.bmp");
}

void handleKeypress(unsigned char key, int, int) {
	switch (key) {
	case 27: //Escape key
		exit(0);
	case 'w': // Aumenta angulo no eixo z
		if (anguloVisaoZ < 180) anguloVisaoZ += 3;
		break;
	case 's': // Diminui angulo no eixo z
		if (anguloVisaoZ > 0) anguloVisaoZ -= 3;
		break;
	case 'a': // Aumenta angulo no eixo x
		if (anguloVisaoX < 180) anguloVisaoX += 3;
		break;
	case 'd': // Diminui angulo no eixo x
		if (anguloVisaoX > -180) anguloVisaoX -= 3;
		break;
	case 9: // Liga/Desliga textura
		texturaLigado = !texturaLigado;
		break;
	case '1': //Increase arm angle
		if (bracoEsquerdo.AnguloBracoZ <= -90 + 45) bracoEsquerdo.AnguloBracoZ += 3;
		break;
	case '2': //Decrease arm angle
		if (bracoEsquerdo.AnguloBracoZ >= -180 + 45) bracoEsquerdo.AnguloBracoZ -= 3;
		break;
	case '3': //Decrease arm angle
		if (bracoEsquerdo.AnguloBracoY <= 90 - 15) bracoEsquerdo.AnguloBracoY += 3;
		break;
	case '4': //Decrease arm angle
		if (bracoEsquerdo.AnguloBracoY >= 0) bracoEsquerdo.AnguloBracoY -= 3;
		break;
	case '5': //Increase forearm angle
		if (bracoEsquerdo.AnguloAntebraco < 90) bracoEsquerdo.AnguloAntebraco += 3;
		break;
	case '6': //Decrease forearm angle
		if (bracoEsquerdo.AnguloAntebraco > 0) bracoEsquerdo.AnguloAntebraco -= 3;
		break;
	case '7': //Decrease forearm angle
		if (bracoEsquerdo.GiroDaMao > -180) bracoEsquerdo.GiroDaMao -= 3;
		break;
	case '8': //Decrease forearm angle
		if (bracoEsquerdo.GiroDaMao < 180) bracoEsquerdo.GiroDaMao += 3;
		break;
	case '9': //Increase clamp angle y axis
		if (bracoEsquerdo.AnguloGarraY < 60) bracoEsquerdo.AnguloGarraY += 3;
		break;
	case '0': //Decrease clamp angle y axis
		if (bracoEsquerdo.AnguloGarraY > 0) bracoEsquerdo.AnguloGarraY -= 3;
		break;
	case 92: //Increase arm angle
		if (bracoDireito.AnguloBracoZ >= 90 - 45) bracoDireito.AnguloBracoZ -= 3;
		break;
	case 'z': //Decrease arm angle
		if (bracoDireito.AnguloBracoZ <= 90 + 45) bracoDireito.AnguloBracoZ += 3;
		break;
	case 'x': //Decrease arm angle
		if (bracoDireito.AnguloBracoY <= 90 - 15) bracoDireito.AnguloBracoY += 3;
		break;
	case 'c': //Decrease arm angle
		if (bracoDireito.AnguloBracoY >= 0) bracoDireito.AnguloBracoY -= 3;
		break;
	case 'v': //Increase forearm angle
		if (bracoDireito.AnguloAntebraco < 90) bracoDireito.AnguloAntebraco += 3;
		break;
	case 'b': //Decrease forearm angle
		if (bracoDireito.AnguloAntebraco > 0) bracoDireito.AnguloAntebraco -= 3;
		break;
	case 'n': //Decrease forearm angle
		if (bracoDireito.GiroDaMao > -180) bracoDireito.GiroDaMao -= 3;
		break;
	case 'm': //Decrease forearm angle
		if (bracoDireito.GiroDaMao < 180) bracoDireito.GiroDaMao += 3;
		break;
	case ',': //Increase clamp angle y axis
		if (bracoDireito.AnguloGarraY < 60) bracoDireito.AnguloGarraY += 3;
		break;
	case '.': //Decrease clamp angle y axis
		if (bracoDireito.AnguloGarraY > 0) bracoDireito.AnguloGarraY -= 3;
		break;
	case 'y': //Increase Leg angle 
		if (pernaEsquerda.LegArt < 180 + 75) pernaEsquerda.LegArt += 3;
		break;
	case 'u': //Decrease Leg angle
		if (pernaEsquerda.LegArt > 180) pernaEsquerda.LegArt -= 3;
		break;
	case 'i': //Increase knee angle
		if (pernaEsquerda.KneeArt < 90) pernaEsquerda.KneeArt += 3;
		break;
	case 'o': //Decrease knee angle
		if (pernaEsquerda.KneeArt > 0) pernaEsquerda.KneeArt -= 3;
		break;
	case 'h': //Increase Leg angle 
		if (pernaDireita.LegArt < 180 + 75) pernaDireita.LegArt += 3;
		break;
	case 'j': //Decrease Leg angle
		if (pernaDireita.LegArt > 180) pernaDireita.LegArt -= 3;
		break;
	case 'k': //Increase knee angle
		if (pernaDireita.KneeArt < 90) pernaDireita.KneeArt += 3;
		break;
	case 'l': //Decrease knee angle
		if (pernaDireita.KneeArt > 0) pernaDireita.KneeArt -= 3;
		break;
	case 'q': //Increase head angle
		if (rotacaoCabeca < 90) rotacaoCabeca += 3;
		break;
	case 'e': //Decrease head angle
		if (rotacaoCabeca > -90) rotacaoCabeca -= 3;
		break;
	case '=': //
		if (zoom > 4) zoom -= 2;
		break;
	case '-': //
		if (zoom < 60) zoom += 2;
		break;
	default:
		printf("\n%s\n", key);
	}

	glutPostRedisplay();
}

void glutInput(int key, int, int) {
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
	if (texturaLigado) {
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else {
		gluQuadricTexture(quadCylinder, 0);
	}

	gluCylinder(quadCylinder, diameter, diameter, lenght, 10.0f, 10.0f);
}

void drawCone(float diameter, float lenght, GLuint texture = INFINITE) {
	if (texturaLigado) {
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

	gluCylinder(quadCylinder, diameter, 0, lenght, 10.0f, 10.0f);
}

void drawDisk(float diameterInner, float diameterOuter, GLuint texture) {
	if (texturaLigado) {
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else {
		gluQuadricTexture(quadCylinder, 0);
	}

	gluDisk(quadCylinder, diameterInner, diameterOuter, 10.0f, 10.0f);
}

void drawSphere(float diameter, GLuint texture) {
	if (texturaLigado) {
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadSphere, 1);
	}
	else {
		gluQuadricTexture(quadSphere, 0);
	}

	gluSphere(quadSphere, diameter, 10.0f, 10.0f);
}

void drawPerna(Perna perna) {
	glTranslatef(0.0f, centroY + perna.DistZ, centroZ + alturaBase + tamanhoJoelho + (diametroEsfera / 2.5f) + tamanhoPerna);
	drawSphere(1.5f * diametroEsfera, _amarelo);

	glRotatef(perna.LegArt, 0.0f, -1.0f, 0.0f);
	drawCylinder(1.5f * diametroCilindro, tamanhoPerna, _marrom);

	glTranslatef(0.0f, 0.0f, tamanhoPerna);
	glTranslatef(0.0f, 0.0f, (diametroEsfera / 5));
	drawSphere(1.5f * diametroEsfera, _amarelo);

	glRotatef(perna.KneeArt, 0.0f, 1.0f, 0.0f);
	drawCylinder(1.5f * diametroCilindro, tamanhoJoelho, _marrom);

	glTranslatef(0.0f, 0.0f, tamanhoJoelho + diametroEsfera);
	drawSphere(2, _metal);

	glPopMatrix();
	glPushMatrix();
}

void drawBraco(Braco braco) {
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(0.0f, braco.DiameterTorso, -alturaTorso / 3);
	drawSphere(2 * diametroEsfera, _amarelo);

	glRotatef(braco.AnguloBracoZ, 1.0f, 0.0f, 0.0f);
	glRotatef(braco.AnguloBracoY, 0.0f, 1.0f, 0.0f);

	drawCylinder(diametroCilindro, tamanhoBraco, _verde);

	glTranslatef(0.0f, 0.0f, tamanhoBraco + diametroEsfera / 5);
	glRotatef(braco.AnguloAntebraco, 0.0f, 1.0f, 0.0f);

	drawSphere(diametroEsfera, _amarelo);
	glTranslatef(0.0f, 0.0f, diametroEsfera / 5);
	drawCylinder(diametroCilindro, tamanhoAntebraco, _metal);

	glTranslatef(0.0f, 0.0f, tamanhoAntebraco + diametroEsfera / 5);
	glRotatef(braco.AnguloGarraZ, 0.0f, 0.0f, 1.0f);

	drawSphere(diametroEsfera, _amarelo);
	glTranslatef(0.0f, 0.0f, diametroEsfera / 2);
	glRotatef(braco.GiroDaMao, 0.0f, 0.0f, 1.0f);

	glPushMatrix();

	glRotatef(braco.AnguloGarraY + 60, 0.0f, 1.0f, 0.0f);

	drawCylinder(diametroCilindro / 3, tamanhoParteDaGarra, _amarelo);
	glTranslatef(0.0f, 0.0f, tamanhoParteDaGarra + diametroEsfera / 15);
	drawSphere(diametroEsfera / 3, _amarelo);

	glTranslatef(0.0f, 0.0f, diametroEsfera / 15);
	glRotatef(-60, 0.0f, 1.0f, 0.0f);

	drawCylinder(diametroCilindro / 3, tamanhoParteDaGarra, _amarelo);
	glTranslatef(0.0f, 0.0f, tamanhoParteDaGarra + diametroEsfera / 15);
	drawSphere(diametroEsfera / 3, _amarelo);

	glTranslatef(0.0f, 0.0f, diametroEsfera / 15);
	glRotatef(-60, 0.0f, 1.0f, 0.0f);
	drawCone(diametroCilindro / 3, tamanhoParteDaGarra, _amarelo);

	glPopMatrix();
	glPushMatrix();

	glRotatef(-braco.AnguloGarraY - 60, 0.0f, 1.0f, 0.0f);

	drawCylinder(diametroCilindro / 3, tamanhoParteDaGarra, _amarelo);
	glTranslatef(0.0f, 0.0f, tamanhoParteDaGarra + diametroEsfera / 15);
	drawSphere(diametroEsfera / 3, _amarelo);

	glTranslatef(0.0f, 0.0f, diametroEsfera / 15);
	glRotatef(60, 0.0f, 1.0f, 0.0f);

	drawCylinder(diametroCilindro / 3, tamanhoParteDaGarra, _amarelo);
	glTranslatef(0.0f, 0.0f, tamanhoParteDaGarra + diametroEsfera / 15);
	drawSphere(diametroEsfera / 3, _amarelo);

	glTranslatef(0.0f, 0.0f, diametroEsfera / 15);
	glRotatef(60, 0.0f, 1.0f, 0.0f);
	drawCone(diametroCilindro / 3, tamanhoParteDaGarra, _amarelo);

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

	olhoX = distanciaOlho * cosf(anguloVisaoZ * PI / 180.0f) * cosf(anguloVisaoX * PI / 180.0f);
	olhoY = distanciaOlho * cosf(anguloVisaoZ * PI / 180.0f) * sinf(anguloVisaoX * PI / 180.0f);
	olhoZ = distanciaOlho * sinf(anguloVisaoZ * PI / 180.0f);

	if (anguloVisaoZ < 90) {
		gluLookAt(olhoX, olhoY, olhoZ, 0.0, 0.0, 0.0, 0.0, 0.0, 1);
	}
	else {
		gluLookAt(olhoX, olhoY, olhoZ, 0.0, 0.0, 0.0, 0.0, 0.0, -1);
	}

	glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

	// drawing color
	glColor3f(1.0f, 1.0f, 1.0f);

	// Origem
	glPushMatrix();

	drawPerna(pernaEsquerda);
	drawPerna(pernaDireita);

	//Tronco
	glTranslatef(0.0f, centroY, centroZ + tamanhoPerna + tamanhoJoelho + alturaBase + diametroEsfera / 2);
	drawDisk(0, diametroTorso, _metal);
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	drawCylinder(diametroTorso, alturaTorso, _verde);
	glRotatef(-90, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.0f, 0.0f, alturaTorso);
	drawDisk(diametroCilindro, diametroTorso, _metal);

	glPushMatrix();

	drawSphere(diametroTorso, _metal);

	//Olhos
	glPushMatrix();
	glTranslatef(-diametroTorso / 2.2, diametroTorso / 3, -diametroOlho / 2);
	glRotatef(-86.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	drawSphere(diametroOlho, _olho);
	glTranslatef(0.0f, 0.0f, diametroTorso / 1.5);
	drawSphere(diametroOlho, _olho);

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

	bracoEsquerdo.AnguloBracoZ = -90.0f;
	bracoEsquerdo.DiameterTorso = 5.0f;

	bracoDireito.AnguloBracoZ = 90.0f;
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
