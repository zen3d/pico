             /*-----------------------------------*/
             /*           >>>Pico 2.0<<<          */
             /*            Theo D'Hondt           */
             /*   VUB Programming Technology Lab  */
             /*             (c) 2002              */
             /*-----------------------------------*/
             /*            Evaluation             */
             /*-----------------------------------*/

#include "Pico.h"
#include "PicoMAI.h"
#include "PicoCNT.h"
#include "PicoMEM.h"
#include "PicoNAT.h"
#include "PicoEVA.h"

/*---------------------- private constants ------------------------*/

#define NAT _eva_NAT_   

/*--------------------- private prototypes ------------------------*/

static _NIL_TYPE_ APL(_NIL_TYPE_);
static _NIL_TYPE_ CLL(_NIL_TYPE_);
static _NIL_TYPE_ CL1(_NIL_TYPE_); 
static _NIL_TYPE_ CL2(_NIL_TYPE_); 
static _NIL_TYPE_ CL3(_NIL_TYPE_);
static _NIL_TYPE_ DCL(_NIL_TYPE_); 
static _NIL_TYPE_ DEF(_NIL_TYPE_); 
static _NIL_TYPE_ EXP(_NIL_TYPE_);
static _NIL_TYPE_ IDX(_NIL_TYPE_); 
static _NIL_TYPE_ ID1(_NIL_TYPE_); 
static _NIL_TYPE_ ID2(_NIL_TYPE_); 
static _NIL_TYPE_ QUA(_NIL_TYPE_); 
static _NIL_TYPE_ QUO(_NIL_TYPE_); 
static _NIL_TYPE_ QU1(_NIL_TYPE_); 
static _NIL_TYPE_ RET(_NIL_TYPE_); 
static _NIL_TYPE_ SET(_NIL_TYPE_); 
static _NIL_TYPE_ SE1(_NIL_TYPE_); 
static _NIL_TYPE_ SE2(_NIL_TYPE_); 
static _NIL_TYPE_ SE3(_NIL_TYPE_); 
static _NIL_TYPE_ TBL(_NIL_TYPE_);
static _NIL_TYPE_ TB1(_NIL_TYPE_);

/*--------------------- private functions -------------------------*/

