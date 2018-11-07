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
#define BOX_HEIGHT 7.0f//The height of the box off of the ground
#define FLOOR_SIZE 25.0f//The length of each side of the floor
#define MODEL_EDGE 120
#define PI 3.14159265

//Escenario
GLfloat sceneryZoom = 1.8;
GLfloat sceneryTransparency = 230; // reflejo del piso
GLfloat sceneryMouseSpinX = 0;
GLfloat sceneryMouseSpinY = 0;
GLfloat sceneryRotation = 0;
bool isSceneryRotating = false;
bool isStencilOn = true;
bool isGlobalShadowOn = true;

//Trompo
GLfloat trompoInclination = 30;
GLfloat trompoRotation = 0;
bool isTrompoSpinning = false;

//Bola
GLfloat ballPositionX = 0;
GLfloat ballPositionY = 0;
GLfloat ballPositionZ = 0;
GLfloat ballRotation = 0;
GLfloat BallSpeed = 1;
GLint ballControl = 2;
GLint ballJumpHeight = 4;
bool isBallJumping = false;

//Helicoptero
char helicopterCommands[90]; //comandos
GLfloat helicopterDirections[90]; //comandos value
GLfloat helicopterMovementX = 5.3;
GLfloat helicopterMovementY = -2.15;
GLfloat helicopterMovementZ = 6;
GLfloat helicopterRotationX = 0;
GLfloat helicopterRotationY = 0;
GLfloat helicopterRotationZ = 0;
GLfloat helicopterPropellerRotation = 0;
GLfloat helicopterPropellerImpulse = 0;
GLfloat helicopterPewPewPewDistance = 0;
GLfloat helicopterLightIntensity = 1.00f;
GLfloat helicopterScale =  .015f;
GLint helicopterCommandCounter = 0;
GLint helicopterCommandInstructionCount = 0;
bool isHelicopterPropellerSpinning = false;
bool isHelicopterCommandActive = false; //auto
bool isHelicopterLaserActive = false;
bool isFaiaFaiaPewPewPew = false;
bool isHelicopterPropellerImpulsing = false;

//light0Intensity y sombras
GLfloat lightColorWhite[] = {0.5f, 0.5f, 0.5f, 0.7f};//blanco
GLfloat lightColorBlue[] = {0.67f, 0.84f, 1.0f, 1.0f};//azul

GLfloat light0Position[] = { 0.0, 7.5, 0.0, 1.0 }; //temp
GLfloat light0Ambient[]  = { 0.2, 0.2, 0.2, 1.0 };
GLfloat light0Diffuse[] = { 1.0, 1.0, 1.0, 0.0 };
GLfloat light0Specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light0SpotDirection[] = { 0, -1, 0 };
GLfloat light0Intensity = 0.3;
GLfloat light0ModelIntensity[] = { light0Intensity, light0Intensity, light0Intensity, light0Intensity };
bool isLight0On = false;

GLfloat light1Position[] = { helicopterMovementX, helicopterMovementY, helicopterMovementZ, 1.0 }; //temp
GLfloat light1Ambient[]  = { 0.2, 0.2, 0.2, 1.0 };
GLfloat light1Diffuse[] = { 1.0, 1.0, 1.0, 0.0 };
GLfloat light1Specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light1SpotDirection[] = { 0, -1, 0 };
GLfloat light1Intensity = 0.7;
GLfloat light1ModelIntensity[] = { light1Intensity, light1Intensity, light1Intensity, light1Intensity };
bool isLight1On = false;

GLfloat shadowIntensity = 0.7;    // darkness of shadow
GLfloat shadowAlpha  = 0.0;        // object rotation angle (degrees)
GLfloat shadow_plane[] = { 0.0, 1.0, 0.0, 0.0 }; // plano para proyectar la sombra

GLint isRightKeyPressed = false;
GLint isLeftKeyPressed  = false;
GLint isUpKeyPressed    = false;
GLint isDownKeyPressed  = false;

GLint ArrowColor;

GLfloat Zoom = 1;
GLint Cubo = 0;

GLfloat Transparencia = 200;
GLfloat _angle = 0;

GLfloat spinX=0.0, spinY=0.0;

//Dirección
GLint dx = 0; GLint dy = -1; GLint dz = 0;

//Posición
GLfloat rX = 0; GLfloat rY = 7.5; GLfloat rZ = 0;

void shadowTransform( float n[], float light[] ){
    float m[16];
    float k;
    int i, j;

    k = n[0] * light[0] + n[1] * light[1] + n[2] * light[2] + n[3] * light[3];

    for ( i = 0; i < 4; i++ ){
        for ( j = 0; j < 4; j++ ){
            m[4*i+j] = -n[i] * light[j];
        }
        m[5*i] += k;
    }

    glMultMatrixf( m );
}

