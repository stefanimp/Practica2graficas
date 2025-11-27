#include <cstdlib>
#include <stdio.h>

#include "igvInterfaz.h"

#include <iostream>
#include <ostream>

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
void igvInterfaz::create_menu ()
{  int menu_id = glutCreateMenu ( menuHandle );
    glutAddMenuEntry ( _instancia->escena.Nombre_EscenaA
                       , _instancia->escena.EscenaA );
    glutAddMenuEntry ( _instancia->escena.Nombre_EscenaB
                       , _instancia->escena.EscenaB );
    glutAddMenuEntry ( _instancia->escena.Nombre_EscenaC
                       , _instancia->escena.EscenaC );

    glutAttachMenu ( GLUT_RIGHT_BUTTON );
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
    create_menu ();
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
    case 'a': case 'A': // Activar/Desactivar animación del Tren
        _instancia->escena.toggleAnimacionModelo();
        break;
    case 's':
    case 'S':
        _instancia->escena.toggleSombreado();
        break;
    case 'h': case 'H': // Activar/Desactivar animación Cámara
        _instancia->escena.toggleAnimacionCamara();
            break;
    case '-': case '_':
        _instancia->camara.zoom(-5);
        break;
    case '+': case '=':
        _instancia->camara.zoom(5);
        break;
        case 'v':

            _instancia->set_cont((_instancia->cont() + 1) % 4);

            switch (_instancia->cont()) {
                case 0:
                    _instancia->camara.set(IGV_PARALELA, {10, 0,0 }, {0, 0, 0}, {0, 1, 0},-5, 5, -5, 5, 1, 100);

                    break;
                case 1:
                    _instancia->camara.set(IGV_PARALELA, {0, 0, 10}, {0, 0, 0}, {0, 1, 0},-5, 5, -5, 5, 1, 100);//Alazado

                    break;
                case 2:

                    _instancia->camara.set(IGV_PARALELA, {0, 10, 0}, {0, 0, 0}, {1, 0, 0},-5, 5, -5, 5, 1, 100);
                    break;
                case 3:
                    _instancia->camara.set(IGV_PARALELA, {3,2,4}, {0,0,0}, {0,1,0}, 90, 1.0, 0.1, 100);//Perfil

                    break;
            }
            _instancia->camara.aplicar();
            break;
        case 'V':
            _instancia->set_cont((_instancia->cont() +1) % 4);

            switch (_instancia->cont()) {
                case 0:
                    _instancia->camara.set(IGV_PERSPECTIVA, {0,5,0}, {0,0,0}, {1,0,0}, 90, 1.0, 0.1, 100);//Planta

                    break;
                case 1:
                    _instancia->camara.set(IGV_PERSPECTIVA, {10,0,0}, {0,0,0}, {0,1,0},90, 1.0, 0.1, 100);//Alazado

                    break;
                case 2:

                    _instancia->camara.set(IGV_PERSPECTIVA, {0,0,10}, {0,0,0}, {0,1,0}, 90, 1.0, 0.1, 100);//Perfil

                    break;
                case 3:

                    _instancia->camara.set(IGV_PERSPECTIVA, {3,2,4}, {0,0,0}, {0,1,0}, 90, 1.0, 0.1, 100);//Perfil

                    break;
            }
            _instancia->camara.aplicar();
            break;
    case '4':
        _instancia->partido = !_instancia->partido; // Solo cambiar el flag
        break;

    case 'y':

            _instancia->camara.gira_eje_der(5);

        break;
    case 'Y':
            _instancia->camara.gira_eje_der(-5);
                break;
        // 2. MOVER CARRO (Grado 2)
    case 'b': // Avanzar carro
        _instancia->escena.cambiarPosicionCarro(0.2);
        break;
    case 'B': // Retroceder carro
        _instancia->escena.cambiarPosicionCarro(-0.2);
        break;

    case 'c': case 'C': _instancia->activar_movimiento(); break;

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (_instancia->partido) {
        // Modo 4 vistas
        int w = glutGet(GLUT_WINDOW_WIDTH);
        int h = glutGet(GLUT_WINDOW_HEIGHT);
        int half_w = w / 2;
        int half_h = h / 2;

        // INFERIOR IZQUIERDA - Alzado
        glViewport(0, 0, half_w, half_h);
        _instancia->camara.set(IGV_PERSPECTIVA, {10,0,0}, {0,0,0}, {0,1,0}, 60, 1.0, 0.1, 100);
        _instancia->camara.aplicar();
        _instancia->escena.visualizar(_instancia->menuSelection);

        // INFERIOR DERECHA - Perfil
        glViewport(half_w, 0, half_w, half_h);
        _instancia->camara.set(IGV_PERSPECTIVA, {0,0,10}, {0,0,0}, {0,1,0}, 60, 1.0, 0.1, 100);
        _instancia->camara.aplicar();
        _instancia->escena.visualizar(_instancia->menuSelection);

        // SUPERIOR IZQUIERDA - Planta
        glViewport(0, half_h, half_w, half_h);
        _instancia->camara.set(IGV_PERSPECTIVA, {0,5,0}, {0,0,0}, {1,0,0}, 60, 1.0, 0.1, 100);
        _instancia->camara.aplicar();
        _instancia->escena.visualizar(_instancia->menuSelection);

        // SUPERIOR DERECHA - Perspectiva
        glViewport(half_w, half_h, half_w, half_h);
        _instancia->camara.set(IGV_PERSPECTIVA, {3,2,4}, {0,0,0}, {0,1,0}, 60, 1.0, 0.1, 100);
        _instancia->camara.aplicar();
        _instancia->escena.visualizar(_instancia->menuSelection);
    }
    else {
        // Modo vista única
        glViewport(0, 0, _instancia->get_ancho_ventana(), _instancia->get_alto_ventana());
        _instancia->camara.aplicar();
        _instancia->escena.visualizar(_instancia->menuSelection);
    }

    glutSwapBuffers();
}