/*-----------------------------------------------------------------*/
/*  APL                                                            */
/*     EXP: [... ... ... ... ARG FUN] -> [... ... ... ... FUN TAB] */
/*     CNT: [... ... ... ... ... APL] -> [... ... ... ... ... CLL] */
/*                                                                 */
/*     EXP: [... ... ... ... ARG FUN] -> [... ... ... ... FUN EXP] */
/*     CNT: [... ... ... ... ... APL] -> [... ... ... ... CLL EXP] */
/*                                                                 */
/*     EXP: [... ... ... ... ARG NAT] -> [... ... ... ... NAT TAB] */
/*     CNT: [... ... ... ... ... APL] -> [... ... ... ... ... NAT] */
/*                                                                 */
/*     EXP: [... ... ... ... ARG NAT] -> [... ... ... ... NAT EXP] */
/*     CNT: [... ... ... ... ... APL] -> [... ... ... ... NAT EXP] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ APL(_NIL_TYPE_)
  { _EXP_TYPE_ arg, exp;
    _TAG_TYPE_ tag;
    _stk_claim_();
    _stk_pop__EXP_(exp);
    _stk_peek_EXP_(arg);
    _stk_poke_EXP_(exp);
    _stk_push_EXP_(arg);
    tag = _ag_get_TAG_(exp);
    switch (tag)
      { case _FUN_TAG_:
          _stk_poke_CNT_(CLL);
          break;
        case _NAT_TAG_:
          _stk_poke_CNT_(NAT); 
          break; 
        default:
          _error_exp_(_ARF_ERROR_, exp); }
    if (!_ag_is_TAB_(arg))  
      _stk_push_CNT_(EXP); }

/*-----------------------------------------------------------------*/
/*  CLL                                                            */
/*     EXP: [... ... ... ... FUN TAB] -> [... ... ... ... DCT EXP] */
/*     CNT: [... ... ... ... ... CLL] -> [... ... ... ... RET EXP] */
/*                                                                 */
/*     EXP: [... ... ... ... FUN TAB] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... RET CLL] -> [... ... ... ... ... EXP] */
/*                                                                 */
/*     EXP: [... ... ... ... FUN TAB] -> [EXP DCT ARG TAB *1* EXP] */
/*     CNT: [... ... ... ... ... CLL] -> [... ... ... ... CL1 EXP] */
/*                                                                 */
/*     EXP: [... ... ... ... FUN TAB] -> [EXP DCT ARG TAB *1* APL] */
/*     CNT: [... ... ... ... ... CLL] -> [... ... ... ... ... CL2] */
/*                                                                 */
/*     EXP: [... ... ... ... FUN TAB] -> [FUN EXP PAR ARG          */
/*                                                    *1* DCT EXP] */
/*     CNT: [... ... ... ... ... CLL] -> [... ... ... ... CL3 EXP] */
/*                                                                 */
/*     EXP: [... ... ... ... FUN TAB] -> [FUN EXP PAR ARG          */
/*                                                    *1* DCT FUN] */
/*     CNT: [... ... ... ... ... CLL] -> [... ... ... ... ... CL3] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ CLL(_NIL_TYPE_)
  { _EXP_TYPE_ act, arg, dct, exp, frm, fun, nam, par, ref, tab;
    _CNT_TYPE_ cnt;
    _UNS_TYPE_ siz, xsz;
    _RES_TYPE_ res;
    _stk_claim_();
    _stk_peek_EXP_(arg);
    if (!_ag_is_TAB_(arg))  
      { _stk_zap__EXP_();
        _stk_peek_EXP_(fun);
        res = _ALT_ERROR_; goto ERR; }
    siz = _ag_get_TAB_SIZ_(arg);
    _mem_claim_SIZ_(siz);
    _stk_pop__EXP_(arg);
    _stk_peek_EXP_(fun);
    par = _ag_get_FUN_ARG_(fun);
    exp = _ag_get_FUN_EXP_(fun);
    dct = _ag_get_FUN_DCT_(fun);
    if (siz == 0)
      { switch (_ag_get_TAG_(par))
          { case _REF_TAG_:
              nam = _ag_get_REF_NAM_(par); 
              dct = _ag_make_VAR_(nam, _EMP_, dct);
              break;
            case _APL_TAG_:
              ref = _ag_get_APL_EXP_(par); 
		          if(!_ag_is_REF_(ref))
					      { res = _AGR_ERROR_; goto ERR; }
					    nam = _ag_get_REF_NAM_(ref);
              dct = _ag_make_VAR_(nam, _EMP_, dct);
              break;
            case _TAB_TAG_:
              siz = _ag_get_TAB_SIZ_(par);
              if (siz != 0)
                { res = _PLE_ERROR_; goto ERR; }; 
              break;
            default:
              { res = _PLT_ERROR_; goto ERR; } }
        _stk_zap__CNT_();
        _stk_peek_CNT_(cnt);
        if (cnt != RET)
          { _stk_poke_EXP_(_DCT_);
            _stk_push_EXP_(exp); 
            _stk_push_CNT_(RET); }
        else
          _stk_poke_EXP_(exp);
        _stk_push_CNT_(EXP); 
        _DCT_ = dct; 
        return; }
    switch (_ag_get_TAG_(par))
      { case _REF_TAG_:
          nam = _ag_get_REF_NAM_(par); 
          tab = _ag_make_TAB_(siz);
          dct = _ag_make_VAR_(nam, tab, dct);
          act = _ag_get_TAB_EXP_(arg, 1);
          _stk_poke_EXP_(exp);
          _stk_push_EXP_(dct);
          _stk_push_EXP_(arg);
          _stk_push_EXP_(tab);
          _stk_push_EXP_(_ONE_);
          _stk_push_EXP_(act);
          _stk_poke_CNT_(CL1);
          _stk_push_CNT_(EXP);
          return; 
        case _APL_TAG_:
          ref = _ag_get_APL_EXP_(par); 
          if(!_ag_is_REF_(ref))
			      { res = _AGR_ERROR_; goto ERR; }
			    nam = _ag_get_REF_NAM_(ref);
			    tab = _ag_make_TAB_(siz);
          dct = _ag_make_VAR_(nam, tab, dct);
          _stk_poke_EXP_(exp);
          _stk_push_EXP_(dct);
          _stk_push_EXP_(arg);
          _stk_push_EXP_(tab);
          _stk_push_EXP_(_ONE_);
          _stk_push_EXP_(par);
          _stk_poke_CNT_(CL2);
          return; 
        case _TAB_TAG_:
          xsz = _ag_get_TAB_SIZ_(par);
          if (siz != xsz)
            { res = _ALS_ERROR_; goto ERR; };
          frm = _ag_get_TAB_EXP_(par, 1);
          act = _ag_get_TAB_EXP_(arg, 1);
          _stk_push_EXP_(exp);
          _stk_push_EXP_(par);
          _stk_push_EXP_(arg);
          _stk_push_EXP_(_ONE_);
          switch (_ag_get_TAG_(frm))
            { case _REF_TAG_:
                nam = _ag_get_REF_NAM_(frm);
                dct = _ag_make_VAR_(nam, _VOI_, dct);
                _stk_push_EXP_(dct);
                _stk_push_EXP_(act);
                _stk_poke_CNT_(CL3);
                _stk_push_CNT_(EXP);
                return;
              case _APL_TAG_:
                ref = _ag_get_APL_EXP_(frm); 
			          if(!_ag_is_REF_(ref))
						      { res = _AGR_ERROR_; goto ERR; }
						    nam = _ag_get_REF_NAM_(ref);
                dct = _ag_make_VAR_(nam, _VOI_, dct);
                arg = _ag_get_APL_ARG_(frm);
                fun = _ag_make_FUN_(nam, arg, act, _DCT_);
                _stk_push_EXP_(dct);
                _stk_push_EXP_(fun);
                _stk_poke_CNT_(CL3);
                return;
              default: 
                { res = _IPT_ERROR_; goto ERR; }}}
    res = _PLT_ERROR_;
  ERR:
    _error_fun_(res, fun); }
             
/*-----------------------------------------------------------------*/
/*  CL1                                                            */
/*     EXP: [EXP DCT ARG TAB NBR VAL] -> [EXP DCT ARG TAB NBR EXP] */
/*     CNT: [... ... ... ... ... CL1] -> [... ... ... ... CL1 EXP] */
/*                                                                 */
/*     EXP: [EXP DCT ARG TAB NBR VAL] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... RET CL1] -> [... ... ... ... ... EXP] */
/*                                                                 */
/*     EXP: [EXP DCT ARG TAB NBR VAL] -> [... ... ... ... DCT EXP] */
/*     CNT: [... ... ... ... ... CL1] -> [... ... ... ... RET EXP] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ CL1(_NIL_TYPE_)
  { _EXP_TYPE_ act, arg, dct, exp, nbr, tab, val;
    _CNT_TYPE_ cnt;
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
      { _stk_zap__EXP_();
        _stk_pop__EXP_(dct);
        _stk_zap__CNT_();
        _stk_peek_CNT_(cnt);
        if (cnt != RET)
          { _stk_peek_EXP_(exp);
            _stk_poke_EXP_(_DCT_);
            _stk_push_EXP_(exp); 
            _stk_push_CNT_(RET); }
        _stk_push_CNT_(EXP); 
        _DCT_ = dct; 
        return; }
    act = _ag_get_TAB_EXP_(arg, ctr+1);
    nbr = _ag_succ_NBR_(nbr);
    _stk_push_EXP_(tab);
    _stk_push_EXP_(nbr);
    _stk_push_EXP_(act);
    _stk_push_CNT_(EXP); }

/*-----------------------------------------------------------------*/
/*  CL2                                                            */
/*     EXP: [EXP DCT ARG TAB NBR APL] -> [EXP DCT ARG TAB NBR APL] */
/*     CNT: [... ... ... ... ... CL2] -> [... ... ... ... ... CL2] */
/*                                                                 */
/*     EXP: [EXP DCT ARG TAB NBR APL] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... RET CL2] -> [... ... ... ... ... EXP] */
/*                                                                 */
/*     EXP: [EXP DCT ARG TAB NBR APL] -> [... ... ... ... DCT EXP] */
/*     CNT: [... ... ... ... ... CL2] -> [... ... ... ... RET EXP] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ CL2(_NIL_TYPE_)
  { _EXP_TYPE_ act, apl, arg, dct, exp, fun, nam, nbr, par, ref, tab;
    _CNT_TYPE_ cnt;
    _RES_TYPE_ res;
    _UNS_TYPE_ ctr, siz;
    _mem_claim_();
    _stk_pop__EXP_(apl);
    _stk_pop__EXP_(nbr);
    _stk_pop__EXP_(tab);
    _stk_peek_EXP_(arg);
    siz = _ag_get_TAB_SIZ_(arg);
    ctr = _ag_get_NBU_(nbr);
    act = _ag_get_TAB_EXP_(arg, ctr);
    ref = _ag_get_APL_EXP_(apl); 
    if(!_ag_is_REF_(ref))
      { res = _AGR_ERROR_; goto ERR; }
    nam = _ag_get_REF_NAM_(ref);
    par = _ag_get_APL_ARG_(apl);
    fun = _ag_make_FUN_(nam, par, act, _DCT_);
    _ag_set_TAB_EXP_(tab, ctr, fun);
    if (ctr == siz)
      { _stk_zap__EXP_();
        _stk_pop__EXP_(dct);
        _stk_zap__CNT_();
        _stk_peek_CNT_(cnt);
        if (cnt != RET)
          { _stk_peek_EXP_(exp);
            _stk_poke_EXP_(_DCT_);
            _stk_push_EXP_(exp); 
            _stk_push_CNT_(RET); }
        _stk_push_CNT_(EXP); 
        _DCT_ = dct; 
        return; }
    nbr = _ag_succ_NBR_(nbr);
    _stk_push_EXP_(tab);
    _stk_push_EXP_(nbr);
    _stk_push_EXP_(apl);
    return;
 ERR:
    _error_exp_(res, ref); }

/*-----------------------------------------------------------------*/
/*  CL3                                                            */
/*     EXP: [FUN EXP PAR ARG          -> [... ... ... ... ... EXP] */
/*                       NBR DCT VAL]                              */
/*     CNT: [... ... ... ... RET CL3] -> [... ... ... ... ... EXP] */
/*                                                                 */
/*     EXP: [FUN EXP PAR ARG          -> [... ... ... ... DCT EXP] */
/*                       NBR DCT VAL]                              */
/*     CNT: [... ... ... ... ... CL3] -> [... ... ... ... RET EXP] */
/*                                                                 */
/*     EXP: [FUN EXP PAR ARG          -> [FUN EXP PAR ARG        ] */
/*                       NBR DCT VAL]                 NBR DCT EXP] */
/*     CNT: [... ... ... ... ... CL3] -> [... ... ... ... CL3 EXP] */
/*                                                                 */
/*     EXP: [FUN EXP PAR ARG          -> [FUN EXP PAR ARG        ] */
/*                       NBR DCT VAL]                 NBR DCT FUN] */
/*     CNT: [... ... ... ... ... CL3] -> [... ... ... ... ... CL3] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ CL3(_NIL_TYPE_)
  { _EXP_TYPE_ act, arg, dct, exp, fun, frm, nam, nbr, par, ref, val;
    _CNT_TYPE_ cnt;
    _UNS_TYPE_ ctr, siz;
    _RES_TYPE_ res;
    _stk_claim_();
    _mem_claim_();
    _stk_pop__EXP_(val);
    _stk_pop__EXP_(dct);
    _stk_pop__EXP_(nbr);
    _stk_pop__EXP_(arg);
    _ag_set_DCT_VAL_(dct, val);
    siz = _ag_get_TAB_SIZ_(arg);
    ctr = _ag_get_NBU_(nbr);
    if (ctr == siz)
      { _stk_zap__EXP_();
        _stk_pop__EXP_(exp);
        _stk_zap__CNT_();
        _stk_peek_CNT_(cnt);
        if (cnt != RET)
          { _stk_poke_EXP_(_DCT_);
            _stk_push_EXP_(exp); 
            _stk_push_CNT_(RET); }
        else
          _stk_poke_EXP_(exp); 
        _stk_push_CNT_(EXP); 
        _DCT_ = dct; 
        return; }
    _stk_peek_EXP_(par);
    frm = _ag_get_TAB_EXP_(par, ++ctr);
    act = _ag_get_TAB_EXP_(arg, ctr);
    nbr = _ag_succ_NBR_(nbr);
    switch (_ag_get_TAG_(frm))
      { case _REF_TAG_:          
          nam = _ag_get_REF_NAM_(frm);    
          dct = _ag_make_VAR_(nam, _VOI_, dct);
          _stk_push_EXP_(arg); 
          _stk_push_EXP_(nbr); 
          _stk_push_EXP_(dct); 
          _stk_push_EXP_(act);
          _stk_push_CNT_(EXP);
          return;
        case _APL_TAG_:
          ref = _ag_get_APL_EXP_(frm); 
          if(!_ag_is_REF_(ref))
			      { res = _AGR_ERROR_; goto ERR; }
			    nam = _ag_get_REF_NAM_(ref);
          par = _ag_get_APL_ARG_(frm);
          dct = _ag_make_VAR_(nam, _VOI_, dct);
          fun = _ag_make_FUN_(nam, par, act, _DCT_);
          _stk_push_EXP_(arg); 
          _stk_push_EXP_(nbr); 
          _stk_push_EXP_(dct); 
          _stk_push_EXP_(fun);
          return; }
    res = _IPT_ERROR_;
  ERR:
    _stk_zap__EXP_();
    _stk_zap__EXP_();
    _stk_peek_EXP_(fun);
    _error_fun_(res, fun); }
          
/*-----------------------------------------------------------------*/
/*  DCL                                                            */
/*     EXP: [... ... ... ... NAM VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... DCL] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ DCL(_NIL_TYPE_)
  { _EXP_TYPE_ nam, val;
    _mem_claim_();
    _stk_pop__EXP_(val);
    _stk_peek_EXP_(nam);
    _DCT_ = _ag_make_CST_(nam, val, _DCT_); 
    _stk_poke_EXP_(val); 
    _stk_zap__CNT_(); }
   
/*-----------------------------------------------------------------*/
/*  DEF                                                            */
/*     EXP: [... ... ... ... NAM VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... DEF] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ DEF(_NIL_TYPE_)
  { _EXP_TYPE_ nam, val;
    _mem_claim_();
    _stk_pop__EXP_(val);
    _stk_peek_EXP_(nam);
    _DCT_ = _ag_make_VAR_(nam, val, _DCT_); 
    _stk_poke_EXP_(val); 
    _stk_zap__CNT_(); }

/*-----------------------------------------------------------------*/
/*  EXP                                                            */
/*     EXP: [... ... ... ... ... APL] -> [... ... ... ... ARG EXP] */
/*     CNT: [... ... ... ... ... EXP] -> [... ... ... ... APL EXP] */
/*                                                                 */
/*     EXP: [... ... ... ... ... DCL] -> [... ... ... ... NAM EXP] */
/*     CNT: [... ... ... ... ... EXP] -> [... ... ... ... DCL EXP] */
/*                                                                 */
/*     EXP: [... ... ... ... ... DCL] -> [... ... ... ... ... FUN] */
/*     CNT: [... ... ... ... ... EXP] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ... ... ... DCL] -> [... NAM INI IDX *1* EXP] */
/*     CNT: [... ... ... ... ... EXP] -> [... ... ... DCL IDX EXP] */
/*                                                                 */
/*     EXP: [... ... ... ... ... DEF] -> [... ... ... ... NAM EXP] */
/*     CNT: [... ... ... ... ... EXP] -> [... ... ... ... DEF EXP] */
/*                                                                 */
/*     EXP: [... ... ... ... ... DEF] -> [... ... ... ... ... FUN] */
/*     CNT: [... ... ... ... ... EXP] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ... ... ... DEF] -> [... NAM INI IDX *1* EXP] */
/*     CNT: [... ... ... ... ... EXP] -> [... ... ... DEF IDX EXP] */
/*                                                                 */
/*     EXP: [... ... ... ... ... QUA] -> [... ... ... ... INV EXP] */
/*     CNT: [... ... ... ... ... EXP] -> [... ... ... ... QUA EXP] */
/*                                                                 */
/*     EXP: [... ... ... ... ... QUO] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... EXP] -> [... ... ... ... ... QUO] */
/*                                                                 */
/*     EXP: [... ... ... ... ... REF] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... EXP] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ... ... ... SET] -> [... ... ... ... DCT EXP] */
/*     CNT: [... ... ... ... ... EXP] -> [... ... ... ... SET EXP] */
/*                                                                 */
/*     EXP: [... ... ... ... ... SET] -> [... ... ... ... ... FUN] */
/*     CNT: [... ... ... ... ... EXP] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ... ... ... SET] -> [... ... ... EXP IDX EXP] */
/*     CNT: [... ... ... ... ... EXP] -> [... ... ... ... SE1 EXP] */
/*                                                                 */
/*     EXP: [... ... ... ... ... TBL] -> [... ... ... ... IDX EXP] */
/*     CNT: [... ... ... ... ... EXP] -> [... ... ... ... TBL EXP] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ EXP(_NIL_TYPE_)
  { _EXP_TYPE_ apl, arg, cmp, dcl, dct, def, exp, ex1, fun,   
               idx, inv, nam, qua, quo, ref, set, tbl, val;
	  _UNS_TYPE_ siz;
	  _RES_TYPE_ res;
	  _stk_peek_EXP_(exp);
    switch (_ag_get_TAG_(exp))
      { case _APL_TAG_:
			    _stk_claim_();
			    _stk_peek_EXP_(apl);
			    exp = _ag_get_APL_EXP_(apl);
			    arg = _ag_get_APL_ARG_(apl);
			    _stk_poke_EXP_(arg);
			    _stk_push_EXP_(exp);
			    _stk_poke_CNT_(APL);
			    _stk_push_CNT_(EXP);
			    return; 
        case _DCL_TAG_:
				  _stk_claim_();
			    _mem_claim_();
			    _stk_peek_EXP_(dcl);
			    inv = _ag_get_DCL_INV_(dcl);
			    exp = _ag_get_DCL_EXP_(dcl);
			    switch (_ag_get_TAG_(inv))
			      { case _REF_TAG_:
			          nam = _ag_get_REF_NAM_(inv);
			          _stk_poke_EXP_(nam);
			          _stk_push_EXP_(exp);
			          _stk_poke_CNT_(DCL);
			          _stk_push_CNT_(EXP); 
			          return;
			        case _APL_TAG_:
			          ref = _ag_get_APL_EXP_(inv);
			          arg = _ag_get_APL_ARG_(inv);
			          if(!_ag_is_REF_(ref))
			            { res = _AGR_ERROR_; goto ERR; }
			          nam = _ag_get_REF_NAM_(ref);
			          _DCT_ = _ag_make_CST_(nam, _VOI_, _DCT_);
			          fun = _ag_make_FUN_(nam, arg, exp, _DCT_);
			          _ag_set_DCT_VAL_(_DCT_, fun);
			          _stk_poke_EXP_(fun); 
			          _stk_zap__CNT_(); 
			          return;
			        case _TBL_TAG_:
			          ref = _ag_get_TBL_EXP_(inv);
			          idx = _ag_get_TBL_IDX_(inv);
			          if(!_ag_is_REF_(ref))
			            { res = _AGR_ERROR_; goto ERR; }
			          nam = _ag_get_REF_NAM_(ref);
			          if (!_ag_is_TAB_(idx))
	                { res = _MTI_ERROR_; goto ERR; }
			          siz = _ag_get_TAB_SIZ_(idx);
			          if (siz == 0)
	                { res = _ETI_ERROR_; goto ERR; }
			          cmp = _ag_get_TAB_EXP_(idx, 1);
			          _stk_poke_EXP_(nam);
			          _stk_push_EXP_(exp);
			          _stk_push_EXP_(idx);
			          _stk_push_EXP_(_ONE_);
			          _stk_push_EXP_(cmp);
			          _stk_poke_CNT_(DCL);
			          _stk_push_CNT_(IDX);
			          _stk_push_CNT_(EXP); 
			          return;
			        default:
			          nam = _NUL_; }
          res = _INV_ERROR_; 
          goto ERR; 
        case _DEF_TAG_:
			    _stk_claim_();
			    _mem_claim_();
			    _stk_peek_EXP_(def);
			    inv = _ag_get_DEF_INV_(def);
			    exp = _ag_get_DEF_EXP_(def);
			    switch (_ag_get_TAG_(inv))
			      { case _REF_TAG_:
			          nam = _ag_get_REF_NAM_(inv);
			          _stk_poke_EXP_(nam);
			          _stk_push_EXP_(exp);
			          _stk_poke_CNT_(DEF);
			          _stk_push_CNT_(EXP); 
			          return;
			        case _APL_TAG_:
			          ref = _ag_get_APL_EXP_(inv);
			          arg = _ag_get_APL_ARG_(inv);
			          if(!_ag_is_REF_(ref))
			            { res = _AGR_ERROR_; goto ERR; }
			          nam = _ag_get_REF_NAM_(ref);
			          _DCT_ = _ag_make_VAR_(nam, _VOI_, _DCT_);
			          fun = _ag_make_FUN_(nam, arg, exp, _DCT_);
			          _ag_set_DCT_VAL_(_DCT_, fun);
			          _stk_poke_EXP_(fun); 
			          _stk_zap__CNT_(); 
			          return;
			        case _TBL_TAG_:
			          ref = _ag_get_TBL_EXP_(inv);
			          idx = _ag_get_TBL_IDX_(inv);
			          if(!_ag_is_REF_(ref))
			            { res = _AGR_ERROR_; goto ERR; }
			          nam = _ag_get_REF_NAM_(ref);
			          if (!_ag_is_TAB_(idx))           
	                { res = _MTI_ERROR_; goto ERR; }
			          siz = _ag_get_TAB_SIZ_(idx);
			          if (siz == 0)           
	                { res = _ETI_ERROR_; goto ERR; }
			          cmp = _ag_get_TAB_EXP_(idx, 1);
			          _stk_poke_EXP_(nam);
			          _stk_push_EXP_(exp);
			          _stk_push_EXP_(idx);
			          _stk_push_EXP_(_ONE_);
			          _stk_push_EXP_(cmp);
			          _stk_poke_CNT_(DEF);
			          _stk_push_CNT_(IDX);
			          _stk_push_CNT_(EXP); 
			          return;
			        default:
			          nam = _NUL_; }
			    res = _INV_ERROR_; 
			    goto ERR;
        case _QUA_TAG_:
			    _stk_claim_();
			    _stk_peek_EXP_(qua);
			    exp = _ag_get_QUA_EXP_(qua);
			    inv = _ag_get_QUA_INV_(qua);
          _stk_poke_EXP_(inv);
          _stk_push_EXP_(exp);
          _stk_poke_CNT_(QUA);
          _stk_push_CNT_(EXP); 
          return; 
        case _QUO_TAG_:
			    _stk_peek_EXP_(quo);
			    exp = _ag_get_QUO_EXP_(quo);
			    _stk_poke_EXP_(exp);
			    _stk_poke_CNT_(QUO);
          return; 
        case _REF_TAG_:
			    _stk_peek_EXP_(ref);
			    nam = _ag_get_REF_NAM_(ref);
			    dct = _dct_locate_(nam, _DCT_);
			    val = _ag_get_DCT_VAL_(dct);
			    _stk_poke_EXP_(val);
			    _stk_zap__CNT_();
          return;
        case _SET_TAG_:
			    _stk_claim_();
			    _mem_claim_();
			    _stk_peek_EXP_(set);
			    inv = _ag_get_SET_INV_(set);
			    exp = _ag_get_SET_EXP_(set);
			    switch (_ag_get_TAG_(inv))
			      { case _REF_TAG_:
			          nam = _ag_get_REF_NAM_(inv);
			          dct = _dct_locate_(nam, _DCT_);
			          if (!_ag_is_VAR_(dct))
			            { res = _SET_ERROR_; goto ERR; };
			          _stk_poke_EXP_(dct);
			          _stk_push_EXP_(exp);
			          _stk_poke_CNT_(SET);
			          _stk_push_CNT_(EXP); 
			          return;
			        case _APL_TAG_:
			          ref = _ag_get_APL_EXP_(inv);
			          arg = _ag_get_APL_ARG_(inv);
			          if(!_ag_is_REF_(ref))
			            { res = _AGR_ERROR_; goto ERR; }
			          nam = _ag_get_REF_NAM_(ref);
			          dct = _dct_locate_(nam, _DCT_);
			          if (!_ag_is_VAR_(dct))
			            { res = _SET_ERROR_; goto ERR; };
			          fun = _ag_make_FUN_(nam, arg, exp, _DCT_);
			          _ag_set_DCT_VAL_(dct, fun);
			          _stk_poke_EXP_(fun); 
			          _stk_zap__CNT_(); 
			          return;
			        case _TBL_TAG_:
			          ex1 = _ag_get_TBL_EXP_(inv);
			          idx = _ag_get_TBL_IDX_(inv);
      			    _stk_poke_EXP_(exp);
      			    _stk_push_EXP_(idx);
      			    _stk_push_EXP_(ex1);
      			    _stk_poke_CNT_(SE1);
      			    _stk_push_CNT_(EXP);
			          return;
			        default:
			          nam = _NUL_; }
			    res = _INV_ERROR_; 
			    goto ERR;
        case _TBL_TAG_:
			    _stk_claim_();
			    _stk_peek_EXP_(tbl);
			    exp = _ag_get_TBL_EXP_(tbl);
			    idx = _ag_get_TBL_IDX_(tbl);
			    _stk_poke_EXP_(idx);
			    _stk_push_EXP_(exp);
			    _stk_poke_CNT_(TBL);
			    _stk_push_CNT_(EXP);
          return; }
    _stk_zap__CNT_();
    return;
 ERR:
    _error_nam_(res, nam); }
   
/*-----------------------------------------------------------------*/
/*  IDX                                                            */
/*     EXP: [... ... INI IDX DIM VAL] -> [... ... TAB INI *1* INI] */
/*     CNT: [... ... ... ... ... IDX] -> [... ... ... ... ID1 EXP] */
/*                                                                 */
/*     EXP: [... ... INI IDX DIM VAL] -> [... ... ... ... ... TAB] */
/*     CNT: [... ... ... ... ... IDX] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... INI IDX DIM VAL] -> [INI IDX TAB DIM *1*      */
/*                                                INI IDX DIM EXP] */
/*     CNT: [... ... ... ... ... IDX] -> [... ... ... ID2 IDX EXP] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ IDX(_NIL_TYPE_)
  { _EXP_TYPE_ dim, exp, idx, ini, nam, ndm, tab, val;
    _SGN_TYPE_ siz;
    _UNS_TYPE_ ctr, usz, us1;
    _RES_TYPE_ res;
    _stk_claim_();
    _stk_peek_EXP_(val);
    if (!_ag_is_NBR_(val)) 
      { res = _NNS_ERROR_; goto ERR; }; 
    siz = _ag_get_NBR_(val);
    usz = (_UNS_TYPE_)siz;
    _mem_claim_SIZ_(usz);
    _stk_pop__EXP_(val);
    _stk_pop__EXP_(dim);
    _stk_pop__EXP_(idx);
    _stk_peek_EXP_(ini);
    us1 = _ag_get_TAB_SIZ_(idx);
    ctr = _ag_get_NBU_(dim);
    if (us1 == ctr)
      { if (siz < 0) 
          { res = _NGS_ERROR_; goto ERR; }; 
        if (siz == 0)
          { _stk_poke_EXP_(_EMP_);
            _stk_zap__CNT_(); }
        else
          { tab = _ag_make_TAB_(usz);
	        _stk_poke_EXP_(tab);
	        _stk_push_EXP_(ini);
	        _stk_push_EXP_(_ONE_);
	        _stk_push_EXP_(ini);
	        _stk_poke_CNT_(ID1);
	        _stk_push_CNT_(EXP); }
        return; }
    if (siz <= 0) 
      { res = _NPS_ERROR_; goto ERR; }; 
    tab = _ag_make_TAB_(usz);
    ndm = _ag_succ_NBR_(dim);
    exp = _ag_get_TAB_EXP_(idx, ctr+1);
    _stk_push_EXP_(idx);
    _stk_push_EXP_(tab);
    _stk_push_EXP_(dim);
    _stk_push_EXP_(_ONE_);
    _stk_push_EXP_(ini);
    _stk_push_EXP_(idx);
    _stk_push_EXP_(ndm);
    _stk_push_EXP_(exp);
    _stk_poke_CNT_(ID2);
    _stk_push_CNT_(IDX);
    _stk_push_CNT_(EXP);
    return;
  ERR:
    do 
      _stk_pop__EXP_(nam);
    while (!_ag_is_TXT_(nam));
    _error_nam_(res, nam); }

/*-----------------------------------------------------------------*/
/*  ID1                                                            */
/*     EXP: [... ... TAB INI NBR VAL] -> [... ... TAB INI NBR INI] */
/*     CNT: [... ... ... ... ... ID1] -> [... ... ... ... ID1 EXP] */
/*                                                                 */
/*     EXP: [... ... TAB INI NBR VAL] -> [... ... ... ... ... TAB] */
/*     CNT: [... ... ... ... ... ID1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ ID1(_NIL_TYPE_)
  { _EXP_TYPE_ ini, nbr, tab, val;
    _UNS_TYPE_ ctr, siz;
    _stk_claim_();
    _stk_pop__EXP_(val);
    _stk_pop__EXP_(nbr);
    _stk_pop__EXP_(ini);
    _stk_peek_EXP_(tab);
    siz = _ag_get_TAB_SIZ_(tab);
    ctr = _ag_get_NBU_(nbr);
    _ag_set_TAB_EXP_(tab, ctr, val);
    if (ctr == siz)
      { _stk_zap__CNT_(); 
        return; }
    nbr = _ag_succ_NBR_(nbr);
    _stk_push_EXP_(ini);
    _stk_push_EXP_(nbr);
    _stk_push_EXP_(ini);
    _stk_push_CNT_(EXP); }
  
/*-----------------------------------------------------------------*/
/*  ID2                                                            */
/*     EXP: [INI IDX TAB DIM NBR VAL] -> [INI IDX TAB DIM NBR      */
/*                                                INI IDX DIM EXP] */
/*     CNT: [... ... ... ... ... ID2] -> [... ... ... ID2 IDX EXP] */
/*                                                                 */
/*     EXP: [INI IDX TAB DIM NBR VAL] -> [... ... ... ... ... TAB] */
/*     CNT: [... ... ... ... ... ID2] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ ID2(_NIL_TYPE_)
  { _EXP_TYPE_ dim, exp, idx, ini, nbr, ndm, tab, val;
    _UNS_TYPE_ ctr, siz;
    _stk_claim_();
    _stk_pop__EXP_(val);
    _stk_pop__EXP_(nbr);
    _stk_pop__EXP_(dim);
    _stk_pop__EXP_(tab);
    _stk_pop__EXP_(idx);
    ctr = _ag_get_NBU_(nbr);
    _ag_set_TAB_EXP_(tab, ctr, val);
    siz = _ag_get_TAB_SIZ_(tab);
    if (ctr == siz)
      { _stk_poke_EXP_(tab);
        _stk_zap__CNT_(); 
        return; }
    _stk_peek_EXP_(ini);
    nbr = _ag_succ_NBR_(nbr);
    ndm = _ag_succ_NBR_(dim);
    ctr = _ag_get_NBU_(ndm);
    exp = _ag_get_TAB_EXP_(idx, ctr);
    _stk_push_EXP_(idx);
    _stk_push_EXP_(tab);
    _stk_push_EXP_(dim);
    _stk_push_EXP_(nbr);
    _stk_push_EXP_(ini);
    _stk_push_EXP_(idx);
    _stk_push_EXP_(ndm);
    _stk_push_EXP_(exp);
    _stk_poke_CNT_(ID2);
    _stk_push_CNT_(IDX);
    _stk_push_CNT_(EXP); }
       
/*-----------------------------------------------------------------*/
/*  QUA                                                            */
/*     EXP: [... ... ... ... REF DCT] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... QUA] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ... ... APL DCT] -> [... ... ... ... FUN TAB] */
/*     CNT: [... ... ... ... ... QUA] -> [... ... ... ... ... CLL] */
/*                                                                 */
/*     EXP: [... ... ... ... APL DCT] -> [... ... ... ... FUN EXP] */
/*     CNT: [... ... ... ... ... QUA] -> [... ... ... ... CLL EXP] */
/*                                                                 */
/*     EXP: [... ... ... ... APL DCT] -> [... ... ... ... NAT TAB] */
/*     CNT: [... ... ... ... ... QUA] -> [... ... ... ... ... NAT] */
/*                                                                 */
/*     EXP: [... ... ... ... APL DCT] -> [... ... ... ... NAT EXP] */
/*     CNT: [... ... ... ... ... QUA] -> [... ... ... ... NAT EXP] */
/*                                                                 */
/*     EXP: [... ... ... ... TBL DCT] -> [... ... IDX *1* TAB EXP] */
/*     CNT: [... ... ... ... ... QUA] -> [... ... ... ... TB1 EXP] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ QUA(_NIL_TYPE_)
  { _EXP_TYPE_ arg, dct, exp, fun, idx, inv, nam, ref, tab, val;  
    _TAG_TYPE_ tag;
    _RES_TYPE_ res;
    _UNS_TYPE_ siz;
    _stk_claim_();
    _stk_pop__EXP_(dct);
    _stk_peek_EXP_(inv);
    if (!_ag_is_DCT_(dct))
      { res = _NAD_ERROR_; goto ERR; };
    tag = _ag_get_TAG_(inv);
    switch (tag)
      { case _REF_TAG_:
          nam = _ag_get_REF_NAM_(inv);
          dct = _dct_locate_cst_(nam, dct);
          val = _ag_get_DCT_VAL_(dct);
          _stk_poke_EXP_(val);
          _stk_zap__CNT_(); 
          return;
        case _APL_TAG_:
          ref = _ag_get_APL_EXP_(inv);
          arg = _ag_get_APL_ARG_(inv);
          if(!_ag_is_REF_(ref))
            { res = _AGR_ERROR_; goto ERR; }
          nam = _ag_get_REF_NAM_(ref);
          dct = _dct_locate_cst_(nam, dct);
          fun = _ag_get_DCT_VAL_(dct);
          _stk_poke_EXP_(fun);
          _stk_push_EXP_(arg);
          switch (_ag_get_TAG_(fun))
            { case _FUN_TAG_:
                _stk_poke_CNT_(CLL);
                break;
              case _NAT_TAG_:
                _stk_poke_CNT_(NAT);
                break; 
              default:
                { res = _NAF_ERROR_; goto ERR; } }
          if (!_ag_is_TAB_(arg))
            _stk_push_CNT_(EXP); 
          return;
        case _TBL_TAG_:
          ref = _ag_get_TBL_EXP_(inv);
          idx = _ag_get_TBL_IDX_(inv);
          if(!_ag_is_REF_(ref))
            { res = _AGR_ERROR_; goto ERR; }
          nam = _ag_get_REF_NAM_(ref);
          if (!_ag_is_TAB_(idx))
            { res = _TRI_ERROR_; goto ERR; };
          siz = _ag_get_TAB_SIZ_(idx);
          if (siz == 0)
            { res = _ZTI_ERROR_; goto ERR; };
          dct = _dct_locate_cst_(nam, dct);
          tab = _ag_get_DCT_VAL_(dct);
          exp = _ag_get_TAB_EXP_(idx, 1);
          _stk_poke_EXP_(idx);
          _stk_push_EXP_(_ONE_);
          _stk_push_EXP_(tab);
          _stk_push_EXP_(exp);
          _stk_poke_CNT_(TB1);
          _stk_push_CNT_(EXP); 
          return; }
    res = _QRO_ERROR_;
    goto ERR;							
 ERR:
    _error_nam_(res, nam); }

/*-----------------------------------------------------------------*/
/*  QUO                                                            */
/*     EXP: [... ... ... ... ... NBR] -> [... ... ... ... ... NBR] */
/*     CNT: [... ... ... ... ... QUO] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ... ... ... FRC] -> [... ... ... ... ... FRC] */
/*     CNT: [... ... ... ... ... QUO] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ... ... ... TXT] -> [... ... ... ... ... TXT] */
/*     CNT: [... ... ... ... ... QUO] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ... ... ... VOI] -> [... ... ... ... ... VOI] */
/*     CNT: [... ... ... ... ... QUO] -> [... ... ... ... ... ...] */
/*                                                                 */
/*     EXP: [... ... ... ... ... APL] -> [... ... APL APL *1* EXP] */
/*     CNT: [... ... ... ... ... QUO] -> [... ... ... ... QU1 QUO] */
/*                                                                 */
/*     EXP: [... ... ... ... ... CST] -> [... ... CST CST *1* EXP] */
/*     CNT: [... ... ... ... ... QUO] -> [... ... ... ... QU1 QUO] */
/*                                                                 */
/*     EXP: [... ... ... ... ... CNT] -> [... ... CNT CNT *1* EXP] */
/*     CNT: [... ... ... ... ... QUO] -> [... ... ... ... QU1 QUO] */
/*                                                                 */
/*     EXP: [... ... ... ... ... DCL] -> [... ... DCL DCL *1* EXP] */
/*     CNT: [... ... ... ... ... QUO] -> [... ... ... ... QU1 QUO] */
/*                                                                 */
/*     EXP: [... ... ... ... ... DEF] -> [... ... DEF DEF *1* EXP] */
/*     CNT: [... ... ... ... ... QUO] -> [... ... ... ... QU1 QUO] */
/*                                                                 */
/*     EXP: [... ... ... ... ... FUN] -> [... ... FUN FUN *1* EXP] */
/*     CNT: [... ... ... ... ... QUO] -> [... ... ... ... QU1 QUO] */
/*                                                                 */
/*     EXP: [... ... ... ... ... NAT] -> [... ... NAT NAT *1* EXP] */
/*     CNT: [... ... ... ... ... QUO] -> [... ... ... ... QU1 QUO] */
/*                                                                 */
/*     EXP: [... ... ... ... ... QUA] -> [... ... QUA QUA *1* EXP] */
/*     CNT: [... ... ... ... ... QUO] -> [... ... ... ... QU1 QUO] */
/*                                                                 */
/*     EXP: [... ... ... ... ... REF] -> [... ... REF REF *1* EXP] */
/*     CNT: [... ... ... ... ... QUO] -> [... ... ... ... QU1 QUO] */
/*                                                                 */
/*     EXP: [... ... ... ... ... SET] -> [... ... SET SET *1* EXP] */
/*     CNT: [... ... ... ... ... QUO] -> [... ... ... ... QU1 QUO] */
/*                                                                 */
/*     EXP: [... ... ... ... ... TAB] -> [... ... TAB TAB *1* EXP] */
/*     CNT: [... ... ... ... ... QUO] -> [... ... ... ... QU1 QUO] */
/*                                                                 */
/*     EXP: [... ... ... ... ... TBL] -> [... ... TBL TBL *1* EXP] */
/*     CNT: [... ... ... ... ... QUO] -> [... ... ... ... QU1 QUO] */
/*                                                                 */
/*     EXP: [... ... ... ... ... VAR] -> [... ... VAR VAR *1* EXP] */
/*     CNT: [... ... ... ... ... QUO] -> [... ... ... ... QU1 QUO] */
/*                                                                 */
/*     EXP: [... ... ... ... ... QUO] -> [... ... ... ... ... EXP] */
/*     CNT: [... ... ... ... ... QUO] -> [... ... ... ... ... EXP] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ QUO(_NIL_TYPE_)
  { _EXP_TYPE_ exp, ex1;  
    _TAG_TYPE_ tag;
    _UNS_TYPE_ siz;
    _stk_claim_();
    _stk_peek_EXP_(exp);
    if (_ag_is_NBR_(exp))
      { _stk_zap__CNT_();
        return; }
    tag = _ag_get_TAG__(exp);
    switch (tag)
      { case _TAB_TAG_:
          siz = _ag_get_TAB_SIZ_(exp);
          if (siz == 0)
            { _stk_poke_EXP_(_EMP_);
              _stk_zap__CNT_();
              return; }
        case _APL_TAG_:
        case _CST_TAG_:
        case _CNT_TAG_:
        case _DCL_TAG_:
        case _DEF_TAG_:
        case _SET_TAG_:
        case _FUN_TAG_:
        case _QUA_TAG_:
        case _NAT_TAG_:
        case _REF_TAG_:
        case _TBL_TAG_:
        case _VAR_TAG_:
          siz = _mem_chunk_size_(exp);
          _mem_claim_SIZ_(siz);
          ex1 = _mem_make_chunk_(tag, siz);
          _stk_peek_EXP_(exp);
          exp = _mem_get_exp_(exp, 1);
          _stk_push_EXP_(ex1);
          _stk_push_EXP_(_ONE_);
          _stk_push_EXP_(exp);
          _stk_poke_CNT_(QU1);
          _stk_push_CNT_(QUO);
          return;
        case _QUO_TAG_:
			    exp = _ag_get_QUO_EXP_(exp);
			    _stk_poke_EXP_(exp);
          _stk_poke_CNT_(EXP);
          return;
        case _TXT_TAG_:
        case _FRC_TAG_:
        case _VOI_TAG_:
          _stk_zap__CNT_();
          return; 
        default:
          _error_exp_(_AGR_ERROR_, exp); }}

/*-----------------------------------------------------------------*/
/*  QU1                                                            */
/*     EXP: [... ... EX1 EX2 POS EXP] -> [... ... EX1 EX2 POS EXP] */
/*     CNT: [... ... ... ... ... QU1] -> [... ... ... ... QU1 QUO] */
/*                                                                 */
/*     EXP: [... ... EX1 EX2 POS EXP] -> [... ... ... ... ... EX2] */
/*     CNT: [... ... ... ... ... QU1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ QU1(_NIL_TYPE_)
  { _EXP_TYPE_ exp, ex1, ex2, pos;  
    _UNS_TYPE_ siz, uns;
    _stk_pop__EXP_(exp);
    _stk_pop__EXP_(pos);
    _stk_pop__EXP_(ex2);
    _stk_peek_EXP_(ex1);
    siz = _mem_chunk_size_(ex1);
    uns = _ag_get_NBU_(pos);
    _mem_set_exp_(ex2, uns, exp);
    if (uns == siz)
      { _stk_poke_EXP_(ex2);
        _stk_zap__CNT_();
        return; }
    exp = _mem_get_exp_(ex1, ++uns);
    pos = _ag_make_NBR_(uns);
    _stk_push_EXP_(ex2);   
    _stk_push_EXP_(pos);   
    _stk_push_EXP_(exp);   
    _stk_push_CNT_(QUO); }
        
/*-----------------------------------------------------------------*/
/*  RET                                                            */
/*     EXP: [... ... ... ... DCT VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... RET] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ RET(_NIL_TYPE_)
  { _EXP_TYPE_ val;
    _stk_pop__EXP_(val);
    _stk_peek_EXP_(_DCT_);
    _stk_poke_EXP_(val);
    _stk_zap__CNT_(); 
    _ESCAPE_; }

/*-----------------------------------------------------------------*/
/*  SET                                                            */
/*     EXP: [... ... ... ... DCT VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... SET] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ SET(_NIL_TYPE_)
  { _EXP_TYPE_ dct, val;
    _stk_pop__EXP_(val);
    _stk_peek_EXP_(dct);
    _ag_set_DCT_VAL_(dct, val);
    _stk_poke_EXP_(val); 
    _stk_zap__CNT_();
    return; }

/*-----------------------------------------------------------------*/
/*  SE1                                                            */
/*     EXP: [... ... ... EXP IDX TAB] -> [... EXP IDX *1* TAB EXP] */
/*     CNT: [... ... ... ... ... SE1] -> [... ... ... ... SE2 EXP] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ SE1(_NIL_TYPE_)
  { _EXP_TYPE_ exp, idx, tab;
    _UNS_TYPE_ siz;
    _stk_claim_();
    _stk_pop__EXP_(tab);
    if (!_ag_is_TAB_(tab))
      _error_exp_(_NAT_ERROR_, tab);
    _stk_peek_EXP_(idx);
    if (!_ag_is_TAB_(idx))
      _error_exp_(_TRI_ERROR_, idx);
    siz = _ag_get_TAB_SIZ_(idx);
    if (siz == 0)
      _error_exp_(_ZTI_ERROR_, idx);
    exp = _ag_get_TAB_EXP_(idx, 1);
    _stk_push_EXP_(_ONE_);
    _stk_push_EXP_(tab);
    _stk_push_EXP_(exp);
    _stk_poke_CNT_(SE2);
    _stk_push_CNT_(EXP); }


/*-----------------------------------------------------------------*/
/*  SE2                                                            */
/*     EXP: [... EXP IDX NBR TAB VAL] -> [... EXP IDX NBR TAB EXP] */
/*     CNT: [... ... ... ... ... SE2] -> [... ... ... ... SE2 EXP] */
/*                                                                 */
/*     EXP: [... EXP IDX NBR TAB VAL] -> [... ... ... TAB NBR EXP] */
/*     CNT: [... ... ... ... ... SE2] -> [... ... ... ... SE3 EXP] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ SE2(_NIL_TYPE_)
  { _EXP_TYPE_ exp, idx, nbr, tab, val;
    _UNS_TYPE_ ctr, siz;
    _SGN_TYPE_ pos;
    _stk_claim_();
    _stk_pop__EXP_(val);
    _stk_pop__EXP_(tab);
    _stk_pop__EXP_(nbr);
    _stk_peek_EXP_(idx);
    if (!_ag_is_TAB_(tab))
      _error_exp_(_NAT_ERROR_, tab);
    siz = _ag_get_TAB_SIZ_(tab);
    if (!_ag_is_NBR_(val))
      _error_exp_(_NNI_ERROR_, val);
    pos = _ag_get_NBR_(val);
    if (pos <= 0)
      _error_exp_(_NPI_ERROR_, val);
    if (pos > siz)
      _error_exp_(_IBS_ERROR_, val);
    ctr = _ag_get_NBU_(nbr);
    siz = _ag_get_TAB_SIZ_(idx);
    if (ctr == siz)
      { _stk_zap__EXP_(); 
        _stk_peek_EXP_(exp);
        _stk_poke_EXP_(tab);
        _stk_push_EXP_(val);
        _stk_push_EXP_(exp);
        _stk_poke_CNT_(SE3); 
        _stk_push_CNT_(EXP);
        return; }
    nbr = _ag_succ_NBR_(nbr);
    val = _ag_get_TAB_EXP_(tab, pos);
    exp = _ag_get_TAB_EXP_(idx, ctr+1);
    _stk_push_EXP_(nbr);
    _stk_push_EXP_(val);
    _stk_push_EXP_(exp);
    _stk_push_CNT_(EXP); }

/*-----------------------------------------------------------------*/
/*  SE3                                                            */
/*     EXP: [... ... ... TAB NBR VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... SE3] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ SE3(_NIL_TYPE_)
  { _EXP_TYPE_ nbr, tab, val;
    _UNS_TYPE_ ctr;
    _stk_pop__EXP_(val);
    _stk_pop__EXP_(nbr);
    _stk_peek_EXP_(tab);
    ctr = _ag_get_NBU_(nbr);
    _ag_set_TAB_EXP_(tab, ctr, val);
    _stk_poke_EXP_(val);
    _stk_zap__CNT_(); }   

/*-----------------------------------------------------------------*/
/*  TBL                                                            */
/*     EXP: [... ... ... ... IDX TAB] -> [... ... IDX *1* TAB EXP] */
/*     CNT: [... ... ... ... ... TBL] -> [... ... ... ... TB1 EXP] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ TBL(_NIL_TYPE_)
  { _EXP_TYPE_ exp, idx, tab;
    _UNS_TYPE_ siz;
    _stk_claim_();
    _stk_pop__EXP_(tab);
    if (!_ag_is_TAB_(tab))
      _error_exp_(_NAT_ERROR_, tab);
    _stk_peek_EXP_(idx);
    if (!_ag_is_TAB_(idx))
      _error_exp_(_TRI_ERROR_, idx);
    siz = _ag_get_TAB_SIZ_(idx);
    if (siz == 0)
      _error_exp_(_ZTI_ERROR_, idx);
    exp = _ag_get_TAB_EXP_(idx, 1);
    _stk_push_EXP_(_ONE_);
    _stk_push_EXP_(tab);
    _stk_push_EXP_(exp);
    _stk_poke_CNT_(TB1);
    _stk_push_CNT_(EXP); }
  
/*-----------------------------------------------------------------*/
/*  TB1                                                            */
/*     EXP: [... ... IDX NBR TAB VAL] -> [... ... IDX NBR TAB EXP] */
/*     CNT: [... ... ... ... ... TB1] -> [... ... ... ... TB1 EXP] */
/*                                                                 */
/*     EXP: [... ... IDX NBR TAB VAL] -> [... ... ... ... ... VAL] */
/*     CNT: [... ... ... ... ... TB1] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ TB1(_NIL_TYPE_)
  { _EXP_TYPE_ exp, idx, nbr, tab, val;
    _UNS_TYPE_ ctr, siz;
    _SGN_TYPE_ pos;
    _stk_claim_();
    _stk_pop__EXP_(val);
    _stk_pop__EXP_(tab);
    _stk_pop__EXP_(nbr);
    _stk_peek_EXP_(idx);
    if (!_ag_is_TAB_(tab))
      _error_exp_(_NAT_ERROR_, tab);
    siz = _ag_get_TAB_SIZ_(tab);
    if (!_ag_is_NBR_(val))
      _error_exp_(_NNI_ERROR_, val);
    pos = _ag_get_NBR_(val);
    if (pos <= 0)
      _error_exp_(_NPI_ERROR_, val);
    if (pos > siz)
      _error_exp_(_IBS_ERROR_, val);
    val = _ag_get_TAB_EXP_(tab, pos);
    ctr = _ag_get_NBU_(nbr);
    siz = _ag_get_TAB_SIZ_(idx);
    if (ctr == siz)
      { _stk_poke_EXP_(val);
        _stk_zap__CNT_(); 
        return; }
    nbr = _ag_succ_NBR_(nbr);
    exp = _ag_get_TAB_EXP_(idx, ctr+1);
    _stk_push_EXP_(nbr);
    _stk_push_EXP_(val);
    _stk_push_EXP_(exp);
    _stk_push_CNT_(EXP); }

/*----------------------- public variables ------------------------*/

_CNT_TYPE_ _eva_EXP_ = EXP;
_CNT_TYPE_ _eva_CAL_ = CLL;
