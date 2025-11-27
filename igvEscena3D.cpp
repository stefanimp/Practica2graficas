#include <cstdlib>
#include <stdio.h>
#include <cmath>
#include <fstream>
#include <sstream>
#include "vector"
#include <igvCilindro.h>
#include <iostream>

#include "igvEscena3D.h"

#include <memory>

#include "igvMallaTriangulos.h"


// Métodos constructores

/**
 * Constructor por defecto
 */
igvEscena3D::igvEscena3D()
{
    malla = new igvCilindro(1,1,40,5);

    try {
        modeloImportado = cargarOBJ("../tren_fin1.obj");
        std::cout << "Modelo cargado correctamente." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error al cargar el archivo: " << e.what() << std::endl;
    }

    animacionModelo = false;
    animacionCamara = false;
    posTrenZ = 0.0f;
    bajandoGancho = true;
    subiendoGancho = false;
    alturaCarga = 0.0f;
    cargado = false;
    soltando = false;
    alturaCaida = 0.0f;
    anguloTorre=0;
    cajaSeleccionada = -1;

}

/**
 * Destructor
 */
igvEscena3D::~igvEscena3D()
{
    if (malla != nullptr)
    {
        delete malla;
        malla = nullptr;
    }
}


// Métodos públicos

/**
 * Método para pintar los ejes coordenados llamando a funciones de OpenGL
 */
void igvEscena3D::pintar_ejes()
{
    GLfloat rojo[] = {1, 0, 0, 1.0};
    GLfloat verde[] = {0, 1, 0, 1.0};
    GLfloat azul[] = {0, 0, 1, 1.0};

    glMaterialfv(GL_FRONT, GL_EMISSION, rojo);
    glBegin(GL_LINES);
    glVertex3f(1000, 0, 0);
    glVertex3f(-1000, 0, 0);
    glEnd();

    glMaterialfv(GL_FRONT, GL_EMISSION, verde);
    glBegin(GL_LINES);
    glVertex3f(0, 1000, 0);
    glVertex3f(0, -1000, 0);
    glEnd();

    glMaterialfv(GL_FRONT, GL_EMISSION, azul);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 1000);
    glVertex3f(0, 0, -1000);
    glEnd();
}

/**
 * Método con las llamadas OpenGL para visualizar la escena
 */
void igvEscena3D::visualizar(int escena)
{
    GLfloat color_malla[] = {0, 0.25, 0};
    GLfloat luz0[4] = {5, 5, 5, 1};
    if (sombreadoSuave) {
        glShadeModel(GL_SMOOTH); // Interpolatorio (Gouraud)
    } else {
        glShadeModel(GL_FLAT);   // Plano (Faceteado)
    }
    glLightfv(GL_LIGHT0, GL_POSITION, luz0);
    glEnable(GL_LIGHT0);

    if (ejes) pintar_ejes();
    if (escena == EscenaA) {
        glPushMatrix();
        glScaled(0.5, 0.5, 0.5);
        pintar_grua(escena);

        if (soltando) {
            glPushMatrix();
            glTranslatef(-posicionCarro, 4.0f + alturaCaida, 0.0f);
            glRotatef(180, 0, 1, 0);
            glScaled(0.5, 0.5, 0.5);
            caja_.visualizar();
            glPopMatrix();
        }
        glPopMatrix();

        glPushMatrix();
        glScaled(0.2, 0.2, 0.2);
        glTranslatef(5.0f, 0.0f, 17.0f - posTrenZ);
        pintar_vagon(0.1f, 0.1f, 0.8f);
        glMaterialfv(GL_FRONT, GL_EMISSION, color_malla);
        if(modeloImportado != nullptr) {
            glPushMatrix();
            glTranslatef(0, 0, -4);
            glRotated(90, 0, 1, 0);
            glScaled(0.5, 0.5, 0.5);
            modeloImportado->visualizar();
            glPopMatrix();
        }
        glPopMatrix();
    }


    // ESCENA B
    if (escena == EscenaB) {
        glPushMatrix();
        glScaled(0.5, 0.5, 0.5);
        pintar_grua(escena);
        glPopMatrix();

        // Tren
        glPushMatrix();
        glScaled(0.2, 0.2, 0.2);
        glTranslatef(5.0f, 0.0f, 17.0f - posTrenZ);
        pintar_vagon(0.1f, 0.1f, 0.8f);

        // Modelo
        glMaterialfv(GL_FRONT, GL_EMISSION, color_malla);
        if(modeloImportado != nullptr) {
            glPushMatrix();
            glTranslatef(0, 0, -4);
            glRotated(90, 0, 1, 0);
            glScaled(0.5, 0.5, 0.5);
            modeloImportado->visualizar();
            glPopMatrix();
        }
        glPopMatrix();
    }

    // ESCENA C (MODIFICADA)
    if (escena == EscenaC) {

        // 1. Pintar Grúa (CON ILUMINACIÓN)
        glPushMatrix();
        glScaled(0.5, 0.5, 0.5);
        pintar_grua(escena);
        glPopMatrix();

        // 2. Pintar Vagón / Cajas (SIN ILUMINACIÓN para que el color sea puro)
        glPushMatrix();
        glScaled(0.2, 0.2, 0.2);
        glTranslatef(5.0f, 0.0f, 0);
        // 3. Pintar Modelo Importado (CON ILUMINACIÓN)
        glMaterialfv(GL_FRONT, GL_EMISSION, color_malla);
        if (modeloImportado != nullptr) {
            glPushMatrix();
            glTranslatef(0, 0, -4);
            glRotated(90, 0, 1, 0);
            glScaled(0.5, 0.5, 0.5);
            modeloImportado->visualizar();
            glPopMatrix();
        }


        glDisable(GL_LIGHTING);
        pintar_vagon(0.1f, 0.1f, 0.8f);
        glEnable(GL_LIGHTING);


        glPopMatrix();


    }
}
/**
 * Método para consultar si hay que dibujar los ejes o no
 * @retval true Si hay que dibujar los ejes
 * @retval false Si no hay que dibujar los ejes
 */
