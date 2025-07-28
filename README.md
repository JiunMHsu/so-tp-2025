# Episode III - Revenge of the Cth

Este es el Trabajo Práctico de Sistemas Operativos de la UTN FRBA, 2025 Primer Cuatrimestre.

Los lineamientos, criterios de evaluación y requerimientos del sistema están especificados en el
documento principal del Trabajo Práctico, véase [Episode III - Revenge of the Cth](./doc/main.pdf).

## Dependencias

Para poder compilar y ejecutar el proyecto, es necesario tener instalada la
biblioteca [so-commons-library] de la cátedra y readline:

```bash
git clone https://github.com/sisoputnfrba/so-commons-library
cd so-commons-library
make debug
make install
```

```bash
sudo apt install libreadline8 libreadline-dev
```

## Compilación y ejecución

Cada módulo del proyecto se compila de forma independiente a través de un
archivo `makefile`. Para compilar un módulo, es necesario ejecutar el comando
`make` desde la carpeta correspondiente.

Como alternativa, se puede ejecutar el script `makeall.sh`.
Éste compilará todos los módulos incluyendo utils.

```bash
bash makeall.sh
```

Los ejecutables resultantes de la compilación se guardarán en la carpeta `bin` de
cada módulo. Ejemplo:

```sh
cd kernel
make
./bin/kernel
```

## Desarrollo por Visual Studio Code

Para importar el workspace, se debe abrir el archivo `tp.code-workspace` desde
la interfaz o ejecutando el siguiente comando desde la carpeta raíz del
repositorio:

```bash
code tp.code-workspace
```

## Deploy

Para desplegar el proyecto, ejecutar el script `deploy.sh` especificando la ip tanto del kernel como
la de memoria en los argumentos.
El mismo modificará los valores de las configs y compilará todos los módulos.

```bash
git clone https://github.com/JiunMHsu/so-tp-2025.git
cd so-tp-2025
bash deploy.sh --kernel=<ip máquina kernel> --memoria=<ip máquina memoria>
```

Ejemplo:

```bash
bash deploy.sh --kernel=192.168.1.37 --memoria=192.168.1.38
```

> [!NOTE]
> Si se omite alguna flag (IP), se tomará el valor por defecto que es `127.0.0.1`.

## Prueba

Las pruebas de este TP consisten en cinco partes, once casos en total 
(véase el documento de [pruebas](/doc/pruebas-finales.pdf)):

```bash
plani corto fifo
plani corto sjf
plani corto srt
plani lym fifo
plani lym pmcp
swap
cache clock
cache clockm
tlb fifo
tlb lru
estabilidad
```

Cada caso requiere su configuración específica, por lo que previo a cada prueba,
se debe ejecutar el script `set-test.sh`.

```bash
bash set-test.sh <prueba>
```

Ejemplo:

```bash
bash set-test.sh plani corto srt
```

[so-commons-library]: https://github.com/sisoputnfrba/so-commons-library
