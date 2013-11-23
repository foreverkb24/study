#include <iostream>
#include <glut.h>
#include <fstream>
using namespace std;

static const long VERTEXS_NUM = 100000; //初始化一个足够大的顶点数组来存放顶点信息
static const long FACES_NUM = 1000000;  //初始化一个足够大的面数组来存放面信息
static const int BUFFER = 100;  //读文件时字符串缓冲区大小
//static const char *filename = "E:/c++project/Test_06/bunny.ply";
//static const char *filename = "E:/c++project/Test_06/47.ply2";
static const char *filename = "E:/c++project/Test_06/bunny_iHsmooth.ply2";

/*定义一个ply文件中顶点坐标模型，包括x轴，y轴，z轴坐标*/
struct PLY {
	float vertexX, vertexY, vertexZ;
};

/*定义ply文件中面的模型，包括构成面的三个点A,B,C*/
struct FaceToPLY {
	int pointA, pointB, pointC;
};

/*将文件中读出的顶点坐标信息放在vertexs数组中*/
PLY vertexs[VERTEXS_NUM];

/*将文件中读出的面的信息放在数组中*/
FaceToPLY faces[FACES_NUM];

/*定义顶点个数和面的个数，都从0开始，每次加一*/
int vertexsValue = -1;
int facesValue = -1;

static GLfloat xRot = 0.0f;
static GLfloat yRot = 0.0f;
static GLfloat u = 0.0f;
static GLfloat v = 0.0f;
static GLfloat m = 0;
static GLfloat n = 0;
void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glTranslated(u, v, -40);
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	if (m >= 1)
		glScalef(m*2.0f, m*2.0f, m*2.0f);
	if (n >= 1)
		glScalef(0.5 / n, 0.5 / n, 0.5 / n);
	glInitNames();
	glPushName(0);

	for (int j = 1; j <= facesValue; ++j) {
		glBegin(GL_TRIANGLES);
		glColor3f(0.2, 0.6, 0.3);
		glVertex3f(vertexs[faces[j].pointA].vertexX,
			vertexs[faces[j].pointA].vertexY,
			vertexs[faces[j].pointA].vertexZ);
		glVertex3f(vertexs[faces[j].pointB].vertexX,
			vertexs[faces[j].pointB].vertexY,
			vertexs[faces[j].pointB].vertexZ);
		glVertex3f(vertexs[faces[j].pointC].vertexX,
			vertexs[faces[j].pointC].vertexY,
			vertexs[faces[j].pointC].vertexZ);
		glEnd();
	}
	for (int i = 0; i <= vertexsValue; ++i) {
		glColor3f(0.4, 0.4, 0.1);
		glLoadName(i);
		glBegin(GL_POINTS);
		glVertex3f(vertexs[i].vertexX, vertexs[i].vertexY, vertexs[i].vertexZ);
		glEnd();
	}
	glPopMatrix();
	glutSwapBuffers();
}

/*解析读入的数据*/
void parseData(char *data){

	char *result = NULL, *next = NULL;
	result = strtok_s(data, " ", &next);
	if (data[0] != '3') {
		++vertexsValue;
		vertexs[vertexsValue].vertexX = atof(result);
		result = strtok_s(NULL, " ", &next);
		vertexs[vertexsValue].vertexY = atof(result);
		result = strtok_s(NULL, " ", &next);
		vertexs[vertexsValue].vertexZ = atof(result);
	}
	else if (data[0] == '3' && data[1] == '.') {
		++vertexsValue;
		vertexs[vertexsValue].vertexX = atof(result);
		result = strtok_s(NULL, " ", &next);
		vertexs[vertexsValue].vertexY = atof(result);
		result = strtok_s(NULL, " ", &next);
		vertexs[vertexsValue].vertexZ = atof(result);
	}
	else {
		++facesValue;
		result = strtok_s(NULL, " ", &next);
		faces[facesValue].pointA = atoi(result);
		result = strtok_s(NULL, " ", &next);
		faces[facesValue].pointB = atoi(result);
		result = strtok_s(NULL, " ", &next);
		faces[facesValue].pointC = atoi(result);
	}
}

/*加载指定文件中的数据*/
void readFile(const char *filename) {
	ifstream file;
	char data[BUFFER];       //每次读入一行数据，存入data中，再对data进行解析
	file.open(filename, ios::in);
	while (file.getline(data, BUFFER)) {
		//getline每次读入一行数据，如果读入的数据开始是数字或者是负号开始的
		//则将数据放入parseData()函数中解析,否则不作处理
		if (data[0] > 44 && data[0] < 58) {
			parseData(data);
		}
	}
	cout << "数据加载成功!" << endl;
	file.close();
}


void SpecialKeys(int key, int x, int y) {

	if (key == GLUT_KEY_F1) u -= 2.5f;
	if (key == GLUT_KEY_F2)	u += 2.5f;
	if (key == GLUT_KEY_F3) v -= 2.5f;
	if (key == GLUT_KEY_F4) v += 2.5f;
	if (key == GLUT_KEY_F5)	m += 1.0f;
	if (key == GLUT_KEY_F6)	n += 1.0f;
	if (key == GLUT_KEY_UP) xRot -= 5.0f;
	if (key == GLUT_KEY_DOWN) xRot += 5.0f;
	if (key == GLUT_KEY_LEFT) yRot -= 5.0f;
	if (key == GLUT_KEY_RIGHT) yRot += 5.0f;
	if (key > 356.0f) xRot = 0.0f;
	if (key < -1.0f) xRot = 355.0f;
	if (key > 356.0f) yRot = 0.0f;
	if (key < -1.0f) yRot = 355.0f;
	// Refresh the Window
	glutPostRedisplay();
}

void ChangeSize(int w,int h) {
	GLfloat fAspect;
	// Prevent a divide by zero
	if (h == 0)
		h = 1;
	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);
	// Calculate aspect ratio of the window
	fAspect = (GLfloat)w / (GLfloat)h;
	// Set the perspective coordinate system
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// Field of view of 45 degrees, near and far planes 1.0 and 425
	gluPerspective(45.0f, fAspect, 1.0, 425.0);
	// Modelview matrix reset
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void SetupRC()
{
	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Set drawing color to green
	glColor3f(0.0f, 1.0f, 0.0f);
	// Set color shading model to flat
	glShadeModel(GL_SMOOTH);
	// Clock wise wound polygons are front facing, this is reversed
	// because we are using triangle fans
	glFrontFace(GL_CW);
}

void TimerFunction(int value)
{
	glutPostRedisplay();
	glutTimerFunc(33, TimerFunction, 1);
}

int main(int argc, char *argv[]) {
	readFile(filename);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGl");
	
	SetupRC();
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	glutDisplayFunc(display);
	glutTimerFunc(33, TimerFunction, 1);
	glutMainLoop();
	return 0;
}