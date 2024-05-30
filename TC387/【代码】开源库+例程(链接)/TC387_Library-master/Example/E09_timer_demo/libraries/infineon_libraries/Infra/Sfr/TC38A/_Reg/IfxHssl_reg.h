/**
 * \file IfxHssl_reg.h
 * \brief
 * \copyright Copyright (c) 2019 Infineon Technologies AG. All rights reserved.
 *
 *
 * Version: TC38XA_UM_V1.1.0.R0
 * Specification: TC3xx User Manual V1.1.0
 * MAY BE CHANGED BY USER [yes/no]: No
 *
 *                                 IMPORTANT NOTICE
 *
 *
 * Use of this file is subject to the terms of use agreed between (i) you or 
 * the company in which ordinary course of business you are acting and (ii) 
 * Infineon Technologies AG or its licensees. If and as long as no such 
 * terms of use are agreed, use of this file is subject to following:


 * Boost Software License - Version 1.0 - August 17th, 2003

 * Permission is hereby granted, free of charge, to any person or 
 * organization obtaining a copy of the software and accompanying 
 * documentation covered by this license (the "Software") to use, reproduce,
 * display, distribute, execute, and transmit the Software, and to prepare
 * derivative works of the Software, and to permit third-parties to whom the 
 * Software is furnished to do so, all subject to the following:

 * The copyright notices in the Software and this entire statement, including
 * the above license grant, this restriction and the following disclaimer, must
 * be included in all copies of the Software, in whole or in part, and all
 * derivative works of the Software, unless such copies or derivative works are
 * solely in the form of machine-executable object code generated by a source
 * language processor.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
 * SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE 
 * FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * \defgroup IfxSfr_Hssl_Registers_Cfg Hssl address
 * \ingroup IfxSfr_Hssl_Registers
 * 
 * \defgroup IfxSfr_Hssl_Registers_Cfg_BaseAddress Base address
 * \ingroup IfxSfr_Hssl_Registers_Cfg
 *
 * \defgroup IfxSfr_Hssl_Registers_Cfg_Hssl0 2-HSSL0
 * \ingroup IfxSfr_Hssl_Registers_Cfg
 *
 *
 */
#ifndef IFXHSSL_REG_H
#define IFXHSSL_REG_H 1
/******************************************************************************/
#include "IfxHssl_regdef.h"
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/

/** \addtogroup IfxSfr_Hssl_Registers_Cfg_BaseAddress
 * \{  */

/** \brief HSSL object */
#define MODULE_HSSL0 /*lint --e(923, 9078)*/ ((*(Ifx_HSSL*)0xF0080000u))
/** \}  */


/******************************************************************************/
/******************************************************************************/
/** \addtogroup IfxSfr_Hssl_Registers_Cfg_Hssl0
 * \{  */
/** \brief 0, Clock Control Register */
#define HSSL0_CLC /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_CLC*)0xF0080000u)

/** \brief 8, Module Identification Register */
#define HSSL0_ID /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_ID*)0xF0080008u)

/** \brief C, CRC Control Register */
#define HSSL0_CRC /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_CRC*)0xF008000Cu)

/** \brief 10, Configuration Register */
#define HSSL0_CFG /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_CFG*)0xF0080010u)

/** \brief 14, Request Flags Register */
#define HSSL0_QFLAGS /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_QFLAGS*)0xF0080014u)

/** \brief 18, Miscellaneous Flags Register */
#define HSSL0_MFLAGS /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_MFLAGS*)0xF0080018u)

/** \brief 1C, Miscellaneous Flags Set Register */
#define HSSL0_MFLAGSSET /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_MFLAGSSET*)0xF008001Cu)

/** \brief 20, Miscellaneous Flags Clear Register */
#define HSSL0_MFLAGSCL /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_MFLAGSCL*)0xF0080020u)

/** \brief 24, Flags Enable Register */
#define HSSL0_MFLAGSEN /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_MFLAGSEN*)0xF0080024u)

/** \brief 28, Stream FIFOs Status Flags Register */
#define HSSL0_SFSFLAGS /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_SFSFLAGS*)0xF0080028u)

