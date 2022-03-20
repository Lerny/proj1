// Project.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
//#include "..\glut\include\GL\freeglut.h"
#include "..\freeglut\include\GL\freeglut.h"
#include <iostream>

float Angle = 0;
bool pickpoly = false;
void display();
int xStart = 0;
int yStart = 0;

float pansqx = .0;
float pansqy = .0;

int  win_width, win_height;

int RetrieveObjectID(int x, int y)
{
	//ofstream out("pick.txt", std::ios::app);
	//char buf[200];
	//std::string intstr;
	//out << " ---------- START ---------- " << std::endl;

	int objectsFound = 0; // Общее количество кликнутых обьектов
	int viewportCoords[4] = { 0 }; // Массив для хранения экранных координат

	// Переменная для хранения ID обьектов, на которые мы кликнули.
	// Мы делаем массив в 32 элемента, т.к. OpenGL также сохраняет другую
	// информацию, которая нам сейчас не нужна. Для каждого обьекта нужно
	// 4 слота.
	unsigned int selectBuffer[32] = { 0 };
	// glSelectBuffer регистрирует массив как буфер выбора обьектов. Первый параметр - размер
	// массива. Второй - сам массив для хранения информации.
	glSelectBuffer(32, selectBuffer); // Регистрируем буфер для хранения выбранных обьектов
	// Эта функция возвращает информацию о многих вещах в OpenGL. Мы передаём GL_VIEWPOR,
	// чтобы получить координаты экрана. Функция сохранит их в переданном вторым параметром массиве
	// в виде top,left,bottom,right.
	glGetIntegerv(GL_VIEWPORT, viewportCoords); // Получаем текущие координаты экрана
	// Теперь выходим из матрицы GL_MODELVIEW и переходим в матрицу GL_PROJECTION.
	// Это даёт возможность использовать X и Y координаты вместо 3D.


	//intstr = gcvt(x, 20, buf);
	//out << intstr << std::endl;
	//intstr = gcvt(y, 20, buf);
	//out << intstr << std::endl;

	//out << " ---------- END ---------- " << std::endl;
	//out.close();
	glMatrixMode(GL_PROJECTION); // Переходим в матрицу проекции
	glPushMatrix(); // Переходим в новые экранные координаты
	// Эта функция делает так, что фреймбуфер не изменяется при рендере в него, вместо этого
	// происходит запись имён (ID) примитивов, которые были бы отрисованы при режиме
	// GL_RENDER. Информация помещается в selectBuffer.
	glRenderMode(GL_SELECT); // Позволяет рендерить обьекты без изменения фреймбуфера
	glLoadIdentity(); // Сбросим матрицу проекции
	// gluPickMatrix позволяет создавать матрицу проекции около нашего курсора. Проще говоря,
	// рендерится только область, которую мы укажем (вокруг курсора). Если обьект рендерится
	// в этой области, его ID сохраняется (Вот он, смысл всей функции).
	// Первые 2 параметра - X и Y координаты начала, следующие 2 - ширина и высота области
	// отрисовки. Последний параметр - экранные координаты. Заметьте, мы вычитаем 'y' из
	// НИЖНЕЙ экранной координаты. Мы сделали это, чтобы перевернуть Y координаты.
	// В 3д-пространстве нулевые y-координаты начинаются внизу, а в экранных координатах
	// 0 по y находится вверху. Также передаём регион 2 на 2 пиксела для поиска в нём обьекта.
	// Это может быть изменено как вам удобнее.
	gluPickMatrix(x, viewportCoords[3] - y, 2, 2, viewportCoords);
	// Далее просто вызываем нашу нормальную функцию gluPerspective, точно так же, как
	// делали при инициализации.
	glOrtho(-1024, 1024, -768, 768, -1000.0f, 1000.0f);
	//gluPerspective(0.0f,(GLfloat)(OGL_W_W)/(GLfloat)(OGL_W_H),0.1f,150.0f);

	glMatrixMode(GL_MODELVIEW); // Возвращаемся в матрицу GL_MODELVIEW
	display(); // Теперь рендерим выбранную зону для выбора обьекта
	// Если мы вернёмся в нормальный режим рендеринга из режима выбора, glRenderMode
	// возвратит число обьектов, найденных в указанном регионе (в gluPickMatrix()).
	objectsFound = glRenderMode(GL_RENDER); // Вернемся в режим отрисовки и получим число обьектов
	glMatrixMode(GL_PROJECTION); // Вернемся в привычную матрицу проекции
	glPopMatrix(); // Выходим из матрицы
	glMatrixMode(GL_MODELVIEW); // Вернемся в матрицу GL_MODELVIEW
	// УФФ! Это было немного сложно. Теперь нам нужно выяснить ID выбранных обьектов.
	// Если они есть - objectsFound должно быть как минимум 1.
	if (objectsFound > 0)
	{
		// Если мы нашли более 1 обьекта, нужно проверить значения глубины всех
		// выбранных обьектов. Обьект с МЕНЬШИМ значением глубины - ближайший
		// к нам обьект, значит и щелкнули мы на него. В зависимости от того, что
		// мы программируем, нам могут понадобится и ВСЕ выбранные обьекты (если
		// некоторые были за ближайшим), но в этом уроке мы позаботимся только о
		// переднем обьекте. Итак, как нам получить значение глубины? Оно сохранено
		// в буфере выбора (selectionBuffer). Для каждого обьекта в нем 4 значения.
		// Первое - "число имен в массиве имен на момент события, далее минимум и
		// максимум значений глубины для всех вершин, которые были выбраны при прошлом
		// событии, далее по содержимое массива имен, нижнее имя - первое;
		// ("the number of names in the name stack at the time of the event, followed
		// by the minimum and maximum depth values of all vertices that hit since the
		// previous event, then followed by the name stack contents, bottom name first.") - MSDN.
		// Единстве, что нам нужно - минимальное значение глубины (второе значение) и
		// ID обьекта, переданного в glLoadName() (четвертое значение).
		// Итак, [0-3] - данные первого обьекта, [4-7] - второго, и т.д...
		// Будте осторожны, так как если вы отображаете на экране 2Д текст, он будет
		// всегда находится как ближайший обьект. Так что убедитесь, что отключили вывод
		// текста при рендеринге в режиме GL_SELECT. Я для этого использую флаг, передаваемый
		// в RenderScene(). Итак, получим обьект с минимальной глубиной!
		// При старте установим ближайшую глубину как глубину первого обьекта.
		// 1 - это минимальное Z-значение первого обьекта.
		unsigned int lowestDepth = selectBuffer[1];
		// Установим выбранный обьект как первый при старте.
		// 3 - ID первого обьекта, переданный в glLoadName().
		int selectedObject = selectBuffer[3];
		// Проходим через все найденные обьекты, начиная со второго (значения первого
		// мы присвоили изначально).
		for (int i = 1; i < objectsFound; i++)
		{
			// Проверяем, не ниже ли значение глубины текущего обьекта, чем предидущего.
			// Заметьте, мы умножаем i на 4 (4 значения на каждый обьект) и прибавляем 1 для глубины.
			if (selectBuffer[(i * 4) + 1] < lowestDepth)
			{
				// Установим новое низшее значение
				lowestDepth = selectBuffer[(i * 4) + 1];
				// Установим текущий ID обьекта
				selectedObject = selectBuffer[(i * 4) + 3];
			}
		}
		// Вернем выбранный обьект
		return selectedObject;
	}
	// Если не щелкнули ни на 1 обьект, вернём 0
	return 0;
}

