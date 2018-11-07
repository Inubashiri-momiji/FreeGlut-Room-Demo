# FreeGlut Room
Freeglut openGL room modeling test

This project uses Freeglut for 3D object modeling using the native GLUT functions.

### Features:
  * Light source
  * Shadows
  * Automatic helicopter controls (using planvuelo.txt)
  * Helicopter lasers and bullets
  * Automatic room rotation
  * Ball jumping simulation
  * Spining "trompo" simulation

### Config:

This app requires manual linking of requeriment files in Main.cpp on init function, line 1562.

```C
    if(!cargarTGA("E:/graficos/proyecto cuarto v2/Bola.tga", &texturas[TEXTURE_BALL]) )
     printf("Error cargando textura de estrella\n");
    if(!cargarTGA("E:/graficos/proyecto cuarto v2/foto.tga", &texturas[TEXTURE_PHOTO]) )
     printf("Error cargando textura de foto\n");
    if(!cargarTGA("E:/graficos/proyecto cuarto v2/floor.tga", &texturas[TEXTURE_FLOOR]) )
     printf("Error cargando textura de pizo\n");
    if(!cargarTGA("E:/graficos/proyecto cuarto v2/Star.tga", &texturas[TEXTURE_STAR]) )
     printf("Error cargando textura de estrella\n");
```

Demo video: [Youtube](https://www.youtube.com/watch?v=ixirlkI1wqM)
