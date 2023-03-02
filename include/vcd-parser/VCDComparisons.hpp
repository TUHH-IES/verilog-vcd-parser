#pragma once

#include <algorithm>
#include <vcd-parser/VCDTypes.hpp>

/*!
@file VCDComparisons.hpp
@brief A file for defining the comparison operators of the VCD parser.
*/

bool operator<(const VCDSignal &a, const VCDSignal &b) {
  if (a.reference != b.reference) return a.reference < b.reference;
  if (a.size != b.size) return a.size < b.size;
  return a.type < b.type;
}

bool operator==(const VCDSignal &a, const VCDSignal &b) {
  if (a.reference == b.reference && a.size == b.size && a.type == b.type) {
    return true;
  }
  return false;
}

bool operator<(const VCDScope &a, const VCDScope &b) {
  if (a.name != b.name) return a.name < b.name;
  if (a.type != b.type) return a.type < b.type;
  if (a.signals.size() != b.signals.size()) return a.signals.size() < b.signals.size();

  std::vector<std::reference_wrapper<const VCDSignal>> v1;
  for (const auto* signal : a.signals) {
    if (signal != nullptr) v1.emplace_back(*signal);
  }
  std::sort(v1.begin(), v1.end());

  std::vector<std::reference_wrapper<const VCDSignal>> v2;
  for (const auto* signal : b.signals) {
    if (signal != nullptr) v2.emplace_back(*signal);
  }
  std::sort(v2.begin(), v2.end());

  return v1 < v2;
}

bool operator==(const VCDScope &a, const VCDScope &b) {
  if (a.name == b.name && a.type == b.type && a.signals.size() == b.signals.size()) {
    std::vector<std::reference_wrapper<const VCDSignal>> v1;
    for (const auto* signal : a.signals) {
      if (signal != nullptr) v1.emplace_back(*signal);
    }
    std::sort(v1.begin(), v1.end());

    std::vector<std::reference_wrapper<const VCDSignal>> v2;
    for (const auto* signal : b.signals) {
      if (signal != nullptr) v2.emplace_back(*signal);
    }
    std::sort(v2.begin(), v2.end());

    return v1 == v2;
  }
  return false;
}

bool operator==(const VCDValue& a, const VCDValue& b) {
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

bool operator==(const VCDTimedValue& a, const VCDTimedValue& b) {
  if (a.time == b.time && a.value == b.value) {
    return true;
  }
  return false;
}

/*!
@brief Compares two VCDFile objects.
*/
VCD_PARSER_EXPORT
bool operator==(const VCDFile &a, const VCDFile &b) {
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

  std::vector<std::reference_wrapper<const VCDSignal>> signals1;
  for (const auto* signal : a.get_signals()) {
    if (signal != nullptr) signals1.emplace_back(*signal);
  }
  std::sort(signals1.begin(), signals1.end());

  std::vector<std::reference_wrapper<const VCDSignal>> signals2;
  for (const auto* signal : b.get_signals()) {
    if (signal != nullptr) signals2.emplace_back(*signal);
  }
  std::sort(signals2.begin(), signals2.end());

  if (signals1 != signals2) {
    return false;
  }

  std::vector<std::reference_wrapper<const VCDScope>> scopes1;
  for (const auto* signal : a.get_scopes()) {
    if (signal != nullptr) scopes1.emplace_back(*signal);
  }
  std::sort(scopes1.begin(), scopes1.end());

  std::vector<std::reference_wrapper<const VCDScope>> scopes2;
  for (const auto* signal : b.get_scopes()) {
    if (signal != nullptr) scopes2.emplace_back(*signal);
  }
  std::sort(scopes2.begin(), scopes2.end());

  if (scopes1 != scopes2) {
    return false;
  }

  return a.val_map == b.val_map;
}