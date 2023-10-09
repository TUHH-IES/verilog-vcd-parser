#pragma once

#include <vcd-parser/VCDTypes.hpp>

#include <ostream>

/*!
@file VCDPrinters.hpp
@brief A file for defining the stream insertion operators for the types of the VCD parser.
*/

inline std::ostream &operator<<(std::ostream &out, const VCDBit &val) {
    switch (val) {
        case VCDBit::VCD_0:
            return out << "0";
        case VCDBit::VCD_1:
            return out << "1";
        case VCDBit::VCD_X:
            return out << "X";
        case VCDBit::VCD_Z:
            return out << "Z";
    }
}

inline std::ostream &operator<<(std::ostream &out, const VCDValueType &val) {
    switch (val) {
        case VCDValueType::SCALAR:
            return out << "scalar";
        case VCDValueType::VECTOR:
            return out << "vector";
        case VCDValueType::REAL:
            return out << "real";
        case VCDValueType::EMPTY:
            return out << "empty";
    }
}

inline std::ostream &operator<<(std::ostream &out, const VCDVarType &val) {
    switch (val) {
        case VCDVarType::VCD_VAR_EVENT:
            return out << "event";
        case VCDVarType::VCD_VAR_INTEGER:
            return out << "integer";
        case VCDVarType::VCD_VAR_PARAMETER:
            return out << "parameter";
        case VCDVarType::VCD_VAR_REAL:
            return out << "real";
        case VCDVarType::VCD_VAR_REALTIME:
            return out << "realtime";
        case VCDVarType::VCD_VAR_REG:
            return out << "reg";
        case VCDVarType::VCD_VAR_SUPPLY0:
            return out << "supply0";
        case VCDVarType::VCD_VAR_SUPPLY1:
            return out << "supply1";
        case VCDVarType::VCD_VAR_TIME:
            return out << "time";
        case VCDVarType::VCD_VAR_TRI:
            return out << "tri";
        case VCDVarType::VCD_VAR_TRIAND:
            return out << "triand";
        case VCDVarType::VCD_VAR_TRIOR:
            return out << "trior";
        case VCDVarType::VCD_VAR_TRIREG:
            return out << "trireg";
        case VCDVarType::VCD_VAR_TRI0:
            return out << "tri0";
        case VCDVarType::VCD_VAR_TRI1:
            return out << "tri1";
        case VCDVarType::VCD_VAR_WAND:
            return out << "wand";
        case VCDVarType::VCD_VAR_WIRE:
            return out << "wire";
        case VCDVarType::VCD_VAR_WOR:
            return out << "wor";
    }
}

inline std::ostream &operator<<(std::ostream &out, const VCDScopeType &val) {
    switch (val) {
        case VCDScopeType::VCD_SCOPE_BEGIN:
            return out << "begin";
        case VCDScopeType::VCD_SCOPE_FORK:
            return out << "fork";
        case VCDScopeType::VCD_SCOPE_FUNCTION:
            return out << "function";
        case VCDScopeType::VCD_SCOPE_MODULE:
            return out << "module";
        case VCDScopeType::VCD_SCOPE_TASK:
            return out << "task";
        case VCDScopeType::VCD_SCOPE_ROOT:
            return out << "root";
    }
}

inline std::ostream &operator<<(std::ostream &out, const VCDTimeUnit &val) {
    switch (val) {
        case VCDTimeUnit::TIME_S:
            return out << "s";
        case VCDTimeUnit::TIME_MS:
            return out << "ms";
        case VCDTimeUnit::TIME_US:
            return out << "us";
        case VCDTimeUnit::TIME_NS:
            return out << "ns";
        case VCDTimeUnit::TIME_PS:
            return out << "ps";
    }
}

inline std::ostream &operator<<(std::ostream &out, const VCDScope &val) {
    return out << "Scope " << val.type << ": " << val.name;
}

inline std::ostream &operator<<(std::ostream &out, const VCDSignal &val) {
    if (val.size > 1) {
        return out << "Signal " << val.type << ": " << val.reference << "(" << val.hash << ")[" << val.size << "]" ;
    }
    else {
        return out << "Signal " << val.type << ": " << val.reference << "(" << val.hash << ")";
    }
}

inline std::ostream &operator<<(std::ostream &out, const VCDBitVector &val) {
    out << "[";
    for (const auto& bit : val) {
        out << bit;
    }
    return out << "]";
}

inline std::ostream &operator<<(std::ostream &out, const VCDValue &val) {
    switch (val.get_type()) {
        case VCDValueType::REAL:
            return out << val.get_value_real();
        case VCDValueType::SCALAR:
            return out << val.get_value_bit();
        case VCDValueType::VECTOR:
            return out << val.get_value_vector();
        case VCDValueType::EMPTY:
            return out << "empty";
    }
}

inline std::ostream &operator<<(std::ostream &out, const VCDTimedValue &val) {
    return out << "t" << val.time << ": " << val.value;
}

inline std::ostream &operator<<(std::ostream &out, const VCDSignalValues &val) {
    out << "{";
    for (const auto& timed_value : val) {
        out << timed_value << "\n";
    }
    return out << "}";
}