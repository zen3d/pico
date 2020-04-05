             /*-----------------------------------*/
             /*           >>>Pico 2.0<<<          */
             /*            Theo D'Hondt           */
             /*   VUB Programming Technology Lab  */
             /*             (c) 2002              */
             /*-----------------------------------*/
             /*          Native functions         */
             /*-----------------------------------*/

#include "Pico.h"
#include "PicoMAI.h"
#include "PicoCNT.h"
#include "PicoEVA.h"
#include "PicoMEM.h"
#include "PicoNAT.h"
#include "PicoPRI.h"
#include "PicoREA.h"
#include "PicoSCA.h"

/*---------------------- private constants ------------------------*/

#define NATIVE_CST_SIZE 5
#define NATIVE_VAR_SIZE 1
#define NATIVE_FUN_SIZE 75

#define SELECT_NBR NATIVE_FUN_SIZE

#define CAL _eva_CAL_
#define EXP _eva_EXP_
#define REA _rea_EXP_
#define PRI _pri_EXP_

   /*--STRINGS--------*/
#define css_STR "<case>"
#define cnt_STR "continuation"
#define nul_STR ""
#define nwl_STR "\n"
#define qte_STR "\""
#define sel_STR "select"
   /*--VARIABLES------*/
#define emp_STR "empty"
#define eol_STR "eoln"
#define nll_STR "null"
#define quo_STR "quote"
#define roo_STR "root"
#define voi_STR "void"
   /*--ARITHMETIC-----*/
#define add_STR "+" 
#define sub_STR "-" 
#define mul_STR "*"  
#define div_STR "/" 
#define idv_STR "/""/" 
#define rem_STR "\\""\\" 
#define pow_STR "^" 
   /*--RELATIONAL-----*/
#define lss_STR "<" 
#define leq_STR "<=" 
#define eql_STR "=" 
#define neq_STR "!=" 
#define grt_STR ">" 
#define geq_STR ">=" 
#define eqv_STR "~" 
#define nqv_STR "!~" 
   /*--CONVERSION-----*/
#define trn_STR "trunc" 
#define abs_STR "abs"  
#define chr_STR "char"  
#define ord_STR "ord"  
#define num_STR "number"  
#define tex_STR "text"  
   /*--UTILITY--------*/
#define rnd_STR "random"  
#define clk_STR "clock"
   /*--TRANSCENDENTAL-*/
#define sqt_STR "sqrt" 
#define sin_STR "sin"  
#define cos_STR "cos"  
#define tan_STR "tan"  
#define asn_STR "arcsin"  
#define acs_STR "arccos"  
#define atn_STR "arctan" 
#define epw_STR "exp" 
#define log_STR "log" 
   /*--STRING---------*/
#define len_STR "length" 
#define epl_STR "explode" 
#define ipl_STR "implode" 
   /*--IDENTITY-------*/
#define ivd_STR "is_void" 
#define inb_STR "is_number" 
#define ifr_STR "is_fraction" 
#define itx_STR "is_text" 
#define itb_STR "is_table" 
#define ifu_STR "is_function" 
#define idc_STR "is_dictionary" 
#define ict_STR "is_continuation" 
   /*--TABLE----------*/
#define siz_STR "size" 
#define tab_STR "table" 
#define pai_STR "#" 
   /*--INTERACTION----*/
#define dsp_STR "display" 
#define acp_STR "accept" 
   /*--BOOLEAN--------*/
#define trr_STR "true" 
#define fls_STR "false" 
#define and_STR "&" 
#define orr_STR "|" 
#define not_STR "!" 
   /*--CONTROL--------*/
#define beg_STR "begin" 
#define iff_STR "if" 
#define whi_STR "while" 
#define unt_STR "until" 
#define for_STR "for" 
#define cas_STR "case" 
   /*--METALEVEL------*/
#define lod_STR "load" 
#define dmp_STR "dump" 
#define rea_STR "read" 
#define eva_STR "eval" 
#define pri_STR "print" 
#define tag_STR "tag" 
#define rnk_STR "rank" 
#define mak_STR "make" 
#define get_STR "get" 
#define set_STR "set" 
   /*--DICTIONARIES---*/
#define cpt_STR "capture" 
#define cmt_STR "commit" 
   /*--CONTINUATIONS--*/
#define cll_STR "call" 
#define con_STR "continue" 
#define esc_STR "escape"
   /*--ERROR----------*/
#define err_STR "error"

/*---------------------- private macro's  -------------------------*/

#define DISPLAY_NAME(INV, NAM)\
   switch (_ag_get_TAG_(INV))\
    { case _REF_TAG_:\
        NAM = _ag_get_REF_NAM_(INV);\
        break;\
      case _APL_TAG_:\
	      { _EXP_TYPE_ exp = _ag_get_APL_EXP_(INV);\
	        if(!_ag_is_REF_(exp))\
	          _error_str_(_AGR_ERROR_, dsp_STR);\
	        NAM = _ag_get_REF_NAM_(exp); }\
	      break;\
      case _TBL_TAG_:\
        { _EXP_TYPE_ exp = _ag_get_TBL_EXP_(INV);\
	        if(!_ag_is_REF_(exp))\
	          _error_str_(_AGR_ERROR_, dsp_STR);\
	        NAM = _ag_get_REF_NAM_(exp); }\
        break;\
      default:\
        _error_str_(_AGR_ERROR_, dsp_STR); }

#define UNARY(CNT, NAM)\
  _EXP_TYPE_ arg, exp;\
  _UNS_TYPE_ siz;\
  _stk_claim_();\
  _stk_peek_EXP_(arg);\
  siz = _ag_get_TAB_SIZ_(arg);\
  if (siz != 1)\
    _error_str_(_NMA_ERROR_, NAM);\
  exp = _ag_get_TAB_EXP_(arg, 1);\
  _stk_poke_EXP_(exp);\
  _stk_poke_CNT_(CNT);\
  _stk_push_CNT_(EXP)

#define BINARY(CNT, NAM)\
  _EXP_TYPE_ arg, exp;\
  _UNS_TYPE_ siz;\
  _stk_claim_();\
  _stk_peek_EXP_(arg);\
  siz = _ag_get_TAB_SIZ_(arg);\
  if (siz != 2)\
    _error_str_(_NMA_ERROR_, NAM);\
  exp = _ag_get_TAB_EXP_(arg, 2);\
  _stk_poke_EXP_(exp);\
  exp = _ag_get_TAB_EXP_(arg, 1);\
  _stk_push_EXP_(exp);\
  _stk_poke_CNT_(CNT);\
  _stk_push_CNT_(SWP);\
  _stk_push_CNT_(EXP)

#define N_ARY(CNT, SIZ, NAM)\
  _EXP_TYPE_ arg, exp;\
  _UNS_TYPE_ siz;\
  _stk_claim_();\
  _stk_peek_EXP_(arg);\
  siz = _ag_get_TAB_SIZ_(arg);\
  if (siz < SIZ)\
    _error_str_(_NMA_ERROR_, NAM);\
  exp = _ag_get_TAB_EXP_(arg, 1);\
  _stk_push_EXP_(exp);\
  _stk_poke_CNT_(CNT);\
  _stk_push_CNT_(EXP)

#define RANK(RNK)\
  if(rnk != RNK)\
    _error_str_(_ALS_ERROR_, mak_STR)

#define TEXT(IDX)\
  ((exp = _ag_get_TAB_EXP_(arg, IDX), _ag_is_TXT_(exp))?\
     exp:\
     _error_str_(_ATC_ERROR_, mak_STR))

#define EXPRESSION(IDX)\
  (exp = _ag_get_TAB_EXP_(arg, IDX))

#define DICTIONARY(IDX)\
  ((exp = _ag_get_TAB_EXP_(arg, IDX), _ag_is_DCT_(exp))?\
     exp:\
     _error_str_(_ATC_ERROR_, mak_STR))

#define TABLE(IDX)\
  ((exp = _ag_get_TAB_EXP_(arg, IDX), _ag_is_TAB_(exp))?\
     exp:\
     _error_str_(_ATC_ERROR_, mak_STR))

#define INVOCATION(IDX)\
  ((exp = _ag_get_TAB_EXP_(arg, IDX), _ag_is_INV_(exp))?\
     exp:\
     _error_str_(_ATC_ERROR_, mak_STR))

#define NUMBER(IDX)\
  ((exp = _ag_get_TAB_EXP_(arg, IDX), _ag_is_NBR_(exp))?\
     exp:\
     _error_str_(_ATC_ERROR_, mak_STR))

#define FRACTION(IDX)\
  ((exp = _ag_get_TAB_EXP_(arg, IDX), _ag_is_FRC_(exp))?\
     exp:\
     _error_str_(_ATC_ERROR_, mak_STR))
    
/*-------------------- private prototypes -------------------------*/

   /*--CONSTANS-------*/
static _EXP_TYPE_ EMP(_NIL_TYPE_);
static _EXP_TYPE_ EOL(_NIL_TYPE_);
static _EXP_TYPE_ NLL(_NIL_TYPE_);
static _EXP_TYPE_ QUO(_NIL_TYPE_);
static _EXP_TYPE_ VOI(_NIL_TYPE_);
   /*--VARIABLES_-----*/
static _EXP_TYPE_ ROO(_NIL_TYPE_);
   /*--ARITHMETIC-----*/
static _NIL_TYPE_ ADD(_NIL_TYPE_);
static _NIL_TYPE_ SUB(_NIL_TYPE_);
static _NIL_TYPE_ MUL(_NIL_TYPE_);
static _NIL_TYPE_ DIV(_NIL_TYPE_);
static _NIL_TYPE_ IDV(_NIL_TYPE_);
static _NIL_TYPE_ REM(_NIL_TYPE_);
static _NIL_TYPE_ POW(_NIL_TYPE_);
   /*--RELATIONAL-----*/
static _NIL_TYPE_ LSS(_NIL_TYPE_);
static _NIL_TYPE_ LEQ(_NIL_TYPE_);
static _NIL_TYPE_ EQL(_NIL_TYPE_);
static _NIL_TYPE_ NEQ(_NIL_TYPE_);
static _NIL_TYPE_ GRT(_NIL_TYPE_);
static _NIL_TYPE_ GEQ(_NIL_TYPE_);
static _NIL_TYPE_ EQV(_NIL_TYPE_);
static _NIL_TYPE_ NQV(_NIL_TYPE_);
   /*--CONVERSION-----*/
static _NIL_TYPE_ TRN(_NIL_TYPE_);
static _NIL_TYPE_ ABS(_NIL_TYPE_);
static _NIL_TYPE_ CHR(_NIL_TYPE_);
static _NIL_TYPE_ ORD(_NIL_TYPE_);
static _NIL_TYPE_ NUM(_NIL_TYPE_);
static _NIL_TYPE_ TEX(_NIL_TYPE_);
   /*--UTILITY--------*/
static _NIL_TYPE_ RND(_NIL_TYPE_);
static _NIL_TYPE_ CLK(_NIL_TYPE_);
   /*--TRANSCENDENTAL-*/
static _NIL_TYPE_ SQT(_NIL_TYPE_);
static _NIL_TYPE_ SIN(_NIL_TYPE_);
static _NIL_TYPE_ COS(_NIL_TYPE_);
static _NIL_TYPE_ TAN(_NIL_TYPE_);
static _NIL_TYPE_ ASN(_NIL_TYPE_);
static _NIL_TYPE_ ACS(_NIL_TYPE_);
static _NIL_TYPE_ ATN(_NIL_TYPE_);
static _NIL_TYPE_ EPW(_NIL_TYPE_);
static _NIL_TYPE_ LOG(_NIL_TYPE_);
   /*--STRING---------*/
static _NIL_TYPE_ LEN(_NIL_TYPE_);
static _NIL_TYPE_ EPL(_NIL_TYPE_);
static _NIL_TYPE_ IPL(_NIL_TYPE_);
   /*--IDENTITY-------*/
static _NIL_TYPE_ IVD(_NIL_TYPE_);
static _NIL_TYPE_ INB(_NIL_TYPE_);
static _NIL_TYPE_ IFR(_NIL_TYPE_);
static _NIL_TYPE_ ITX(_NIL_TYPE_);
static _NIL_TYPE_ ITB(_NIL_TYPE_);
static _NIL_TYPE_ IFU(_NIL_TYPE_);
static _NIL_TYPE_ IDC(_NIL_TYPE_);
static _NIL_TYPE_ ICT(_NIL_TYPE_);
   /*--TABLE----------*/
static _NIL_TYPE_ SIZ(_NIL_TYPE_);
static _NIL_TYPE_ TAB(_NIL_TYPE_);
static _NIL_TYPE_ PAI(_NIL_TYPE_);
   /*--INTERACTION----*/
static _NIL_TYPE_ DSP(_NIL_TYPE_);
static _NIL_TYPE_ ACP(_NIL_TYPE_);
   /*--BOOLEAN--------*/
static _NIL_TYPE_ TRR(_NIL_TYPE_);
static _NIL_TYPE_ FLS(_NIL_TYPE_);
static _NIL_TYPE_ AND(_NIL_TYPE_);
static _NIL_TYPE_ ORR(_NIL_TYPE_);
static _NIL_TYPE_ NOT(_NIL_TYPE_);
   /*--CONTROL--------*/
static _NIL_TYPE_ BEG(_NIL_TYPE_);
static _NIL_TYPE_ IFF(_NIL_TYPE_);
static _NIL_TYPE_ WHI(_NIL_TYPE_);
static _NIL_TYPE_ UNT(_NIL_TYPE_);
static _NIL_TYPE_ FOR(_NIL_TYPE_);
static _NIL_TYPE_ CAS(_NIL_TYPE_);
   /*--METALEVEL------*/
static _NIL_TYPE_ LOD(_NIL_TYPE_);
static _NIL_TYPE_ DMP(_NIL_TYPE_);
static _NIL_TYPE_ RED(_NIL_TYPE_);
static _NIL_TYPE_ EVA(_NIL_TYPE_);
static _NIL_TYPE_ PRT(_NIL_TYPE_);
static _NIL_TYPE_ TAG(_NIL_TYPE_);
static _NIL_TYPE_ RNK(_NIL_TYPE_);
static _NIL_TYPE_ MAK(_NIL_TYPE_);
static _NIL_TYPE_ GET(_NIL_TYPE_);
static _NIL_TYPE_ SET(_NIL_TYPE_);
   /*--DICTIONARIES---*/
static _NIL_TYPE_ CPT(_NIL_TYPE_);
static _NIL_TYPE_ CMT(_NIL_TYPE_);
   /*--CONTINUATIONS--*/
static _NIL_TYPE_ CLL(_NIL_TYPE_);
static _NIL_TYPE_ CON(_NIL_TYPE_);
static _NIL_TYPE_ ESC(_NIL_TYPE_);
   /*--ERROR----------*/
static _NIL_TYPE_ ERR(_NIL_TYPE_);
   /*--CASE-----------*/
static _NIL_TYPE_ SEL(_NIL_TYPE_);
   /*--AUXILIARY------*/
static _NIL_TYPE_ AB1(_NIL_TYPE_);
static _NIL_TYPE_ AC1(_NIL_TYPE_);
static _NIL_TYPE_ AD1(_NIL_TYPE_);
static _NIL_TYPE_ AD2(_NIL_TYPE_);
static _NIL_TYPE_ AN1(_NIL_TYPE_);
static _NIL_TYPE_ AN2(_NIL_TYPE_);
static _NIL_TYPE_ AS1(_NIL_TYPE_);
static _NIL_TYPE_ AT1(_NIL_TYPE_);
static _NIL_TYPE_ BE1(_NIL_TYPE_);
static _NIL_TYPE_ CA1(_NIL_TYPE_);
static _NIL_TYPE_ CH1(_NIL_TYPE_);
static _NIL_TYPE_ CL1(_NIL_TYPE_);
static _NIL_TYPE_ CM1(_NIL_TYPE_);
static _NIL_TYPE_ CO1(_NIL_TYPE_);
static _NIL_TYPE_ CO2(_NIL_TYPE_);
static _NIL_TYPE_ DI1(_NIL_TYPE_);
static _NIL_TYPE_ DI2(_NIL_TYPE_);
static _NIL_TYPE_ DS1(_NIL_TYPE_);
static _NIL_TYPE_ DS2(_NIL_TYPE_);
static _NIL_TYPE_ DS3(_NIL_TYPE_);
static _NIL_TYPE_ EP1(_NIL_TYPE_);
static _NIL_TYPE_ EP2(_NIL_TYPE_);
static _NIL_TYPE_ EQ1(_NIL_TYPE_);
static _NIL_TYPE_ EQ2(_NIL_TYPE_);
static _NIL_TYPE_ ER1(_NIL_TYPE_);
static _NIL_TYPE_ EV1(_NIL_TYPE_);
static _NIL_TYPE_ FO1(_NIL_TYPE_);
static _NIL_TYPE_ FO2(_NIL_TYPE_);
static _NIL_TYPE_ FO3(_NIL_TYPE_);
static _NIL_TYPE_ FO4(_NIL_TYPE_);
static _NIL_TYPE_ GE1(_NIL_TYPE_);
static _NIL_TYPE_ GE2(_NIL_TYPE_);
static _NIL_TYPE_ GR1(_NIL_TYPE_);
static _NIL_TYPE_ IC1(_NIL_TYPE_);
static _NIL_TYPE_ ID1(_NIL_TYPE_);
static _NIL_TYPE_ ID2(_NIL_TYPE_);
static _NIL_TYPE_ ID3(_NIL_TYPE_);
static _NIL_TYPE_ IF1(_NIL_TYPE_);
static _NIL_TYPE_ IF2(_NIL_TYPE_);
static _NIL_TYPE_ IF3(_NIL_TYPE_);
static _NIL_TYPE_ IF4(_NIL_TYPE_);
static _NIL_TYPE_ IN1(_NIL_TYPE_);
static _NIL_TYPE_ IP1(_NIL_TYPE_);
static _NIL_TYPE_ IT1(_NIL_TYPE_);
static _NIL_TYPE_ IT2(_NIL_TYPE_);
static _NIL_TYPE_ IV1(_NIL_TYPE_);
static _NIL_TYPE_ LE1(_NIL_TYPE_);
static _NIL_TYPE_ LE2(_NIL_TYPE_);
static _NIL_TYPE_ LO1(_NIL_TYPE_);
static _NIL_TYPE_ LO2(_NIL_TYPE_);
static _NIL_TYPE_ LS1(_NIL_TYPE_);
static _NIL_TYPE_ MA1(_NIL_TYPE_);
static _NIL_TYPE_ MA2(_NIL_TYPE_);
static _NIL_TYPE_ MU1(_NIL_TYPE_);
static _NIL_TYPE_ MU2(_NIL_TYPE_);
static _NIL_TYPE_ NE1(_NIL_TYPE_);
static _NIL_TYPE_ NO1(_NIL_TYPE_);
static _NIL_TYPE_ NQ2(_NIL_TYPE_);
static _NIL_TYPE_ NAT(_NIL_TYPE_);
static _NIL_TYPE_ NU1(_NIL_TYPE_);
static _NIL_TYPE_ OR1(_NIL_TYPE_);
static _NIL_TYPE_ OR2(_NIL_TYPE_);
static _NIL_TYPE_ OR3(_NIL_TYPE_);
static _NIL_TYPE_ PA1(_NIL_TYPE_);
static _NIL_TYPE_ PO1(_NIL_TYPE_);
static _NIL_TYPE_ PO2(_NIL_TYPE_);
static _NIL_TYPE_ PR1(_NIL_TYPE_);
static _NIL_TYPE_ RE1(_NIL_TYPE_);
static _NIL_TYPE_ RE2(_NIL_TYPE_);
static _NIL_TYPE_ RE3(_NIL_TYPE_);
static _NIL_TYPE_ RN1(_NIL_TYPE_);
static _NIL_TYPE_ RST(_NIL_TYPE_);
static _NIL_TYPE_ SE1(_NIL_TYPE_);
static _NIL_TYPE_ SE2(_NIL_TYPE_);
static _NIL_TYPE_ SE3(_NIL_TYPE_);
static _NIL_TYPE_ SE4(_NIL_TYPE_);
static _NIL_TYPE_ SI1(_NIL_TYPE_);
static _NIL_TYPE_ SI2(_NIL_TYPE_);
static _NIL_TYPE_ SQ1(_NIL_TYPE_);
static _NIL_TYPE_ SU1(_NIL_TYPE_);
static _NIL_TYPE_ SU2(_NIL_TYPE_);
static _NIL_TYPE_ SWP(_NIL_TYPE_);
static _NIL_TYPE_ TA1(_NIL_TYPE_);
static _NIL_TYPE_ TA2(_NIL_TYPE_);
static _NIL_TYPE_ TA3(_NIL_TYPE_);
static _NIL_TYPE_ TE1(_NIL_TYPE_);
static _NIL_TYPE_ TR1(_NIL_TYPE_);
static _NIL_TYPE_ UN1(_NIL_TYPE_);
static _NIL_TYPE_ UN2(_NIL_TYPE_);
static _NIL_TYPE_ WH1(_NIL_TYPE_);
static _NIL_TYPE_ WH2(_NIL_TYPE_);

/*--------------------- private variables -------------------------*/

static _EXP_TYPE_ arg_VAR;
static _EXP_TYPE_ bln_VAR;
static _EXP_TYPE_ css_VAR;
static _EXP_TYPE_ dct_VAR;
static _EXP_TYPE_ fls_VAR;
static _EXP_TYPE_ tru_VAR;

static _EXP_TYPE_ cnt_NAM;
static _EXP_TYPE_ esc_NAM;
static _EXP_TYPE_ sel_NAM;

