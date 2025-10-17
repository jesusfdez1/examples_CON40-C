# CON40-C: Do not perform operations on shared atomic variables in isolation


Este repositorio contiene implementaciones prácticas de la regla de seguridad **CON40-C** del estándar CERT C Coding Standard. La regla establece que no se deben realizar operaciones sobre variables atómicas compartidas de forma aislada, ya que esto puede conducir a condiciones de carrera en entornos multihilo.

## Contexto 


En programación concurrente, las operaciones atómicas proporcionan garantías de atomicidad para operaciones individuales. Sin embargo, cuando se realizan múltiples operaciones atómicas de forma secuencial para completar una tarea lógica única, el conjunto de operaciones no es atómico. Esto crea una ventana temporal donde otros hilos pueden interferir, causando inconsistencias en los datos compartidos.

### Vulnerabilidades
La violación de CON40-C puede resultar en:
- **Actualizaciones perdidas**: Actualizaciones perdidas cuando múltiples hilos leen el mismo valor antes de escribir
- **Condiciones de carrera**: El resultado depende del orden de ejecución de los hilos
- **Inconsistencia de datos**: Inconsistencias en el estado de las variables compartidas
- **Comportamiento no determinista**: Comportamiento no determinista que dificulta la depuración

## Estructura del Repositorio

```
examples_CON40-C/
│
├── README.md
│
├── example-1/                     # Operaciones de toggle en atomic_bool
│   ├── error.c                    # Código no conforme
│   ├── solution.c                 # Solución con atomic_compare_exchange_weak
│   ├── solution_2.c               # Solución con operador XOR
│   └── Makefile
│
└── example-2/                     # Operaciones de incremento en atomic_int
    ├── error.c                    # Código no conforme
    ├── solution.c                 # Solución con atomic_fetch_add
    ├── solution_2.c               # Solución con operador ++
    └── Makefile
```

## Requisitos del Sistema

- **Compilador**: GCC 4.9+ o Clang 3.1+ con soporte para C11
- **Make**: GNU Make o compatible
- **Sistema Operativo**: Linux/Unix, Windows (MinGW, MSYS2, WSL), macOS

## Instrucciones de Compilación

```bash
# Compilar un ejemplo
cd example-1/ o cd example-2/
make

# Compilar archivo específico
make error
make solution
make solution_2

# Ejecutar todos los ejemplos
make run

# Limpiar archivos compilados
make clean
```

## Referencias


- Herlihy, M., & Shavit, N. (2012). *The Art of Multiprocessor Programming*. Morgan Kaufmann.
- Williams, A. (2019). *C++ Concurrency in Action* (2nd ed.). Manning Publications.
- Butenhof, D. R. (1997). *Programming with POSIX Threads*. Addison-Wesley.

