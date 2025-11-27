#ifndef __IGVESCENA3D
#define __IGVESCENA3D

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else

#include <GL/glut.h>
#include "caja.h"
#endif // defined(__APPLE__) && defined(__MACH__)


#include "igvMallaTriangulos.h"

/**
 * Los objetos de esta clase representan escenas 3D para su visualizaci�n
 */
class igvEscena3D
{
private:
    // Atributos
    bool ejes = true; ///< Indica si hay que dibujar los _ejes coordenados o no

    // TODO: Apartado A: A�adir aqu� los atributos con los �ngulos de rotaci�n en X, Y y Z.
    double rotX, rotY, rotZ;
    igvMallaTriangulos* malla = nullptr; ///< Malla de tri�ngulos asociada a la escena
    igvMallaTriangulos* modeloImportado=nullptr;

  // Rotación de la pluma (Grado 1)
     // Elevación del gancho (Grado 3)
    bool seleccionando=false;
    void pintar_base();
    void pintar_torre_vertical(); // El mástil
    void pintar_pluma();          // Brazo horizontal y contrapeso
    void pintar_carro();          // La pieza que se mueve horizontalmente
    void pintar_gancho(int escena);         // El cable y el gancho
    void pintar_grua(int escena);
    void pintar_vagon(float r, float g, float b);
    bool animacionModelo; // ¿Se mueve el tren?
    bool animacionCamara; // ¿Gira la cámara sola?
    float posTrenZ;
protected:
    bool sombreadoSuave; // Variable para controlar el estado




public:
    void toggleSombreado();
    int cajaSeleccionada = -1;
    double posicionCarro = 2.0;  // Desplazamiento del carro (Grado 2)
    double longitudCable = 1.0;
    const int EscenaA = 1;   ///< Identificador interno de la escena A
    const int EscenaB = 2;   ///< Identificador interno de la escena B
    const int EscenaC = 3;
    const char *Nombre_EscenaA = "Animacion ";   ///< Etiqueta de la escena A
    const char *Nombre_EscenaB = "Movimiento grafo de escena";   ///< Etiqueta de la escena B
    const char* Nombre_EscenaC = "Seleccion con raton";   ///< Etiqueta de la escena C
    igvCaja caja_;
    igvCaja caja1;
    igvCaja caja2;
    igvCaja caja3;
    double anguloTorre = 0;
        // Constructores por defecto y destructor
    igvEscena3D();
    ~igvEscena3D();
     bool bajandoGancho;
    bool subiendoGancho;
    GLfloat alturaCarga;
    bool cargado;
    bool soltando;
    float alturaCaida;

    double posicion_carro() const {
        return posicionCarro;
    }

    // M�todos
    // m�todo con las llamadas OpenGL para visualizar la escena
    void visualizar(int escena);

    bool get_ejes();

    void set_ejes(bool _ejes);

    igvMallaTriangulos* getMalla() const
    {
        return malla;
    }

    // Apartado A: métodos para incrementar los ángulos
    void incrX() {
        rotX += 10;
    }

    void incrY() {
        rotY += 10;
    }

    void incrZ() {
        rotZ += 10;
    }

    void decrX() {
        rotX -= 10;
    }

    void decrY() {
        rotY -= 10;
    }

    void decrZ() {
        rotZ -= 10;
    }




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
    void toggleAnimacionModelo();
    void toggleAnimacionCamara();
    void paso_simulacion();
    bool getAnimacionCamara() { return animacionCamara; }
    void reset();
private:
    void pintar_ejes();
    igvMallaTriangulos* cargarOBJ(const std::string& filename);




};

#endif   // __IGVESCENA3D
