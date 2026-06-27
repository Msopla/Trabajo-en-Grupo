#pragma once

using namespace System;

public interface class IVistaArchivo
{
    property String^ RutaArchivo
    {
        String^ get();
    }

    property String^ Contenido
    {
        String^ get();
        void set(String^ valor);
    }

    void MostrarInformacion(String^ mensaje);
    void MostrarError(String^ titulo, String^ mensaje);
};
