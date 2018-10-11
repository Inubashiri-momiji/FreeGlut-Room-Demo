#include <windows.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include <GL/freeglut.h>
#include <GL/freeglut_ext.h>
#include <stdio.h>
#include <math.h>
#include "TGATextura.h" //Este include administra los archivos TGA
#define TEXTURE_FLOOR 0
#define TEXTURE_STAR 1
#define TEXTURE_PHOTO 2
#define TEXTURE_BALL 3
#define PI 3.14159265
char direccion[90]; //comandos
GLfloat desplazamiento[90]; //comandos value
GLfloat LIGHT_POS = 1.4f; //The length of each side of the cube
GLfloat BOX_HEIGHT = 7.0f; //The height of the box off of the ground
GLfloat FLOOR_SIZE = 25.0f; //The length of each side of the floor

GLfloat Zoom = 1.8;
GLfloat Luz = 0.51f;
GLfloat LuzHeli = 0.50f;
GLfloat Transparencia = 230; // reflejo del piso
GLfloat Rotacion = 0;
GLfloat RotacionEscenario = 0;
GLfloat RotacionHeli = 0;
GLfloat BallRot = 0;
GLfloat BallSpeed = 1;
GLfloat cupTopDepth = 35;
GLfloat margin = cupTopDepth * .275;
GLfloat inclination = 30;
GLfloat cerca = 0;
GLfloat xball = 0;
GLfloat yball = 0;
GLfloat zball = 0;
GLfloat spinX = 0;
GLfloat spinY = 0;
GLfloat BallRadians = 0;
GLfloat movX = 0;
GLfloat movY = 0;
GLfloat movZ = 0;
GLfloat rotX = 0;
GLfloat rotY = 0;
GLfloat rotZ = 0;
GLfloat distanciaBala = 0;
GLfloat incvelocidad = .05;
GLfloat mdesplazamiento = 0;
GLfloat impulse = 0;
GLint MaxPunt = 0;
GLint edge = 80;
GLint Meridianos = 0;
GLint ballControl = 2;
GLint ballJumpHeight = 4;
GLint k = 0;
GLint planCount = 0;
GLint planInstructionCount = 0;
bool planvuelo = false; //auto
bool laserControl = false;
bool faiaFaiaPewPewPewPew = false;
bool Rot = false;
bool spinHeli = false;
bool ImpulseSpinHeli = false;
bool isBallJump = false;
bool isTrompoSpinning = false;

void handleKeypress(unsigned char key, int x, int y) {
  printf("%c",key);
  switch (key) {
    case 27: //Escape key
         exit(0);
    break;
    case 'Z':
        movZ += movZ > 2200 ? 0: 0.5; // se mueve hacia afuera
    break;
    case 'z':
        movZ -= movZ > 2200 ? 0: 0.5; // se mueve hacia adentro
    break;
    case 'w':
        movY += movY > 2200 ? 0: 0.5; // se mueve hacia arriba
    break;
    case 's':
        movY -= movY < -2000 ? 0: 0.5; // se mueve hacia abajo
    break;
    case 'a':
        movX -= movX < -2200 ? 0: 0.5; // Se mueve a la izquierda
    break;
    case 'd':
        movX += movX > 2200 ? 0: 0.5; // se mueve  a la derecha
    break;

    case '(':
        rotZ += rotZ > 360 ? 0: 1;  // rota hacia abajo
    break;
    case '"':
        rotZ -= rotZ > 360 ? 0: 1; // rota hacia arriba
    break;
    case '$':
        rotY += rotY > 360 ? 0: 1;  //rota de lado
    break;
    case '&':
        rotY -= rotY < -360 ? 0: 1; //rota de lado
    break;
    case ')':
        rotX -= rotX < -360 ? 0: 1; //rota de cabeza
    break;
    case '!':
        rotX += rotX > 360 ? 0: 1; // rota de cabeza
    break;


    case 'F': //Posición de la luz
        LIGHT_POS += .2;
    break;

    case 'f': //Posición de la luz
        LIGHT_POS -= .2;
    break;

    case 'n': //Acciona el laser del helicoptero
        laserControl = !laserControl;
    break;

    case 'm': //Acciona los disparos del helicoptero
        faiaFaiaPewPewPewPew = !faiaFaiaPewPewPewPew;
    break;

    case 'r': //La esfera gira
         Rot = !Rot;
    break;

    case 'l': //Luz
         Luz >= 0.0 ? Luz -= 0.003 : Luz;
    break;
    case 'L': //Luz
         Luz <= 0.8 ? Luz += 0.003 : Luz;
    break;
    case '-':
        if (Zoom > 0) Zoom -= 0.03;
    break;

    case '+':
        if (Zoom < 10) Zoom += 0.03;
    break;

    case 't': //Sube
         if (++Transparencia > 255) Transparencia = 0;
    break;
    case 'T': //Sube
         if (--Transparencia <= 0) Transparencia = 255;
    break;
    case 'A': //automagico
         planvuelo = !planvuelo;
         planvuelo ? printf("\nModo automagico activado\n") : printf("\nModo automagico desactivado\n");
         planvuelo ? glEnable(GL_LIGHT1) : glDisable(GL_LIGHT1);
         planCount = planInstructionCount = 0;

    break;

    case 'e': //mueve las helices del helicoptero
         spinHeli = !spinHeli;
         if (spinHeli)
            ImpulseSpinHeli = !ImpulseSpinHeli;
    break;
    case 'b': //mueve las helices del helicoptero
         isBallJump = true;
    break;
    case 'v': //mueve las helices del helicoptero
         isTrompoSpinning = true;
    break;
  }
}