static _EXP_TYPE_ CCH;
static _EXP_TYPE_ MAX;
static _EXP_TYPE_ MIN;


static struct { _STR_TYPE_ nam;
                _INI_TYPE_ ini; } cst_TAB[] =
   /*--VARIABLES------*/
   {{ emp_STR, EMP },
    { eol_STR, EOL },
    { nll_STR, NLL },
    { quo_STR, QUO },
    { voi_STR, VOI }};

static struct { _STR_TYPE_ nam;
                _INI_TYPE_ ini; } var_TAB[] =
   /*--VARIABLES------*/
   {{ roo_STR, ROO }};

static struct { _STR_TYPE_ nam;
                _CNT_TYPE_ cnt; } fun_TAB[] =
    /*--ARITHMETIC-----*/
   {{ add_STR, ADD },
    { sub_STR, SUB },
    { mul_STR, MUL },
    { div_STR, DIV },
    { idv_STR, IDV },
    { rem_STR, REM },
    { pow_STR, POW },
    /*--RELATIONAL-----*/ 
    { lss_STR, LSS },
    { leq_STR, LEQ },
    { eql_STR, EQL },
    { neq_STR, NEQ },
    { grt_STR, GRT },
    { geq_STR, GEQ },
    { eqv_STR, EQV },
    { nqv_STR, NQV },
    /*--CONVERSION-----*/ 
    { trn_STR, TRN },
    { abs_STR, ABS },
    { chr_STR, CHR },
    { ord_STR, ORD },
    { num_STR, NUM },
    { tex_STR, TEX },
   /*--UTILITY--------*/
    { rnd_STR, RND },
    { clk_STR, CLK },
    /*--TRANSCENDENTAL-*/ 
    { sqt_STR, SQT },
    { sin_STR, SIN },
    { cos_STR, COS },
    { tan_STR, TAN },
    { asn_STR, ASN },
    { acs_STR, ACS },
    { atn_STR, ATN },
    { epw_STR, EPW },
    { log_STR, LOG },
    /*--STRING---------*/
    { len_STR, LEN },
    { epl_STR, EPL },
    { ipl_STR, IPL },
    /*--IDENTITY-------*/
    { ivd_STR, IVD },
    { inb_STR, INB },
    { ifr_STR, IFR },
    { itx_STR, ITX },
    { itb_STR, ITB },
    { ifu_STR, IFU },
    { idc_STR, IDC },
    { ict_STR, ICT },
    /*--TABLE----------*/
    { siz_STR, SIZ },
    { tab_STR, TAB },
    { pai_STR, PAI },
    /*--INTERACTION----*/
    { dsp_STR, DSP },
    { acp_STR, ACP },
    /*--BOOLEAN--------*/
    { trr_STR, TRR },
    { fls_STR, FLS },
    { and_STR, AND },
    { orr_STR, ORR },
    { not_STR, NOT },
    /*--CONTROL--------*/ 
    { beg_STR, BEG },
    { iff_STR, IFF },
    { whi_STR, WHI },
    { unt_STR, UNT },
    { for_STR, FOR },
    { cas_STR, CAS },
    /*--METALEVEL------*/ 
    { lod_STR, LOD },
    { dmp_STR, DMP },
    { rea_STR, RED },
    { eva_STR, EVA },
    { pri_STR, PRT },
    { tag_STR, TAG },
    { rnk_STR, RNK },
    { mak_STR, MAK },
    { get_STR, GET },
    { set_STR, SET },
    /*--DICTIONARIES--*/ 
    { cpt_STR, CPT },
    { cmt_STR, CMT },
    /*--CONTINUATIONS--*/ 
    { cll_STR, CLL },
    { con_STR, CON },
    { esc_STR, ESC },
    /*--ERROR----------*/ 
    { err_STR, ERR },
    /*--CASE-----------*/ 
    { sel_STR, SEL }};
    
/*--------------------- private functions -------------------------*/
 
/*-----------------------------------------------------------------*/
/*  SWP                                                            */
/*     EXP: [... ... ... ... EXP VAL] -> [... ... ... ... VAL EXP] */
/*     CNT: [... ... ... ... ... SWP] -> [... ... ... ... ... EXP] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ SWP(_NIL_TYPE_)
  { _EXP_TYPE_ exp, val;
    _stk_pop__EXP_(val);
    _stk_peek_EXP_(exp);
    _stk_poke_EXP_(val);
    _stk_push_EXP_(exp);
    _stk_poke_CNT_(EXP); }
    
/*-----------------------------------------------------------------*/
/*  RST                                                            */
/*     EXP: [... ... ... ... DCT VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... RST] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ RST(_NIL_TYPE_)
  { _EXP_TYPE_ val;
    _stk_pop__EXP_(val);
    _stk_peek_EXP_(_DCT_);
    _stk_poke_EXP_(val);
    _stk_zap__CNT_(); }
 
/*-----------------------------------------------------------------*/
/*  IGN                                                            */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... ...] */
/*     CNT: [... ... ... ... ... IGN] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ IGN(_NIL_TYPE_)
  { _stk_zap__EXP_();
    _stk_zap__CNT_(); }
    
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*-------------------------V A R I A B L E S-----------------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
static _EXP_TYPE_ EMP(_NIL_TYPE_)
  { return _ag_make_TAB_(0); }
  
static _EXP_TYPE_ EOL(_NIL_TYPE_)
  { return _CNT_make_NAM_(nwl_STR); }
  
static _EXP_TYPE_ NLL(_NIL_TYPE_)
  { return _CNT_make_NAM_(nul_STR); }
  
static _EXP_TYPE_ QUO(_NIL_TYPE_)
  { return _CNT_make_NAM_(qte_STR); }
  
static _EXP_TYPE_ ROO(_NIL_TYPE_)
  { return _VOI_; }
  
static _EXP_TYPE_ VOI(_NIL_TYPE_)
  { return _VOI_; }

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*-------------A R I T H M E T I C    O P E R A T O R S------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
 
/*-----------------------------------------------------------------*/
/*  ADD                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ARG EXP] */
/*     CNT: [... ... ... ... ... ADD] -> [... ... ... ... AD1 EXP] */
/*-----------------------------------------------------------------*/
    
static _NIL_TYPE_ ADD(_NIL_TYPE_)
  { N_ARY(AD1, 1, add_STR); }
   
/*-----------------------------------------------------------------*/
/*  AD1                                                            */
/*     EXP: [... ... ... ... ARG VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... AD1] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ... ... ARG VAL] -> [... ... ARG *2* VAL EXP] */
/*     CNT: [... ... ... ... ... AD1] -> [... ... ... ... AD2 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ AD1(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp, val;
    _UNS_TYPE_ siz;
    _stk_claim_();
    _stk_pop__EXP_(val);
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    if (siz > 1)
      { exp = _ag_get_TAB_EXP_(arg, 2);
        _stk_push_EXP_(_TWO_);
        _stk_push_EXP_(val);
        _stk_push_EXP_(exp);
        _stk_poke_CNT_(AD2);
        _stk_push_CNT_(EXP); 
        return;}
    if (!_ag_is_NUM_(val))
      _error_str_(_ATC_ERROR_, add_STR);
    _stk_poke_EXP_(val);
    _stk_zap__CNT_(); }
  
/*-----------------------------------------------------------------*/
/*  AD2                                                            */
/*     EXP: [... ... ARG NBR VAL VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... AD2] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ARG NBR VAL VAL] -> [... ... ARG NBR VAL EXP] */
/*     CNT: [... ... ... ... ... AD2] -> [... ... ... ... AD2 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ AD2(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp, nbr, va1, va2, val;
    _UNS_TYPE_ ctr, len, siz;
    _FLO_TYPE_ fl1, fl2, flo;
    _STR_TYPE_ st1, st2;
    _SGN_TYPE_ sg1, sg2, sgn;
    _mem_claim_();
    _stk_pop__EXP_(va2);
    _stk_pop__EXP_(va1);
    switch (_ag_get_TAG_(va1))
      { case _NBR_TAG_:
          sg1 = _ag_get_NBR_(va1);
          switch (_ag_get_TAG_(va2))
            { case _NBR_TAG_:
                sg2 = _ag_get_NBR_(va2);
                sgn = sg1 + sg2;
                if (labs(sgn) > _NBR_MAX_)
                  _error_str_(_NBR_ERROR_, add_STR); 
                val = _ag_make_NBR_(sgn);
                break;
              case _FRC_TAG_:
                fl2 = _ag_get_FRC_(va2);
                flo = sg1 + fl2;
                val = _ag_make_FRC_(flo);
                break;
              default:
                _error_str_(_ATC_ERROR_, add_STR); }
          break;
        case _FRC_TAG_:
          fl1 = _ag_get_FRC_(va1);
          switch (_ag_get_TAG_(va2))
            { case _NBR_TAG_:
                sg2 = _ag_get_NBR_(va2);
                flo = fl1 + sg2;
                val = _ag_make_FRC_(flo);
                break;
              case _FRC_TAG_:
                fl2 = _ag_get_FRC_(va2);
                flo = fl1 + fl2;
                val = _ag_make_FRC_(flo);
                break;
              default:
                _error_str_(_ATC_ERROR_, add_STR); }
          break;
        case _TXT_TAG_:
          st1 = _ag_get_TXT_(va1);
          switch (_ag_get_TAG_(va2))
            { case _TXT_TAG_:
                st2 = _ag_get_TXT_(va2);
                len = strlen(st1)+strlen(st2);
                if (len > _BUF_SIZE_)
                  _error_str_(_BUF_ERROR_, add_STR); 
                strcpy(_BUF_, st1);
                strcat(_BUF_, st2);
                _mem_claim_STR_(_BUF_);
                val = _CNT_make_NAM_(_BUF_);
                break;
              default:
                _error_str_(_ATC_ERROR_, add_STR); }
          break;
        default:
          _error_str_(_ATC_ERROR_, add_STR); }
    _stk_pop__EXP_(nbr);
    ctr = _ag_get_NBR_(nbr);
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    if (ctr < siz)
      { nbr = _ag_succ_NBR_(nbr);
        exp = _ag_get_TAB_EXP_(arg, ctr+1);
        _stk_push_EXP_(nbr);
        _stk_push_EXP_(val);
        _stk_push_EXP_(exp);
        _stk_push_CNT_(EXP);
        return; }
    _stk_poke_EXP_(val);
    _stk_zap__CNT_(); }
   
/*-----------------------------------------------------------------*/
/*  SUB                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ARG EXP] */
/*     CNT: [... ... ... ... ... SUB] -> [... ... ... ... SU1 EXP] */
/*-----------------------------------------------------------------*/
    
static _NIL_TYPE_ SUB(_NIL_TYPE_)
  { N_ARY(SU1, 1, sub_STR); }
 
/*-----------------------------------------------------------------*/
/*  SU1                                                            */
/*     EXP: [... ... ... ... ARG VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... SU1] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ... ... ARG VAL] -> [... ... ARG *2* VAL EXP] */
/*     CNT: [... ... ... ... ... SU1] -> [... ... ... ... SU2 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ SU1(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp, val;
    _UNS_TYPE_ siz;
    _SGN_TYPE_ sgn;
    _FLO_TYPE_ flo;
    _stk_claim_();
    _stk_pop__EXP_(val);
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    if (siz > 1)
      { exp = _ag_get_TAB_EXP_(arg, 2);
        _stk_push_EXP_(_TWO_);
        _stk_push_EXP_(val);
        _stk_push_EXP_(exp);
        _stk_poke_CNT_(SU2);
        _stk_push_CNT_(EXP);
        return; }
    switch (_ag_get_TAG_(val))
      { case _NBR_TAG_:
          sgn = _ag_get_NBR_(val);
          sgn = -sgn;
          val = _ag_make_NBR_(sgn);
          break;
        case _FRC_TAG_:
          flo = _ag_get_FRC_(val);
          flo = -flo;
          val = _ag_make_FRC_(flo);
          break;
        default:
         _error_str_(_ATC_ERROR_, sub_STR); }
    _stk_poke_EXP_(val);
    _stk_zap__CNT_(); }
    
/*-----------------------------------------------------------------*/
/*  SU2                                                            */
/*     EXP: [... ... ARG NBR VAL VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... SU2] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ARG NBR VAL VAL] -> [... ... ARG NBR VAL EXP] */
/*     CNT: [... ... ... ... ... SU2] -> [... ... ... ... SU2 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ SU2(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp, nbr, va1, va2, val;
    _UNS_TYPE_ ctr, siz;
    _FLO_TYPE_ fl1, fl2, flo;
    _SGN_TYPE_ sg1, sg2, sgn;
    _mem_claim_();
    _stk_pop__EXP_(va2);
    _stk_pop__EXP_(va1);
    switch (_ag_get_TAG_(va1))
      { case _NBR_TAG_:
          sg1 = _ag_get_NBR_(va1);
          switch (_ag_get_TAG_(va2))
            { case _NBR_TAG_:
                sg2 = _ag_get_NBR_(va2);
                sgn = sg1 - sg2;
                if (labs(sgn) > _NBR_MAX_)
                  _error_str_(_NBR_ERROR_, sub_STR); 
                val = _ag_make_NBR_(sgn);
                break;
              case _FRC_TAG_:
                fl2 = _ag_get_FRC_(va2);
                flo = sg1 - fl2;
                val = _ag_make_FRC_(flo);
                break;
              default:
                _error_str_(_ATC_ERROR_, sub_STR); }
          break;
        case _FRC_TAG_:
          fl1 = _ag_get_FRC_(va1);
          switch (_ag_get_TAG_(va2))
            { case _NBR_TAG_:
                sg2 = _ag_get_NBR_(va2);
                flo = fl1 - sg2;
                val = _ag_make_FRC_(flo);
                break;
              case _FRC_TAG_:
                fl2 = _ag_get_FRC_(va2);
                flo = fl1 - fl2;
                val = _ag_make_FRC_(flo);
                break;
              default:
                _error_str_(_ATC_ERROR_, sub_STR); }
          break;
        default:
          _error_str_(_ATC_ERROR_, sub_STR); }
    _stk_pop__EXP_(nbr);
    ctr = _ag_get_NBR_(nbr);
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    if (ctr < siz)
      { nbr = _ag_succ_NBR_(nbr);
        exp = _ag_get_TAB_EXP_(arg, ctr+1);
        _stk_push_EXP_(nbr);
        _stk_push_EXP_(val);
        _stk_push_EXP_(exp);		 
        _stk_push_CNT_(EXP);
        return; }
    _stk_poke_EXP_(val);
    _stk_zap__CNT_(); }
      
/*-----------------------------------------------------------------*/
/*  MUL                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ARG EXP] */
/*     CNT: [... ... ... ... ... MUL] -> [... ... ... ... MU1 EXP] */
/*-----------------------------------------------------------------*/
    
static _NIL_TYPE_ MUL(_NIL_TYPE_)
  { N_ARY(MU1, 2, mul_STR); }
   
/*-----------------------------------------------------------------*/
/*  MU1                                                            */
/*     EXP: [... ... ... ... ARG VAL] -> [... ... ARG *2* VAL EXP] */
/*     CNT: [... ... ... ... ... MU1] -> [... ... ... ... MU2 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ MU1(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp, val;
    _stk_claim_();
    _stk_pop__EXP_(val);
    _stk_peek_EXP_(arg);
    exp = _ag_get_TAB_EXP_(arg, 2);
    _stk_push_EXP_(_TWO_);
    _stk_push_EXP_(val);
    _stk_push_EXP_(exp);
    _stk_poke_CNT_(MU2);
    _stk_push_CNT_(EXP); }
 
/*-----------------------------------------------------------------*/
/*  MU2                                                            */
/*     EXP: [... ... ARG NBR VAL VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... MU2] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ARG NBR VAL VAL] -> [... ... ARG NBR VAL EXP] */
/*     CNT: [... ... ... ... ... MU2] -> [... ... ... ... MU2 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ MU2(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp, nbr, va1, va2, val;
    _UNS_TYPE_ ctr, siz;
    _FLO_TYPE_ fl1, fl2, flo;
    _SGN_TYPE_ sg1, sg2, sgn;
    _mem_claim_();
    _stk_pop__EXP_(va2);
    _stk_pop__EXP_(va1);
    switch (_ag_get_TAG_(va1))
      { case _NBR_TAG_:
          sg1 = _ag_get_NBR_(va1);
          switch (_ag_get_TAG_(va2))
            { case _NBR_TAG_:
                sg2 = _ag_get_NBR_(va2);
                flo = (_FLO_TYPE_)sg1 * (_FLO_TYPE_)sg2;
                if (fabs(flo) > _NBR_MAX_)
                  _error_str_(_NBR_ERROR_, mul_STR);
                sgn = (_SGN_TYPE_)flo;
                if (labs(sgn) > _NBR_MAX_)
                  _error_str_(_NBR_ERROR_, mul_STR); 
                val = _ag_make_NBR_(sgn);
                break;
              case _FRC_TAG_:
                fl2 = _ag_get_FRC_(va2);
                flo = sg1 * fl2;
                val = _ag_make_FRC_(flo);
                break;
              default:
                _error_str_(_ATC_ERROR_, mul_STR); }
          break;
        case _FRC_TAG_:
          fl1 = _ag_get_FRC_(va1);
          switch (_ag_get_TAG_(va2))
            { case _NBR_TAG_:
                sg2 = _ag_get_NBR_(va2);
                flo = fl1 * sg2;
                val = _ag_make_FRC_(flo);
                break;
              case _FRC_TAG_:
                fl2 = _ag_get_FRC_(va2);
                flo = fl1 * fl2;
                val = _ag_make_FRC_(flo);
                break;
              default:
                _error_str_(_ATC_ERROR_, mul_STR); }
          break;
        default:
          _error_str_(_ATC_ERROR_, mul_STR); }
    _stk_pop__EXP_(nbr);
    ctr = _ag_get_NBR_(nbr);
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    if (ctr < siz)
      { nbr = _ag_succ_NBR_(nbr);
        exp = _ag_get_TAB_EXP_(arg, ctr+1);
        _stk_push_EXP_(nbr);
        _stk_push_EXP_(val);
        _stk_push_EXP_(exp);  
        _stk_push_CNT_(EXP);
        return; }
    _stk_poke_EXP_(val);
    _stk_zap__CNT_(); }
        
/*-----------------------------------------------------------------*/
/*  DIV                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ARG EXP] */
/*     CNT: [... ... ... ... ... DIV] -> [... ... ... ... DI1 EXP] */
/*-----------------------------------------------------------------*/
    
static _NIL_TYPE_ DIV(_NIL_TYPE_)
  { N_ARY(DI1, 2, div_STR); }
 
/*-----------------------------------------------------------------*/
/*  DI1                                                            */
/*     EXP: [... ... ... ... ARG VAL] -> [... ... ARG *2* VAL EXP] */
/*     CNT: [... ... ... ... ... DI1] -> [... ... ... ... DI2 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ DI1(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp, val;
    _stk_claim_();
    _stk_pop__EXP_(val);
    _stk_peek_EXP_(arg);
    exp = _ag_get_TAB_EXP_(arg, 2);
    _stk_push_EXP_(_TWO_);
    _stk_push_EXP_(val);
    _stk_push_EXP_(exp);
    _stk_poke_CNT_(DI2);
    _stk_push_CNT_(EXP); }
    
/*-----------------------------------------------------------------*/
/*  DI2                                                            */
/*     EXP: [... ... ARG NBR VAL VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... DI2] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ARG NBR VAL VAL] -> [... ... ARG NBR VAL EXP] */
/*     CNT: [... ... ... ... ... DI2] -> [... ... ... ... DI2 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ DI2(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp, nbr, va1, va2, val;
    _UNS_TYPE_ ctr, siz;
    _FLO_TYPE_ fl1, fl2, flo;
    _SGN_TYPE_ sg1, sg2;
    _mem_claim_();
    _stk_pop__EXP_(va2);
    _stk_pop__EXP_(va1);
    switch (_ag_get_TAG_(va1))
      { case _NBR_TAG_:
          sg1 = _ag_get_NBR_(va1);
          switch (_ag_get_TAG_(va2))
            { case _NBR_TAG_:
                sg2 = _ag_get_NBR_(va2);
                if (sg2 == 0)
                  _error_str_(_ZDV_ERROR_, div_STR);
                flo = (_FLO_TYPE_)sg1 / (_FLO_TYPE_)sg2;
                val = _ag_make_FRC_(flo);
                break;
              case _FRC_TAG_:
                fl2 = _ag_get_FRC_(va2);
                if (fabs(fl2) < _FLO_MIN_)
                  _error_str_(_ZDV_ERROR_, div_STR);
                flo = sg1 / fl2;
                val = _ag_make_FRC_(flo);
                break;
              default:
                _error_str_(_ATC_ERROR_, div_STR); }
          break;
        case _FRC_TAG_:
          fl1 = _ag_get_FRC_(va1);
          switch (_ag_get_TAG_(va2))
            { case _NBR_TAG_:
                sg2 = _ag_get_NBR_(va2);
                if (sg2 == 0)
                  _error_str_(_ZDV_ERROR_, div_STR);
                flo = fl1 / sg2;
                val = _ag_make_FRC_(flo);
                break;
              case _FRC_TAG_:
                fl2 = _ag_get_FRC_(va2);
                if (fabs(fl2) < _FLO_MIN_)
                  _error_str_(_ZDV_ERROR_, div_STR);
                flo = fl1 / fl2;
                val = _ag_make_FRC_(flo);
                break;
              default:
                _error_str_(_ATC_ERROR_, div_STR); }
          break;
        default:
          _error_str_(_ATC_ERROR_, div_STR); }
    _stk_pop__EXP_(nbr);
    ctr = _ag_get_NBR_(nbr);
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    if (ctr < siz)
      { nbr = _ag_succ_NBR_(nbr);
        exp = _ag_get_TAB_EXP_(arg, ctr+1);
        _stk_push_EXP_(nbr);
        _stk_push_EXP_(val);
        _stk_push_EXP_(exp);		
        _stk_push_CNT_(EXP);
        return; }
    _stk_poke_EXP_(val);
    _stk_zap__CNT_(); }
 
/*-----------------------------------------------------------------*/
/*  IDV                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ARG EXP] */
/*     CNT: [... ... ... ... ... IDV] -> [... ... ... ... ID1 EXP] */
/*-----------------------------------------------------------------*/
    
