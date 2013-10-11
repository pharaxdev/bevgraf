#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

GLsizei winWidth = 800, winHeight = 800;
GLint dragged = -1;
const int N = 60;


class Point {
public:
	float x,y;
	void init( float _x, float _y) { this->x = _x; this->y = _y; }
};
class Circle
{
public:
	Point midPoint;
	Circle(int, int, float);
	float Radius;
	Point CirclePoints [ N ];
	void Draw();
	void CalcCirclePoints();

};
Circle::Circle(int x, int y, float Radius)
{
	this->Radius = Radius;
	midPoint.init(x,y);
	float step=2*M_PI/N;
	int m = 0;
	for(float i=0.0; i<2*M_PI; i+=step)
	{
		CirclePoints[m].x=midPoint.x+Radius*cos(i);
		CirclePoints[m].y=midPoint.y+Radius*sin(i);
		m++;
	}
}
void Circle::Draw()
{
	for(int i=0; i< N; i++)
	{
		glVertex2f(this->CirclePoints[i].x,this->CirclePoints[i].y);
	}
}
void Circle::CalcCirclePoints()
{
	float step=2*M_PI/N;
	int m = 0;
	for(float i=0.0; i<2*M_PI; i+=step)
	{
		CirclePoints[m].x=midPoint.x+Radius*cos(i);
		CirclePoints[m].y=midPoint.y+Radius*sin(i);
		m++;
	}
}
GLint Round( GLfloat N ) {return (GLint)( N + 0.5 ); }
void init( ) {
	glClearColor( 1.0, 1.0, 1.0, 0.0 );
	glMatrixMode( GL_PROJECTION );
	gluOrtho2D( 0.0, winWidth, 0.0, winHeight );
	glShadeModel( GL_FLAT );
	glEnable( GL_POINT_SMOOTH );
	glPointSize( 10.0 );
	glLineWidth( 1.2 );
}
Circle * centerCircle = new Circle(winWidth/2,winHeight/2,100);
Circle * movingCircle = new Circle(600,400,100);
Point Points[2];
Point mvCirclePPoint;
float radLong = 0;

void display( ) {
	glClear( GL_COLOR_BUFFER_BIT ) ;
	float x,y;

	glColor3f(0.0,0.0,1.0);
	glBegin( GL_LINE_LOOP );// forgó kör	
	movingCircle->Draw();
	glEnd();

	glColor3f(1.0,0.0,0.0);
	glBegin( GL_LINE_LOOP );// középsõ kör
	centerCircle->Draw();
	glEnd();

	glBegin( GL_LINES );	// a csuszka savja
	glVertex2i(50,50);
	glVertex2i(750,50);
	glEnd();

	glBegin( GL_POINTS );	// a csuszka pontja
	glVertex2f(Points[1].x,Points[1].y);
	glEnd();

	glColor3f(0.0,0.0,1.0);
	glBegin( GL_LINE_STRIP );
	for (float i=0; i<radLong; i+=0.1)
	{
		x=(centerCircle->Radius+movingCircle->Radius)*cos(i) - movingCircle->Radius*cos(((centerCircle->Radius+movingCircle->Radius)/movingCircle->Radius)*i);
		y=(centerCircle->Radius+movingCircle->Radius)*sin(i) - movingCircle->Radius*sin(((centerCircle->Radius+movingCircle->Radius)/movingCircle->Radius)*i);
		glVertex2f(x+centerCircle->midPoint.x,y+centerCircle->midPoint.y);
		movingCircle->midPoint.x = (centerCircle->Radius+movingCircle->Radius)*cos(i)+centerCircle->midPoint.x;
		movingCircle->midPoint.y = (centerCircle->Radius+movingCircle->Radius)*sin(i)+centerCircle->midPoint.y;
		movingCircle->CalcCirclePoints();
		//mvCirclePPoint.x = movingCircle->Radius*cos(2*radLong);
		//mvCirclePPoint.y = movingCircle->Radius*sin(2*radLong);
		glutPostRedisplay();
	}
	glEnd();
	/*
	// Szamoljuk a P pontot...
	glColor3f(1.0,0.0,0.0);
	glBegin(GL_POINTS);
	glVertex2f(movingCircle->midPoint.x-mvCirclePPoint.x,movingCircle->midPoint.y-mvCirclePPoint.y);
	glEnd();
	*/

	glBegin(GL_POINTS);	// P pont
	if(radLong == 0)
		glVertex2f(centerCircle->midPoint.x+centerCircle->Radius,y+centerCircle->midPoint.y);
	else
		glVertex2f(x+centerCircle->midPoint.x,y+centerCircle->midPoint.y);
	glEnd();

	glColor3f(1.0,0.0,0.0);	// a sugar beallito pont
	glBegin( GL_POINTS );
	glVertex2f(Points[0].x,Points[0].y);
	glEnd();

	glutSwapBuffers( );
}

GLfloat dist2( Point P1, Point P2 ) {
	GLfloat d1 = P1.x - P2.x;
	GLfloat d2 = P1.y - P2.y;
	return d1 * d1 + d2 * d2;
}