void DibujaModeloPersona(float x, float y, float z, float scale){
    GLUquadricObj *Cabina;
    Cabina = gluNewQuadric();
    gluQuadricDrawStyle(Cabina, GLU_FILL);
    gluQuadricNormals(Cabina, GLU_NONE);
    glPushMatrix();
            glTranslatef(x,y,z);
            glTranslatef(0,0,5);
            glScalef(.5,.5,.5);
            glScalef(scale,scale,scale);
            glRotatef(90,1,0,0);
            glRotatef(-90,0,0,1);
            glColor4ub(203, 175, 135, 0);
            glPushMatrix(); //Cabeza
                glScalef(1, 1, 1.15);
                glutSolidSphere(1, edge,30);
            glPopMatrix();
            glTranslatef(0,0,1.5);
            glPushMatrix(); //cuello
                glColor4ub(206,169,137,0);
                glScalef(1, 1, 1.06);
                glutSolidSphere(.7, edge,30);
            glPopMatrix();
            glColor4ub(203, 175, 135, 0);
            glTranslatef(0,0,.5);
            glPushMatrix(); //Pecho
                gluCylinder(Cabina, 1.3, .9, 2.5, 30, 30);
                gluDisk(Cabina,0,1.3,edge,40);
                glTranslatef(0,0,2.5);
                gluDisk(Cabina,0,.9,edge,40);
            glPopMatrix();
            glTranslatef(0,0,.6);
            glPushMatrix(); //brazo izquierdo
                glTranslatef(-1.5,0,0);
                glScalef(1, 1, 1.06);
                glColor4ub(206,169,137,0);
                glutSolidSphere(.5, edge,30); //articulacion brazo
                glRotatef(10,0,-1,0);
                glTranslatef(0,0,.45);
                glColor4ub(203, 175, 135, 0);
                gluCylinder(Cabina, .4, .3, 1.6, edge, 30); //brazo
                glTranslatef(0,0,1.8);
                glColor4ub(206,169,137,0);
                glutSolidSphere(.3, edge, 50); //articulacion codo
                glTranslatef(0,0,.3);
                glColor4ub(203, 175, 135, 0);
                glRotated(80,1,0,0);
                gluCylinder(Cabina, .3, .15, 1.9, edge, 30); //brazo
                glTranslatef(-.2,0,2);
                glRotated(75,0,1,0);
                glScalef(1,.5,.5);
                glutSolidSphere(1, 10, 2); //mano
            glPopMatrix();
            glPushMatrix(); //brazo derecho
                glRotated(180,0,0,1);
                glTranslatef(-1.5,0,0);
                glScalef(1, 1, 1.06);
                glColor4ub(206,169,137,0);
                glutSolidSphere(.5, edge,30); //articulacion brazo
                glRotatef(10,0,-1,0);
                glTranslatef(0,0,.45);
                glColor4ub(203, 175, 135, 0);
                gluCylinder(Cabina, .4, .3, 1.6, edge, 30); //brazo
                glTranslatef(0,0,1.8);
                glColor4ub(206,169,137,0);
                glutSolidSphere(.3, edge, 50); //articulacion codo
                glTranslatef(0,0,.3);
                glColor4ub(203, 175, 135, 0);
                gluCylinder(Cabina, .3, .15, 1.9, edge, 30); //brazo
                glTranslatef(-.2,0,2);
                glRotated(75,0,1,0);
                glScalef(1,.5,.5);
                glutSolidSphere(1, 10, 2); //mano
            glPopMatrix();
            glTranslatef(0,0,2.2);
            glPushMatrix(); //abdomen
                glColor4ub(206,169,137,0);
                glScalef(1.2, 1, 1.06);
                glutSolidSphere(.7, edge,30);
            glPopMatrix();
            glColor4ub(203, 175, 135, 0);
            glTranslatef(0,0,.55);
            glPushMatrix(); //cintura
                gluCylinder(Cabina, .7, 1.2, 1.3, 30, 30);
                gluDisk(Cabina,0,.7,edge,40);
                glTranslatef(0,0,1.3);
                gluDisk(Cabina,0,1.2,edge,40);
            glPopMatrix();
            glTranslatef(0,0,1.6);
            glPushMatrix(); //pierna izquierdo
                glTranslatef(-.62,0,0);
                glColor4ub(206,169,137,0);
                glutSolidSphere(.5, edge,30); //articulacion pierna
                glTranslatef(0,0,.42);
                glColor4ub(203, 175, 135, 0);
                gluCylinder(Cabina, .5, .4, 3, edge, 30); //pierna
                glTranslatef(0,0,3.2);
                glColor4ub(206,169,137,0);
                glutSolidSphere(.35, edge, 50); //articulacion pierna
                glTranslatef(0,0,.32);
                glColor4ub(203, 175, 135, 0);
                gluCylinder(Cabina, .3, .15, 2.5, edge, 30); //pierna
                glTranslatef(0,0,2.55);
                glColor4ub(206,169,137,0);
                glutSolidSphere(.13, edge, 40); //articulación pie
                glTranslatef(0,0,.47);
                glScalef(.5, .5, .7);
                glColor4ub(203, 175, 135, 0);
                glutSolidSphere(.6, 10, 4); //pie
            glPopMatrix();
            glPushMatrix(); //pierna derecho
                glRotated(180,0,0,1);
                glTranslatef(-.62,0,0);
                glColor4ub(206,169,137,0);
                glutSolidSphere(.5, edge,30); //articulacion pierna
                glTranslatef(0,0,.42);
                glColor4ub(203, 175, 135, 0);
                gluCylinder(Cabina, .5, .4, 3, edge, 30); //pierna
                glTranslatef(0,0,3.2);
                glColor4ub(206,169,137,0);
                glutSolidSphere(.35, edge, 50); //articulacion pierna
                glTranslatef(0,0,.32);
                glColor4ub(203, 175, 135, 0);
                gluCylinder(Cabina, .3, .15, 2.5, edge, 30); //pierna
                glTranslatef(0,0,2.55);
                glColor4ub(206,169,137,0);
                glutSolidSphere(.13, edge, 40); //articulación pie
                glTranslatef(0,0,.47);
                glScalef(.5, .5, .7);
                glColor4ub(203, 175, 135, 0);
                glutSolidSphere(.6, 10, 4); //pie
            glPopMatrix();

    glPopMatrix();
    gluDeleteQuadric(Cabina);
}