void handleKeypress(unsigned char key, int x, int y) {
  switch (key) {
    case 27: //Escape key
         exit(0);
    break;
    case 'Z':
        helicopterMovementZ += helicopterMovementZ > 2200 ? 0: 0.5; // se mueve hacia afuera
        light1Position[2] = helicopterMovementZ * helicopterScale;
    break;
    case 'z':
        helicopterMovementZ -= helicopterMovementZ > 2200 ? 0: 0.5; // se mueve hacia adentro
        light1Position[2] = helicopterMovementZ * helicopterScale;
    break;
    case 'w':
        helicopterMovementY += helicopterMovementY > 2200 ? 0: 0.5; // se mueve hacia arriba
        light1Position[1] = helicopterMovementY * helicopterScale+3;
    break;
    case 's':
        helicopterMovementY -= helicopterMovementY < -2000 ? 0: 0.5; // se mueve hacia abajo
        light1Position[1] = helicopterMovementY * helicopterScale-3;
    break;
    case 'a':
        helicopterMovementX -= helicopterMovementX < -2200 ? 0: 0.5; // Se mueve a la izquierda
        light1Position[0] = helicopterMovementX * helicopterScale;
    break;
    case 'd':
        helicopterMovementX += helicopterMovementX > 2200 ? 0: 0.5; // se mueve  a la derecha
        light1Position[0] = helicopterMovementX * helicopterScale;
    break;

    case '(':
        helicopterRotationZ += helicopterRotationZ > 360 ? 0: 1;  // rota hacia abajo
    break;
    case '"':
        helicopterRotationZ -= helicopterRotationZ > 360 ? 0: 1; // rota hacia arriba
    break;
    case '$':
        helicopterRotationY += helicopterRotationY > 360 ? 0: 1;  //rota de lado
    break;
    case '&':
        helicopterRotationY -= helicopterRotationY < -360 ? 0: 1; //rota de lado
    break;
    case ')':
        helicopterRotationX -= helicopterRotationX < -360 ? 0: 1; //rota de cabeza
    break;
    case '!':
        helicopterRotationX += helicopterRotationX > 360 ? 0: 1; // rota de cabeza
    break;


   /* case 'F': //Posición de la light0Intensity
        lightBulbPosition += .2;
    break;

    case 'f': //Posición de la light0Intensity
        lightBulbPosition -= .2;
    break;*/

    case 'n': //Acciona el laser del helicoptero
        isHelicopterLaserActive = !isHelicopterLaserActive;
    break;

    case 'm': //Acciona los disparos del helicoptero
        isFaiaFaiaPewPewPew = !isFaiaFaiaPewPewPew;
    break;

    case 'r': //La esfera gira
         isSceneryRotating = !isSceneryRotating;
    break;

    case 'l': //light0Intensity
         light0Intensity >= 0.0 ? light0Intensity -= 0.003 : light0Intensity;
    break;
    case 'L': //light0Intensity
         light0Intensity <= 0.8 ? light0Intensity += 0.003 : light0Intensity;
    break;
    case '-':
        if (sceneryZoom > 0) sceneryZoom -= 0.03;
    break;

    case '+':
        if (sceneryZoom < 10) sceneryZoom += 0.03;
    break;

    case 't': //Sube
         if (++sceneryTransparency > 255) sceneryTransparency = 0;
    break;
    case 'T': //Sube
         if (--sceneryTransparency <= 0) sceneryTransparency = 255;
    break;
    case 'A': //automagico
         isHelicopterCommandActive = !isHelicopterCommandActive;
         isHelicopterCommandActive ? printf("\nModo automagico activado\n") : printf("\nModo automagico desactivado\n");
         if (isHelicopterCommandActive){
            glEnable(GL_LIGHT1);
            isLight1On = true;
         }else{
            glDisable(GL_LIGHT1);
            isLight1On = false;
         }
         helicopterCommandCounter = helicopterCommandInstructionCount = 0;

    break;

    case 'e': //mueve las helices del helicoptero
         isHelicopterPropellerSpinning = !isHelicopterPropellerSpinning;
         if (isHelicopterPropellerSpinning)
            isHelicopterPropellerImpulsing = !isHelicopterPropellerImpulsing;
    break;
    case 'b': //mueve las helices del helicoptero
         isBallJumping = true;
    break;
    case 'v': //mueve las helices del helicoptero
         isTrompoSpinning = true;
    break;
    case 'S':
        isGlobalShadowOn = !isGlobalShadowOn;
    break;
  }
}

