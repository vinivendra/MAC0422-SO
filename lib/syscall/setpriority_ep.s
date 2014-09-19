.sect .text
.extern	__setpriority_ep
.define	_setpriority_ep
.align 2

_setpriority_ep:
	jmp	__setpriority_ep