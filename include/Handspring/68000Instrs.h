// =========================================================================
// 68000 Instructions
// =========================================================================

// DOLATER... get rid of these opcode constants in favor of v3.0 patch
//			file format that can use real assembly code

#define ADD_W_D0_D0 						D040	// ADD.W D0, D0
#define ADDQ_W_4_A7 						584F	// ADDQ.W #4, A7
#define ADDQ_W_1_D4 						5244	// ADDQ.W #1, D4
#define ADDQ_W_08_A7						504F	// ADDQ.W #8, A7
#define ADDQ_L_02_A7						548F	// ADDQ.L #8, A7
#define ADDQ_L_08_A7						508F	// ADDQ.L #8, A7
#define ANDW_DO_A2REL						C16A	// AND.W D0, data(A2)
#define ANDI_B_A2REL						022A	// ANDI.B #data, data2(A2)
#define ANDI_B_A3REL						022B	// ANDI.B #data, data2(A3)
#define ANDI_W_A2REL						026A	// ANDI.W #data, data2(A2)
#define ANDI_W_D0							0240	// ANDI.W #data, D0
#define ANDI_L_D0							0280	// ANDI.L #data, D0
#define BEQ_B_BASE							6700	// BEQ.S *+data
#define BGE_B_BASE							6C00	
#define BHI_B_BASE							6200
#define BLE_B_BASE							6F00	// BLE.S *+data
#define BNE_B_BASE							6600
#define BPL_B_BASE							6A00
#define BRA_B_BASE							6000
#define BLE_W								6F00	
#define BNE_W								6600
#define BRA_W								6000
#define CLR_B__SP							4227	// CLR.B (-SP)
#define CMP_W_D6_D7 						BE46	// CMP.W D6, D7
#define CMP_L_I_A0							B1FC	// CMP.L #data, A0
#define CMP_L_I_A2							B5FC	// CMP.L #data, A2
#define CMP_L_I_A4							B9FC	// CMP.L #data, A4
#define CMPI_B_A3REL						0C2B	// CMPI.B #data, data2(A3)
#define CMPI_L_A3REL						0CAB
#define CMPI_W_D0							0C40	// CMPI.W #data, D0
#define CMPI_W_D3							0C43	// CMPI.W #data, D3
#define CMPI_W_D4							0C44	// CMPI.W #data, D4
#define JMPREL								4EFA	// JMP *+data
#define JSRREL								4EBA	// JSR *+data
#define LEA_SPREL_SP						4FEF	// LEA data(SP), SP
#define LINK_A6 							4E56	// LINK A6, #data
#define MOVE_B_A0REL_D1 					1228	// MOVE.B data(A0), D1
#define MOVE_B_A2REL_D0 					102A	// MOVE.B data(A2), D0
#define MOVE_B_A6REL_D1 					122E	// MOVE.B data(A6), D1
#define MOVE_B_D0_D4						1800	// MOVE.B D0, D4
#define MOVE_B_D0__SP						1F00	// MOVE.B DO, -(SP)
#define MOVE_B_I_A0REL						117C	// MOVE.B #data, data2(A0)
#define MOVE_B_I_ABSW						11FC	// MOVE.B #data, (data2).W
#define MOVE_B_I__SP						1F3C	// MOVE.B #data, -(SP)
#define MOVE_W_A0REL_D6 					3C28	// MOVE.W data(A0), D6
#define MOVE_W_A2REL_D0 					302A	// MOVE.W data(A2), D0
#define MOVE_W_A3REL_D0 					302B	// MOVE.W data(A3), D0
#define MOVE_W_A2REL_D7 					3E2A	// MOVE.W data(A2), D7
#define MOVE_W_D0_A5REL 					3B40	// MOVE.W D0, data(A5)
#define MOVE_W_D5_D0						3005	// MOVE.W D5, D0
#define MOVE_W_I_A2REL						357C	// MOVE.W #data, data2(A2)
#define MOVE_W_I_D3 						363C	// MOVE.W #data, D3
#define MOVE_W_I_D4 						383C	// MOVE.W #data, D4
#define MOVE_W_I__SP						3F3C	// MOVE.W #data, -(SP)
#define MOVE_W_PCIDX_D3 					363B	// MOVE.W dis(PC+2, reg), D3
#define MOVE_L_A0_A4						2848	// MOVE.L A0, A4
#define MOVE_L_A3_D0						200B	// MOVE.L A3, D0
#define MOVE_L_A4_D0						200C	// MOVE.L A4, D0
#define MOVE_L_A4_A2						244C	// MOVE.L A4, A2
#define MOVE_L_A0__SP						2F08	// MOVE.L A0, -(SP)
#define MOVE_L_A2__SP						2F0A	// MOVE.L A2, -(SP)
#define MOVE_L_A6REL_A0 					206E	// MOVE.L data(A6), A0
#define MOVE_L_A6REL_A1 					226E	// MOVE.L data(A6), A1
#define MOVE_L_A6REL_A2 					246E	// MOVE.L data(A6), A2
#define MOVE_L_A6REL_D6 					3C2E	// MOVE.L data(A6), D6
#define MOVE_L_ABSW_D0						2038	// MOVE.L (data).W, D0
#define MOVE_L_D3__SP						2F03	// MOVE.L D3, -(SP)
#define MOVE_L_I_A0 						207C	// MOVE.L #data, A0
#define MOVE_L_I_D0 						203C	// MOVE.L #data, D0
#define MOVE_L_I_D4 						283C	// MOVE.L #data, D4
#define MOVE_L_I__SP						2F3C	// MOVE.L #data, -(SP)
#define MOVEM_L__SP 						48E7	// MOVEM.L list, -(SP)
#define MOVEQ_00_D3 						7600	// MOVEQ #00, D3
#define MOVEQ_L_00_D6						7C00	// MOVEQ.L #00, D6
#define NOOP								4E71	// NOOP
#define ORI_B_A2REL 						002A	// ORI.B #data, data2(A2)
#define ORI_B_A3REL 						002B	// ORI.B #data, data2(A3)
#define ORI_W_A2REL 						006A	// ORI.W #data, data2(A2)
#define PEA_A6REL							486E	// PEA data(A6)
#define PEA_PCREL							487A	// PEA data(PC)
#define RTS 								4E75	// RTS
#define ST_B__SP							50E7	// ST.B (-SP)
#define SUBQ_W_7_D0 						5F40	// SUBQ.W #7, D0
#define TRAPHS								4E41	// TRAP #1
#define TRAPSYS 							4E4F	// TRAP #15
#define TST_B_D4							4A04	// TST.B D4
#define TST_B_SP_							4A1F	// TST.B (SP)+
#define TST_W_D0							4A40	// TST.W D0
#define TST_W_D3							4A43	// TST.W D3
#define TST_L_D1							4A81	// TST.L D1
#define TST_L_D4							4A84	// TST.L D4
#define TST_L_A6REL 						4AAE	// TST.L data(A6)
#define UNLK_A6 							4E5E	// UNLK A6

