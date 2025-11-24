#include <cstdlib>
#include <stdio.h>

#include "igvInterfaz.h"

// Aplicaci�n del patr�n de dise�o Singleton
igvInterfaz* igvInterfaz::_instancia = nullptr;

// M�todos p�blicos ----------------------------------------

/**
 * M�todo para acceder al objeto �nico de la clase, en aplicaci�n del patr�n de
 * dise�o Singleton
 * @return Una referencia al objeto �nico de la clase
 */
igvInterfaz& igvInterfaz::getInstancia()
{
    if (!_instancia)
    {
        _instancia = new igvInterfaz;
    }

    return *_instancia;
}

/**
 * Crea el mundo que se visualiza en la ventana
 */
void igvInterfaz::crear_mundo()
{
    // inicia la c�mara
    _instancia->camara.set(IGV_PARALELA, {3.0, 2.0, 4}, {0, 0, 0}
                           , {0, 1.0, 0}, -1 * 1.5, 1 * 1.5, -1 * 1.5, 1 * 1.5
                           , -1 * 3, 200);
}

/**
 * Inicializa todos los par�metros para crear una ventana de visualizaci�n
 * @param argc N�mero de par�metros por l�nea de comandos al ejecutar la
 *             aplicaci�n
 * @param argv Par�metros por l�nea de comandos al ejecutar la aplicaci�n
 * @param _ancho_ventana Ancho inicial de la ventana de visualizaci�n
 * @param _alto_ventana Alto inicial de la ventana de visualizaci�n
 * @param _pos_X Coordenada X de la posici�n inicial de la ventana de
 *               visualizaci�n
 * @param _pos_Y Coordenada Y de la posici�n inicial de la ventana de
 *               visualizaci�n
 * @param _titulo T�tulo de la ventana de visualizaci�n
 * @pre Se asume que todos los par�metros tienen valores v�lidos
 * @post Cambia el alto y ancho de ventana almacenado en el objeto
 */
void igvInterfaz::configura_entorno(int argc, char** argv, int _ancho_ventana
                                    , int _alto_ventana, int _pos_X, int _pos_Y
                                    , std::string _titulo)
{
    // inicializaci�n de las variables de la interfaz
    ancho_ventana = _ancho_ventana;
    alto_ventana = _alto_ventana;

    // inicializaci�n de la ventana de visualizaci�n
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(_ancho_ventana, _alto_ventana);
    glutInitWindowPosition(_pos_X, _pos_Y);
    glutCreateWindow(_titulo.c_str());

    glEnable(GL_DEPTH_TEST); // activa el ocultamiento de superficies por z-buffer
    glClearColor(1.0, 1.0, 1.0, 0.0); // establece el color de fondo de la ventana

    glEnable(GL_LIGHTING); // activa la iluminaci�n de la escena
    glEnable(GL_NORMALIZE); // normaliza los vectores normales para c�lculo iluminaci�n

    crear_mundo(); // crea el mundo a visualizar en la ventana
}

/**
 * M�todo para visualizar la escena y esperar a eventos sobre la interfaz
 */
void igvInterfaz::inicia_bucle_visualizacion()
{
    glutMainLoop(); // inicia el bucle de visualizaci�n de GLUT
}

/**
 * M�todo para control de eventos del teclado
 * @param key C�digo de la tecla pulsada
 * @param x Coordenada X de la posici�n del cursor del rat�n en el momento del
 *          evento de teclado
 * @param y Coordenada Y de la posici�n del cursor del rat�n en el momento del
 *          evento de teclado
 * @pre Se asume que todos los par�metros tienen valores v�lidos
 * @post Los atributos de la clase pueden cambiar, dependiendo de la tecla pulsada
 */
void igvInterfaz::keyboardFunc(unsigned char key, int x, int y)
{
    switch (key)
    {

    case 'x': // Apartado A: rotar X positivo
        _instancia->escena.incrX();
        break;
    case 'X': // Apartado A: rotar X negativo
        _instancia->escena.decrX();
        break;
    case 'y': // Apartado A: rotar Y positivo
        _instancia->escena.incrY();
        break;
    case 'Y': // Apartado C: rotar y negativo
        _instancia->escena.decrY();
        break;
    case 'z': // Apartado C: rotar z positivo
        _instancia->escena.incrZ();
        break;
    case 'Z': // Apartado C: rotar Z negativo
        _instancia->escena.decrZ();
        break;
    case 'n':
        _instancia->escena.getMalla()->cambiarnormales();
        break;
    case 'N':
        _instancia->escena.getMalla()->cambiarnormales();
        break;
    case 'g':
        _instancia->escena.getMalla()->cambiarvis();
        break;
    case 'G':
        _instancia->escena.getMalla()->cambiarvis();
        break;
    case 'c':
        _instancia->activar_movimiento();
        break;
    case 'C':
        _instancia->activar_movimiento();
        break;
    case '-':
        _instancia->camara.zoom(-10);
        break;
    case '+':
        _instancia->camara.zoom(10);
        break;

    case 'e': // activa/desactiva la visualizacion de los ejes
         _instancia->escena.set_ejes(_instancia->escena.get_ejes() ? false : true);
        break;
    case 27: // tecla de escape para SALIR
        exit(1);
        break;
    }
    glutPostRedisplay(); // renueva el contenido de la ventana de visi�n
}

