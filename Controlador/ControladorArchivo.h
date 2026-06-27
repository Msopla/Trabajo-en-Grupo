#pragma once

#include "../Modelo/Archivo.h"
#include "../Modelo/ExcepcionesArchivo.h"
#include "../Vista/IVistaArchivo.h"

#include <string>

using namespace System;
using namespace System::Text;

public ref class ControladorArchivo
{
private:
    IVistaArchivo^ vista;
    Archivo* modelo;

    static std::string convertirAStdString(String^ texto)
    {
        if (String::IsNullOrEmpty(texto))
        {
            return std::string();
        }

        array<Byte>^ bytes = (gcnew UTF8Encoding(false, true))->GetBytes(texto);
        std::string resultado;
        resultado.resize(bytes->Length);

        for (int i = 0; i < bytes->Length; ++i)
        {
            resultado[static_cast<std::size_t>(i)] =
                static_cast<char>(bytes[i]);
        }
        return resultado;
    }

    static String^ convertirAString(const std::string& texto)
    {
        array<Byte>^ bytes =
            gcnew array<Byte>(static_cast<int>(texto.size()));

        for (std::size_t i = 0; i < texto.size(); ++i)
        {
            bytes[static_cast<int>(i)] =
                static_cast<Byte>(
                    static_cast<unsigned char>(texto[i]));
        }

        return (gcnew UTF8Encoding(false, true))->GetString(bytes);
    }

    void actualizarRuta()
    {
        modelo->setNombreArchivo(convertirAStdString(vista->RutaArchivo));
    }

public:
    explicit ControladorArchivo(IVistaArchivo^ nvista)
        : vista(nvista),
          modelo(new Archivo("", "Hoja de vida del doctor"))
    {
    }

    ~ControladorArchivo()
    {
        this->!ControladorArchivo();
    }

    !ControladorArchivo()
    {
        delete modelo;
        modelo = nullptr;
    }

    void Leer()
    {
        try
        {
            actualizarRuta();
            vista->Contenido = convertirAString(modelo->leerTexto());
            vista->MostrarInformacion(
                "Archivo leído correctamente. Puede editarlo y guardar "
                "los cambios.");
        }
        catch (const ArchivoCorruptoException& excepcion)
        {
            vista->MostrarError(
                "Archivo corrupto o inaccesible",
                convertirAString(excepcion.what()));
        }
        catch (const FormatoIncorrectoException& excepcion)
        {
            vista->MostrarError(
                "Formato incorrecto",
                convertirAString(excepcion.what()));
        }
        catch (const std::exception& excepcion)
        {
            vista->MostrarError(
                "Error inesperado",
                convertirAString(excepcion.what()));
        }
    }

    void Guardar()
    {
        try
        {
            actualizarRuta();
            modelo->guardarTexto(convertirAStdString(vista->Contenido));
            vista->MostrarInformacion(
                "Cambios guardados correctamente en el archivo existente.");
        }
        catch (const ArchivoCorruptoException& excepcion)
        {
            vista->MostrarError(
                "Archivo corrupto o inaccesible",
                convertirAString(excepcion.what()));
        }
        catch (const FormatoIncorrectoException& excepcion)
        {
            vista->MostrarError(
                "Formato incorrecto",
                convertirAString(excepcion.what()));
        }
        catch (const std::exception& excepcion)
        {
            vista->MostrarError(
                "Error inesperado",
                convertirAString(excepcion.what()));
        }
    }
};
