             /*-----------------------------------*/
             /*           >>>Pico 2.0<<<          */
             /*            Theo D'Hondt           */
             /*   VUB Programming Technology Lab  */
             /*             (c) 2002              */
             /*-----------------------------------*/
             /*            Chunk store            */
             /*-----------------------------------*/

#include "Pico.h"
#include "PicoMAI.h"
#include "PicoMEM.h"

/*---------------------- private constants ------------------------*/

#define mem_strbp 0x00000000
#define mem_strBP 0x00000003
#define mem_stRbP 0x00000005
#define mem_stRBP 0x00000007
#define mem_STRbP 0xFFFFFFFD

#define mem_FRST _mem_MEM_ + _EXP_SIZE_
#define mem_ROOT _mem_MEM_

#define mem_SMAX 0x00FFFFFF

/*---------------------- private macro's --------------------------*/

#define mem_BNP(ADR) (mem_strBp | (ADR))
  
#define mem_BPT(ADR) (mem_strBP | (ADR))
  
#define mem_BSY(CEL) ((CEL) & mem_strBp)
  
#define mem_DEC(ADR) ((ADR) -= mem_stRbp)
  
#define mem_FLG(CEL) ((CEL) & mem_stRBP)
  
#define mem_FPT(ADR) (mem_strbP | (ADR))
  
#define mem_FRE(CEL) ((CEL) &= mem_STRbP)
   
#define mem_INC(ADR) ((ADR) += mem_stRbp)

#define mem_IDX(PTR) ((PTR) >> mem_r_SHFT)
  
#define mem_NHD(SIZ) (mem_strbP | ((SIZ) << mem_s_SHFT))
  
#define mem_ADD(ADR, SIZ) ((ADR) += (SIZ) << mem_r_SHFT)
  
#define mem_SUM(ADR, SIZ) ((ADR) + ((SIZ) << mem_r_SHFT))

#define mem_HDR(TAG, SIZ) (mem_strbP | ((TAG) << mem_t_SHFT) | ((SIZ) << mem_s_SHFT))
  
#define mem_PRE(ADR, REF) (ADR = REF - mem_stRbp)
  
#define mem_RHD(TAG, SIZ) (mem_stRbP | ((TAG) << mem_t_SHFT) | ((SIZ) << mem_s_SHFT))
  
/*--------------------- private variables -------------------------*/

static _UNS_TYPE_ mem_LOW  = _UNS_MAX_;
static _UNS_TYPE_ mem_HIGH = _NIL_VALUE_;

/*--------------------- private functions -------------------------*/

static _NIL_TYPE_ mark(_NIL_TYPE_)
  { _UNS_TYPE_ cur, siz, ths;
    _EXP_TYPE_ cel;
    cur = mem_ROOT;
    for (;;)
      { cel = mem_CEL(cur);
        switch (mem_FLG(cel))
          { case mem_strbP:  
            case mem_stRbP:
              ths = mem_ADR(cel);
              cel = mem_CEL(ths);
              switch (mem_FLG(cel))
                { case mem_strbP:
                    mem_CEL(ths) = mem_BPT(cur);
                    mem_CEL(cur) = cel;
                    siz = mem_SIZ(cel);
                    cur = mem_SUM(ths, siz);
                    continue;   
                  case mem_strBP:  
                  case mem_stRBP:   
                    ths = mem_ADR(cel);
                    cel = mem_CEL(ths); 
                  case mem_stRbP: 
                    mem_CEL(ths) = mem_BPT(cur);
                    mem_CEL(cur) = cel;
                    mem_DEC(cur);
                    continue; }
              continue;
            case mem_strBP:  
            case mem_stRBP:   
              cur = mem_ADR(cel);
              if (cur == mem_ROOT)
                return;
            default: 
              mem_DEC(cur); }}}

static _NIL_TYPE_ sweep(_NIL_TYPE_)
  { _UNS_TYPE_ cel, fre, frs, ptr, siz, ths;
    _UNS_TYPE_ src = mem_FRST;
    _UNS_TYPE_ dst = src;
    while (src < _mem_FRE_)
      { cel =  mem_CEL(src);
        if (mem_BSY(cel))
          { ptr = mem_FPT(dst);
            do
              { ths = mem_ADR(cel);
                cel = mem_CEL(ths);
                mem_CEL(ths) = ptr; }
            while (mem_BSY(cel));
            mem_CEL(src) = mem_BNP(cel);
            siz = 1 + mem_SIZ(cel);
            mem_ADD(src, siz);
            mem_ADD(dst, siz); }
        else
          { fre = src;
            frs = 0;
            do
              { siz = 1 + mem_SIZ(cel);
                if ((frs + siz) >= mem_SMAX)
                  break; 
                frs += siz;
                mem_ADD(src, siz);
                if (src >= _mem_FRE_)
                  break;
                cel = mem_CEL(src); }
            while (!mem_BSY(cel));
            mem_CEL(fre) = mem_NHD(frs - 1); }}}
              
static _NIL_TYPE_ compact(_NIL_TYPE_)
  { _UNS_TYPE_ cel, siz;
    _UNS_TYPE_ src = mem_FRST;
    _UNS_TYPE_ dst = src;
    while (src < _mem_FRE_)
      { cel = mem_CEL(src);
        siz = mem_SIZ(cel);
        if (mem_BSY(cel))
          { mem_FRE(mem_CEL(src));
            do
              { mem_CEL(dst) = mem_CEL(src);
                mem_INC(dst);
                mem_INC(src); }
            while (siz--); }
        else
          mem_ADD(src, siz + 1); }
    _mem_FRE_ = dst; }

/*----------------------- public variables ------------------------*/

