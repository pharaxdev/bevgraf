#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

GLsizei winWidth = 800, winHeight = 800;

GLint dragged = -1;

GLint Round( GLfloat n ) {return (GLint)( n + 0.5 ); }
struct Pont {
	float x,y;
};
const int n = 30;
class Circle
{
public:
	Pont kozepPont;
	Circle(int x, int y, int r);
	int r;
	Pont pontjai [ n +1 ];
	void rajzol();
	void szamol();

};
Circle::Circle(int x, int y, int r)
{
	this->r = r;
	kozepPont.x = x;
	kozepPont.y = y;
	float lepes=2*M_PI/n;
	int m = 0;
	for(float i=0.0; i<2*M_PI; i+=lepes)
	{
		pontjai[m].x=kozepPont.x+r*cos(i);
		pontjai[m].y=kozepPont.y+r*sin(i);
		m++;
	}
}
void Circle::rajzol()
{
	for(int i=0; i< n; i++)
	{
		glVertex2f(this->pontjai[i].x,this->pontjai[i].y);
	}
}
void Circle::szamol()
{
	float lepes=2*M_PI/n;
	int m = 0;
	for(float i=0.0; i<2*M_PI; i+=lepes)
	{
		pontjai[m].x=kozepPont.x+r*cos(i);
		pontjai[m].y=kozepPont.y+r*sin(i);
		m++;
	}
}

void init( ) {
	glClearColor( 1.0, 1.0, 1.0, 0.0 );
	glMatrixMode( GL_PROJECTION );
	gluOrtho2D( 0.0, winWidth, 0.0, winHeight );
	glShadeModel( GL_FLAT );
	glEnable( GL_POINT_SMOOTH );
	glPointSize( 10.0 );
	glLineWidth( 1.2 );
}
Circle * nagykor = new Circle(winWidth/2,winHeight/2,100);
Circle * kiskor = new Circle(600,400,100);
Pont kozosPont;
Pont pont;
Pont pontok[2];
float meddig = 0;

void display( ) {
	glClear( GL_COLOR_BUFFER_BIT ) ;
	glColor3f(0.0,0.0,0.0);
	float x,y;

	glBegin( GL_LINE_LOOP );
	nagykor->rajzol();
	glEnd();
	glBegin( GL_LINE_LOOP );
	kiskor->rajzol();
	glEnd();

	for(int i =0; i< n; i++)
	{
		for(int j=0; j<n; j++)
		{
			if((kiskor->pontjai[i].x == nagykor->pontjai[j].x) && (kiskor->pontjai[i].y == nagykor->pontjai[j].y))
			{
				pontok[0].x = kiskor->pontjai[i].x;
				pontok[0].y = kiskor->pontjai[i].y;
			}
		}
	}

	glColor3f( 1.0, 0.0, 0.0 );
	glBegin( GL_POINTS );
	glVertex2f(pontok[0].x,pontok[0].y);
	glEnd();

	glBegin( GL_LINES );
	glVertex2i(50,50);
	glVertex2i(750,50);
	glEnd();

	glBegin( GL_POINTS );
	glVertex2f(pontok[1].x,pontok[1].y);
	glEnd();

	glColor3f(0.0,0.0,1.0);
	glBegin( GL_POINTS );
	glVertex2f(nagykor->kozepPont.x+nagykor->r,nagykor->kozepPont.y);
	glEnd();
	// Epiciklois

	glColor3f(0.0,0.0,1.0);
	glBegin( GL_LINE_STRIP );
	for (float i=0; i<meddig; i+=0.1)
	{
		x=(nagykor->r+kiskor->r)*cos(i) - kiskor->r*cos(((nagykor->r+kiskor->r)/(float)kiskor->r)*i);
		y=(nagykor->r+kiskor->r)*sin(i) - kiskor->r*sin(((nagykor->r+kiskor->r)/(float)kiskor->r)*i);
		glVertex2f(x+nagykor->kozepPont.x,y+nagykor->kozepPont.y);
		kiskor->kozepPont.x = (nagykor->r+kiskor->r)*cos(i)+nagykor->kozepPont.x;
		kiskor->kozepPont.y = (nagykor->r+kiskor->r)*sin(i)+nagykor->kozepPont.y;
		kiskor->szamol();
	}
	glEnd();

	glutSwapBuffers( );
}

