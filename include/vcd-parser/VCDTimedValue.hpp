#pragma once

#include <deque>
#include <vcd-parser/VCDValue.hpp>

//! A signal value tagged with times.
struct VCDTimedValue {
  VCDTime     time{};
  VCDValue    value;
};

//! A vector of tagged time/value pairs, sorted by time values.
typedef std::deque<VCDTimedValue> VCDSignalValues;