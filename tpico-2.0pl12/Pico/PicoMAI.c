             /*-----------------------------------*/
             /*           >>>Pico 2.0<<<          */
             /*            Theo D'Hondt           */
             /*   VUB Programming Technology Lab  */
             /*             (c) 2002              */
             /*-----------------------------------*/
             /*           Main program            */
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

#ifdef PROFILE_ON
#include "Profiler.h"
#endif

/*---------------------- private constants ------------------------*/

enum { GLB_ROOT  = 1, 
       NAM_ROOT  = 2,
       CNT_ROOT  = 3,
       NAT_ROOT  = 4,
       RST_ROOT  = 5,
       ROOT_SIZE = 5 };
       
static const _UNS_TYPE_ RST_SIZE = 32;

static const _SES_TYPE_ UNUSED = (_SES_TYPE_)-1;       

/*---------------------- private variables ------------------------*/

static enum 
     { NO_INIT  = 0, 
       DISABLED = 1,
       ENABLED  = 2 } STATUS = NO_INIT;
static _BYT_TYPE_ SES_IDX;
static _DSC_TYPE_ SES_TAB[_MAX_SES_];
static _EXP_TYPE_ ROOT;
static _EXP_TYPE_ GLOBAL;
static _EXP_TYPE_ CNT;
static _SIZ_TYPE_ NAM;

/*---------------------- private macros ---------------------------*/

#define CNT_INITIALISE(IDX)\
  _ag_set_TAB_EXP_(CNT, IDX + 1, _CNT_initialise_())  

#define CNT_CAPTURE(IDX)\
  _CNT_capture_(_ag_get_TAB_EXP_(CNT, IDX + 1))
  
#define CNT_RESTORE(IDX)\
  _CNT_restore_(_ag_get_TAB_EXP_(CNT, IDX + 1))
  
#define CNT_GET(IDX)\
  _ag_get_TAB_EXP_(CNT, IDX + 1)
  
#define CNT_VOID(IDX)\
  _ag_set_TAB_EXP_(CNT, IDX + 1, _VOI_)
  
/*--------------------- private functions -------------------------*/

static _NIL_TYPE_ exit_loop(_NIL_TYPE_)
  { SES_TAB[SES_IDX].res = _PICO_DONE_;
    SES_TAB[SES_IDX].dct = _DCT_;
    SES_TAB[SES_IDX].esc = _ESC_;
    _stk_zap__CNT_();
    longjmp(SES_TAB[SES_IDX].exi, 1); }
   
static _NIL_TYPE_ do_loop(_EXI_TYPE_ * Exi)
  {
#ifdef PROFILE_ON
    ProfilerSetStatus(1);
#endif
    if (setjmp(*Exi) == 0) 
      for(;;) 
        _stk_call_CNT_(); 
#ifdef PROFILE_ON
    ProfilerSetStatus(0);
#endif
   }
  
/*----------------------- public variables ------------------------*/

_BUF_TYPE_ _BUF_;
_BYT_TYPE_ _DLY_;
_UNS_TYPE_ _MAX_;
_EXP_TYPE_ _NAM_;
_EXP_TYPE_ _NAT_;
_EXP_TYPE_ _RST_;

/*----------------------- public functions ------------------------*/

_NIL_TYPE_ _accept_(_NIL_TYPE_)
  { SES_TAB[SES_IDX].res = _DO_ACCEPT_;
    longjmp(SES_TAB[SES_IDX].exi, 1); }

_NIL_TYPE_ _load_(const _STR_TYPE_ Str)
  { SES_TAB[SES_IDX].res = _DO_LOAD_;
    STATUS = DISABLED;
    _PICO_MESSAGE_(SES_TAB[SES_IDX].ses, Str);
    STATUS = ENABLED;
    longjmp(SES_TAB[SES_IDX].exi, 1); }

