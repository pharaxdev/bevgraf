#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

int ww = 1000;//ablak szelesseg
int wh = 600;//ablak magassag

int draggedPoint = -1;//Megragadott pont
int pickRadius = 10;//megfogasi tavolsag

#define N 4 // Kontroll pontok
#define D 2 // Dimenziok szama

typedef struct Pont {
	GLfloat x,y;
} PONT;

GLfloat ctrlPointsColor[N][3] = {
	{1.0, 1.0, 0.0},	//P1	sarga
	{1.0, 1.0, 0.0},	//P2	sarga
};

GLfloat G[3][4] = 
{
	//P1,P2,R1,R2
	{ 700, 450, 0, 0 },//x
	{ 300, 150, 0, 0 },//y
	{ 1, 1, 1, 1 } //1
};
static GLfloat M[4][4] =
{
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0}
};

static GLfloat M_Inverz[4][4] = // M inverze 
{
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0},
	{0, 0, 0, 0}
};
static GLfloat T[4] = 
{
	0,//t3
	0,//t2
	0,//t1
	1 //1
};

static GLfloat C[3][4] = //G*MInverz
{
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0}
};
static GLfloat CT[3] =
{
	0,0,0
};
static GLfloat ErintoPontok[2][2] = { {500,200}, {400,400} };
static GLfloat _T[2] = { 0, 1 };
// window to view porthoz
float vright; float vbottom; float vleft; float vtop;	// Ahova akarjuk ablakmeret
float wright; float wbottom; float wleft; float wtop;	// Ahonnan akarjuk ablakmeret

void initM()
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			M[j][i] = pow(_T[i], 3-j);
		}
	}
	for (int i = 2; i < 4; i++)
	{
		M[0][i] = 3*_T[i-2]*_T[i-2];
		M[1][i] = 2*_T[i-2];
		M[2][i] = 1;
		M[3][i] = 0;
	}
}
void MatrixSzorzas_4x4vs4x1( float A[4][4],float B[4], float  C[4])
{
	for(int i = 0; i<4; i++)
	{
		float sum = 0;
		for (int j = 0; j < 4; j++)
		{
			sum += A[i][j]*B[j];
		}
		C[i] = sum;
	}
}
void MatrixSzorzas_3x4vs4x1( float A[3][4],float B[4], float C[3])
{
	for (int i = 0; i < 3; i++)
	{
		float sum = 0;
		for (int j = 0; j < 4; j++)
		{
			sum += A[i][j]*B[j];
		}
		C[i] = sum;
	}
}
void MatrixSzorzas_3x4vs4x4( float A[3][4], float B[4][4], float C[3][4])
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			float sum = 0;
			for (int k = 0; k < 4; k++)
			{
				sum += A[i][k]*B[k][j];
			}
			C[i][j] = sum;

		}
	}
}

float Determinans3x3Matrix(float C[3][3])	// 3x3 matrix determinansa
{
	float Pozitiv=C[0][0]*C[1][1]*C[2][2]+C[1][0]*C[2][1]*C[0][2]+C[0][1]*C[1][2]*C[2][0];
	float Negativ=C[0][2]*C[1][1]*C[2][0]+C[1][2]*C[2][1]*C[0][0]+C[0][1]*C[1][0]*C[2][2];
	return    Pozitiv-Negativ;
}
void aldeterminans(float A[4][4],float B[3][3],int akti,int aktj)	// aldete
{
	int l=0;
	for(int i=0; i<4; i++)
	{
		int k=0;
		if(i==akti) continue;

		for(int j=0; j<4; j++)
		{
			if(j==aktj) continue;
			B[l][k]=A[i][j];
			k++;
		}
		l++;
	}
}
void MInverz(float A[4][4], float B[4][4])
{
	float E[3][3];
	float inverz[4][4];
	float detD =0;
	for(int i=0; i<4; i++)
	{
		for(int j=0; j<4; j++)
		{
			B[i][j]=inverz[j][i];
		}
	}

	for(int i=0;i<4;i++)
	{
		aldeterminans(A,E,0,i);
		detD+=A[0][i]*pow(-1,i)*Determinans3x3Matrix(E);
	}

	if(detD!=0)
	{
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<4; j++)
			{
				aldeterminans(A,E,i,j);
				inverz[i][j]=1/detD*pow(-1,(i+1)+(j+1))*Determinans3x3Matrix(E);
			}
		}
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<4; j++)
			{
				B[i][j]=inverz[j][i];
			}
		}
	}
}
void initG()
{
	G[0][2] = ErintoPontok[0][0] - G[0][0];
	G[1][2] = ErintoPontok[0][1] - G[1][0];
	G[0][3] = ErintoPontok[1][0] - G[0][1];
	G[1][3] = ErintoPontok[1][1] - G[1][1];
}
void display2DControlPoints()
{
	glPointSize(10.0);

	for (int i = 0; i < 2; i++)
	{
		glBegin(GL_POINTS);
		glColor3fv(ctrlPointsColor[i]);
		glVertex2f(G[0][i], G[1][i]);
		glEnd();
	}
	for (int i = 0; i < 2; i++)
	{
		glBegin(GL_POINTS);
		glColor3f(0.7,0.7,0.7);
		glVertex2f(ErintoPontok[i][0],ErintoPontok[i][1]);
		glEnd();
	}
	glFlush();
}