void drawPersonWoodenModel(float x, float y, float z, float scale, bool shadow){
    GLUquadricObj *Cabina;
    Cabina = gluNewQuadric();
    gluQuadricDrawStyle(Cabina, GLU_FILL);
    gluQuadricNormals(Cabina, GLU_NONE);
    glPushMatrix();
            glTranslatef(x,y,z);
            glScalef(.5,.5,.5);
            glScalef(scale,scale,scale);
            glRotatef(90,1,0,0);
            glRotatef(-90,0,0,1);
            if(!isGlobalShadowOn || !shadow)
                glColor4ub(203, 175, 135, 0);
            glPushMatrix(); //Cabeza
                glScalef(1, 1, 1.15);
                glutSolidSphere(1, MODEL_EDGE,30);
            glPopMatrix();
            glTranslatef(0,0,1.5);
            glPushMatrix(); //cuello
                if(!isGlobalShadowOn || !shadow)
                    glColor4ub(206,169,137,0);
                glScalef(1, 1, 1.06);
                glutSolidSphere(.7, MODEL_EDGE,30);
            glPopMatrix();
            if(!isGlobalShadowOn || !shadow)
                glColor4ub(203, 175, 135, 0);
            glTranslatef(0,0,.5);
            glPushMatrix(); //Pecho
                gluCylinder(Cabina, 1.3, .9, 2.5, 30, 30);
                gluDisk(Cabina,0,1.3,MODEL_EDGE,40);
                glTranslatef(0,0,2.5);
                gluDisk(Cabina,0,.9,MODEL_EDGE,40);
            glPopMatrix();
            glTranslatef(0,0,.6);
            glPushMatrix(); //brazo izquierdo
                glTranslatef(-1.5,0,0);
                glScalef(1, 1, 1.06);
                if(!isGlobalShadowOn || !shadow)
                    glColor4ub(206,169,137,0);
                glutSolidSphere(.5, MODEL_EDGE,30); //articulacion brazo
                glRotatef(10,0,-1,0);
                glTranslatef(0,0,.45);
                if(!isGlobalShadowOn || !shadow)
                    glColor4ub(203, 175, 135, 0);
                gluCylinder(Cabina, .4, .3, 1.6, MODEL_EDGE, 30); //brazo
                glTranslatef(0,0,1.8);
                if(!isGlobalShadowOn || !shadow)
                    glColor4ub(206,169,137,0);
                glutSolidSphere(.3, MODEL_EDGE, 50); //articulacion codo
                glTranslatef(0,0,.3);
                if(!isGlobalShadowOn || !shadow)
                    glColor4ub(203, 175, 135, 0);
                glRotated(80,1,0,0);
                gluCylinder(Cabina, .3, .15, 1.9, MODEL_EDGE, 30); //brazo
                glTranslatef(-.2,0,2);
                glRotated(75,0,1,0);
                glScalef(1,.5,.5);
                glutSolidSphere(1, 10, 2); //mano
            glPopMatrix();
            glPushMatrix(); //brazo derecho
                glRotated(180,0,0,1);
                glTranslatef(-1.5,0,0);
                glScalef(1, 1, 1.06);
                if(!isGlobalShadowOn || !shadow)
                    glColor4ub(206,169,137,0);
                glutSolidSphere(.5, MODEL_EDGE,30); //articulacion brazo
                glRotatef(10,0,-1,0);
                glTranslatef(0,0,.45);
                if(!isGlobalShadowOn || !shadow)
                    glColor4ub(203, 175, 135, 0);
                gluCylinder(Cabina, .4, .3, 1.6, MODEL_EDGE, 30); //brazo
                glTranslatef(0,0,1.8);
                if(!isGlobalShadowOn || !shadow)
                    glColor4ub(206,169,137,0);
                glutSolidSphere(.3, MODEL_EDGE, 50); //articulacion codo
                glTranslatef(0,0,.3);
                if(!isGlobalShadowOn || !shadow)
                    glColor4ub(203, 175, 135, 0);
                gluCylinder(Cabina, .3, .15, 1.9, MODEL_EDGE, 30); //brazo
                glTranslatef(-.2,0,2);
                glRotated(75,0,1,0);
                glScalef(1,.5,.5);
                glutSolidSphere(1, 10, 2); //mano
            glPopMatrix();
            glTranslatef(0,0,2.2);
            glPushMatrix(); //abdomen
                if(!isGlobalShadowOn || !shadow)
                    glColor4ub(206,169,137,0);
                glScalef(1.2, 1, 1.06);
                glutSolidSphere(.7, MODEL_EDGE,30);
            glPopMatrix();
            if(!isGlobalShadowOn || !shadow)
                glColor4ub(203, 175, 135, 0);
            glTranslatef(0,0,.55);
            glPushMatrix(); //cintura
                gluCylinder(Cabina, .7, 1.2, 1.3, 30, 30);
                gluDisk(Cabina,0,.7,MODEL_EDGE,40);
                glTranslatef(0,0,1.3);
                gluDisk(Cabina,0,1.2,MODEL_EDGE,40);
            glPopMatrix();
            glTranslatef(0,0,1.6);
            glPushMatrix(); //pierna izquierdo
                glTranslatef(-.62,0,0);
                if(!isGlobalShadowOn || !shadow)
                    glColor4ub(206,169,137,0);
                glutSolidSphere(.5, MODEL_EDGE,30); //articulacion pierna
                glTranslatef(0,0,.42);
                if(!isGlobalShadowOn || !shadow)
                    glColor4ub(203, 175, 135, 0);
                gluCylinder(Cabina, .5, .4, 3, MODEL_EDGE, 30); //pierna
                glTranslatef(0,0,3.2);
                if(!isGlobalShadowOn || !shadow)
                    glColor4ub(206,169,137,0);
                glutSolidSphere(.35, MODEL_EDGE, 50); //articulacion pierna
                glTranslatef(0,0,.32);
                if(!isGlobalShadowOn || !shadow)
                    glColor4ub(203, 175, 135, 0);
                gluCylinder(Cabina, .3, .15, 2.5, MODEL_EDGE, 30); //pierna
                glTranslatef(0,0,2.55);
                if(!isGlobalShadowOn || !shadow)
                    glColor4ub(206,169,137,0);
                glutSolidSphere(.13, MODEL_EDGE, 40); //articulación pie
                glTranslatef(0,0,.47);
                glScalef(.5, .5, .7);
                if(!isGlobalShadowOn || !shadow)
                    glColor4ub(203, 175, 135, 0);
                glutSolidSphere(.6, 10, 4); //pie
            glPopMatrix();
            glPushMatrix(); //pierna derecho
                glRotated(180,0,0,1);
                glTranslatef(-.62,0,0);
                if(!isGlobalShadowOn || !shadow)
                    glColor4ub(206,169,137,0);
                glutSolidSphere(.5, MODEL_EDGE,30); //articulacion pierna
                glTranslatef(0,0,.42);
                if(!isGlobalShadowOn || !shadow)
                    glColor4ub(203, 175, 135, 0);
                gluCylinder(Cabina, .5, .4, 3, MODEL_EDGE, 30); //pierna
                glTranslatef(0,0,3.2);
                if(!isGlobalShadowOn || !shadow)
                    glColor4ub(206,169,137,0);
                glutSolidSphere(.35, MODEL_EDGE, 50); //articulacion pierna
                glTranslatef(0,0,.32);
                if(!isGlobalShadowOn || !shadow)
                    glColor4ub(203, 175, 135, 0);
                gluCylinder(Cabina, .3, .15, 2.5, MODEL_EDGE, 30); //pierna
                glTranslatef(0,0,2.55);
                if(!isGlobalShadowOn || !shadow)
                    glColor4ub(206,169,137,0);
                glutSolidSphere(.13, MODEL_EDGE, 40); //articulación pie
                glTranslatef(0,0,.47);
                glScalef(.5, .5, .7);
                if(!isGlobalShadowOn || !shadow)
                    glColor4ub(203, 175, 135, 0);
                glutSolidSphere(.6, 10, 4); //pie
            glPopMatrix();

    glPopMatrix();
    gluDeleteQuadric(Cabina);
}

void drawLightBulb(float x, float y, float z, float scale) {
    GLUquadricObj *Cabina;
    Cabina = gluNewQuadric();

    gluQuadricDrawStyle(Cabina, GLU_FILL);
    glPushMatrix();
        glTranslatef(x, y, z);
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
            glMaterialfv(GL_FRONT, GL_DIFFUSE, light0Diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, light0Specular);
            glMaterialf(GL_FRONT, GL_SHININESS, 65.0f);
            glColor4ub(255, 255, 0, 1);
            glutSolidSphere(0.5, 50, 50);
        glPopMatrix();
    glPopMatrix();
    gluDeleteQuadric(Cabina);
}