#define _DbgBreak							4E48

#define MOVEM_MASK_D34567_A234				1F38
#define MOVEM_MASK_D3456_A23				1E30

#define EXT_WORD_D0_W_BASE					0000

// =========================================================================
// TRAP #1 (TRAPHS) constants
// =========================================================================

// ...obviously, this is a copy of a similar table elsewhere.  However,
// that table is (likely) an enum, which doesn't work with the preprocessor (doh!)
#define _HalPadHtalCncGetProfileInfo		0xA00D
#define _HalFindOverlayDatabase 			0xA00E
#define _HalPrvLaunchCompareVers			0xA010
#define _HalModelAnimationBaseIDGet 		0xA015

// =========================================================================
// TRAP HsSelectorConstants
// =========================================================================

#define hsSelectorPrvCallSafely 			0x0002
#define hsSelectorPrvCallSafelyNewStack 	0x0003
#define hsSelectorPrvSetButtonDefault		0x0025
#define hsSelectorPrvFindAppFromIDs			0x0026

// =========================================================================
// TRAP #15 (TRAPSYS) constants
// =========================================================================

#define _MemChunkNew						0xA011
#define _ErrDisplayFileLineMsg				0xA084
#define _EvtAddEventToQueue					0xA11B
#define _HwrLCDWake 						0xA250
#define _SysEvGroupWait 					0xA2CB
#define _HsSelector 						0xA349


// =========================================================================
// These are the constants associated with 2 ASCII characters
// =========================================================================

#define Constant_Hs 						0x4873
#define Constant_Us 						0x5573
#define Constant_ch 						0x6368
#define Constant_pt 						0x7074
#define Constant_sh 						0x7368
#define Constant_wm 						0x776D

// =========================================================================
// Character codes
// =========================================================================
#define	vchrCalc							0x010B
#define	vchrHard1							0x0204
#define	vchrHard2							0x0205
#define	vchrHard3							0x0206
#define	vchrHard4							0x0207
#define	vchrHardCradle						0x0209
#define	vchrHardCradle2						0x020A
