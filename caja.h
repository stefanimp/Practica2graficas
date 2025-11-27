#ifndef __IGVCAJA
#define __IGVCAJA

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else

#include <GL/glut.h>

#endif   // defined(__APPLE__) && defined(__MACH__)

/**
 * Los objetos de esta clase representan cajas en 3D
 */
class igvCaja
{  private:
    float color[3] = { 0, 0, 0 };   ///< Color RGB de la caja (valores [0..1])
    GLubyte colorUByte[3] = { 0, 0, 0 };   ///< Color RGB de la caja (valores [0..255])
    float posx; // Posición X
    float posz; // Posición Z
    float posy;
public:
    //Constructores y destructores
    /// Constructor por defecto
    igvCaja () = default;
    igvCaja ( float r, float g, float b );
    igvCaja ( const igvCaja &orig );
    ~igvCaja () = default;
    float angulo=0;
    bool valor=false;
    //M�todo para visualizar una caja
    void visualizar ();
    int altura=0;
    //Devuelve el color
    float *getColor ();
    GLubyte *getColorByte (); //Para comparar con el pixel obtenido en la selecci�n
    void incrementaColor ();
    void set_color ( float r1, float g1, float b1 );
    bool seleccionada1();      // Retorna si está seleccionada (usa la variable 'valor')
    float posx1();             // Retorna posición X
    float posz1();             // Retorna posición Z
    void set_posx(float x);    // Asigna posición X
    void set_posz(float z);

    bool valor1() const {
        return valor;
    }

    float angulo1() const {
        return angulo;
    }

    void set_angulo(float angulo) {
        this->angulo = angulo;
    }

    bool esTuColor(GLubyte r, GLubyte g, GLubyte b) ;

    void set_altura(int altura) {
        this->altura = altura;
    }
    void set_valor(bool v);
    void mover(float dx,float dy, float dz);

    void situarCircular(float anguloGrados, float radio, float altura);
};

#endif   // __IGVCAJA