void drawBall(float x, float y, float z, float scale, bool shadow) {
    glPushMatrix();
        glScalef(scale,scale,scale);
        glTranslatef(x, y, z);
        glTranslatef(0, .7, 5);
        glRotatef(90, 1, 0, 0);
        if(!isGlobalShadowOn || !shadow)
            glColor4ub(255, 255, 255, 0);
        GLUquadricObj *sphere=NULL;
        sphere = gluNewQuadric();
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,texturas[TEXTURE_BALL].ID);
        gluQuadricDrawStyle(sphere, GLU_FILL);
        gluQuadricTexture(sphere, TRUE);
        gluQuadricNormals(sphere, GLU_NONE);

        glTranslatef(-7, -6.3, 7); //Posiciona en el suelo
        glTranslatef(ballPositionX, ballPositionY, ballPositionZ); //mueve la bola
        if(ballControl == 2)
            glRotatef(ballRotation, 1, 0, 0);
        //glutSolidSphere(.7,50,50);
        gluSphere(sphere, .7,50,50);
        glDisable(GL_TEXTURE_2D);
        gluDeleteQuadric(sphere);
    glPopMatrix();

}

void drawHelicopter(float x, float y, float z, float scale, bool shadow){

    GLUquadricObj *Cabina;
    Cabina = gluNewQuadric();
    gluQuadricDrawStyle(Cabina, GLU_FILL);
    gluQuadricNormals( Cabina, GLU_NONE); // For if lighting is to be used.
    glPushMatrix();
        glScalef(scale,scale,scale);
        float multiSpeed =  40 * helicopterPropellerImpulse ;
        if(isHelicopterCommandActive){
            try{
                if(helicopterCommandCounter == helicopterDirections[helicopterCommandInstructionCount]){ //Ya el conteo llegó al final, se avanza a la siguiente instrucción
                    helicopterCommandInstructionCount++;
                    helicopterCommandCounter = 0;
                }else{
                    unsigned char letter = helicopterCommands[helicopterCommandInstructionCount];
                    if (letter == NULL){
                        isHelicopterCommandActive = !isHelicopterCommandActive;
                        helicopterCommandCounter = helicopterCommandInstructionCount = 0;
                        printf("\n\nFinalizo el modo automagico\n");
                        glDisable(GL_LIGHT1);
                    }
                    if(!isHelicopterPropellerImpulsing){
                        handleKeypress(letter,0,0);
                        helicopterCommandCounter++;
                    }
                }
            }catch(...){
            printf("Entro al catch");
            }
        }
        glLightfv(GL_LIGHT1, GL_POSITION, light1Position);
        glTranslatef(x, y, z);
        glTranslatef(helicopterMovementX, helicopterMovementY, helicopterMovementZ);


        glRotatef(helicopterRotationX, 1, 0,0);
        glRotatef(helicopterRotationY, 0, 1,0);
        glRotatef(helicopterRotationZ, 0, 0,1);

        //========================================Dibuja esfera
        glPushMatrix();
            if(!isGlobalShadowOn || !shadow)
                glColor4ub(0, 0, 255, 0);   //Azul
            glScalef(1.3, 1, 1);
            glutSolidSphere(20,50,10);
        glPopMatrix();

        //========================================Dibuja helices

        if(!isGlobalShadowOn || !shadow)
            glColor4ub(210, 210, 210, 0);   //Verde
        glPushMatrix();
            glTranslatef(0, 20, 0);
            glPushMatrix();
                //hacer una variable global auxiliar que toma la última rotación de que tuvo, esto para ir disminuyendolo con el multiplicador cuando ya terminó el plan de vuelo
                glRotatef(helicopterPropellerRotation*multiSpeed, 0, -1, 0);
                glScalef(60, 1, 5);
                glutSolidCube (1.0);
            glPopMatrix();
            glPushMatrix();
                glRotatef(helicopterPropellerRotation*multiSpeed, 0, -1, 0);
                glScalef(5, 1, 60);
                glutSolidCube (1.0);
            glPopMatrix();

        //========================================Dibuja la base helice
            glPushMatrix();
                if(!isGlobalShadowOn || !shadow)
                    glColor4ub(255, 255,255, 0);
                glTranslatef(0, 0, 0); //x, y, z
                glScalef(1.6, 1.6, 1.6);
                glutSolidSphere(2,50,10);
            glPopMatrix();
        glPopMatrix();

            //========================================Dibuja helice trasera

        if(!isGlobalShadowOn || !shadow)
            glColor4ub(210, 210, 210, 0);   //Verde
        glPushMatrix();
            glTranslatef(60, 0, 6);
            glRotatef(90, 0, 1, 0);
            glPushMatrix();
                glRotatef(helicopterPropellerRotation*multiSpeed, 1, 0, 0);
                glScalef(1, 5, 60);
                glutSolidCube (.5);
            glPopMatrix();
            glPushMatrix();
                glRotatef(helicopterPropellerRotation*multiSpeed, 1, 0, 0);
                glScalef(1, 60, 5);
                glutSolidCube (.5);
            glPopMatrix();
        glPopMatrix();


        //========================================Dibuja la base de la helice trasera
            glPushMatrix();
                glTranslatef(62.5, 0, 0); //x, y, z
                if(!isGlobalShadowOn || !shadow)
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
            if(!isGlobalShadowOn || !shadow)
                glColor4ub(0, 0, 255, 0);   //Azul
            gluCylinder(Cabina, 6, 2, 46, 50, 1);
        glPopMatrix();

        //======================================== Dibuja las patas
        glPushMatrix();
            if(!isGlobalShadowOn || !shadow)
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
                if(!isGlobalShadowOn || !shadow)
                    glColor4ub(255, 0, 0, 0);
                glTranslatef(45, 00, 0); //x, y, z
                glScalef(1, .7, .7);
                glutSolidSphere(2,50,10);

            glPopMatrix();

            glPushMatrix();
                if(!isGlobalShadowOn || !shadow)
                    glColor4ub(255, 0, 0, 0);
                glScalef(1, .7, .7);
                glutSolidSphere(2,50,10);
                //Bala
                if(isFaiaFaiaPewPewPew){
                    helicopterPewPewPewDistance += 0.9f;
                    if (helicopterPewPewPewDistance >= 88.0f)
                        helicopterPewPewPewDistance = 0.0f;
                    glTranslatef(-helicopterPewPewPewDistance,0,0);
                    if(!isGlobalShadowOn || !shadow)
                        glColor4ub(255, 0, 0, 0);
                    glScalef(1, .7, .7);
                    glutSolidSphere(2,50,10);
                }
            glPopMatrix();

            glPushMatrix();
                if (isHelicopterLaserActive) {

                    glBegin(GL_LINES);
                        if(!isGlobalShadowOn || !shadow)
                            glColor4ub(255, 31, 31, 0);
                        glVertex3f( -350.0f, 0.0f, -0.0f ); glVertex3f( 0.0f, 0.0f, -0.0f ); //Línea Norte a Sur
                        glVertex3f( -350.0f, 0.0f,  30.0f ); glVertex3f( 0.0f, 0.0f,  30.0f ); //Línea Norte a Sur
                    glEnd ();
                }
            glPopMatrix();
        glPopMatrix();

        //pata 2
        glPushMatrix();
            if(!isGlobalShadowOn || !shadow)
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
                if(!isGlobalShadowOn || !shadow)
                    glColor4ub(255, 0, 0, 0);
                glTranslatef(45, 00, 0); //x, y, z
                glScalef(1, .7, .7);
                glutSolidSphere(2,50,10);
            glPopMatrix();

            glPushMatrix();
                if(!isGlobalShadowOn || !shadow)
                    glColor4ub(255, 0, 0, 0);
                glScalef(1, .7, .7);
                glutSolidSphere(2,50,10);
                //bala

                if(isFaiaFaiaPewPewPew){
                    helicopterPewPewPewDistance += 0.9f;
                    if (helicopterPewPewPewDistance >= 88.0f)
                        helicopterPewPewPewDistance = 0.0f;
                    if(!isGlobalShadowOn || !shadow)
                        glColor4ub(255, 0, 0, 0);
                    glTranslatef(-helicopterPewPewPewDistance, 0, 0); //x, y, z
                    glScalef(1, .7, .7);
                    glutSolidSphere(2,50,10);
                }
            glPopMatrix();
        glPopMatrix();
        //========================
    glPopMatrix();
    gluDeleteQuadric(Cabina);

}