void display() {
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glLoadIdentity();

	glInitNames();

	/*GLfloat material_diffuse[] = { 0.7, 0.2, 0.2, 1.0 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse);
	GLfloat light0_diffuse[] = { 0.9, 0.9, 1.0 };
	GLfloat light0_position[] = { 0.0, 0.0, 1000.0, 0.0 };
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/

	/*glMatrixMode(GL_PROJECTION);
	
	glOrtho(-1024, 1024, -768, 768, -1000.0f, 1000.0f);*/

	//glRotatef(Angle, 1.0f, 1.0f, 1.0f);
	//Angle += 0.01;

	glBegin(GL_LINES);
	glColor3f(0.1, 0.2, 0.3);
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

	glPushMatrix();
	glPushName(300); // Присвоение треугольнику идентификатора '300
	glTranslatef(pansqx, pansqy, 0.0f);
	glBegin(GL_QUADS);

	if (pickpoly)
		glColor3f(1.0f, 0.0f, 0.0f);
	else
		glColor3f(0.5f, 0.0f, 1.0f);
	//glNormal3f(0, 0, -1);

	glVertex3f(100.0f, 100.0f, 0);
	glVertex3f(200.0f, 100.0f, 0);
	glVertex3f(200.0f, 200.0f, 0);
	glVertex3f(100.0f, 200.0f, 0);
	glEnd();
	glPopName();
	glPopMatrix();

	glFlush();

	glMatrixMode(GL_PROJECTION); // Выбор матрицы проектирования
	glLoadIdentity();
	//gluPerspective(45.0f,(GLfloat)OGL_W_W/(GLfloat)OGL_W_H,-0.1f,150.0f);
	glOrtho(-1024, 1024, -768, 768, -1000.0f, 1000.0f);
	glMatrixMode(GL_MODELVIEW); // Выбор матрицы просмотра вида
	glLoadIdentity();
}

void lmbutton(int button, int state, int x, int y)
{
	int ObjectID;
	ObjectID = RetrieveObjectID(x, y);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && ObjectID == 300) {
		//squarepan = true;
		xStart = x;
		yStart = y;
		pickpoly = true;

		HCURSOR hcHand = LoadCursor(NULL, IDC_HAND);
		SetCursor(hcHand);
		//glutSetCursor(GLUT_CURSOR_INFO);
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		pickpoly = false;

		HCURSOR hcArrow = LoadCursor(NULL, IDC_ARROW);
		SetCursor(hcArrow);
		//glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
	}
}

void mouseMove(int x, int y)
{
	if (pickpoly) {
		int dx, dy;
		dx = xStart - x;
		dy = yStart - y;
		
		pansqx -= dx * (1024.0 / win_width) * 2.0;
		xStart = x;

		pansqy += dy * (768.0 / win_height) * 2.0;
		yStart = y;
	}
}

void on_resize(int w, int h)
{
	win_width = w;
	win_height = h;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	display(); // refresh window.
}

int main(int argc, char** argv)
{
	HWND hConsole = GetConsoleWindow();
	ShowWindow(hConsole, SW_HIDE);//прячем оконо консоли

	glutInit(&argc, argv);
	//glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1024, 768);
	glutInitWindowPosition(0, 0);
	//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutCreateWindow("Triangle");
	
	glutDisplayFunc(display);
	glutIdleFunc(display);
	//glutPostRedisplay();

	//Обработчики мыши
	glutMouseFunc(lmbutton);
	glutMotionFunc(mouseMove);

	glutReshapeFunc(on_resize);
	

	glutMainLoop();
	return 0;
}