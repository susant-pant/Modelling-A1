/* Susant Pant 10153138
Boilerplate code provided by Troy Alderson
*/

#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <iostream>
#include <math.h>

using namespace std;

#define PI 3.14159
GLFWwindow *window;
int w, h;
double mouseX, mouseY;

float bigRadius = 1.f;				// radius of big circle
float smallRadius = bigRadius/3.f;	// radius of small circle
float globalU = 0.f;				// value of the parameter u
float uSpeed = 0.01f;				// amount by which u increments
int numCycles = 1;					// number of times small circle reaches the original point in the big circle
float rotate = 0.f;					// amount that the image will be rotated by
float zoom = 1.f;					// how much the image is scaled by
bool animated = true;				// whether the hypocycloid is animated or not

void render (float u) {
	glEnable (GL_DEPTH_TEST);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Functions for changing transformation matrix
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity ();
	glTranslatef (0.f, 0.f, 0.f);
	glRotatef (rotate, 0.f, 0.f, 1.f);
	glScalef (zoom, zoom, zoom);

	//Functions for changing projection matrix
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho (-1, 1, -1, 1, -1, 1);
	//gluPerspective (fov, aspect ratio, near plane, far plane)
	//glFrustum

	float x;
	float y;

	// Creates the red hypocycloid
	glBegin (GL_LINE_STRIP);
	for (float i = 0; i <= u; i += 0.001){
		//redraws everything from the start to the current u value of the hypocycloid
		x = (bigRadius - smallRadius)*cos(i) + (smallRadius*cos(((bigRadius - smallRadius) * i)/smallRadius));
		y = (bigRadius - smallRadius)*sin(i) - (smallRadius*sin(((bigRadius - smallRadius) * i)/smallRadius));
		glColor3f (1.f, 0.f, 0.f);
		glVertex2f (x,y);
	}
	glEnd ();

	// Creates the big circle
	glBegin (GL_LINE_STRIP);
	for (float i = 0; i <= 2*PI; i += 0.001){
		x = bigRadius * cos(i);
		y = bigRadius * sin(i);
		glColor3f (1.f, 1.f, 1.f);
		glVertex2f (x,y);
	}
	glEnd ();

	//Creates the small circle, which moves as u increments
	glBegin (GL_LINE_STRIP);
	for (float i = 0; i <= 2*PI; i += 0.001){
		x = smallRadius * cos(i) + ((bigRadius - smallRadius) * cos(u));
		y = smallRadius * sin(i) + ((bigRadius - smallRadius) * sin(u));
		glColor3f (1.f, 1.f, 1.f);
		glVertex2f (x,y);
	}
	glEnd ();

	// Creates the line from the center of the small circle to the point where the hypocycloid is drawn
	glBegin (GL_LINE_STRIP);
	glColor3f (1.f, 1.f, 1.f);
	glVertex2f ((bigRadius - smallRadius) * cos(u), (bigRadius - smallRadius) * sin(u));
	glColor3f (1.f, 1.f, 1.f);
	glVertex2f (
		(bigRadius - smallRadius)*cos(u) + (smallRadius*cos(((bigRadius - smallRadius) * u)/smallRadius)),
		(bigRadius - smallRadius)*sin(u) - (smallRadius*sin(((bigRadius - smallRadius) * u)/smallRadius)));
	glEnd();
}

void keyboard (GLFWwindow *sender, int key, int scancode, int action, int mods) {
	// Changes the speed of the small circle's rotation, limited between 0.1 and 0.01
	if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT))
		if (uSpeed < 0.1f)
			uSpeed += 0.01f;
	if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT))
		if (uSpeed > 0.01f)
			uSpeed -= 0.01f;

	// Changes the radius of the big circle, limited between 1 and 0.1
	if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
		if (bigRadius > 0.1f)
			bigRadius -= 0.01f;
	if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
		if (bigRadius < 1.f)
			bigRadius += 0.01f;

	// Changes the radius of the small circle, limited between 1/3 and 0.1/3
	if (key == GLFW_KEY_LEFT && (action == GLFW_PRESS || action == GLFW_REPEAT))
		if (smallRadius > 0.1f/3.f)
			smallRadius -= 0.01f/3.f;
	if (key == GLFW_KEY_RIGHT && (action == GLFW_PRESS || action == GLFW_REPEAT))
		if (smallRadius < 1.f/3.f)
			smallRadius += 0.01f/3.f;

	// Changes the number of times the small circle goes around the big circle, limited between 10 and 1
	if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
		if (numCycles < 10)
			numCycles++;
	if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
		if (numCycles > 1)
			numCycles--;

	// Changes the rotation of the hypocycloid, limited between 0 and 360 degrees
	if (key == GLFW_KEY_C && (action == GLFW_PRESS || action == GLFW_REPEAT))
		if (rotate < 360.f)
			rotate++;
	if (key == GLFW_KEY_V && (action == GLFW_PRESS || action == GLFW_REPEAT))
		if (rotate > 0.f)
			rotate--;

	// Changes the scale of the hypocycloid, limited between 1 and 0.25
	if (key == GLFW_KEY_Z && (action == GLFW_PRESS || action == GLFW_REPEAT))
		if (zoom < 1.f)
			zoom += 0.01f;
	if (key == GLFW_KEY_X && (action == GLFW_PRESS || action == GLFW_REPEAT))
		if (zoom > 0.25f)
			zoom -= 0.01f;

	// Pauses the animatioon
	if (key == GLFW_KEY_SPACE && (action == GLFW_PRESS))
		animated = !animated;

	// Closes the window
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int main () {
	if (!glfwInit())
		return 1;

	window = glfwCreateWindow (640, 640, "Susant Pant A1", NULL, NULL);
	if (!window)
		return 1;

	glfwMakeContextCurrent (window);
	glfwSetKeyCallback (window, keyboard);
	while (!glfwWindowShouldClose (window)) {
		glfwGetFramebufferSize (window, &w, &h);
		glViewport (0, 0, w, h);

		render (globalU);

		// Resets u value to 0 and gets the animation to start from scratch
		if (animated){
			globalU += uSpeed;
			if (globalU > numCycles*2*PI)
				globalU = 0.f;
		}
		// Else displays the whole hypocycloid
		else
			globalU = numCycles*2*PI;

		glfwSwapBuffers (window);
		glfwPollEvents();
	}

	glfwDestroyWindow (window);
	glfwTerminate();
	return 0;
}