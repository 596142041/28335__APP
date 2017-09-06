;####################################################
;APP_INFO.asm
;Created on:2017年8月8日-下午11:27:04
;Author:admin
;Version:
;#####################################################
    .sect "APP_info"
	.int	0xFFFF		;MCU_ID,0x3F3FF8,0x11=同步主驱,0x22=同步ISG,03=异步ISG,04=异步油泵,05=异步汽泵,06=异步空调,07=同步空调,08=同步油泵,09=同步汽泵,252=异步主驱
	.int	0xFFFF		;MCU_ver,0x3F3FF9
    .int	0xFFFF		;Flashtype,0x3F3FFA,0xFFF1-cana,0xFFF2-canb,0xFFF3-coma
	.int	0xFFFF		;baud,0x3F3FFB,0xFFF1=125,0xFFF4=500k,0xFFF2或其它=250k
	.int	0xFFFF		;Filetype,0x3F3FFC,0xFFFe=加密,0xFFFF或其它=未加密
	.int	0xFFFF		;0x3F3FFD
	.int	0xFFFF		;0x3F3FFE
	.int	0x55BB		;FlashFlg,0x3F3FFF
	.int	0xFF11		;MCU_ID,0x3F3FF8,0x11=同步主驱,0x22=同步ISG,03=异步ISG,04=异步油泵,05=异步汽泵,06=异步空调,07=同步空调,08=同步油泵,09=同步汽泵,252=异步主驱
	.int	0xFF02		;MCU_ver,0x3F3FF9
    .int	0xFFFF		;Flashtype,0x3F3FFA,0xFFF1-cana,0xFFF2-canb,0xFFF3-coma
	.int	0xFFFF		;baud,0x3F3FFB,0xFFF1=125,0xFFF4=500k,0xFFF2或其它=250k
	.int	0xFFFF		;Filetype,0x3F3FFC,0xFFFe=加密,0xFFFF或其它=未加密
	.int	0xFFFF		;0x3F3FFD
	.int	0xFFFF		;0x3F3FFE
	.int	0x55BB		;FlashFlg,0x3F3FFF
