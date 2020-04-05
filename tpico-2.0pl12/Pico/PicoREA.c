             /*-----------------------------------*/
             /*           >>>Pico 2.0<<<          */
             /*            Theo D'Hondt           */
             /*   VUB Programming Technology Lab  */
             /*             (c) 2002              */
             /*-----------------------------------*/
             /*              Reading              */
             /*-----------------------------------*/


#include "Pico.h"
#include "PicoMAI.h"
#include "PicoCNT.h"
#include "PicoMEM.h"
#include "PicoNAT.h"
#include "PicoREA.h"
#include "PicoSCA.h"

/*--------------------- private constants -------------------------*/

#define REALLOC_FACTOR 1.5

/*--------------------- private macros ----------------------------*/

#define _(PTR) ((_EXP_TYPE_ *)(mem_ADR(PTR)))
      
/*--------------------- private prototypes ------------------------*/

static _NIL_TYPE_ expression(_NIL_TYPE_);
static _NIL_TYPE_ invocation(_NIL_TYPE_);
static _NIL_TYPE_ operand   (_NIL_TYPE_);
static _NIL_TYPE_ prefix    (_NIL_TYPE_);

/*--------------------- private variables -------------------------*/

static _TKN_TYPE_ TKN;
static _UNS_TYPE_ TOP;

/*--------------------- private functions -------------------------*/

static _NIL_TYPE_ push(_EXP_TYPE_ Exp)
  { _EXP_TYPE_ old;
    _UNS_TYPE_ nws, siz;
    siz = _ag_get_TAB_SIZ_(_RST_); 
    _(_RST_)[++TOP] = Exp;  
    if (TOP == siz)
      { nws = (_UNS_TYPE_)(siz*REALLOC_FACTOR);
        _mem_claim_SIZ_(nws);
        old = _RST_;
        _RST_ = _ag_make_TAB_(nws);
        for (; siz; siz--)
          _(_RST_)[siz] = _(old)[siz]; }}
        
static _NIL_TYPE_ poke(_EXP_TYPE_ Exp)
  { _(_RST_)[TOP] = Exp; }
        
static _EXP_TYPE_ pop(_NIL_TYPE_)
  { return _(_RST_)[TOP--]; }
        
static _EXP_TYPE_ peek(_NIL_TYPE_)
  { return _(_RST_)[TOP]; }
  
static _NIL_TYPE_ skip(_NIL_TYPE_)
  { TKN = _scan_(); }
        
static _NIL_TYPE_ expect(_TKN_TYPE_ Tkn, _RES_TYPE_ Err)
  { if (TKN == Tkn)
      skip();
    else
      _scan_error_(Err); }
          
static _NIL_TYPE_ infix(_CNT_TYPE_ Opr, _TKN_TYPE_ Tkn)
  { _EXP_TYPE_ apl, arg, opr, ref, txt;
    _UNS_TYPE_ cnt;
    _STR_TYPE_ str;
    Opr();
    while (TKN == Tkn)
      { str = _scan_string_();
        _mem_claim_STR_(str);
        txt = _CNT_make_NAM_(str);
        cnt = 1;
        do
          { cnt++;
            skip();
            push(txt);
            Opr();
            opr = pop();
            txt = peek();
            poke(opr);
            str = _ag_get_TXT_(txt); }
        while ((TKN == Tkn) && !strcmp(str, _scan_string_()));
        push(txt);
        _mem_claim_SIZ_(cnt);
        txt = pop();
        arg = _ag_make_TAB_(cnt);
        for (; cnt > 0; cnt--)
          _(arg)[cnt] = pop(); 
        ref = _ag_make_REF_(txt);
        apl = _ag_make_APL_(ref, arg);
        push(apl); }}
                
/*-----------------------------------------------------------------*/
/*  semicolonlist ::= expression { ; expression }                  */
/*  commalist     ::= expression { , expression }                  */
/*-----------------------------------------------------------------*/
static _NIL_TYPE_ list(_TKN_TYPE_ Sep, _TKN_TYPE_ Trm, _RES_TYPE_ Err)
  { _EXP_TYPE_ tab;
    _UNS_TYPE_ cnt;
    for (cnt = 1 ;; cnt++)
      { expression();
        if (TKN == Trm)
          { skip();
            _mem_claim_SIZ_(cnt);
            tab = _ag_make_TAB_(cnt);
            for (; cnt > 0; cnt--)
              _(tab)[cnt] = pop();
            push(tab);
            return; }
				if (TKN == Sep)
				  skip();
				else
				  _scan_error_(Err); }}

