#pragma once

#include <algorithm>
#include <vcd-parser/VCDTypes.hpp>
#include <vcd-parser/VCDTypes.hpp>
#include <vcd-parser/VCDTimedValue.hpp>
#include <vcd-parser/VCDFile.hpp>

/*!
@file VCDComparisons.hpp
@brief A file for defining the comparison operators of the VCD parser.
*/

inline bool operator<(const VCDSignal &a, const VCDSignal &b) {
  if (a.reference != b.reference) return a.reference < b.reference;
  if (a.size != b.size) return a.size < b.size;
  return a.type < b.type;
}

inline bool operator==(const VCDSignal &a, const VCDSignal &b) {
  if (a.reference == b.reference && a.size == b.size && a.type == b.type) {
    return true;
  }
  return false;
}

inline bool operator<(const VCDScope &a, const VCDScope &b) {
  if (a.name != b.name) return a.name < b.name;
  if (a.type != b.type) return a.type < b.type;
  if (a.signals.size() != b.signals.size()) return a.signals.size() < b.signals.size();

  std::vector<std::reference_wrapper<const VCDSignal>> signals1;
  signals1.reserve(a.signals.size());
  for (const auto* signal : a.signals) {
    if (signal != nullptr) signals1.emplace_back(*signal);
  }
  std::sort(signals1.begin(), signals1.end());

  std::vector<std::reference_wrapper<const VCDSignal>> signals2;
  signals2.reserve(b.signals.size());
  for (const auto* signal : b.signals) {
    if (signal != nullptr) signals2.emplace_back(*signal);
  }
  std::sort(signals2.begin(), signals2.end());

  return signals1 < signals2;
}

inline bool operator==(const VCDScope &a, const VCDScope &b) {
  if (a.name == b.name && a.type == b.type && a.signals.size() == b.signals.size()) {
    std::vector<std::reference_wrapper<const VCDSignal>> signals1;
    signals1.reserve(a.signals.size());
    for (const auto* signal : a.signals) {
      if (signal != nullptr) signals1.emplace_back(*signal);
    }
    std::sort(signals1.begin(), signals1.end());

    std::vector<std::reference_wrapper<const VCDSignal>> signals2;
    signals2.reserve(b.signals.size());
    for (const auto* signal : b.signals) {
      if (signal != nullptr) signals2.emplace_back(*signal);
    }
    std::sort(signals2.begin(), signals2.end());

    return signals1 == signals2;
  }
  return false;
}

inline bool operator==(const VCDValue& a, const VCDValue& b) {
  if (a.get_type() != b.get_type()) {
    return false;
  }

  if (a.get_type() == VCDValueType::REAL) {
    return a.get_value_real() == b.get_value_real();
  }
  else if (a.get_type() == VCDValueType::SCALAR) {
    return a.get_value_bit() == b.get_value_bit();
  }
  else if (a.get_type() == VCDValueType::VECTOR) {
    return a.get_value_vector() == b.get_value_vector();
  }
  // VCDValueType::EMPTY
  return true;
}

inline bool operator==(const VCDTimedValue& a, const VCDTimedValue& b) {
  if (a.time == b.time && a.value == b.value) {
    return true;
  }
  return false;
}

/*!
@brief Compares two VCDFile objects.
*/
inline bool operator==(const VCDFile &a, const VCDFile &b) {
  if (a.time_units != b.time_units) {
    return false;
  }

  if (a.time_resolution != b.time_resolution) {
    return false;
  }

  if (a.get_signals().size() != b.get_signals().size()) {
    return false;
  }

  if (a.get_timestamps().size() != b.get_timestamps().size()) {
    return false;
  }

  std::vector<std::reference_wrapper<const VCDSignal>> signals1(a.get_signals().begin(), a.get_signals().end());
  std::sort(signals1.begin(), signals1.end());

  std::vector<std::reference_wrapper<const VCDSignal>> signals2(b.get_signals().begin(), b.get_signals().end());
  std::sort(signals2.begin(), signals2.end());

  if (signals1 != signals2) {
    return false;
  }

  std::vector<std::reference_wrapper<const VCDScope>> scopes1(a.get_scopes().begin(), a.get_scopes().end());
  std::sort(scopes1.begin(), scopes1.end());

  std::vector<std::reference_wrapper<const VCDScope>> scopes2(b.get_scopes().begin(), b.get_scopes().end());
  std::sort(scopes2.begin(), scopes2.end());

  if (scopes1 != scopes2) {
    return false;
  }

  return a.val_map == b.val_map;
}