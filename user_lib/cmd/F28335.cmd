MEMORY
{
	PAGE 0: /* Program Memory */

	ZONE0 :  origin = 0x004000,  length = 0x001000 /* XINTF zone 0 */
	RAML0 :  origin = 0x008000,  length = 0x001000 /* on-chip RAM block L0 */
	RAML1 :  origin = 0x009000,  length = 0x001000 /* on-chip RAM block L1 */
	RAML2 :  origin = 0x00A000,  length = 0x001000 /* on-chip RAM block L2 */
	RAML3 :  origin = 0x00B000,  length = 0x001000 /* on-chip RAM block L3 */
	ZONE6 :  origin = 0x0100000, length = 0x100000 /* XINTF zone 6 */
	ZONE7A : origin = 0x0200000, length = 0x00FC00 /* XINTF zone 7 - program space */
	/*-----------------------------------------------------------------------------*/
	/*-----------------------------------------------------------------------------
	 FLASHH : origin = 0x300000, length = 0x008000
	 FLASHG : origin = 0x308000, length = 0x008000
	 --------------------------------------------------------------------------------*/
	BEGIN  : origin = 0x310000, length = 0x000002
	FLASHF : origin = 0x310002, length = 0x007FFE
	FLASHE : origin = 0x318000, length = 0x008000
	FLASHD : origin = 0x320000, length = 0x008000
	FLASHC : origin = 0x328000, length = 0x008000
	FLASHB : origin = 0x330000, length = 0x008000
	FLASHA : origin = 0x338000, length = 0x007F80
	/*-----------------------------------------------------------------------------*/
	CSM_RSVD : origin = 0x33FF80, length = 0x000076 /* Part of FLASHA.  Program with all 0x0000 when CSM is in use. */

	CSM_PWL : origin = 0x33FFF8, length = 0x000008 /* Part of FLASHA.  CSM password locations in FLASHA */
	OTP : origin = 0x380400, length = 0x000400 /* on-chip OTP */
	ADC_CAL : origin = 0x380080, length = 0x000009 /* ADC_cal function in Reserved memory */

	IQTABLES : origin = 0x3FE000, length = 0x000b50 /* IQ Math Tables in Boot ROM */
	IQTABLES2 : origin = 0x3FEB50, length = 0x00008c /* IQ Math Tables in Boot ROM */
	FPUTABLES : origin = 0x3FEBDC, length = 0x0006A0 /* FPU Tables in Boot ROM */
	ROM : origin = 0x3FF27C, length = 0x000D44 /* Boot ROM */
	RESET : origin = 0x3FFFC0, length = 0x000002 /* part of boot ROM  */
	VECTORS : origin = 0x3FFFC2, length = 0x00003E /* part of boot ROM  */

	PAGE 1 : /* Data Memory */

	BOOT_RSVD : origin = 0x000000, length = 0x000050 /* Part of M0, BOOT rom will use this for stack */
	RAMM0 : origin = 0x000050, length = 0x0003B0 /* on-chip RAM block M0 */
	RAMM1 : origin = 0x000400, length = 0x000400 /* on-chip RAM block M1 */
	RAML4 : origin = 0x00C000, length = 0x001000 /* on-chip RAM block L1 */
	RAML5 : origin = 0x00D000, length = 0x001000 /* on-chip RAM block L1 */
	RAML6 : origin = 0x00E000, length = 0x001000 /* on-chip RAM block L1 */
	RAML7 : origin = 0x00F000, length = 0x001000 /* on-chip RAM block L1 */
	ZONE7B : origin = 0x20FC00, length = 0x000400 /* XINTF zone 7 - data space */

}

SECTIONS
{

	/* Allocate program areas: */
	.cinit : > FLASHF PAGE = 0
	.pinit : > FLASHF, PAGE = 0
	.text : > FLASHF PAGE = 0
	codestart : > BEGIN PAGE = 0

	ramfuncs : LOAD = FLASHF,
				RUN = RAML0,
				LOAD_START(_RamfuncsLoadStart),
				LOAD_END(_RamfuncsLoadEnd),
				RUN_START(_RamfuncsRunStart),
				PAGE = 0
	Flash28_API:
	{
		-l Flash28335_API_V210.lib(.econst)
		-l Flash28335_API_V210.lib(.text)
	}
			LOAD = FLASHF,
			RUN = RAML0,
			LOAD_START(_Flash28_API_LoadStart),
			LOAD_END(_Flash28_API_LoadEnd),
			RUN_START(_Flash28_API_RunStart),
			PAGE = 0
	csmpasswds : > CSM_PWL PAGE = 0
	csm_rsvd : > CSM_RSVD PAGE = 0
	.stack : > RAMM1 PAGE = 1
	.ebss : > RAML4 PAGE = 1
	.esysmem : > RAMM1 PAGE = 1
	.econst : > FLASHF PAGE = 0
	.switch : > FLASHF PAGE = 0
	IQmath : > FLASHF PAGE = 0 /* Math Code */
	IQmathTables : > IQTABLES, PAGE = 0, TYPE = NOLOAD

	FPUmathTables : > FPUTABLES, PAGE = 0, TYPE = NOLOAD
	DMARAML4 : > RAML4, PAGE = 1
	DMARAML5 : > RAML5, PAGE = 1
	DMARAML6 : > RAML6, PAGE = 1
	DMARAML7 : > RAML7, PAGE = 1
	ZONE7DATA : > ZONE7B, PAGE = 1
	.reset : > RESET, PAGE = 0, TYPE = DSECT
	vectors : > VECTORS PAGE = 0, TYPE = DSECT
	/* Allocate ADC_cal function (pre-programmed by factory into TI reserved memory) */
	.adc_cal : load = ADC_CAL, PAGE = 0, TYPE = NOLOAD

}
