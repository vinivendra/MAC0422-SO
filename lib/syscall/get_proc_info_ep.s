.sect .text
.extern	__get_proc_info_ep
.define	_get_proc_info_ep
.align 2

_get_proc_info_ep:
	jmp	__get_proc_info_ep