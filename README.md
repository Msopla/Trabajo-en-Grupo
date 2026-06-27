# Hoja de vida del doctor

Aplicación Windows Forms en C++/CLI para leer y escribir una hoja de vida
guardada en un archivo `.txt` existente.

## Estructura MVC

- `Modelo/Archivo.*`: lectura, escritura y validaciones del archivo.
- `Vista/VistaPrincipal.h`: formulario y componentes visuales.
- `Controlador/ControladorArchivo.h`: coordina la vista y el modelo.

## Uso

1. Use `Archivo_para_agregar/hoja_vida_doctor.txt` como plantilla o copie su
   contenido en otro archivo guardado con codificación UTF-8.
2. Abra `Trabajo_en_Grupo.slnx` en Visual Studio.
3. Compile para `x64` y ejecute.
4. Pulse **Examinar**, seleccione el archivo y use **Guardar cambios** para
   sobrescribir ese mismo archivo.

La aplicación no crea el archivo `.txt` inicial.
El proyecto está configurado para .NET Framework 4.7.2, que ya está instalado
en el equipo.

## Pruebas solicitadas

1. **Funcionamiento correcto:** use la plantilla completa suministrada.
2. **Archivo corrupto:** seleccione un `.txt` vacío. Se mostrará la excepción
   `ArchivoCorruptoException`.
3. **Formato incorrecto:** use un `.txt` que no comience con
   `HOJA DE VIDA DEL DOCTOR` o elimine un campo obligatorio. Se mostrará la
   excepción `FormatoIncorrectoException`.

También se considera corrupto un archivo con bytes nulos, UTF-8 inválido,
errores de lectura o tamaño mayor a 1 MB.
