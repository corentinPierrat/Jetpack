/*
** EPITECH PROJECT, 2024
** B-OOP-400-NCY-4-1-tekspice-theotime.collier
** File description:
** Error.cpp
*/

#include "Error.hpp"

Error::Error(const std::string& msg) : message(msg)
{
}

const char* Error::what() const noexcept
{
    return message.c_str();
}

Warning::Warning(const std::string& msg) : message(msg)
{
}

void Warning::print() const
{
       std::cerr << "Warning: " << message << std::endl;
}