void drawTeapot(float x, float y, float z, bool shadow){
glPushMatrix();
    glTranslatef(0,-6.4,0);
    glTranslatef(x,y,z);
    if(!isGlobalShadowOn || !shadow)
        glColor4ub(200,200,200,0);
    glutSolidTeapot(1);
glPopMatrix();
}

void drawTable(float x, float y, float z, bool shadow){
    GLUquadricObj *Cabina;
    Cabina = gluNewQuadric();
    gluQuadricDrawStyle(Cabina, GLU_FILL);
    gluQuadricNormals( Cabina, GLU_NONE); // For if lighting is to be used.


    glPushMatrix();
        glRotated(90,1,0,0);
        glTranslatef(0,0,3);
        glTranslatef(x,y,z);
        if(!isGlobalShadowOn || !shadow)
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

void drawPortrait(float x, float y, float z, bool shadow){
    GLUquadricObj *Cabina;
    Cabina = gluNewQuadric();
    gluQuadricDrawStyle(Cabina, GLU_FILL);
    gluQuadricNormals(Cabina, GLU_NONE);

    glPushMatrix();

        if(!isGlobalShadowOn || !shadow)
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

void drawCup(int x, int y, int z, float rotar, bool shadow){
    GLUquadricObj *Cabina;
    //int cylinderCount = 0;
    int increase = 4;
    int decrease = 4;
    int depth = 35;
    GLfloat margin = depth * .275;
    Cabina = gluNewQuadric();
    gluQuadricDrawStyle(Cabina, GLU_FILL);
    gluQuadricNormals( Cabina, GLU_NONE); // For if lighting is to be used.
    glEnable(GL_BLEND);

    glPushMatrix();
        glScalef(.009,.009,.009);
        glRotatef(rotar, 1, 0, 0);
        if(rotar == 0)
            glTranslatef(0,-820, -550);
        if(!isGlobalShadowOn || !shadow)
            glColor4ub(255, 255, 255, 150);
        //========================================Dibuja la base de la helice trasera
        glTranslatef(50,100, 600);
        glTranslatef(x,y, z);
        glPushMatrix();
//1
            gluCylinder(Cabina, depth - increase, depth, 10, MODEL_EDGE, 10);
//2
            depth += increase;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth - increase , depth, 10, MODEL_EDGE, 10);
//3
            depth += increase*.5;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth - increase*.5, depth, 10, MODEL_EDGE, 10);
//4
            depth -= decrease;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease, depth + decrease, 10, MODEL_EDGE, 10);
//5
            depth -= decrease;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease*2, depth + decrease, 10, MODEL_EDGE, 10);

//6
            depth -= decrease * 1.5;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease*2.5, depth + decrease, 10, MODEL_EDGE, 10);
//7
            depth -= decrease * 1.5;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease*2.5, depth + decrease, 10, MODEL_EDGE, 10);
//8
            depth -= decrease * 2;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease*3, depth + decrease, 10, MODEL_EDGE, 10);
//9
            depth -= decrease * 2;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease*3, depth + decrease, 10, MODEL_EDGE, 10);
//10
            depth -= decrease * 1.5;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease*2.5, depth + decrease, 10, MODEL_EDGE, 10);
//11 Agarre
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease , depth + decrease, 40, MODEL_EDGE, 10);

//12 apoyo
            depth += increase * 1.5;
            glTranslatef(0, 0, margin+30); //x, y, z
            gluCylinder(Cabina, depth - increase*.5, depth + increase*5, 30, MODEL_EDGE, 10);
