;####################################################
;APP_INFO.asm
;Created on:2017年8月8日-下午11:27:04
;Author:admin
;Version:
;#####################################################
    .sect "APP_info"
	.int	0xFf11		;MCU_ID,0x3f3ff8,0x11=同步主驱,0x22=同步ISG,03=异步ISG,04=异步油泵,05=异步汽泵,06=异步空调,07=同步空调,08=同步油泵,09=同步汽泵,252=异步主驱
	.int	0xFf02		;MCU_ver,0x3f3ff9
    .int	0xFfff		;flashtype,0x3f3ffA,0xFff1-cana,0xFff2-canb,0xFff3-coma
	.int	0xFfff		;baud,0x3f3ffB,0xFff1=125,0xFff4=500k,0xFff2或其它=250k
	.int	0xFfff		;filetype,0x3f3ffC,0xFffe=加密,0xFfff或其它=未加密
	.int	0xFfff		;0x3f3ffD
	.int	0xFfff		;0x3f3ffE
	.int	0x55BB		;flashflg,0x3f3ffF
	.int	0xFf11		;MCU_ID,0x3f3ff8,0x11=同步主驱,0x22=同步ISG,03=异步ISG,04=异步油泵,05=异步汽泵,06=异步空调,07=同步空调,08=同步油泵,09=同步汽泵,252=异步主驱
	.int	0xFf02		;MCU_ver,0x3f3ff9
    .int	0xFfff		;flashtype,0x3f3ffA,0xFff1-cana,0xFff2-canb,0xFff3-coma
	.int	0xFfff		;baud,0x3f3ffB,0xFff1=125,0xFff4=500k,0xFff2或其它=250k
	.int	0xFfff		;filetype,0x3f3ffC,0xFffe=加密,0xFfff或其它=未加密
	.int	0xFfff		;0x3f3ffD
	.int	0xFfff		;0x3f3ffE
	.int	0x55BB		;flashflg,0x3f3ffF