void Hermite2()
{
	initM();
	MInverz(M,M_Inverz);
	glBegin(GL_LINE_STRIP);
	glColor3f(0.2,0.4,0.7);
	initG();
	MatrixSzorzas_3x4vs4x4(G,M_Inverz,C);
	for (float i = _T[0]; i < _T[1]; i+=0.01)
	{
		T[0] = i*i*i;
		T[1] = i*i;
		T[2] = i;
		T[3] = 1;
		MatrixSzorzas_3x4vs4x1(C,T,CT);
		glVertex2f(CT[0],CT[1]);
	}
	glEnd();
	glBegin(GL_LINE_STRIP);
	glColor3f(0.5,0.5,0.5);
	glVertex2f(G[0][0], G[1][0]);
	glVertex2f(ErintoPontok[0][0], ErintoPontok[0][1]);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glColor3f(0.5,0.5,0.5);
	glVertex2f(G[0][1], G[1][1]);
	glVertex2f(ErintoPontok[1][0], ErintoPontok[1][1]);
	glEnd();
}
void init( ) {
	glClearColor( 0.0, 0.0, 0.0, 0.0 );
	glMatrixMode( GL_PROJECTION );
	gluOrtho2D( 0.0, ww, 0.0, wh );
	glShadeModel( GL_FLAT );
	glEnable( GL_POINT_SMOOTH );
	glPointSize( 10.0 );
	glLineWidth( 1.5 );
}

void display( ) {
	glClear(GL_COLOR_BUFFER_BIT);
	Hermite2();
	display2DControlPoints();
	glFlush();
	glutSwapBuffers( );
}

void keyboard( unsigned char key, int x, int y ) {
	switch( key ) {
	case 27:
		exit(1);
	}
}

void myPick(int button, int state, int xPosition, int yPosition)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		GLuint newX = xPosition;
		GLuint newY = wh - yPosition;

		int choiceFound = 0;

		for (int i = 0; i < 2 && !choiceFound; i++)
		{
			if ((abs(G[0][i] - newX) <= pickRadius) &&
				(abs(G[1][i] - newY) <= pickRadius))
			{
				draggedPoint = i;
				choiceFound = 1;
			}
		}
		for (int i = 0; i < 2 && !choiceFound; i++)
		{
			if ((abs(ErintoPontok[i][0] - newX) <= pickRadius) &&
				(abs(ErintoPontok[i][1] - newY) <= pickRadius))
			{
				draggedPoint = i+2;
				choiceFound = 1;
			}
		}
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		draggedPoint = -1;
	}
}
void myMouseMove(int xPosition, int yPosition)
{
	if (draggedPoint > -1 && draggedPoint < 2)
	{
		GLuint newX = xPosition;
		GLuint newY = wh - yPosition;

		if(newX <=0 || newX >= ww)
		{
			if(newY <=0 || newY >= wh)
				return;
			G[1][draggedPoint] = newY;
			glutPostRedisplay();
			return;
		}
		else if(newY <=0 || newY >= wh)
		{
			G[0][draggedPoint] = newX;
			glutPostRedisplay();
			return;
		}
		else
		{
			G[0][draggedPoint] = newX;
			G[1][draggedPoint] = newY;
			glutPostRedisplay();
			return;
		}
	}
	if(draggedPoint > 1)
	{
		GLuint newX = xPosition;
		GLuint newY = wh - yPosition;

		if(newX <=0 || newX >= ww)
		{
			if(newY <=0 || newY >= wh)
				return;
			ErintoPontok[draggedPoint-2][1] = newY;
			glutPostRedisplay();
			return;
		}
		else if(newY <=0 || newY >= wh)
		{
			ErintoPontok[draggedPoint-2][0] = newX;
			glutPostRedisplay();
			return;
		}
		else
		{
			ErintoPontok[draggedPoint-2][0] = newX;
			ErintoPontok[draggedPoint-2][1] = newY;
			glutPostRedisplay();
			return;
		}
	}

}

int main( int argc, char** argv ) {
	initM();
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize( ww, wh );
	glutInitWindowPosition( 400,200 );
	glutCreateWindow( "HaziFeladat" );
	init( );
	glutDisplayFunc( display );
	glutMouseFunc( myPick );
	glutKeyboardFunc( keyboard );
	glutMotionFunc( myMouseMove );
	glutMainLoop( );
	return 0;
}