bool igvEscena3D::get_ejes()
{
    return ejes;
}

/**
 * Método para activar o desactivar el dibujado de los _ejes
 * @param _ejes Indica si hay que dibujar los ejes (true) o no (false)
 * @post El estado del objeto cambia en lo que respecta al dibujado de ejes,
 *       de acuerdo al valor pasado como parámetro
 */
void igvEscena3D::set_ejes(bool _ejes)
{
    ejes = _ejes;
}

/**
 * Método para importar archivos en formato OBJ
 * @param filename
 * @return
 */
igvMallaTriangulos* igvEscena3D::cargarOBJ(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo OBJ: " + filename);
    }

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
        }
        else if (prefix == "f") {
            unsigned int v1, v2, v3;
            std::string s1, s2, s3;
            iss >> s1 >> s2 >> s3;

            auto parseIndex = [](const std::string& s) {
                std::stringstream ss(s);
                unsigned int idx;
                ss >> idx;
                return idx - 1; // los OBJ empiezan en 1
            };

            v1 = parseIndex(s1);
            v2 = parseIndex(s2);
            v3 = parseIndex(s3);

            indices.push_back(v1);
            indices.push_back(v2);
            indices.push_back(v3);
        }
    }

    file.close();

    // Crear arrays planos para el constructor
    float* vertexArray = new float[vertices.size()];
    std::copy(vertices.begin(), vertices.end(), vertexArray);

    unsigned int* indexArray = new unsigned int[indices.size()];
    std::copy(indices.begin(), indices.end(), indexArray);

    long int numVertices = vertices.size() / 3;
    long int numTriangulos = indices.size() / 3;

    return new igvMallaTriangulos(numVertices, vertexArray,
                                  numTriangulos, indexArray);
}



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



void igvEscena3D::pintar_gancho(int escena) {
    // Cable
    if (escena==EscenaA || escena==EscenaB) {
        glDisable(GL_LIGHTING);
        glColor3f(0.0, 0.0, 0.0);
        glBegin(GL_LINES);
        glVertex3f(0, 0, 0);
        glVertex3f(0, -longitudCable, 0);
        glEnd();
        glEnable(GL_LIGHTING);

        // Gancho
        glPushMatrix();
        glTranslatef(0, -longitudCable, 0);

        GLfloat color[] = {0.8, 0.0, 0.0, 1.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, color);
        glutSolidCube(0.4);

        // LA CAJA ENGANCHADA
        // Solo visible cuando está cargada Y NO está soltando
        if (cargado && !soltando) {
            glPushMatrix();
            glTranslatef(0, -1.0, 0);
            glScaled(0.5,0.5,0.5);
            caja_.visualizar();  // Misma instancia caja_
            glPopMatrix();
        }

        glPopMatrix();
    }
    if (escena==EscenaC) {

    }
}