static _NIL_TYPE_ IDV(_NIL_TYPE_)
  { N_ARY(ID1, 2, idv_STR); }
 
/*-----------------------------------------------------------------*/
/*  ID1                                                            */
/*     EXP: [... ... ... ... ARG VAL] -> [... ... ARG *2* VAL EXP] */
/*     CNT: [... ... ... ... ... ID1] -> [... ... ... ... ID2 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ ID1(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp, val;
    _stk_claim_();
    _stk_pop__EXP_(val);
    _stk_peek_EXP_(arg);
    exp = _ag_get_TAB_EXP_(arg, 2);
    _stk_push_EXP_(_TWO_);
    _stk_push_EXP_(val);
    _stk_push_EXP_(exp);
    _stk_poke_CNT_(ID2);
    _stk_push_CNT_(EXP); }
    
/*-----------------------------------------------------------------*/
/*  ID2                                                            */
/*     EXP: [... ... ARG NBR VAL VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... ID2] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ARG NBR VAL VAL] -> [... ... ARG NBR VAL EXP] */
/*     CNT: [... ... ... ... ... ID2] -> [... ... ... ... ID2 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ ID2(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp, nbr, va1, va2, val;
    _UNS_TYPE_ ctr, siz;
    _SGN_TYPE_ sg1, sg2, sgn;
    _mem_claim_();
    _stk_pop__EXP_(va2);
    _stk_pop__EXP_(va1);
    switch (_ag_get_TAG_(va1))
      { case _NBR_TAG_:
          sg1 = _ag_get_NBR_(va1);
          switch (_ag_get_TAG_(va2))
            { case _NBR_TAG_:
                sg2 = _ag_get_NBR_(va2);
                if (sg2 == 0)
                  _error_str_(_ZDV_ERROR_, idv_STR);
                sgn = sg1 / sg2;
                val = _ag_make_NBR_(sgn);
                break;
              default:
                _error_str_(_ATC_ERROR_, idv_STR); }
          break;
        default:
          _error_str_(_ATC_ERROR_, idv_STR); }
    _stk_pop__EXP_(nbr);
    ctr = _ag_get_NBR_(nbr);
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    if (ctr < siz)
      { nbr = _ag_succ_NBR_(nbr);
        exp = _ag_get_TAB_EXP_(arg, ctr+1);
        _stk_push_EXP_(nbr);
        _stk_push_EXP_(val);
        _stk_push_EXP_(exp);		
        _stk_push_CNT_(EXP);
        return; }
    _stk_poke_EXP_(val);
    _stk_zap__CNT_(); }
 
/*-----------------------------------------------------------------*/
/*  REM                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ARG EXP] */
/*     CNT: [... ... ... ... ... REM] -> [... ... ... ... RE1 EXP] */
/*-----------------------------------------------------------------*/
    
static _NIL_TYPE_ REM(_NIL_TYPE_)
  { N_ARY(RE1, 2, rem_STR); }
  
/*-----------------------------------------------------------------*/
/*  RE1                                                            */
/*     EXP: [... ... ... ... ARG VAL] -> [... ... ARG *2* VAL EXP] */
/*     CNT: [... ... ... ... ... RE1] -> [... ... ... ... RE2 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ RE1(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp, val;
    _stk_claim_();
    _stk_pop__EXP_(val);
    _stk_peek_EXP_(arg);
    exp = _ag_get_TAB_EXP_(arg, 2);
    _stk_push_EXP_(_TWO_);
    _stk_push_EXP_(val);
    _stk_push_EXP_(exp);
    _stk_poke_CNT_(RE2);
    _stk_push_CNT_(EXP); }
    
/*-----------------------------------------------------------------*/
/*  RE2                                                            */
/*     EXP: [... ... ARG NBR VAL VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... RE2] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ARG NBR VAL VAL] -> [... ... ARG NBR VAL EXP] */
/*     CNT: [... ... ... ... ... RE2] -> [... ... ... ... RE2 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ RE2(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp, nbr, va1, va2, val;
    _UNS_TYPE_ ctr, siz;
    _SGN_TYPE_ sg1, sg2, sgn;
    _mem_claim_();
    _stk_pop__EXP_(va2);
    _stk_pop__EXP_(va1);
    switch (_ag_get_TAG_(va1))
      { case _NBR_TAG_:
          sg1 = _ag_get_NBR_(va1);
          switch (_ag_get_TAG_(va2))
            { case _NBR_TAG_:
                sg2 = _ag_get_NBR_(va2);
                if (sg2 == 0)
                  _error_str_(_ZDV_ERROR_, rem_STR);
                sgn = sg1 % sg2;
                val = _ag_make_NBR_(sgn);
                break;
              default:
                _error_str_(_ATC_ERROR_, rem_STR); }
          break;
        default:
          _error_str_(_ATC_ERROR_, rem_STR); }
    _stk_pop__EXP_(nbr);
    ctr = _ag_get_NBR_(nbr);
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    if (ctr < siz)
      { nbr = _ag_succ_NBR_(nbr);
        exp = _ag_get_TAB_EXP_(arg, ctr+1);
        _stk_push_EXP_(nbr);
        _stk_push_EXP_(val);
        _stk_push_EXP_(exp); 
        _stk_push_CNT_(EXP);
        return; }
    _stk_poke_EXP_(val);
    _stk_zap__CNT_(); }
 
/*-----------------------------------------------------------------*/
/*  POW                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ARG EXP] */
/*     CNT: [... ... ... ... ... POW] -> [... ... ... ... PO1 EXP] */
/*-----------------------------------------------------------------*/
    
static _NIL_TYPE_ POW(_NIL_TYPE_)
  { N_ARY(PO1, 2, pow_STR); }
 
/*-----------------------------------------------------------------*/
/*  PO1                                                            */
/*     EXP: [... ... ... ... ARG VAL] -> [... ... ARG *2* VAL EXP] */
/*     CNT: [... ... ... ... ... PO1] -> [... ... ... ... PO2 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ PO1(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp, val;
    _stk_claim_();
    _stk_pop__EXP_(val);
    _stk_peek_EXP_(arg);
    exp = _ag_get_TAB_EXP_(arg, 2);
    _stk_push_EXP_(_TWO_);
    _stk_push_EXP_(val);
    _stk_push_EXP_(exp);
    _stk_poke_CNT_(PO2);
    _stk_push_CNT_(EXP); }
    
/*-----------------------------------------------------------------*/
/*  PO2                                                            */
/*     EXP: [... ... ARG NBR VAL VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... PO2] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ARG NBR VAL VAL] -> [... ... ARG NBR VAL EXP] */
/*     CNT: [... ... ... ... ... PO2] -> [... ... ... ... PO2 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ PO2(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp, nbr, va1, va2, val;
    _UNS_TYPE_ ctr, siz;
    _FLO_TYPE_ fl1, fl2, flo;
    _SGN_TYPE_ sg1, sg2;
    _mem_claim_();
    _stk_pop__EXP_(va2);
    _stk_pop__EXP_(va1);
    switch (_ag_get_TAG_(va1))
      { case _NBR_TAG_:
          sg1 = _ag_get_NBR_(va1);
          switch (_ag_get_TAG_(va2))
            { case _NBR_TAG_:
                sg2 = _ag_get_NBR_(va2);
                flo = pow(sg1, sg2);
                if (fabs(flo) >= _FRC_MAX_)
                  _error_str_(_INF_ERROR_, pow_STR);
                val = _ag_make_FRC_(flo);
                break;
              case _FRC_TAG_:
                if (sg1 < 0)
                  _error_str_(_NEG_ERROR_, pow_STR);
                fl2 = _ag_get_FRC_(va2);
                flo = pow(sg1, fl2);
                if (fabs(flo) >= _FRC_MAX_)
                  _error_str_(_INF_ERROR_, pow_STR);
                val = _ag_make_FRC_(flo);
                break;
              default:
                _error_str_(_ATC_ERROR_, pow_STR); }
          break;
        case _FRC_TAG_:
          fl1 = _ag_get_FRC_(va1);
          switch (_ag_get_TAG_(va2))
            { case _NBR_TAG_:
                sg2 = _ag_get_NBR_(va2);
                flo = pow(fl1, sg2);
                if (fabs(flo) >= _FRC_MAX_)
                  _error_str_(_INF_ERROR_, pow_STR);
                val = _ag_make_FRC_(flo);
                break;
              case _FRC_TAG_:
                if (fl1 < 0)
                  _error_str_(_NEG_ERROR_, pow_STR);
                fl2 = _ag_get_FRC_(va2);
                flo = pow(fl1, fl2);
                if (fabs(flo) >= _FRC_MAX_)
                  _error_str_(_INF_ERROR_, pow_STR);
                val = _ag_make_FRC_(flo);
                break;
              default:
                _error_str_(_ATC_ERROR_, pow_STR); }
          break;
        default:
          _error_str_(_ATC_ERROR_, pow_STR); }
    _stk_pop__EXP_(nbr);
    ctr = _ag_get_NBR_(nbr);
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    if (ctr < siz)
      { nbr = _ag_succ_NBR_(nbr);
        exp = _ag_get_TAB_EXP_(arg, ctr+1);
        _stk_push_EXP_(nbr);
        _stk_push_EXP_(val);
        _stk_push_EXP_(exp);		 
        _stk_push_CNT_(EXP);
        return; }
    _stk_poke_EXP_(val);
    _stk_zap__CNT_(); }
    
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*------------R E L A T I O N A L    O P E R A T O R S-------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
 
/*-----------------------------------------------------------------*/
/*  LSS                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... EXP EXP] */
/*     CNT: [... ... ... ... ... LSS] -> [... ... ... LS1 SWP EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ LSS(_NIL_TYPE_)
  { BINARY(LS1, lss_STR); }
 
/*-----------------------------------------------------------------*/
/*  LS1                                                            */
/*     EXP: [... ... ... ... VAL VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... LS1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
  
static _NIL_TYPE_ LS1(_NIL_TYPE_)
  { _EXP_TYPE_ va1, va2, val;
    _FLO_TYPE_ fl1, fl2;
    _SGN_TYPE_ sg1, sg2;
    _STR_TYPE_ st1, st2;
    _stk_pop__EXP_(va2);
    _stk_peek_EXP_(va1);
    _stk_zap__CNT_();
    if (_ag_is_NBR_(va1))
      { sg1 = _ag_get_NBR_(va1);
        if (_ag_is_NBR_(va2))
          { sg2 = _ag_get_NBR_(va2);
            val = (sg1 < sg2)?tru_VAR:fls_VAR;
            _stk_poke_EXP_(val);
            return; }
        if (_ag_is_FRC__(va2))
          { fl2 = _ag_get_FRC_(va2);
            val = (sg1 < fl2)?tru_VAR:fls_VAR;
            _stk_poke_EXP_(val);
            return; }}
    else        
      switch (_ag_get_TAG__(va1))
        { case _FRC_TAG_:
            { fl1 = _ag_get_FRC_(va1);
              if (_ag_is_NBR_(va2))
                { sg2 = _ag_get_NBR_(va2);
                  val = (fl1 < sg2)?tru_VAR:fls_VAR;
                  _stk_poke_EXP_(val);
                  return; }
              if (_ag_is_FRC__(va2))
                { fl2 = _ag_get_FRC_(va2);
                  val = (fl1 < fl2)?tru_VAR:fls_VAR;
                  _stk_poke_EXP_(val);
                  return; }}
          case _TXT_TAG_:
            { st1 = _ag_get_TXT_(va1);
              if (_ag_is_TXT_(va2))
                { st2 = _ag_get_TXT_(va2);
                  val = (strcmp(st1, st2) < 0)?tru_VAR:fls_VAR;
                  _stk_poke_EXP_(val);
                  return; }}}
    _error_str_(_ATC_ERROR_, lss_STR); }

/*-----------------------------------------------------------------*/
/*  LEQ                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... EXP EXP] */
/*     CNT: [... ... ... ... ... LEQ] -> [... ... ... LE1 SWP EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ LEQ(_NIL_TYPE_)
  { BINARY(LE1, leq_STR); }
 
/*-----------------------------------------------------------------*/
/*  LE1                                                            */
/*     EXP: [... ... ... ... VAL VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... LE1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ LE1(_NIL_TYPE_)
  { _EXP_TYPE_ va1, va2, val;
    _FLO_TYPE_ fl1, fl2;
    _SGN_TYPE_ sg1, sg2;
    _STR_TYPE_ st1, st2;
    _stk_pop__EXP_(va2);
    _stk_peek_EXP_(va1);
    _stk_zap__CNT_();
    if (_ag_is_NBR_(va1))
      { sg1 = _ag_get_NBR_(va1);
        if (_ag_is_NBR_(va2))
          { sg2 = _ag_get_NBR_(va2);
            val = (sg1 <= sg2)?tru_VAR:fls_VAR;
            _stk_poke_EXP_(val);
            return; }
        if (_ag_is_FRC__(va2))
          { fl2 = _ag_get_FRC_(va2);
            val = (sg1 <= fl2)?tru_VAR:fls_VAR;
            _stk_poke_EXP_(val);
            return; }}
    else        
      switch (_ag_get_TAG__(va1))
        { case _FRC_TAG_:
            { fl1 = _ag_get_FRC_(va1);
              if (_ag_is_NBR_(va2))
                { sg2 = _ag_get_NBR_(va2);
                  val = (fl1 <= sg2)?tru_VAR:fls_VAR;
                  _stk_poke_EXP_(val);
                  return; }
              if (_ag_is_FRC__(va2))
                { fl2 = _ag_get_FRC_(va2);
                  val = (fl1 <= fl2)?tru_VAR:fls_VAR;
                  _stk_poke_EXP_(val);
                  return; }}
          case _TXT_TAG_:
            { st1 = _ag_get_TXT_(va1);
              if (_ag_is_TXT_(va2))
                { st2 = _ag_get_TXT_(va2);
                  val = (strcmp(st1, st2) <= 0)?tru_VAR:fls_VAR;
                  _stk_poke_EXP_(val);
                  return; }}}
    _error_str_(_ATC_ERROR_, leq_STR); }
 
/*-----------------------------------------------------------------*/
/*  EQL                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... EXP EXP] */
/*     CNT: [... ... ... ... ... EQL] -> [... ... ... EQ1 SWP EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ EQL(_NIL_TYPE_)
  { BINARY(EQ1, eql_STR); }

/*-----------------------------------------------------------------*/
/*  EQ1                                                            */
/*     EXP: [... ... ... ... VAL VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... EQ1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ EQ1(_NIL_TYPE_)
  { _EXP_TYPE_ va1, va2, val;
    _FLO_TYPE_ fl1, fl2;
    _SGN_TYPE_ sg1, sg2;
    _STR_TYPE_ st1, st2;
    _stk_pop__EXP_(va2);
    _stk_peek_EXP_(va1);
    _stk_zap__CNT_();
    if (_ag_is_NBR_(va1))
      { sg1 = _ag_get_NBR_(va1);
        if (_ag_is_NBR_(va2))
          { sg2 = _ag_get_NBR_(va2);
            val = (sg1 == sg2)?tru_VAR:fls_VAR;
            _stk_poke_EXP_(val);
            return; }
        if (_ag_is_FRC__(va2))
          { fl2 = _ag_get_FRC_(va2);
            val = (fabs(sg1 - fl2) < _FLO_MIN_)?tru_VAR:fls_VAR;
            _stk_poke_EXP_(val);
            return; }}
    else        
      switch (_ag_get_TAG__(va1))
        { case _FRC_TAG_:
            { fl1 = _ag_get_FRC_(va1);
              if (_ag_is_NBR_(va2))
                { sg2 = _ag_get_NBR_(va2);
                  val = (fabs(fl1 - sg2) < _FLO_MIN_)?tru_VAR:fls_VAR;
                  _stk_poke_EXP_(val);
                  return; }
              if (_ag_is_FRC__(va2))
                { fl2 = _ag_get_FRC_(va2);
                  val = (fabs(fl1 - fl2) < _FLO_MIN_)?tru_VAR:fls_VAR;
                  _stk_poke_EXP_(val);
                  return; }}
          case _TXT_TAG_:
            { st1 = _ag_get_TXT_(va1);
              if (_ag_is_TXT_(va2))
                { st2 = _ag_get_TXT_(va2);
                  val = (strcmp(st1, st2) == 0)?tru_VAR:fls_VAR;
                  _stk_poke_EXP_(val);
                  return; }}}
    _error_str_(_ATC_ERROR_, eql_STR); }
   
/*-----------------------------------------------------------------*/
/*  NEQ                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... EXP EXP] */
/*     CNT: [... ... ... ... ... NEQ] -> [... ... ... NE1 SWP EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ NEQ(_NIL_TYPE_)
  { BINARY(NE1, neq_STR); }
   
/*-----------------------------------------------------------------*/
/*  NE1                                                            */
/*     EXP: [... ... ... ... VAL VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... NE1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ NE1(_NIL_TYPE_)
  { _EXP_TYPE_ va1, va2, val;
    _FLO_TYPE_ fl1, fl2;
    _SGN_TYPE_ sg1, sg2;
    _STR_TYPE_ st1, st2;
    _stk_pop__EXP_(va2);
    _stk_peek_EXP_(va1);
    _stk_zap__CNT_();
    if (_ag_is_NBR_(va1))
      { sg1 = _ag_get_NBR_(va1);
        if (_ag_is_NBR_(va2))
          { sg2 = _ag_get_NBR_(va2);
            val = (sg1 != sg2)?tru_VAR:fls_VAR;
            _stk_poke_EXP_(val);
            return; }
        if (_ag_is_FRC__(va2))
          { fl2 = _ag_get_FRC_(va2);
            val = (fabs(sg1 - fl2) >= _FLO_MIN_)?tru_VAR:fls_VAR;
            _stk_poke_EXP_(val);
            return; }}
    else        
      switch (_ag_get_TAG__(va1))
        { case _FRC_TAG_:
            { fl1 = _ag_get_FRC_(va1);
              if (_ag_is_NBR_(va2))
                { sg2 = _ag_get_NBR_(va2);
                  val = (fabs(fl1 - sg2) >= _FLO_MIN_)?tru_VAR:fls_VAR;
                  _stk_poke_EXP_(val);
                  return; }
              if (_ag_is_FRC__(va2))
                { fl2 = _ag_get_FRC_(va2);
                  val = (fabs(fl1 - fl2) >= _FLO_MIN_)?tru_VAR:fls_VAR;
                  _stk_poke_EXP_(val);
                  return; }}
          case _TXT_TAG_:
            { st1 = _ag_get_TXT_(va1);
              if (_ag_is_TXT_(va2))
                { st2 = _ag_get_TXT_(va2);
                  val = (strcmp(st1, st2) != 0)?tru_VAR:fls_VAR;
                  _stk_poke_EXP_(val);
                  return; }}}
    _error_str_(_ATC_ERROR_, neq_STR); }
 
/*-----------------------------------------------------------------*/
/*  GRT                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... EXP EXP] */
/*     CNT: [... ... ... ... ... GRT] -> [... ... ... GR1 SWP EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ GRT(_NIL_TYPE_)
  { BINARY(GR1, grt_STR); }
 
/*-----------------------------------------------------------------*/
/*  GR1                                                            */
/*     EXP: [... ... ... ... VAL VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... GR1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ GR1(_NIL_TYPE_)
  { _EXP_TYPE_ va1, va2, val;
    _FLO_TYPE_ fl1, fl2;
    _SGN_TYPE_ sg1, sg2;
    _STR_TYPE_ st1, st2;
    _stk_pop__EXP_(va2);
    _stk_peek_EXP_(va1);
    _stk_zap__CNT_();
    if (_ag_is_NBR_(va1))
      { sg1 = _ag_get_NBR_(va1);
        if (_ag_is_NBR_(va2))
          { sg2 = _ag_get_NBR_(va2);
            val = (sg1 > sg2)?tru_VAR:fls_VAR;
            _stk_poke_EXP_(val);
            return; }
        if (_ag_is_FRC__(va2))
          { fl2 = _ag_get_FRC_(va2);
            val = (sg1 > fl2)?tru_VAR:fls_VAR;
            _stk_poke_EXP_(val);
            return; }}
    else        
      switch (_ag_get_TAG__(va1))
        { case _FRC_TAG_:
            { fl1 = _ag_get_FRC_(va1);
              if (_ag_is_NBR_(va2))
                { sg2 = _ag_get_NBR_(va2);
                  val = (fl1 > sg2)?tru_VAR:fls_VAR;
                  _stk_poke_EXP_(val);
                  return; }
              if (_ag_is_FRC__(va2))
                { fl2 = _ag_get_FRC_(va2);
                  val = (fl1 > fl2)?tru_VAR:fls_VAR;
                  _stk_poke_EXP_(val);
                  return; }}
          case _TXT_TAG_:
            { st1 = _ag_get_TXT_(va1);
              if (_ag_is_TXT_(va2))
                { st2 = _ag_get_TXT_(va2);
                  val = (strcmp(st1, st2) > 0)?tru_VAR:fls_VAR;
                  _stk_poke_EXP_(val);
                  return; }}}
    _error_str_(_ATC_ERROR_, grt_STR); }
 
/*-----------------------------------------------------------------*/
/*  GEQ                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... EXP EXP] */
/*     CNT: [... ... ... ... ... GEQ] -> [... ... ... GE1 SWP EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ GEQ(_NIL_TYPE_)
  { BINARY(GE1, geq_STR); }
 
/*-----------------------------------------------------------------*/
/*  GE1                                                            */
/*     EXP: [... ... ... ... VAL VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... GE1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
  
static _NIL_TYPE_ GE1(_NIL_TYPE_)
  { _EXP_TYPE_ va1, va2, val;
    _FLO_TYPE_ fl1, fl2;
    _SGN_TYPE_ sg1, sg2;
    _STR_TYPE_ st1, st2;
    _stk_pop__EXP_(va2);
    _stk_peek_EXP_(va1);
    _stk_zap__CNT_();
    if (_ag_is_NBR_(va1))
      { sg1 = _ag_get_NBR_(va1);
        if (_ag_is_NBR_(va2))
          { sg2 = _ag_get_NBR_(va2);
            val = (sg1 >= sg2)?tru_VAR:fls_VAR;
            _stk_poke_EXP_(val);
            return; }
        if (_ag_is_FRC__(va2))
          { fl2 = _ag_get_FRC_(va2);
            val = (sg1 >= fl2)?tru_VAR:fls_VAR;
            _stk_poke_EXP_(val);
            return; }}
    else        
      switch (_ag_get_TAG__(va1))
        { case _FRC_TAG_:
            { fl1 = _ag_get_FRC_(va1);
              if (_ag_is_NBR_(va2))
                { sg2 = _ag_get_NBR_(va2);
                  val = (fl1 >= sg2)?tru_VAR:fls_VAR;
                  _stk_poke_EXP_(val);
                  return; }
              if (_ag_is_FRC__(va2))
                { fl2 = _ag_get_FRC_(va2);
                  val = (fl1 >= fl2)?tru_VAR:fls_VAR;
                  _stk_poke_EXP_(val);
                  return; }}
          case _TXT_TAG_:
            { st1 = _ag_get_TXT_(va1);
              if (_ag_is_TXT_(va2))
                { st2 = _ag_get_TXT_(va2);
                  val = (strcmp(st1, st2) >= 0)?tru_VAR:fls_VAR;
                  _stk_poke_EXP_(val);
                  return; }}}
    _error_str_(_ATC_ERROR_, geq_STR); }
 
/*-----------------------------------------------------------------*/
/*  EQV                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... EXP EXP] */
/*     CNT: [... ... ... ... ... EQV] -> [... ... ... EQ2 SWP EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ EQV(_NIL_TYPE_)
  { BINARY(EQ2, eqv_STR); }
   
/*-----------------------------------------------------------------*/
/*  EQ2                                                            */
/*     EXP: [... ... ... ... VAL VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... EQ2] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ EQ2(_NIL_TYPE_)
  { _EXP_TYPE_ va1, va2, val;
    _stk_pop__EXP_(va2);
    _stk_peek_EXP_(va1);
    _stk_zap__CNT_();
    val = (_mem_is_same_(va1, va2))?tru_VAR:fls_VAR; 
    _stk_poke_EXP_(val); }
 
/*-----------------------------------------------------------------*/
/*  NQV                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... EXP EXP] */
/*     CNT: [... ... ... ... ... NQV] -> [... ... ... NQ2 SWP EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ NQV(_NIL_TYPE_)
  { BINARY(NQ2, nqv_STR); }
   
/*-----------------------------------------------------------------*/
/*  NQ2                                                            */
/*     EXP: [... ... ... ... VAL VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... NQ2] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ NQ2(_NIL_TYPE_)
  { _EXP_TYPE_ va1, va2, val;
    _stk_pop__EXP_(va2);
    _stk_peek_EXP_(va1);
    _stk_zap__CNT_();
    val = (_mem_is_same_(va1, va2))?fls_VAR:tru_VAR; 
    _stk_poke_EXP_(val); }
 
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*-------------C O N V E R S I O N     O P E R A T O R S-----------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
 
/*-----------------------------------------------------------------*/
/*  TRN                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... TRN] -> [... ... ... ... TR1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ TRN(_NIL_TYPE_)
  { UNARY(TR1, trn_STR); }
 
/*-----------------------------------------------------------------*/
/*  TR1                                                            */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... TR1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ TR1(_NIL_TYPE_)
  { _EXP_TYPE_ nbr, val;
    _FLO_TYPE_ flo;
    _SGN_TYPE_ sgn;
    _stk_peek_EXP_(val);
    _stk_zap__CNT_();
    if (_ag_is_NBR_(val))
      return; 
    if (_ag_is_FRC__(val))
      { flo = _ag_get_FRC_(val);
        sgn = (_SGN_TYPE_)flo;
        if (labs(sgn) > _NBR_MAX_)
          _error_str_(_NBR_ERROR_, trn_STR);
        nbr = _ag_make_NBR_(sgn);
        _stk_poke_EXP_(nbr);     
        return; }
    _error_str_(_ATC_ERROR_, trn_STR); }
    