/** \brief 30, Initiator Write Data Register 0 */
#define HSSL0_I0_IWD /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_I_IWD*)0xF0080030u)
/** Alias (User Manual Name) for HSSL0_I0_IWD.
* To use register names with standard convension, please use HSSL0_I0_IWD.
*/
#define HSSL0_IWD0 (HSSL0_I0_IWD)

/** \brief 34, Initiator Control Data Register 0 */
#define HSSL0_I0_ICON /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_I_ICON*)0xF0080034u)
/** Alias (User Manual Name) for HSSL0_I0_ICON.
* To use register names with standard convension, please use HSSL0_I0_ICON.
*/
#define HSSL0_ICON0 (HSSL0_I0_ICON)

/** \brief 38, Initiator Read Write Address Register 0 */
#define HSSL0_I0_IRWA /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_I_IRWA*)0xF0080038u)
/** Alias (User Manual Name) for HSSL0_I0_IRWA.
* To use register names with standard convension, please use HSSL0_I0_IRWA.
*/
#define HSSL0_IRWA0 (HSSL0_I0_IRWA)

/** \brief 3C, Initiator Read Data Register 0 */
#define HSSL0_I0_IRD /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_I_IRD*)0xF008003Cu)
/** Alias (User Manual Name) for HSSL0_I0_IRD.
* To use register names with standard convension, please use HSSL0_I0_IRD.
*/
#define HSSL0_IRD0 (HSSL0_I0_IRD)

/** \brief 40, Initiator Write Data Register 1 */
#define HSSL0_I1_IWD /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_I_IWD*)0xF0080040u)
/** Alias (User Manual Name) for HSSL0_I1_IWD.
* To use register names with standard convension, please use HSSL0_I1_IWD.
*/
#define HSSL0_IWD1 (HSSL0_I1_IWD)

/** \brief 44, Initiator Control Data Register 1 */
#define HSSL0_I1_ICON /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_I_ICON*)0xF0080044u)
/** Alias (User Manual Name) for HSSL0_I1_ICON.
* To use register names with standard convension, please use HSSL0_I1_ICON.
*/
#define HSSL0_ICON1 (HSSL0_I1_ICON)

/** \brief 48, Initiator Read Write Address Register 1 */
#define HSSL0_I1_IRWA /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_I_IRWA*)0xF0080048u)
/** Alias (User Manual Name) for HSSL0_I1_IRWA.
* To use register names with standard convension, please use HSSL0_I1_IRWA.
*/
#define HSSL0_IRWA1 (HSSL0_I1_IRWA)

/** \brief 4C, Initiator Read Data Register 1 */
#define HSSL0_I1_IRD /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_I_IRD*)0xF008004Cu)
/** Alias (User Manual Name) for HSSL0_I1_IRD.
* To use register names with standard convension, please use HSSL0_I1_IRD.
*/
#define HSSL0_IRD1 (HSSL0_I1_IRD)

/** \brief 50, Initiator Write Data Register 2 */
#define HSSL0_I2_IWD /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_I_IWD*)0xF0080050u)
/** Alias (User Manual Name) for HSSL0_I2_IWD.
* To use register names with standard convension, please use HSSL0_I2_IWD.
*/
#define HSSL0_IWD2 (HSSL0_I2_IWD)

/** \brief 54, Initiator Control Data Register 2 */
#define HSSL0_I2_ICON /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_I_ICON*)0xF0080054u)
/** Alias (User Manual Name) for HSSL0_I2_ICON.
* To use register names with standard convension, please use HSSL0_I2_ICON.
*/
#define HSSL0_ICON2 (HSSL0_I2_ICON)

/** \brief 58, Initiator Read Write Address Register 2 */
#define HSSL0_I2_IRWA /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_I_IRWA*)0xF0080058u)
/** Alias (User Manual Name) for HSSL0_I2_IRWA.
* To use register names with standard convension, please use HSSL0_I2_IRWA.
*/
#define HSSL0_IRWA2 (HSSL0_I2_IRWA)