void DibujaBombillo(float x, float y, float z, float scale) {
    GLUquadricObj *Cabina;
    Cabina = gluNewQuadric();
    gluQuadricDrawStyle(Cabina, GLU_FILL);
    glPushMatrix();
        glTranslatef(x, y, z);
        glTranslatef(0, LIGHT_POS, 0);
        glScalef(scale, scale, scale);

        //Cable lampara
        glPushMatrix();
            glColor4ub(100, 100, 100, 0);
            glRotatef(-90, 1,0,0);
            glTranslatef(0,0,1.5);
            gluCylinder(Cabina, .05, .05, 1, 40, 3);

            glColor4ub(0, 0, 150, 0);
            glTranslatef(0,0.02,0);
            glutSolidCylinder(.025, 1.2, 40, 3);

            glColor4ub(78, 59, 49, 0);
            glTranslatef(0,-0.04,0);
            glutSolidCylinder(.025, 1.2, 40, 3);
        glPopMatrix();

        //Lampara
        glPushMatrix();
            glColor4ub(100, 100, 100, 0);
            glRotatef(-90, 1,0,0);
            gluCylinder(Cabina, .7, .05, 1.6, 40, 3);
            glTranslatef(0,0,-.2);
            gluCylinder(Cabina, .7, .7, .2, 40, 3);
        glPopMatrix();
        //bombillo
        glPushMatrix();
            glColor4ub(255, 255, 0, 0);
            glutSolidSphere(0.5, 50, 50);
        glPopMatrix();
    glPopMatrix();
    gluDeleteQuadric(Cabina);
}

void DibujaBola(float x, float y, float z, float scale) {
    glPushMatrix();
        glScalef(scale,scale,scale);
        glTranslatef(x, y, z);
        glTranslatef(0, .7, 5);
        glRotatef(90, 1, 0, 0);
        glColor4ub(255, 255, 255, 0);
        GLUquadricObj *sphere=NULL;
        sphere = gluNewQuadric();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,texturas[TEXTURE_BALL].ID);
        gluQuadricDrawStyle(sphere, GLU_FILL);
        gluQuadricTexture(sphere, TRUE);
        gluQuadricNormals(sphere, GLU_NONE);

        glTranslatef(-7, -6.3, 7); //Posiciona en el suelo
        glTranslatef(xball, yball, zball); //mueve la bola
        if(ballControl == 2)
            glRotatef(BallRot, 1, 0, 0);
        //glutSolidSphere(.7,50,50);
        gluSphere(sphere, .7,50,50);
        glDisable(GL_TEXTURE_2D);
        gluDeleteQuadric(sphere);
    glPopMatrix();

}

