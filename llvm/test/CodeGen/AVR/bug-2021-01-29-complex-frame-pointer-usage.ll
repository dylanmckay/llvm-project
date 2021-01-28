; RUN: llc < %s -march=avr -mcpu=atmega328 -filetype=obj -o /dev/null --print-options 2>&1 | FileCheck %s --check-prefix=CHECK-ERROR
; RUN: llc < %s -march=avr -mcpu=atmega328 -filetype=obj | llvm-objdump -S - | FileCheck %s --check-prefix=CHECK-OBJDUMP

; This test verifies that the AVR backend can successfully lower code
; which is very register heavy, containing many references to the frame
; pointer.
;
; Before this bug was fixed, this testcase would fail with the message:
;
;   LLVM ERROR: Not supported instr: <MCInst XXX <MCOperand Reg:1> <MCOperand Imm:15> <MCOperand Reg:53>>
;
;     where XXX is the OpCode of either the STDWSPQRr instruction or the STDSPQRr instruction.
;
; The issue was that the ISel lowering pass would lower many stack slot stores to these
; instructions, but the frame pointer elimination code (that is designed to rewrite these two
; instructions to real instructions) was only designed to run for STDWSPQRr/STDSPQRr instructions
; that appeared in the basic blocks that contained the FrameSetup/FrameDestroy instructions.
;
; The bug was fixed by modifying the code so that it unconditionally runs on STDWSPQRr/STDSPQRr
; instructions and always expands them with the relevant STDWPtrQRr or STDPtrQRr instructions.
;
; This bug was originally discovered due to the Rust compiler_builtins library. Its 0.1.37 release
; contained a 128-bit software division/remainder routine that exercised this buggy branch in the code.

; CHECK-ERROR-NOT: LLVM ERROR: Not supported instr

declare { i128, i128 } @div_rem_u128(i128, i128) addrspace(1)

; CHECK-OBJDUMP-LABEL: main
define i128 @main(i128 %a, i128 %b) addrspace(1) {
start:
  %b_neg = icmp slt i128 %b, 0
  %divisor = select i1 %b_neg, i128 0, i128 %b
  %result = tail call fastcc addrspace(1) { i128, i128 } @div_rem_u128(i128 undef, i128 %divisor)

  ; CHECK-OBJDUMP: ret
  ret i128 0
}

