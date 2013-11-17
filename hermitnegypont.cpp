#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

int ww = 1200;//ablak szelesseg
int wh = 600;//ablak magassag

int draggedPoint = -1;//Megragadott pont
int pickRadius = 10;//megfogasi tavolsag

#define N 4 // Kontroll pontok

typedef struct Pont {
	GLfloat x,y;
} PONT;

GLfloat ctrlPointsColor[N][3] = {
	{1.0, 0.0, 0.0},	//P1	piros
	{0.0, 1.0, 0.0},	//P2	zold
	{1.0, 1.0, 0.0},	//P3	sarga
	{0.7, 0.5, 0.9}		//P4	lila
};

GLfloat G[3][4] = 
{
	//P1,P2,P3,P4
	{ 100, 150, 200, 250 },//x
	{ 200, 300, 300, 200 },//y
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
GLfloat savPontok[4][2] = {
	{0,0},
	{0,0},
	{0,0},
	{0,0}
};
static GLfloat MiT[4] = { 0, 0, 0, 0 };
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
static GLfloat _T[4] = { -1, 0, 1, 2 };
static GLfloat GMiT[3] = { 0, 0, 0 };
// window to view porthoz
float vright; float vbottom; float vleft; float vtop;	// Ahova akarjuk ablakmeret
float wright; float wbottom; float wleft; float wtop;	// Ahonnan akarjuk ablakmeret


float min = _T[0];
float max = _T[0];
void minmax()
{
	min = _T[0];
	max = _T[0];
	for (int i = 0; i < 4; i++)
	{
		if(_T[i] > max)
			max = _T[i];
		if(_T[i] < min)
			min = _T[i];
	}
}
void initM()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			M[j][i] = pow(_T[i], 3-j);
		}
	}
	minmax();
}
void MatrixSzorzas_4x4vs4x1( float A[4][4],float B[4], float C[4])
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
float Determinans3x3Matrix(float C[3][3])
{
	float Pozitiv=C[0][0]*C[1][1]*C[2][2]+C[1][0]*C[2][1]*C[0][2]+C[0][1]*C[1][2]*C[2][0];
	float Negativ=C[0][2]*C[1][1]*C[2][0]+C[1][2]*C[2][1]*C[0][0]+C[0][1]*C[1][0]*C[2][2];
	return    Pozitiv-Negativ;
}
void aldeterminans(float A[4][4],float B[3][3],int akti,int aktj)
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
	float det =0;
	
	for(int i=0;i<4;i++)
	{
		aldeterminans(A,E,0,i);
		det+=A[0][i]*pow(-1,i)*Determinans3x3Matrix(E);
	}

	if(det!=0){
		for(int i=0; i<4; i++)
		{
			for(int j=0; j<4; j++)
			{
				aldeterminans(A,E,i,j);
				inverz[i][j]=1/det*pow(-1,(i+1)+(j+1))*Determinans3x3Matrix(E);
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
void WindowToViewPort(Pont * p)
{
	p->x = vleft + (p->x - wleft)*((vright-vleft)/(wright-wleft));
	p->y = vbottom + (p->y - wbottom)*((vtop-vbottom)/(wtop-wbottom));
}
void display2DControlPoints()
{
	glPointSize(10.0);
	for (int i = 0; i < 4; i++)
	{
		glBegin(GL_POINTS);
		glColor3fv(ctrlPointsColor[i]);
		glVertex2f(G[0][i], G[1][i]);
		glEnd();
	}
}
void displayFelezo()
{
	glBegin(GL_LINES);
	glColor3f(0.2,0.4,0.6);
	glVertex2f(ww/2+100,wh);
	glVertex2f(ww/2+100,0);
	glEnd();
}
void displayKoordinataRendszer()
{
	glColor3f(1.0f, 1.0f, 1.0f);
	vright = ww; vbottom = 0; vleft = ww/2+100; vtop = wh;
	wright = 15; wbottom = -15; wleft = -15; wtop = 15;
	glBegin(GL_LINES);
	Pont vBal, vJobb, fFelso, fAlso;
	vBal.x = -15; vBal.y = 0; vJobb.x = 15; vJobb.y = 0;
	fAlso.x = 0; fAlso.y = -15; fFelso.x = 0; fFelso.y = 15;

	WindowToViewPort(&vBal); WindowToViewPort(&vJobb); WindowToViewPort(&fFelso); WindowToViewPort(&fAlso);

	glVertex2f(vBal.x,vBal.y);
	glVertex2f(vJobb.x,vJobb.y);

	glVertex2f(fAlso.x,fAlso.y);
	glVertex2f(fFelso.x,fFelso.y);
	glEnd();
}


void displaySav()
{
	glBegin(GL_LINES);
	glColor3f(1.0,1.0,1.0);
	glVertex2f(50,50);
	glVertex2f(500+100+50,50);
	glEnd();
	for (int i = 0; i < 4; i++)
	{
		glBegin(GL_POINTS);
		glColor3fv(ctrlPointsColor[i]);

		glVertex2fv(savPontok[i]);
		glEnd();
	}

}
void initSavPontok()
{
	for (int i = 0; i < 4; i++)
	{
		savPontok[i][0] = 300+50 + _T[i]*100;
		savPontok[i][1] = 50;
	}
}
void Hermite2()
{
	initM();
	MInverz(M,M_Inverz);
	glBegin(GL_LINE_STRIP);
	glColor3f(0.2,0.4,0.7);
	MatrixSzorzas_3x4vs4x4(G,M_Inverz,C);
	for (float i = min; i < max; i+=0.001)
	{
		for (int j = 0; j < 4; j++)
		{
			T[j] = pow(i,3-j);
		}
		MatrixSzorzas_3x4vs4x1(C,T,CT);
		glVertex2f(CT[0],CT[1]);
	}
	glEnd();
}
void displaySulyfuggveny()
{
	MInverz(M,M_Inverz);
	minmax();
	vright = ww; vbottom = 0; vleft = ww/2+100; vtop = wh;
	wright = 3; wbottom = -3; wleft = -3; wtop = 3;
	for (int i = 0; i < 4; i++)
	{
		glBegin(GL_LINE_STRIP);
		glColor3fv(ctrlPointsColor[i]);
		Pont p;
		for (float j = min; j < max; j+=0.01)
		{
			p.x=j;
			p.y = M_Inverz[i][0]*pow(j,3) + M_Inverz[i][1]*pow(j,2) + M_Inverz[i][2]*pow(j,1) + M_Inverz[i][3]*pow(j,0);
			WindowToViewPort(&p);
			glVertex2f(p.x,p.y);
		}
		glEnd();
	}
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
	displaySulyfuggveny();
	displayFelezo();
	displayKoordinataRendszer();
	displaySav();
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

		for (int i = 0; i < N && !choiceFound; i++)
		{
			if ((abs(G[0][i] - newX) <= pickRadius) &&
				(abs(G[1][i] - newY) <= pickRadius))
			{
				draggedPoint = i;
				choiceFound = 1;
			}
		}
		for (int i = 0; i < N && !choiceFound; i++)
		{
			if ((abs(savPontok[i][0] - newX) <= pickRadius) &&
				(abs(savPontok[i][1] - newY) <= pickRadius))
			{
				draggedPoint = i+4;
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
	if ((draggedPoint > -1) && (draggedPoint < 4))
	{
		GLuint newX = xPosition;
		GLuint newY = wh - yPosition;

		if(newX <=0 || newX >= ww/2)
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
	if(draggedPoint>3)
	{
		GLuint newX = xPosition;
		if(newX < 50)
		{
			savPontok[draggedPoint-4][0]=50;
			_T[draggedPoint-4] = -3;
			glutPostRedisplay();
			return;
		}
		if(newX > ww/2+50)
		{
			savPontok[draggedPoint-4][0] = ww/2+50;
			_T[draggedPoint-4] = 3;
			glutPostRedisplay();
			return;
		}
		savPontok[draggedPoint-4][0] = newX;
		_T[draggedPoint-4] = (savPontok[draggedPoint-4][0]-350)/100.0;
		glutPostRedisplay();
	}
}

int main( int argc, char** argv ) {
	initSavPontok();
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize( ww, wh );
	glutInitWindowPosition( 100,200 );
	glutCreateWindow( "HaziFeladat" );
	init( );
	glutDisplayFunc( display );
	glutMouseFunc( myPick );
	glutKeyboardFunc( keyboard );
	glutMotionFunc( myMouseMove );
	glutMainLoop( );
	return 0;
}