/*-----------------------------------------------------------------*/
/*  ABS                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... ABS] -> [... ... ... ... AB1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ ABS(_NIL_TYPE_)
  { UNARY(AB1, abs_STR); }
   
/*-----------------------------------------------------------------*/
/*  AB1                                                            */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... AB1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ AB1(_NIL_TYPE_)
  { _EXP_TYPE_ frc, nbr, val;
    _FLO_TYPE_ flo;
    _SGN_TYPE_ sgn;
    _mem_claim_();
    _stk_peek_EXP_(val);
    _stk_zap__CNT_();
    if (_ag_is_NBR_(val))
      { sgn = _ag_get_NBR_(val);
        sgn = labs(sgn);
        nbr = _ag_make_NBR_(sgn);
        _stk_poke_EXP_(nbr);    
        return; }
    if (_ag_is_FRC__(val))
      { flo = _ag_get_FRC_(val);
        flo = fabs(flo);
        frc = _ag_make_FRC_(flo);
        _stk_poke_EXP_(frc);    
        return; }
    _error_str_(_ATC_ERROR_, abs_STR); }
    
/*-----------------------------------------------------------------*/
/*  CHR                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... CHR] -> [... ... ... ... CH1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ CHR(_NIL_TYPE_)
  { UNARY(CH1, chr_STR); }
 
/*-----------------------------------------------------------------*/
/*  CH1                                                            */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... CH1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ CH1(_NIL_TYPE_)
  { _EXP_TYPE_ chr, val;
    _SGN_TYPE_ sgn;
    unsigned char str[2] = "_"; //bug fix, was: _STR_TYPE_ str = "_";
    _mem_claim_STR_(str);
    _stk_peek_EXP_(val);
    if (_ag_is_NBR_(val))
      { sgn = _ag_get_NBR_(val);
        if ((sgn >= 0) && (sgn <= 255))
          { str[0] = (_CHA_TYPE_)sgn; //could segfault -- dvd
            chr = _CNT_make_NAM_(str);
            _stk_poke_EXP_(chr);    
            _stk_zap__CNT_();
            return; }
        _error_str_(_RNG_ERROR_, chr_STR); }
    _error_str_(_ATC_ERROR_, chr_STR); }
    
/*-----------------------------------------------------------------*/
/*  ORD                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... ORD] -> [... ... ... ... OR1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ ORD(_NIL_TYPE_)
  { UNARY(OR1, ord_STR); }
 
/*-----------------------------------------------------------------*/
/*  OR1                                                            */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... OR1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ OR1(_NIL_TYPE_)
  { _EXP_TYPE_ nbr, val;
    _STR_TYPE_ str;
    _CHA_TYPE_ chr;
    _stk_peek_EXP_(val);
    if (_ag_is_TXT_(val))
      { str = _ag_get_TXT_(val);
        if (strlen(str) == 1)
          { chr = str[0];
            nbr = _ag_make_NBR_(chr);
            _stk_poke_EXP_(nbr);     
            _stk_zap__CNT_();
            return; }
        _error_str_(_RNG_ERROR_, ord_STR); }
    _error_str_(_ATC_ERROR_, ord_STR); }
 
/*-----------------------------------------------------------------*/
/*  NUM                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... NUM] -> [... ... ... ... NU1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ NUM(_NIL_TYPE_)
  { UNARY(NU1, num_STR); }
 
/*-----------------------------------------------------------------*/
/*  NU1                                                            */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... NU1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ NU1(_NIL_TYPE_)
  { _EXP_TYPE_ val;
    _FLO_TYPE_ flo;
    _SGN_TYPE_ sgn;
    _STR_TYPE_ str, end;
    _mem_claim_();
    _stk_peek_EXP_(val);
    if (_ag_is_TXT_(val))
      { str = _ag_get_TXT_(val);
        sgn = strtol(str, &end, 10);
        if (strlen(end))
          { flo = strtod(str, &end);
            if (strlen(end))
              val = _VOI_;
            else
              val = _ag_make_FRC_(flo); }  
        else
          { if (labs(sgn) > _NBR_MAX_)
              _error_str_(_NBR_ERROR_, num_STR);
            val = _ag_make_NBR_(sgn); }    
        _stk_poke_EXP_(val); 
        _stk_zap__CNT_();
        return; }
    _error_str_(_ATC_ERROR_, num_STR); }
    
/*-----------------------------------------------------------------*/
/*  TEX                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... TEX] -> [... ... ... ... TE1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ TEX(_NIL_TYPE_)
  { UNARY(TE1, tex_STR); }
 
/*-----------------------------------------------------------------*/
/*  TE1                                                            */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... TE1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ TE1(_NIL_TYPE_)
  { _EXP_TYPE_ val;
    _SGN_TYPE_ nbr;
    _FLO_TYPE_ flo;
    _stk_peek_EXP_(val);
    _stk_zap__CNT_();
    if (_ag_is_NBR_(val)) 
      { nbr = _ag_get_NBR_(val);
        sprintf(_BUF_, "%i", nbr);
        _mem_claim_STR_(_BUF_);
        val = _CNT_make_NAM_(_BUF_); }
    else
      switch (_ag_get_TAG__(val))
        { case _FRC_TAG_: 
            flo = _ag_get_FRC_(val);
            sprintf(_BUF_, "%g", flo);
            _mem_claim_STR_(_BUF_);
            val = _CNT_make_NAM_(_BUF_);    
            break;
          case _TXT_TAG_: 
            break;
          default:
            val = _VOI_; }
    _stk_poke_EXP_(val); }
    
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*----------------U T I L I T Y     O P E R A T O R S--------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
 
/*-----------------------------------------------------------------*/
/*  RND                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... NBR] */
/*     CNT: [... ... ... ... ... RND] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ RND(_NIL_TYPE_)
  { _EXP_TYPE_ arg, nbr;
    _UNS_TYPE_ siz;
    _SGN_TYPE_ sgn;
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    if (siz == 0)
      { sgn = rand()*rand();
        nbr = _ag_make_NBR_(sgn);    
        _stk_poke_EXP_(nbr);
        _stk_zap__CNT_(); 
        return;}
    _error_str_(_NMA_ERROR_, rnd_STR); }
      
/*-----------------------------------------------------------------*/
/*  CLK                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... FRC] */
/*     CNT: [... ... ... ... ... CLK] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ CLK(_NIL_TYPE_)
  { _EXP_TYPE_ arg, frc;
    _UNS_TYPE_ siz;
    _FLO_TYPE_ flo;
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    if (siz == 0)
      { flo = (_FLO_TYPE_)clock() / CLOCKS_PER_SEC;
        frc = _ag_make_FRC_(flo);    
        _stk_poke_EXP_(frc);
        _stk_zap__CNT_(); 
        return;}
    _error_str_(_NMA_ERROR_, clk_STR); }
      
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*---------T R A N S C E N D E N T A L     O P E R A T O R S-------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
 
/*-----------------------------------------------------------------*/
/*  SQT                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... SQT] -> [... ... ... ... SQ1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ SQT(_NIL_TYPE_)
  { UNARY(SQ1, sqt_STR); }
 
/*-----------------------------------------------------------------*/
/*  SQ1                                                            */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... SQ1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ SQ1(_NIL_TYPE_)
  { _EXP_TYPE_ frc, val;
    _FLO_TYPE_ flo;
    _SGN_TYPE_ sgn;
    _mem_claim_();
    _stk_peek_EXP_(val);
    if (_ag_is_NBR_(val)) 
      { sgn = _ag_get_NBR_(val);
        if (sgn < 0)
          _error_str_(_NEG_ERROR_, sqt_STR);
        flo = sqrt(sgn);
        frc = _ag_make_FRC_(flo);
        _stk_poke_EXP_(frc);    
        _stk_zap__CNT_();
        return; }
    if (_ag_is_FRC__(val)) 
      { flo = _ag_get_FRC_(val);
        if (flo < 0)
          _error_str_(_NEG_ERROR_, sqt_STR);
        flo = sqrt(flo);
        frc = _ag_make_FRC_(flo);
        _stk_poke_EXP_(frc);    
        _stk_zap__CNT_();
        return; }
    _error_str_(_ATC_ERROR_, sqt_STR); }
    
/*-----------------------------------------------------------------*/
/*  SIN                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... SIN] -> [... ... ... ... SI1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ SIN(_NIL_TYPE_)
  { UNARY(SI1, sin_STR); }
 
/*-----------------------------------------------------------------*/
/*  SI1                                                            */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... SI1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ SI1(_NIL_TYPE_)
  { _EXP_TYPE_ frc, val;
    _SGN_TYPE_ sgn;
    _FLO_TYPE_ flo;
    _mem_claim_();
    _stk_peek_EXP_(val);
    if (_ag_is_NBR_(val)) 
      { sgn = _ag_get_NBR_(val);
        flo = sin(sgn);
        frc = _ag_make_FRC_(flo);
        _stk_poke_EXP_(frc);    
        _stk_zap__CNT_();
        return; }
    if (_ag_is_FRC__(val)) 
      { flo = _ag_get_FRC_(val);
        flo = sin(flo);
        frc = _ag_make_FRC_(flo);
        _stk_poke_EXP_(frc);    
        _stk_zap__CNT_();
        return; }
    _error_str_(_ATC_ERROR_, sin_STR); }
    
/*-----------------------------------------------------------------*/
/*  COS                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... COS] -> [... ... ... ... CO1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ COS(_NIL_TYPE_)
  { UNARY(CO1, cos_STR); }
 
/*-----------------------------------------------------------------*/
/*  CO1                                                            */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... CO1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ CO1(_NIL_TYPE_)
  { _EXP_TYPE_ frc, val;
    _SGN_TYPE_ sgn;
    _FLO_TYPE_ flo;
    _mem_claim_();
    _stk_peek_EXP_(val);
    if (_ag_is_NBR_(val)) 
      { sgn = _ag_get_NBR_(val);
        flo = cos(sgn);
        frc = _ag_make_FRC_(flo);
        _stk_poke_EXP_(frc);    
        _stk_zap__CNT_();
        return; }
    if (_ag_is_FRC__(val)) 
      { flo = _ag_get_FRC_(val);
        flo = cos(flo);
        frc = _ag_make_FRC_(flo);
        _stk_poke_EXP_(frc);    
        _stk_zap__CNT_();
        return; }
    _error_str_(_ATC_ERROR_, cos_STR); }
    
/*-----------------------------------------------------------------*/
/*  TAN                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... TAN] -> [... ... ... ... TA1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ TAN(_NIL_TYPE_)
  { UNARY(TA1, tan_STR); }
 
/*-----------------------------------------------------------------*/
/*  TA1                                                            */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... TA1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ TA1(_NIL_TYPE_)
  { _EXP_TYPE_ frc, val;
    _SGN_TYPE_ sgn;
    _FLO_TYPE_ flo;
    _stk_peek_EXP_(val);
    if (_ag_is_NBR_(val)) 
      { sgn = _ag_get_NBR_(val);
        flo = tan(sgn);
        if (fabs(flo) >= _FRC_MAX_)
          _error_str_(_INF_ERROR_, tan_STR);
        frc = _ag_make_FRC_(flo);
        _stk_poke_EXP_(frc);    
        _stk_zap__CNT_();
        return; }
    if (_ag_is_FRC__(val)) 
      { flo = _ag_get_FRC_(val);
        flo = tan(flo);
        if (fabs(flo) >= _FRC_MAX_)
          _error_str_(_INF_ERROR_, tan_STR);
        frc = _ag_make_FRC_(flo);
        _stk_poke_EXP_(frc);    
        _stk_zap__CNT_();
        return; }
    _error_str_(_ATC_ERROR_, tan_STR); }
  
/*-----------------------------------------------------------------*/
/*  ASN                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... ASN] -> [... ... ... ... AS1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ ASN(_NIL_TYPE_)
  { UNARY(AS1, asn_STR); }
 
/*-----------------------------------------------------------------*/
/*  AS1                                                            */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... AS1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ AS1(_NIL_TYPE_)
  { _EXP_TYPE_ frc, val;
    _FLO_TYPE_ flo;
    _SGN_TYPE_ sgn;
    _stk_peek_EXP_(val);
    if (_ag_is_NBR_(val)) 
      { sgn = _ag_get_NBR_(val);
        if ((sgn < -1) | (sgn > +1))
          _error_str_(_RNG_ERROR_, asn_STR);
        flo = asin(sgn);
        frc = _ag_make_FRC_(flo);
        _stk_poke_EXP_(frc);    
        _stk_zap__CNT_();
        return; }
    if (_ag_is_FRC__(val)) 
      { flo = _ag_get_FRC_(val);
        if ((flo < -1) | (flo > +1))
          _error_str_(_RNG_ERROR_, asn_STR);
        flo = asin(flo);
        frc = _ag_make_FRC_(flo);
        _stk_poke_EXP_(frc);    
        _stk_zap__CNT_();
        return; }
    _error_str_(_ATC_ERROR_, asn_STR); }
    
/*-----------------------------------------------------------------*/
/*  ACS                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... ACS] -> [... ... ... ... AC1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ ACS(_NIL_TYPE_)
  { UNARY(AC1, acs_STR); }
 
/*-----------------------------------------------------------------*/
/*  AC1                                                            */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... AC1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ AC1(_NIL_TYPE_)
  { _EXP_TYPE_ frc, val;
    _FLO_TYPE_ flo;
    _SGN_TYPE_ sgn;
    _stk_peek_EXP_(val);
    if (_ag_is_NBR_(val)) 
      { sgn = _ag_get_NBR_(val);
        if ((sgn < -1) | (sgn > +1))
          _error_str_(_RNG_ERROR_, acs_STR);
        flo = acos(sgn);
        frc = _ag_make_FRC_(flo);
        _stk_poke_EXP_(frc);    
        _stk_zap__CNT_();
        return; }
    if (_ag_is_FRC__(val)) 
      { flo = _ag_get_FRC_(val);
        if ((flo < -1) | (flo > +1))
          _error_str_(_RNG_ERROR_, acs_STR);
        flo = acos(flo);
        frc = _ag_make_FRC_(flo);
        _stk_poke_EXP_(frc);    
        _stk_zap__CNT_();
        return; }
    _error_str_(_ATC_ERROR_, acs_STR); }
    
/*-----------------------------------------------------------------*/
/*  ATN                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... ATN] -> [... ... ... ... AT1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ ATN(_NIL_TYPE_)
  { UNARY(AT1, atn_STR); }
 
/*-----------------------------------------------------------------*/
/*  AT1                                                            */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... AT1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ AT1(_NIL_TYPE_)
  { _EXP_TYPE_ frc, val;
    _FLO_TYPE_ flo;
    _SGN_TYPE_ sgn;
    _stk_peek_EXP_(val);
    if (_ag_is_NBR_(val)) 
      { sgn = _ag_get_NBR_(val);
        flo = atan(sgn);
        frc = _ag_make_FRC_(flo);
        _stk_poke_EXP_(frc);    
        _stk_zap__CNT_();
        return; }
    if (_ag_is_FRC__(val)) 
      { flo = _ag_get_FRC_(val);
        flo = atan(flo);
        frc = _ag_make_FRC_(flo);
        _stk_poke_EXP_(frc);    
        _stk_zap__CNT_();
        return; }
    _error_str_(_ATC_ERROR_, atn_STR); }
    
/*-----------------------------------------------------------------*/
/*  EPW                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... EPW] -> [... ... ... ... EP1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ EPW(_NIL_TYPE_)
  { UNARY(EP1, epw_STR); }
 
/*-----------------------------------------------------------------*/
/*  EP1                                                            */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... EP1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ EP1(_NIL_TYPE_)
  { _EXP_TYPE_ frc, val;
    _FLO_TYPE_ flo;
    _SGN_TYPE_ sgn;
    _stk_peek_EXP_(val);
    if (_ag_is_NBR_(val)) 
      { sgn = _ag_get_NBR_(val);
        flo = exp(sgn);
        if (fabs(flo) >= _FRC_MAX_)
          _error_str_(_INF_ERROR_, epw_STR);
        frc = _ag_make_FRC_(flo);
        _stk_poke_EXP_(frc);    
        _stk_zap__CNT_();
        return; }
    if (_ag_is_FRC__(val)) 
      { flo = _ag_get_FRC_(val);
        flo = exp(flo);
        if (fabs(flo) >= _FRC_MAX_)
          _error_str_(_INF_ERROR_, epw_STR);
        frc = _ag_make_FRC_(flo);
        _stk_poke_EXP_(frc);    
        _stk_zap__CNT_();
        return; }
    _error_str_(_ATC_ERROR_, epw_STR); }
    
