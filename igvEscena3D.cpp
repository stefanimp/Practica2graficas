#include <cstdlib>
#include <stdio.h>
#include <cmath>
#include <fstream>
#include <sstream>
#include "vector"
#include <iostream>

#include "igvEscena3D.h"
#include "igvCilindro.h" // Se mantiene por si usas cilindros en la malla

// Constructor
igvEscena3D::igvEscena3D(){
    rotX = 0; rotY = 0; rotZ = 0;

    // Inicialización Grúa Torre
    anguloTorre = 0.0;
    posicionCarro = 2.0; // Empieza a mitad de la pluma
    longitudCable = 1.5;

    // Crear cilindro (Apartado B)
    malla = new igvCilindro(1,1,40,5);

    try {
        modeloImportado = cargarOBJ("../airplane.obj");
    } catch (const std::exception& e) {
        // Silenciamos error si no existe para que no moleste en la grúa
    }
}

igvEscena3D::~igvEscena3D() {
    if (malla != nullptr) { delete malla; malla = nullptr; }
    if (modeloImportado != nullptr) { delete modeloImportado; modeloImportado = nullptr; }
}

void igvEscena3D::pintar_ejes() {
    GLfloat rojo[] = {1, 0, 0, 1.0};
    GLfloat verde[] = {0, 1, 0, 1.0};
    GLfloat azul[] = {0, 0, 1, 1.0};

    glMaterialfv(GL_FRONT, GL_EMISSION, rojo);
    glBegin(GL_LINES); glVertex3f(1000, 0, 0); glVertex3f(-1000, 0, 0); glEnd();

    glMaterialfv(GL_FRONT, GL_EMISSION, verde);
    glBegin(GL_LINES); glVertex3f(0, 1000, 0); glVertex3f(0, -1000, 0); glEnd();

    glMaterialfv(GL_FRONT, GL_EMISSION, azul);
    glBegin(GL_LINES); glVertex3f(0, 0, 1000); glVertex3f(0, 0, -1000); glEnd();
}

void igvEscena3D::visualizar(void) {
    GLfloat luz0[4] = {5, 5, 5, 1};
    glLightfv(GL_LIGHT0, GL_POSITION, luz0);
    glEnable(GL_LIGHT0);

    if (ejes) pintar_ejes();

    // --- TRANSFORMACIÓN GLOBAL ---
    glPushMatrix();
    glRotatef(rotX, 1, 0, 0);
    glRotatef(rotY, 0, 1, 0);
    glRotatef(rotZ, 0, 0, 1);

    // DIBUJAR LA GRÚA
    pintar_grua();

    glPopMatrix();
}

bool igvEscena3D::get_ejes() { return ejes; }
void igvEscena3D::set_ejes(bool _ejes) { ejes = _ejes; }

// --- IMPLEMENTACIÓN GEOMÉTRICA DE LA GRÚA TORRE ---

void igvEscena3D::pintar_base() {
    glPushMatrix();
    GLfloat color_base[] = {0.5, 0.5, 0.5, 1.0}; // Gris hormigón
    glMaterialfv(GL_FRONT, GL_EMISSION, color_base);
    // Base rectangular grande en el suelo
    glScalef(2.0, 0.2, 2.0);
    glutSolidCube(1.0);
    glPopMatrix();
}

void igvEscena3D::pintar_torre_vertical() {
    glPushMatrix();
    GLfloat color[] = {1.0, 0.8, 0.0, 1.0}; // Amarillo grúa
    glMaterialfv(GL_FRONT, GL_EMISSION, color);
    // Mástil alto (6 unidades de alto)
    glScalef(0.5, 6.0, 0.5);
    glutSolidCube(1.0);
    glPopMatrix();
}

