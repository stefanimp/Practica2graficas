#ifndef __IGVINTERFAZ
#define __IGVINTERFAZ

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else

#include <GL/glut.h>

#endif   // defined(__APPLE__) && defined(__MACH__)

#include <string>

#include "igvEscena3D.h"
#include "igvCamara.h"

/**
 * Los objetos de esta clase encapsulan la interfaz y el estado de la aplicaci�n
 */
class igvInterfaz
{  private:
      // Atributos
      int ancho_ventana = 0; ///< Ancho de la ventana de visualizaci�n
      int alto_ventana = 0;  ///< Alto de la ventana de visualizaci�n

      igvEscena3D escena; ///< Escena que se visualiza en la ventana definida por igvInterfaz
      igvCamara camara; ///< C�mara que se utiliza para visualizar la escena
      int activacion_camara = 0;
      // Aplicaci�n del patr�n de dise�o Singleton
      static igvInterfaz* _instancia; ///< Direcci�n de memoria del objeto �nico de la clase
      /// Constructor por defecto
      igvInterfaz () = default;


   public:
      // Aplicaci�n del patr�n de dise�o Singleton
      static igvInterfaz& getInstancia ();
      // Constructores por defecto y destructor

      /// Destructor
      ~igvInterfaz () = default;

      // M�todos est�ticos
      // callbacks de eventos
      // callbacks de eventos
      static void keyboardFunc ( unsigned char key, int x, int y ); // m�todo para control de eventos del teclado
      static void reshapeFunc ( int w, int h ); // m�todo que define la camara de vision y el viewport
      // se llama autom�ticamente cuando se cambia el tama�o de la ventana
      static void displayFunc (); // m�todo para visualizar la escena
    void activar_movimiento() {
        if (activacion_camara==0) {
            activacion_camara=1;
        }
        else {
            activacion_camara=0;;
        }
    };

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
    static void specialFunc(int key, int x, int y);
};

#endif   // __IGVINTERFAZ
