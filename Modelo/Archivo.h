#pragma once

#include <string>

class Archivo
{
private:
    std::string nombreArchivo;
    std::string descripcion;

    void validarExtension() const;
    static void validarUtf8(const std::string& contenido);
    static void validarFormatoHojaVida(const std::string& contenido);

public:
    Archivo(const std::string& nnombre, const std::string& ndescripcion);
    ~Archivo() = default;

    const std::string& getNombreArchivo() const;
    const std::string& getDescripcion() const;
    void setNombreArchivo(const std::string& nnombre);

    bool abrirArchivo(const std::string& narchivo) const;
    std::string leerTexto() const;
    void guardarTexto(const std::string& contenido) const;
};