void DibujaHelicoptero(float x, float y, float z, float scale){
    GLUquadricObj *Cabina;
    Cabina = gluNewQuadric();
    gluQuadricDrawStyle(Cabina, GLU_FILL);
    gluQuadricNormals( Cabina, GLU_NONE); // For if lighting is to be used.
    glPushMatrix();

    float multiSpeed =  40 * impulse ;
    if(planvuelo){
        try{
            if(planCount == desplazamiento[planInstructionCount]){ //Ya el conteo llegó al final, se avanza a la siguiente instrucción
                planInstructionCount++;
                planCount = 0;
            }else{
                unsigned char letter = direccion[planInstructionCount];
                if (letter == NULL){
                    planvuelo = !planvuelo;
                    planCount = planInstructionCount = 0;
                    printf("\n\nFinalizo el modo automagico\n");
                    glDisable(GL_LIGHT1);
                }
                if(!ImpulseSpinHeli){
                    handleKeypress(letter,0,0);
                    planCount++;
                }
            }
        }catch(...){
        printf("Entro al catch");
        }
    }
    glTranslatef(0,-6.35,0);
    glTranslatef(x, y, z);

    glScalef(scale,scale,scale);
    glTranslatef(movX, movY, movZ);
    glRotatef(rotX, 1, 0,0);
    glRotatef(rotY, 0, 1,0);
    glRotatef(rotZ, 0, 0,1);

    GLfloat lmodel_ambient_heli[] = { LuzHeli, LuzHeli, LuzHeli, LuzHeli };
    GLfloat lightColor_heli[] = {1.0f, 1.0f, 1.0f, 1.0f}; //Amarillo
    GLfloat lightPos_heli[] = {movX+x, movY+y, movZ+y, 1};
    GLfloat spot_direction_heli[] = { 0, -1, 0 };
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor_heli);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos_heli);
    glLightfv(GL_LIGHT1, GL_AMBIENT, lmodel_ambient_heli);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 25.0);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction_heli);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);
        //========================================Dibuja esfera
        glPushMatrix();
            glColor4ub(0, 0, 255, 0);   //Azul
            glScalef(1.3, 1, 1);
            glutSolidSphere(20,50,10);
        glPopMatrix();

        //========================================Dibuja helices

        glColor4ub(210, 210, 210, 0);   //Verde
        glPushMatrix();
            glTranslatef(0, 20, 0);
            glPushMatrix();
                //hacer una variable global auxiliar que toma la última rotación de que tuvo, esto para ir disminuyendolo con el multiplicador cuando ya terminó el plan de vuelo
                glRotatef(RotacionHeli*multiSpeed, 0, -1, 0);
                glScalef(60, 1, 5);
                glutSolidCube (1.0);
            glPopMatrix();
            glPushMatrix();
                glRotatef(RotacionHeli*multiSpeed, 0, -1, 0);
                glScalef(5, 1, 60);
                glutSolidCube (1.0);
            glPopMatrix();

        //========================================Dibuja la base helice
            glPushMatrix();
               glColor4ub(255, 255,255, 0);
                glTranslatef(0, 0, 0); //x, y, z
                glScalef(1.6, 1.6, 1.6);
                glutSolidSphere(2,50,10);
            glPopMatrix();
        glPopMatrix();

            //========================================Dibuja helice trasera

        glColor4ub(210, 210, 210, 0);   //Verde
        glPushMatrix();
            glTranslatef(60, 0, 6);
            glRotatef(90, 0, 1, 0);
            glPushMatrix();
                glRotatef(RotacionHeli*multiSpeed, 1, 0, 0);
                glScalef(1, 5, 60);
                glutSolidCube (.5);
            glPopMatrix();
            glPushMatrix();
                glRotatef(RotacionHeli*multiSpeed, 1, 0, 0);
                glScalef(1, 60, 5);
                glutSolidCube (.5);
            glPopMatrix();
        glPopMatrix();


        //========================================Dibuja la base de la helice trasera
            glPushMatrix();
                glTranslatef(62.5, 0, 0); //x, y, z
                glColor4ub(255, 0, 0, 0);
                glRotatef(-20, 0, 1, 0);
                glScalef(1, 1, 1);
                gluCylinder(Cabina, 2, .1, 7.70, 20, 1);
            glPopMatrix();

        //======================================== Dibuja alas de la helice

        glPushMatrix();
            glPushMatrix();
                glTranslatef(67, 15, -5); //x, y, z
                glRotatef(90, 1, 0, 0);
                glRotatef(-20, 1, 1, 0);
                glScalef(1, .3, 1);
                gluCylinder(Cabina, 1, 3, 15, 20, 1);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(63, -12, -8); //x, y, z
                glRotatef(90, 1, 0, 0);
                glRotatef(-150, 1, 0, 0);
                glScalef(1, .3, 1);
                gluCylinder(Cabina, 1, 3, 15, 20, 1);
            glPopMatrix();
            //======================================== Dibuja final de las alas de  helice
            glPushMatrix();
                glTranslatef(65, 0, 0); //x, y, z
                glScalef(2, 1, 1);
                glutSolidSphere(2,50,10);
            glPopMatrix();
        glPopMatrix();


        //======================================== Dibuja la pieza hacia la hélice trasera


        gluQuadricDrawStyle(Cabina, GLU_FILL);
        glPushMatrix();
            glTranslatef(20, 0, 0); //x, y, z
            glRotatef(90, 0, 1, 0);
            glColor4ub(0, 0, 255, 0);   //Azul
            gluCylinder(Cabina, 6, 2, 46, 50, 1);
        glPopMatrix();

        //======================================== Dibuja las patas
        glPushMatrix();
            glColor4ub(245, 245, 245, 0);
            glTranslatef(-20, -30, -15); //x, y, z
            glPushMatrix();
                glRotatef(90, 0, 1, 0);
                gluCylinder(Cabina, 1.4, 1.4, 45, 40, 1);
            glPopMatrix();

            glPushMatrix();
                glTranslatef(15, 00, 0); //x, y, z
                glRotatef(45, -1, 0, 1);
                glRotatef(100, 0, .3, 1);
                gluCylinder(Cabina, 1.4, 1.4, 20, 40, 1);

            glPopMatrix();

            glPushMatrix();
                glTranslatef(30, 00, 0); //x, y, z
                glRotatef(45, -1, 0, 1);
                glRotatef(100, 0, .3, 1);
                gluCylinder(Cabina, 1.4, 1.4, 20, 40, 1);
            glPopMatrix();

            //luces

            glPushMatrix();
                glColor4ub(255, 0, 0, 0);
                glTranslatef(45, 00, 0); //x, y, z
                glScalef(1, .7, .7);
                glutSolidSphere(2,50,10);

            glPopMatrix();

            glPushMatrix();
                glColor4ub(255, 0, 0, 0);
                glScalef(1, .7, .7);
                glutSolidSphere(2,50,10);
                //Bala
                if(faiaFaiaPewPewPewPew){
                    distanciaBala += 0.9f;
                    if (distanciaBala >= 88.0f)
                        distanciaBala = 0.0f;
                    glTranslatef(-distanciaBala,0,0);
                    glColor4ub(255, 0, 0, 0);
                    glScalef(1, .7, .7);
                    glutSolidSphere(2,50,10);
                }
            glPopMatrix();

            glPushMatrix();
                if (laserControl) {

                    glBegin(GL_LINES);
                        glColor4ub(255, 31, 31, 0);
                        glVertex3f( -350.0f, 0.0f, -0.0f ); glVertex3f( 0.0f, 0.0f, -0.0f ); //Línea Norte a Sur
                        glVertex3f( -350.0f, 0.0f,  30.0f ); glVertex3f( 0.0f, 0.0f,  30.0f ); //Línea Norte a Sur
                    glEnd ();
                }
            glPopMatrix();
        glPopMatrix();

        //pata 2
        glPushMatrix();
            glColor4ub(245, 245, 245, 0);
            glTranslatef(-20, -30, 15); //x, y, z
            glRotatef(290, 1, 0, 0);
            glPushMatrix();
                glRotatef(90, 0, 1, 0);
                gluCylinder(Cabina, 1.4, 1.4, 45, 40, 1);
            glPopMatrix();

            glPushMatrix();
                glTranslatef(15, 00, 0); //x, y, z
                glRotatef(45, -1, 0, 1);
                glRotatef(100, 0, .3, 1);
                gluCylinder(Cabina, 1.4, 1.4, 20, 40, 1);

            glPopMatrix();

            glPushMatrix();
                glTranslatef(30, 00, 0); //x, y, z
                glRotatef(45, -1, 0, 1);
                glRotatef(100, 0, .3, 1);
                gluCylinder(Cabina, 1.4, 1.4, 20, 40, 1);
            glPopMatrix();

        //luces
            glPushMatrix();
                glColor4ub(255, 0, 0, 0);
                glTranslatef(45, 00, 0); //x, y, z
                glScalef(1, .7, .7);
                glutSolidSphere(2,50,10);
            glPopMatrix();

            glPushMatrix();
                glColor4ub(255, 0, 0, 0);
                glScalef(1, .7, .7);
                glutSolidSphere(2,50,10);
                //bala

                if(faiaFaiaPewPewPewPew){
                    distanciaBala += 0.9f;
                    if (distanciaBala >= 88.0f)
                        distanciaBala = 0.0f;
                    glColor4ub(255, 0, 0, 0);
                    glTranslatef(-distanciaBala, 0, 0); //x, y, z
                    glScalef(1, .7, .7);
                    glutSolidSphere(2,50,10);
                }
            glPopMatrix();
        glPopMatrix();
        //========================
    glPopMatrix();
    gluDeleteQuadric(Cabina);

}

void dibujaTetera(float x, float y, float z){
glPushMatrix();
    glTranslatef(0,-6.4,0);
    glTranslatef(x,y,z);
    glColor4ub(200,200,200,0);
    glutSolidTeapot(1);
glPopMatrix();
}

