;####################################################
;APP_INFO.asm
;Created on:2017��8��8��-����11:27:04
;Author:admin
;Version:
;#####################################################
    .sect "APP_info"
	.int	0xFFFF		;MCU_ID,0x3F3FF8,0x11=ͬ������,0x22=ͬ��ISG,03=�첽ISG,04=�첽�ͱ�,05=�첽����,06=�첽�յ�,07=ͬ���յ�,08=ͬ���ͱ�,09=ͬ������,252=�첽����
	.int	0xFFFF		;MCU_ver,0x3F3FF9
    .int	0xFFFF		;Flashtype,0x3F3FFA,0xFFF1-cana,0xFFF2-canb,0xFFF3-coma
	.int	0xFFFF		;baud,0x3F3FFB,0xFFF1=125,0xFFF4=500k,0xFFF2������=250k
	.int	0xFFFF		;Filetype,0x3F3FFC,0xFFFe=����,0xFFFF������=δ����
	.int	0xFFFF		;0x3F3FFD
	.int	0xFFFF		;0x3F3FFE
	.int	0x55BB		;FlashFlg,0x3F3FFF
	.int	0xFF11		;MCU_ID,0x3F3FF8,0x11=ͬ������,0x22=ͬ��ISG,03=�첽ISG,04=�첽�ͱ�,05=�첽����,06=�첽�յ�,07=ͬ���յ�,08=ͬ���ͱ�,09=ͬ������,252=�첽����
	.int	0xFF02		;MCU_ver,0x3F3FF9
    .int	0xFFFF		;Flashtype,0x3F3FFA,0xFFF1-cana,0xFFF2-canb,0xFFF3-coma
	.int	0xFFFF		;baud,0x3F3FFB,0xFFF1=125,0xFFF4=500k,0xFFF2������=250k
	.int	0xFFFF		;Filetype,0x3F3FFC,0xFFFe=����,0xFFFF������=δ����
	.int	0xFFFF		;0x3F3FFD
	.int	0xFFFF		;0x3F3FFE
	.int	0x55BB		;FlashFlg,0x3F3FFF