/*-----------------------------------------------------------------*/
/*  LOG                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... LOG] -> [... ... ... ... LO1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ LOG(_NIL_TYPE_)
  { UNARY(LO1, log_STR); }
 
/*-----------------------------------------------------------------*/
/*  LO1                                                            */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... LO1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ LO1(_NIL_TYPE_)
  { _EXP_TYPE_ frc, val;
    _FLO_TYPE_ flo;
    _SGN_TYPE_ sgn;
    _stk_peek_EXP_(val);
    if (_ag_is_NBR_(val)) 
      { sgn = _ag_get_NBR_(val);
        if (sgn <= 0)
          _error_str_(_NEG_ERROR_, log_STR);
        flo = log(sgn);
        frc = _ag_make_FRC_(flo);
        _stk_poke_EXP_(frc);    
        _stk_zap__CNT_();
        return; }
    if (_ag_is_FRC__(val)) 
      { flo = _ag_get_FRC_(val);
        if (flo <= 0)
          _error_str_(_NEG_ERROR_, log_STR);
        flo = log(flo);
        frc = _ag_make_FRC_(flo);
        _stk_poke_EXP_(frc);    
        _stk_zap__CNT_();
        return; }
    _error_str_(_ATC_ERROR_, log_STR); }
    
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*------------------S T R I N G     O P E R A T O R S--------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
 
/*-----------------------------------------------------------------*/
/*  LEN                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... LEN] -> [... ... ... ... LE2 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ LEN(_NIL_TYPE_)
  { UNARY(LE2, len_STR); }
 
/*-----------------------------------------------------------------*/
/*  LE2                                                            */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... LE2] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ LE2(_NIL_TYPE_)
  { _EXP_TYPE_ nbr, val;
    _STR_TYPE_ str;
    _UNS_TYPE_ siz;
    _stk_peek_EXP_(val);
    _stk_zap__CNT_();
    if (_ag_is_TXT_(val))
      { str = _ag_get_TXT_(val);
        siz = strlen(str);
        nbr = _ag_make_NBR_(siz);
        _stk_poke_EXP_(nbr);    
        return; }   
    _error_str_(_ATC_ERROR_, len_STR); }

/*-----------------------------------------------------------------*/
/*  EPL                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... EPL] -> [... ... ... ... EP2 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ EPL(_NIL_TYPE_)
  { UNARY(EP2, epl_STR); }
  
/*-----------------------------------------------------------------*/
/*  EP2                                                            */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... *E*] */
/*     CNT: [... ... ... ... ... EP2] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... TAB] */
/*     CNT: [... ... ... ... ... EP2] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ EP2(_NIL_TYPE_)
  { _EXP_TYPE_ nbr, tab, val;
    _STR_TYPE_ str;
    _UNS_TYPE_ len, ord;
    _stk_peek_EXP_(val);     
    _stk_zap__CNT_();
    if (_ag_is_TXT_(val))
      { str = _ag_get_TXT_(val);
        if (strlen(str) > _BUF_SIZE_)
          _error_str_(_BUF_ERROR_, ipl_STR);
        strncpy(_BUF_, str, _BUF_SIZE_);
        len = strlen(_BUF_);
        if (len == 0)
          { _stk_poke_EXP_(_EMP_);
            return; }
        _mem_claim_SIZ_(len);
        tab = _ag_make_TAB_(len);
        _stk_poke_EXP_(tab);  
        while (len)
          { ord = _BUF_[len - 1];
            nbr = _ag_make_NBR_(ord);
            _ag_set_TAB_EXP_(tab, len--, nbr); }    
        return; }
    _error_str_(_ATC_ERROR_, epl_STR); }
 
/*-----------------------------------------------------------------*/
/*  IPL                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... IPL] -> [... ... ... ... IP1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ IPL(_NIL_TYPE_)
  { UNARY(IP1, ipl_STR); }
  
/*-----------------------------------------------------------------*/
/*  IP1                                                            */
/*     EXP: [... ... ... ... ... TAB] -> [... ... ... ... ... *N*] */
/*     CNT: [... ... ... ... ... IP1] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ... ... ... TAB] -> [... ... ... ... ... STR] */
/*     CNT: [... ... ... ... ... IP1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ IP1(_NIL_TYPE_)
  { _EXP_TYPE_ nbr, val;
    _UNS_TYPE_ len, ord;
    _stk_peek_EXP_(val);
    _stk_zap__CNT_();
    if (_ag_is_TAB_(val))
      { len = _ag_get_TAB_SIZ_(val);
        if (len == 0)
          { _stk_poke_EXP_(_NUL_);
            return; }
        if (len > _BUF_SIZE_)
          _error_str_(_BUF_ERROR_, ipl_STR);
        _BUF_[len] = 0;
        while (len)
          { nbr = _ag_get_TAB_EXP_(val, len--);
            if (!_ag_is_NBR_(nbr))
              _error_str_(_ATC_ERROR_, ipl_STR);
            ord = _ag_get_NBR_(nbr);
            if ((ord < 0) || (ord > 255))
              _error_str_(_ATC_ERROR_, ipl_STR);
            _BUF_[len] = ord; }
        _mem_claim_STR_(_BUF_);
        val = _CNT_make_NAM_(_BUF_);
        _stk_poke_EXP_(val);
        return; }
    _error_str_(_ATC_ERROR_, ipl_STR); }
 
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*----------------I D E N T I T Y    O P E R A T O R S-------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
 
/*-----------------------------------------------------------------*/
/*  IVD                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... IVD] -> [... ... ... ... IV1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ IVD(_NIL_TYPE_)
  { UNARY(IV1, ivd_STR); }
 
/*-----------------------------------------------------------------*/
/*  IV1                                                            */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... IV1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ IV1(_NIL_TYPE_)
  { _EXP_TYPE_ val;
    _stk_peek_EXP_(val);
    _stk_zap__CNT_();
    if (_ag_is_VOI_(val))
      { _stk_poke_EXP_(tru_VAR);
        return; }
    _stk_poke_EXP_(fls_VAR); }
 
/*-----------------------------------------------------------------*/
/*  INB                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... INB] -> [... ... ... ... IN1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ INB(_NIL_TYPE_)
  { UNARY(IN1, inb_STR); }
 
/*-----------------------------------------------------------------*/
/*  IN1                                                            */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... IN1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ IN1(_NIL_TYPE_)
  { _EXP_TYPE_ val;
    _stk_peek_EXP_(val);
    _stk_zap__CNT_();
    if (_ag_is_NBR_(val))
      { _stk_poke_EXP_(tru_VAR);
        return; }
    _stk_poke_EXP_(fls_VAR); }
    
/*-----------------------------------------------------------------*/
/*  IFR                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... IFR] -> [... ... ... ... IF3 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ IFR(_NIL_TYPE_)
  { UNARY(IF3, ifr_STR); }
 
/*-----------------------------------------------------------------*/
/*  IF3                                                            */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... IF3] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ IF3(_NIL_TYPE_)
  { _EXP_TYPE_ val;
    _stk_peek_EXP_(val);
    _stk_zap__CNT_();
    if (_ag_is_FRC_(val))
      { _stk_poke_EXP_(tru_VAR);
        return; }
    _stk_poke_EXP_(fls_VAR); }
    
/*-----------------------------------------------------------------*/
/*  ITX                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... ITX] -> [... ... ... ... IT1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ ITX(_NIL_TYPE_)
  { UNARY(IT1, itx_STR); }
 
/*-----------------------------------------------------------------*/
/*  IT1                                                            */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... IT1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ IT1(_NIL_TYPE_)
  { _EXP_TYPE_ val;
    _stk_peek_EXP_(val);
    _stk_zap__CNT_();
    if (_ag_is_TXT_(val))
      { _stk_poke_EXP_(tru_VAR);
        return; }
    _stk_poke_EXP_(fls_VAR); }
    
/*-----------------------------------------------------------------*/
/*  ITB                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... ITB] -> [... ... ... ... IT2 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ ITB(_NIL_TYPE_)
  { UNARY(IT2, itb_STR); }
 
/*-----------------------------------------------------------------*/
/*  IT2                                                            */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... IT2] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ IT2(_NIL_TYPE_)
  { _EXP_TYPE_ val;
    _stk_peek_EXP_(val);
    _stk_zap__CNT_();
    if (_ag_is_TAB_(val))
      { _stk_poke_EXP_(tru_VAR);
        return; }
    _stk_poke_EXP_(fls_VAR); }
 
/*-----------------------------------------------------------------*/
/*  IFU                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... IFU] -> [... ... ... ... IF4 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ IFU(_NIL_TYPE_)
  { UNARY(IF4, ifu_STR); }
 
/*-----------------------------------------------------------------*/
/*  IF4                                                            */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... IF4] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ IF4(_NIL_TYPE_)
  { _EXP_TYPE_ val;
    _stk_peek_EXP_(val);
    _stk_zap__CNT_();
    if (_ag_is_FNC_(val))
      { _stk_poke_EXP_(tru_VAR);
        return; }
    _stk_poke_EXP_(fls_VAR); }
    
/*-----------------------------------------------------------------*/
/*  IDC                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... IDC] -> [... ... ... ... ID3 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ IDC(_NIL_TYPE_)
  { UNARY(ID3, idc_STR); }
 
/*-----------------------------------------------------------------*/
/*  ID3                                                            */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... ID3] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ ID3(_NIL_TYPE_)
  { _EXP_TYPE_ val;
    _stk_peek_EXP_(val);
    _stk_zap__CNT_();
    if (_ag_is_DCT_(val))
      { _stk_poke_EXP_(tru_VAR);
        return; }
    _stk_poke_EXP_(fls_VAR); }
 
/*-----------------------------------------------------------------*/
/*  ICT                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... ICT] -> [... ... ... ... IC1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ ICT(_NIL_TYPE_)
  { UNARY(IC1, ict_STR); }
 
/*-----------------------------------------------------------------*/
/*  IC1                                                            */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... IC1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ IC1(_NIL_TYPE_)
  { _EXP_TYPE_ val;
    _stk_peek_EXP_(val);
    _stk_zap__CNT_();
    if (_ag_is_CNT_(val))
      { _stk_poke_EXP_(tru_VAR);
        return; }
    _stk_poke_EXP_(fls_VAR); }
 
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*--------------------T A B L E     O P E R A T O R S--------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
 
/*-----------------------------------------------------------------*/
/*  SIZ                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... SIZ] -> [... ... ... ... SI2 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ SIZ(_NIL_TYPE_)
  { UNARY(SI2, siz_STR); }
 
/*-----------------------------------------------------------------*/
/*  SI2                                                            */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... SI2] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ SI2(_NIL_TYPE_)
  { _EXP_TYPE_ nbr, val;
    _UNS_TYPE_ siz;
    _stk_peek_EXP_(val);
    if (_ag_is_TAB_(val))
      { siz = _ag_get_TAB_SIZ_(val);
        nbr = _ag_make_NBR_(siz); 
        _stk_poke_EXP_(nbr);    
        _stk_zap__CNT_();
        return; }
    _error_str_(_NAT_ERROR_, siz_STR); }
     
/*-----------------------------------------------------------------*/
/*  TAB                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ARG TAB NBR EXP] */
/*     CNT: [... ... ... ... ... TAB] -> [... ... ... ... TA2 EXP] */
/*                                                                 */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... TAB] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ TAB(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp, tab;
    _UNS_TYPE_ siz;
    _stk_claim_();
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    if (siz == 0)
      { _stk_poke_EXP_(_EMP_);
        _stk_zap__CNT_();
        return; }
    _mem_claim_SIZ_(siz);
    tab = _ag_make_TAB_(siz);    
    _stk_peek_EXP_(arg);
    _stk_push_EXP_(tab);
    _stk_push_EXP_(_ONE_);
    exp = _ag_get_TAB_EXP_(arg, 1);
    _stk_push_EXP_(exp);
    _stk_poke_CNT_(TA2); 
    _stk_push_CNT_(EXP); }
 
