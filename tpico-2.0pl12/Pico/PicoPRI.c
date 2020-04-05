             /*-----------------------------------*/
             /*           >>>Pico 2.0<<<          */
             /*            Theo D'Hondt           */
             /*   VUB Programming Technology Lab  */
             /*             (c) 2002              */
             /*-----------------------------------*/
             /*              Printing             */
             /*-----------------------------------*/

#include "Pico.h"
#include "PicoMAI.h"
#include "PicoCNT.h"
#include "PicoMEM.h"
#include "PicoNAT.h"
#include "PicoPRI.h"
                    
/*---------------------- private constants ------------------------*/

#define CNT_str "<continuation>"
#define DCT_str "<dictionary>"
#define DCL_str "<declaration %s>"
#define DEF_str "<definition %s>"
#define FRC_str "%g"
#define FUN_str "<function %s>"
#define NAT_str "<native %s>"
#define NBR_str "%i"
#define QUA_str "<qualification>"
#define REF_str "<reference %s>"
#define SET_str "<assignment %s>"
#define TAB_str "<table> "
#define TBL_str "<tabulation>"
#define VOI_str "<void>"

/*-------------------- private prototypes -------------------------*/

static _NIL_TYPE_ print_APL(const _EXP_TYPE_);
static _NIL_TYPE_ print_CST(const _EXP_TYPE_);
static _NIL_TYPE_ print_CNT(const _EXP_TYPE_);
static _NIL_TYPE_ print_DCL(const _EXP_TYPE_);
static _NIL_TYPE_ print_DEF(const _EXP_TYPE_);
static _NIL_TYPE_ print_EXP(const _EXP_TYPE_);
static _NIL_TYPE_ print_FRC(const _EXP_TYPE_);
static _NIL_TYPE_ print_FUN(const _EXP_TYPE_);
static _NIL_TYPE_ print_NAT(const _EXP_TYPE_);
static _NIL_TYPE_ print_NBR(const _EXP_TYPE_);
static _NIL_TYPE_ print_QUA(const _EXP_TYPE_);
static _NIL_TYPE_ print_QUO(const _EXP_TYPE_);
static _NIL_TYPE_ print_REF(const _EXP_TYPE_);
static _NIL_TYPE_ print_SET(const _EXP_TYPE_);
static _NIL_TYPE_ print_TAB(const _EXP_TYPE_);
static _NIL_TYPE_ print_TBL(const _EXP_TYPE_);
static _NIL_TYPE_ print_TXT(const _EXP_TYPE_);
static _NIL_TYPE_ print_VAR(const _EXP_TYPE_);
static _NIL_TYPE_ print_VOI(const _EXP_TYPE_);

/*--------------------- private functions -------------------------*/

static _NIL_TYPE_ print_APL(const _EXP_TYPE_ apl)
  { _EXP_TYPE_ exp, arg;
    _UNS_TYPE_ cnt, siz;
    exp = _ag_get_APL_EXP_(apl);
    arg = _ag_get_APL_ARG_(apl);
    print_EXP(exp); 
    if(_ag_is_TAB_(arg))
		  { _print_("("); 
		    siz = _ag_get_TAB_SIZ_(arg);
		    if(siz > 0)
		      { exp = _ag_get_TAB_EXP_(arg, 1);
		        print_EXP(exp);
		        for (cnt = 2; cnt <= siz; cnt++)
		          { exp = _ag_get_TAB_EXP_(arg, cnt);
		            _print_(","); 
		            print_EXP(exp); }}
		    _print_(")"); }
		else
		  { _print_("@"); 
		    print_EXP(arg); }}

static _NIL_TYPE_ print_CST(const _EXP_TYPE_ dct)
  { _print_(DCT_str); }
                               
static _NIL_TYPE_ print_DCL(const _EXP_TYPE_ dcl)
  { _EXP_TYPE_ exp, inv;
    inv = _ag_get_DCL_INV_(dcl);
    exp = _ag_get_DCL_EXP_(dcl);
    print_EXP(inv);
    _print_(":: ");
    print_EXP(exp); }
  
static _NIL_TYPE_ print_DEF(const _EXP_TYPE_ def)
  { _EXP_TYPE_ exp, inv;
    inv = _ag_get_DEF_INV_(def);
    exp = _ag_get_DEF_EXP_(def);
    print_EXP(inv);
    _print_(": ");
    print_EXP(exp); }
  
static _NIL_TYPE_ print_CNT(const _EXP_TYPE_ CNT)
  { _print_(CNT_str); }

static _NIL_TYPE_ print_FRC(const _EXP_TYPE_ frc)
  { _FLO_TYPE_ flo;
    flo = _ag_get_FRC_(frc);
    sprintf(_BUF_, FRC_str, flo);
    _print_(_BUF_); }
                               
