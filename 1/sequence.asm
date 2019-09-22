section .data
  SYS_READ equ 0
  SYS_OPEN equ 2
  SYS_CLOSE equ 3
  SYS_EXIT equ 60
  BUF_SIZE equ 8192

section .bss
buf: resb BUF_SIZE
fd: resb 8

section .text
  global _start
_start:
  pop rcx
  cmp rcx, 2                    ; check arguments number
  jne exit_one
  add rsp, 8                    ; move pointer to file name
  pop rdi                       ; get file name
  mov rsi, 0                    ; read only flag
  mov rax, SYS_OPEN             ; open file
  syscall
  test rax, rax                 ; check if opened successfully
  je exit_one                   ; if not exit with 1
  mov [fd], rax                 ; keep file descriptor in fd
  call read_file
  xor r12, r12                  ; prepare registers for first permutation
  xor r13, r13
  xor r14, r14
  xor r15, r15
  xor bh, bh                    ; zero flag, 1 if last byte was 0
first_permutation:
  test rdi, rdi                 ; check if there are no bytes left
  jz exit_one
  mov cl, byte [rsi]
  and cl, 63                    ; keep the remainder of number in cl
  mov rax, 1                    ; initiate rax before bit shifting
  sal rax, cl                   ; all bits set to 0 apart from proper bit
  cmp byte [rsi], 192           ; numbers from 192-255 are marked in r12 
  jb fp_below_192
  test rax, r12                 ; if that number has already been marked
  jnz exit_one                  ; then
  add r12, rax                  ; else
  jmp first_advance
fp_below_192:
  cmp byte [rsi], 128           ; numbers from 128-191 are marked in r12
  jb fp_below_128
  test rax, r13
  jnz exit_one
  add r13, rax
  jmp first_advance
fp_below_128:
  cmp byte [rsi], 64            ; numbers from 64-127 are marked in r12
  jb fp_below_64
  test rax, r14
  jnz exit_one
  add r14, rax
  jmp first_advance
fp_below_64:
  cmp byte [rsi], 0             ; numbers from 1-63 are marked in r12
  je first_zero
  test rax, r15
  jnz exit_one
  add r15, rax
first_advance:
  inc rsi                        ; take next byte
  dec rdi
  jmp first_permutation
first_zero:
  mov bh, 1
next_permutation:
  xor r8, r8                    ; prepare registers for next permutations
  xor r9, r9
  xor r10, r10
  xor r11, r11
next_advance:
  inc rsi
  dec rdi
next_check:
  test rdi, rdi                 ; check if all bytes were read from buffer
  jnz next_continue             ; else continue
  test bl, bl                   ; check if end of file
  jnz next_exit                 ; else read new portion of the file
  call read_file
  jmp next_check
next_continue:
  xor bh, bh
  mov cl, byte [rsi]
  and cl, 63
  mov rax, 1
  sal rax, cl
  cmp byte [rsi], 192
  jb np_below_192
  test rax, r8
  jnz exit_one
  add r8, rax                   ;  mark 192-255 on r8
  jmp next_advance
np_below_192:
  cmp byte [rsi], 128
  jb np_below_128
  test rax, r9
  jnz exit_one
  add r9, rax                   ; mark 128-191 on r9
  jmp next_advance
np_below_128:
  cmp byte [rsi], 64
  jb np_below_64
  test rax, r10
  jnz exit_one
  add r10, rax                  ; mark 64-127 on r10
  jmp next_advance
np_below_64:
  cmp byte [rsi], 0
  je next_zero
  test rax, r11
  jnz exit_one
  add r11, rax                  ; mark 0-63 on r11
  jne next_advance
next_zero:
  mov bh, 1
  xor r8, r12                  ; compare this permutation with the first one
  jne exit_one                 ; if any registers do not match then sequence is incorrect
  xor r9, r13
  jne exit_one
  xor r10, r14
  jne exit_one
  xor r11, r15
  jne exit_one
  jmp next_advance
next_exit:
  test bh, 1                   ; check if last byte was 0
  je exit_one                  ; if not then sequence is incorrect
  jmp exit                     ; else last permutation matches and so the sequence is correct
exit:
  call close_file
  mov rax, SYS_EXIT
  xor rdi, rdi
  syscall
exit_one:
  call close_file
  mov rax, SYS_EXIT
  mov rdi, 1
  syscall
read_file:
  push r11                      ; remember r11 before syscall
  mov rdi, [fd]
  mov rax, SYS_READ
  mov rsi, buf
  mov rdx, BUF_SIZE
  syscall
  pop r11                       ; recall r11 after syscall
  cmp rax, BUF_SIZE
  setne bl                      ; eof flag marks end of file
  mov rdi, rax                  ; keep # of bytes read in rdi
  ret
close_file:
  mov rax, SYS_CLOSE
  mov rdi, [fd]
  syscall
  ret