GLint getActivePoint1( Point *p, GLint size, GLint sens, GLint x, GLint y ) {
	GLint i, s = sens * sens;
	Point P;
	P.x = x;
	P.y = y;

	for ( i = 0; i < size; i++ )
		if ( dist2( p[i], P ) < s )
			return i;
	return -1;
}

GLint getActivePoint2( Point *p, GLint size, GLint sens, GLint x, GLint y ) {
	GLint i;
	for ( i = 0; i < size; i++ )
		if ( fabs( (*( p + i )).x - x ) < sens && fabs( (*( p + i )).y - y ) < sens )
			return i;
	return -1;
}

void processMouse( GLint button, GLint action, GLint xMouse , GLint yMouse ) {
	GLint i;
	if ( button == GLUT_LEFT_BUTTON && action == GLUT_DOWN )
		if ( ( i = getActivePoint1( Points, 2, 8, xMouse, winHeight - yMouse ) ) != -1 )
			dragged = i;
	if ( button == GLUT_LEFT_BUTTON && action == GLUT_UP  )
		dragged = -1;
}

void processMouseActiveMotion( GLint xMouse, GLint yMouse ) {
	GLint i;
	if ( dragged == 0 )
	{
		if( Points[1].x != 50 )
			return;
		centerCircle->Radius = xMouse - centerCircle->midPoint.x;
		centerCircle->CalcCirclePoints();
		movingCircle->Radius = movingCircle->midPoint.x - xMouse;
		movingCircle->CalcCirclePoints();
		Points[0].x = xMouse;
		glutPostRedisplay( );
	}
	else if( dragged > 0 )
	{
		if(xMouse>750)
		{
			Points[1].x = 750;
			return;
		}
		else if(xMouse<50)
		{
			Points[1].x=50;
			radLong=0;
			movingCircle->midPoint.x=centerCircle->midPoint.x+centerCircle->Radius+movingCircle->Radius;
			movingCircle->midPoint.y=centerCircle->midPoint.y;
			movingCircle->CalcCirclePoints();
			glutPostRedisplay();
			return;
		}
		Points[1].x = xMouse;
		radLong = ((Points[1].x-50) / 700)*100*M_PI;
		glutPostRedisplay();
	}
}
void keyboard( unsigned char key, int x, int y ) {
	switch( key ) {
	case 'r':
		radLong = 0;
		movingCircle->midPoint.x=centerCircle->midPoint.x+centerCircle->Radius+movingCircle->Radius;
		movingCircle->midPoint.y=centerCircle->midPoint.y;
		movingCircle->CalcCirclePoints();
		Points[1].x=50;
		glutPostRedisplay();
		break;
	case 't':
		centerCircle->Radius=movingCircle->Radius=100;
		centerCircle->CalcCirclePoints();
		movingCircle->CalcCirclePoints();
		Points[0].x = centerCircle->midPoint.x+centerCircle->Radius;
		glutPostRedisplay();
		break;
	case 'd':
		std::cout << "Kozepso kor sugara: " << centerCircle->Radius << "\nKulso kor sugara: " << movingCircle->Radius << std::endl;
		break;
	case 'b':
		std::cout << "Kozepso kor sugara: ";
		std::cin >> centerCircle->Radius;
		std::cout << "Kulso kor sugara: ";
		std::cin >> movingCircle->Radius;
		radLong = 0;
		centerCircle->CalcCirclePoints();
		movingCircle->midPoint.x=centerCircle->midPoint.x+centerCircle->Radius+movingCircle->Radius;
		movingCircle->midPoint.y=centerCircle->midPoint.y;
		movingCircle->CalcCirclePoints();
		Points[1].x=50;
		Points[0].x=centerCircle->midPoint.x+centerCircle->Radius;
		glutPostRedisplay();
		break;
	case 'h':
		std::cout << "r - Visszaallitja a ket kort a kezdo allapotba, de a sugaruk nem valtozik\n";
		std::cout << "t - Visszaallitja a ket kort a kezdo allpotba, a sugaruk visszaall az EREDETIRE\n";
		std::cout << "d - Kiirja a ket kor aktualis sugarat\n";
		std::cout << "b - Bekeri a ket kor sugarat es arra allitja be a koroket (CSAK EGESZ SZAM)\n";
		break;
	case 27:
		exit(1);

	}
	glutPostRedisplay( );
}

int main( int argc, char** argv ) {
	Points[0].init(centerCircle->midPoint.x+centerCircle->Radius,centerCircle->midPoint.y);
	Points[1].init(50,50);
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize( winWidth, winHeight );
	glutInitWindowPosition( 600,0 );
	glutCreateWindow( "HaziFeladat" );
	init( );
	glutDisplayFunc( display );
	glutMouseFunc( processMouse );
	glutKeyboardFunc( keyboard );
	glutMotionFunc( processMouseActiveMotion );
	glutMainLoop( );
	return 0;
}