void DibujaMesa(float x, float y, float z){
    GLUquadricObj *Cabina;
    Cabina = gluNewQuadric();
    gluQuadricDrawStyle(Cabina, GLU_FILL);
            gluQuadricNormals(Cabina, GLU_NONE);

    glPushMatrix();
        glRotated(90,1,0,0);
        glTranslatef(0,0,3);
        glTranslatef(x,y,z);
        glColor4ub(205,133,63,0);
        glTranslatef(0,0,1);
        //tapa
        glPushMatrix();
            glScalef(.9,.9,.1);
            glutSolidCylinder(2,2,70,1);
        glPopMatrix();
        //Soporte de la tapa
        glPushMatrix();
            glutSolidCone(.7,.7,70,1);
        glPopMatrix();
        //mastil tapa
        glPushMatrix();
            gluCylinder(Cabina, .20, .27, 1.6, 40, 3);
        glPopMatrix();
        //decoracion mastil
        glTranslatef(0,0,1.7);
        glPushMatrix();
            glPushMatrix();
                glRotated(180,1,0,0);
                glutSolidCone(.4,.4,70,1);
            glPopMatrix();
            gluCylinder(Cabina, .40, .40, .09, 40, 3);
            glTranslatef(0,0,.08);
            glutSolidCone(.4,.4,70,1);
            glTranslatef(0,0,.58);
            glutSolidSphere(.47,30,30);
            glTranslatef(0,0,.58);
            glPushMatrix();
                glRotated(180,1,0,0);
                glutSolidCone(.6,.2,70,1);
            glPopMatrix();
        glPopMatrix();

        //tapa de abajo
        glTranslatef(0,0,1.2);
        glPushMatrix();
            glScalef(.9,.9,.1);
            glutSolidCylinder(1,1,70,1);
        glPopMatrix();
    glPopMatrix();
    gluDeleteQuadric(Cabina);
}

void DibujaPortaRetrato(float x, float y, float z){
    GLUquadricObj *Cabina;
    Cabina = gluNewQuadric();
    gluQuadricDrawStyle(Cabina, GLU_FILL);
    gluQuadricNormals(Cabina, GLU_NONE);

    glPushMatrix();

        glColor4ub(130, 82, 1,0);
        glTranslatef(2,-6.5,0);
        glTranslatef(x,y,z);
        glRotatef(-90,0,1,0);
        //Foto
        glPushMatrix();
            glClearColor (0, 0, 0, 255); //Colores para iniciar la ventana (Fondo)
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D,texturas[TEXTURE_PHOTO].ID);
            glScalef(0,.5,.5);
            //glutSolidCube(1);
            glRotatef(90,0,1,0);
              glBegin(GL_QUADS);
                glTexCoord2f(0, 0); glVertex2f(-1, -1);  // Bottom Left Of The Texture and Quad
                glTexCoord2f(1, 0); glVertex2f( 1, -1);  // Bottom Right Of The Texture and Quad
                glTexCoord2f(1, 1); glVertex2f( 1,  1);  // Top Right Of The Texture and Quad
                glTexCoord2f(0, 1); glVertex2f(-1,  1);  // Top Left Of The Texture and Quad
              glEnd();
            glDisable(GL_TEXTURE_2D);

        glPopMatrix();
        //tapa de atrás
        glPushMatrix();
            glTranslatef(-.05,0,0);
            glScalef(0,1,1);
            glutSolidCube(1);
        glPopMatrix();
        //Soporte
        glPushMatrix();

            glTranslatef(-.22,-.17,0);
            glRotatef(-30,0,0,1);
            glScalef(.03, .7, .3);
            glutSolidCube(1);
        glPopMatrix();
        //marco
        glPushMatrix();
            glRotatef(90,0,0,1);
            glScalef(.85,.85,.85);
            glTranslatef(-.5,0,-.5);
            //↓
            glPushMatrix();
                glTranslatef(0,0,-.075);
                gluCylinder(Cabina, .1, .1, 1.28, 6, 1);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(1.15,0,0);
                gluCylinder(Cabina, .1, .1, 1.23, 6, 1);
            glPopMatrix();
            glPushMatrix();
                glRotatef(90,0,1,0);
                gluCylinder(Cabina, .1, .1, 1.23, 6, 1);
            glPopMatrix();
            glPushMatrix();
                glRotatef(90,0,1,0);
                glTranslatef(-1.15,0,0);
                gluCylinder(Cabina, .1, .1, 1.23, 6, 1);
            glPopMatrix();
        glPopMatrix();

    glPopMatrix();
    gluDeleteQuadric(Cabina);

}

void DibujaCopa(int x, int y, int z, float rotar){
    GLUquadricObj *Cabina;
    //int cylinderCount = 0;
    int increase = 4;
    int decrease = 4;
    Cabina = gluNewQuadric();
    gluQuadricDrawStyle(Cabina, GLU_FILL);
    gluQuadricNormals( Cabina, GLU_NONE); // For if lighting is to be used.
    glEnable(GL_BLEND);

    glPushMatrix();
        //glTranslatef(movX, movY, movZ);
        glScalef(.009,.009,.009);
        glRotatef(rotar, 1, 0, 0);
        if(rotar == 0)
            glTranslatef(0,-820, -550);
        glColor4ub(255, 255, 255, 150);
        int depth = cupTopDepth;
        //========================================Dibuja la base de la helice trasera
        glTranslatef(50,100, 600);
        glTranslatef(x,y, z);
        glPushMatrix();
//1
            gluCylinder(Cabina, depth - increase, depth, 10, edge, 10);
//2
            depth += increase;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth - increase , depth, 10, edge, 10);
//3
            depth += increase*.5;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth - increase*.5, depth, 10, edge, 10);
//4
            depth -= decrease;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease, depth + decrease, 10, edge, 10);
//5
            depth -= decrease;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease*2, depth + decrease, 10, edge, 10);

//6
            depth -= decrease * 1.5;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease*2.5, depth + decrease, 10, edge, 10);
//7
            depth -= decrease * 1.5;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease*2.5, depth + decrease, 10, edge, 10);
//8
            depth -= decrease * 2;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease*3, depth + decrease, 10, edge, 10);
//9
            depth -= decrease * 2;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease*3, depth + decrease, 10, edge, 10);
//10
            depth -= decrease * 1.5;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease*2.5, depth + decrease, 10, edge, 10);
//11 Agarre
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease , depth + decrease, 40, edge, 10);

//12 apoyo
            depth += increase * 1.5;
            glTranslatef(0, 0, margin+30); //x, y, z
            gluCylinder(Cabina, depth - increase*.5, depth + increase*5, 30, edge, 10);
//13
            depth += increase * 1.5;
            glTranslatef(0, 0, margin+15); //x, y, z
            gluCylinder(Cabina, 1, depth + increase*3.4, 5, edge, 10);

        glPopMatrix();

    glPopMatrix();
    glDisable(GL_BLEND);
    gluDeleteQuadric(Cabina);

}

