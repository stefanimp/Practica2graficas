#include "caja.h"
#include <iostream>
#include <GL/glut.h>

igvCaja::igvCaja(float r, float g, float b) : color{r, g, b}, valor(false), posx(0.0f), posz(0.0f), altura(0.0f)
{
    // Convertir a bytes 0-255 para picking
    colorUByte[0] = static_cast<GLubyte>(color[0] * 255);
    colorUByte[1] = static_cast<GLubyte>(color[1] * 255);
    colorUByte[2] = static_cast<GLubyte>(color[2] * 255);
}

igvCaja::igvCaja(const igvCaja &orig)
    : color{orig.color[0], orig.color[1], orig.color[2]},
      colorUByte{orig.colorUByte[0], orig.colorUByte[1], orig.colorUByte[2]},
      valor(orig.valor),
      posx(orig.posx),
      posz(orig.posz),
      altura(orig.altura)
{
}


void igvCaja::visualizar()
{
    glPushMatrix();

    // Usar color de selección si está seleccionada
    if (valor) {
        GLfloat colorSeleccion[3] = {0.30f, 0.9f, 0.7f};
        glMaterialfv(GL_FRONT, GL_EMISSION, colorSeleccion);
        glColor3fv(colorSeleccion);
    } else {
        glMaterialfv(GL_FRONT, GL_EMISSION, color);
        glColor3fv(color);
    }

    // Posición dinámica
    glTranslatef(posx, 0.5f + posy, posz);

    // Escala por defecto de la caja
    glScalef(1.8f, 2.0f, 3.5f);
    glutSolidCube(1.0f);

    glPopMatrix();
}

/**
 * Getters y setters
 */
float* igvCaja::getColor() { return color; }
GLubyte* igvCaja::getColorByte() { return colorUByte; }

void igvCaja::set_color(float r, float g, float b)
{
    color[0] = r;
    color[1] = g;
    color[2] = b;

    // Actualizar versión en bytes
    colorUByte[0] = static_cast<GLubyte>(r * 255);
    colorUByte[1] = static_cast<GLubyte>(g * 255);
    colorUByte[2] = static_cast<GLubyte>(b * 255);
}

bool igvCaja::seleccionada1() { return valor; }
void igvCaja::set_valor(bool v) { valor = v; }

float igvCaja::posx1() { return posx; }
float igvCaja::posz1() { return posz; }

void igvCaja::set_posx(float x) { posx = x; }
void igvCaja::set_posz(float z) { posz = z; }
void igvCaja::set_posy(float y) {
    posy = y;
}
void igvCaja::mover(float dx,float dy, float dz)
{
    posx += dx;
    posy+=dy;
    posz += dz;
}


// En igvCaja.h


// En igvCaja.cpp
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void igvCaja::situarCircular(float anguloGrados, float radio, float altura) {
    // Convertir grados a radianes
    float radianes = anguloGrados * (M_PI / 180.0f);

    // Fórmula mágica: Polar a Cartesiano
    this->posx = radio * cos(radianes);
    this->posz = -radio * sin(radianes); // El negativo suele ser necesario en OpenGL para el eje Z

    // Ajustamos la altura (sumamos 0.5 si el origen de la caja está en su centro y mide 1.0)
    this->posy = altura;
}
