# Plan: Compilar PSeInt con Emscripten y tratar dependencias no portables (wxWidgets)

## Objetivo
Generar una versión web (WASM) de PSeInt priorizando el intérprete y herramientas de lógica, separando la UI basada en wxWidgets para reemplazarla por una interfaz web.

## Alcance inicial (MVP)
1. Compilar el intérprete (núcleo) a WebAssembly.
2. Proveer una UI web mínima para editar, ejecutar y ver salida.
3. Aplazar o reemplazar componentes dependientes de wxWidgets (wxPSeInt, psdraw3, psdrawE, psterm, pseval) con alternativas web.

## Inventario de módulos y decisión de portabilidad
- Núcleo portable:
  - pseint/ (intérprete, parser, ejecución): objetivo principal.
  - psexport/ (exportador a C++): opcional en una fase posterior.
- Dependiente de wxWidgets (no portable directamente):
  - wxPSeInt/, psdraw3/, psdrawE/, psterm/, pseval/.
- Librerías de terceros en fuente:
  - hoewrap/hoedown y dtl/ pueden compilar con Emscripten si no dependen de APIs de sistema. Evaluar caso por caso.

## Estrategia general
1. Crear un build del núcleo con Emscripten (WASM) y exponer una API C/C++ estable (funciones para cargar pseudocódigo, compilar/verificar, ejecutar, capturar salida y errores).
2. Implementar UI web para:
   - Editor (texto) con resaltado simple o integración con un editor web (por ejemplo, Monaco o CodeMirror).
   - Consola de salida y panel de errores.
   - Botones de ejecutar/detener, y selector de ejemplos.
3. Integrar con Emscripten FS para lectura/escritura de archivos (memoria o IndexedDB).

## Plan paso a paso
### 1) Preparación
- Añadir un directorio `web/` para la UI y assets.
- Definir un objetivo de build nuevo en Makefile o un script de build específico para Emscripten.
- Validar el núcleo en C++17 sea compatible con clang/emscripten (evitar `fork`, `exec`, sockets nativos, threads no soportados, etc.).

### 2) Aislar el núcleo
- Identificar las funciones de entrada del intérprete (punto de ejecución, carga de código, errores).
- Si están acopladas a UI/IO de consola, crear una capa `core_api`:
  - `core_init()`
  - `core_load_source(const char* src)`
  - `core_check()` -> devuelve errores
  - `core_run()` -> devuelve salida
  - `core_get_last_error()`
- Redirigir salida/errores a buffers en memoria para exponer a la UI web.

### 3) Build con Emscripten
- Crear una lista mínima de fuentes para compilar sólo el núcleo.
- Usar `em++` con opciones recomendadas:
  - `-s MODULARIZE=1` y `-s EXPORT_ES6=1` (si se integra con bundler)
  - `-s ALLOW_MEMORY_GROWTH=1`
  - `-s EXPORTED_FUNCTIONS=["_core_init", ...]`
  - `-s EXPORTED_RUNTIME_METHODS=["cwrap", "ccall"]`
  - `-O2` en producción, `-O0 -g` en debug
- Verificar tamaños y tiempos de carga.

### 4) UI web
- Proveer un `index.html` con:
  - Editor de texto.
  - Botón "Verificar" y "Ejecutar".
  - Consola de salida.
- Conectar con `cwrap/ccall` para invocar funciones exportadas.
- Manejar errores como strings formateados por el núcleo.

### 5) Persistencia de archivos
- Usar Emscripten FS en memoria para archivos temporales.
- Para persistencia, montar IndexedDB (IDBFS) y sincronizar bajo demanda.

### 6) Reemplazo de wxWidgets
- No portar wxWidgets; crear UI web equivalente.
- Para funciones específicas:
  - Editor principal (wxPSeInt): reemplazar por editor web.
  - Terminal (psterm): consola web.
  - Diagramas (psdraw3/psdrawE): reemplazar por renderizado en canvas/SVG o postergar.
  - Ejercicios autocontenidos (pseval): reemplazar por vistas web con ejecución embebida.

## Problemas típicos y mitigaciones
- Uso de filesystem nativo: reemplazar por Emscripten FS.
- Subprocesos: Emscripten soporta pthreads con restricciones; evitar de inicio.
- Tiempo de ejecución: usar `-O2` y revisar hot paths.
- Localización/codificación: asegurar UTF-8 en entrada/salida.

## Entregables
1. Script de build WASM para el núcleo.
2. `web/` con UI mínima funcional.
3. Documentación de API y flujo de ejecución.

## Fases posteriores
- Agregar exportación a otros lenguajes (psexport).
- Soporte de diagramas (canvas/SVG) y herramientas avanzadas.
- Integración con almacenamiento local y ejemplos.

## Criterios de aceptación (MVP)
- Se puede pegar pseudocódigo, verificar y ejecutar en el navegador.
- Se muestran errores y salida en la UI.
- El build no depende de wxWidgets.