void DibujaTrompo(int x, int y, int z){
    GLUquadricObj *Cabina;
    int increase = 4;
    int decrease = 4;
    int depth = cupTopDepth;
    int inclina = inclination > 25 ? inclination * 3.5 : inclination * 3;
    Cabina = gluNewQuadric();
    gluQuadricDrawStyle(Cabina, GLU_FILL);
    gluQuadricNormals( Cabina, GLU_NONE); // For if lighting is to be used.

    glPushMatrix();
        glTranslatef(x, y, z);
        glRotatef(90, 1, 0, 0);
        glScalef(.009,.009,.009);
        glRotatef(-Rotacion, 0, 0, 1);
        glRotatef(inclination, 1, 0, 0);
        glTranslatef(7,-6.4,665 + inclina);

        glPushMatrix();
//Parte de arriba
            glPushMatrix();
//sup
            glColor4ub(70, 30, 0, 0);
            glTranslatef(0, 0, margin*-2); //x, y, z
            gluCylinder(Cabina, 1, depth*.3, 8, edge, 1);
//bot
            glColor4ub(170, 62, 1, 0);
            glTranslatef(0, 0, margin*.8); //x, y, z
            gluCylinder(Cabina, depth*.3 , depth*.3, 12, edge, 1);
            glPopMatrix();
//Tapa
            glPushMatrix();
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D,texturas[TEXTURE_STAR].ID);
                gluQuadricTexture(Cabina, GL_TRUE);
                glColor4ub(255, 255, 0, 0); //Amarillo
                gluDisk(Cabina, 0, depth - increase, 5, 1);
                glDisable(GL_TEXTURE_2D);
            glPopMatrix();

//Forma de copa
//1
            glColor4ub(130, 82, 1, 0);
            gluCylinder(Cabina, depth - increase, depth, 10, edge, 1);
//2
            glColor4ub(125, 77, 1, 0);
            depth += increase;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth - increase , depth, 10, edge, 1);
//3
            glColor4ub(130, 82, 1, 0);
            depth += increase*.5;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth - increase*.5, depth, 10, edge, 1);
//4
            glColor4ub(130, 82, 1, 0);
            depth -= decrease;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease, depth + decrease, 10, edge, 1);
//5
            glColor4ub(125, 77, 1, 0);
            depth -= decrease;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease*2, depth + decrease, 10, edge, 1);

//6
            glColor4ub(125, 77, 1, 0);
            depth -= decrease * 1.5;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease*2.5, depth + decrease, 10, edge, 1);
//7
            glColor4ub(130, 82, 1, 0);
            depth -= decrease * 1.5;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease*2.5, depth + decrease, 10, edge, 1);
//8
            glColor4ub(120, 77, 1, 0);
            depth -= decrease * 2;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease*3, depth + decrease, 10, edge, 1);
//9
            glColor4ub(120, 77, 1, 0);
            depth -= decrease * 2;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease*3, depth + decrease, 10, edge, 1);

//10
            glColor4ub(120, 77, 1, 0);
            depth -= decrease * 1.5;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease*2.5, depth + decrease, 10, edge, 1);
//11 Agarre
            glColor4ub(200, 200, 200, 0);
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease , depth + decrease, 10, edge, 1);

//12 apoyo
            glColor4ub(200, 200, 200, 0);
            depth += increase * 1.5;
            glTranslatef(0, 0, margin); //x, y, z
            glutSolidSphere(depth, edge, 4);

        glPopMatrix();

    glPopMatrix();
    gluDeleteQuadric(Cabina);

}