/*-----------------------------------------------------------------*/
/*  TA2                                                            */
/*     EXP: [... ... ARG TAB NBR VAL] -> [... ... ARG TAB NBR EXP] */
/*     CNT: [... ... ... ... ... TA2] -> [... ... ... ... TA2 EXP] */
/*                                                                 */
/*     EXP: [... ... ARG TAB NBR VAL] -> [... ... ... ... ... TAB] */
/*     CNT: [... ... ... ... ... TA2] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ TA2(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp, nbr, tab, val;
    _UNS_TYPE_ ctr, siz;
    _stk_claim_();
    _stk_pop__EXP_(val);
    _stk_pop__EXP_(nbr);
    _stk_pop__EXP_(tab);
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    ctr = _ag_get_NBU_(nbr);
    _ag_set_TAB_EXP_(tab, ctr, val);
    if (ctr < siz)
      { _stk_push_EXP_(tab);
        nbr = _ag_succ_NBR_(nbr);
        _stk_push_EXP_(nbr);
        exp = _ag_get_TAB_EXP_(arg, ctr+1);
        _stk_push_EXP_(exp);
        _stk_push_CNT_(EXP); 
        return; }
    _stk_poke_EXP_(tab);
    _stk_zap__CNT_(); }
 
/*-----------------------------------------------------------------*/
/*  PAI                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... EXP EXP] */
/*     CNT: [... ... ... ... ... PAI] -> [... ... ... PA1 SWP EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ PAI(_NIL_TYPE_)
  { BINARY(PA1, pai_STR); }
   
/*-----------------------------------------------------------------*/
/*  PA1                                                            */
/*     EXP: [... ... ... ... VAL VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... PA1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ PA1(_NIL_TYPE_)
  { _EXP_TYPE_ pai, val, vax;
    _mem_claim_();
    _stk_pop__EXP_(val);
    _stk_peek_EXP_(vax);
    pai = _ag_make_TAB_TWO_(vax, val);
    _stk_zap__CNT_();
    _stk_poke_EXP_(pai); }
 
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*------------I N T E R A C T I O N    O P E R A T O R S-----------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
 
/*-----------------------------------------------------------------*/
/*  DSP                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ARG *1* EXP] */
/*     CNT: [... ... ... ... ... DSP] -> [... ... ... ... DS1 EXP] */
/*                                                                 */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... *N*] */
/*     CNT: [... ... ... ... ... DSP] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ DSP(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp;
    _UNS_TYPE_ siz;
    _stk_claim_();
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    if (siz == 0)
      { _stk_poke_EXP_(_NUL_);
        _stk_zap__CNT_();
        return; }
    _stk_poke_EXP_(arg);
    _stk_push_EXP_(_ONE_);
    exp = _ag_get_TAB_EXP_(arg, 1);
    _stk_push_EXP_(exp);
    _stk_poke_CNT_(DS1); 
    _stk_push_CNT_(EXP); }
 
/*-----------------------------------------------------------------*/
/*  DS1                                                            */
/*     EXP: [... ... ... ARG NBR VAL] -> [... ... ARG NBR EXP VAL] */
/*     CNT: [... ... ... ... ... DS1] -> [... ... ... DS1 EXP DS2] */
/*                                                                 */
/*     EXP: [... ... ... ARG NBR VAL] -> [... ... ... ... *N* VAL] */
/*     CNT: [... ... ... ... ... DS1] -> [... ... ... ... ... DS2] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ DS1(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp, nbr, val;
    _UNS_TYPE_ ctr, siz;
    _stk_claim_();
    _stk_pop__EXP_(val);
    _stk_pop__EXP_(nbr);
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    ctr = _ag_get_NBU_(nbr);
    if (ctr < siz)
      { nbr = _ag_succ_NBR_(nbr);
        _stk_push_EXP_(nbr);
        exp = _ag_get_TAB_EXP_(arg, ctr+1);
        _stk_push_EXP_(exp); 
        _stk_push_EXP_(val); 
        _stk_push_CNT_(EXP);
        _stk_push_CNT_(DS2);
        return; }
    _stk_poke_EXP_(_NUL_);
    _stk_push_EXP_(val);
    _stk_poke_CNT_(DS2); }
        
/*-----------------------------------------------------------------*/
/*  DS2                                                            */
/*     EXP: [... ... ... ... ... EXP] -> [... ... ... ... ... ...] */
/*     CNT: [... ... ... ... ... DS2] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ... ... ... EXP] -> [... ... ... TAB NBR EXP] */
/*     CNT: [... ... ... ... ... DS2] -> [... ... ... ... DS3 DS2] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ DS2(_NIL_TYPE_)
  { _EXP_TYPE_ exp, inv, nam, ref;
    _SGN_TYPE_ sgn;
    _FLO_TYPE_ flo;
    _STR_TYPE_ str;
    _UNS_TYPE_ siz;
    _stk_claim_();
    _stk_peek_EXP_(exp);
    if(_ag_is_NBR_(exp))
      { sgn = _ag_get_NBR_(exp);
        sprintf(_BUF_, "%i", sgn);
        _stk_zap__EXP_();
        _stk_zap__CNT_();
        _display_(_BUF_);
        return; }
    switch (_ag_get_TAG__(exp))
      { case _APL_TAG_:
          ref = _ag_get_APL_EXP_(exp);
          if(_ag_is_REF_(ref))
            { nam = _ag_get_REF_NAM_(ref);
              str = _ag_get_TXT_(nam); }
          else
              str = "<expression>";
          sprintf(_BUF_, "<application %s>", str);
          break;
        case _CST_TAG_:
          sprintf(_BUF_, "<dictionary>");
          break;
        case _CNT_TAG_:
          sprintf(_BUF_, "<context>");
          break;
        case _DCL_TAG_:
          inv = _ag_get_DCL_INV_(exp);
          DISPLAY_NAME(inv, nam);
          str = _ag_get_TXT_(nam);
          sprintf(_BUF_, "<declaration %s>", str);
          break;
        case _DEF_TAG_:
          inv = _ag_get_DEF_INV_(exp);
          DISPLAY_NAME(inv, nam);
          str = _ag_get_TXT_(nam);
          sprintf(_BUF_, "<definition %s>", str);
          break;
        case _FRC_TAG_:
          flo = _ag_get_FRC_(exp);
          sprintf(_BUF_, "%g", flo);
          break;
        case _FUN_TAG_:
          nam = _ag_get_FUN_NAM_(exp);
          str = _ag_get_TXT_(nam);
           sprintf(_BUF_, "<function %s>", str);
          break;
        case _NAT_TAG_:
          nam = _ag_get_NAT_NAM_(exp);
          str = _ag_get_TXT_(nam);
          sprintf(_BUF_, "<native %s>", str);
          break;
        case _QUA_TAG_:
          nam = _ag_get_QUA_EXP_(exp);
          str = _ag_get_TXT_(nam);
          sprintf(_BUF_, "<qualification %s>", str);
          break;
        case _REF_TAG_:
          nam = _ag_get_REF_NAM_(exp);
          str = _ag_get_TXT_(nam);
          sprintf(_BUF_, "<reference %s>", str);
          break;
        case _SET_TAG_:
          inv = _ag_get_SET_INV_(exp);
          DISPLAY_NAME(inv, nam);
          str = _ag_get_TXT_(nam);
          sprintf(_BUF_, "<assignment %s>", str);
          break;
        case _TBL_TAG_:
          ref = _ag_get_TBL_EXP_(exp);
          if(_ag_is_REF_(ref))
            { nam = _ag_get_REF_NAM_(ref);
              str = _ag_get_TXT_(nam); }
          else
              str = "<expression>";
          sprintf(_BUF_, "<tabulation %s>", str);
          break;
        case _TAB_TAG_:
          siz = _ag_get_TAB_SIZ_(exp);
          if (siz > 0)
            { _stk_poke_EXP_(exp);
              exp = _ag_get_TAB_EXP_(exp, 1);
              _stk_push_EXP_(_ONE_);
              _stk_push_EXP_(exp);
              _stk_poke_CNT_(DS3);
              _stk_push_CNT_(DS2); 
              sprintf(_BUF_, "["); 
              _display_(_BUF_);             
              return; }
            sprintf(_BUF_, "[]"); 
          break;
        case _TXT_TAG_:
          str = _ag_get_TXT_(exp);
          sprintf(_BUF_, "%s", str);
          break;
        case _VAR_TAG_:
          sprintf(_BUF_, "<dictionary>");
          break;
        case _VOI_TAG_:
          sprintf(_BUF_, "<void>");
          break;
        default:
          _error_str_(_ATC_ERROR_, dsp_STR); }
    _stk_zap__EXP_();
    _stk_zap__CNT_();
    _display_(_BUF_); }
                                
/*-----------------------------------------------------------------*/
/*  DS3                                                            */
/*     EXP: [... ... ... ... TAB NBR] -> [... ... ... ... ... ...] */
/*     CNT: [... ... ... ... ... DS3] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ... ... TAB NBR] -> [... ... ... TAB NBR EXP] */
/*     CNT: [... ... ... ... ... DS3] -> [... ... ... ... DS3 DS2] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ DS3(_NIL_TYPE_)
  { _EXP_TYPE_ exp, nbr;
    _UNS_TYPE_ idx, siz;
    _stk_claim_();
    _stk_pop__EXP_(nbr);
    _stk_peek_EXP_(exp);
    idx = _ag_get_NBU_(nbr); 
    siz = _ag_get_TAB_SIZ_(exp);
    if (idx < siz)
      { sprintf(_BUF_, ", ");
        nbr = _ag_succ_NBR_(nbr);
        _stk_push_EXP_(nbr);
        exp = _ag_get_TAB_EXP_(exp, idx+1);
        _stk_push_EXP_(exp); 
        _stk_push_CNT_(DS2); }
    else
      { _stk_zap__EXP_();
        _stk_zap__CNT_();
        sprintf(_BUF_, "]"); }
    _display_(_BUF_); }
       
/*-----------------------------------------------------------------*/
/*  ACP                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... ...] */
/*     CNT: [... ... ... ... ... ACP] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ ACP(_NIL_TYPE_)
  { _EXP_TYPE_ arg;
    _UNS_TYPE_ siz;
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    if (siz != 0)
      _error_str_(_NMA_ERROR_, acp_STR);
    _stk_zap__EXP_();
    _stk_zap__CNT_();
    _accept_(); } 
 
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*------------------B O O L E A N    O P E R A T O R S-------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
 
/*-----------------------------------------------------------------*/
/*  TRR                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... DCT EXP] */
/*     CNT: [... ... ... ... ... TRR] -> [... ... ... ... RST EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ TRR(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp;
    _UNS_TYPE_ siz;
    _stk_claim_();
    _stk_peek_EXP_(arg);
    _stk_poke_EXP_(_DCT_);
    siz = _ag_get_TAB_SIZ_(arg);
    if (siz != 2)
      _error_str_(_NMA_ERROR_, trr_STR); 
    exp = _ag_get_TAB_EXP_(arg, 1);
    _stk_push_EXP_(exp);
    _stk_poke_CNT_(RST); 
    _stk_push_CNT_(EXP); }
    
/*-----------------------------------------------------------------*/
/*  FLS                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... DCT EXP] */
/*     CNT: [... ... ... ... ... FLS] -> [... ... ... ... RST EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ FLS(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp;
    _UNS_TYPE_ siz;
    _stk_claim_();
    _stk_peek_EXP_(arg);
    _stk_poke_EXP_(_DCT_);
    siz = _ag_get_TAB_SIZ_(arg);
    if (siz != 2)
      _error_str_(_NMA_ERROR_, fls_STR); 
    exp = _ag_get_TAB_EXP_(arg, 2);
    _stk_push_EXP_(exp);
    _stk_poke_CNT_(RST); 
    _stk_push_CNT_(EXP); }
    
/*-----------------------------------------------------------------*/
/*  AND                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ARG EXP] */
/*     CNT: [... ... ... ... ... AND] -> [... ... ... ... AN1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ AND(_NIL_TYPE_)
  { N_ARY(AN1, 2, and_STR); }

/*-----------------------------------------------------------------*/
/*  AN1                                                            */
/*     EXP: [... ... ... ... ARG *T*] -> [... ... ARG *2* DCT EXP] */
/*     CNT: [... ... ... ... ... AN1] -> [... ... ... ... AN2 EXP] */
/*                                                                 */
/*     EXP: [... ... ... ... ARG *F*] -> [... ... ... ... ... *F*] */
/*     CNT: [... ... ... ... ... AN1] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ... ... ARG FUN] -> [... ... ... ARG FUN TAB] */
/*     CNT: [... ... ... ... ... AN1] -> [... ... ... ... AN1 CAL] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ AN1(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp, fun;
    _stk_claim_();
    _stk_pop__EXP_(fun);
    if (_mem_is_same_(fun, tru_VAR))
      { _stk_peek_EXP_(arg);
        exp = _ag_get_TAB_EXP_(arg, 2);
        _stk_push_EXP_(_TWO_);
        _stk_push_EXP_(_DCT_);
        _stk_push_EXP_(exp);
        _stk_poke_CNT_(AN2);
        _stk_push_CNT_(EXP);
        return; }
    if (_mem_is_same_(fun, fls_VAR))
      { _stk_poke_EXP_(fls_VAR);
        _stk_zap__CNT_(); 
        return; }
    if (_ag_is_FUN_(fun))
      { _stk_push_EXP_(fun);
        _stk_push_EXP_(bln_VAR);
        _stk_push_CNT_(CAL);
        return; }
    _error_str_(_NAB_ERROR_, and_STR); }
 
/*-----------------------------------------------------------------*/
/*  AN2                                                            */
/*     EXP: [... ... ARG NBR DCT *T*] -> [... ... ARG NBR DCT EXP] */
/*     CNT: [... ... ... ... ... AN2] -> [... ... ... ... AN2 EXP] */
/*                                                                 */
/*     EXP: [... ... ARG NBR DCT *T*] -> [... ... ... ... ... *T*] */
/*     CNT: [... ... ... ... ... AN2] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ARG NBR DCT *F*] -> [... ... ... ... ... *F*] */
/*     CNT: [... ... ... ... ... AN2] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ARG NBR DCT FUN] -> [... ARG NBR DCT FUN TAB] */
/*     CNT: [... ... ... ... ... AN2] -> [... ... ... ... AN2 CAL] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ AN2(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp, fun, nbr;
    _UNS_TYPE_ ctr, siz;
    _stk_claim_();
    _stk_pop__EXP_(fun);
    _stk_pop__EXP_(_DCT_);
    _stk_pop__EXP_(nbr);
    if (_mem_is_same_(fun, tru_VAR))
      { ctr = _ag_get_NBR_(nbr);
        _stk_peek_EXP_(arg);
        siz = _ag_get_TAB_SIZ_(arg);
        if (ctr < siz)
          { nbr = _ag_succ_NBR_(nbr);
            exp = _ag_get_TAB_EXP_(arg, ctr+1);
            _stk_push_EXP_(nbr);
            _stk_push_EXP_(_DCT_);
            _stk_push_EXP_(exp);
            _stk_push_CNT_(EXP);
             return; }
        _stk_poke_EXP_(tru_VAR);
        _stk_zap__CNT_(); 
        return; }
    if (_mem_is_same_(fun, fls_VAR))
      { _stk_poke_EXP_(fls_VAR);
        _stk_zap__CNT_(); 
        return; }
    if (_ag_is_FUN_(fun))
      { _stk_push_EXP_(nbr);
        _stk_push_EXP_(_DCT_);
        _stk_push_EXP_(fun);
        _stk_push_EXP_(bln_VAR);
        _stk_push_CNT_(CAL);
        return; }
    _error_str_(_NAB_ERROR_, and_STR); }
 
/*-----------------------------------------------------------------*/
/*  ORR                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ARG EXP] */
/*     CNT: [... ... ... ... ... ORR] -> [... ... ... ... OR2 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ ORR(_NIL_TYPE_)
  { N_ARY(OR2, 2, orr_STR); }
    
/*-----------------------------------------------------------------*/
/*  OR2                                                            */
/*     EXP: [... ... ... ... ARG *T*] -> [... ... ... ... ... *T*] */
/*     CNT: [... ... ... ... ... OR2] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ... ... ARG *F*] -> [... ... ARG *2* DCT EXP] */
/*     CNT: [... ... ... ... ... OR2] -> [... ... ... ... OR3 EXP] */
/*                                                                 */
/*     EXP: [... ... ... ... ARG FUN] -> [... ... ... ARG FUN TAB] */
/*     CNT: [... ... ... ... ... OR2] -> [... ... ... ... OR2 CAL] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ OR2(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp, fun;
    _stk_claim_();
    _stk_pop__EXP_(fun);
    if (_mem_is_same_(fun, tru_VAR))
      { _stk_poke_EXP_(tru_VAR);
        _stk_zap__CNT_(); 
        return; }
    if (_mem_is_same_(fun, fls_VAR))
      { _stk_peek_EXP_(arg);
        exp = _ag_get_TAB_EXP_(arg, 2);
        _stk_push_EXP_(_TWO_);
        _stk_push_EXP_(_DCT_);
        _stk_push_EXP_(exp);
        _stk_poke_CNT_(OR3);
        _stk_push_CNT_(EXP);
        return; }
    if (_ag_is_FUN_(fun))
      { _stk_push_EXP_(fun);
        _stk_push_EXP_(bln_VAR);
        _stk_push_CNT_(CAL);
        return; }
    _error_str_(_NAB_ERROR_, orr_STR); }
 
/*-----------------------------------------------------------------*/
/*  OR3                                                            */
/*     EXP: [... ... ARG NBR DCT *T*] -> [... ... ... ... ... *T*] */
/*     CNT: [... ... ... ... ... OR3] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ARG NBR DCT *F*] -> [... ... ARG NBR DCT EXP] */
/*     CNT: [... ... ... ... ... OR3] -> [... ... ... ... OR3 EXP] */
/*                                                                 */
/*     EXP: [... ... ARG NBR DCT *F*] -> [... ... ... ... ... *F*] */
/*     CNT: [... ... ... ... ... OR3] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ARG NBR DCT FUN] -> [... ARG NBR DCT FUN TAB] */
/*     CNT: [... ... ... ... ... OR3] -> [... ... ... ... OR3 CAL] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ OR3(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp, fun, nbr;
    _UNS_TYPE_ ctr, siz;
    _stk_claim_();
    _stk_pop__EXP_(fun);
    _stk_pop__EXP_(_DCT_);
    _stk_pop__EXP_(nbr);
    if (_mem_is_same_(fun, tru_VAR))
      { _stk_poke_EXP_(tru_VAR);
        _stk_zap__CNT_(); 
        return; }
    if (_mem_is_same_(fun, fls_VAR))
      { ctr = _ag_get_NBR_(nbr);
        _stk_peek_EXP_(arg);
        siz = _ag_get_TAB_SIZ_(arg);
        if (ctr < siz)
          { nbr = _ag_succ_NBR_(nbr);
            exp = _ag_get_TAB_EXP_(arg, ctr+1);
            _stk_push_EXP_(nbr);
            _stk_push_EXP_(_DCT_);
            _stk_push_EXP_(exp);
            _stk_push_CNT_(EXP);
            return; }
        _stk_poke_EXP_(fls_VAR);
        _stk_zap__CNT_();
        return; }
    if (_ag_is_FUN_(fun))
      { _stk_push_EXP_(nbr);
        _stk_push_EXP_(_DCT_);
        _stk_push_EXP_(fun);
        _stk_push_EXP_(bln_VAR);
        _stk_push_CNT_(CAL);
        return; }
    _error_str_(_NAB_ERROR_, orr_STR); }
    
/*-----------------------------------------------------------------*/
/*  NOT                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... NOT] -> [... ... ... ... NO1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ NOT(_NIL_TYPE_)
  { UNARY(NO1, not_STR); }
 
/*-----------------------------------------------------------------*/
/*  NO1                                                            */
/*     EXP: [... ... ... ... ... NAT] -> [... ... ... ... ... *T*] */
/*     CNT: [... ... ... ... ... NO1] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ... ... ... NAT] -> [... ... ... ... ... *F*] */
/*     CNT: [... ... ... ... ... NO1] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ... ... ... FUN] -> [... ... ... ... FUN TAB] */
/*     CNT: [... ... ... ... ... NO1] -> [... ... ... ... NO1 CAL] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ NO1(_NIL_TYPE_)
  { _EXP_TYPE_ fun;
    _stk_claim_();
    _stk_peek_EXP_(fun);
    if (_mem_is_same_(fun, tru_VAR))
      { _stk_poke_EXP_(fls_VAR);
        _stk_zap__CNT_(); 
        return; }
    if (_mem_is_same_(fun, fls_VAR))
      { _stk_poke_EXP_(tru_VAR);
        _stk_zap__CNT_(); 
        return; }
    if (_ag_is_FUN_(fun))
      { _stk_push_EXP_(bln_VAR);
        _stk_push_CNT_(CAL);
        return; }
    _error_str_(_NAB_ERROR_, not_STR); }
    
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*----------------C O N T R O L    O P E R A T O R S---------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
 
/*-----------------------------------------------------------------*/
/*  BEG                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ARG *1* EXP] */
/*     CNT: [... ... ... ... ... BEG] -> [... ... ... ... BE1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ BEG(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp;
    _UNS_TYPE_ siz;
    _stk_claim_();
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    if (siz == 0)
      _error_str_(_NMA_ERROR_, beg_STR);
    _stk_poke_EXP_(arg);
    _stk_push_EXP_(_ONE_);
    exp = _ag_get_TAB_EXP_(arg, 1);
    _stk_push_EXP_(exp);
    _stk_poke_CNT_(BE1); 
    _stk_push_CNT_(EXP); }
 
/*-----------------------------------------------------------------*/
/*  BE1                                                            */
/*     EXP: [... ... ... ARG NBR VAL] -> [... ... ... ARG NBR EXP] */
/*     CNT: [... ... ... ... ... BE1] -> [... ... ... ... BE1 EXP] */
/*                                                                 */
/*     EXP: [... ... ... ARG NBR VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... BE1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ BE1(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp, nbr, val;
    _UNS_TYPE_ ctr, siz;
    _stk_claim_();
    _stk_pop__EXP_(val);
    _stk_pop__EXP_(nbr);
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    ctr = _ag_get_NBU_(nbr);
    if (ctr < siz)
      { nbr = _ag_succ_NBR_(nbr);
        _stk_push_EXP_(nbr);
        exp = _ag_get_TAB_EXP_(arg, ctr+1);
        _stk_push_EXP_(exp);
        _stk_push_CNT_(EXP); 
        return; }
    _stk_poke_EXP_(val);
    _stk_zap__CNT_(); }
        
/*-----------------------------------------------------------------*/
/*  IFF                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... CSQ PRD] */
/*     CNT: [... ... ... ... ... IFF] -> [... ... ... ... IF1 EXP] */
/*                                                                 */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ALT CSQ PRD] */
/*     CNT: [... ... ... ... ... IFF] -> [... ... ... ... IF2 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ IFF(_NIL_TYPE_)
  { _EXP_TYPE_ alt, arg, csq, prd;
    _UNS_TYPE_ siz;
    _stk_claim_();
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    if (siz == 2)
      { prd = _ag_get_TAB_EXP_(arg, 1);
        csq = _ag_get_TAB_EXP_(arg, 2);
        _stk_poke_EXP_(csq);
        _stk_push_EXP_(prd);
        _stk_poke_CNT_(IF1); 
        _stk_push_CNT_(EXP); 
        return; }
    if (siz == 3)
      { prd = _ag_get_TAB_EXP_(arg, 1);
        csq = _ag_get_TAB_EXP_(arg, 2);
        alt = _ag_get_TAB_EXP_(arg, 3);
        _stk_poke_EXP_(alt);
        _stk_push_EXP_(csq);
        _stk_push_EXP_(prd);
        _stk_poke_CNT_(IF2); 
        _stk_push_CNT_(EXP); 
        return; }
   _error_str_(_NMA_ERROR_, iff_STR); }
   
/*-----------------------------------------------------------------*/
/*  IF1                                                            */
/*     EXP: [... ... ... ... CSQ NAT] -> [... ... ... ... DCT CSQ] */
/*     CNT: [... ... ... ... ... IF1] -> [... ... ... ... RST EXP] */
/*                                                                 */
/*     EXP: [... ... ... ... CSQ NAT] -> [... ... ... ... ... _V_] */
/*     CNT: [... ... ... ... ... IF1] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ... ... CSQ FUN] -> [... ... ... CSQ FUN TAB] */
/*     CNT: [... ... ... ... ... IF1] -> [... ... ... ... IF1 CAL] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ IF1(_NIL_TYPE_)
  { _EXP_TYPE_ csq, fun;
    _stk_claim_();
    _stk_pop__EXP_(fun);
    _stk_peek_EXP_(csq);
    if (_mem_is_same_(fun, tru_VAR))
      { _stk_poke_EXP_(_DCT_);
        _stk_push_EXP_(csq);
        _stk_poke_CNT_(RST);
        _stk_push_CNT_(EXP);
        return; }
    if (_mem_is_same_(fun, fls_VAR))
      { _stk_poke_EXP_(_VOI_);
        _stk_zap__CNT_();
        return; }
    _stk_push_EXP_(fun);
    if (_ag_is_FUN_(fun))
      { _stk_push_EXP_(bln_VAR);
        _stk_push_CNT_(CAL);
        return; }
    _error_str_(_NAB_ERROR_, iff_STR); }
 
/*-----------------------------------------------------------------*/
/*  IF2                                                            */
/*     EXP: [... ... ... ALT CSQ NAT] -> [... ... ... ... DCT CSQ] */
/*     CNT: [... ... ... ... ... IF2] -> [... ... ... ... RST EXP] */
/*                                                                 */
/*     EXP: [... ... ... ALT CSQ NAT] -> [... ... ... ... DCT ALT] */
/*     CNT: [... ... ... ... ... IF2] -> [... ... ... ... RST EXP] */
/*                                                                 */
/*     EXP: [... ... ... ALT CSQ FUN] -> [... ... ALT CSQ FUN TAB] */
/*     CNT: [... ... ... ... ... IF2] -> [... ... ... ... IF2 CAL] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ IF2(_NIL_TYPE_)
  { _EXP_TYPE_ alt, csq, fun;
    _stk_claim_();
    _stk_pop__EXP_(fun);
    _stk_pop__EXP_(csq);
    _stk_peek_EXP_(alt);
    if (_mem_is_same_(fun, tru_VAR))
      { _stk_poke_EXP_(_DCT_);
        _stk_push_EXP_(csq);
        _stk_poke_CNT_(RST);
        _stk_push_CNT_(EXP);
        return; }
    if (_mem_is_same_(fun, fls_VAR))
      { _stk_poke_EXP_(_DCT_);
        _stk_push_EXP_(alt);
        _stk_poke_CNT_(RST);
        _stk_push_CNT_(EXP);
        return; }
    _stk_push_EXP_(csq);
    _stk_push_EXP_(fun);
    if (_ag_is_FUN_(fun))
      { _stk_push_EXP_(bln_VAR);
        _stk_push_CNT_(CAL);
        return; }
    _error_str_(_NAB_ERROR_, iff_STR); }
 
/*-----------------------------------------------------------------*/
/*  WHI                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... PRD BOD DCT *V* PRD] */
/*     CNT: [... ... ... ... ... WHI] -> [... ... ... ... WH1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ WHI(_NIL_TYPE_)
  { _EXP_TYPE_ arg, bod, prd;
    _UNS_TYPE_ siz;
    _stk_claim_();
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    if (siz != 2)
      _error_str_(_NMA_ERROR_, whi_STR); 
    prd = _ag_get_TAB_EXP_(arg, 1);
    bod = _ag_get_TAB_EXP_(arg, 2);
    _stk_poke_EXP_(prd);
    _stk_push_EXP_(bod);
    _stk_push_EXP_(_DCT_);
    _stk_push_EXP_(_VOI_);
    _stk_push_EXP_(prd);
    _stk_poke_CNT_(WH1); 
    _stk_push_CNT_(EXP); }
 
/*-----------------------------------------------------------------*/
/*  WH1                                                            */
/*     EXP: [... PRD BOD DCT VAL NAT] -> [... ... PRD BOD DCT BOD] */
/*     CNT: [... ... ... ... ... WH1] -> [... ... ... ... WH2 EXP] */
/*                                                                 */
/*     EXP: [... PRD BOD DCT VAL NAT] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... WH1] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... PRD BOD DCT VAL FUN] -> [PRD BOD DCT VAL FUN TAB] */
/*     CNT: [... ... ... ... ... WH1] -> [... ... ... ... WH1 CAL] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ WH1(_NIL_TYPE_)
  { _EXP_TYPE_ bod, fun, val;
    _stk_claim_();
    _stk_pop__EXP_(fun);
    if (_mem_is_same_(fun, tru_VAR))
      { _stk_zap__EXP_();
        _stk_pop__EXP_(_DCT_);
        _stk_peek_EXP_(bod);
        _stk_push_EXP_(_DCT_);
        _stk_push_EXP_(bod);
        _stk_poke_CNT_(WH2); 
        _stk_push_CNT_(EXP);
        return; }
    if (_mem_is_same_(fun, fls_VAR))
      { _stk_pop__EXP_(val);
        _stk_pop__EXP_(_DCT_);
        _stk_zap__EXP_();
        _stk_poke_EXP_(val);
        _stk_zap__CNT_(); 
        return; }
    _stk_push_EXP_(fun);
    if (_ag_is_FUN_(fun))
      { _stk_push_EXP_(bln_VAR);
        _stk_push_CNT_(CAL);
        return; }
    _error_str_(_NAB_ERROR_, whi_STR); }
   
/*-----------------------------------------------------------------*/
/*  WH2                                                            */
/*     EXP: [... ... PRD BOD DCT VAL] -> [... PRD BOD DCT VAL PRD] */
/*     CNT: [... ... ... ... ... WH2] -> [... ... ... ... WH1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ WH2(_NIL_TYPE_)
  { _EXP_TYPE_ bod, prd, val;
    _stk_claim_();
    _stk_pop__EXP_(val);
    _stk_pop__EXP_(_DCT_);
    _stk_pop__EXP_(bod);
    _stk_peek_EXP_(prd);
    _stk_push_EXP_(bod);
    _stk_push_EXP_(_DCT_);
    _stk_push_EXP_(val);
    _stk_push_EXP_(prd);
    _stk_poke_CNT_(WH1); 
    _stk_push_CNT_(EXP); 
    _ESCAPE_; }
 
/*-----------------------------------------------------------------*/
/*  UNT                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... PRD BOD DCT BOD] */
/*     CNT: [... ... ... ... ... UNT] -> [... ... ... ... UN1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ UNT(_NIL_TYPE_)
  { _EXP_TYPE_ arg, bod, prd;
    _UNS_TYPE_ siz;
    _stk_claim_();
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    if (siz != 2)
      _error_str_(_NMA_ERROR_, unt_STR); 
    prd = _ag_get_TAB_EXP_(arg, 1);
    bod = _ag_get_TAB_EXP_(arg, 2);
    _stk_poke_EXP_(prd);
    _stk_push_EXP_(bod);
    _stk_push_EXP_(_DCT_);
    _stk_push_EXP_(bod);
    _stk_poke_CNT_(UN1); 
    _stk_push_CNT_(EXP); }

/*-----------------------------------------------------------------*/
/*  UN1                                                            */
/*     EXP: [... ... PRD BOD DCT VAL] -> [... PRD BOD DCT VAL PRD] */
/*     CNT: [... ... ... ... ... UN1] -> [... ... ... ... UN2 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ UN1(_NIL_TYPE_)
  { _EXP_TYPE_  bod, prd, val;
    _stk_claim_();
    _stk_pop__EXP_(val);
    _stk_pop__EXP_(_DCT_);
    _stk_pop__EXP_(bod);
    _stk_peek_EXP_(prd);
    _stk_push_EXP_(bod);
    _stk_push_EXP_(_DCT_);
    _stk_push_EXP_(val);
    _stk_push_EXP_(prd);
    _stk_poke_CNT_(UN2); 
    _stk_push_CNT_(EXP); 
    _ESCAPE_; }
 
/*-----------------------------------------------------------------*/
/*  UN2                                                            */
/*     EXP: [... PRD BOD DCT VAL NAT] -> [... ... PRD BOD DCT BOD] */
/*     CNT: [... ... ... ... ... UN2] -> [... ... ... ... UN1 EXP] */
/*                                                                 */
/*     EXP: [... PRD BOD DCT VAL NAT] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... UN2] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... PRD BOD DCT VAL FUN] -> [PRD BOD DCT VAL FUN TAB] */
/*     CNT: [... ... ... ... ... UN2] -> [... ... ... ... UN2 CAL] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ UN2(_NIL_TYPE_)
  { _EXP_TYPE_ bod, fun, val;
    _stk_claim_();
    _stk_pop__EXP_(fun);
    if (_mem_is_same_(fun, fls_VAR))
      { _stk_zap__EXP_();
        _stk_pop__EXP_(_DCT_);
        _stk_peek_EXP_(bod);
        _stk_push_EXP_(_DCT_);
        _stk_push_EXP_(bod);
        _stk_poke_CNT_(UN1); 
        _stk_push_CNT_(EXP);
        return; }
    if (_mem_is_same_(fun, tru_VAR))
      { _stk_pop__EXP_(val);
        _stk_pop__EXP_(_DCT_);
        _stk_zap__EXP_();
        _stk_poke_EXP_(val);
        _stk_zap__CNT_(); 
        return; }
    _stk_push_EXP_(fun);
    if (_ag_is_FUN_(fun))
      { _stk_push_EXP_(bln_VAR);
        _stk_push_CNT_(CAL);
        return; }
    _error_str_(_NAB_ERROR_, unt_STR); }
   
/*-----------------------------------------------------------------*/
/*  FOR                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... PRD INC BOD INI] */
/*     CNT: [... ... ... ... ... FOR] -> [... ... ... ... FO1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ FOR(_NIL_TYPE_)
  { _EXP_TYPE_ arg, bod, inc, ini, prd;
    _UNS_TYPE_ siz;
    _stk_claim_();
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    if (siz != 4)
      _error_str_(_NMA_ERROR_, for_STR); 
    ini = _ag_get_TAB_EXP_(arg, 1);
    prd = _ag_get_TAB_EXP_(arg, 2);
    inc = _ag_get_TAB_EXP_(arg, 3);
    bod = _ag_get_TAB_EXP_(arg, 4);
    _stk_poke_EXP_(prd);
    _stk_push_EXP_(inc);
    _stk_push_EXP_(bod);
    _stk_push_EXP_(ini);
    _stk_poke_CNT_(FO1); 
    _stk_push_CNT_(EXP); }
 
/*-----------------------------------------------------------------*/
/*  FO1                                                            */
/*     EXP: [... ... PRD INC BOD VAL] -> [PRD INC BOD DCT *V* PRD] */
/*     CNT: [... ... ... ... ... FO1] -> [... ... ... ... FO2 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ FO1(_NIL_TYPE_)
  { _EXP_TYPE_ bod, inc, prd;
    _stk_claim_();
    _stk_zap__EXP_();
    _stk_pop__EXP_(bod);
    _stk_pop__EXP_(inc);
    _stk_peek_EXP_(prd);
    _stk_push_EXP_(inc);
    _stk_push_EXP_(bod);
    _stk_push_EXP_(_DCT_);
    _stk_push_EXP_(_VOI_);
    _stk_push_EXP_(prd);
    _stk_poke_CNT_(FO2); 
    _stk_push_CNT_(EXP); }
    
/*-----------------------------------------------------------------*/
/*  FO2                                                            */
/*     EXP: [PRD INC BOD DCT VAL NAT] -> [... PRD INC BOD DCT BOD] */
/*     CNT: [... ... ... ... ... FO2] -> [... ... ... ... FO3 EXP] */
/*                                                                 */
/*     EXP: [PRD INC BOD DCT VAL NAT] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... FO2] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [PRD INC BOD DCT VAL FUN] -> [PRD INC BOD DCT          */
/*                                                    VAL FUN TAB] */
/*     CNT: [... ... ... ... ... FO2] -> [... ... ... ... FO2 CAL] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ FO2(_NIL_TYPE_)
  { _EXP_TYPE_ bod, fun, val;
    _stk_claim_();
    _stk_pop__EXP_(fun);
    if (_mem_is_same_(fun, tru_VAR))
      { _stk_zap__EXP_();
        _stk_pop__EXP_(_DCT_);
        _stk_peek_EXP_(bod);
        _stk_push_EXP_(_DCT_);
        _stk_push_EXP_(bod);
        _stk_poke_CNT_(FO3); 
        _stk_push_CNT_(EXP);
        return; }
    if (_mem_is_same_(fun, fls_VAR))
      { _stk_pop__EXP_(val);
        _stk_pop__EXP_(_DCT_);
        _stk_zap__EXP_();
        _stk_zap__EXP_();
        _stk_poke_EXP_(val);
        _stk_zap__CNT_(); 
        return; }
    _stk_push_EXP_(fun);
    if (_ag_is_FUN_(fun))
      { _stk_push_EXP_(bln_VAR);
        _stk_push_CNT_(CAL);
        return; }
    _error_str_(_NAB_ERROR_, for_STR); }
   
/*-----------------------------------------------------------------*/
/*  FO3                                                            */
/*     EXP: [... PRD INC BOD DCT VAL] -> [PRD INC BOD DCT VAL INC] */
/*     CNT: [... ... ... ... ... FO3] -> [... ... ... ... FO4 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ FO3(_NIL_TYPE_)
  { _EXP_TYPE_ bod, inc, val;
    _stk_claim_();
    _stk_pop__EXP_(val);
    _stk_pop__EXP_(_DCT_);
    _stk_pop__EXP_(bod);
    _stk_peek_EXP_(inc);
    _stk_push_EXP_(bod);
    _stk_push_EXP_(_DCT_);
    _stk_push_EXP_(val);
    _stk_push_EXP_(inc);
    _stk_poke_CNT_(FO4); 
    _stk_push_CNT_(EXP); 
    _ESCAPE_; }
    
/*-----------------------------------------------------------------*/
/*  FO4                                                            */
/*     EXP: [PRD INC BOD DCT VAL VAL] -> [PRD INC BOD DCT VAL PRD] */
/*     CNT: [... ... ... ... ... FO4] -> [... ... ... ... FO2 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ FO4(_NIL_TYPE_)
  { _EXP_TYPE_ bod, inc, prd, val;
    _stk_claim_();
    _stk_zap__EXP_();
    _stk_pop__EXP_(val);
    _stk_pop__EXP_(_DCT_);
    _stk_pop__EXP_(bod);
    _stk_pop__EXP_(inc);
    _stk_peek_EXP_(prd);
    _stk_push_EXP_(inc);
    _stk_push_EXP_(bod);
    _stk_push_EXP_(_DCT_);
    _stk_push_EXP_(val);
    _stk_push_EXP_(prd);
    _stk_poke_CNT_(FO2); 
    _stk_push_CNT_(EXP); }
 
/*-----------------------------------------------------------------*/
/*  CAS                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [-V- ARG +++ --- *1* EXP] */
/*     CNT: [... ... ... ... ... CAS] -> [... ... ... ... CA1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ CAS(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp;
    _UNS_TYPE_ siz;
    _stk_claim_();
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    if (siz == 0)
      _error_str_(_CCA_ERROR_, cas_STR); 
    _stk_poke_EXP_(_VOI_);
    _stk_push_EXP_(arg);
    _stk_push_EXP_(MAX);
    _stk_push_EXP_(MIN);
    _stk_push_EXP_(_ONE_);
    exp = _ag_get_TAB_EXP_(arg, 1);
    _stk_push_EXP_(exp);
    _stk_poke_CNT_(CA1); 
    _stk_push_CNT_(EXP); }

/*-----------------------------------------------------------------*/
/*  CA1                                                            */
/*     EXP: [DFV ARG MIN MAX NBR VAL] -> [VAL DFV ARG              */
/*                                                MIN MAX NBR EXP] */
/*     CNT: [... ... ... ... ... CA1] -> [... ... ... ... CA1 EXP] */
/*                                                                 */
/*     EXP: [DFV ARG MIN MAX NBR VAL] -> [... ... ... ... ... FUN] */
/*     CNT: [... ... ... ... ... CA1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ CA1(_NIL_TYPE_)
  { _EXP_TYPE_ arg, dfv, exp, fun, idx, max, min, nbr, ref, tab, val;
    _UNS_TYPE_ ctr, siz, szz;
    _SGN_TYPE_ ixx, mnn, mxx;
    _stk_claim_();
    _stk_pop__EXP_(val);
    if (_ag_is_TAB_(val))
      { siz = _ag_get_TAB_SIZ_(val);
        if (siz == 2)
          { idx = _ag_get_TAB_EXP_(val, 1);
            _stk_pop__EXP_(nbr);
            _stk_pop__EXP_(max);
            _stk_pop__EXP_(min);
            _stk_pop__EXP_(arg);
            _stk_peek_EXP_(dfv);
            _stk_poke_EXP_(val);
            mxx = _ag_get_NBR_(max);
            mnn = _ag_get_NBR_(min);
            if (_ag_is_VOI_(idx))
              if (_ag_is_VOI_(dfv))
                dfv = _ag_get_TAB_EXP_(val, 2); 
              else
                _error_str_(_DDC_ERROR_, cas_STR);
            else   
              { if (!_ag_is_NBR_(idx))
                  _error_str_(_ICT_ERROR_, cas_STR);
                ixx = _ag_get_NBR_(idx);
                if (mxx<ixx)
                  { mxx = ixx; max = idx; }
                if (mnn>ixx)
                  { mnn = ixx; min = idx; } }
            siz = _ag_get_TAB_SIZ_(arg);
            ctr = _ag_get_NBU_(nbr);
            if (ctr < siz)
              { _stk_push_EXP_(dfv);
                _stk_push_EXP_(arg);
                _stk_push_EXP_(min);
                _stk_push_EXP_(max);
                nbr = _ag_succ_NBR_(nbr);
                _stk_push_EXP_(nbr);
                exp = _ag_get_TAB_EXP_(arg, ctr+1);
                _stk_push_EXP_(exp); 
                _stk_push_CNT_(EXP);
                return; }
            szz = (_UNS_TYPE_)(mxx-mnn+2);
            _mem_claim_SIZ_(szz);
            tab = _ag_make_TAB_(szz);    
            for (ixx = 1; ixx <= szz; ixx++)
              _ag_set_TAB_EXP_(tab, ixx, dfv);
            for (ctr = 1; ctr <= siz; ctr++)
              { _stk_pop__EXP_(val);
                idx = _ag_get_TAB_EXP_(val, 1);
                if( !_ag_is_VOI_(idx))
                  { ixx = _ag_get_NBR_(idx);
                    exp = _ag_get_TAB_EXP_(tab, ixx-mnn+1);
                    if (!_mem_is_same_(exp, dfv))
                      _error_str_(_DCT_ERROR_, cas_STR);
                    val = _ag_get_TAB_EXP_(val, 2);
                    _ag_set_TAB_EXP_(tab, ixx-mnn+1, val); }}
            arg = _ag_make_TAB_3_(css_VAR, tab, min); 
            ref = _ag_make_REF_(sel_NAM); 
            exp = _ag_make_APL_(ref, arg);    
            fun = _ag_make_FUN_(sel_NAM, arg_VAR, exp, dct_VAR);
            _stk_push_EXP_(fun); 
            _stk_zap__CNT_(); 
            return; }}
    _error_str_(_ICC_ERROR_, cas_STR); }
    
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*-----------------M E T A     O P E R A T O R S-------------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
 
/*-----------------------------------------------------------------*/
/*  LOD                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... LOD] -> [... ... ... ... LO2 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ LOD(_NIL_TYPE_)
  { UNARY(LO2, lod_STR); }
    
/*-----------------------------------------------------------------*/
/*  LO2                                                            */
/*     EXP: [... ... ... ... ... TXT] -> [... ... ... ... ... ...] */
/*     CNT: [... ... ... ... ... LO2] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ LO2(_NIL_TYPE_)
  { _EXP_TYPE_ txt;
    _STR_TYPE_ str;
    _stk_peek_EXP_(txt);
    _stk_poke_EXP_(_NUL_);
    _stk_zap__CNT_();
    if (_ag_is_TXT_(txt))
     { str = _ag_get_TXT_(txt);
       sprintf(_BUF_, "%s", str);
       _load_(_BUF_); }
    _error_str_(_ATC_ERROR_, lod_STR); }
 
/*-----------------------------------------------------------------*/
/*  DMP                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... ...] */
/*     CNT: [... ... ... ... ... DMP] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ DMP(_NIL_TYPE_)
  { _error_str_(_CTL_ERROR_, dmp_STR); }
    
/*-----------------------------------------------------------------*/
/*  RED                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... RED] -> [... ... ... ... RE3 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ RED(_NIL_TYPE_)
  { UNARY(RE3, rea_STR); }
 
/*-----------------------------------------------------------------*/
/*  RE3                                                            */
/*     EXP: [... ... ... ... ... TXT] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... RE3] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ RE3(_NIL_TYPE_)
  { _EXP_TYPE_ txt;
    _STR_TYPE_ str;
    _stk_peek_EXP_(txt);
    if (_ag_is_TXT_(txt))
      { str = _ag_get_TXT_(txt);
        _stk_zap__CNT_();
        _stk_poke_EXP_(_read_(str));
        return; }
    _error_str_(_ATC_ERROR_, rea_STR); }
 
/*-----------------------------------------------------------------*/
/*  EVA                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... EVA] -> [... ... ... ... EV1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ EVA(_NIL_TYPE_)
  { UNARY(EV1, eva_STR); }
 
/*-----------------------------------------------------------------*/
/*  EV1                                                            */
/*     EXP: [... ... ... ... ... AGR] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... EV1] -> [... ... ... ... ... EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ EV1(_NIL_TYPE_)
  { _stk_poke_CNT_(EXP); }
 
/*-----------------------------------------------------------------*/
/*  PRT                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... PRT] -> [... ... ... ... PR1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ PRT(_NIL_TYPE_)
  { UNARY(PR1, pri_STR); }
 
/*-----------------------------------------------------------------*/
/*  PR1                                                            */
/*     EXP: [... ... ... ... ... EXP] -> [... ... ... ... *N* EXP] */
/*     CNT: [... ... ... ... ... PR1] -> [... ... ... ... ... PRI] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ PR1(_NIL_TYPE_)
  { _EXP_TYPE_ exp;
    _stk_peek_EXP_(exp);
    _stk_poke_EXP_(_NUL_);
    _stk_push_EXP_(exp);
    _stk_poke_CNT_(PRI); }
 
/*-----------------------------------------------------------------*/
/*  TAG                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... TAG] -> [... ... ... ... TA3 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ TAG(_NIL_TYPE_)
  { UNARY(TA3, tag_STR); }
  
/*-----------------------------------------------------------------*/
/*  TA3                                                            */
/*     EXP: [... ... ... ... ... EXP] -> [... ... ... ... ... NBR] */
/*     CNT: [... ... ... ... ... TA3] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ TA3(_NIL_TYPE_)
  { _EXP_TYPE_ exp, nbr;
    _TAG_TYPE_ tag;
    _stk_peek_EXP_(exp);
    tag = (_TAG_TYPE_)_ag_get_TAG_(exp);
    nbr = _ag_make_NBR_(tag);    
    _stk_poke_EXP_(nbr);
    _stk_zap__CNT_(); }
 
/*-----------------------------------------------------------------*/
/*  RNK                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... RNK] -> [... ... ... ... RN1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ RNK(_NIL_TYPE_)
  { UNARY(RN1, rnk_STR); }
 
/*-----------------------------------------------------------------*/
/*  RN1                                                            */
/*     EXP: [... ... ... ... ... VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... RN1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ RN1(_NIL_TYPE_)
  { _EXP_TYPE_ exp, nbr;
    _UNS_TYPE_ rnk;
    _stk_peek_EXP_(exp);
    _stk_zap__CNT_();
    if (_ag_is_NBR_(exp))
      { _stk_poke_EXP_(_ZRO_);
        return; }
    switch (_ag_get_TAG_(exp))
      { case _APL_TAG_:
          rnk = _APL_RANK_;
          break;
        case _CST_TAG_:
          rnk = _CST_RANK_;
          break;
        case _CNT_TAG_:
          rnk = _CNT_RANK_;
          break; 
        case _DCL_TAG_:
          rnk = _DCL_RANK_;
          break;
        case _DEF_TAG_:
          rnk = _DEF_RANK_;
          break;
        case _FRC_TAG_:    
          _stk_poke_EXP_(_ZRO_);
          return;
        case _FUN_TAG_:
          rnk = _FUN_RANK_;
          break;
        case _NAT_TAG_:
          rnk = _NAT_RANK_;
          break;
        case _QUA_TAG_:
          rnk = _QUA_RANK_;
          break;
        case _REF_TAG_:
          rnk = _REF_RANK_;
          break;
        case _SET_TAG_:
          rnk = _SET_RANK_;
          break;
        case _TAB_TAG_:
          rnk = _ag_get_TAB_SIZ_(exp);
          break;
        case _TBL_TAG_:
          rnk = _TBL_RANK_;
          break;
        case _TXT_TAG_:     
          _stk_poke_EXP_(_ZRO_);
          return;
        case _VAR_TAG_:
          rnk = _VAR_RANK_;
          break;
        case _VOI_TAG_:
          _stk_poke_EXP_(_ZRO_);
          return; }
    nbr = _ag_make_NBR_(rnk);
    _stk_poke_EXP_(nbr); }
 
/*-----------------------------------------------------------------*/
/*  MAK                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ARG TAB *1* EXP] */
/*     CNT: [... ... ... ... ... MAK] -> [... ... ... ... MA1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ MAK(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp, tab;
    _UNS_TYPE_ siz;
    _stk_claim_();
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    if (siz == 0)
      _error_str_(_ALS_ERROR_, mak_STR); 
    _mem_claim_SIZ_(siz);
    _stk_peek_EXP_(arg);
    tab = _ag_make_TAB_(siz);
    exp = _ag_get_TAB_EXP_(arg, 1);
    _stk_push_EXP_(tab);
    _stk_push_EXP_(_ONE_);
    _stk_push_EXP_(exp);
    _stk_poke_CNT_(MA1); 
    _stk_push_CNT_(EXP); } 
 
/*-----------------------------------------------------------------*/
/*  MA1                                                            */
/*     EXP: [... ... ARG TAB NBR VAL] -> [... ... ARG TAB NBR EXP] */
/*     CNT: [... ... ... ... ... MA1] -> [... ... ... ... MA1 EXP] */
/*                                                                 */
/*     EXP: [... ... ARG TAB NBR VAL] -> [... ... ... ... ... TAB] */
/*     CNT: [... ... ... ... ... MA1] -> [... ... ... ... ... MA2] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ MA1(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp, nbr, tab, val;
    _UNS_TYPE_ ctr, siz;
    _stk_claim_();
    _stk_pop__EXP_(val);
    _stk_pop__EXP_(nbr);
    _stk_pop__EXP_(tab);
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    ctr = _ag_get_NBU_(nbr);
    _ag_set_TAB_EXP_(tab, ctr, val);
    if (ctr == siz)
      { _stk_poke_EXP_(tab);
        _stk_poke_CNT_(MA2);
        return; }
    exp = _ag_get_TAB_EXP_(arg, ctr+1);
    nbr = _ag_succ_NBR_(nbr);
    _stk_push_EXP_(tab);
    _stk_push_EXP_(nbr);
    _stk_push_EXP_(exp);
    _stk_push_CNT_(EXP); }

/*-----------------------------------------------------------------*/
/*  MA2                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... MA2] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ MA2(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp, nbr, tab;
    _UNS_TYPE_ idx, rnk, tag;
    _stk_claim_();
    _mem_claim_();
    _stk_peek_EXP_(arg);
    rnk = _ag_get_TAB_SIZ_(arg) - 1;
    nbr = _ag_get_TAB_EXP_(arg, 1);
    if(!_ag_is_NBR_(nbr))
      _error_str_(_TAG_ERROR_, mak_STR);
    tag = _ag_get_NBU_(nbr);
    switch (tag)
      { case _APL_TAG_:
          RANK(_APL_RANK_);
          exp = _ag_make_APL_(TEXT(2),  
                              EXPRESSION(3));
          break;
        case _CST_TAG_:
          RANK(_CST_RANK_);
          exp = _ag_make_CST_(TEXT(2),  
                              EXPRESSION(3),  
                              DICTIONARY(4));
          break;
        case _CNT_TAG_:
          RANK(_CNT_RANK_);
          exp = _ag_make_CNT_(DICTIONARY(2),  
                              NUMBER(3),  
                              NUMBER(4),  
                              TABLE(5));
          break; 
        case _DCL_TAG_:
          RANK(_DCL_RANK_);
          exp = _ag_make_DCL_(INVOCATION(2),  
                              EXPRESSION(3));
          break;
        case _DEF_TAG_:
          RANK(_DEF_RANK_);
          exp = _ag_make_DEF_(INVOCATION(2), 
                              EXPRESSION(3));
          break;
        case _FRC_TAG_:    
          RANK(_FRC_RANK_);
          exp = FRACTION(2);
          break;
        case _FUN_TAG_:
          RANK(_FUN_RANK_);
          exp = _ag_make_FUN_(TEXT(2), 
                              EXPRESSION(3), 
                              EXPRESSION(4), 
                              DICTIONARY(5));
          break;
        case _NAT_TAG_:
          RANK(_NAT_RANK_);
          exp = _ag_make_NAT_(TEXT(2),  
                              NUMBER(3));
          break;
        case _NBR_TAG_:    
          RANK(_NBR_RANK_);
          exp = NUMBER(2);
          break;
        case _QUA_TAG_:
          RANK(_QUA_RANK_);
          exp = _ag_make_QUA_(TEXT(2),  
                              INVOCATION(3));
          break;
        case _REF_TAG_:
          RANK(_REF_RANK_);
          exp = _ag_make_REF_(TEXT(2));
          break;
        case _SET_TAG_:
          RANK(_SET_RANK_);
          exp = _ag_make_SET_(INVOCATION(2),  
                              EXPRESSION(3));
          break;
        case _TAB_TAG_:
          _mem_claim_SIZ_(rnk);
          _stk_peek_EXP_(arg);
          if (rnk == 0)
            { exp = _EMP_;
              break; }
          tab = _ag_make_TAB_(rnk);
          for (idx = 1; idx <= rnk; idx++)
            { exp = _ag_get_TAB_EXP_(arg, idx+1);
              _ag_set_TAB_EXP_(tab, idx, exp); }
          exp = tab;
          break;
        case _TBL_TAG_:
          RANK(_TBL_RANK_);
          exp = _ag_make_TBL_(TEXT(2),  
                              TABLE(3));
          break;
        case _TXT_TAG_:     
          RANK(_TXT_RANK_);
          exp = TEXT(2);
          break;
        case _VAR_TAG_:
          RANK(_VAR_RANK_);
          exp = _ag_make_VAR_(TEXT(2),  
                              EXPRESSION(3),  
                              DICTIONARY(4));
          break;
        case _VOI_TAG_:
          exp = _VOI_;
          break;
        default:
          _error_str_(_TAG_ERROR_, mak_STR); }
    _stk_poke_EXP_(exp);
    _stk_zap__CNT_(); }
 
/*-----------------------------------------------------------------*/
/*  GET                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... EXP EXP] */
/*     CNT: [... ... ... ... ... GET] -> [... ... ... GE2 SWP EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ GET(_NIL_TYPE_)
  { BINARY(GE2, get_STR); }
 
/*-----------------------------------------------------------------*/
/*  GE2                                                            */
/*     EXP: [... ... ... ... AGR VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... GE2] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ GE2(_NIL_TYPE_)
  { _EXP_TYPE_ agr, nbr, val;
    _UNS_TYPE_ siz;
    _SGN_TYPE_ pos;
    _stk_claim_();
    _stk_pop__EXP_(nbr);
    _stk_peek_EXP_(agr);
    if (_ag_is_raw_(agr))
      _error_exp_(_TIR_ERROR_, agr);
    siz = _ag_get_TAB_SIZ_(agr);
    if (!_ag_is_NBR_(nbr))
      _error_exp_(_NNI_ERROR_, nbr);
    pos = _ag_get_NBR_(nbr);
    if (pos <= 0)
      _error_exp_(_NPI_ERROR_, nbr);
    if (pos > siz)
      _error_exp_(_IBS_ERROR_, nbr);
    val = _ag_get_TAB_EXP_(agr, pos);
    _stk_poke_EXP_(val);
    _stk_zap__CNT_(); }
    
/*-----------------------------------------------------------------*/
/*  SET                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ARG EXP] */
/*     CNT: [... ... ... ... ... SET] -> [... ... ... ... SE2 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ SET(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp;
    _UNS_TYPE_ siz;
    _stk_claim_();
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    if (siz != 3)
      _error_str_(_NMA_ERROR_, set_STR); 
    exp = _ag_get_TAB_EXP_(arg, 1);
    _stk_push_EXP_(exp);
    _stk_poke_CNT_(SE2); 
    _stk_push_CNT_(EXP); }
   
/*-----------------------------------------------------------------*/
/*  SE2                                                            */
/*     EXP: [... ... ... ... ARG AGR] -> [... ... ... AGR ARG EXP] */
/*     CNT: [... ... ... ... ... SE2] -> [... ... ... ... SE3 EXP] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ SE2(_NIL_TYPE_)
  { _EXP_TYPE_ agr, arg, exp;
    _stk_claim_();
    _stk_pop__EXP_(agr);
    _stk_peek_EXP_(arg);
    exp = _ag_get_TAB_EXP_(arg, 2);
    _stk_poke_EXP_(agr);
    _stk_push_EXP_(arg);
    _stk_push_EXP_(exp);
    _stk_poke_CNT_(SE3); 
    _stk_push_CNT_(EXP); }
    
/*-----------------------------------------------------------------*/
/*  SE3                                                            */
/*     EXP: [... ... ... AGR ARG NBR] -> [... ... ... AGR NBR EXP] */
/*     CNT: [... ... ... ... ... SE3] -> [... ... ... ... SE4 EXP] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ SE3(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp, nbr;
    _stk_pop__EXP_(nbr);
    _stk_peek_EXP_(arg);
    exp = _ag_get_TAB_EXP_(arg, 3);
    _stk_poke_EXP_(nbr);
    _stk_push_EXP_(exp);
    _stk_poke_CNT_(SE4); 
    _stk_push_CNT_(EXP); }
    
/*-----------------------------------------------------------------*/
/*  SE4                                                            */
/*     EXP: [... ... ... AGR NBR VAL] -> [... ... ... ... ... AGR] */
/*     CNT: [... ... ... ... ... SE4] -> [... ... ... ... ... *V*] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ SE4(_NIL_TYPE_)
  { _EXP_TYPE_ agr, nbr, val;
    _UNS_TYPE_ siz;
    _SGN_TYPE_ pos;
    _stk_pop__EXP_(val);
    _stk_pop__EXP_(nbr);
    _stk_peek_EXP_(agr);
    if (_ag_is_raw_(agr))
      _error_exp_(_TIR_ERROR_, agr);
    siz = _ag_get_TAB_SIZ_(agr);
    if (!_ag_is_NBR_(nbr))
      _error_exp_(_NNI_ERROR_, nbr);
    pos = _ag_get_NBR_(nbr);
    if (pos <= 0)
      _error_exp_(_NPI_ERROR_, nbr);
    if (pos > siz)
      _error_exp_(_IBS_ERROR_, nbr);
    _ag_set_TAB_EXP_(agr, pos, val);
    _stk_poke_EXP_(_VOI_);
    _stk_zap__CNT_(); }   

/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*---------------------C O N T I N U A T I O N S-------------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
 
/*-----------------------------------------------------------------*/
/*  CLL                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... DCT EXP] */
/*     CNT: [... ... ... ... ... CLL] -> [... ... ... ... CL1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ CLL(_NIL_TYPE_)
  { _EXP_TYPE_ arg, cnt, exp;
    _UNS_TYPE_ siz;
    _stk_claim_();
    _mem_claim_SIZ_(_CNT_size_());
    _stk_pop__EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    if (siz != 1)
      _error_str_(_NMA_ERROR_, cll_STR); 
    cnt = _ag_make_CNT_(_VOI_, _VOI_, _VOI_, _VOI_);
    _stk_zap__CNT_();
    _CNT_save_(cnt);
    _stk_push_EXP_(_DCT_);
    _DCT_ = _ag_make_VAR_(cnt_NAM, cnt, _DCT_);
    exp = _ag_get_TAB_EXP_(arg, 1);
    _stk_push_EXP_(exp);
    _stk_push_CNT_(CL1); 
    _stk_push_CNT_(EXP); }

/*-----------------------------------------------------------------*/
/*  CL1                                                            */
/*     EXP: [... ... ... ... DCT VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... CL1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ CL1(_NIL_TYPE_)
  { _EXP_TYPE_ val;
    _stk_pop__EXP_(val);
    _stk_peek_EXP_(_DCT_);
    _stk_poke_EXP_(val);
    _stk_zap__CNT_(); }
 
/*-----------------------------------------------------------------*/
/*  CON                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... EXP EXP] */
/*     CNT: [... ... ... ... ... CON] -> [... ... ... CO2 SWP EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ CON(_NIL_TYPE_)
  { BINARY(CO2, con_STR); }
 
/*-----------------------------------------------------------------*/
/*  CO2                                                            */
/*     EXP: [... ... ... ... CNT VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... CO2] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ CO2(_NIL_TYPE_)
  { _EXP_TYPE_ cnt, val;
    _stk_pop__EXP_(val); // dvd
    _stk_peek_EXP_(cnt); // dvd
    _stk_push_EXP_(val); // dvd
    if (_ag_is_CNT_(cnt)) // dvd
       _mem_claim_SIZ_(_ag_get_TAB_SIZ_(_ag_get_CNT_TAB_(cnt))); // dvd
    // claim pulled out of _CNT_load_ to prevent corruption
    // this patch is dirty but minimally intrusive
    _stk_pop__EXP_(val);
    _stk_pop__EXP_(cnt);
    if (_ag_is_CNT_(cnt))
      { _CNT_load_(cnt);
        _stk_push_EXP_(val);
        return; }
    _error_str_(_ATC_ERROR_, con_STR); }
 
/*-----------------------------------------------------------------*/
/*  ESC                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... _V_] */
/*     CNT: [... ... ... ... ... ESC] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ ESC(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp, fun;
    _UNS_TYPE_ siz;
    _mem_claim_();
    fun = _ag_make_FUN_(esc_NAM, _EMP_, _VOI_, _VOI_); 
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    if (siz != 1)
      _error_str_(_NMA_ERROR_, esc_STR); 
    exp = _ag_get_TAB_EXP_(arg, 1);
    _ag_set_FUN_EXP_(fun, exp);
    _ag_set_FUN_DCT_(fun, _DCT_); 
    _ESC_ = fun;
    _stk_poke_EXP_(_VOI_);
    _stk_zap__CNT_(); }
 
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*----------------------D I C T I O N A R I E S--------------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
 
/*-----------------------------------------------------------------*/
/*  CPT                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... DCT] */
/*     CNT: [... ... ... ... ... CPT] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ CPT(_NIL_TYPE_)
  { _EXP_TYPE_ arg;
    _UNS_TYPE_ siz;
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    if (siz > 0)
      _error_str_(_NMA_ERROR_, cpt_STR);
    _stk_poke_EXP_(_DCT_);
    _stk_zap__CNT_(); } 
 
/*-----------------------------------------------------------------*/
/*  CMT                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... CMT] -> [... ... ... ... CM1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ CMT(_NIL_TYPE_)
  { UNARY(CM1, cmt_STR); }
  
/*-----------------------------------------------------------------*/
/*  CM1                                                            */
/*     EXP: [... ... ... ... ... DCT] -> [... ... ... ... ... *V*] */
/*     CNT: [... ... ... ... ... CM1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ CM1(_NIL_TYPE_)
  { _EXP_TYPE_ dct;
    _stk_peek_EXP_(dct);
    if (_ag_is_DCT_(dct))
      { _DCT_ = dct;
        _stk_poke_EXP_(_VOI_);
        _stk_zap__CNT_(); 
        return; }
    _error_str_(_ATC_ERROR_, cmt_STR); }
 
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------E R R O R---------------------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
 
/*-----------------------------------------------------------------*/
/*  ERR                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... ERR] -> [... ... ... ... ER1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ ERR(_NIL_TYPE_)
  { UNARY(ER1, err_STR); }
 
/*-----------------------------------------------------------------*/
/*  ER1                                                            */
/*     EXP: [... ... ... ... ... TXT] -> [... ... ... ... ... TXT] */
/*     CNT: [... ... ... ... ... ER1] -> [... ... ... ... ... ER1] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ ER1(_NIL_TYPE_)
  { _EXP_TYPE_ txt;
    _stk_peek_EXP_(txt);
    if (_ag_is_TXT_(txt))
       _error_nam_(_USR_ERROR_, txt); 
    _error_str_(_ATC_ERROR_, err_STR); }
 
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*------------------------------C A S E----------------------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
 
/*-----------------------------------------------------------------*/
/*  SEL                                                            */
/*     EXP: [... ... ... ... ... ARG] -> [... ... ... NBR TAB EXP] */
/*     CNT: [... ... ... ... ... SEL] -> [... ... ... ... SE1 EXP] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ SEL(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp, nbr, tab;
    _UNS_TYPE_ siz;
    _stk_claim_();
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    if (siz != 3)
      _error_str_(_NMA_ERROR_, sel_STR); 
    nbr = _ag_get_TAB_EXP_(arg, 3);
    _stk_poke_EXP_(nbr);
    tab = _ag_get_TAB_EXP_(arg, 2);
    _stk_push_EXP_(tab);
    exp = _ag_get_TAB_EXP_(arg, 1);
    _stk_push_EXP_(exp);
    _stk_poke_CNT_(SE1); 
    _stk_push_CNT_(EXP); }
   
/*-----------------------------------------------------------------*/
/*  SE1                                                            */
/*     EXP: [... ... ... NBR TAB IDX] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... SE1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ SE1(_NIL_TYPE_)
  { _EXP_TYPE_ idx, nbr, tab, val;
    _UNS_TYPE_ siz;
    _SGN_TYPE_ ixx, mxx;
    _stk_pop__EXP_(idx);
    if (_ag_is_NBR_(idx))
      { _stk_pop__EXP_(tab);
        siz = _ag_get_TAB_SIZ_(tab);
        _stk_peek_EXP_(nbr);
        ixx = _ag_get_NBR_(idx);
        mxx = _ag_get_NBR_(nbr);
        ixx = ixx-mxx+1;
        if ((ixx < 1) | (ixx > siz))
          ixx = (_SGN_TYPE_)siz;
        val = _ag_get_TAB_EXP_(tab, ixx);
        _stk_poke_EXP_(val);
        _stk_zap__CNT_(); 
        return; }
    _error_str_(_ATC_ERROR_, sel_STR); }
 
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*----------------------------N A T I V E--------------------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
 
/*-----------------------------------------------------------------*/
/*  NAT                                                            */
/*     EXP: [... ... ... ... NAT TAB] -> [... ... ... ... ... TAB] */
/*     CNT: [... ... ... ... ... NAT] -> [... ... ... ... ... ***] */
/*-----------------------------------------------------------------*/
 
