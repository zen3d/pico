             /*-----------------------------------*/
             /*           >>>Pico 2.0<<<          */
             /*            Theo D'Hondt           */
             /*   VUB Programming Technology Lab  */
             /*             (c) 2002              */
             /*-----------------------------------*/
             /*             Contexts              */
             /*-----------------------------------*/

#include "Pico.h"
#include "PicoMAI.h"
#include "PicoCNT.h"
#include "PicoMEM.h"
#include "PicoNAT.h"

/*-------------------- private variables -------------------------*/

static _SIZ_TYPE_ NAM_top = 1;
static _SIZ_TYPE_ NAM_siz = 0;

/*--------------------- public variables -------------------------*/

_EXP_TYPE_ _DCT_;
_UNS_TYPE_ _CNT_;
_UNS_TYPE_ _EXP_;
_EXP_TYPE_ _STK_;
                           
/*--------------------- public functions -------------------------*/

_EXP_TYPE_ _dct_locate_(const _EXP_TYPE_ Nam, 
                        const _EXP_TYPE_ Dct)
  { _EXP_TYPE_ dct;
    dct = Dct;
    for (;;)
      { if (_mem_is_same_(dct, _NAT_))
          return _nat_in_cache(Nam); 
        switch (_mem_get_tag_(dct))
          { case _VOI_TAG_:
              _error_nam_(_UDI_ERROR_, Nam);
            case _VAR_TAG_:
            case _CST_TAG_:
              if (_mem_is_same_(Nam, _ag_get_DCT_NAM_(dct)))
                return dct;
              dct = _ag_get_DCT_DCT_(dct);
              continue; }}}
      
_EXP_TYPE_ _dct_locate_cst_(const _EXP_TYPE_ Nam, 
                            const _EXP_TYPE_ Dct)
  { _EXP_TYPE_ dct;
    dct = Dct;
    for (;;)
      { if (_mem_is_same_(dct, _NAT_))
          return _nat_in_cache(Nam);
        switch (_mem_get_tag_(dct))
          { case _VOI_TAG_:
              _error_nam_(_UDI_ERROR_, Nam);
            case _VAR_TAG_:
              if (_mem_is_same_(Nam, _ag_get_DCT_NAM_(dct)))
                _error_nam_(_NAC_ERROR_, Nam);
              dct = _ag_get_DCT_DCT_(dct);
              continue;
            case _CST_TAG_:
              if (_mem_is_same_(Nam, _ag_get_DCT_NAM_(dct)))
                return dct;
              dct = _ag_get_DCT_DCT_(dct);
              continue; }}}
      
_NIL_TYPE_ _CNT_setup_(const _SIZ_TYPE_ Siz)
  { _NAM_ = _ag_make_TAB_(Siz);
    NAM_top = 1;
    NAM_siz = Siz; }
  
_EXP_TYPE_ _CNT_set_NAM_(const _STR_TYPE_ Str)
  { _EXP_TYPE_ nam;
    if (NAM_top > NAM_siz)
      _error_(_TMN_ERROR_);
    _mem_claim_STR_(Str);
    nam = _ag_make_TXT_(Str);
    _ag_set_TAB_EXP_(_NAM_, NAM_top++, nam);
    return nam; }
   
_EXP_TYPE_ _CNT_make_NAM_(const _STR_TYPE_ Str)
  { _SIZ_TYPE_ idx;
    _EXP_TYPE_ nam;
    for (idx = 1; idx < NAM_top; idx++)
      { nam = _ag_get_TAB_EXP_(_NAM_, idx);
        if (!strcmp(Str, _ag_get_TXT_(nam)))
          return nam; }
    return _CNT_set_NAM_(Str); }
   
_EXP_TYPE_ _CNT_initialise_(_NIL_TYPE_)
  { _EXP_TYPE_ cnt, top, stk;
    _mem_claim_SIZ_(_STK_INIT_);
    stk = _ag_make_TAB_(_STK_INIT_);
    top = _ag_make_NBR_(_STK_INIT_ + 1);
    cnt = _ag_make_CNT_(_DCT_, _ZRO_, top, stk);
    return cnt; }

_NIL_TYPE_ _CNT_capture_(const _EXP_TYPE_ Cnt)
  { _EXP_TYPE_ bot, top;
    _UNS_TYPE_ adr, lwn, upn;
    adr = (_UNS_TYPE_)_mem_cast_raw_(_STK_, _TAB_TAG_);
    lwn = 1 + (_EXP_ - adr)/_EXP_SIZE_;
    upn = 1 + (_CNT_ - adr)/_EXP_SIZE_;
    bot = _ag_make_NBR_(lwn);                  
    top = _ag_make_NBR_(upn);                  
    _ag_set_CNT_DCT_(Cnt, _DCT_); 
    _ag_set_CNT_BOT_(Cnt, bot);
    _ag_set_CNT_TOP_(Cnt, top);
    _ag_set_CNT_TAB_(Cnt, _STK_); }

