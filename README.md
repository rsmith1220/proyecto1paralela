# Proyecto 1 Paralela
Por: Cayetano Molina, Jose Pablo Monzon, Rebecca Smith

Este proyecto es un screensaver utilizando SDL2 y OpenGL. El screensaver muestra circulos de diferente tamano que rebotan contra los bordes de la pantalla. Asimismo se mantiene un control de los frames de la ventana. 

## Instalacion

Se requiere instalar SDL2:
```bash
sudo apt-get install libsdl2-dev
```

## Ejecucion
Al abrir una terminal en en la misma carpeta donde se encuentra el `main.cpp` se compila de la siguiente manera:

```bash
g++ main.cpp -o <name> 'sdl2-config --cflags --libs'
```

Y luego se ejecuta de la siguiente manera
```bash
./<name> 
```

