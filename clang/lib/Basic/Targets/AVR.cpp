//===--- AVR.cpp - Implement AVR target feature support -------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements AVR TargetInfo objects.
//
//===----------------------------------------------------------------------===//

#include "AVR.h"
#include "clang/Basic/AVR.h"
#include "clang/Basic/MacroBuilder.h"
#include "llvm/ADT/StringSwitch.h"

using namespace clang;
using namespace clang::targets;
using namespace clang::targets::avr;

// This list should be kept up-to-date with AVRDevices.td in LLVM.
static MCUInfo AVRMcus[] = {
    {"at90s1200", "__AVR_AT90S1200__", ADDR_UNSET},
    {"attiny11", "__AVR_ATtiny11__", ADDR_UNSET},
    {"attiny12", "__AVR_ATtiny12__", ADDR_UNSET},
    {"attiny15", "__AVR_ATtiny15__", ADDR_UNSET},
    {"attiny28", "__AVR_ATtiny28__", ADDR_UNSET},
    {"at90s2313", "__AVR_AT90S2313__", ADDR_UNSET},
    {"at90s2323", "__AVR_AT90S2323__", ADDR_UNSET},
    {"at90s2333", "__AVR_AT90S2333__", ADDR_UNSET},
    {"at90s2343", "__AVR_AT90S2343__", ADDR_UNSET},
    {"attiny22", "__AVR_ATtiny22__", ADDR_UNSET},
    {"attiny26", "__AVR_ATtiny26__", ADDR_UNSET},
    {"at86rf401", "__AVR_AT86RF401__", ADDR_UNSET},
    {"at90s4414", "__AVR_AT90S4414__", ADDR_UNSET},
    {"at90s4433", "__AVR_AT90S4433__", ADDR_UNSET},
    {"at90s4434", "__AVR_AT90S4434__", ADDR_UNSET},
    {"at90s8515", "__AVR_AT90S8515__", ADDR_UNSET},
    {"at90c8534", "__AVR_AT90c8534__", ADDR_UNSET},
    {"at90s8535", "__AVR_AT90S8535__", ADDR_UNSET},
    {"ata5272", "__AVR_ATA5272__", ADDR_UNSET},
    {"attiny13", "__AVR_ATtiny13__", ADDR_UNSET},
    {"attiny13a", "__AVR_ATtiny13A__", ADDR_UNSET},
    {"attiny2313", "__AVR_ATtiny2313__", ADDR_UNSET},
    {"attiny2313a", "__AVR_ATtiny2313A__", ADDR_UNSET},
    {"attiny24", "__AVR_ATtiny24__", ADDR_UNSET},
    {"attiny24a", "__AVR_ATtiny24A__", ADDR_UNSET},
    {"attiny4313", "__AVR_ATtiny4313__", ADDR_UNSET},
    {"attiny44", "__AVR_ATtiny44__", ADDR_UNSET},
    {"attiny44a", "__AVR_ATtiny44A__", ADDR_UNSET},
    {"attiny84", "__AVR_ATtiny84__", ADDR_UNSET},
    {"attiny84a", "__AVR_ATtiny84A__", ADDR_UNSET},
    {"attiny25", "__AVR_ATtiny25__", ADDR_UNSET},
    {"attiny45", "__AVR_ATtiny45__", ADDR_UNSET},
    {"attiny85", "__AVR_ATtiny85__", ADDR_UNSET},
    {"attiny261", "__AVR_ATtiny261__", ADDR_UNSET},
    {"attiny261a", "__AVR_ATtiny261A__", ADDR_UNSET},
    {"attiny461", "__AVR_ATtiny461__", ADDR_UNSET},
    {"attiny461a", "__AVR_ATtiny461A__", ADDR_UNSET},
    {"attiny861", "__AVR_ATtiny861__", ADDR_UNSET},
    {"attiny861a", "__AVR_ATtiny861A__", ADDR_UNSET},
    {"attiny87", "__AVR_ATtiny87__", ADDR_UNSET},
    {"attiny43u", "__AVR_ATtiny43U__", ADDR_UNSET},
    {"attiny48", "__AVR_ATtiny48__", ADDR_UNSET},
    {"attiny88", "__AVR_ATtiny88__", ADDR_UNSET},
    {"attiny828", "__AVR_ATtiny828__", ADDR_UNSET},
    {"at43usb355", "__AVR_AT43USB355__", ADDR_UNSET},
    {"at76c711", "__AVR_AT76C711__", ADDR_UNSET},
    {"atmega103", "__AVR_ATmega103__", ADDR_UNSET},
    {"at43usb320", "__AVR_AT43USB320__", ADDR_UNSET},
    {"attiny167", "__AVR_ATtiny167__", ADDR_UNSET},
    {"at90usb82", "__AVR_AT90USB82__", ADDR_UNSET},
    {"at90usb162", "__AVR_AT90USB162__", ADDR_UNSET},
    {"ata5505", "__AVR_ATA5505__", ADDR_UNSET},
    {"atmega8u2", "__AVR_ATmega8U2__", ADDR_UNSET},
    {"atmega16u2", "__AVR_ATmega16U2__", ADDR_UNSET},
    {"atmega32u2", "__AVR_ATmega32U2__", ADDR_UNSET},
    {"attiny1634", "__AVR_ATtiny1634__", ADDR_UNSET},
    {"atmega8", "__AVR_ATmega8__", ADDR_UNSET},
    {"ata6289", "__AVR_ATA6289__", ADDR_UNSET},
    {"atmega8a", "__AVR_ATmega8A__", ADDR_UNSET},
    {"ata6285", "__AVR_ATA6285__", ADDR_UNSET},
    {"ata6286", "__AVR_ATA6286__", ADDR_UNSET},
    {"atmega48", "__AVR_ATmega48__", ADDR_UNSET},
    {"atmega48a", "__AVR_ATmega48A__", ADDR_UNSET},
    {"atmega48pa", "__AVR_ATmega48PA__", ADDR_UNSET},
    {"atmega48p", "__AVR_ATmega48P__", ADDR_UNSET},
    {"atmega88", "__AVR_ATmega88__", ADDR_UNSET},
    {"atmega88a", "__AVR_ATmega88A__", ADDR_UNSET},
    {"atmega88p", "__AVR_ATmega88P__", ADDR_UNSET},
    {"atmega88pa", "__AVR_ATmega88PA__", ADDR_UNSET},
    {"atmega8515", "__AVR_ATmega8515__", ADDR_UNSET},
    {"atmega8535", "__AVR_ATmega8535__", ADDR_UNSET},
    {"atmega8hva", "__AVR_ATmega8HVA__", ADDR_UNSET},
    {"at90pwm1", "__AVR_AT90PWM1__", ADDR_UNSET},
    {"at90pwm2", "__AVR_AT90PWM2__", ADDR_UNSET},
    {"at90pwm2b", "__AVR_AT90PWM2B__", ADDR_UNSET},
    {"at90pwm3", "__AVR_AT90PWM3__", ADDR_UNSET},
    {"at90pwm3b", "__AVR_AT90PWM3B__", ADDR_UNSET},
    {"at90pwm81", "__AVR_AT90PWM81__", ADDR_UNSET},
    {"ata5790", "__AVR_ATA5790__", ADDR_UNSET},
    {"ata5795", "__AVR_ATA5795__", ADDR_UNSET},
    {"atmega16", "__AVR_ATmega16__", ADDR_UNSET},
    {"atmega16a", "__AVR_ATmega16A__", ADDR_UNSET},
    {"atmega161", "__AVR_ATmega161__", ADDR_UNSET},
    {"atmega162", "__AVR_ATmega162__", ADDR_UNSET},
    {"atmega163", "__AVR_ATmega163__", ADDR_UNSET},
    {"atmega164a", "__AVR_ATmega164A__", ADDR_UNSET},
    {"atmega164p", "__AVR_ATmega164P__", ADDR_UNSET},
    {"atmega164pa", "__AVR_ATmega164PA__", ADDR_UNSET},
    {"atmega165", "__AVR_ATmega165__", ADDR_UNSET},
    {"atmega165a", "__AVR_ATmega165A__", ADDR_UNSET},
    {"atmega165p", "__AVR_ATmega165P__", ADDR_UNSET},
    {"atmega165pa", "__AVR_ATmega165PA__", ADDR_UNSET},
    {"atmega168", "__AVR_ATmega168__", ADDR_UNSET},
    {"atmega168a", "__AVR_ATmega168A__", ADDR_UNSET},
    {"atmega168p", "__AVR_ATmega168P__", ADDR_UNSET},
    {"atmega168pa", "__AVR_ATmega168PA__", ADDR_UNSET},
    {"atmega169", "__AVR_ATmega169__", ADDR_UNSET},
    {"atmega169a", "__AVR_ATmega169A__", ADDR_UNSET},
    {"atmega169p", "__AVR_ATmega169P__", ADDR_UNSET},
    {"atmega169pa", "__AVR_ATmega169PA__", ADDR_UNSET},
    {"atmega32", "__AVR_ATmega32__", ADDR_UNSET},
    {"atmega32a", "__AVR_ATmega32A__", ADDR_UNSET},
    {"atmega323", "__AVR_ATmega323__", ADDR_UNSET},
    {"atmega324a", "__AVR_ATmega324A__", ADDR_UNSET},
    {"atmega324p", "__AVR_ATmega324P__", ADDR_UNSET},
    {"atmega324pa", "__AVR_ATmega324PA__", ADDR_UNSET},
    {"atmega325", "__AVR_ATmega325__", ADDR_UNSET},
    {"atmega325a", "__AVR_ATmega325A__", ADDR_UNSET},
    {"atmega325p", "__AVR_ATmega325P__", ADDR_UNSET},
    {"atmega325pa", "__AVR_ATmega325PA__", ADDR_UNSET},
    {"atmega3250", "__AVR_ATmega3250__", ADDR_UNSET},
    {"atmega3250a", "__AVR_ATmega3250A__", ADDR_UNSET},
    {"atmega3250p", "__AVR_ATmega3250P__", ADDR_UNSET},
    {"atmega3250pa", "__AVR_ATmega3250PA__", ADDR_UNSET},
    {"atmega328", "__AVR_ATmega328__", ADDR_UNSET},
    {"atmega328p", "__AVR_ATmega328P__", ADDR_UNSET},
    {"atmega329", "__AVR_ATmega329__", ADDR_UNSET},
    {"atmega329a", "__AVR_ATmega329A__", ADDR_UNSET},
    {"atmega329p", "__AVR_ATmega329P__", ADDR_UNSET},
    {"atmega329pa", "__AVR_ATmega329PA__", ADDR_UNSET},
    {"atmega3290", "__AVR_ATmega3290__", ADDR_UNSET},
    {"atmega3290a", "__AVR_ATmega3290A__", ADDR_UNSET},
    {"atmega3290p", "__AVR_ATmega3290P__", ADDR_UNSET},
    {"atmega3290pa", "__AVR_ATmega3290PA__", ADDR_UNSET},
    {"atmega406", "__AVR_ATmega406__", ADDR_UNSET},
    {"atmega64", "__AVR_ATmega64__", ADDR_UNSET},
    {"atmega64a", "__AVR_ATmega64A__", ADDR_UNSET},
    {"atmega640", "__AVR_ATmega640__", ADDR_UNSET},
    {"atmega644", "__AVR_ATmega644__", ADDR_UNSET},
    {"atmega644a", "__AVR_ATmega644A__", ADDR_UNSET},
    {"atmega644p", "__AVR_ATmega644P__", ADDR_UNSET},
    {"atmega644pa", "__AVR_ATmega644PA__", ADDR_UNSET},
    {"atmega645", "__AVR_ATmega645__", ADDR_UNSET},
    {"atmega645a", "__AVR_ATmega645A__", ADDR_UNSET},
    {"atmega645p", "__AVR_ATmega645P__", ADDR_UNSET},
    {"atmega649", "__AVR_ATmega649__", ADDR_UNSET},
    {"atmega649a", "__AVR_ATmega649A__", ADDR_UNSET},
    {"atmega649p", "__AVR_ATmega649P__", ADDR_UNSET},
    {"atmega6450", "__AVR_ATmega6450__", ADDR_UNSET},
    {"atmega6450a", "__AVR_ATmega6450A__", ADDR_UNSET},
    {"atmega6450p", "__AVR_ATmega6450P__", ADDR_UNSET},
    {"atmega6490", "__AVR_ATmega6490__", ADDR_UNSET},
    {"atmega6490a", "__AVR_ATmega6490A__", ADDR_UNSET},
    {"atmega6490p", "__AVR_ATmega6490P__", ADDR_UNSET},
    {"atmega64rfr2", "__AVR_ATmega64RFR2__", ADDR_UNSET},
    {"atmega644rfr2", "__AVR_ATmega644RFR2__", ADDR_UNSET},
    {"atmega16hva", "__AVR_ATmega16HVA__", ADDR_UNSET},
    {"atmega16hva2", "__AVR_ATmega16HVA2__", ADDR_UNSET},
    {"atmega16hvb", "__AVR_ATmega16HVB__", ADDR_UNSET},
    {"atmega16hvbrevb", "__AVR_ATmega16HVBREVB__", ADDR_UNSET},
    {"atmega32hvb", "__AVR_ATmega32HVB__", ADDR_UNSET},
    {"atmega32hvbrevb", "__AVR_ATmega32HVBREVB__", ADDR_UNSET},
    {"atmega64hve", "__AVR_ATmega64HVE__", ADDR_UNSET},
    {"at90can32", "__AVR_AT90CAN32__", ADDR_UNSET},
    {"at90can64", "__AVR_AT90CAN64__", ADDR_UNSET},
    {"at90pwm161", "__AVR_AT90PWM161__", ADDR_UNSET},
    {"at90pwm216", "__AVR_AT90PWM216__", ADDR_UNSET},
    {"at90pwm316", "__AVR_AT90PWM316__", ADDR_UNSET},
    {"atmega32c1", "__AVR_ATmega32C1__", ADDR_UNSET},
    {"atmega64c1", "__AVR_ATmega64C1__", ADDR_UNSET},
    {"atmega16m1", "__AVR_ATmega16M1__", ADDR_UNSET},
    {"atmega32m1", "__AVR_ATmega32M1__", ADDR_UNSET},
    {"atmega64m1", "__AVR_ATmega64M1__", ADDR_UNSET},
    {"atmega16u4", "__AVR_ATmega16U4__", ADDR_UNSET},
    {"atmega32u4", "__AVR_ATmega32U4__", ADDR_UNSET},
    {"atmega32u6", "__AVR_ATmega32U6__", ADDR_UNSET},
    {"at90usb646", "__AVR_AT90USB646__", ADDR_UNSET},
    {"at90usb647", "__AVR_AT90USB647__", ADDR_UNSET},
    {"at90scr100", "__AVR_AT90SCR100__", ADDR_UNSET},
    {"at94k", "__AVR_AT94K__", ADDR_UNSET},
    {"m3000", "__AVR_AT000__", ADDR_UNSET},
    {"atmega128", "__AVR_ATmega128__", ADDR_UNSET},
    {"atmega128a", "__AVR_ATmega128A__", ADDR_UNSET},
    {"atmega1280", "__AVR_ATmega1280__", ADDR_UNSET},
    {"atmega1281", "__AVR_ATmega1281__", ADDR_UNSET},
    {"atmega1284", "__AVR_ATmega1284__", ADDR_UNSET},
    {"atmega1284p", "__AVR_ATmega1284P__", ADDR_UNSET},
    {"atmega128rfa1", "__AVR_ATmega128RFA1__", ADDR_UNSET},
    {"atmega128rfr2", "__AVR_ATmega128RFR2__", ADDR_UNSET},
    {"atmega1284rfr2", "__AVR_ATmega1284RFR2__", ADDR_UNSET},
    {"at90can128", "__AVR_AT90CAN128__", ADDR_UNSET},
    {"at90usb1286", "__AVR_AT90USB1286__", ADDR_UNSET},
    {"at90usb1287", "__AVR_AT90USB1287__", ADDR_UNSET},
    {"atmega2560", "__AVR_ATmega2560__", ADDR_UNSET},
    {"atmega2561", "__AVR_ATmega2561__", ADDR_UNSET},
    {"atmega256rfr2", "__AVR_ATmega256RFR2__", ADDR_UNSET},
    {"atmega2564rfr2", "__AVR_ATmega2564RFR2__", ADDR_UNSET},
    {"atxmega16a4", "__AVR_ATxmega16A4__", ADDR_UNSET},
    {"atxmega16a4u", "__AVR_ATxmega16a4U__", ADDR_UNSET},
    {"atxmega16c4", "__AVR_ATxmega16C4__", ADDR_UNSET},
    {"atxmega16d4", "__AVR_ATxmega16D4__", ADDR_UNSET},
    {"atxmega32a4", "__AVR_ATxmega32A4__", ADDR_UNSET},
    {"atxmega32a4u", "__AVR_ATxmega32A4U__", ADDR_UNSET},
    {"atxmega32c4", "__AVR_ATxmega32C4__", ADDR_UNSET},
    {"atxmega32d4", "__AVR_ATxmega32D4__", ADDR_UNSET},
    {"atxmega32e5", "__AVR_ATxmega32E5__", ADDR_UNSET},
    {"atxmega16e5", "__AVR_ATxmega16E5__", ADDR_UNSET},
    {"atxmega8e5", "__AVR_ATxmega8E5__", ADDR_UNSET},
    {"atxmega32x1", "__AVR_ATxmega32X1__", ADDR_UNSET},
    {"atxmega64a3", "__AVR_ATxmega64A3__", ADDR_UNSET},
    {"atxmega64a3u", "__AVR_ATxmega64A3U__", ADDR_UNSET},
    {"atxmega64a4u", "__AVR_ATxmega64A4U__", ADDR_UNSET},
    {"atxmega64b1", "__AVR_ATxmega64B1__", ADDR_UNSET},
    {"atxmega64b3", "__AVR_ATxmega64B3__", ADDR_UNSET},
    {"atxmega64c3", "__AVR_ATxmega64C3__", ADDR_UNSET},
    {"atxmega64d3", "__AVR_ATxmega64D3__", ADDR_UNSET},
    {"atxmega64d4", "__AVR_ATxmega64D4__", ADDR_UNSET},
    {"atxmega64a1", "__AVR_ATxmega64A1__", ADDR_UNSET},
    {"atxmega64a1u", "__AVR_ATxmega64A1U__", ADDR_UNSET},
    {"atxmega128a3", "__AVR_ATxmega128A3__", ADDR_UNSET},
    {"atxmega128a3u", "__AVR_ATxmega128A3U__", ADDR_UNSET},
    {"atxmega128b1", "__AVR_ATxmega128B1__", ADDR_UNSET},
    {"atxmega128b3", "__AVR_ATxmega128B3__", ADDR_UNSET},
    {"atxmega128c3", "__AVR_ATxmega128C3__", ADDR_UNSET},
    {"atxmega128d3", "__AVR_ATxmega128D3__", ADDR_UNSET},
    {"atxmega128d4", "__AVR_ATxmega128D4__", ADDR_UNSET},
    {"atxmega192a3", "__AVR_ATxmega192A3__", ADDR_UNSET},
    {"atxmega192a3u", "__AVR_ATxmega192A3U__", ADDR_UNSET},
    {"atxmega192c3", "__AVR_ATxmega192C3__", ADDR_UNSET},
    {"atxmega192d3", "__AVR_ATxmega192D3__", ADDR_UNSET},
    {"atxmega256a3", "__AVR_ATxmega256A3__", ADDR_UNSET},
    {"atxmega256a3u", "__AVR_ATxmega256A3U__", ADDR_UNSET},
    {"atxmega256a3b", "__AVR_ATxmega256A3B__", ADDR_UNSET},
    {"atxmega256a3bu", "__AVR_ATxmega256A3BU__", ADDR_UNSET},
    {"atxmega256c3", "__AVR_ATxmega256C3__", ADDR_UNSET},
    {"atxmega256d3", "__AVR_ATxmega256D3__", ADDR_UNSET},
    {"atxmega384c3", "__AVR_ATxmega384C3__", ADDR_UNSET},
    {"atxmega384d3", "__AVR_ATxmega384D3__", ADDR_UNSET},
    {"atxmega128a1", "__AVR_ATxmega128A1__", ADDR_UNSET},
    {"atxmega128a1u", "__AVR_ATxmega128A1U__", ADDR_UNSET},
    {"atxmega128a4u", "__AVR_ATxmega128a4U__", ADDR_UNSET},
    {"attiny4", "__AVR_ATtiny4__", ADDR_UNSET},
    {"attiny5", "__AVR_ATtiny5__", ADDR_UNSET},
    {"attiny9", "__AVR_ATtiny9__", ADDR_UNSET},
    {"attiny10", "__AVR_ATtiny10__", ADDR_UNSET},
    {"attiny20", "__AVR_ATtiny20__", ADDR_UNSET},
    {"attiny40", "__AVR_ATtiny40__", ADDR_UNSET},
    {"attiny102", "__AVR_ATtiny102__", ADDR_UNSET},
    {"attiny104", "__AVR_ATtiny104__", ADDR_UNSET},
};


