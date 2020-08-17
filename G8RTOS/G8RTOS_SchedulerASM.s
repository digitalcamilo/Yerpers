; +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+
; | SUMMARY: G8RTOS_SchedulerASM.s                                  |
; | Holds all ASM functions needed for the scheduler				|
; +-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+-----+

	; Functions Defined
	.def G8RTOS_Start, PendSV_Handler

	; Dependencies
	.ref CurrentlyRunningThread, G8RTOS_Scheduler

	.thumb		; Set to thumb mode
	.align 2	; Align by 2 bytes (thumb mode uses allignment by 2 or 4)
	.text		; Text section

; Need to have the address defined in file 
; (label needs to be close enough to asm code to be reached with PC relative addressing)
RunningPtr: .field CurrentlyRunningThread, 32

; G8RTOS_Start
;	Sets the first thread to be the currently running thread
;	Starts the currently running thread by setting Link Register to tcb's Program Counter
G8RTOS_Start:

	.asmfunc

	; load thread SP
	ldr r0, RunningPtr		; load currently running thread
	ldr r1, [r0]			; r1 = RunningPtr
	ldr SP, [r1]			; load new thread SP

	; restore initial context
	pop {r4 - r11}			; pop r4 - r11
	pop {r0 - r3}			; pop r0 - r3
	pop {r12}				; pop r12
	add SP, SP, #4			; discard LR
	pop {LR}				; pop thread PC to LR
	add SP, SP, #4			; discard PSR

	CPSIE I					; enable interrupts

	bx LR					; start thread

	.endasmfunc

; PendSV_Handler
; - Performs a context switch in G8RTOS
; 	- Saves remaining registers into thread stack
;	- Saves current stack pointer to tcb
;	- Calls G8RTOS_Scheduler to get new tcb
;	- Set stack pointer to new stack pointer from new tcb
;	- Pops registers from thread stack
PendSV_Handler:
	
	.asmfunc

	CPSID I					; disable interrupts to prevent interrupt during context switch

	push {r4 - r11}			; save remaining registers into thread stack

	; save current sp to tcb
	ldr r0, RunningPtr		; load currently running thread
	ldr r1, [r0]			; r1 = RunningPtr
	str SP, [r1]			; store thread sp into TCB

	push {r0, LR}			; save RunningPtr and LR so they are not overwritten by G8RTOS_Scheduler

	bl G8RTOS_Scheduler		; get new tcb

	pop {r0, LR}			; restore RunningPtr and LR

	; set sp to new tcb sp
	ldr r1, [r0]			; r1 = RunningPtr
	ldr SP, [r1]			; load new thread sp

	pop {r4 - r11}			; restore registers

	CPSIE I					; enable interrupts

	bx LR					; restore rest of context

	.endasmfunc
	
	; end of the asm file
	.align
	.end