_NIL_TYPE_ _error_(const _RES_TYPE_ Err)
  { CNT_CAPTURE(SES_IDX);
    CNT = CNT_GET(SES_IDX);
    SES_TAB[SES_IDX].res = Err;
    _DCT_ = SES_TAB[SES_IDX].dct;
    SES_TAB[SES_IDX].esc = _VOI_;
    CNT_INITIALISE(SES_IDX);
    CNT_RESTORE(SES_IDX);
    longjmp(SES_TAB[SES_IDX].exi, 1); }

_NIL_TYPE_ _reclaim_(const _UNS_TYPE_ Mrg)
  { static _BYT_TYPE_ res;
    _escape_();
    CNT_CAPTURE(SES_IDX);
    STATUS = DISABLED;
    _PICO_RECLAIM_(_RECLAIM_YES_);
    res = _mem_reclaim_(Mrg);
    _PICO_RECLAIM_(_RECLAIM_NO_);
    STATUS = ENABLED; 
    if (res)
      { CNT_RESTORE(SES_IDX); 
        GLOBAL = _ag_get_TAB_EXP_(ROOT, GLB_ROOT);
        _NAM_  = _ag_get_TAB_EXP_(ROOT, NAM_ROOT);
        CNT    = _ag_get_TAB_EXP_(ROOT, CNT_ROOT);
        _NAT_  = _ag_get_TAB_EXP_(ROOT, NAT_ROOT);
        _RST_  = _ag_get_TAB_EXP_(ROOT, RST_ROOT);
        if(_nat_reset_())
          return; }
    STATUS = NO_INIT;
    SES_TAB[SES_IDX].res = _MEM_ERROR_;
    longjmp(SES_TAB[SES_IDX].exi, 1); }

_EXP_TYPE_ _error_str_(const _RES_TYPE_ Err,
                       const _STR_TYPE_ Str)
  { STATUS = DISABLED;
    _PICO_MESSAGE_(SES_TAB[SES_IDX].ses, Str);
    STATUS = ENABLED;
    _error_(Err);
    return _VOI_; }

_NIL_TYPE_ _error_nam_(const _RES_TYPE_ Err,
                       const _EXP_TYPE_ Txt)
  { _error_str_(Err, _ag_get_TXT_(Txt)); }
 
_NIL_TYPE_ _error_exp_(const _RES_TYPE_ Err,
                       const _EXP_TYPE_ Exp )
  { _error_(Err); }
 
_NIL_TYPE_ _error_fun_(const _RES_TYPE_ Err,
                       const _EXP_TYPE_ Fun)
  { _error_nam_(Err, _ag_get_FUN_NAM_(Fun)); }
 
_NIL_TYPE_ _print_(const _STR_TYPE_ Str)
  { STATUS = DISABLED;
    _PICO_PRINT_(SES_TAB[SES_IDX].ses, Str);
    STATUS = ENABLED; }

_NIL_TYPE_ _mark_(const _POS_TYPE_ Sta,
                  const _POS_TYPE_ Sto)
  { STATUS = DISABLED;
    _PICO_MARK_(SES_TAB[SES_IDX].ses, Sta, Sto);
    STATUS = ENABLED; }

_NIL_TYPE_ _escape_(_NIL_TYPE_)
  { _ESC_TYPE_ esc;
    STATUS = DISABLED;
    esc = _PICO_ESCAPE_(SES_TAB[SES_IDX].ses);
    STATUS = ENABLED;
    if (esc == _ESCAPE_YES_)
      { SES_TAB[SES_IDX].res = _DO_ESCAPE_;
        longjmp(SES_TAB[SES_IDX].exi, 1); }
    if (_ag_is_VOI_(_ESC_))
      return;
    _nat_interrupt_(); }

_NIL_TYPE_ _display_(const _STR_TYPE_ Str)
  { _ESC_TYPE_ esc;
    STATUS = DISABLED;
    esc = _PICO_DISPLAY_(SES_TAB[SES_IDX].ses, Str);
    STATUS = ENABLED;
    if (esc == _ESCAPE_YES_)
      { SES_TAB[SES_IDX].res = _DO_ESCAPE_;
        longjmp(SES_TAB[SES_IDX].exi, 1); }
    if (_ag_is_VOI_(_ESC_))
      return;
    _nat_interrupt_(); }