//13
            depth += increase * 1.5;
            glTranslatef(0, 0, margin+15); //x, y, z
            gluCylinder(Cabina, 1, depth + increase*3.4, 5, MODEL_EDGE, 10);

        glPopMatrix();

    glPopMatrix();
    glDisable(GL_BLEND);
    gluDeleteQuadric(Cabina);

}

void drawTrompo(int x, int y, int z, bool shadow){
    GLUquadricObj *Cabina;
    int increase = 4;
    int decrease = 4;
    int depth = 35;
    GLfloat margin = depth * .275;
    int inclina = trompoInclination > 25 ? trompoInclination * 3.5 : trompoInclination * 3;
    Cabina = gluNewQuadric();
    gluQuadricDrawStyle(Cabina, GLU_FILL);
    gluQuadricNormals( Cabina, GLU_NONE); // For if lighting is to be used.

    glPushMatrix();
        glTranslatef(x, y, z);
        glRotatef(90, 1, 0, 0);
        glScalef(.009,.009,.009);
        glRotatef(-trompoRotation, 0, 0, 1);
        glRotatef(trompoInclination, 1, 0, 0);
        glTranslatef(7,-6.4,665 + inclina);

        glPushMatrix();
//Parte de arriba
            glPushMatrix();
//sup
            if(!isGlobalShadowOn || !shadow)
                glColor4ub(70, 30, 0, 0);
            glTranslatef(0, 0, margin*-2); //x, y, z
            gluCylinder(Cabina, 1, depth*.3, 8, MODEL_EDGE, 1);
//bot
            if(!isGlobalShadowOn || !shadow)
                glColor4ub(170, 62, 1, 0);
            glTranslatef(0, 0, margin*.8); //x, y, z
            gluCylinder(Cabina, depth*.3 , depth*.3, 12, MODEL_EDGE, 1);
            glPopMatrix();
//Tapa
            glPushMatrix();
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D,texturas[TEXTURE_STAR].ID);
                gluQuadricTexture(Cabina, GL_TRUE);
                if(!isGlobalShadowOn || !shadow)
                    glColor4ub(255, 255, 0, 0); //Amarillo
                gluDisk(Cabina, 0, depth - increase, 5, 1);
                glDisable(GL_TEXTURE_2D);
            glPopMatrix();

//Forma de copa
//1
            if(!isGlobalShadowOn || !shadow)
                glColor4ub(130, 82, 1, 0);
            gluCylinder(Cabina, depth - increase, depth, 10, MODEL_EDGE, 1);
//2
            if(!isGlobalShadowOn || !shadow)
                glColor4ub(125, 77, 1, 0);
            depth += increase;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth - increase , depth, 10, MODEL_EDGE, 1);
//3
            if(!isGlobalShadowOn || !shadow)
                glColor4ub(130, 82, 1, 0);
            depth += increase*.5;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth - increase*.5, depth, 10, MODEL_EDGE, 1);
//4
            if(!isGlobalShadowOn || !shadow)
                glColor4ub(130, 82, 1, 0);
            depth -= decrease;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease, depth + decrease, 10, MODEL_EDGE, 1);
//5
            if(!isGlobalShadowOn || !shadow)
                glColor4ub(125, 77, 1, 0);
            depth -= decrease;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease*2, depth + decrease, 10, MODEL_EDGE, 1);

//6
            if(!isGlobalShadowOn || !shadow)
                glColor4ub(125, 77, 1, 0);
            depth -= decrease * 1.5;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease*2.5, depth + decrease, 10, MODEL_EDGE, 1);
//7
            if(!isGlobalShadowOn || !shadow)
                glColor4ub(130, 82, 1, 0);
            depth -= decrease * 1.5;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease*2.5, depth + decrease, 10, MODEL_EDGE, 1);
//8
            if(!isGlobalShadowOn || !shadow)
                glColor4ub(120, 77, 1, 0);
            depth -= decrease * 2;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease*3, depth + decrease, 10, MODEL_EDGE, 1);
//9
            if(!isGlobalShadowOn || !shadow)
                glColor4ub(120, 77, 1, 0);
            depth -= decrease * 2;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease*3, depth + decrease, 10, MODEL_EDGE, 1);

//10
            if(!isGlobalShadowOn || !shadow)
                glColor4ub(120, 77, 1, 0);
            depth -= decrease * 1.5;
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease*2.5, depth + decrease, 10, MODEL_EDGE, 1);
//11 Agarre
            if(!isGlobalShadowOn || !shadow)
                glColor4ub(200, 200, 200, 0);
            glTranslatef(0, 0, margin); //x, y, z
            gluCylinder(Cabina, depth + decrease , depth + decrease, 10, MODEL_EDGE, 1);

