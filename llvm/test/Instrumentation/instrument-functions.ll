; RUN: opt -passes=instrument-functions -S %s | FileCheck %s

define i32 @add(i32 %x, i32 %y) {
entry:
  %res = add i32 %x, %y
  ret i32 %res
}

; CHECK: call void @instrument_start()
; CHECK: call void @instrument_end()
