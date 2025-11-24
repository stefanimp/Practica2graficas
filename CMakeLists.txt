cmake_minimum_required(VERSION 3.25)
project(pr3a)

set(CMAKE_CXX_STANDARD 14)

# Incluir el directorio actual para encontrar las cabeceras (.h)
include_directories(.)

# Definir el ejecutable y sus archivos fuente
add_executable(${PROJECT_NAME}
        igvCamara.cpp
        igvCamara.h
        igvCilindro.cpp
        igvCilindro.h
        igvEscena3D.cpp
        igvEscena3D.h
        igvInterfaz.cpp
        igvInterfaz.h
        igvMallaTriangulos.cpp
        igvMallaTriangulos.h
        igvPunto3D.cpp
        igvPunto3D.h
        pr2a.cpp)

# Configuración para LINUX (se deja igual por si acaso)
if (LINUX)
    find_path(OPENGL_REGISTRY_INCLUDE_DIRS "GL/glcorearb.h")
    target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_REGISTRY_INCLUDE_DIRS})

    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES})
    target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIR})

    find_package(GLUT REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE GLUT::GLUT)
endif ()

# Configuración para WINDOWS (MODIFICADO PARA VCPKG + MINGW)
if (WIN32)
    # Definición necesaria para usar constantes matemáticas como M_PI en Windows
    add_definitions(-D_USE_MATH_DEFINES)

    # Buscar paquete estándar de OpenGL (viene con vcpkg o el sistema)
    find_package(OpenGL REQUIRED)

    # Buscar paquete estándar de GLUT (vcpkg mapea FreeGLUT a GLUT automáticamente)
    find_package(GLUT REQUIRED)

    # Vincular las librerías encontradas al proyecto
    target_link_libraries(${PROJECT_NAME}
            PRIVATE
            OpenGL::GL   # Librería base OpenGL
            OpenGL::GLU  # Utilidades GLU
            GLUT::GLUT   # Librería GLUT/FreeGLUT
    )
endif ()