/*-----------------------------------------------------------------*/
/*  name          ::= <name>                                       */
/*-----------------------------------------------------------------*/
static _NIL_TYPE_ name(_NIL_TYPE_)
  { _EXP_TYPE_ ref, txt;
    _STR_TYPE_ str;
    str = _scan_string_();
    _mem_claim_STR_(str);
    txt = _CNT_make_NAM_(str);
    skip();
    ref = _ag_make_REF_(txt);
    push(ref); }

/*-----------------------------------------------------------------*/
/*  operator      ::= <operator>                                   */
/*-----------------------------------------------------------------*/
static _NIL_TYPE_ operator(_NIL_TYPE_)
  { _EXP_TYPE_ ref, txt;
    _STR_TYPE_ str;
    str = _scan_string_();
    _mem_claim_STR_(str);
    txt = _CNT_make_NAM_(str);
    skip();
    ref = _ag_make_REF_(txt);
    push(ref); }

/*-----------------------------------------------------------------*/
/*  application   ::= "(" [ commalist ] ")"                        */
/*-----------------------------------------------------------------*/
static _NIL_TYPE_ application(_NIL_TYPE_)
  { _EXP_TYPE_ apl, arg, exp;
    skip();
    if (TKN == _RPR_TOKEN_)
      { skip();
        push(_EMP_); }
    else
      list(_COM_TOKEN_, _RPR_TOKEN_, _RPR_ERROR_);
    _mem_claim_();
    arg = pop();
    exp = peek();
    apl = _ag_make_APL_(exp, arg);
    poke(apl); }

/*-----------------------------------------------------------------*/
/*  apply         ::= "@" operand                                  */
/*-----------------------------------------------------------------*/
static _NIL_TYPE_ apply(_NIL_TYPE_)
  { _EXP_TYPE_ apl, opr, exp;
    skip();
    operand();
    _mem_claim_();
    opr = pop();
    exp = peek();
    apl = _ag_make_APL_(exp, opr);
    poke(apl); }

/*-----------------------------------------------------------------*/
/*  tabulation    ::= "[" commalist "]"                            */
/*-----------------------------------------------------------------*/
static _NIL_TYPE_ tabulation(_NIL_TYPE_)
  { _EXP_TYPE_ exp, tab, tbl;
    skip();
    list(_COM_TOKEN_, _RBR_TOKEN_, _RBR_ERROR_);
    _mem_claim_();
    tab = pop();
    exp = peek();
    tbl = _ag_make_TBL_(exp, tab);
    poke(tbl); }

/*-----------------------------------------------------------------*/
/*  qualification ::= "." qualifier    													   */
/*                            [ application | apply | tabulation ] */
/*  qualifier     ::= name | operator                              */
/*-----------------------------------------------------------------*/
static _NIL_TYPE_ qualification(_NIL_TYPE_)
  { _EXP_TYPE_ exp, inv, qua;
    skip();
    switch (TKN)
   	  { case _NAM_TOKEN_:
   	      name();
   	      break;
   	    case _AOP_TOKEN_:
   	    case _MOP_TOKEN_:
   	    case _ROP_TOKEN_:
   	    case _XOP_TOKEN_:
   	      operator();
   	      break;
        default:
   	      _scan_error_(_VAR_ERROR_); }
    switch (TKN)
   	  { case _LPR_TOKEN_:
   	      application();
   	      break;
        case _CAT_TOKEN_:
   	      apply();		
   	      break;
        case _LBR_TOKEN_:
   	      tabulation();  
   	      break; }
   	_mem_claim_();
    inv = pop();
    exp = peek();
    qua = _ag_make_QUA_(exp, inv);
    poke(qua); }
   	
/*-----------------------------------------------------------------*/
/*  invocation    ::= [ ( application   |                          */
/*                        apply         |                          */
/*                        tabulation    |                          */
/*                        qualification ) invocation ]             */            
/*-----------------------------------------------------------------*/
static _NIL_TYPE_ invocation(_NIL_TYPE_)
  { switch (TKN)
   	  { case _LPR_TOKEN_:
   	      application(); 
   	      break;
        case _CAT_TOKEN_:
   	      apply();	 
   	      break;
        case _LBR_TOKEN_:
   	      tabulation();  
   	      break;
        case _PER_TOKEN_:
   	      qualification(); 
   	      break;
        default:
   	      return; }
    invocation(); }

/*-----------------------------------------------------------------*/
/*  number        ::= <number>                                     */
/*-----------------------------------------------------------------*/
static _NIL_TYPE_ number(_NIL_TYPE_) 
  { _EXP_TYPE_ nbr;
    _LNG_TYPE_ lng;
    lng = _scan_number_();
    skip();
    nbr = _ag_make_NBR_(lng);
    push(nbr); }

/*-----------------------------------------------------------------*/
/*  fraction      ::= <fraction>                                   */
/*-----------------------------------------------------------------*/
static _NIL_TYPE_ _fraction(_NIL_TYPE_) 
  { _EXP_TYPE_ frc;
    _FLO_TYPE_ flo;
    _mem_claim_();
    flo = _scan_fraction_();
    skip();
    frc = _ag_make_FRC_(flo);
    push(frc); }