/*---------------------- exported functions -----------------------*/

_RES_TYPE_ _PICO_INIT_(const _ADR_TYPE_ Adr,
                       const _SIZ_TYPE_ Siz)
  { _SIZ_TYPE_ max = Siz/_EXP_SIZE_;
    STATUS = NO_INIT; 
    if (sizeof(long) != 4)
      return _MEM_ERROR_;
    if (sizeof(double) != 8)
      return _MEM_ERROR_;
    if ((_SIZ_TYPE_)Adr % _EXP_SIZE_ != 0)
      return _MEM_ERROR_;
    if ((_SIZ_TYPE_)Siz % _EXP_SIZE_ != 0)
      return _MEM_ERROR_;
    if (max < _GLOBAL_SIZE_)
      return _MEM_ERROR_;
    _mem_setup_(Adr, Siz);
    NAM = (_SIZ_TYPE_)max*_NAM_FRACT_;
    ROOT = _ag_make_TAB_(ROOT_SIZE);
    _mem_set_root_(ROOT);
    CNT = _ag_make_TAB_(_MAX_SES_);
    for (SES_IDX = 0 ; SES_IDX < _MAX_SES_ ; SES_IDX++)
      SES_TAB[SES_IDX].ses = UNUSED; 
    _CNT_setup_(NAM);
    if(!_nat_install_())
      { return _MEM_ERROR_; } 
    GLOBAL = _NAT_;
    _RST_ = _ag_make_TAB_(RST_SIZE);
    _ag_set_TAB_EXP_(ROOT, GLB_ROOT, GLOBAL);
    _ag_set_TAB_EXP_(ROOT, NAM_ROOT, _NAM_);
    _ag_set_TAB_EXP_(ROOT, CNT_ROOT, CNT);
    _ag_set_TAB_EXP_(ROOT, NAT_ROOT, _NAT_);
    _ag_set_TAB_EXP_(ROOT, RST_ROOT, _RST_);
    _DLY_ = 0;
    _MAX_ = (_UNS_TYPE_)(max*_STK_FRACT_);
    STATUS = ENABLED; 
    return _PICO_DONE_; }

_RES_TYPE_ _PICO_SESSION_(const _SES_TYPE_ Ses)
  { static _BYT_TYPE_ idx;
    if (STATUS == NO_INIT)
      return _CTL_ERROR_;
    for (idx = 0 ; idx < _MAX_SES_ ; idx++)
      if (SES_TAB[idx].ses == Ses)
        return _DPS_ERROR_;
    for (idx = 0 ; idx < _MAX_SES_ ; idx++)
      if (SES_TAB[idx].ses == UNUSED)
        { SES_TAB[idx].ses = Ses;
          SES_TAB[idx].res = _PICO_DONE_;
          SES_TAB[idx].dct = GLOBAL;
          SES_TAB[idx].esc = _VOI_;
          _DCT_ = GLOBAL;
          CNT_INITIALISE(idx);    
          return _PICO_DONE_; }
    return _TMS_ERROR_; }
   
_RES_TYPE_ _PICO_DO_(const _SES_TYPE_ Ses,
                     const _STR_TYPE_ Str,
                     const _MRK_TYPE_ Mrk)
  { if (STATUS != ENABLED)
      return _CTL_ERROR_;
    for (SES_IDX = 0 ; SES_IDX < _MAX_SES_ ; SES_IDX++)
      if (SES_TAB[SES_IDX].ses == Ses)
        { CNT_RESTORE(SES_IDX);
          _stk_claim_();
          if(Mrk == _MARK_YES_)
            _scan_init_(Str, _TRUE_);
          else
            _scan_init_(Str, _FALSE_);
          _stk_push_CNT_(exit_loop);
          _stk_push_CNT_(_pri_EXP_);
          _stk_push_CNT_(_eva_EXP_);
          _stk_push_CNT_(_rea_EXP_);
          _ESC_ = SES_TAB[SES_IDX].esc;
          do_loop(&SES_TAB[SES_IDX].exi);
          CNT_CAPTURE(SES_IDX); 
          return SES_TAB[SES_IDX].res; }
    return _SNA_ERROR_; }

