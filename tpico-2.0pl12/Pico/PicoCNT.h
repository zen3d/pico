             /*-----------------------------------*/
             /*           >>>Pico 2.0<<<          */
             /*            Theo D'Hondt           */
             /*   VUB Programming Technology Lab  */
             /*             (c) 2002              */
             /*-----------------------------------*/
             /*             Contexts              */
             /*-----------------------------------*/

/*------------------------ public macros --------------------------*/


#define _stk_claim_()\
  if (_CNT_ - _EXP_ <= _STK_CLAIM_) _CNT_expand_()

#define _stk_poke_CNT_(CNT)\
  *(_CNT_TYPE_ *)_CNT_ = (CNT)

#define _stk_push_CNT_(CNT)\
  *(_CNT_TYPE_ *)(_CNT_ -= _EXP_SIZE_) = CNT

#define _stk_peek_CNT_(CNT)\
  CNT =  *(_CNT_TYPE_ *)_CNT_

#define _stk_call_CNT_()\
  (*(_CNT_TYPE_ *)_CNT_)()

#define _stk_zap__CNT_()\
  _CNT_ += _EXP_SIZE_

#define _stk_poke_EXP_(EXP)\
  *(_EXP_TYPE_ *)_EXP_ = EXP

#define _stk_push_EXP_(EXP)\
  *(_EXP_TYPE_ *)(_EXP_ += _EXP_SIZE_) = EXP

#define _stk_peek_EXP_(EXP)\
  EXP = *(_EXP_TYPE_ *)_EXP_

#define _stk_zap__EXP_()\
  ( *(_EXP_TYPE_ *)_EXP_ = _VOI_,\
    _EXP_ -= _EXP_SIZE_ )

#define _stk_pop__EXP_(EXP)\
  ( EXP = *(_EXP_TYPE_ *)_EXP_,\
    *(_EXP_TYPE_ *)_EXP_ = _VOI_,\
    _EXP_ -= _EXP_SIZE_ )
      
/*----------------------- public variables ------------------------*/

extern _EXP_TYPE_ _DCT_;
extern _UNS_TYPE_ _CNT_;
extern _UNS_TYPE_ _EXP_;
extern _EXP_TYPE_ _STK_;
                           
/*---------------------- public prototypes ------------------------*/

_EXP_TYPE_     _dct_locate_(const _EXP_TYPE_, 
                            const _EXP_TYPE_);

_EXP_TYPE_ _dct_locate_cst_(const _EXP_TYPE_, 
                            const _EXP_TYPE_);

_NIL_TYPE_      _CNT_setup_(const _SIZ_TYPE_);

_EXP_TYPE_    _CNT_set_NAM_(const _STR_TYPE_);

_EXP_TYPE_   _CNT_make_NAM_(const _STR_TYPE_);

_EXP_TYPE_ _CNT_initialise_(      _NIL_TYPE_);

_NIL_TYPE_    _CNT_capture_(const _EXP_TYPE_);

_NIL_TYPE_    _CNT_restore_(const _EXP_TYPE_);

_NIL_TYPE_     _CNT_expand_(      _NIL_TYPE_);

_UNS_TYPE_       _CNT_size_(      _NIL_TYPE_);

_NIL_TYPE_       _CNT_save_(const _EXP_TYPE_);

_NIL_TYPE_       _CNT_load_(const _EXP_TYPE_);