/** \brief 5C, Initiator Read Data Register 2 */
#define HSSL0_I2_IRD /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_I_IRD*)0xF008005Cu)
/** Alias (User Manual Name) for HSSL0_I2_IRD.
* To use register names with standard convension, please use HSSL0_I2_IRD.
*/
#define HSSL0_IRD2 (HSSL0_I2_IRD)

/** \brief 60, Initiator Write Data Register 3 */
#define HSSL0_I3_IWD /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_I_IWD*)0xF0080060u)
/** Alias (User Manual Name) for HSSL0_I3_IWD.
* To use register names with standard convension, please use HSSL0_I3_IWD.
*/
#define HSSL0_IWD3 (HSSL0_I3_IWD)

/** \brief 64, Initiator Control Data Register 3 */
#define HSSL0_I3_ICON /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_I_ICON*)0xF0080064u)
/** Alias (User Manual Name) for HSSL0_I3_ICON.
* To use register names with standard convension, please use HSSL0_I3_ICON.
*/
#define HSSL0_ICON3 (HSSL0_I3_ICON)

/** \brief 68, Initiator Read Write Address Register 3 */
#define HSSL0_I3_IRWA /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_I_IRWA*)0xF0080068u)
/** Alias (User Manual Name) for HSSL0_I3_IRWA.
* To use register names with standard convension, please use HSSL0_I3_IRWA.
*/
#define HSSL0_IRWA3 (HSSL0_I3_IRWA)

/** \brief 6C, Initiator Read Data Register 3 */
#define HSSL0_I3_IRD /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_I_IRD*)0xF008006Cu)
/** Alias (User Manual Name) for HSSL0_I3_IRD.
* To use register names with standard convension, please use HSSL0_I3_IRD.
*/
#define HSSL0_IRD3 (HSSL0_I3_IRD)

/** \brief 70, Target Current Data Register 0 */
#define HSSL0_T0_TCD /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_T_TCD*)0xF0080070u)
/** Alias (User Manual Name) for HSSL0_T0_TCD.
* To use register names with standard convension, please use HSSL0_T0_TCD.
*/
#define HSSL0_TCD0 (HSSL0_T0_TCD)

/** \brief 74, Target Current Address Register 0 */
#define HSSL0_T0_TCA /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_T_TCA*)0xF0080074u)
/** Alias (User Manual Name) for HSSL0_T0_TCA.
* To use register names with standard convension, please use HSSL0_T0_TCA.
*/
#define HSSL0_TCA0 (HSSL0_T0_TCA)

/** \brief 78, Target Current Data Register 1 */
#define HSSL0_T1_TCD /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_T_TCD*)0xF0080078u)
/** Alias (User Manual Name) for HSSL0_T1_TCD.
* To use register names with standard convension, please use HSSL0_T1_TCD.
*/
#define HSSL0_TCD1 (HSSL0_T1_TCD)

/** \brief 7C, Target Current Address Register 1 */
#define HSSL0_T1_TCA /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_T_TCA*)0xF008007Cu)
/** Alias (User Manual Name) for HSSL0_T1_TCA.
* To use register names with standard convension, please use HSSL0_T1_TCA.
*/
#define HSSL0_TCA1 (HSSL0_T1_TCA)

/** \brief 80, Target Current Data Register 2 */
#define HSSL0_T2_TCD /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_T_TCD*)0xF0080080u)
/** Alias (User Manual Name) for HSSL0_T2_TCD.
* To use register names with standard convension, please use HSSL0_T2_TCD.
*/
#define HSSL0_TCD2 (HSSL0_T2_TCD)

/** \brief 84, Target Current Address Register 2 */
#define HSSL0_T2_TCA /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_T_TCA*)0xF0080084u)
/** Alias (User Manual Name) for HSSL0_T2_TCA.
* To use register names with standard convension, please use HSSL0_T2_TCA.
*/
#define HSSL0_TCA2 (HSSL0_T2_TCA)

