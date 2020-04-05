             /*-----------------------------------*/
             /*          >>>PicoShell<<<          */
             /*            Theo D'Hondt           */
             /*   VUB Programming Technology Lab  */
             /*             (c) 2002              */
             /*-----------------------------------*/
             /*    PicoStrings 2.0 header file    */
             /*-----------------------------------*/
             
#define PICO_VERSION   "     Pico 2.0 (September 2003)     "

#define AGR_ERROR_TEXT "abstract grammar violation"
#define ALS_ERROR_TEXT "argument list size incompatibility"
#define ALT_ERROR_TEXT "argument list type violation"
#define ARF_ERROR_TEXT "application requires function"
#define ATC_ERROR_TEXT "argument type conflict"
#define BUF_ERROR_TEXT "text Buffer overflow"
#define CAL_ERROR_TEXT "call mismatch"
#define CAS_ERROR_TEXT "case argument error"
#define CCA_ERROR_TEXT "case clauses absent"
#define CCH_ERROR_TEXT "cache overflow"
#define CTL_ERROR_TEXT "control violation"
#define DCT_ERROR_TEXT "duplicate case tag"
#define DDC_ERROR_TEXT "duplicate default clause"
#define DIG_ERROR_TEXT "digit required"
#define DPS_ERROR_TEXT "duplicate session identifier"
#define ETI_ERROR_TEXT "empty tabulation index"
#define EXP_ERROR_TEXT "expression violation"
#define EXT_ERROR_TEXT "excess token(s)"
#define FRC_ERROR_TEXT "fraction too large"
#define IAG_ERROR_TEXT "invalid argument"
#define IBS_ERROR_TEXT "index beyond size"
#define ICC_ERROR_TEXT "invalid case clause"
#define ICT_ERROR_TEXT "illegal case tag"
#define IDX_ERROR_TEXT "table index violation"
#define IIX_ERROR_TEXT "invalid index"
#define ILL_ERROR_TEXT "illegal character"
#define INF_ERROR_TEXT "infinite value"
#define INI_ERROR_TEXT "illegal native index"
#define INV_ERROR_TEXT "invocation required"
#define IPM_ERROR_TEXT "invalid parameter"
#define IPT_ERROR_TEXT "illegal parameter type"
#define MEM_ERROR_TEXT "storage overflow"
#define MTI_ERROR_TEXT "malformed tabulation index"
#define NAB_ERROR_TEXT "not a boolean"
#define NAC_ERROR_TEXT "not a constant"
#define NAD_ERROR_TEXT "not a dictionary"
#define NAF_ERROR_TEXT "not a function"
#define NAT_ERROR_TEXT "not a table"
#define NBR_ERROR_TEXT "number too large"
#define NEG_ERROR_TEXT "negative argument"
#define NMA_ERROR_TEXT "non-matching argument list"
#define NNI_ERROR_TEXT "non numeric index"
#define NNS_ERROR_TEXT "non numeric size"
#define NPI_ERROR_TEXT "non positive index"
#define NPS_ERROR_TEXT "non positive size"
#define OPD_ERROR_TEXT "operand expected"
#define PLE_ERROR_TEXT "parameter list not empty"
#define PLT_ERROR_TEXT "parameter list type violation"
#define QRO_ERROR_TEXT "qualification requires operation"
#define RBC_ERROR_TEXT "right brace expected"
#define RBR_ERROR_TEXT "right bracket expected"
#define REE_ERROR_TEXT "reentrancy violation"
#define RNG_ERROR_TEXT "range error"
#define RPR_ERROR_TEXT "right parenthesis expected"
#define SET_ERROR_TEXT "invalid assignment"
#define SIZ_ERROR_TEXT "invalid size"
#define SNA_ERROR_TEXT "session not active"
#define STK_ERROR_TEXT "stack error"
#define TAG_ERROR_TEXT "invalid tag"
#define TIR_ERROR_TEXT "tabulation into raw data"
#define TMN_ERROR_TEXT "too many names"
#define TMS_ERROR_TEXT "too many sessions"
#define TRI_ERROR_TEXT "tabulation requires index"
#define TXT_ERROR_TEXT "non-terminated text"
#define UDI_ERROR_TEXT "undefined identifier"
#define USR_ERROR_TEXT "user error"
#define VAR_ERROR_TEXT "variable required"
#define ZDV_ERROR_TEXT "zero division"
#define ZTI_ERROR_TEXT "zero tabulation index"
#define NGS_ERROR_TEXT "negative size"

static char * PicoErrorMessages[] = 
  { AGR_ERROR_TEXT, 
		ALS_ERROR_TEXT, 
		ALT_ERROR_TEXT, 
		ARF_ERROR_TEXT, 
		ATC_ERROR_TEXT, 
		BUF_ERROR_TEXT, 
		CAL_ERROR_TEXT, 
		CAS_ERROR_TEXT, 
		CCA_ERROR_TEXT, 
		CCH_ERROR_TEXT, 
		CTL_ERROR_TEXT, 
		DCT_ERROR_TEXT, 
		DDC_ERROR_TEXT, 
		DIG_ERROR_TEXT, 
		DPS_ERROR_TEXT, 
		ETI_ERROR_TEXT, 
		EXP_ERROR_TEXT, 
		EXT_ERROR_TEXT, 
		FRC_ERROR_TEXT, 
		IAG_ERROR_TEXT, 
		IBS_ERROR_TEXT, 
		ICC_ERROR_TEXT, 
		ICT_ERROR_TEXT, 
		IDX_ERROR_TEXT, 
		IIX_ERROR_TEXT, 
		ILL_ERROR_TEXT, 
		INF_ERROR_TEXT, 
		INI_ERROR_TEXT, 
		INV_ERROR_TEXT, 
		IPM_ERROR_TEXT, 
		IPT_ERROR_TEXT, 
		MEM_ERROR_TEXT, 
		MTI_ERROR_TEXT, 
		NAB_ERROR_TEXT, 
		NAC_ERROR_TEXT, 
		NAD_ERROR_TEXT, 
		NAF_ERROR_TEXT, 
		NAT_ERROR_TEXT, 
		NBR_ERROR_TEXT, 
		NEG_ERROR_TEXT, 
		NMA_ERROR_TEXT, 
		NNI_ERROR_TEXT, 
		NNS_ERROR_TEXT, 
		NPI_ERROR_TEXT, 
		NPS_ERROR_TEXT, 
		OPD_ERROR_TEXT, 
		PLE_ERROR_TEXT, 
		PLT_ERROR_TEXT, 
		QRO_ERROR_TEXT, 
		RBC_ERROR_TEXT, 
		RBR_ERROR_TEXT, 
		REE_ERROR_TEXT, 
		RNG_ERROR_TEXT, 
		RPR_ERROR_TEXT, 
		SET_ERROR_TEXT, 
		SIZ_ERROR_TEXT, 
		SNA_ERROR_TEXT, 
		STK_ERROR_TEXT, 
		TAG_ERROR_TEXT, 
		TIR_ERROR_TEXT, 
		TMN_ERROR_TEXT, 
		TMS_ERROR_TEXT, 
		TRI_ERROR_TEXT, 
		TXT_ERROR_TEXT, 
		UDI_ERROR_TEXT, 
		USR_ERROR_TEXT, 
		VAR_ERROR_TEXT,
		ZDV_ERROR_TEXT, 
		ZTI_ERROR_TEXT , 
		NGS_ERROR_TEXT };
		
#define PICO_ERROR(ERR)\
  PicoErrorMessages[ERR - 1]	
		
		
