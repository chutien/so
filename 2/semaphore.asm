global proberen
global verhogen
global proberen_time
extern get_os_time

section .text
; Proberen gets semaphore pointer in rdi and value in esi.
align 8
proberen:
  cmp dword [rdi], esi          ; Busy waiting.
  jl proberen                   ; Wait condition.
  lock sub dword [rdi], esi     ; Atomically decrement sempahore by value.
  jge proberen_end              ; If semaphore was open then return
  lock add dword [rdi], esi     ; else atomically undo decrementation of sempahore by value
  jmp proberen                  ; and busily wait.
proberen_end:
  ret

; Verhogen gets semaphore pointer in rdi and value in esi.
align 8
verhogen:
  lock add dword [rdi], esi     ; Atomically increment semaphore.
  ret

; Proberen time gets semaphore pointer in rdi and value in esi.
align 8
proberen_time:
  mov r12, rdi                  ; Preserve semaphore pointer.
  mov r13d, esi                 ; Preserve value.
  sub rsp, 8
  call get_os_time
  add rsp, 8
  mov r14, rax                  ; Preserve starting OS time.
  mov rdi, r12                  ; Use preserved semaphore pointer.
  mov esi, r13d                 ; Use preserved value address
  sub rsp, 8
  call proberen
  call get_os_time
  add rsp, 8
  sub rax, r14                  ; Calculate OS time difference.
  ret