/** \brief 88, Target Current Data Register 3 */
#define HSSL0_T3_TCD /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_T_TCD*)0xF0080088u)
/** Alias (User Manual Name) for HSSL0_T3_TCD.
* To use register names with standard convension, please use HSSL0_T3_TCD.
*/
#define HSSL0_TCD3 (HSSL0_T3_TCD)

/** \brief 8C, Target Current Address Register 3 */
#define HSSL0_T3_TCA /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_T_TCA*)0xF008008Cu)
/** Alias (User Manual Name) for HSSL0_T3_TCA.
* To use register names with standard convension, please use HSSL0_T3_TCA.
*/
#define HSSL0_TCA3 (HSSL0_T3_TCA)

/** \brief 90, Target Status Register */
#define HSSL0_TSTAT /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_TSTAT*)0xF0080090u)

/** \brief 94, Target ID Address Register */
#define HSSL0_TIDADD /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_TIDADD*)0xF0080094u)

/** \brief 98, Security Control Register */
#define HSSL0_SEC /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_SEC*)0xF0080098u)

/** \brief 9C, Multi Slave Control Register */
#define HSSL0_MSCR /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_MSCR*)0xF008009Cu)

/** \brief A0, Initiator Stream Start Address Register */
#define HSSL0_IS_SA0 /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_IS_SA*)0xF00800A0u)
/** Alias (User Manual Name) for HSSL0_IS_SA0.
* To use register names with standard convension, please use HSSL0_IS_SA0.
*/
#define HSSL0_ISSA0 (HSSL0_IS_SA0)

/** \brief A4, Initiator Stream Start Address Register */
#define HSSL0_IS_SA1 /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_IS_SA*)0xF00800A4u)
/** Alias (User Manual Name) for HSSL0_IS_SA1.
* To use register names with standard convension, please use HSSL0_IS_SA1.
*/
#define HSSL0_ISSA1 (HSSL0_IS_SA1)

/** \brief A8, Initiator Stream Current Address Register */
#define HSSL0_IS_CA /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_IS_CA*)0xF00800A8u)
/** Alias (User Manual Name) for HSSL0_IS_CA.
* To use register names with standard convension, please use HSSL0_IS_CA.
*/
#define HSSL0_ISCA (HSSL0_IS_CA)

/** \brief AC, Initiator Stream Frame Count Register */
#define HSSL0_IS_FC /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_IS_FC*)0xF00800ACu)
/** Alias (User Manual Name) for HSSL0_IS_FC.
* To use register names with standard convension, please use HSSL0_IS_FC.
*/
#define HSSL0_ISFC (HSSL0_IS_FC)

/** \brief B0, Target Stream Start Address Register 0 */
#define HSSL0_TS_SA0 /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_TS_SA*)0xF00800B0u)
/** Alias (User Manual Name) for HSSL0_TS_SA0.
* To use register names with standard convension, please use HSSL0_TS_SA0.
*/
#define HSSL0_TSSA0 (HSSL0_TS_SA0)

/** \brief B4, Target Stream Start Address Register 1 */
#define HSSL0_TS_SA1 /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_TS_SA*)0xF00800B4u)
/** Alias (User Manual Name) for HSSL0_TS_SA1.
* To use register names with standard convension, please use HSSL0_TS_SA1.
*/
#define HSSL0_TSSA1 (HSSL0_TS_SA1)

/** \brief B8, Target Stream Current Address Register */
#define HSSL0_TS_CA /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_TS_CA*)0xF00800B8u)
/** Alias (User Manual Name) for HSSL0_TS_CA.
* To use register names with standard convension, please use HSSL0_TS_CA.
*/
#define HSSL0_TSCA (HSSL0_TS_CA)

/** \brief BC, Target Stream Frame Count Register */
#define HSSL0_TS_FC /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_TS_FC*)0xF00800BCu)
/** Alias (User Manual Name) for HSSL0_TS_FC.
* To use register names with standard convension, please use HSSL0_TS_FC.
*/
#define HSSL0_TSFC (HSSL0_TS_FC)