static _NIL_TYPE_ print_EXP(const _EXP_TYPE_ exp)
  { if (_ag_is_NBR_(exp))
      { print_NBR(exp);
        return; }
    switch (_ag_get_TAG__(exp))
      { case _APL_TAG_:
          print_APL(exp);
          return;
        case _CST_TAG_:
          print_CST(exp);
          return;
        case _CNT_TAG_:
          print_CNT(exp);
          return;
        case _DCL_TAG_:
          print_DCL(exp);
          return;
        case _DEF_TAG_:
          print_DEF(exp);
          return;
        case _FRC_TAG_:
          print_FRC(exp);
          return;
        case _FUN_TAG_:
          print_FUN(exp);
          return;
        case _QUA_TAG_:
          print_QUA(exp);
          return;
        case _QUO_TAG_:
          print_QUO(exp);
          return;
        case _NAT_TAG_:
          print_NAT(exp);
          return;
        case _REF_TAG_:
          print_REF(exp);
          return;
        case _TAB_TAG_:
          print_TAB(exp);
          return;
        case _TBL_TAG_:
          print_TBL(exp);
          return;
        case _TXT_TAG_:
          print_TXT(exp);
          return;
        case _SET_TAG_:
          print_SET(exp);
          return;
        case _VAR_TAG_:
          print_VAR(exp);
          return;
        case _VOI_TAG_:
          print_VOI(exp);
          return; }}

static _NIL_TYPE_ print_FUN(const _EXP_TYPE_ fun)
  { _EXP_TYPE_ nam;
    _STR_TYPE_ str;
    nam = _ag_get_FUN_NAM_(fun);
    str = _ag_get_TXT_(nam);
    sprintf(_BUF_, FUN_str, str);
    _print_(_BUF_); }
                                
static _NIL_TYPE_ print_NAT(const _EXP_TYPE_ nat)
  { _EXP_TYPE_ nam;
    _STR_TYPE_ str;
    nam = _ag_get_NAT_NAM_(nat);
    str = _ag_get_TXT_(nam);
    sprintf(_BUF_, NAT_str, str);
    _print_(_BUF_); }

static _NIL_TYPE_ print_NBR(const _EXP_TYPE_ nbr)
  { _SGN_TYPE_ sgn;
    sgn = _ag_get_NBR_(nbr);
    sprintf(_BUF_, NBR_str, sgn);
    _print_(_BUF_); }

static _NIL_TYPE_ print_QUA(const _EXP_TYPE_ qua)
  { _EXP_TYPE_ exp, inv;
    exp = _ag_get_QUA_EXP_(qua);
    inv = _ag_get_QUA_INV_(qua);
    print_EXP(exp);
    _print_(".");
    print_EXP(inv); }
                                
static _NIL_TYPE_ print_QUO(const _EXP_TYPE_ qua)
  { _EXP_TYPE_ exp;
    exp = _ag_get_QUO_EXP_(qua);
    _print_("'");
    print_EXP(exp); }
                                
static _NIL_TYPE_ print_REF(const _EXP_TYPE_ ref)
  { _EXP_TYPE_ nam;
    _STR_TYPE_ str;
    nam = _ag_get_REF_NAM_(ref);
    str = _ag_get_TXT_(nam);
    _print_(str); }

static _NIL_TYPE_ print_SET(const _EXP_TYPE_ set)
  { _EXP_TYPE_ exp, inv;
    inv = _ag_get_SET_INV_(set);
    exp = _ag_get_SET_EXP_(set);
    print_EXP(inv);
    _print_(":= ");
    print_EXP(exp); }
   
static _NIL_TYPE_ print_TAB(const _EXP_TYPE_ tab)
  { _print_(TAB_str); }
                               
static _NIL_TYPE_ print_TBL(const _EXP_TYPE_ tbl)
  { _EXP_TYPE_ exp, idx;
    _UNS_TYPE_ cnt, siz;
    exp = _ag_get_TBL_EXP_(tbl);
    idx = _ag_get_TBL_IDX_(tbl);
    print_EXP(exp); 
    if(_ag_is_TAB_(idx))
		  { _print_("["); 
		    siz = _ag_get_TAB_SIZ_(idx);
		    if(siz > 0)
		      { exp = _ag_get_TAB_EXP_(idx, 1);
		        print_EXP(exp);
		        for (cnt = 2; cnt <= siz; cnt++)
		          { exp = _ag_get_TAB_EXP_(idx, cnt);
		            _print_(","); 
		            print_EXP(exp); }}
		    _print_("]"); }
		else
		  _error_(_AGR_ERROR_); }

static _NIL_TYPE_ print_TXT(const _EXP_TYPE_ txt)
  { _STR_TYPE_ str;
    str = _ag_get_TXT_(txt);
    _print_(str); }
      
static _NIL_TYPE_ print_VAR(const _EXP_TYPE_ var)
  { _print_(DCT_str); }
                               
static _NIL_TYPE_ print_VOI(const _EXP_TYPE_ voi)
  { _print_(VOI_str); }
       
/*-----------------------------------------------------------------*/
/*  PRI                                                            */
/*     EXP: [... ... ... ... ... EXP] -> [... ... ... ... ... ...] */
/*     CNT: [... ... ... ... ... PRI] -> [... ... ... ... ... ...] */
/*-----------------------------------------------------------------*/

static _NIL_TYPE_ PRI(_NIL_TYPE_)
  { _EXP_TYPE_ exp;
    _stk_pop__EXP_(exp);
    _stk_zap__CNT_();
    print_EXP(exp); }
                               
/*----------------------- public variables ------------------------*/

_CNT_TYPE_ _pri_EXP_ = PRI;

