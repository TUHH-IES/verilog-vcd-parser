#pragma once

#include <vcd-parser/VCDTypes.hpp>
#include <vcd-parser/VCDValue.hpp>
#include <vcd-parser/VCDTimedValue.hpp>

#include <stdexcept>
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

#if !defined(VCD_PARSER_EXPORT)
#define VCD_PARSER_EXPORT
#endif

/*!
@brief Top level object to represent a single VCD file.
*/
class VCDFile {

public:
  //! Instance a new VCD file container.
  VCD_PARSER_EXPORT
  VCDFile() = default;

  //! Destructor
  VCD_PARSER_EXPORT
  ~VCDFile() {

    // Delete signals and scopes.

    for (VCDScope *scope : scopes)
    {
      for (VCDSignal *signal : scope->signals)
      {
        delete signal;
      }
      delete scope;
    }
  }

  //! Timescale of the VCD file.
  VCDTimeUnit time_units;

  //! Multiplier of the VCD file time units.
  VCDTimeRes time_resolution;

  //! Date string of the VCD file.
  std::string date;

  //! Version string of the simulator which generated the VCD.
  std::string version;

  //! Version string of the simulator which generated the VCD.
  std::string comment;

  //! Root scope node of the VCD signals
  VCDScope *root_scope;

  /*!
  @brief Add a new scope object to the VCD file
  @param s in - The VCDScope object to add to the VCD file.
  */
  void add_scope(VCDScope *s) {
    scopes.emplace_back(s);
  }

  /*!
  @brief Add a new signal to the VCD file
  @param s in - The VCDSignal object to add to the VCD file.
  */
  void add_signal(VCDSignal *s) {
    signals.emplace_back(s);

    // Add a timestream entry
    if (val_map.find(s->hash) == val_map.end())
    {
      // Values will be populated later.
      val_map[s->hash] = VCDSignalValues();
    }
  }


  /*!
  @brief Add a new timestamp value to the VCD file.
  @details Add a time stamp to the sorted array of existing
  timestamps in the file.
  @param time in - The timestamp value to add to the file.
  */
  void add_timestamp(VCDTime time) {
    times.emplace_back(time);
  }


  /*!
  @brief Return the scope object in the VCD file with this name
  @param name in - The name of the scope to get and return.
  */
  VCD_PARSER_EXPORT
  VCDScope* get_scope(const VCDScopeName& name) {
    for (auto&& scope : scopes)
    {
      if (scope->name == name)
      {
        return scope;
      }
    }
    return nullptr;
  }


  /*!
  @brief Add a new signal value to the VCD file, tagged by time.
  @param time_val in - A signal value, tagged by the time it occurs.
  @param hash in - The VCD hash value representing the signal.
  */
  void add_signal_value(const VCDTimedValue& time_val, const VCDSignalHash& hash) {
    val_map[hash].emplace_back(time_val);
  }


  /*!
  @brief Get the value of a particular signal at a specified time.
  @note The supplied time value does not need to exist in the
  vector returned by get_timestamps().
  @param hash in - The hashcode for the signal to identify it.
  @param time in - The time at which we want the value of the signal.
  @param erase_prior in - Erase signals prior to this time. Avoids O(n^2) searching times when scanning large .vcd files sequentially.
  @returns A pointer to the value at the supplie time, or nullptr if
  no such record can be found.
  */
  const VCDValue& get_signal_value_at(const VCDSignalHash& hash, VCDTime time, bool erase_prior = false) {
    auto find = val_map.find(hash);
    if (find == val_map.end())
    {
      throw std::runtime_error("Signal not found");
    }

    auto& vals = find->second;

    if (vals.empty())
    {
      throw std::runtime_error("Empty signal");
    }

    auto erase_until = vals.begin();
    bool found = false;

    for (auto it = vals.begin(); it != vals.end(); ++it)
    {
      if (it->time <= time)
      {
        erase_until = it;
        found = true;
      }
      else
      {
        break;
      }
    }

    if (!found) {
      throw std::runtime_error("Element not found");
    }

    if (erase_prior)
    {
      // avoid O(n^2) performance for large sequential scans
      vals.erase(vals.begin(), erase_until);
    }

    return erase_until->value;
  }

  /*!
  @brief Get a vector of VCD time values
  @param hash in - The hashcode for the signal to identify it.
  @returns A pointer to the vector of time values, or nullptr if hash not found
  */
  const VCDSignalValues& get_signal_values(const VCDSignalHash& hash) {
    return val_map.at(hash);
  }

  /*!
  @brief Return a pointer to the set of timestamp samples present in
         the VCD file.
  */
  const std::vector<VCDTime>& get_timestamps() {
    return times;
  }

  /*!
  @brief Get a vector of all scopes present in the file.
  */
  VCD_PARSER_EXPORT
  std::vector<VCDScope*>& get_scopes() {
    return scopes;
  }

  /*!
  @brief Return a flattened vector of all signals in the file.
  */
  VCD_PARSER_EXPORT
  std::vector<VCDSignal*>& get_signals() {
    return signals;
  }

protected:
  //! Flat vector of all signals in the file.
  std::vector<VCDSignal*> signals;

  //! Flat mao of all scope objects in the file, keyed by name.
  std::vector<VCDScope *> scopes;

  //! Vector of time values present in the VCD file - sorted, asc
  std::vector<VCDTime> times;

  //! Map of hashes onto vectors of times and signal values.
  std::unordered_map<VCDSignalHash, VCDSignalValues> val_map;
};