/*-----------------------------------------------------------------*/
/*  text          ::= <text>                                       */
/*-----------------------------------------------------------------*/
static _NIL_TYPE_ text(_NIL_TYPE_)
  { _EXP_TYPE_ txt;
    _STR_TYPE_ str;
    str = _scan_string_();
    _mem_claim_STR_(str);
    txt = _CNT_make_NAM_(str);
    skip();
    push(txt); }
                                                      
/*-----------------------------------------------------------------*/
/*  quotation     ::= "'" expression                               */
/*-----------------------------------------------------------------*/
static _NIL_TYPE_ quotation(_NIL_TYPE_)
  { _EXP_TYPE_ exp, quo;
    skip(); 
    expression();
    _mem_claim_();
    exp = peek();
    quo = _ag_make_QUO_(exp);
    poke(quo); }

/*-----------------------------------------------------------------*/
/*  begin         ::= "{" semicolonlist "}" invocation             */
/*-----------------------------------------------------------------*/
static _NIL_TYPE_ begin(_NIL_TYPE_)
  { _EXP_TYPE_ apl, arg;
    skip();
    list(_SMC_TOKEN_, _RBC_TOKEN_, _RBC_ERROR_);
    _mem_claim_();
    arg = peek();
    apl = _ag_make_APL_(_BEG_, arg);
    poke(apl);
    invocation(); }

/*-----------------------------------------------------------------*/
/*  variable      ::= <name> invocation )                          */
/*-----------------------------------------------------------------*/
static _NIL_TYPE_ variable(_NIL_TYPE_)
  { name();
    invocation(); }

/*-----------------------------------------------------------------*/
/*  reference     ::= number | fraction | text | quotation |       */
/*                    begin  | prefix   | variable                 */
/*-----------------------------------------------------------------*/
static _NIL_TYPE_ reference(_NIL_TYPE_)
  { _EXP_TYPE_ apl, exp, ref, tab;
    switch (TKN)
      { case _NBR_TOKEN_:
          number();
          break;
        case _FRC_TOKEN_:
          _fraction();
          break;
        case _TXT_TOKEN_:
          text();
          break;
        case _QUO_TOKEN_:
          quotation();
          break;
        case _LBC_TOKEN_:
          begin();
          break;
        case _AOP_TOKEN_:
        case _MOP_TOKEN_:
        case _ROP_TOKEN_:
        case _XOP_TOKEN_:
          prefix();
          break;
        case _NAM_TOKEN_:
          variable(); 
          break;
        default:
          return; }
    _mem_claim_();
    exp = pop();
    tab = _ag_make_TAB_ONE_(exp);
    ref = peek();
    apl = _ag_make_APL_(ref, tab);
    poke(apl); }
 
/*-----------------------------------------------------------------*/
/*  prefix       ::= operator ( reference | invocation )           */
/*-----------------------------------------------------------------*/
static _NIL_TYPE_ prefix(_NIL_TYPE_)
  { operator();
    switch (TKN)
      { case _NBR_TOKEN_:
        case _FRC_TOKEN_:
        case _TXT_TOKEN_:
        case _QUO_TOKEN_:
        case _AOP_TOKEN_:
        case _MOP_TOKEN_:
        case _ROP_TOKEN_:
        case _XOP_TOKEN_:
        case _NAM_TOKEN_:
        case _LBC_TOKEN_:
          reference();
          return;
        default: 
          invocation(); }}   	
   	
/*-----------------------------------------------------------------*/
/*  table         ::= "[" [ commalist ] "]" invocation             */
/*-----------------------------------------------------------------*/
static _NIL_TYPE_ table(_NIL_TYPE_)
  { _EXP_TYPE_ apl, arg;
    skip();
    if(TKN == _RBR_TOKEN_) 
      { skip();
        push(_EMP_); }
    else
      list(_COM_TOKEN_, _RBR_TOKEN_, _RBR_ERROR_);
    _mem_claim_();
    arg = peek();
    apl = _ag_make_APL_(_TAB_, arg);
    poke(apl);
    invocation(); }

/*-----------------------------------------------------------------*/
/*  subexpression ::= "(" expression ")" invocation                */
/*-----------------------------------------------------------------*/
static _NIL_TYPE_ subexpression(_NIL_TYPE_)
  { skip();
    expression();
    expect(_RPR_TOKEN_, _RPR_ERROR_);
    invocation(); }

