#pragma once

#include <deque>


//! A signal value tagged with times.
struct VCDTimedValue {
  VCDTime     time{};
  VCDValue    value;
};

//! A vector of tagged time/value pairs, sorted by time values.
typedef std::deque<VCDTimedValue> VCDSignalValues;