#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <complex>

#define magassag 300
#define szelesseg 400

struct Pont {
	float x,y;
};
class Vonal {
private:
	Pont * egyenes;
	int n;
	int akt;
	Pont iranyVektor;
	float hossz;
	char nev;
public:
	Vonal(char );
	void fel();
	void le();
	void rajzol();
	float cErtek;
	Pont normalVektor;
	Pont p1,p2;

};
Vonal::Vonal(char _nev) 
{
	nev = _nev;
	n=6;
	egyenes = new Pont[n+1];
	hossz=(magassag+szelesseg)/n;
	if(nev == 'b')
	{
		int i = 0;
		for(float m=szelesseg/2; m > 0; m-=hossz)
		{
			egyenes[i].x=m;
			egyenes[i].y=1;
			i++;
		}

		double a,b,c;
		c=hossz;
		a=egyenes[i-1].x;
		b=sqrt(c*c-a*a);
		egyenes[i].x=1;
		egyenes[i].y=b;
		i++;
		for(float m=b+hossz; m < magassag; m+=hossz)
		{
			egyenes[i].x=1;
			egyenes[i].y=m;
			i++;
		}
		a=magassag-egyenes[i-1].y;
		b=sqrt(c*c-a*a);
		egyenes[i].x=b;
		egyenes[i].y=magassag-1;
		i++;
		for(float m=b+hossz; m < szelesseg/2; m+=hossz)
		{
			egyenes[i].x=m;
			egyenes[i].y=magassag-1;
			i++;
		}
		egyenes[i].x=szelesseg/2;
		egyenes[i].y=magassag-1;
	}
	else if(nev == 'j')
	{
		int i = 0;
		for(float m=szelesseg/2; m < szelesseg; m+=hossz)
		{
			egyenes[i].x=m;
			egyenes[i].y=1;
			i++;
		}
		double a,b,c;
		c=hossz;
		a=szelesseg-egyenes[i-1].x;
		b=sqrt(c*c-a*a);
		egyenes[i].x=szelesseg-1;
		egyenes[i].y=b;
		i++;
		for(float m=b+hossz; m < magassag; m+=hossz)
		{
			egyenes[i].x=szelesseg-1;
			egyenes[i].y=m;
			i++;
		}
		a=magassag-egyenes[i-1].y;
		b=sqrt(c*c-a*a);
		egyenes[i].x=szelesseg-b-1;
		egyenes[i].y=magassag-1;
		i++;
		for(float m=szelesseg-b-hossz; m > szelesseg/2; m-=hossz)
		{
			egyenes[i].x=m;
			egyenes[i].y=magassag-1;
			i++;
		}
		egyenes[i].x=szelesseg/2;
		egyenes[i].y=magassag-1;
	}
	srand(time(NULL));
	akt = rand()%n;
	p1.x=egyenes[akt].x;
	p1.y=egyenes[akt].y;
	p2.x=egyenes[akt+1].x;
	p2.y=egyenes[akt+1].y;
	iranyVektor.x=p2.x-p1.x;
	iranyVektor.y=p2.y-p1.y;
	if(nev=='b')
	{
		normalVektor.x=iranyVektor.y;
		normalVektor.y=-iranyVektor.x;
	}
	else if(nev=='j')
	{
		normalVektor.x=-iranyVektor.y;
		normalVektor.y=iranyVektor.x;
	}
	std::cout <<nev <<  ":\t(" << normalVektor.x <<","<< normalVektor.y << ")" << std::endl;
}
void Vonal::fel()
{	
	if(akt>=n)
		return;
	p1.x=egyenes[akt].x;
	p1.y=egyenes[akt].y;
	p2.x=egyenes[akt+1].x;
	p2.y=egyenes[akt+1].y;
	++akt;
	iranyVektor.x=p2.x-p1.x;
	iranyVektor.y=p2.y-p2.y;
	if(nev=='b')
	{
		normalVektor.x=iranyVektor.y;
		normalVektor.y=-iranyVektor.x;
	}
	else if(nev=='j')
	{
		normalVektor.x=-iranyVektor.y;
		normalVektor.y=iranyVektor.x;
	}
}
void Vonal::le()
{	
	if(akt<=0)
		return;
	p1.x=egyenes[akt-1].x;
	p1.y=egyenes[akt-1].y;
	p2.x=egyenes[akt].x;
	p2.y=egyenes[akt].y;
	--akt;
	iranyVektor.x=p2.x-p1.x;
	iranyVektor.y=p2.y-p2.y;
	if(nev=='b')
	{
		normalVektor.x=iranyVektor.y;
		normalVektor.y=-iranyVektor.x;
	}
	else if(nev=='j')
	{
		normalVektor.x=-iranyVektor.y;
		normalVektor.y=iranyVektor.x;
	}
}
void Vonal::rajzol()
{
	glColor3f(1.0, 0.0, 0.0);
	glVertex2i( p1.x, p1.y );
	glVertex2i( p2.x, p2.y );
}
Vonal * bal = new Vonal('b');
Vonal * jobb = new Vonal('j');
float pontEgyenesTavolsag(Pont a, Pont b, Pont c)
{
	float xTavolsag= b.x-a.x;
	float yTavolsag= b.y-a.y;
	if( (xTavolsag == 0) && (yTavolsag == 0) )	//Megegyezik az egyenes ket vegpontja
	{
		xTavolsag = c.x - a.x;
		yTavolsag = c.y - a.y;
		return sqrt(xTavolsag * xTavolsag + yTavolsag * yTavolsag);
	}
	float t = ((c.x - a.x) * xTavolsag + (c.y - a.y) * yTavolsag) / (xTavolsag * xTavolsag + yTavolsag * yTavolsag);

    if (t < 0)
    {
        //point is nearest to the first point i.e x1 and y1
        xTavolsag = c.x - a.x;
        yTavolsag = c.y - a.y;
    }
    else if (t > 1)
    {
        //point is nearest to the end point i.e x2 and y2
        xTavolsag = c.x - b.x;
        yTavolsag = c.y - b.y;
    }
    else
    {
        //if perpendicular line intersect the line segment.
        xTavolsag = c.x - (a.x + t * xTavolsag);
        yTavolsag = c.y - (a.y + t * yTavolsag);
    }

    //returning shortest distance
    return sqrt(xTavolsag * xTavolsag + yTavolsag * yTavolsag);
}
class Labda {
private:
	GLfloat pontok[ 25 ][ 2 ];
	Pont iranyVektor;
	Pont kozepPont;
	int sugar;
public:
	Labda();
	void rajzol();
	void mozgat(int);
};
Labda::Labda()
{
	float lepes=2*M_PI/25;
	sugar = 5;
	int m=0;
	this->kozepPont.x=szelesseg/2;
	this->kozepPont.y=magassag/2;
	for(float i=0.0; i<2*M_PI; i+=lepes)
	{
		pontok[m][0]=kozepPont.x+sugar*cos(i);
		pontok[m][1]=kozepPont.y+sugar*sin(i);
		m++;
	}
	iranyVektor.x=rand()%3;
	iranyVektor.y=rand()%4;
}
void Labda::rajzol()
{
	for (int i = 0; i < 25; i++ )
		glVertex2fv ( pontok[ i ] );
}
void Labda::mozgat(int j)
{
	if ( kozepPont.x+sugar > szelesseg || kozepPont.x-sugar < 0 || kozepPont.y+sugar > magassag || kozepPont.y-sugar < 0 )
		return;
	
	float balTav = pontEgyenesTavolsag(bal->p1, bal->p2, kozepPont);
	float jobbTav = pontEgyenesTavolsag(jobb->p1, jobb->p2, kozepPont);
	if( balTav <= sugar )
	{
		float normAbsz = sqrt(bal->normalVektor.x*bal->normalVektor.x + bal->normalVektor.y*bal->normalVektor.y);
		float labdAbsz = sqrt(iranyVektor.x*iranyVektor.x + iranyVektor.y*iranyVektor.y);

	}
	else
	{
		for(int i=0; i< 25; i++)
		{
			this->pontok[i][0]+=iranyVektor.x;
			this->pontok[i][1]+=iranyVektor.y;
		}
		kozepPont.x+=iranyVektor.x;
		kozepPont.y+=iranyVektor.y;
	}
}