//12 apoyo
            if(!isGlobalShadowOn || !shadow)
                glColor4ub(200, 200, 200, 0);
            depth += increase * 1.5;
            glTranslatef(0, 0, margin); //x, y, z
            glutSolidSphere(depth, MODEL_EDGE, 4);

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
                    glColor4ub(25, 25, 112,sceneryTransparency);
                }else{
                    glColor4ub(112, 112, 112,sceneryTransparency);
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

void drawChair (bool shadow) {

   GLUquadricObj *qobj; //Define el objeto
   qobj = gluNewQuadric();
   gluQuadricDrawStyle(qobj, GLU_FILL); //Se dibuja un trompo
   gluQuadricNormals( qobj, GLU_NONE); // For if lighting is to be used.
    if(!isGlobalShadowOn || !shadow)
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

void drawUmbrella (bool shadow) {

   GLUquadricObj *qobj;
   qobj = gluNewQuadric();
   gluQuadricDrawStyle(qobj, GLU_FILL);
   gluQuadricNormals( qobj, GLU_NONE);

   glPushMatrix();
     glTranslatef(0, -3.7 ,0);
     glRotatef(45, 1, 0, 0);
     if(!isGlobalShadowOn || !shadow)
        glColor4ub(255, 0, 0, 0); //Capa roja
     gluCylinder(qobj, 0, 3.9, 1, 16, 1);
     if(!isGlobalShadowOn || !shadow)
        glColor4ub(100, 100, 100, 0); //Palo gris
     gluCylinder(qobj, 0.05, 0.05, 4.7, 10, 1);
   glPopMatrix();

   glPushMatrix();
    gluQuadricDrawStyle(qobj, GLU_LINE);
    if(!isGlobalShadowOn || !shadow)
        glColor4ub(20, 20, 20, 0); //varillas
     glTranslatef(0, -3.8 ,0);
     glRotatef(45, 1, 0, 0);
     gluCylinder(qobj, 0, 3.97, 1, 16, 1);
   glPopMatrix();

   gluDeleteQuadric(qobj);

}

void drawEveryObject(bool shadow){
    drawPersonWoodenModel(6.5,0,5,1,shadow);
    drawBall(0, 0, -5, 1, shadow);
    drawUmbrella(shadow);
    drawTeapot(-2, 0, 2, shadow);
    drawTrompo(-4, 0, -5, shadow);//izq-derecha, arriba-abajo, frente-atras
    drawChair(shadow);
    drawCup(40,00,-200,90, shadow);
    drawCup(200,0,0,0, shadow);
    drawTable(3,5,0, shadow);
    drawPortrait(1,3,5, shadow);
    drawHelicopter(0, 0, 0, helicopterScale, shadow);

}

bool light0Shadow(){

    if (isLight0On){
       glDisable( GL_LIGHTING );
       glDisable( GL_DEPTH_TEST );

        if ( isStencilOn ){
            glEnable( GL_STENCIL_TEST );
            glStencilFunc( GL_EQUAL, 0x1, 0xffffffff );
            glStencilOp( GL_KEEP, GL_KEEP, GL_ZERO );
        }

        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glColor4f( 0.0, 0.0, 0.0, shadowIntensity ); //color de la sombra

        glPushMatrix();
            //glTranslatef(0, BOX_HEIGHT, 0);
            shadowTransform( shadow_plane, light0Position );
            drawEveryObject(1);
        glPopMatrix();

        glDisable( GL_BLEND );
        glDisable( GL_STENCIL_TEST );
        glEnable( GL_DEPTH_TEST );
        glEnable( GL_LIGHTING );
        return true;
    }else{
        return false;
    }
}

bool light1Shadow(){

    if (isLight1On){
       glDisable( GL_LIGHTING );
       glDisable( GL_DEPTH_TEST );

        if ( isStencilOn ){
            glEnable( GL_STENCIL_TEST );
            glStencilFunc( GL_EQUAL, 0x1, 0xffffffff );
            glStencilOp( GL_KEEP, GL_KEEP, GL_ZERO );
        }

        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        glColor4f( 0.0, 0.0, 0.0, shadowIntensity ); //color de la sombra

        glPushMatrix();
            //glTranslatef(0, BOX_HEIGHT, 0);
            shadowTransform( shadow_plane, light1Position );
            drawEveryObject(1);
        glPopMatrix();

        glDisable( GL_BLEND );
        glDisable( GL_STENCIL_TEST );
        glEnable( GL_DEPTH_TEST );
        glEnable( GL_LIGHTING );
        return true;
    }else{
        return false;
    }

}

void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, light0Diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, light0Specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 65.0f);

    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColorWhite);
    glLightfv(GL_LIGHT0, GL_POSITION, light0Position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light0ModelIntensity);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, light0SpotDirection);
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0);

    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColorBlue);
    glLightfv(GL_LIGHT1, GL_POSITION, light1Position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light1ModelIntensity);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, light1SpotDirection);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);

    if(light0Intensity > 0){
        glEnable(GL_LIGHT0);
        isLight0On = true;
    }else{
        glDisable(GL_LIGHT0);
        isLight0On = false;
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0, 0, -40);
    glRotatef(30, 1, 0, 0);

    glScalef(sceneryZoom, sceneryZoom, sceneryZoom); //Zoom del escenario
    glRotatef(sceneryMouseSpinX, 1, 0, 0); //movimiento x del escenario
    glRotatef(sceneryMouseSpinY, 0, 1, 0); //movimiento y del escenario
    if (isSceneryRotating) glRotatef(sceneryRotation, 0, 1, 0);

    glLightfv(GL_LIGHT0, GL_POSITION, light0Position);

    drawLightBulb(light0Position[0],light0Position[1],light0Position[2],1);

    if ( isGlobalShadowOn && isStencilOn ){
       glEnable( GL_STENCIL_TEST );
       glStencilFunc( GL_ALWAYS, 0x1, 0xffffffff );
       glStencilOp( GL_REPLACE, GL_REPLACE, GL_REPLACE );
    }


    drawFloor();


    if(isGlobalShadowOn){
        light0Shadow();
        light1Shadow();
    }

    glPushMatrix(); //objetos
        glTranslatef(0, BOX_HEIGHT, 0);
        drawEveryObject(0);
    glPopMatrix();


    glPushMatrix(); //objetos reflejados
        glScalef(1, -1, 1);
        glTranslatef(0, BOX_HEIGHT, 0);
        drawEveryObject(0);
    glPopMatrix();


  glDisable(GL_STENCIL_TEST);
  glEnable(GL_BLEND);
  drawFloor();
  glDisable(GL_BLEND);

  glutSwapBuffers();
}

