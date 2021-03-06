// Copyright 2014 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_COMPILER_REGISTER_CONFIGURATION_H_
#define V8_COMPILER_REGISTER_CONFIGURATION_H_

#include "src/base/macros.h"
#include "src/machine-type.h"

namespace v8 {
namespace internal {

// An architecture independent representation of the sets of registers available
// for instruction creation.
class RegisterConfiguration {
 public:
  // Define the optimized compiler selector for register configuration
  // selection.
  //
  // TODO(X87): This distinction in RegisterConfigurations is temporary
  // until x87 TF supports all of the registers that Crankshaft does.
  enum CompilerSelector { CRANKSHAFT, TURBOFAN };

  enum AliasingKind {
    // Registers alias a single register of every other size (e.g. Intel).
    OVERLAP,
    // Registers alias two registers of the next smaller size (e.g. ARM).
    COMBINE
  };

  // Architecture independent maxes.
  static const int kMaxGeneralRegisters = 32;
  static const int kMaxFPRegisters = 32;

  static const RegisterConfiguration* ArchDefault(CompilerSelector compiler);

  RegisterConfiguration(int num_general_registers, int num_double_registers,
                        int num_allocatable_general_registers,
                        int num_allocatable_double_registers,
                        AliasingKind fp_aliasing_kind,
                        const int* allocatable_general_codes,
                        const int* allocatable_double_codes,
                        char const* const* general_names,
                        char const* const* float_names,
                        char const* const* double_names);

  int num_general_registers() const { return num_general_registers_; }
  int num_float_registers() const { return num_float_registers_; }
  int num_double_registers() const { return num_double_registers_; }
  int num_allocatable_general_registers() const {
    return num_allocatable_general_registers_;
  }
  int num_allocatable_double_registers() const {
    return num_allocatable_double_registers_;
  }
  int num_allocatable_float_registers() const {
    return num_allocatable_float_registers_;
  }
  AliasingKind fp_aliasing_kind() const { return fp_aliasing_kind_; }
  int32_t allocatable_general_codes_mask() const {
    return allocatable_general_codes_mask_;
  }
  int32_t allocatable_double_codes_mask() const {
    return allocatable_double_codes_mask_;
  }
  int GetAllocatableGeneralCode(int index) const {
    return allocatable_general_codes_[index];
  }
  int GetAllocatableDoubleCode(int index) const {
    return allocatable_double_codes_[index];
  }
  int GetAllocatableFloatCode(int index) const {
    return allocatable_float_codes_[index];
  }
  const char* GetGeneralRegisterName(int code) const {
    return general_register_names_[code];
  }
  const char* GetFloatRegisterName(int code) const {
    return float_register_names_[code];
  }
  const char* GetDoubleRegisterName(int code) const {
    return double_register_names_[code];
  }
  const int* allocatable_general_codes() const {
    return allocatable_general_codes_;
  }
  const int* allocatable_double_codes() const {
    return allocatable_double_codes_;
  }
  const int* allocatable_float_codes() const {
    return allocatable_float_codes_;
  }

  // Aliasing calculations for floating point registers, when fp_aliasing_kind()
  // is COMBINE. Currently only implemented for kFloat32, or kFloat64 reps.
  // Returns the number of aliases, and if > 0, alias_base_index is set to the
  // index of the first alias.
  int GetAliases(MachineRepresentation rep, int index,
                 MachineRepresentation other_rep, int* alias_base_index) const;
  // Returns a value indicating whether two registers alias each other, when
  // fp_aliasing_kind() is COMBINE. Currently only implemented for kFloat32, or
  // kFloat64 reps.
  bool AreAliases(MachineRepresentation rep, int index,
                  MachineRepresentation other_rep, int other_index) const;

 private:
  const int num_general_registers_;
  int num_float_registers_;
  const int num_double_registers_;
  int num_allocatable_general_registers_;
  int num_allocatable_double_registers_;
  int num_allocatable_float_registers_;
  AliasingKind fp_aliasing_kind_;
  int32_t allocatable_general_codes_mask_;
  int32_t allocatable_double_codes_mask_;
  const int* allocatable_general_codes_;
  const int* allocatable_double_codes_;
  int allocatable_float_codes_[kMaxFPRegisters];
  char const* const* general_register_names_;
  char const* const* float_register_names_;
  char const* const* double_register_names_;
};

}  // namespace internal
}  // namespace v8

#endif  // V8_COMPILER_REGISTER_CONFIGURATION_H_