void drawFloor(){
    GLfloat Lado = 12;
    GLfloat Brick = Lado/8;
    GLint Cuadros = 18;

    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texturas[TEXTURE_FLOOR].ID);
    glTranslatef(-Brick, 0, 0);
    glBegin(GL_QUADS);
    for(int i=0;i<Cuadros;i++){
       for(int j=0;j<Cuadros;j++){
            if((i+j)%2==0){
                glColor4ub(25, 25, 112,Transparencia);
            }else{
                glColor4ub(112, 112, 112,Transparencia);
            }
                glTexCoord2f(0, 0); glVertex3f(-Lado+(i*Brick), 0, -Lado+(j*Brick));
                glTexCoord2f(1, 0); glVertex3f(-(Lado-Brick)+(i*Brick), 0, -Lado+(j*Brick));
                glTexCoord2f(1, 1); glVertex3f(-(Lado-Brick)+(i*Brick), 0, -(Lado-Brick)+(j*Brick));
                glTexCoord2f(0, 1); glVertex3f(-Lado+(i*Brick), 0, -(Lado-Brick)+(j*Brick));
        }
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void DibujaSilla (void) {

   //GLint Lados = 30;

   GLUquadricObj *qobj; //Define el objeto
   qobj = gluNewQuadric();
   gluQuadricDrawStyle(qobj, GLU_FILL); //Se dibuja un trompo
   gluQuadricNormals( qobj, GLU_NONE); // For if lighting is to be used.
    GLfloat ambient[]   = { 0.20, 0.05, 0.05, 1.0 };
    GLfloat diffuse[]   = { 0.89, 0.64, 0.14, 1.0 };
    GLfloat specular[]  = { 0.00, 0.00, 0.00, 1.0 };
    GLfloat emission[]  = { 0.00, 0.00, 0.00, 1.0 };

    glMaterialfv( GL_FRONT, GL_AMBIENT,   ambient );
    glMaterialfv( GL_FRONT, GL_DIFFUSE,   diffuse );
    glMaterialfv( GL_FRONT, GL_SPECULAR,  specular );
    glMaterialfv( GL_FRONT, GL_EMISSION,  emission );
    glColor4ub(205,133,63,0);
    glPushMatrix();
     glTranslatef(0, -7 ,0);
     glRotatef(-270,1,0,0);
     //4 patas
     glPushMatrix();
        glRotatef(-4,1,1,0); //inclinación de las patas
        glPushMatrix();
            glRotatef(315,0,0,1); //inclinación de las patas
            glRotatef(195,1,0,0); //inclinación de las patas 199
            gluCylinder(qobj, .1, .1, 2, 7, 1);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(1.5,0,0);
            glRotatef(225,0,0,1); //inclinación de las patas
            glRotatef(165,1,0,0); //inclinación de las patas
            gluCylinder(qobj, .1, .1, 2, 7, 1);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(0,1.5,.1);
            glRotatef(315,0,0,1); //inclinación de las patas
            glRotatef(168,0,1,0); //inclinación de las patas
            gluCylinder(qobj, .1, .1, 2, 7, 1);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(1.5,1.5,0);
            glRotatef(315,0,0,1); //inclinación de las patas
            glRotatef(165,1,0,0); //inclinación de las patas
            gluCylinder(qobj, .1, .1, 2, 7, 1);
        glPopMatrix();
     glPopMatrix();

     //Cuadro en las patas
     glPushMatrix();
        glRotatef(-90,1,0,0);
        glTranslatef(.2,1,.1);
        glPushMatrix();
            gluCylinder(qobj, .1, .1, 1.2, 6, 1);
        glPopMatrix();
        glPushMatrix();
            glTranslatef(1.15,0,0);
            gluCylinder(qobj, .1, .1, 1.2, 6, 1);
        glPopMatrix();
        glPushMatrix();
            glRotatef(90,0,1,0);
            gluCylinder(qobj, .1, .1, 1.2, 6, 1);
        glPopMatrix();
        glPushMatrix();
            glRotatef(90,0,1,0);
            glTranslatef(-1.15,0,0);
            gluCylinder(qobj, .1, .1, 1.2, 6, 1);
        glPopMatrix();
     glPopMatrix();

     // Asiento
     glPushMatrix();
        glTranslatef(.8,.7,-1.90);
        glScalef(.9,.9,.1);
        glutSolidSphere(1, 50, 50);
     glPopMatrix();
   glPopMatrix();

   gluDeleteQuadric(qobj);

}

void drawUmbrella (void) {

   //GLint Lados = 30;

   GLUquadricObj *qobj; //Define el objeto
   qobj = gluNewQuadric();
   gluQuadricDrawStyle(qobj, GLU_FILL); //Se dibuja un trompo
   gluQuadricNormals( qobj, GLU_NONE); // For if lighting is to be used.

    GLfloat ambient[]   = { 0.20, 0.05, 0.05, 1.0 };
    GLfloat diffuse[]   = { 0.89, 0.64, 0.14, 1.0 };
    GLfloat specular[]  = { 0.00, 0.00, 0.00, 1.0 };
    GLfloat emission[]  = { 0.00, 0.00, 0.00, 1.0 };
    GLfloat shininess[] = { 128.0 };

    glMaterialfv( GL_FRONT, GL_AMBIENT,   ambient );
    glMaterialfv( GL_FRONT, GL_DIFFUSE,   diffuse );
    glMaterialfv( GL_FRONT, GL_SPECULAR,  specular );
    glMaterialfv( GL_FRONT, GL_EMISSION,  emission );
    glMaterialfv( GL_FRONT, GL_SHININESS, shininess );

   glPushMatrix();
     glTranslatef(0, -3.7 ,0);
     glRotatef(45, 1, 0, 0);
     glColor4ub(255, 0, 0, 0); //Capa roja
     gluCylinder(qobj, 0, 3.9, 1, 16, 1);
     glColor4ub(100, 100, 100, 0); //Palo gris
     gluCylinder(qobj, 0.05, 0.05, 4.7, 10, 1);
   glPopMatrix();

   glPushMatrix();
    gluQuadricDrawStyle(qobj, GLU_LINE);
    glColor4ub(20, 20, 20, 0); //varillas
     glTranslatef(0, -3.8 ,0);
     glRotatef(45, 1, 0, 0);
     gluCylinder(qobj, 0, 3.97, 1, 16, 1);
   glPopMatrix();

   /*glPushMatrix();
    gluQuadricDrawStyle(qobj, GLU_LINE);
    glColor4ub(20, 20, 20, 0); //varillas
     glTranslatef(0, -5 ,0);
     glRotatef(45, 1, 0, 0);
     glRotatef(180, 1, 0, 0);
     gluCylinder(qobj, 0, 3, .1, 16, 1);
   glPopMatrix();*/
   gluDeleteQuadric(qobj);

}

void drawScene() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  Luz > 0 ? glEnable(GL_LIGHT0) : glDisable(GL_LIGHT0);
  LuzHeli > 0 ? glEnable(GL_LIGHT1) : glDisable(GL_LIGHT1);
  GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat diffuseMaterial[] = { 1.0, 1.0, 1.0, 0.0 };
    GLfloat lmodel_ambient[] = { Luz, Luz, Luz, Luz };
    GLfloat lightColor[] = {1.0f, 1.0f, 1.0f, 1.0f};//blanco
    GLfloat lightPos[] = {0, LIGHT_POS+BOX_HEIGHT+2, 0, 1};
    GLfloat spot_direction[] = { 0, -1, 0 };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);



  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0, 0, -40);
  glRotatef(30, 1, 0, 0);
  glScalef(Zoom, Zoom, Zoom);
  glRotatef(spinX, 1, 0, 0);
  glRotatef(spinY, 0, 1, 0);

  if (Rot) glRotatef(RotacionEscenario, 0, 1, 0);

  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialf(GL_FRONT, GL_SHININESS, 65.0f);

    //printf("dx=%d dy=%d dz=%d \n", dx, dy, dz);


  glPushMatrix();
    glTranslatef(0, BOX_HEIGHT, 0);
    DibujaBombillo(0,0,0,1);
    DibujaModeloPersona(6.5,0,0,1);

    DibujaBola(0,0,-5,1);
    drawUmbrella();
    dibujaTetera(-2,0,2);
    DibujaTrompo(-4, 0, -5);//izq-derecha, arriba-abajo, frente-atras
    DibujaSilla();
    DibujaCopa(40,00,-200,90);
    DibujaCopa(200,0,0,0);
    DibujaMesa(3,5,0);
    DibujaPortaRetrato(1,3,5);
    DibujaHelicoptero(5.3, 4.2, 6, .015);

  glPopMatrix();

  glEnable(GL_STENCIL_TEST); //Enable using the stencil buffer
  glColorMask(0, 0, 0, 0); //Disable drawing colors to the screen
  glDisable(GL_DEPTH_TEST); //Disable depth testing
  glStencilFunc(GL_ALWAYS, 1, 1); //Make the stencil test always pass

  //Make pixels in the stencil buffer be set to 1 when the stencil test passes
  glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

  //Aquí se dibuja el piso
  drawFloor();

  glColorMask(1, 1, 1, 1); //Enable drawing colors to the screen
  glEnable(GL_DEPTH_TEST); //Enable depth testing
  //Make the stencil test pass only when the pixel is 1 in the stencil buffer
  glStencilFunc(GL_EQUAL, 1, 1);
  glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //Make the stencil buffer not change

  //Draw the cube, reflected vertically, at all pixels where the stencil
  //buffer is 1
  glPushMatrix();
    glScalef(1, -1, 1);
    glTranslatef(0, BOX_HEIGHT, 0);
    //Aquí se dibujan los objetos que se van a reflejar
    DibujaModeloPersona(6.5,0,0,1);
    DibujaBola(0,0,-5,1);
    dibujaTetera(-2,0,2);
    DibujaTrompo(-4, 0, -5);//izq-derecha, arriba-abajo, frente-atras
    DibujaSilla();
    drawUmbrella();
    DibujaPortaRetrato(1,3,5);
    DibujaMesa(3,5,0);
    DibujaHelicoptero(5.3, 4.2, 6, .015);

    DibujaCopa(40,0,-200,90);
    DibujaCopa(200,0,0,0);
  glPopMatrix();

  glDisable(GL_STENCIL_TEST); //Disable using the stencil buffer
  //Blend the floor onto the screen
  glEnable(GL_BLEND);

  //Aquí se dibuja el piso que refleja
  drawFloor();
  glDisable(GL_BLEND);

  glutSwapBuffers();
}

