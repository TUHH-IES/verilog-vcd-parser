#pragma once

#include <vcd-parser/VCDTypes.hpp>

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
  /*!
  @brief Create a new VCDValue with the type VCD_SCALAR
  */
  VCDValue(VCDBit value) {
    this -> type = VCDValueType::VCD_SCALAR;
    this ->m_value.val_bit = value;
  }

  /*!
  @brief Create a new VCDValue with the type VCD_VECTOR
  */
  VCDValue(VCDBitVector *value) {
    this -> type = VCDValueType::VCD_VECTOR;
    this ->m_value.val_vector= value;
  }

  /*!
  @brief Create a new VCDValue with the type VCD_VECTOR
  */
  VCDValue(VCDReal value) {
    this -> type = VCDValueType::VCD_REAL;
    this ->m_value.val_real = value;
  }


  ~VCDValue() {
    if (this->type == VCDValueType::VCD_VECTOR)
    {
      delete this->m_value.val_vector;
    }
  }


  //! Return the type of value stored by this class instance.
  VCDValueType get_type() {
    return this -> type;
  }

  //! Get the bit value of the instance.
  [[nodiscard]] VCDBit get_value_bit() const {
    return this ->m_value.val_bit;
  }

  //! Get the vector value of the instance.
  [[nodiscard]] VCDBitVector *get_value_vector() const {
    return this ->m_value.val_vector;
  }

  //! Get the real value of the instance.
  [[nodiscard]] VCDReal get_value_real() const {
    return this ->m_value.val_real;
  }


protected:
  //! The type of value this instance stores.
  VCDValueType type;

  //! The actual value stored, as identified by type.
  union valstore {
    VCDBit val_bit;          //!< Value as a bit
    VCDBitVector *val_vector;//!< Value as a bit vector
    VCDReal val_real;        //!< Value as a real number (double).
  } m_value;
};
