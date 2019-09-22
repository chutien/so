global get_os_time, add_os_time, stack_pointer

section .bss

; Globalny licznik czasu
align 8
global_os_timer: resq 1

; Tu funkcja get_os_time umieszcza wartość rsp, jeśli stos nie jest prawidłowo
; wyrównany przy jej wywołaniu. Jeśli jest prawidłowo wyrównany, to ta zmienna
; ma wartość zero - zero nigdy nie jest prawidłową wartością wskaźnika stosu.
align 8
stack_pointer:   resq 1

section .text

align 16
get_os_time:
  ; Sprawdź, czy stos jest poprawnie wyrównany.
  mov   rax, rsp
  and   al, 0xf
  cmp   al, 0x8
  ; Zapamiętaj wartość rsp, jeśli stos nie jest poprawnie wyrównany.
  je    stack_properly_aligned
  mov   [stack_pointer], rsp
stack_properly_aligned:
  ; Zapisz złośliwe wartości w rejestrach, które można zmieniać.
  mov   rdi, 0
  mov   rsi, 999999
  mov   r10, 0x5555555555555555
  mov   r11, 0xaaaaaaaaaaaaaaaa
  xor   rdx, r10
  xor   rcx, r11
  xor   r8,  r10
  xor   r9,  r11
  ; Zwiększ czas systemowy i zwróć jego poprzednią wartość.
  mov   eax, 1
  lock \
  xadd  [global_os_timer], rax
  ret

align 16
add_os_time:
  lock \
  add   [global_os_timer], rdi
  ret