void igvEscena3D::pintar_grua(int escena) {
    if (escena=EscenaA|| escena==EscenaB) {
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
        pintar_gancho(escena);

        glPopMatrix(); // Fin Carro+Gancho

        glPopMatrix(); // Fin Torre
    }
    if (escena==EscenaC) {

        pintar_base();


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
        pintar_gancho(escena);

        glPopMatrix(); // Fin Carro+Gancho

        glPopMatrix(); // Fin Torre
    }
}




void igvEscena3D::pintar_vagon(float r, float g, float b) {
    glPushMatrix();

    // NOTA: Ignoramos los parámetros r, g, b para poder dar un color único a cada parte.

    // 1. Primera caja (la carga que se engancha) - COLOR ROJO
    // Solo visible cuando NO está cargada Y NO está soltando
    if (!cargado && !soltando) {
        glPushMatrix();
        glTranslatef(0, 0, 0);

        caja_.set_color(1.0f, 0.0f, 0.0f); // Rojo
        caja_.visualizar();

        glPopMatrix();
    }

    // 2. Segunda caja del vagón - COLOR VERDE
    glPushMatrix();
    glTranslatef(0, 0, 4.5);

    caja1.set_color(0.0f, 1.0f, 0.0f); // Verde
    caja1.visualizar();

    glPopMatrix();

    // 3. Tercera caja del vagón - COLOR AZUL
    glPushMatrix();
    glTranslatef(0, 0, 9);

    caja2.set_color(0.0f, 0.0f, 1.0f); // Azul
    caja2.visualizar();

    glPopMatrix();

    glPopMatrix();
}

void igvEscena3D::toggleAnimacionModelo() {
    animacionModelo = !animacionModelo;
}

void igvEscena3D::toggleAnimacionCamara() {
    animacionCamara = !animacionCamara;
}


// paso_simulacion() permanece igual
void igvEscena3D::paso_simulacion() {
    if (animacionModelo) {
        // FASE 1: BAJANDO
        if (bajandoGancho) {
            posTrenZ += 0.1f;
            if (posTrenZ > 40.0f) posTrenZ = 0.0f;

            cambiarLongitudCable(0.02);

            if (longitudCable > 4.5f) {
                bajandoGancho = false;
                subiendoGancho = true;
                cargado = true;
            }
        }
        // FASE 2: SUBIENDO
        else if (subiendoGancho) {
            cambiarLongitudCable(-0.05);
            if (longitudCable < 0.6f) {
                subiendoGancho = false;
            }
        }
        // FASE 4: SOLTANDO
        else if (soltando) {
            alturaCaida -= 0.05f;

            if (alturaCaida < -5.0f) {
                soltando = false;
                cargado = false;
                bajandoGancho = true;

                anguloTorre = 0.0f;
                posTrenZ = 0.0f;
                longitudCable = 1.0f;
                alturaCaida = 0.0f;
            }
        }
        // FASE 3: GIRANDO
        else {
            anguloTorre += 0.5f;

            if (anguloTorre >= 180.0f) {
                cargado = false;
                soltando = true;
                alturaCaida = 0.0f;
            }
        }
    }
}


void igvEscena3D::reset() {
    // 1. Reiniciar variables de la grúa
    anguloTorre = 0.0f;
    // Asumiendo que tienes una variable para esto (basado en tus getters/setters)
    // posicionCarro = 0.0f;
    longitudCable = 1.0f; // O el valor inicial que tuviera (ej. 4.0)

    // 2. Deseleccionar cajas
    cajaSeleccionada = -1;

    // 3. Reiniciar posición de las cajas (a 0 o donde deban empezar)
    caja_.set_posx(0); caja_.set_posz(0); caja_.set_posy(0);
    caja1.set_posx(0); caja1.set_posz(0); caja1.set_posy(0);
    caja2.set_posx(0); caja2.set_posz(0); caja2.set_posy(0);

    // 4. Reiniciar variables de animación del Tren (Escena A)
    posTrenZ = 0.0f;
    bajandoGancho = true;
    subiendoGancho = false;
    cargado = false;
    soltando = false;
    alturaCaida = 0.0f;
    alturaCarga = 0.0f;
}


void igvEscena3D::toggleSombreado() {
    sombreadoSuave = !sombreadoSuave;
    std::cout << "Sombreado: " << (sombreadoSuave ? "Suave (Gouraud)" : "Plano (Flat)") << std::endl;
}