/*-----------------------------------------------------------------*/
/*  operand       ::= reference | table | subexpression            */
/*  reference     ::= number | fraction | text | quotation         */
/*                    begin  | prefix   | variable                 */
/*-----------------------------------------------------------------*/
static _NIL_TYPE_ operand(_NIL_TYPE_)
  { switch (TKN)
      { case _NBR_TOKEN_:
          number();
          return;
        case _FRC_TOKEN_:
          _fraction(); 
          return;
        case _TXT_TOKEN_:
          text();		 
          return;
        case _QUO_TOKEN_:
          quotation();	 
          return;
        case _LBC_TOKEN_:
          begin();	 
          return;
        case _AOP_TOKEN_:
        case _MOP_TOKEN_:
        case _ROP_TOKEN_:
        case _XOP_TOKEN_:
          prefix(); 
          return;
        case _NAM_TOKEN_:
          variable();		 
          return;
        case _LBR_TOKEN_:
          table();	 
          return;
        case _LPR_TOKEN_:
          subexpression(); 
          return;
        default: 
          _scan_error_(_OPD_ERROR_); }}
  
/*-----------------------------------------------------------------*/
/*  factor        ::= operand { power operand }                    */
/*-----------------------------------------------------------------*/
static _NIL_TYPE_ factor(_NIL_TYPE_) 
  { infix(operand, _XOP_TOKEN_); }

/*-----------------------------------------------------------------*/
/*  term          ::= factor { multiplier factor }                 */
/*-----------------------------------------------------------------*/
static _NIL_TYPE_ term(_NIL_TYPE_) 
  { infix(factor, _MOP_TOKEN_); }

/*-----------------------------------------------------------------*/
/*  comparand     ::= term { adder term }                          */
/*-----------------------------------------------------------------*/
static _NIL_TYPE_ comparand(_NIL_TYPE_) 
  { infix(term, _AOP_TOKEN_); }

/*-----------------------------------------------------------------*/
/*  operation     ::= comparand { comparator comparand }           */
/*-----------------------------------------------------------------*/
static _NIL_TYPE_ operation(_NIL_TYPE_)
  { infix(comparand, _ROP_TOKEN_); }

/*-----------------------------------------------------------------*/
/*  definition    ::= ":" expression                               */
/*-----------------------------------------------------------------*/
static _NIL_TYPE_ definition(_NIL_TYPE_) 
  { _EXP_TYPE_ def, exp, opr;
    skip(); 
    expression();
    _mem_claim_();
    exp = pop();
    opr = peek();
    def = _ag_make_DEF_(opr, exp);
    poke(def); }
        
/*-----------------------------------------------------------------*/
/*  declaration   ::= "::" expression                              */
/*-----------------------------------------------------------------*/
static _NIL_TYPE_ declaration(_NIL_TYPE_) 
  { _EXP_TYPE_ dcl, exp, opr;
    skip(); 
    expression();
    _mem_claim_();
    exp = pop();
    opr = peek();
    dcl = _ag_make_DCL_(opr, exp);
    poke(dcl); }
        
/*-----------------------------------------------------------------*/
/*  assignment    ::= ":=" expression                              */
/*-----------------------------------------------------------------*/
static _NIL_TYPE_ assignment(_NIL_TYPE_) 
  { _EXP_TYPE_ exp, opr, set;
    skip(); 
    expression();
    _mem_claim_();
    exp = pop();
    opr = peek();
    set = _ag_make_SET_(opr, exp);
    poke(set); }
        
/*-----------------------------------------------------------------*/
/*  expression    ::= operation [ definition  | 									 */
/*                                declaration |                    */
/*                                assignment  ]                    */
/*-----------------------------------------------------------------*/
static _NIL_TYPE_ expression(_NIL_TYPE_)
  { operation();
    switch (TKN)
      { case _COL_TOKEN_:
          definition();
          return;
        case _CCL_TOKEN_:
          declaration(); 
          return;
        case _CEQ_TOKEN_:
          assignment();
          return; }}
         
/*-------------------- public functions ------------------------*/

_EXP_TYPE_ _read_(_STR_TYPE_ Str)
  { _scan_init_(Str, _FALSE_); 
    skip(); 
    TOP = 0;
    expression();
    expect(_END_TOKEN_, _EXT_ERROR_);
    return pop(); }
    
/*-----------------------------------------------------------------*/
/*  REA                                                            */
/*     EXP: [... ... ... ... ... ...] -> [... ... ... ... ... ...] */
/*     CNT: [... ... ... ... ... REA] -> [... ... ... ... EXT EXP] */
/*-----------------------------------------------------------------*/
_NIL_TYPE_ _rea_EXP_(_NIL_TYPE_)
  { _EXP_TYPE_ exp;
    _stk_claim_();
    skip(); 
    TOP = 0;
    expression();
    expect(_END_TOKEN_, _EXT_ERROR_);
    exp = pop();
    _stk_push_EXP_(exp);
    _stk_zap__CNT_(); }
