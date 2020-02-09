.section .rodata
.align 2

prime:    .space 412  ! prime[103]
.print:
  .string "%d "
  .align 2
  .text

.print_enter:
  .string "\n"
  .text
  .global main

! $r2 == load_prime[i]
! $r9 = i, $r3 = j
! $r8 = prime_address, $r6 = cnt
! $r4 = #1
! $r5 = #0
! $r7 == iterator

main:
    movi    $r9, #1             ! i = 1
    la      $r8, prime          ! r8 = prime address
    movi    $r2, #0
    movi    $r4, #1

!for(int i = 0; i < 100; i++)
.set_prime:
    move    $r7, $r9
    sltsi   $r7, $r7, #100       ! i < 100
    beqz    $r7, .init           ! if start to for_1
    sw      $r4, [$r8+($r9<<2)]
    addi    $r9, $r9, #1         ! i++
    b       .set_prime

!----------------------------------------------------------
!for(int i = 2; i < 100; i++)
.init:
     movi    $r9, #1                 ! for(int i = 1)
     movi    $r4, #1
     movi    $r5, #0

.for_1:
    addi    $r9, $r9, #1             ! i++
    move    $r7, $r9
    sltsi   $r7, $r7, #100           ! i < 100
    beqz    $r7, .print_prime_code   ! if no return



    lw      $r2, [$r8+($r9<<2)]      ! $r2 = prime[i]
    beq     $r2, $r4, .go_for_2      ! if prime[i] = 1, jump to go_for_2
    b       .for_1


.go_for_2:
    move   $r3, $r9             ! j = i
    add    $r3, $r3, $r9        ! j = i+i


.for_2:
    move    $r7, $r3
    sltsi   $r7, $r7, #100       ! j < 100

    sw      $r5, [$r8+($r3<<2)]  ! prime[j] = 0
    add     $r3, $r3, $r9        ! j = j + i
    bnez    $r7, .for_2
    b       .for_1

!----------------------------------------------------------
!for(int i = 2; i < 100; i++)
.print_prime_code:
    movi $r9, #1                 ! i = 1
    movi $r6, #0                 ! cnt = 0

.i_add1:
    addi $r9, $r9, #1            ! i++

.is_prime:

    lw $r4, [$r8+($r9<<2)]       ! prime[i]

    slti $r7, $r9, #100          ! i < 100
    beqz $r7, .end_code

    beqz $r4, .i_add1            ! if(prime[i])

    la   $r0, .print
    move $r1, $r9
    bal  printf                  ! printf("%d")

    addi $r6, $r6, #1            ! cnt += 1
    slti $r5, $r6, #10           ! cnt == 10 ,printf("\n");
    bnez $r5,.i_add1


.print_enter_code:
    la       $r0, .print_enter
    bal      printf              ! printf("\n")
    movi     $r6, #0             ! cnt=0
    b        .i_add1


!------------------------------------------------
.end_code:
    .end