void igvEscena3D::pintar_pluma() {
    glPushMatrix();
    // 1. Pluma Delantera (Larga)
    glPushMatrix();
    GLfloat color[] = {1.0, 0.8, 0.0, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, color);
    glTranslatef(2.5, 0, 0); // Desplazada hacia adelante
    glScalef(5.0, 0.4, 0.4); // 5 unidades de largo
    glutSolidCube(1.0);
    glPopMatrix();

    // 2. Contrapluma (Trasera)
    glPushMatrix();
    glTranslatef(-1.0, 0, 0);
    glScalef(2.0, 0.4, 0.4);
    glutSolidCube(1.0);
    glPopMatrix();

    // 3. Contrapeso (Gris)
    glPushMatrix();
    GLfloat colorGris[] = {0.3, 0.3, 0.3, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, colorGris);
    glTranslatef(-1.5, -0.3, 0);
    glScalef(0.8, 0.6, 0.6);
    glutSolidCube(1.0);
    glPopMatrix();

    // 4. Cabina (Azulada)
    glPushMatrix();
    GLfloat colorCristal[] = {0.0, 0.5, 0.8, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, colorCristal);
    glTranslatef(0.4, 0.4, 0.4); // Al lado del mástil
    glutSolidCube(0.6);
    glPopMatrix();
    glPopMatrix();
}

void igvEscena3D::pintar_carro() {
    glPushMatrix();
    GLfloat color[] = {0.2, 0.2, 0.2, 1.0}; // Negro/Gris oscuro
    glMaterialfv(GL_FRONT, GL_EMISSION, color);
    glScalef(0.5, 0.3, 0.5); // Pequeño y plano
    glutSolidCube(1.0);
    glPopMatrix();
}

void igvEscena3D::pintar_gancho() {
    // 1. El Cable (Línea)
    glDisable(GL_LIGHTING);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);              // Desde el carro
    glVertex3f(0, -longitudCable, 0); // Hacia abajo
    glEnd();
    glEnable(GL_LIGHTING);

    // 2. El Gancho (Bloque rojo en la punta)
    glPushMatrix();
    glTranslatef(0, -longitudCable, 0);
    GLfloat color[] = {0.8, 0.0, 0.0, 1.0};
    glMaterialfv(GL_FRONT, GL_EMISSION, color);
    glutSolidCube(0.4);
    glPopMatrix();
}

// --- ENSAMBLAJE DE LA JERARQUÍA ---
void igvEscena3D::pintar_grua() {
    // NIVEL 1: Base (Suelo)
    pintar_base();

    // NIVEL 1.5: Torre Vertical
    // Subimos 3 unidades (mitad de su altura 6) para que apoye en el suelo (y=0)
    glPushMatrix();
    glTranslatef(0, 3.0, 0);
    pintar_torre_vertical();

    // Nos movemos a la cima de la torre (otra vez 3 hacia arriba desde el centro)
    glTranslatef(0, 3.0, 0);

    // NIVEL 2: Parte Giratoria (Pluma) -> GRADO LIBERTAD 1
    glRotatef(anguloTorre, 0, 1, 0);
    pintar_pluma();

    // NIVEL 3: El Carro -> GRADO LIBERTAD 2
    // El carro es hijo de la pluma. Se mueve en X local.
    glPushMatrix();
    glTranslatef(posicionCarro, -0.2, 0); // Posición dinámica en X
    pintar_carro();

    // NIVEL 4: El Gancho -> GRADO LIBERTAD 3
    // El gancho es hijo del carro. Cuelga en Y local.
    pintar_gancho();

    glPopMatrix(); // Fin Carro+Gancho

    glPopMatrix(); // Fin Torre
}

// ... Métodos de carga OBJ y malla se mantienen igual ...
igvMallaTriangulos* igvEscena3D::cargarOBJ(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Error OBJ");
    // (Implementación simplificada para brevedad, mantener la tuya original si funciona)
    return nullptr;
}
void igvEscena3D::pintar_malla_de_triangulos() {
    // Implementación vacía o tu lógica
}