#ifndef __IGVESCENA3D
#define __IGVESCENA3D

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#endif

#include "igvMallaTriangulos.h"

class igvEscena3D
{
private:
    // Atributos
    bool ejes = true;
    double rotX = 0, rotY = 0, rotZ = 0;

    igvMallaTriangulos* malla = nullptr;
    igvMallaTriangulos* modeloImportado = nullptr;

    // --- ATRIBUTOS DE LA GRÚA TORRE ---
    double anguloTorre = 0;      // Rotación de la pluma (Grado 1)
    double posicionCarro = 2.0;  // Desplazamiento del carro (Grado 2)
    double longitudCable = 1.0;  // Elevación del gancho (Grado 3)

    // --- MÉTODOS INTERNOS DE DIBUJADO ---
    void pintar_ejes();
    igvMallaTriangulos* cargarOBJ(const std::string& filename);
    void pintar_malla_de_triangulos();

    // Métodos específicos de la Grúa Torre
    void pintar_base();
    void pintar_torre_vertical(); // El mástil
    void pintar_pluma();          // Brazo horizontal y contrapeso
    void pintar_carro();          // La pieza que se mueve horizontalmente
    void pintar_gancho();         // El cable y el gancho
    void pintar_grua();           // Método que ensambla todo

public:
    // Constructores y destructor
    igvEscena3D();
    ~igvEscena3D();

    // Métodos públicos
    void visualizar();
    bool get_ejes();
    void set_ejes(bool _ejes);
    igvMallaTriangulos* getMalla() const { return malla; }

    // Rotación de la escena completa
    void incrX() { rotX += 10; }
    void incrY() { rotY += 10; }
    void incrZ() { rotZ += 10; }
    void decrX() { rotX -= 10; }
    void decrY() { rotY -= 10; }
    void decrZ() { rotZ -= 10; }

    // --- MÉTODOS PÚBLICOS DE CONTROL DE LA GRÚA (Con límites) ---

    // Grado 1: Rotación Torre (Giro infinito)
    void cambiarAnguloTorre(double inc) {
        anguloTorre += inc;
    }

    // Grado 2: Traslación Carro (Límite: no salirse de la pluma)
    void cambiarPosicionCarro(double inc) {
        double nuevaPos = posicionCarro + inc;
        // Límites: 1.0 (cerca del mástil) a 4.5 (punta de la pluma)
        if (nuevaPos > 1.0 && nuevaPos < 4.5) {
            posicionCarro = nuevaPos;
        }
    }

    // Grado 3: Longitud Cable (Límite: no chocar suelo ni subir demasiado)
    void cambiarLongitudCable(double inc) {
        double nuevaLong = longitudCable + inc;
        // Límites: 0.5 (arriba) a 5.0 (cerca del suelo)
        if (nuevaLong > 0.5 && nuevaLong < 5.0) {
            longitudCable = nuevaLong;
        }
    }
};

#endif