_NIL_TYPE_ _CNT_restore_(const _EXP_TYPE_ Cnt)
  { _EXP_TYPE_ bot, top;
    _UNS_TYPE_ adr, lwn, upn;
    _DCT_ = _ag_get_CNT_DCT_(Cnt); 
    bot   = _ag_get_CNT_BOT_(Cnt);
    top   = _ag_get_CNT_TOP_(Cnt);
    _STK_ = _ag_get_CNT_TAB_(Cnt);
    lwn   = _ag_get_NBU_(bot);
    upn   = _ag_get_NBU_(top);
    adr   = (_UNS_TYPE_)_mem_cast_raw_(_STK_, _TAB_TAG_);
    _EXP_ = adr + (lwn - 1)*_EXP_SIZE_; 
    _CNT_ = adr + (upn - 1)*_EXP_SIZE_; }

_NIL_TYPE_ _CNT_expand_(_NIL_TYPE_)
  { _EXP_TYPE_ exp, stk;
    _UNS_TYPE_ adr, idx, lwn, siz, upn, xsz;
    siz = _ag_get_TAB_SIZ_(_STK_); 
    xsz = 2*siz;
    if (xsz > _MAX_)
      _error_(_STK_ERROR_); 
    _mem_claim_SIZ_(xsz);
    adr = (_UNS_TYPE_)_mem_cast_raw_(_STK_, _TAB_TAG_);
    lwn = 1 + (_EXP_ - adr)/_EXP_SIZE_;
    upn = 1 + (_CNT_ - adr)/_EXP_SIZE_;
    stk = _ag_make_TAB_(xsz);
    for (idx = 1 ; idx <= lwn ; idx++)
      { exp = _ag_get_TAB_EXP_(_STK_, idx);
        _ag_set_TAB_EXP_(stk, idx, exp); }
    for (idx = siz ; idx >= upn ; idx--)
      { exp = _ag_get_TAB_EXP_(_STK_, idx);
        _ag_set_TAB_EXP_(stk, xsz--, exp); }
    for ( ; xsz > lwn ; xsz--)
      _ag_set_TAB_EXP_(stk, xsz, _VOI_);
    adr   = (_UNS_TYPE_)_mem_cast_raw_(stk, _TAB_TAG_);
    _EXP_ = adr + (lwn - 1)*_EXP_SIZE_; 
    _CNT_ = adr + (upn + siz - 1)*_EXP_SIZE_; 
    _STK_ = stk; }

_UNS_TYPE_ _CNT_size_(_NIL_TYPE_)
  { _UNS_TYPE_ siz;
    siz = _ag_get_TAB_SIZ_(_STK_);
    return siz; }

_NIL_TYPE_ _CNT_save_(const _EXP_TYPE_ Cnt)
  { _EXP_TYPE_ bot, exp, stk, top; 
    _UNS_TYPE_ adr, idx, lwn, siz, upn;
    siz = _ag_get_TAB_SIZ_(_STK_);
    // _mem_claim_SIZ_(siz); // dvd: _mem_claim_SIZ_ in CLL in PicoNAT.c
                             //      should do the job
    adr = (_UNS_TYPE_)_mem_cast_raw_(_STK_, _TAB_TAG_);
    lwn = 1 + (_EXP_ - adr)/_EXP_SIZE_;
    upn = 1 + (_CNT_ - adr)/_EXP_SIZE_;
    bot = _ag_make_NBR_(lwn);                  
    top = _ag_make_NBR_(upn);                  
    stk = _ag_make_TAB_(siz);
    for (idx = 1 ; idx <= siz ; idx++)
      { exp = _ag_get_TAB_EXP_(_STK_, idx);
        _ag_set_TAB_EXP_(stk, idx, exp); }
    _ag_set_CNT_DCT_(Cnt, _DCT_);
    _ag_set_CNT_BOT_(Cnt, bot);
    _ag_set_CNT_TOP_(Cnt, top);
    _ag_set_CNT_TAB_(Cnt, stk); }
   
_NIL_TYPE_ _CNT_load_(const _EXP_TYPE_ Cnt)
  { _EXP_TYPE_ bot, exp, stk, top; 
    _UNS_TYPE_ adr, idx, lwn, siz, upn;
    _DCT_ = _ag_get_CNT_DCT_(Cnt); 
    bot   = _ag_get_CNT_BOT_(Cnt);
    top   = _ag_get_CNT_TOP_(Cnt);
    stk   = _ag_get_CNT_TAB_(Cnt);
    siz   = _ag_get_TAB_SIZ_(stk);
    // _mem_claim_SIZ_(siz); // dvd: may cause corruption: cfr CO2 thunk
    _STK_ = _ag_make_TAB_(siz);
    for (idx = 1 ; idx <= siz ; idx++)
      { exp = _ag_get_TAB_EXP_(stk, idx);
        _ag_set_TAB_EXP_(_STK_, idx, exp); }
    lwn   = _ag_get_NBU_(bot);
    upn   = _ag_get_NBU_(top);
    adr   = (_UNS_TYPE_)_mem_cast_raw_(_STK_, _TAB_TAG_);
    _EXP_ = adr + (lwn - 1)*_EXP_SIZE_; 
    _CNT_ = adr + (upn - 1)*_EXP_SIZE_; }
    
