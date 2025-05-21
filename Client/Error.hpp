/*
** EPITECH PROJECT, 2024
** B-OOP-400-NCY-4-1-tekspice-theotime.collier
** File description:
** Error.hpp
*/

#pragma once

#include <iostream>

class Error : public std::exception {

public:
    Error(const std::string& msg);

    const char* what() const noexcept override;

    private:
    std::string message;

};

class Warning : public std::exception {

public:
    Warning(const std::string& msg);

    void print() const;

    private:
    std::string message;

};