/**
 * M�todo para inicializar los callbacks GLUT
 */
void igvInterfaz::inicializa_callbacks() {

    glutKeyboardFunc(keyboardFunc);
    glutSpecialFunc(specialFunc);
    glutMouseFunc(mouseFunc);
    glutMotionFunc(motionFunc);
    glutReshapeFunc(reshapeFunc);
    glutDisplayFunc(displayFunc);
    glutIdleFunc(idle);


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

void igvInterfaz::motionFunc(GLint x, GLint y) {

    if (_instancia->menuSelection == _instancia->escena.EscenaC &&
        _instancia->boton_retenido) {

        GLint dx = x - _instancia->cursorX;
        GLint dy = y - _instancia->cursorY;

        if (_instancia->escena.cajaSeleccionada == 0) {


            if (_instancia->mover_angulo_torre_) {
                _instancia->escena.anguloTorre = -10;
                _instancia->escena.caja_.mover(1,5,0);
                _instancia->escena.cambiarPosicionCarro(3.4);
                _instancia->escena.cambiarLongitudCable(2.5);
                _instancia->mover_angulo_torre_ = false;
            }

            // 2. Actualizar el ángulo con el ratón (FALTABA ESTO)
            _instancia->escena.anguloTorre += dx * 0.15f;

            // 3. Cálculos manuales
            float radianes = _instancia->escena.anguloTorre * (M_PI / 180.0f);

            // Asumo que tienes una función o variable para esto.
            // Si te da error usa: _instancia->escena.posicionCarro
            float radio = _instancia->escena.posicion_carro();

            float radioAmpliado = radio*4 ;
            float posxCalculada = radioAmpliado * cos(radianes);
            float poszCalculada = -radioAmpliado * sin(radianes);

            _instancia->escena.caja_.set_posx(posxCalculada);
            _instancia->escena.caja_.set_posz(poszCalculada);
        }

        else if (_instancia->escena.cajaSeleccionada == 1) {

            // 1. Inicialización
            if (_instancia->mover_angulo_torre1) {
                _instancia->escena.caja1.mover(-1,5,0);
                _instancia->escena.anguloTorre = -45;
                _instancia->escena.cambiarPosicionCarro(4);
                _instancia->escena.cambiarLongitudCable(2);
                _instancia->mover_angulo_torre1 = false;
            }


            _instancia->escena.anguloTorre += dx * 0.15f;

            float radianes = _instancia->escena.anguloTorre * (M_PI / 180.0f);
            float radio = _instancia->escena.posicion_carro();
            float radioAmpliado = radio*4 ;
            float posxCalculada = radioAmpliado * cos(radianes);
            float poszCalculada = -radioAmpliado * sin(radianes);

            _instancia->escena.caja1.set_posx(posxCalculada);
            _instancia->escena.caja1.set_posz(poszCalculada);
        }
        else if (_instancia->escena.cajaSeleccionada == 2) {


            if (_instancia->mover_angulo_torre2) {
                _instancia->escena.anguloTorre = -65;
                _instancia->escena.caja2.mover(0,5,-10);
                _instancia->escena.cambiarPosicionCarro(0.5);
                _instancia->escena.cambiarLongitudCable(3);
                _instancia->mover_angulo_torre2 = false;
            }


            _instancia->escena.anguloTorre += dx * 0.15f;

            float radianes = _instancia->escena.anguloTorre * (M_PI / 180.0f);
            float radio = _instancia->escena.posicion_carro();
            float radioAmpliado = radio*4 ;
            float posxCalculada = radioAmpliado * cos(radianes);
            float poszCalculada = -radioAmpliado * sin(radianes);

            _instancia->escena.caja2.set_posx(posxCalculada-5);
            _instancia->escena.caja2.set_posz(poszCalculada-4);
        }

    }

    _instancia->cursorX = x;
    _instancia->cursorY = y;

    glutPostRedisplay();
}


void igvInterfaz::mouseFunc(GLint boton, GLint estado, GLint x, GLint y) {
    if (_instancia->menuSelection != _instancia->escena.EscenaC)
        return;

    if (boton == GLUT_LEFT_BUTTON) {

        if (estado == GLUT_DOWN) {

            _instancia->modo = IGV_SELECCIONAR;
            _instancia->boton_retenido = true;
            _instancia->cursorX = x;
            _instancia->cursorY = y;
            glDrawBuffer(GL_BACK);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glDisable(GL_LIGHTING);
            glDisable(GL_TEXTURE_2D);
            glDisable(GL_DITHER);

            _instancia->camara.aplicar();
            _instancia->escena.visualizar(_instancia->menuSelection);
            glFlush();
            glFinish();

            GLint vp[4];
            glGetIntegerv(GL_VIEWPORT, vp);

            GLubyte pixel[3];
            glReadPixels(x, vp[3] - y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);


            GLubyte* cR = _instancia->escena.caja_.getColorByte();
            GLubyte* cV = _instancia->escena.caja1.getColorByte();
            GLubyte* cA = _instancia->escena.caja2.getColorByte();


            _instancia->escena.cajaSeleccionada = -1;


            if (_instancia->mismoColor(pixel, cR)) {
                _instancia->escena.cajaSeleccionada = 0;

                std::cout << "Caja roja seleccionada\n";
            }
            else if (_instancia->mismoColor(pixel, cV)) {
                _instancia->escena.cajaSeleccionada = 1;

                std::cout << "Caja verde seleccionada\n";
            }
            else if (_instancia->mismoColor(pixel, cA)) {
                _instancia->escena.cajaSeleccionada = 2;
                std::cout << "Caja azul seleccionada\n";
            }
            else {
                std::cout << "Nada seleccionado\n";
            }

            glEnable(GL_LIGHTING);
            glEnable(GL_TEXTURE_2D);
            glEnable(GL_DITHER);
        }
        else {
            _instancia->modo = IGV_VISUALIZAR;
            _instancia->boton_retenido = false;
        }
    }

    glutPostRedisplay();
}




void igvInterfaz::specialFunc(int key, int x, int y) {
    if (_instancia->activacion_camara == 0) {
        switch (key) {
            case GLUT_KEY_RIGHT: _instancia->escena.cambiarAnguloTorre(5); break;
            case GLUT_KEY_LEFT:  _instancia->escena.cambiarAnguloTorre(-5); break;
            case GLUT_KEY_UP:    _instancia->escena.cambiarLongitudCable(-0.2); break;
            case GLUT_KEY_DOWN:  _instancia->escena.cambiarLongitudCable(0.2); break;
        }
    } else {
        switch (key) {
            case GLUT_KEY_RIGHT: _instancia->camara.movimento_orbita(5, 0); break;
            case GLUT_KEY_LEFT:  _instancia->camara.movimento_orbita(-5, 0); break;
            case GLUT_KEY_UP:    _instancia->camara.movimiento_cabeceo(2); break;
            case GLUT_KEY_DOWN:  _instancia->camara.movimiento_cabeceo(-2); break;
        }
    }
    glutPostRedisplay();
}

void igvInterfaz::idle() {
    if (_instancia->menuSelection==_instancia->escena.EscenaA) {

        _instancia->escena.paso_simulacion();

        if (_instancia->escena.getAnimacionCamara()) {
            _instancia->camara.movimento_orbita(0.5, 0);
        }


        glutPostRedisplay();
    }
}


void igvInterfaz::menuHandle(int value)
{
    _instancia->menuSelection = value;
    _instancia->escena.reset();
    _instancia->mover_angulo_torre_ = true;
    _instancia->mover_angulo_torre1 = true;
    _instancia->mover_angulo_torre2 = true;
    glutPostRedisplay();
}