_RES_TYPE_ _PICO_LOAD_(const _SES_TYPE_ Ses,
                       const _STR_TYPE_ Str)
  { if (STATUS != ENABLED)
      return _CTL_ERROR_;
    for (SES_IDX = 0 ; SES_IDX < _MAX_SES_ ; SES_IDX++)
      if (SES_TAB[SES_IDX].ses == Ses)
        { if (SES_TAB[SES_IDX].res != _DO_LOAD_)
            return _CTL_ERROR_;
          CNT_RESTORE(SES_IDX);
          _stk_claim_();
          _scan_init_(Str, _TRUE_); // always mark for tpico -- dvd
          _stk_push_CNT_(_pri_EXP_);
          _stk_push_CNT_(_eva_EXP_);
          _stk_push_CNT_(_rea_EXP_);
          _ESC_ = SES_TAB[SES_IDX].esc;
          do_loop(&SES_TAB[SES_IDX].exi);
          CNT_CAPTURE(SES_IDX); 
          return SES_TAB[SES_IDX].res; }
    return _SNA_ERROR_; }

_RES_TYPE_ _PICO_ACCEPT_(const _SES_TYPE_ Ses,
                         const _STR_TYPE_ Str)
  { _EXP_TYPE_ nam;
    if (STATUS != ENABLED)
      return _CTL_ERROR_;
    for (SES_IDX = 0 ; SES_IDX < _MAX_SES_ ; SES_IDX++)
      if (SES_TAB[SES_IDX].ses == Ses)
        { if (SES_TAB[SES_IDX].res != _DO_ACCEPT_)
            return _CTL_ERROR_;
          CNT_RESTORE(SES_IDX);
          _stk_claim_();
          _mem_claim_STR_(Str);
                  nam = _CNT_make_NAM_(Str);
          _stk_push_EXP_(nam); 
          _ESC_ = SES_TAB[SES_IDX].esc;        
          do_loop(&SES_TAB[SES_IDX].exi);
          CNT_CAPTURE(SES_IDX);
          return SES_TAB[SES_IDX].res; }
    return _SNA_ERROR_; }

_RES_TYPE_ _PICO_CONTINUE_(const _SES_TYPE_ Ses)
  { if (STATUS != ENABLED)
      return _CTL_ERROR_;
    for (SES_IDX = 0 ; SES_IDX < _MAX_SES_ ; SES_IDX++)
      if (SES_TAB[SES_IDX].ses == Ses)
        { if (SES_TAB[SES_IDX].res != _DO_ESCAPE_)
            return _CTL_ERROR_;
          CNT_RESTORE(SES_IDX);
          _ESC_ = SES_TAB[SES_IDX].esc;
          do_loop(&SES_TAB[SES_IDX].exi);
          CNT_CAPTURE(SES_IDX);
          return SES_TAB[SES_IDX].res; }
    return _SNA_ERROR_; }         

_RES_TYPE_ _PICO_KILL_(const _SES_TYPE_ Ses)
  { static _BYT_TYPE_ idx;
    if (STATUS != ENABLED)
      return _CTL_ERROR_;
    for (idx = 0 ; idx < _MAX_SES_ ; idx++)
      if (SES_TAB[idx].ses == Ses)
        { SES_TAB[idx].ses = UNUSED;
          SES_TAB[idx].dct = _VOI_;
          SES_TAB[idx].esc = _VOI_;
          CNT_VOID(idx);
          return _PICO_DONE_; }
    return _SNA_ERROR_; }
