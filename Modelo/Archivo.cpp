#include "Archivo.h"
#include "ExcepcionesArchivo.h"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <vector>

namespace
{
    constexpr std::streamoff TAMANIO_MAXIMO = 1024 * 1024;

    std::filesystem::path convertirRuta(const std::string& rutaUtf8)
    {
        return std::filesystem::u8path(rutaUtf8);
    }
}

Archivo::Archivo(const std::string& nnombre, const std::string& ndescripcion)
    : nombreArchivo(nnombre), descripcion(ndescripcion)
{
}

const std::string& Archivo::getNombreArchivo() const
{
    return nombreArchivo;
}

const std::string& Archivo::getDescripcion() const
{
    return descripcion;
}

void Archivo::setNombreArchivo(const std::string& nnombre)
{
    nombreArchivo = nnombre;
}

bool Archivo::abrirArchivo(const std::string& narchivo) const
{
    std::ifstream archivo(convertirRuta(narchivo), std::ios::binary);
    return archivo.is_open();
}

void Archivo::validarExtension() const
{
    if (nombreArchivo.empty())
    {
        throw FormatoIncorrectoException(
            "No se ha seleccionado ningún archivo.");
    }

    std::string extension = convertirRuta(nombreArchivo).extension().string();
    std::transform(
        extension.begin(),
        extension.end(),
        extension.begin(),
        [](unsigned char caracter) {
            return static_cast<char>(std::tolower(caracter));
        });

    if (extension != ".txt")
    {
        throw FormatoIncorrectoException(
            "Formato incorrecto: seleccione un archivo con extensión .txt.");
    }
}

void Archivo::validarUtf8(const std::string& contenido)
{
    for (std::size_t i = 0; i < contenido.size();)
    {
        const unsigned char primero =
            static_cast<unsigned char>(contenido[i]);

        if (primero == 0)
        {
            throw ArchivoCorruptoException(
                "Archivo corrupto: contiene bytes nulos no válidos.");
        }

        std::size_t continuaciones = 0;
        if (primero <= 0x7F)
        {
            ++i;
            continue;
        }
        if (primero >= 0xC2 && primero <= 0xDF)
        {
            continuaciones = 1;
        }
        else if (primero >= 0xE0 && primero <= 0xEF)
        {
            continuaciones = 2;
        }
        else if (primero >= 0xF0 && primero <= 0xF4)
        {
            continuaciones = 3;
        }
        else
        {
            throw ArchivoCorruptoException(
                "Archivo corrupto: la codificación no es UTF-8 válida.");
        }

        if (i + continuaciones >= contenido.size())
        {
            throw ArchivoCorruptoException(
                "Archivo corrupto: la secuencia UTF-8 está incompleta.");
        }

        for (std::size_t j = 1; j <= continuaciones; ++j)
        {
            const unsigned char siguiente =
                static_cast<unsigned char>(contenido[i + j]);
            if ((siguiente & 0xC0) != 0x80)
            {
                throw ArchivoCorruptoException(
                    "Archivo corrupto: la codificación no es UTF-8 válida.");
            }
        }

        if ((primero == 0xE0 &&
             static_cast<unsigned char>(contenido[i + 1]) < 0xA0) ||
            (primero == 0xED &&
             static_cast<unsigned char>(contenido[i + 1]) >= 0xA0) ||
            (primero == 0xF0 &&
             static_cast<unsigned char>(contenido[i + 1]) < 0x90) ||
            (primero == 0xF4 &&
             static_cast<unsigned char>(contenido[i + 1]) >= 0x90))
        {
            throw ArchivoCorruptoException(
                "Archivo corrupto: contiene una secuencia UTF-8 inválida.");
        }

        i += continuaciones + 1;
    }
}

void Archivo::validarFormatoHojaVida(const std::string& contenido)
{
    const std::vector<std::string> camposObligatorios = {
        "HOJA DE VIDA DEL DOCTOR",
        "Nombres:",
        "Apellidos:",
        "Cédula:",
        "Especialidad:",
        "Registro profesional:",
        "FORMACIÓN ACADÉMICA",
        "EXPERIENCIA PROFESIONAL"
    };

    if (contenido.rfind("HOJA DE VIDA DEL DOCTOR", 0) != 0)
    {
        throw FormatoIncorrectoException(
            "Formato incorrecto: la primera línea debe ser "
            "\"HOJA DE VIDA DEL DOCTOR\".");
    }

    for (const std::string& campo : camposObligatorios)
    {
        if (contenido.find(campo) == std::string::npos)
        {
            throw FormatoIncorrectoException(
                "Formato incorrecto: falta el campo o sección \"" +
                campo + "\".");
        }
    }
}

std::string Archivo::leerTexto() const
{
    validarExtension();

    std::ifstream archivo(
        convertirRuta(nombreArchivo),
        std::ios::binary | std::ios::ate);

    if (!archivo.is_open())
    {
        throw ArchivoCorruptoException(
            "No se pudo abrir el archivo. Verifique que exista y tenga "
            "permisos de lectura.");
    }

    const std::streamoff tamanio = archivo.tellg();
    if (tamanio <= 0)
    {
        throw ArchivoCorruptoException(
            "Archivo corrupto: el archivo está vacío.");
    }
    if (tamanio > TAMANIO_MAXIMO)
    {
        throw ArchivoCorruptoException(
            "Archivo corrupto: supera el tamaño máximo permitido de 1 MB.");
    }

    archivo.seekg(0, std::ios::beg);
    std::string contenido(
        (std::istreambuf_iterator<char>(archivo)),
        std::istreambuf_iterator<char>());

    if (archivo.bad())
    {
        throw ArchivoCorruptoException(
            "Archivo corrupto: ocurrió un error durante la lectura.");
    }

    if (contenido.size() >= 3 &&
        static_cast<unsigned char>(contenido[0]) == 0xEF &&
        static_cast<unsigned char>(contenido[1]) == 0xBB &&
        static_cast<unsigned char>(contenido[2]) == 0xBF)
    {
        contenido.erase(0, 3);
    }

    validarUtf8(contenido);
    validarFormatoHojaVida(contenido);
    return contenido;
}

void Archivo::guardarTexto(const std::string& contenido) const
{
    validarExtension();

    if (contenido.empty())
    {
        throw FormatoIncorrectoException(
            "Formato incorrecto: la hoja de vida no puede quedar vacía.");
    }
    if (contenido.size() > static_cast<std::size_t>(TAMANIO_MAXIMO))
    {
        throw ArchivoCorruptoException(
            "El contenido supera el tamaño máximo permitido de 1 MB.");
    }

    validarUtf8(contenido);
    validarFormatoHojaVida(contenido);

    const std::filesystem::path ruta = convertirRuta(nombreArchivo);
    std::ifstream comprobacion(ruta, std::ios::binary);
    if (!comprobacion.is_open())
    {
        throw ArchivoCorruptoException(
            "El archivo no existe. Agréguelo primero; la aplicación no crea "
            "el archivo .txt inicial.");
    }
    comprobacion.close();

    std::ofstream archivo(ruta, std::ios::binary | std::ios::trunc);
    if (!archivo.is_open())
    {
        throw ArchivoCorruptoException(
            "No se pudo abrir el archivo para escritura. Verifique sus "
            "permisos.");
    }

    archivo.write(
        contenido.data(),
        static_cast<std::streamsize>(contenido.size()));
    archivo.flush();

    if (!archivo.good())
    {
        throw ArchivoCorruptoException(
            "Ocurrió un error al escribir el archivo; no se pudo guardar.");
    }
}
