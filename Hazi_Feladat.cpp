#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>

/* KONFIG */
const int n = 4;		//Sokszog szogei
const int m = 40;		//m reszre osztjuk fel az kepernyo oldalat
GLfloat nSokszogPontjai[ n+1 ][ 2 ];
const double _mag = 800;
const double korSugar=200;

#define DEBUG
#define DEBUG_LEVEL 2
/* KONFIG VEGE */

void nSokSzogPontokDef()
{
	float lepes=2*M_PI/n;
	int m=0;
	for(float i=0.0; i<2*M_PI; i+=lepes)
	{
		nSokszogPontjai[m][0]=(_mag/2)+korSugar*cos(i);
		nSokszogPontjai[m][1]=(_mag/2)+korSugar*sin(i);
		m++;
	}
}
void init (void)
{
	glClearColor (0.0, 0.0, 0.0, 0.0); // Set display-window color to white.
	glMatrixMode (GL_PROJECTION); // Set projection parameters.
	gluOrtho2D (0.0, _mag, 0.0, _mag);    // at kell irni a glutInitWindowSize ertekeire (0,x,0,y)
}


void hazi_feladat (void)
{
	glClear (GL_COLOR_BUFFER_BIT);  // Clear display window.
	glColor3f(0.0 , 0.0 , 0.5);     // kek vonal szinek
	glBegin (GL_LINES);				// halo
		for(int i=0; i< _mag; i+=(_mag/m))
		{
			//bal also
			glVertex2i(_mag-i,0);
			glVertex2i(0,i);
			//bal felso
			glVertex2i(0+i,_mag);
			glVertex2i(0,0+i);
			//jobb also
			glVertex2i(0+i,0);
			glVertex2i(_mag,0+i);
			//jobb felso
			glVertex2i(0+i,_mag);
			glVertex2i(_mag,_mag-i);
		}
	glEnd();
	#ifdef DEBUG
	#if DEBUG_LEVEL > 0
		int vonalak = 0;
	#endif
	#endif
	glBegin(GL_LINES);	//sokszog rajzolasa
		for(int i=0; i<n; i++)
			for(int j=i+1; j<n; j++)
			{
				glVertex2fv(nSokszogPontjai[i]);
				glVertex2fv(nSokszogPontjai[j]);
				#ifdef DEBUG
				#if DEBUG_LEVEL > 0
					vonalak++;
				#endif
				#if DEBUG_LEVEL > 1
					std::cout << vonalak << ".\t( " << nSokszogPontjai[i][0] << " , " << nSokszogPontjai[i][1] <<" )   ->   ";
					std::cout << "( " << nSokszogPontjai[j][0] << " , " << nSokszogPontjai[j][1] <<" )\n";
				#endif
				#endif
			}
	glEnd();		//sokszog rajzolas vege
	#ifdef DEBUG
	#if DEBUG_LEVEL > 0
		std::cout <<"Vonalak szama:\t" << vonalak << std::endl;		//Osszesen hany vonalat rajzoltunk
	#endif
	#endif
	glFlush ( ); 
}



int main (int argc, char** argv)
{
	nSokSzogPontokDef();
	glutInit (&argc, argv);                         // Initialize GLUT.
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);   // Set display mode.
	glutInitWindowPosition (600, 0);   // Set top-left display-window position.
	glutInitWindowSize (_mag, _mag);      // Set display-window width and height.
	glutCreateWindow ("Hazi"); // Create display window.
	init ( );                            // Execute initialization procedure.
	glutDisplayFunc (hazi_feladat);       // Send graphics to display window.
	glutMainLoop ( );                    // Display everything and wait.
	return 0;
}