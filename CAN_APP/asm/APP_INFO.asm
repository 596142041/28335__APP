;####################################################
;APP_INFO.asm
;Created on:2017��8��8��-����11:27:04
;Author:admin
;Version:
;#####################################################
    .sect "APP_info"
	.int	0xFf11		;MCU_ID,0x3f3ff8,0x11=ͬ������,0x22=ͬ��ISG,03=�첽ISG,04=�첽�ͱ�,05=�첽����,06=�첽�յ�,07=ͬ���յ�,08=ͬ���ͱ�,09=ͬ������,252=�첽����
	.int	0xFf02		;MCU_ver,0x3f3ff9
    .int	0xFfff		;flashtype,0x3f3ffA,0xFff1-cana,0xFff2-canb,0xFff3-coma
	.int	0xFfff		;baud,0x3f3ffB,0xFff1=125,0xFff4=500k,0xFff2������=250k
	.int	0xFfff		;filetype,0x3f3ffC,0xFffe=����,0xFfff������=δ����
	.int	0xFfff		;0x3f3ffD
	.int	0xFfff		;0x3f3ffE
	.int	0x55BB		;flashflg,0x3f3ffF
	.int	0xFf11		;MCU_ID,0x3f3ff8,0x11=ͬ������,0x22=ͬ��ISG,03=�첽ISG,04=�첽�ͱ�,05=�첽����,06=�첽�յ�,07=ͬ���յ�,08=ͬ���ͱ�,09=ͬ������,252=�첽����
	.int	0xFf02		;MCU_ver,0x3f3ff9
    .int	0xFfff		;flashtype,0x3f3ffA,0xFff1-cana,0xFff2-canb,0xFff3-coma
	.int	0xFfff		;baud,0x3f3ffB,0xFff1=125,0xFff4=500k,0xFff2������=250k
	.int	0xFfff		;filetype,0x3f3ffC,0xFffe=����,0xFfff������=δ����
	.int	0xFfff		;0x3f3ffD
	.int	0xFfff		;0x3f3ffE
	.int	0x55BB		;flashflg,0x3f3ffF