Labda * labda = new Labda();

void init( ) {
	glClearColor ( 1.0, 1.0, 1.0, 0.0 );
	glMatrixMode ( GL_PROJECTION );
	gluOrtho2D ( 0.0, szelesseg, 0.0, magassag );
	glShadeModel ( GL_FLAT );
}

void display( )
{
	glClear(GL_COLOR_BUFFER_BIT);
	

	glBegin( GL_LINES );
	bal->rajzol();
	jobb->rajzol();
	glColor3f(0.0, 0.0, 0.0);
	glVertex2i(szelesseg/2,0);
	glVertex2i(szelesseg/2,magassag);
	glEnd( );
	glColor3f(0.0, 0.0, 0.0);
	glBegin( GL_LINE_LOOP );
	labda->rajzol();
	glEnd();

	glutSwapBuffers( );
}

void update( int n )
{
	labda->mozgat(1);
	glutPostRedisplay( );
	glutTimerFunc( 1, update, 0 );
}

void keyboard( unsigned char key, int x, int y ) {
	switch( key ) {
	case 27:
		exit( 0 );
		break;
	case 'w':
		bal->fel();
		break;
	case 's':
		bal->le();
		break;
	case 'i':
		jobb->fel();
		break;
	case 'k':
		jobb->le();
		break;
	}
	glutPostRedisplay( );
}

int main(int argc, char** argv)
{
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB );
	glutInitWindowSize( szelesseg, magassag );
	glutInitWindowPosition ( 200, 100 );
	glutCreateWindow( "Hazi..." );
	init( );
	glutDisplayFunc( display );
	glutKeyboardFunc( keyboard );
	glutTimerFunc( 1, update, 0 );
	glutMainLoop( );
	return 0;
}