GLfloat dist2( Pont P1, Pont P2 ) {
	GLfloat t1 = P1.x - P2.x;
	GLfloat t2 = P1.y - P2.y;
	return t1 * t1 + t2 * t2;
}

GLint getActivePoint1( Pont *p, GLint size, GLint sens, GLint x, GLint y ) {
	GLint i, s = sens * sens;
	Pont P;
	P.x = x;
	P.y = y;

	for ( i = 0; i < size; i++ )
		if ( dist2( p[i], P ) < s )
			return i;
	return -1;
}

GLint getActivePoint2( Pont *p, GLint size, GLint sens, GLint x, GLint y ) {
	GLint i;
	for ( i = 0; i < size; i++ )
		if ( fabs( (*( p + i )).x - x ) < sens && fabs( (*( p + i )).y - y ) < sens )
			return i;
	return -1;
}

void processMouse( GLint button, GLint action, GLint xMouse , GLint yMouse ) {
	GLint i;
	if ( button == GLUT_LEFT_BUTTON && action == GLUT_DOWN )
		if ( ( i = getActivePoint1( pontok, 2, 8, xMouse, winHeight - yMouse ) ) != -1 )
			dragged = i;
	if ( button == GLUT_LEFT_BUTTON && action == GLUT_UP  )
		dragged = -1;
}

void processMouseActiveMotion( GLint xMouse, GLint yMouse ) {
	GLint i;
	if ( dragged == 0 )
	{
		if( pontok[1].x != 50 )
			return;
		pontok[0].x = xMouse;
		pontok[0].y = yMouse;
		nagykor->r = xMouse - nagykor->kozepPont.x;
		nagykor->szamol();
		kiskor->r = kiskor->kozepPont.x - xMouse;
		kiskor->szamol();
		glutPostRedisplay( );
	}
	else if( dragged > 0 )
	{
		if(xMouse>750)
		{
			pontok[1].x = 750;
			return;
		}
		else if(xMouse<50)
		{
			pontok[1].x=50;
			meddig=0;
			kiskor->kozepPont.x=nagykor->kozepPont.x+nagykor->r+kiskor->r;
			kiskor->kozepPont.y=nagykor->kozepPont.y;
			kiskor->szamol();
			return;
		}
		pontok[1].x = xMouse;
		meddig = ((pontok[1].x-50) / 700)*100*M_PI;
		glutPostRedisplay();
	}
}
void keyboard( unsigned char key, int x, int y ) {
	switch( key ) {
	case 'r':
		meddig = 0;
		kiskor->kozepPont.x=nagykor->kozepPont.x+nagykor->r+kiskor->r;
		kiskor->kozepPont.y=nagykor->kozepPont.y;
		kiskor->szamol();
		pontok[1].x=50;
		break;
	case 't':
		nagykor->r=kiskor->r=100;
		nagykor->szamol();
		kiskor->szamol();
		break;
	case 27:
		exit(1);

	}
	glutPostRedisplay( );
}

int main( int argc, char** argv ) {
	pontok[1].x = 50;
	pontok[1].y = 50;
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize( winWidth, winHeight );
	glutInitWindowPosition( 600,0 );
	glutCreateWindow( "Harmadik házi" );
	init( );
	glutDisplayFunc( display );
	glutMouseFunc( processMouse );
	glutKeyboardFunc( keyboard );
	glutMotionFunc( processMouseActiveMotion );
	glutMainLoop( );
	return 0;
}

