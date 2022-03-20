// Project.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
//#include "..\glut\include\GL\freeglut.h"
#include "..\freeglut\include\GL\freeglut.h"
#include <iostream>

float Angle = 0;

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1024, 1024, -768, 768, -1000.0f, 1000.0f);

	glRotatef(Angle, 1.0f, 1.0f, 1.0f);
	Angle += 0.01;

	glBegin(GL_TRIANGLES);
	glColor3f(0.1, 0.2, 0.3);
	glVertex3f(100.0f, 100.0f, 0);
	glVertex3f(200.0f, 100.0f, 0);
	glVertex3f(200.0f, 200.0f, 0);
	glEnd();

	glBegin(GL_LINES);
	//Оси
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1000.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1000.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1000.0f);
	//Буква X
	glVertex3f(1010.0f, 15.0f, 0.0f);
	glVertex3f(1030.0f, -15.0f, 0.0f);
	glVertex3f(1010.0f, -15.0f, 0.0f);
	glVertex3f(1030.0f, 15.0f, 0.0f);
	//Буква Y
	glVertex3f(0.0f, 1010.0f, 0.0f);
	glVertex3f(0.0f, 1030.0f, 0.0f);
	glVertex3f(0.0f, 1030.0f, 0.0f);
	glVertex3f(-10.0f, 1050.0f, 0.0f);
	glVertex3f(0.0f, 1030.0f, 0.0f);
	glVertex3f(10.0f, 1050.0f, 0.0f);
	//Буква Z
	glVertex3f(-15.0f, 0.0f, 1010.0f);
	glVertex3f(15.0f, 0.0f, 1010.0f);
	glVertex3f(-15.0f, 0.0f, 1050.0f);
	glVertex3f(15.0f, 0.0f, 1050.0f);
	glVertex3f(-15.0f, 0.0f, 1010.0f);
	glVertex3f(15.0f, 0.0f, 1050.0f);
	glEnd();

	glFlush();
}

int main(int argc, char** argv)
{
	HWND hConsole = GetConsoleWindow();
	ShowWindow(hConsole, SW_HIDE);//прячем оконо консоли

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1024, 1024);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Triangle");
	
	glutDisplayFunc(display);
	glutIdleFunc(display);

	glutMainLoop();
	return 0;
}