void init() {
    FILE *comandos;
    glClearColor(0, 0, 0, 0); //Fondo negro en toda la escena
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_MULTISAMPLE);
    glHint(GL_LINE_SMOOTH, GL_NICEST);
    glHint(GL_POINT_SMOOTH, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH, GL_NICEST);
    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



    //cargar textura
     // detect current settings
        GLint iMultiSample = 0;
        GLint iNumSamples = 0;
        glGetIntegerv(GL_SAMPLE_BUFFERS, &iMultiSample);
        glGetIntegerv(GL_SAMPLES, &iNumSamples);
        printf("MSAA on, GL_SAMPLE_BUFFERS = %d, GL_SAMPLES = %d\n", iMultiSample, iNumSamples);

    if(!cargarTGA("E:/graficos/proyecto cuarto/Bola.tga", &texturas[TEXTURE_BALL]) )
     printf("Error cargando textura de estrella\n");
    if(!cargarTGA("E:/graficos/proyecto cuarto/foto.tga", &texturas[TEXTURE_PHOTO]) )
     printf("Error cargando textura de foto\n");
    if(!cargarTGA("E:/graficos/proyecto cuarto/floor.tga", &texturas[TEXTURE_FLOOR]) )
     printf("Error cargando textura de pizo\n");
    if(!cargarTGA("E:/graficos/proyecto cuarto/Star.tga", &texturas[TEXTURE_STAR]) )
     printf("Error cargando textura de estrella\n");

    if((comandos = fopen("E:/graficos/proyecto cuarto/planvuelo.txt", "r")) == NULL)
     printf("Error cargando el plan de vuelo\n");

    k = 0;
    while (fscanf(comandos, "%c %f", &direccion[k], &desplazamiento [k]) != EOF) k++;
    MaxPunt = k - 1;
    fclose(comandos);
}

void handleResize(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}

void spinDisplay(void) {
    //printf("rotacion: %.6f \n", Rotacion);
    GLfloat auxSpeed = 0;
    RotacionEscenario += 0.29;
    if(planvuelo){
        RotacionHeli += 0.69;
    }
    if(isTrompoSpinning){
        inclination > 0 ? inclination -= .0308: inclination;
        Rotacion += 0.69;
    }
    //Inicio movimiento rotación helicptero

    if (ImpulseSpinHeli){
        impulse += .004;
        if(impulse >= .9999)
            ImpulseSpinHeli = false;
    }
    if(!spinHeli && impulse > 0.0){
        impulse -= 0.005;
        //Rotacion -= 0.69;
       // printf("%f\n", impulse);
        if(impulse <= 0)
            impulse = 0.0;
    }//else Rotacion += 0.69;

    //fin movimiento rotación helicptero
    if (Rotacion >= 360)  Rotacion = 0;
    if (Rotacion < 0)  Rotacion = 360;
    if (RotacionHeli >= 360)  RotacionHeli = 0;
    if (RotacionHeli < 0)  RotacionHeli = 360;
    if (RotacionEscenario >= 360)  RotacionEscenario = 0;
    //--------------Inicio movimiento de la bola ------------
    if (BallSpeed >= 0 && isBallJump){
        yball += .07 * BallSpeed;
        BallRot += 8 * BallSpeed;
        auxSpeed = BallSpeed - .3;
        if (BallRot >= 360)  BallRot = 0;
        if(auxSpeed >= 0 && BallRot >= 0 && BallRot <= 180){
            BallRadians = (BallRot*PI/180) * .5 ;
            zball = -sin(BallRadians)* ballJumpHeight * auxSpeed ;
        }
        else if(auxSpeed >= 0){
                BallRadians = (PI - (BallRot-180) * PI/180) * .5;
                zball = -sin(BallRadians) * ballJumpHeight * auxSpeed ;
             }

        BallSpeed -= .002;
    }
    //--------------Fin movimiento de la bola ------------
    glutPostRedisplay(); //Vuelve a dibujar
}

void mouse(int button, int state, int x, int y) {
   switch (button) {
      case GLUT_LEFT_BUTTON:
         if (state == GLUT_DOWN) {
            glutIdleFunc(spinDisplay);
         }
         break;

      case GLUT_RIGHT_BUTTON:
      case GLUT_MIDDLE_BUTTON:
         if (state == GLUT_DOWN)
            glutIdleFunc(NULL);
         break;

      default:
         break;
   }
}

void mouseMotion(int x, int y) {
     spinX = y;
     spinY = x;
     //printf("X %5.2f Y %5.2f\n", spinX, spinY);
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
  glutInitWindowSize(1280,720);

  glutCreateWindow("Proyecto");
  init();

  glutDisplayFunc(drawScene);
  glutKeyboardFunc(handleKeypress);
  glutReshapeFunc(handleResize);
  glutMouseFunc(mouse); //Activa los controles del mouse
  glutMotionFunc(mouseMotion);

  glutMainLoop();

  return 0;
}
