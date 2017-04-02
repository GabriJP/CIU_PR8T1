#include <stdio.h>
#include <cmath>

#include <GL/glew.h>
#include <GL/freeglut.h>

#define WINDOW_ANCHO 500
#define WINDOW_ALTO 500

// Espacio para las variables globales de la ventana
float gl_fovy = 60.0f;
float gl_fovy_min = 10.0f, gl_fovy_max = 90.0f, gl_fovy_paso = 1.0f;
float gl_cerca = 1.0f, gl_lejos = 100.0f;
float gl_center = 7.0f; // centro donde pondremos los objetos


// Espacio para otras variables globales
double theta = M_PI_2, phi = 0.0;
double radio = 7.0;

// Espacio para la declaración de funciones
void InitGlew();

void InitGL();

void Display();

void ReshapeSize(int ancho, int alto);

void PosicionRaton(int x, int y);

void RuedaRaton(int rueda, int sentido, int x, int y);

//main
int main(int argc, char *argv[]) {

    glutInit(&argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WINDOW_ANCHO, WINDOW_ALTO);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("OpenGL Practica ");
    InitGlew(); // despues de crear la primera ventana
    InitGL();
    glutDisplayFunc(Display); // registra la funcion de rendering
    glutReshapeFunc(ReshapeSize);
    glutMotionFunc(PosicionRaton);
    glutMouseWheelFunc(RuedaRaton);

    glutMainLoop(); // bucle principal

    return 0;
}

void InitGlew() {

    // para poder utilizar trasnparentemente todas las extensiones de OpenGL

    GLenum glew_init = glewInit();
    if (glew_init != GLEW_OK) {
        // Problem: glewInit failed, something is seriously wrong.
        fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_init));
    } else
        fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
}

void setViewport() {
    int ancho = glutGet(GLUT_WINDOW_WIDTH);
    int alto = glutGet(GLUT_WINDOW_HEIGHT);
    glViewport(0, 0, ancho, alto); // utiliza la totalidad de la ventana
}

void setProjection() {
    int ancho = glutGet(GLUT_WINDOW_WIDTH);
    int alto = glutGet(GLUT_WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(gl_fovy, (float) ancho / (float) alto, gl_cerca, gl_lejos);
}

void setModelView() {

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void setLight0() {
    float ambient[4] = {0.2, 0.2, 0.2, 1.0};
    float diffuse[4] = {1, 1, 1, 1.0};
    float specular[4] = {1, 1, 1, 1.0};

    float position[4] = {1, 1, 1, 1.0};
    float direction[4] = {-1, -1, -1, 1.0};
    float cut_off = 90.0f;

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

    glLightfv(GL_LIGHT0, GL_POSITION, position);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, direction);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, cut_off);

    glEnable(GL_LIGHT0);
}

void InitGL() {

    glClearColor(0.0, 0.0, 0.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    // para cambiar el criterio de lo que se ve.
    // LESS es el de defecto: se ve lo que tiene menor profundidad
    glDepthFunc(GL_LESS);
    // TO DO

    setViewport();
    setProjection();

    glEnable(GL_LIGHTING);
//    glShadeModel(GL_FLAT);
    glShadeModel(GL_SMOOTH);

    setLight0();
}

// cambio de dimensiones de la ventana, manteniendo la dimension de los objetos
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void ReshapeSize(int ancho, int alto) {
#pragma clang diagnostic pop

    setViewport();
    setProjection();
    glutPostRedisplay();
}

void PosicionRaton(int x, int y) {

    int ancho = glutGet(GLUT_WINDOW_WIDTH);
    int alto = glutGet(GLUT_WINDOW_HEIGHT);

    theta = M_PI * (double) y / (double) alto;
    phi = -M_PI + 2 * M_PI * (double) x / (double) ancho;
    glutPostRedisplay();
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"
void RuedaRaton(int rueda, int sentido, int x, int y) {
#pragma clang diagnostic pop

    gl_fovy -= gl_fovy_paso * (float) sentido;
    if (gl_fovy < gl_fovy_min) gl_fovy = gl_fovy_min;
    if (gl_fovy > gl_fovy_max) gl_fovy = gl_fovy_max;
    setProjection();
    glutPostRedisplay();
}


void setMaterial() {
    const GLfloat ambient[4] = {0.2, 0.2, 0.2, 1.0};
    const GLfloat diffuse[4] = {0.4, 0.4, 0.4, 1.0};
    const GLfloat specular[4] = {0.9, 0.9, 0.9, 1.0};
    const GLfloat shininess = 3.0f;

    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

// función de gestion de ventana
void Display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // borra todo lo existente en el framebuffer
    setModelView();

    // poicionamos el centro de coordenadas
    glTranslatef(0.0f, 0.0f, -gl_center);

    // el eje cenital es el Y
    float x = (float) (radio * sin(theta) * sin(phi));
    float y = (float) (radio * cos(theta));
    float z = (float) (radio * sin(theta) * cos(phi));
    gluLookAt(x, y, z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f); // mira al (0,0,0)

    // TO DO
    setMaterial();
    glutSolidTeapot(2.0f);


    glFlush(); // actualiza el framebuffer
    glutSwapBuffers(); // en caso de animacion
}