static constexpr llvm::StringLiteral ValidFamilyNames[] = {
    "avr1",      "avr2",      "avr25",     "avr3",      "avr31",
    "avr35",     "avr4",      "avr5",      "avr51",     "avr6",
    "avrxmega1", "avrxmega2", "avrxmega3", "avrxmega4", "avrxmega5",
    "avrxmega6", "avrxmega7", "avrtiny"};

bool AVRTargetInfo::isValidCPUName(StringRef Name) const {
  bool IsFamily =
      llvm::find(ValidFamilyNames, Name) != std::end(ValidFamilyNames);

  bool IsMCU =
      llvm::find_if(AVRMcus, [&](const MCUInfo &Info) {
        return Info.getName() == Name;
      }) != std::end(AVRMcus);
  return IsFamily || IsMCU;
}

void AVRTargetInfo::fillValidCPUList(SmallVectorImpl<StringRef> &Values) const {
  Values.append(std::begin(ValidFamilyNames), std::end(ValidFamilyNames));
  for (const MCUInfo &Info : AVRMcus)
    Values.push_back(Info.getName());
}

void AVRTargetInfo::getTargetDefines(const LangOptions &Opts,
                                     MacroBuilder &Builder) const {
  Builder.defineMacro("AVR");
  Builder.defineMacro("__AVR");
  Builder.defineMacro("__AVR__");
  Builder.defineMacro("__ELF__");

  if (!this->CPU.empty()) {
    auto It = llvm::find_if(
        AVRMcus, [&](const MCUInfo &Info) { return Info.getName() == this->CPU; });

    if (It != std::end(AVRMcus))
      Builder.defineMacro(It->getDefineName());
  }
}

namespace clang {
namespace targets {

llvm::Optional<MCUInfo*> avr::MCUInfo::find(llvm::StringRef McuName) {
  for (MCUInfo McuInfo : AVRMcus) {
    if (McuName == McuInfo.Name) {
      return llvm::Optional<MCUInfo*>(&McuInfo);
    }
  }

  return llvm::Optional<MCUInfo*>();
}

} // namespace targets
} // namespace clang

