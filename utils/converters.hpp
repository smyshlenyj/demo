#pragma once

#include <string>

#include "operation.hpp"
#include "calc.grpc.pb.h"

inline std::string operationToString(Operation operation)
{
    switch (operation)
    {
        case Operation::ADD:
            return "ADD";
        case Operation::SUB:
            return "SUB";
        case Operation::MUL:
            return "MUL";
        case Operation::DIV:
            return "DIV";
        case Operation::POW:
            return "POW";
        case Operation::FACT:
            return "FACT";
        case Operation::UNKNOWN:
            return "UNKNOWN";
    }
    return "INVALID";
}

inline Operation operationFromString(const std::string& str)
{
    if (str == "ADD") return Operation::ADD;
    if (str == "SUB") return Operation::SUB;
    if (str == "MUL") return Operation::MUL;
    if (str == "DIV") return Operation::DIV;
    if (str == "POW") return Operation::POW;
    if (str == "FACT") return Operation::FACT;

    throw std::runtime_error("Unknown operation in cache: " + str);
}

inline Operation toLocalOperation(calc::Operation grpcOperation)
{
    switch (grpcOperation)
    {
        case calc::Operation::ADD:
            return Operation::ADD;
        case calc::Operation::SUB:
            return Operation::SUB;
        case calc::Operation::MUL:
            return Operation::MUL;
        case calc::Operation::DIV:
            return Operation::DIV;
        case calc::Operation::POW:
            return Operation::POW;
        case calc::Operation::FACT:
            return Operation::FACT;
        default:
            throw std::runtime_error("Unknown gRPC operation");
    }
}

inline calc::Operation toGrpcOperation(Operation op)
{
    switch (op)
    {
        case Operation::ADD:
            return calc::Operation::ADD;
        case Operation::SUB:
            return calc::Operation::SUB;
        case Operation::MUL:
            return calc::Operation::MUL;
        case Operation::DIV:
            return calc::Operation::DIV;
        case Operation::POW:
            return calc::Operation::POW;
        case Operation::FACT:
            return calc::Operation::FACT;
        case Operation::UNKNOWN:
            throw std::invalid_argument("Unknown operation");
    }
    throw std::runtime_error("Unknown operation");
}