_UNS_TYPE_ _mem_FRE_ = _NIL_VALUE_;
_UNS_TYPE_ _mem_MEM_ = _NIL_VALUE_;
_UNS_TYPE_ _mem_TOP_ = _NIL_VALUE_;

/*----------------------- public functions ------------------------*/

_NIL_TYPE_ _mem_setup_(const _ADR_TYPE_ Adr,
                       const _UNS_TYPE_ Siz)
  { _mem_MEM_ = (_UNS_TYPE_)Adr;
    _mem_TOP_ = _mem_MEM_ + Siz;
    _mem_FRE_ = mem_FRST; }
   
_EXP_TYPE_ _mem_make_chunk_(const _TAG_TYPE_ Tag,
                            const _UNS_TYPE_ Siz)
  { _EXP_TYPE_ exp = mem_FPT(_mem_FRE_);
    _UNS_TYPE_ idx = Siz;
    mem_CEL(_mem_FRE_) = mem_HDR(Tag, Siz);
    while (idx--)
      { mem_INC(_mem_FRE_);
        mem_CEL(_mem_FRE_) = mem_strbp; }
    mem_INC(_mem_FRE_);
    return exp; }

_EXP_TYPE_ _mem_fill_chunk_(const _TAG_TYPE_ Tag,
                            const _UNS_TYPE_ Siz,
                                             ...)                                        
  { _EXP_TYPE_ exp = mem_FPT(_mem_FRE_);
    _UNS_TYPE_ idx = Siz;
    _ARG_TYPE_ ptr;
    mem_CEL(_mem_FRE_) = mem_HDR(Tag, Siz);
    va_start(ptr, Siz);
    while (idx--)
      { mem_INC(_mem_FRE_);
        mem_CEL(_mem_FRE_) = va_arg(ptr, _EXP_TYPE_); }
    va_end(ptr);
    mem_INC(_mem_FRE_);
    return exp; }
   
_EXP_TYPE_ _mem_make_raw_(const _TAG_TYPE_ Tag,
                          const _UNS_TYPE_ Siz)
  { _EXP_TYPE_ exp = mem_FPT(_mem_FRE_);
    mem_CEL(_mem_FRE_) = mem_RHD(Tag, Siz);
    mem_ADD(_mem_FRE_, Siz + 1);
    return exp; }

_EXP_TYPE_ _mem_fill_raw_(const _TAG_TYPE_ Tag,
                          const _UNS_TYPE_ Len,
                          const _ADR_TYPE_ Adr)
  { _EXP_TYPE_ exp = mem_FPT(_mem_FRE_);
    _UNS_TYPE_ siz = (Len + _EXP_SIZE_ - 1) / _EXP_SIZE_;
    mem_CEL(_mem_FRE_) = mem_RHD(Tag, siz);
    mem_INC(_mem_FRE_);
    memcpy((void *)_mem_FRE_, Adr, Len);
    mem_ADD(_mem_FRE_, siz);
    return exp; }

_BYT_TYPE_ _mem_reclaim_(const _UNS_TYPE_ Mrg)
  { if(mem_LOW <= mem_HIGH)
      return _FALSE_;
    mark();
    sweep();
    compact(); 
    return (_mem_FRE_ + Mrg) < _mem_TOP_; }
      
_NIL_TYPE_ _mem_set_root_(const _EXP_TYPE_ exp)
  { mem_CEL(_mem_MEM_) = exp; } 
  
_NIL_TYPE_ _mem_enter_cache_(const _EXP_TYPE_ Exp)
  { _UNS_TYPE_ idx = mem_IDX(Exp);
    if(idx > mem_HIGH)
      mem_HIGH = idx;
    if(idx < mem_LOW)
      mem_LOW = idx; }
                                  
_BYT_TYPE_ _mem_make_cache_(const _TAG_TYPE_   Tag,
                                  _EXP_TYPE_ * Cch)
  { _UNS_TYPE_ siz;
    _EXP_TYPE_ ref;
    if(mem_LOW > mem_HIGH)
      return _FALSE_;
    siz = mem_HIGH - mem_LOW + 2;
    if((_mem_FRE_ + siz) >= _mem_TOP_)
      return _FALSE_; 
    ref = _mem_make_number_(mem_LOW);
    *Cch = _mem_make_chunk_(Tag, siz);
    _mem_set_exp_(*Cch, siz, ref);
    mem_LOW  = _UNS_MAX_;
    mem_HIGH = _NIL_VALUE_;
    return _TRUE_; }

_BYT_TYPE_ _mem_set_cache_(const _EXP_TYPE_ Cch,
                           const _EXP_TYPE_ Exp,
                           const _EXP_TYPE_ Val)
  { _UNS_TYPE_ siz = _mem_chunk_size_(Cch);
    _EXP_TYPE_ ref = _mem_get_exp_(Cch, siz);
    _UNS_TYPE_ bas = _mem_get_UNS_(ref);
    _SGN_TYPE_ idx = mem_IDX(Exp) - bas + 1;
    if((idx >= siz) || (idx < 1))
      return _FALSE_;
    _mem_set_exp_(Cch, idx, Val);
    return _TRUE_; }

_BYT_TYPE_ _mem_get_cache_(const _EXP_TYPE_   Cch,
                           const _EXP_TYPE_   Exp,
                                 _EXP_TYPE_ * Val)
  { _UNS_TYPE_ siz = _mem_chunk_size_(Cch);
    _EXP_TYPE_ ref = _mem_get_exp_(Cch, siz);
    _UNS_TYPE_ bas = _mem_get_UNS_(ref);
    _SGN_TYPE_ idx = mem_IDX(Exp) - bas + 1;
    if((idx >= siz) || (idx < 1))
      return _FALSE_;
    *Val = _mem_get_exp_(Cch, idx);
    return _TRUE_; }