void animate(void) {
    //printf("trompoRotation: %.6f \n", trompoRotation);
    GLfloat auxSpeed = 0;
    GLfloat BallRadians = 0;
    static float theta  = 0.0;       // angle of light
    static float dtheta = 0.1 / 10.0; // angle step for light
    float dalpha = 4.0;      // angle step (in degrees) for object

    sceneryRotation += 0.29;
    if(isHelicopterCommandActive){
        helicopterPropellerRotation += 0.69;
    }
    if(isTrompoSpinning){
        trompoInclination > 0 ? trompoInclination -= .0308: trompoInclination;
        trompoRotation += 0.69;
    }
    //Inicio movimiento rotación helicptero

    if (isHelicopterPropellerImpulsing){
        helicopterPropellerImpulse += .004;
        if(helicopterPropellerImpulse >= .9999)
            isHelicopterPropellerImpulsing = false;
    }
    if(!isHelicopterPropellerSpinning && helicopterPropellerImpulse > 0.0){
        helicopterPropellerImpulse -= 0.005;
        //trompoRotation -= 0.69;
       // printf("%f\n", helicopterPropellerImpulse);
        if(helicopterPropellerImpulse <= 0)
            helicopterPropellerImpulse = 0.0;
    }//else trompoRotation += 0.69;

    //fin movimiento rotación helicptero
    if (trompoRotation >= 360)  trompoRotation = 0;
    if (trompoRotation < 0)  trompoRotation = 360;
    if (helicopterPropellerRotation >= 360)  helicopterPropellerRotation = 0;
    if (helicopterPropellerRotation < 0)  helicopterPropellerRotation = 360;
    if (sceneryRotation >= 360)  sceneryRotation = 0;
    //--------------Inicio movimiento de la bola ------------
    if (BallSpeed >= 0 && isBallJumping){
        ballPositionY += .07 * BallSpeed;
        ballRotation += 8 * BallSpeed;
        auxSpeed = BallSpeed - .3;
        if (ballRotation >= 360)  ballRotation = 0;
        if(auxSpeed >= 0 && ballRotation >= 0 && ballRotation <= 180){
            BallRadians = (ballRotation*PI/180) * .5 ;
            ballPositionZ = -sin(BallRadians)* ballJumpHeight * auxSpeed ;
        }
        else if(auxSpeed >= 0){
                BallRadians = (PI - (ballRotation-180) * PI/180) * .5;
                ballPositionZ = -sin(BallRadians) * ballJumpHeight * auxSpeed ;
             }

        BallSpeed -= .002;
    }
    //--------------Fin movimiento de la bola ------------

    theta += dtheta;
    //light0Position[0] = 10.0 * sin( theta ); //movimiento de prueba para las sombras
    //light0Position[1] = 10.0 * cos( theta );//movimiento de prueba para las sombras
    if ( theta > M_PI / 2.0 || theta < -M_PI / 2.0 ) dtheta = -dtheta;
    shadowAlpha += dalpha;

    glutPostRedisplay(); //Vuelve a dibujar
}

void spinDisplay(void) {

   _angle += 0.002;
   if (_angle > 360) {
       _angle -= 360;
   }

   glutPostRedisplay(); //Vuelve a dibujar
}

void handleSpecialKeypress(int key, int x, int y) {

 switch (key) {
    case GLUT_KEY_LEFT:
         isLeftKeyPressed = true;
         if (!isRightKeyPressed) {
             if(rX > -25)
                --rX;
         }
    break;

    case GLUT_KEY_RIGHT:
         isRightKeyPressed = true;
         if (!isLeftKeyPressed) {
             if (rX < 25)
                ++rX;
         }
    break;

    case GLUT_KEY_UP:
         isUpKeyPressed = true;
         if (!isDownKeyPressed) {
             if (rY < 25)
                ++rY;
         }
    break;

    case GLUT_KEY_DOWN:
         isDownKeyPressed = true;
         if (!isUpKeyPressed) {
             if (rY > 0)
                --rY;
         }
    break;
 }
}

void handleSpecialKeyReleased(int key, int x, int y) {
 switch (key) {
 case GLUT_KEY_LEFT:
      isLeftKeyPressed = false;
 break;

 case GLUT_KEY_RIGHT:
      isRightKeyPressed = false;
 break;

 case GLUT_KEY_UP:
      isUpKeyPressed = false;
 break;

 case GLUT_KEY_DOWN:
      isDownKeyPressed = false;
 break;

 }
}

void init() {
    FILE *comandos;
    GLint iMultiSample = 0;
    GLint iNumSamples = 0;
    glClearColor(0, 0, 0, 0); //Fondo negro en toda la escena
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_NORMALIZE);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //Inicio AA
    glEnable(GL_MULTISAMPLE);
    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_SRC_ALPHA_SATURATE, GL_ONE);

    glDepthFunc( GL_LESS );
    glShadeModel ( GL_SMOOTH );

       // detect current settings
    glGetIntegerv(GL_SAMPLE_BUFFERS, &iMultiSample);
    glGetIntegerv(GL_SAMPLES, &iNumSamples);
    printf("MSAA on, GL_SAMPLE_BUFFERS = %d, GL_SAMPLES = %d\n", iMultiSample, iNumSamples);
    //fin AA
    //cargar textura

    if(!cargarTGA("E:/graficos/proyecto cuarto v2/Bola.tga", &texturas[TEXTURE_BALL]) )
     printf("Error cargando textura de estrella\n");
    if(!cargarTGA("E:/graficos/proyecto cuarto v2/foto.tga", &texturas[TEXTURE_PHOTO]) )
     printf("Error cargando textura de foto\n");
    if(!cargarTGA("E:/graficos/proyecto cuarto v2/floor.tga", &texturas[TEXTURE_FLOOR]) )
     printf("Error cargando textura de pizo\n");
    if(!cargarTGA("E:/graficos/proyecto cuarto v2/Star.tga", &texturas[TEXTURE_STAR]) )
     printf("Error cargando textura de estrella\n");

    if((comandos = fopen("E:/graficos/proyecto cuarto v2/planvuelo.txt", "r")) == NULL)
     printf("Error cargando el plan de vuelo\n");

    GLint counter = 0;
    while (fscanf(comandos, "%c %f", &helicopterCommands[counter], &helicopterDirections [counter]) != EOF) counter++;
    fclose(comandos);
}

void handleResize(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}

void mouse(int button, int state, int x, int y) {
   switch (button) {
      case GLUT_LEFT_BUTTON:
         if (state == GLUT_DOWN) {
            glutIdleFunc(animate);
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
     sceneryMouseSpinX = y;
     sceneryMouseSpinY = x;
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutSetOption(GLUT_MULTISAMPLE, 16);
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