/**
 * M�todo que define la c�mara de visi�n y el viewport. Se llama autom�ticamente
 * cuando se cambia el tama�o de la ventana.
 * @param w Nuevo ancho de la ventana
 * @param h Nuevo alto de la ventana
 * @pre Se asume que todos los par�metros tienen valores v�lidos
 */
void igvInterfaz::reshapeFunc(int w, int h)
{
    // dimensiona el viewport al nuevo ancho y alto de la ventana
    // guardamos valores nuevos de la ventana de visualizaci�n
    _instancia->set_ancho_ventana(w);
    _instancia->set_alto_ventana(h);

    // establece los par�metros de la c�mara y de la proyecci�n
    _instancia->camara.aplicar();
}

/**
 * M�todo para visualizar la escena
 */
void igvInterfaz::displayFunc()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // borra la ventana y el z-buffer

    // se establece el viewport
    glViewport(0, 0, _instancia->get_ancho_ventana(), _instancia->get_alto_ventana());

    // establece los par�metros de la c�mara y de la proyecci�n
    _instancia->camara.aplicar();

    //visualiza la escena
    _instancia->escena.visualizar();

    // refresca la ventana
    glutSwapBuffers(); // se utiliza, en vez de glFlush(), para evitar el parpadeo
}

/**
 * M�todo para inicializar los callbacks GLUT
 */
void igvInterfaz::inicializa_callbacks()
{
    glutKeyboardFunc ( keyboardFunc );
    glutSpecialFunc(specialFunc);
    glutReshapeFunc ( reshapeFunc );
    glutDisplayFunc ( displayFunc );
}

/**
 * M�todo para consultar el ancho de la ventana de visualizaci�n
 * @return El valor almacenado como ancho de la ventana de visualizaci�n
 */
int igvInterfaz::get_ancho_ventana()
{
    return ancho_ventana;
}

/**
 * M�todo para consultar el alto de la ventana de visualizaci�n
 * @return El valor almacenado como alto de la ventana de visualizaci�n
 */
int igvInterfaz::get_alto_ventana()
{
    return alto_ventana;
}

/**
 * M�todo para cambiar el ancho de la ventana de visualizaci�n
 * @param _ancho_ventana Nuevo valor para el ancho de la ventana de visualizaci�n
 * @pre Se asume que el par�metro tiene un valor v�lido
 * @post El ancho de ventana almacenado en la aplicaci�n cambia al nuevo valor
 */
void igvInterfaz::set_ancho_ventana(int _ancho_ventana)
{
    ancho_ventana = _ancho_ventana;
}

/**
 * M�todo para cambiar el alto de la ventana de visualizaci�n
 * @param _alto_ventana Nuevo valor para el alto de la ventana de visualizaci�n
 * @pre Se asume que el par�metro tiene un valor v�lido
 * @post El alto de ventana almacenado en la aplicaci�n cambia al nuevo valor
 */
void igvInterfaz::set_alto_ventana(int _alto_ventana)
{
    alto_ventana = _alto_ventana;
}


void igvInterfaz::specialFunc(int key, int x, int y) {
    if (_instancia->activacion_camara==0) {
        switch (key) {
            case GLUT_KEY_RIGHT:
                _instancia->escena.incrX();
                break;
            case GLUT_KEY_LEFT:
                _instancia->escena.incrY();
                break;
            case GLUT_KEY_UP:
                _instancia->escena.incrZ();
                break;
            case GLUT_KEY_DOWN:
                _instancia->escena.decrX();
                break;
        }

    }
    else {
        switch (key) {
            case GLUT_KEY_RIGHT:
                _instancia->camara.movimento_orbita(5,0);
                break;
            case GLUT_KEY_LEFT:
                _instancia->camara.movimento_orbita(-5,0);
                break;
            case GLUT_KEY_UP:
                _instancia->camara.movimiento_cabeceo(2);
                break;
            case GLUT_KEY_DOWN:
                _instancia->camara.movimiento_cabeceo(-2);
                break;

        }

    }
    glutPostRedisplay();
}