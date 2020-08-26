//===---- AVR.h - Utilities for compiling AVR code  -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_BASIC_AVR_H
#define LLVM_CLANG_BASIC_AVR_H

namespace clang {
namespace targets {
namespace avr {
  const unsigned ADDR_UNSET = ~0;

  /// Information about a specific microcontroller.
  class LLVM_LIBRARY_VISIBILITY MCUInfo {
  public:
    MCUInfo(const char *Name, const char *DefineName,
            unsigned SectionAddressData)
      : Name(Name), DefineName(DefineName),
        SectionAddressData(SectionAddressData) { }

    static llvm::Optional<MCUInfo*> find(llvm::StringRef McuName);

    StringRef getName() const { return StringRef(this->Name); }
    StringRef getDefineName() const { return StringRef(this->DefineName); }

    llvm::Optional<unsigned> getSectionAddressData() const {
      if (this->SectionAddressData == ADDR_UNSET)
        return llvm::Optional<unsigned>();

      return llvm::Optional<unsigned>(this->SectionAddressData);
    }

  private:

    const char *Name;
    const char *DefineName;
    unsigned SectionAddressData;
  };

} // end namespace avr
} // end namespace targets
} // end namespace clang

#endif

