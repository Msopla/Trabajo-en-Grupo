#pragma once

#include <stdexcept>
#include <string>

class ArchivoCorruptoException : public std::runtime_error
{
public:
    explicit ArchivoCorruptoException(const std::string& mensaje)
        : std::runtime_error(mensaje)
    {
    }
};

class FormatoIncorrectoException : public std::runtime_error
{
public:
    explicit FormatoIncorrectoException(const std::string& mensaje)
        : std::runtime_error(mensaje)
    {
    }
};
