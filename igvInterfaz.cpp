#include <cstdlib>
#include <stdio.h>
#include "igvInterfaz.h"

igvInterfaz* igvInterfaz::_instancia = nullptr;

igvInterfaz& igvInterfaz::getInstancia() {
    if (!_instancia) _instancia = new igvInterfaz;
    return *_instancia;
}

void igvInterfaz::crear_mundo() {
    _instancia->camara.set(IGV_PARALELA, {5.0, 5.0, 10.0}, {0, 2, 0}
            , {0, 1.0, 0}, -1 * 5, 1 * 5, -1 * 5, 1 * 5
            , -1 * 3, 200);
}

void igvInterfaz::configura_entorno(int argc, char** argv, int _ancho_ventana, int _alto_ventana, int _pos_X, int _pos_Y, std::string _titulo) {
    ancho_ventana = _ancho_ventana;
    alto_ventana = _alto_ventana;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(_ancho_ventana, _alto_ventana);
    glutInitWindowPosition(_pos_X, _pos_Y);
    glutCreateWindow(_titulo.c_str());
    glEnable(GL_DEPTH_TEST);
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glEnable(GL_LIGHTING);
    glEnable(GL_NORMALIZE);
    crear_mundo();
}

void igvInterfaz::inicia_bucle_visualizacion() {
    glutMainLoop();
}

void igvInterfaz::keyboardFunc(unsigned char key, int x, int y) {
    switch (key) {
        // Rotaciones de cámara globales
        case 'x': _instancia->escena.incrX(); break;
        case 'X': _instancia->escena.decrX(); break;
        case 'y': _instancia->escena.incrY(); break;
        case 'Y': _instancia->escena.decrY(); break;
        case 'z': _instancia->escena.incrZ(); break;
        case 'Z': _instancia->escena.decrZ(); break;

            // Opciones visuales
        case 'n': case 'N': _instancia->escena.getMalla()->cambiarnormales(); break;
        case 'g': case 'G': _instancia->escena.getMalla()->cambiarvis(); break;
        case 'e': _instancia->escena.set_ejes(!_instancia->escena.get_ejes()); break;
        case 'c': case 'C': _instancia->activar_movimiento(); break;

            // ZOOM (Corregido para aceptar '+' sin shift)
        case '-': case '_':
            _instancia->camara.zoom(-5);
            break;
        case '+': case '=':
            _instancia->camara.zoom(5);
            break;

            // --- CONTROLES DE LA GRÚA TORRE ---

            // 1. ROTAR TORRE (Grado 1)
        case 't': _instancia->escena.cambiarAnguloTorre(5); break;
        case 'T': _instancia->escena.cambiarAnguloTorre(-5); break;

            // 2. MOVER CARRO (Grado 2)
        case 'b': // Avanzar carro
            _instancia->escena.cambiarPosicionCarro(0.2);
            break;
        case 'B': // Retroceder carro
            _instancia->escena.cambiarPosicionCarro(-0.2);
            break;

            // 3. SUBIR/BAJAR GANCHO (Grado 3)
        case 'a': // Bajar gancho (aumentar cable)
            _instancia->escena.cambiarLongitudCable(0.2);
            break;
        case 'A': // Subir gancho
            _instancia->escena.cambiarLongitudCable(-0.2);
            break;

        case 27: exit(1); break;
    }
    glutPostRedisplay();
}

void igvInterfaz::reshapeFunc(int w, int h) {
    _instancia->set_ancho_ventana(w);
    _instancia->set_alto_ventana(h);
    _instancia->camara.aplicar();
}

void igvInterfaz::displayFunc() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, _instancia->get_ancho_ventana(), _instancia->get_alto_ventana());
    _instancia->camara.aplicar();
    _instancia->escena.visualizar();
    glutSwapBuffers();
}

void igvInterfaz::inicializa_callbacks() {
    glutKeyboardFunc(keyboardFunc);
    glutSpecialFunc(specialFunc);
    glutReshapeFunc(reshapeFunc);
    glutDisplayFunc(displayFunc);
}

// Getters y Setters
int igvInterfaz::get_ancho_ventana() { return ancho_ventana; }
int igvInterfaz::get_alto_ventana() { return alto_ventana; }
void igvInterfaz::set_ancho_ventana(int _ancho_ventana) { ancho_ventana = _ancho_ventana; }
void igvInterfaz::set_alto_ventana(int _alto_ventana) { alto_ventana = _alto_ventana; }

void igvInterfaz::specialFunc(int key, int x, int y) {
    if (_instancia->activacion_camara == 0) {
        switch (key) {
            case GLUT_KEY_RIGHT: _instancia->escena.incrX(); break;
            case GLUT_KEY_LEFT:  _instancia->escena.incrY(); break;
            case GLUT_KEY_UP:    _instancia->escena.incrZ(); break;
            case GLUT_KEY_DOWN:  _instancia->escena.decrX(); break;
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