static _NIL_TYPE_ NAT(_NIL_TYPE_)
  { _EXP_TYPE_ idx, nam, nat, tab;
    _CNT_TYPE_ cnt;
    _UNS_TYPE_ uns;
    _stk_pop__EXP_(tab);
    _stk_peek_EXP_(nat);
    if (!_ag_is_TAB_(tab))
      { nam = _ag_get_NAT_NAM_(nat);
        _error_nam_(_ALT_ERROR_, nam); }
    _stk_poke_EXP_(tab);
    idx = _ag_get_NAT_IDX_(nat);
    uns = _ag_get_NBU_(idx);
    if (uns > NATIVE_FUN_SIZE)
      { nam = _ag_get_NAT_NAM_(nat);
        _error_nam_(_INI_ERROR_, nam); }
    cnt = fun_TAB[uns].cnt;
    _stk_poke_CNT_(cnt); }
        
/*----------------------- public variables ------------------------*/
 
_EXP_TYPE_ _BEG_;
_EXP_TYPE_ _EMP_;
_EXP_TYPE_ _ESC_;
_EXP_TYPE_ _NUL_;
_EXP_TYPE_ _ONE_;
_EXP_TYPE_ _TAB_;
_EXP_TYPE_ _TWO_;
_EXP_TYPE_ _VOI_;
_EXP_TYPE_ _ZRO_;
 
