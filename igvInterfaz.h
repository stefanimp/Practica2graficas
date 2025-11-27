#ifndef __IGVINTERFAZ
#define __IGVINTERFAZ

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#include <GL/glut.h>

#endif   // defined(__APPLE__) && defined(__MACH__)

#include <string>

#include "igvEscena3D.h"
#include "igvCamara.h"
#include "caja.h"
enum modoInterfaz
{  IGV_VISUALIZAR ///< En la ventana se va a visualizar de manera normal la escena
   , IGV_SELECCIONAR /**< Se ha hecho clic en la ventana de visualización, y la
                      *   escena se debe visualizar en modo selección
                      */
};


/**
 * Los objetos de esta clase encapsulan la interfaz y el estado de la aplicaci�n
 */
class igvInterfaz
{  private:
      // Atributos
      int ancho_ventana = 0;
      int alto_ventana = 0;
      igvCaja caja;
      igvEscena3D escena;
      igvCamara camara;


      static igvInterfaz* _instancia;

      igvInterfaz () = default;
    modoInterfaz modo = IGV_VISUALIZAR;
    int cursorX = 0 , cursorY = 0;
    int objeto_seleccionado = -1;
    bool boton_retenido = false;
    int activacion_camara = 0;
    unsigned char the_pixel[3];
    GLint boton1;
    int ultima_i=0;
    int cont1;
    bool partido=false;
    int menuSelection =1;

   public:
    bool mover_angulo_torre2=true;
    bool mover_angulo_torre1=true;
    bool mover_angulo_torre_=true;
      static igvInterfaz& getInstancia ();

    void create_menu();

      ~igvInterfaz () = default;


      static void keyboardFunc ( unsigned char key, int x, int y );
      static void reshapeFunc ( int w, int h );
      static void displayFunc ();
    static void mouseFunc ( GLint boton, GLint estado, GLint x, GLint y ); // control de pulsacion del raton
    static void motionFunc ( GLint x, GLint y ); // control del desplazamiento del raton con boton pulsado
    static void specialFunc(int key, int x, int y);
    static void idle() ;

    static void menuHandle ( int value );

      // Metodos
      // crea el mundo que se visualiza en la ventana
      void crear_mundo ();

      // inicializa todos los par�metros para crear una ventana de visualizaci�n
      void configura_entorno ( int argc, char **argv // par�metros del main
                             , int _ancho_ventana, int _alto_ventana // ancho y alto de la ventana de visualizaci�n
                             , int _pos_X, int _pos_Y // posici�n inicial de la ventana de visualizaci�n
                             , std::string _titulo // t�tulo de la ventana de visualizaci�n
                             );



      void inicializa_callbacks (); // inicializa todos los callbacks

      void inicia_bucle_visualizacion (); // visualiza la escena y espera a eventos sobre la interfaz

      // m�todos get_ y set_ de acceso a los atributos
      int get_ancho_ventana ();

      int get_alto_ventana ();

      void set_ancho_ventana ( int _ancho_ventana );

      void set_alto_ventana ( int _alto_ventana );
    void activar_movimiento() {
        if (activacion_camara==0) {
            activacion_camara=1;
        }
        else {
            activacion_camara=0;;
        }
    };

      int cont() const {
          return cont1;
      }

      void set_cont(int cont1) {
          this->cont1 = cont1;
      }
    bool mismoColor(GLubyte pixel[3], GLubyte* colorCaja) {
          return pixel[0] == colorCaja[0] &&
                 pixel[1] == colorCaja[1] &&
                 pixel[2] == colorCaja[2];
      }
};

#endif   // __IGVINTERFAZ
