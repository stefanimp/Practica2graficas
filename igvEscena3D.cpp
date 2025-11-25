 #include <cstdlib>
#include <stdio.h>
#include <cmath>
#include <fstream>
#include <sstream>
#include "vector"
#include "igvCilindro.h"
#include <iostream>
#include "igvEscena3D.h"
#include "igvMallaTriangulos.h"


// M�todos constructores

/**
 * Constructor por defecto
 */
igvEscena3D::igvEscena3D(){

    anguloTorre = 0.0;
    anguloBrazo = 0.0;
    anguloAntebrazo = 0.0;
    rotX = 0.0;
    rotY = 0.0;
    rotZ = 0.0;
    // TODO: Apartado B: Inserta el c�digo para crear un cilindro
    malla = new igvCilindro(1,1,40,5);

    try {
        modeloImportado = cargarOBJ("../airplane_triangles.obj");
        std::cout << "Modelo cargado correctamente." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error al cargar el archivo: " << e.what() << std::endl;
    }
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


// M�todos p�blicos

/**
 * M�todo para pintar los ejes coordenados llamando a funciones de OpenGL
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
 * M�todo con las llamadas OpenGL para visualizar la escena
 */
// En igvEscena3D.cpp

 void igvEscena3D::visualizar(void)
 {
     // ... (Luces y Ejes se quedan igual) ...
     GLfloat luz0[4] = {5, 5, 5, 1};
     glLightfv(GL_LIGHT0, GL_POSITION, luz0);
     glEnable(GL_LIGHT0);

     if (ejes) pintar_ejes();

     // --- TRANSFORMACIÓN GLOBAL DE LA ESCENA ---
     glPushMatrix();
     // Aplicamos las rotaciones globales controladas por teclado (Apartado A del PDF)
     glRotatef(rotX, 1, 0, 0);
     glRotatef(rotY, 0, 1, 0);
     glRotatef(rotZ, 0, 0, 1);

     // --- AQUÍ DIBUJAMOS TU GRÚA ---
     pintar_grua();

     glPopMatrix(); // Restaurar matriz
 }

/**
 * M�todo para consultar si hay que dibujar los ejes o no
 * @retval true Si hay que dibujar los ejes
 * @retval false Si no hay que dibujar los ejes
 */
bool igvEscena3D::get_ejes()
{
    return ejes;
}

/**
 * M�todo para activar o desactivar el dibujado de los _ejes
 * @param _ejes Indica si hay que dibujar los ejes (true) o no (false)
 * @post El estado del objeto cambia en lo que respecta al dibujado de ejes,
 *       de acuerdo al valor pasado como par�metro
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



    // GRUA
    // Método auxiliar para pintar una sola rueda
    void igvEscena3D::pintar_rueda() {
        glPushMatrix();
        // El cilindro se dibuja a lo largo del eje Z por defecto.
        // Lo rotamos 90 grados o lo escalamos según necesitemos.
        // Asumiremos una rueda negra.
        GLfloat color_rueda[] = {0.2, 0.2, 0.2, 1.0};
        glMaterialfv(GL_FRONT, GL_EMISSION, color_rueda);

        // Radio 0.25, ancho 0.2
        GLUquadric* q = gluNewQuadric();
        gluCylinder(q, 0.25, 0.25, 0.2, 20, 5);

        // Tapa frontal de la rueda (disco)
        glPushMatrix();
        glTranslatef(0, 0, 0.2);
        gluDisk(q, 0, 0.25, 20, 5);
        glPopMatrix();

        // Tapa trasera
        glPushMatrix();
        glRotatef(180, 1,0,0);
        gluDisk(q, 0, 0.25, 20, 5);
        glPopMatrix();

        gluDeleteQuadric(q);
        glPopMatrix();
    }

// Método auxiliar para pintar la base cuadrada
 void igvEscena3D::pintar_base() {
     glPushMatrix();
     GLfloat color_base[] = {0.8, 0.6, 0.0, 1.0}; // Color amarillo grúa
     glMaterialfv(GL_FRONT, GL_EMISSION, color_base);

     // Escalamos el cubo para que sea rectangular (largo, alto, ancho)
     glScalef(2.0, 0.5, 1.0);
     glutSolidCube(1.0);
     glPopMatrix();
 }
 void igvEscena3D::pintar_torre() {
     glPushMatrix();
     GLfloat color[] = {0.8, 0.6, 0.0, 1.0}; // Amarillo
     glMaterialfv(GL_FRONT, GL_EMISSION, color);
     // Una torre cilíndrica o cúbica
     glScalef(0.5, 1.0, 0.5);
     glutSolidCube(1.0);
     glPopMatrix();
 }

 void igvEscena3D::pintar_brazo() {
     glPushMatrix();
     GLfloat color[] = {0.8, 0.0, 0.0, 1.0}; // Rojo para distinguir
     glMaterialfv(GL_FRONT, GL_EMISSION, color);

     // Desplazamos para que el eje de rotación esté en el extremo, no en el centro
     glTranslatef(0.0, 1.0, 0.0);
     glScalef(0.4, 2.0, 0.4);
     glutSolidCube(1.0);
     glPopMatrix();
 }

 void igvEscena3D::pintar_antebrazo() {
     glPushMatrix();
     GLfloat color[] = {0.0, 0.8, 0.8, 1.0}; // Cian
     glMaterialfv(GL_FRONT, GL_EMISSION, color);

     glTranslatef(0.0, 0.75, 0.0);
     glScalef(0.3, 1.5, 0.3);
     glutSolidCube(1.0);
     glPopMatrix();
 }

// Método principal que ensambla la grúa
 void igvEscena3D::pintar_grua() {
     // 1. NIVEL 1: La Base y Ruedas (fijas en el suelo)
     pintar_base();

     // Ruedas (Hijas de la base)
     glPushMatrix(); glTranslatef(-0.8, -0.25, 0.5); pintar_rueda(); glPopMatrix();
     glPushMatrix(); glTranslatef(-0.8, -0.25, -0.7); pintar_rueda(); glPopMatrix();
     glPushMatrix(); glTranslatef(0.8, -0.25, 0.5); pintar_rueda(); glPopMatrix();
     glPushMatrix(); glTranslatef(0.8, -0.25, -0.7); pintar_rueda(); glPopMatrix();

     // --- COMIENZO DE LA PARTE ARTICULADA ---

     glPushMatrix();
     // Subimos a la parte superior de la base
     glTranslatef(0.0, 0.5, 0.0); // 0.5 es la mitad de la altura de la base (aprox)

     // 2. NIVEL 2: La Torre (Grado de Libertad 1: Rotación Y)
     glRotatef(anguloTorre, 0, 1, 0);
     pintar_torre();

     // Ahora nos movemos al topo de la torre para poner el brazo
     glTranslatef(0.0, 0.5, 0.0);

     // 3. NIVEL 3: El Brazo (Grado de Libertad 2: Rotación Z - Elevación)
     glRotatef(anguloBrazo, 0, 0, 1);
     pintar_brazo();

     // Nos movemos al final del brazo (longitud del brazo es aprox 2.0, ver escalado en pintar_brazo)
     glTranslatef(0.0, 2.0, 0.0);

     // 4. NIVEL 4: El Antebrazo (Grado de Libertad 3: Rotación Z)
     glRotatef(anguloAntebrazo, 0, 0, 1);
     pintar_antebrazo();

     glPopMatrix(); // Fin de la parte articulada
 }