/** \brief C0, Access Window Start Register 0 */
#define HSSL0_AW0_AWSTART /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_AW_AWSTART*)0xF00800C0u)
/** Alias (User Manual Name) for HSSL0_AW0_AWSTART.
* To use register names with standard convension, please use HSSL0_AW0_AWSTART.
*/
#define HSSL0_AWSTART0 (HSSL0_AW0_AWSTART)

/** \brief C4, Access Window End Register 0 */
#define HSSL0_AW0_AWEND /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_AW_AWEND*)0xF00800C4u)
/** Alias (User Manual Name) for HSSL0_AW0_AWEND.
* To use register names with standard convension, please use HSSL0_AW0_AWEND.
*/
#define HSSL0_AWEND0 (HSSL0_AW0_AWEND)

/** \brief C8, Access Window Start Register 1 */
#define HSSL0_AW1_AWSTART /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_AW_AWSTART*)0xF00800C8u)
/** Alias (User Manual Name) for HSSL0_AW1_AWSTART.
* To use register names with standard convension, please use HSSL0_AW1_AWSTART.
*/
#define HSSL0_AWSTART1 (HSSL0_AW1_AWSTART)

/** \brief CC, Access Window End Register 1 */
#define HSSL0_AW1_AWEND /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_AW_AWEND*)0xF00800CCu)
/** Alias (User Manual Name) for HSSL0_AW1_AWEND.
* To use register names with standard convension, please use HSSL0_AW1_AWEND.
*/
#define HSSL0_AWEND1 (HSSL0_AW1_AWEND)

/** \brief D0, Access Window Start Register 2 */
#define HSSL0_AW2_AWSTART /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_AW_AWSTART*)0xF00800D0u)
/** Alias (User Manual Name) for HSSL0_AW2_AWSTART.
* To use register names with standard convension, please use HSSL0_AW2_AWSTART.
*/
#define HSSL0_AWSTART2 (HSSL0_AW2_AWSTART)

/** \brief D4, Access Window End Register 2 */
#define HSSL0_AW2_AWEND /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_AW_AWEND*)0xF00800D4u)
/** Alias (User Manual Name) for HSSL0_AW2_AWEND.
* To use register names with standard convension, please use HSSL0_AW2_AWEND.
*/
#define HSSL0_AWEND2 (HSSL0_AW2_AWEND)

/** \brief D8, Access Window Start Register 3 */
#define HSSL0_AW3_AWSTART /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_AW_AWSTART*)0xF00800D8u)
/** Alias (User Manual Name) for HSSL0_AW3_AWSTART.
* To use register names with standard convension, please use HSSL0_AW3_AWSTART.
*/
#define HSSL0_AWSTART3 (HSSL0_AW3_AWSTART)

/** \brief DC, Access Window End Register 3 */
#define HSSL0_AW3_AWEND /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_AW_AWEND*)0xF00800DCu)
/** Alias (User Manual Name) for HSSL0_AW3_AWEND.
* To use register names with standard convension, please use HSSL0_AW3_AWEND.
*/
#define HSSL0_AWEND3 (HSSL0_AW3_AWEND)

/** \brief E0, Access Rules Register */
#define HSSL0_AR /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_AR*)0xF00800E0u)

/** \brief E8, OCDS Control and Status */
#define HSSL0_OCS /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_OCS*)0xF00800E8u)

/** \brief EC, Kernel Reset Status Clear Register */
#define HSSL0_KRSTCLR /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_KRSTCLR*)0xF00800ECu)

/** \brief F0, Kernel Reset Register 1 */
#define HSSL0_KRST1 /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_KRST1*)0xF00800F0u)

/** \brief F4, Kernel Reset Register 0 */
#define HSSL0_KRST0 /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_KRST0*)0xF00800F4u)

/** \brief F8, Access Enable Register 1 */
#define HSSL0_ACCEN1 /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_ACCEN1*)0xF00800F8u)

/** \brief FC, Access Enable Register 0 */
#define HSSL0_ACCEN0 /*lint --e(923, 9078)*/ (*(volatile Ifx_HSSL_ACCEN0*)0xF00800FCu)


/** \}  */

/******************************************************************************/

/******************************************************************************/

#endif /* IFXHSSL_REG_H */