_CNT_TYPE_ _eva_NAT_ = NAT;
 
/*----------------------- public functions ------------------------*/
 
/*-----------------------------------------------------------------*/
/*  INT                                                            */
/*     EXP: [... ... ... ... ... ...] -> [... ... ... ... *X* *E*] */
/*     CNT: [... ... ... ... ... _I_] -> [... ... ... ... IGN CAL] */
/*-----------------------------------------------------------------*/
 
 _NIL_TYPE_ _nat_interrupt_(_NIL_TYPE_)
  { _stk_claim_();
    _stk_push_EXP_(_ESC_);
    _stk_push_EXP_(_EMP_);
    _stk_push_CNT_(IGN);
    _stk_push_CNT_(CAL); }
    
_EXP_TYPE_ _nat_in_cache(const _EXP_TYPE_ Nam)
  { _EXP_TYPE_ exp;
    if((!_mem_get_cache_(CCH, Nam, &exp)) || _ag_is_VOI_(exp))
      _error_nam_(_UDI_ERROR_, Nam);
    return exp; }
             
_BYT_TYPE_ _nat_install_(_NIL_TYPE_)
  { _EXP_TYPE_ dct, nam, nbr, val;
    _STR_TYPE_ str;
    _UNS_TYPE_ idx;
    MAX = _ag_make_NBR_(+ _NBR_MAX_);    
    MIN = _ag_make_NBR_(- _NBR_MAX_);     
    _ZRO_ = _ag_make_NBR_(0); 
    _ONE_ = _ag_make_NBR_(1);    
    _TWO_ = _ag_make_NBR_(2);    
    _VOI_ = _ag_make_VOI_();  
    for (idx = 0; idx < NATIVE_CST_SIZE; idx++)
      { str = cst_TAB[idx].nam;
        nam = _CNT_set_NAM_(str); }
    for (idx = 0; idx < NATIVE_VAR_SIZE; idx++)
      { str = var_TAB[idx].nam;
        nam = _CNT_set_NAM_(str); }
    for (idx = 0; idx < NATIVE_FUN_SIZE; idx++)
      { str = fun_TAB[idx].nam;
        nam = _CNT_set_NAM_(str); }
    dct = _VOI_; 																					
    for (idx = 0; idx < NATIVE_CST_SIZE; idx++)
      { str = cst_TAB[idx].nam;
        nam = _CNT_make_NAM_(str);
        val = cst_TAB[idx].ini(); 
        dct = _ag_make_CST_(nam, val, dct); }
    for (idx = 0; idx < NATIVE_VAR_SIZE; idx++)
      { str = var_TAB[idx].nam;
        nam = _CNT_make_NAM_(str);
        val = var_TAB[idx].ini(); 
        dct = _ag_make_VAR_(nam, val, dct); }
    for (idx = 0; idx < NATIVE_FUN_SIZE; idx++)
      { str = fun_TAB[idx].nam;
        nam = _CNT_make_NAM_(str);
        nbr = _ag_make_NBR_(idx); 
        val = _ag_make_NAT_(nam, nbr);  
        dct = _ag_make_CST_(nam, val, dct); }
    _NAT_ = dct;    
    srand(clock());
    return _nat_reset_(); }
    
_BYT_TYPE_ _nat_reset_(_NIL_TYPE_)
  { _EXP_TYPE_ css, dct, nam, nat, nbr;
    _STR_TYPE_ str;
    _UNS_TYPE_ idx;
    for (idx = 0; idx < NATIVE_CST_SIZE; idx++)
      { str = cst_TAB[idx].nam;
        nam = _CNT_make_NAM_(str); 
        _mem_enter_cache_(nam); }
    for (idx = 0; idx < NATIVE_FUN_SIZE; idx++)
      { str = fun_TAB[idx].nam;
        nam = _CNT_make_NAM_(str); 
        _mem_enter_cache_(nam); }
    if(!_mem_make_cache_(_TAB_TAG_, &CCH))
       return _FALSE_;
    dct = _NAT_;
    do
      { nam = _ag_get_DCT_NAM_(dct);
        if(!_mem_set_cache_(CCH, nam, dct))
          return _FALSE_;
        dct = _ag_get_DCT_DCT_(dct); }
    while (!_ag_is_VOI_(dct));
    tru_VAR = _ag_get_DCT_VAL_(_nat_in_cache(_CNT_make_NAM_(trr_STR)));
    fls_VAR = _ag_get_DCT_VAL_(_nat_in_cache(_CNT_make_NAM_(fls_STR)));
    _EMP_ = _ag_get_DCT_VAL_(_nat_in_cache(_CNT_make_NAM_(emp_STR)));
    _VOI_ = _ag_get_DCT_VAL_(_nat_in_cache(_CNT_make_NAM_(voi_STR)));
    _ESC_ = _VOI_;
    _NUL_ = _CNT_make_NAM_(nul_STR);    
    _BEG_ = _ag_make_REF_(_CNT_make_NAM_(beg_STR));     
    _TAB_ = _ag_make_REF_(_CNT_make_NAM_(tab_STR));     
    cnt_NAM = _CNT_make_NAM_(cnt_STR);
    esc_NAM = _CNT_make_NAM_(esc_STR);
    sel_NAM = _CNT_make_NAM_(sel_STR);  
    css     = _CNT_make_NAM_(css_STR); 
    nbr     = _ag_make_NBR_(SELECT_NBR);      
    nat     = _ag_make_NAT_(sel_NAM, nbr);   
    dct_VAR = _ag_make_VAR_(sel_NAM, nat, _VOI_);
    css_VAR = _ag_make_REF_(css);
    arg_VAR = _ag_make_TAB_ONE_(css_VAR);
    bln_VAR = _ag_make_TAB_TWO_(tru_VAR, fls_VAR); 
    return _TRUE_; }
