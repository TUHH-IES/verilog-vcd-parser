#pragma once

#include <vcd-parser/VCDTypes.hpp>

#include <variant>

/*!
@brief Represents a single value found in a VCD File.
@details Can contain a single bit (a scalar), a bti vector, or an
IEEE floating point number.
*/
class VCDValue {

public:
  //! Convert a VCDBit to a single char
  static char VCDBit2Char(VCDBit b) {
    switch (b)
    {
      case (VCDBit::VCD_0):
        return '0';
      case (VCDBit::VCD_1):
        return '1';
      case (VCDBit::VCD_Z):
        return 'Z';
      case (VCDBit::VCD_X):
      default:
        return 'X';
    }
  }

public:
  VCDValue() = default;

  /*!
  @brief Create a new VCDValue with the type VCD_SCALAR
  */
  explicit VCDValue(VCDBit value) {
    type = VCDValueType::SCALAR;
    m_value = value;
  }

  /*!
  @brief Create a new VCDValue with the type VCD_VECTOR
  */
  explicit VCDValue(const VCDBitVector& value) {
    type = VCDValueType::VECTOR;
    m_value = value;
  }

  /*!
  @brief Create a new VCDValue with the type VCD_VECTOR
  */
  explicit VCDValue(VCDReal value) {
    type = VCDValueType::REAL;
    m_value = value;
  }

  //! Return the type of value stored by this class instance.
  [[nodiscard]] VCDValueType get_type() const {
    return type;
  }

  //! Get the bit value of the instance.
  [[nodiscard]] VCDBit get_value_bit() const {
    return std::get<VCDBit>(m_value);
  }

  //! Get the vector value of the instance.
  [[nodiscard]] VCDBitVector get_value_vector() const {
    return std::get<VCDBitVector>(m_value);
  }

  //! Get the real value of the instance.
  [[nodiscard]] VCDReal get_value_real() const {
    return std::get<VCDReal>(m_value);
  }


protected:
  //! The type of value this instance stores.
  VCDValueType type = VCDValueType::EMPTY;

  //! The actual value stored, as identified by type.
  std::variant<VCDBit, VCDBitVector, VCDReal> m_value;
};
