             /*-----------------------------------*/
             /*           >>>Pico 2.0<<<          */
             /*            Theo D'Hondt           */
             /*   VUB Programming Technology Lab  */
             /*             (c) 2002              */
             /*-----------------------------------*/
             /*           Main program            */
             /*-----------------------------------*/

#include <assert.h>
#include <errno.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*--------------------- private constants -------------------------*/

#define APL_EXP_INDEX 1
#define APL_ARG_INDEX 2

#define CNT_DCT_INDEX 1
#define CNT_BOT_INDEX 2
#define CNT_TOP_INDEX 3
#define CNT_TAB_INDEX 4

#define DCL_INV_INDEX 1
#define DCL_EXP_INDEX 2

#define DCT_NAM_INDEX 1
#define DCT_VAL_INDEX 2
#define DCT_DCT_INDEX 3

#define DEF_INV_INDEX 1
#define DEF_EXP_INDEX 2

#define FUN_NAM_INDEX 1
#define FUN_ARG_INDEX 2
#define FUN_EXP_INDEX 3
#define FUN_DCT_INDEX 4

#define NAT_NAM_INDEX 1
#define NAT_IDX_INDEX 2

#define QUA_EXP_INDEX 1
#define QUA_INV_INDEX 2

#define QUO_EXP_INDEX 1

#define REF_NAM_INDEX 1

#define SET_INV_INDEX 1
#define SET_EXP_INDEX 2

#define TBL_EXP_INDEX 1
#define TBL_IDX_INDEX 2

/*----------------------- public contants -------------------------*/

#define _GLOBAL_SIZE_ 4000

#define _EXP_SIZE_    sizeof(_EXP_TYPE_)
#define _FLO_SIZE_    sizeof(_FLO_TYPE_)

#define _FLO_MIN_     DBL_MIN
#define _FRC_MAX_     DBL_MAX
#define _UNS_MAX_     ULONG_MAX

#define _STK_CLAIM_   24
#define _MEM_CLAIM_   10
#define _NIL_VALUE_   0

#define _MAX_SES_     _PICO_MAX_
#define _STK_INIT_    64
#define _STK_FRACT_   0.05
#define _NAM_FRACT_   0.05
#define _BUF_SIZE_    32768

#define _FALSE_       0
#define _TRUE_        1

#define _APL_RANK_    2
#define _CST_RANK_    3
#define _CNT_RANK_    4
#define _DCL_RANK_    2
#define _DEF_RANK_    2
#define _FRC_RANK_    1
#define _FUN_RANK_    4
#define _NAT_RANK_    2
#define _NBR_RANK_    1
#define _QUA_RANK_    2
#define _QUO_RANK_    1
#define _REF_RANK_    1
#define _SET_RANK_    2
#define _TAB_RANK_    0
#define _TBL_RANK_    2
#define _TXT_RANK_    1
#define _VAR_RANK_    3
#define _VOI_RANK_    0

/*------------------------ public macros --------------------------*/

#define _ag_make_APL_(EXP, ARG)\
  _mem_fill_chunk_(_APL_TAG_, _APL_RANK_, EXP, ARG)              

#define _ag_make_CST_(NAM, VAL, DCT)\
  _mem_fill_chunk_(_CST_TAG_, _CST_RANK_, NAM, VAL, DCT)       

#define _ag_make_CNT_(DCT, BOT, TOP, TAB)\
  _mem_fill_chunk_(_CNT_TAG_, _CNT_RANK_, DCT, BOT, TOP, TAB)  
       
#define _ag_make_DCL_(INV, EXP)\
  _mem_fill_chunk_(_DCL_TAG_, _DCL_RANK_, INV, EXP)             
   
#define _ag_make_DEF_(INV, EXP)\
  _mem_fill_chunk_(_DEF_TAG_, _DEF_RANK_, INV, EXP)             
   
#define _ag_make_FRC_(FLP)\
  _mem_fill_raw_(_FRC_TAG_, _FLO_SIZE_, &FLP)

#define _ag_make_FUN_(NAM, ARG, EXP, DCT)\
  _mem_fill_chunk_(_FUN_TAG_, _FUN_RANK_, NAM, ARG, EXP, DCT)             

#define _ag_make_NAT_(NAM, IDX)\
  _mem_fill_chunk_(_NAT_TAG_, _NAT_RANK_, NAM, IDX)

#define _ag_make_NBR_(SGN)\
  _mem_make_number_(SGN)                         
                               
#define _ag_make_QUA_(EXP, INV)\
  _mem_fill_chunk_(_QUA_TAG_, _QUA_RANK_, EXP, INV)              
   
#define _ag_make_QUO_(EXP)\
  _mem_fill_chunk_(_QUO_TAG_, _QUO_RANK_, EXP)              
   
#define _ag_make_REF_(NAM)\
  _mem_fill_chunk_(_REF_TAG_, _REF_RANK_, NAM)      

#define _ag_make_SET_(INV, EXP)\
  _mem_fill_chunk_(_SET_TAG_, _SET_RANK_, INV, EXP) 
  
#define _ag_make_TAB_(SIZ)\
  _mem_make_chunk_(_TAB_TAG_, SIZ)                  

#define _ag_make_TAB_ONE_(EXP1)\
  _mem_fill_chunk_(_TAB_TAG_, 1, EXP1)                  

#define _ag_make_TAB_TWO_(EXP1, EXP2)\
  _mem_fill_chunk_(_TAB_TAG_, 2, EXP1, EXP2)                  

#define _ag_make_TAB_3_(EXP1, EXP2, EXP3)\
  _mem_fill_chunk_(_TAB_TAG_, 3, EXP1, EXP2, EXP3)                  

#define _ag_make_TBL_(EXP, IDX)\
  _mem_fill_chunk_(_TBL_TAG_, _TBL_RANK_, EXP, IDX)              
   
#define _ag_make_TXT_(STR)\
  _mem_fill_raw_(_TXT_TAG_, strlen(STR) + 1, (void *)STR)

#define _ag_make_VAR_(NAM, VAL, DCT)\
  _mem_fill_chunk_(_VAR_TAG_, _VAR_RANK_, NAM, VAL, DCT)       

#define _ag_make_VOI_()\
  _mem_make_chunk_(_VOI_TAG_, _VOI_RANK_)
                             
/*-----------------------------------------------------------------*/

#define _ag_get_APL_EXP_(APL)\
  _mem_get_exp_(APL, APL_EXP_INDEX)

#define _ag_get_APL_ARG_(APL)\
  _mem_get_exp_(APL, APL_ARG_INDEX)

#define _ag_get_CNT_DCT_(ENV)\
  _mem_get_exp_(ENV, CNT_DCT_INDEX)

#define _ag_get_CNT_BOT_(ENV)\
  _mem_get_exp_(ENV, CNT_BOT_INDEX)

#define _ag_get_CNT_TOP_(ENV)\
  _mem_get_exp_(ENV, CNT_TOP_INDEX)

#define _ag_get_CNT_TAB_(ENV)\
  _mem_get_exp_(ENV, CNT_TAB_INDEX)

#define _ag_get_DCL_INV_(DCL)\
  _mem_get_exp_(DCL, DCL_INV_INDEX)

#define _ag_get_DCL_EXP_(DCL)\
  _mem_get_exp_(DCL, DCL_EXP_INDEX)

#define _ag_get_DCT_NAM_(DCT)\
  _mem_get_exp_(DCT, DCT_NAM_INDEX)

#define _ag_get_DCT_VAL_(DCT)\
  _mem_get_exp_(DCT, DCT_VAL_INDEX)

#define _ag_get_DCT_DCT_(DCT)\
  _mem_get_exp_(DCT, DCT_DCT_INDEX)

#define _ag_get_DEF_INV_(DEF)\
  _mem_get_exp_(DEF, DEF_INV_INDEX)

#define _ag_get_DEF_EXP_(DEF)\
  _mem_get_exp_(DEF, DEF_EXP_INDEX)

#define _ag_get_FUN_NAM_(FUN)\
  _mem_get_exp_(FUN, FUN_NAM_INDEX)
   
#define _ag_get_FUN_ARG_(FUN)\
  _mem_get_exp_(FUN, FUN_ARG_INDEX)
   
#define _ag_get_FUN_EXP_(FUN)\
  _mem_get_exp_(FUN, FUN_EXP_INDEX)

#define _ag_get_FUN_DCT_(FUN)\
  _mem_get_exp_(FUN, FUN_DCT_INDEX)
 
#define _ag_get_FRC_(FRC)\
  (*(_FLP_TYPE_)(_mem_cast_raw_(FRC, _FRC_TAG_)))
    
#define _ag_get_NAT_NAM_(NAT)\
  _mem_get_exp_(NAT, NAT_NAM_INDEX)

#define _ag_get_NAT_IDX_(NAT)\
  _mem_get_exp_(NAT, NAT_IDX_INDEX)

#define _ag_get_NBR_(NBR)\
  _mem_get_SGN_(NBR)
 
#define _ag_get_NBU_(NBR)\
  _mem_get_UNS_(NBR)
 
#define _ag_get_QUA_EXP_(QUA)\
  _mem_get_exp_(QUA, QUA_EXP_INDEX)

#define _ag_get_QUA_INV_(QUA)\
  _mem_get_exp_(QUA, QUA_INV_INDEX)

#define _ag_get_QUO_EXP_(QUO)\
  _mem_get_exp_(QUO, QUO_EXP_INDEX)

#define _ag_get_REF_NAM_(REF)\
  _mem_get_exp_(REF, REF_NAM_INDEX)

#define _ag_get_SET_INV_(SET)\
  _mem_get_exp_(SET, SET_INV_INDEX)

#define _ag_get_SET_EXP_(SET)\
  _mem_get_exp_(SET, SET_EXP_INDEX)
 
#define _ag_get_TAB_SIZ_(TAB)\
  _mem_chunk_size_(TAB)
  
#define _ag_get_TAB_EXP_(TAB, IDX)\
  _mem_get_exp_(TAB, IDX)

#define _ag_get_TBL_EXP_(TBL)\
  _mem_get_exp_(TBL, TBL_EXP_INDEX)

#define _ag_get_TBL_IDX_(TBL)\
  _mem_get_exp_(TBL, TBL_IDX_INDEX)

#define _ag_get_TXT_(TXT)\
  (_STR_TYPE_)_mem_cast_raw_(TXT, _TXT_TAG_)
 
/*-----------------------------------------------------------------*/

#define _ag_set_APL_EXP_(APL, EXP)\
  _mem_set_exp_(APL, APL_EXP_INDEX, EXP)

#define _ag_set_APL_ARG_(APL, ARG)\
  _mem_set_exp_(APL, APL_ARG_INDEX, ARG)

#define _ag_set_CNT_DCT_(ENV, DCT)\
  _mem_set_exp_(ENV, CNT_DCT_INDEX, DCT)

#define _ag_set_CNT_BOT_(ENV, LWR)\
  _mem_set_exp_(ENV, CNT_BOT_INDEX, LWR)

#define _ag_set_CNT_TOP_(ENV, UPR)\
  _mem_set_exp_(ENV, CNT_TOP_INDEX, UPR)

#define _ag_set_CNT_TAB_(ENV, TAB)\
  _mem_set_exp_(ENV, CNT_TAB_INDEX, TAB)

#define _ag_set_DCL_INV_(DCL, REF)\
  _mem_set_exp_(DCL, DCL_INV_INDEX, REF)

#define _ag_set_DCL_EXP_(DCL, EXP)\
  _mem_set_exp_(DCL, DCL_EXP_INDEX, EXP)

#define _ag_set_DCT_NAM_(DCT, NAM)\
  _mem_set_exp_(DCT, DCT_NAM_INDEX, NAM)

#define _ag_set_DCT_VAL_(DCT, VAL)\
  _mem_set_exp_(DCT, DCT_VAL_INDEX, VAL)

#define _ag_set_DCT_DCT_(DCT, DCX)\
  _mem_set_exp_(DCT, DCT_DCT_INDEX, DCX)

#define _ag_set_DEF_INV_(DEF, REF)\
  _mem_set_exp_(DEF, DEF_INV_INDEX, REF)

#define _ag_set_DEF_EXP_(DEF, EXP)\
  _mem_set_exp_(DEF, DEF_EXP_INDEX, EXP)

#define _ag_set_FUN_NAM_(FUN, NAM)\
  _mem_set_exp_(FUN, FUN_NAM_INDEX, NAM)
   
#define _ag_set_FUN_ARG_(FUN, ARG)\
  _mem_set_exp_(FUN, FUN_ARG_INDEX, ARG)
   
#define _ag_set_FUN_EXP_(FUN, EXP)\
  _mem_set_exp_(FUN, FUN_EXP_INDEX, EXP)

#define _ag_set_FUN_DCT_(FUN, DCT)\
  _mem_set_exp_(FUN, FUN_DCT_INDEX, DCT)
 
#define _ag_set_NAT_NAM_(NAT, NAM)\
  _mem_set_exp_(NAT, NAT_NAM_INDEX, NAM)

#define _ag_set_NAT_IDX_(NAT, IDX)\
  _mem_set_exp_(NAT, NAT_IDX_INDEX, IDX)

#define _ag_set_QUA_EXP_(QUA, EXP)\
  _mem_set_exp_(QUA, QUA_EXP_INDEX, EXP)

#define _ag_set_QUA_INV_(QUA, INV)\
  _mem_set_exp_(QUA, QUA_INV_INDEX, INV)

#define _ag_set_QUO_EXP_(QUO, EXP)\
  _mem_set_exp_(QUO, QUO_EXP_INDEX, EXP)

#define _ag_set_REF_NAM_(REF, NAM)\
  _mem_set_exp_(REF, REF_NAM_INDEX, NAM)

#define _ag_set_SET_INV_(SET, REF)\
  _mem_set_exp_(SET, SET_INV_INDEX, REF)

#define _ag_set_SET_EXP_(SET, EXP)\
  _mem_set_exp_(SET, SET_EXP_INDEX, EXP)

#define _ag_set_TAB_EXP_(TAB, IDX, EXP)\
  _mem_set_exp_(TAB, IDX, EXP)
 
#define _ag_set_TAB_CNT_(TAB, IDX, CNT)\
  _mem_set_cnt_(TAB, IDX, CNT)

#define _ag_set_TBL_EXP_(TBL, EXP)\
  _mem_set_exp_(TBL, TBL_EXP_INDEX, EXP)

#define _ag_set_TBL_IDX_(TBL, IDX)\
  _mem_set_exp_(TBL, TBL_IDX_INDEX, IDX)

/*-----------------------------------------------------------------*/

#define _ag_get_TAG_(AGR)\
  ((_mem_is_number_(AGR))\
    ? _NBR_TAG_ : _mem_get_tag_(AGR))
      
#define _ag_get_TAG__(AGR)\
  _mem_get_tag_(AGR)
      
#define _ag_get_NAM_(INV, NAM)\
  switch (_ag_get_TAG_(INV))\
    { case _REF_TAG_:\
        NAM = _ag_get_REF_NAM_(INV);\
        break;\
      case _APL_TAG_:\
        { _EXP_TYPE_ exp = _ag_get_APL_EXP_(INV);\
          if(!_ag_is_REF_(exp))\
            _error_exp_(_AGR_ERROR_, exp);\
          NAM = _ag_get_REF_NAM_(exp); }\
        break;\
      case _TBL_TAG_:\
        { _EXP_TYPE_ exp = _ag_get_TBL_EXP_(INV);\
          if(!_ag_is_REF_(exp))\
            _error_exp_(_AGR_ERROR_, exp);\
          NAM = _ag_get_REF_NAM_(exp); }\
        break;\
      default:\
        _error_(_AGR_ERROR_); }

#define _ag_is_raw_(EXP)\
  ((_mem_is_number_(EXP))\
    ? _TRUE_ : _mem_is_raw_(EXP))
 
#define _ag_is_DCT_(EXP)\
  ((_mem_is_number_(EXP))\
    ? _FALSE_ : ((_mem_get_tag_(EXP) == _VAR_TAG_) ||\
                 (_mem_get_tag_(EXP) == _CST_TAG_) ||\
                 (_mem_get_tag_(EXP) == _VOI_TAG_)))
 
#define _ag_is_INV_(EXP)\
  ((_mem_is_number_(EXP))\
    ? _FALSE_ : ((_mem_get_tag_(EXP) == _REF_TAG_) ||\
                 (_mem_get_tag_(EXP) == _APL_TAG_) ||\
                 (_mem_get_tag_(EXP) == _TBL_TAG_)))
 
#define _ag_is_FNC_(EXP)\
  ((_mem_is_number_(EXP))\
    ? _FALSE_ : ((_mem_get_tag_(EXP) == _FUN_TAG_) ||\
                 (_mem_get_tag_(EXP) == _NAT_TAG_)))
 
#define _ag_is_CNT_(EXP)\
  ((_mem_is_number_(EXP))\
    ? _FALSE_ : (_mem_get_tag_(EXP) == _CNT_TAG_))
 
#define _ag_is_FRC_(EXP)\
  ((_mem_is_number_(EXP))\
    ? _FALSE_ : (_mem_get_tag_(EXP) == _FRC_TAG_))
 
#define _ag_is_FRC__(EXP)\
  (_mem_get_tag_(EXP) == _FRC_TAG_)
 
#define _ag_is_FUN_(EXP)\
  ((_mem_is_number_(EXP))\
    ? _FALSE_ : (_mem_get_tag_(EXP) == _FUN_TAG_))
 
#define _ag_is_NAT_(EXP)\
  ((_mem_is_number_(EXP))\
    ? _FALSE_ : (_mem_get_tag_(EXP) == _NAT_TAG_))
 
#define _ag_is_NBR_(EXP)\
  (_mem_is_number_(EXP))

#define _ag_is_NUM_(EXP)\
  ((_mem_is_number_(EXP))\
    ? _TRUE_ : _ag_is_FRC_(EXP))
 
#define _ag_is_TAB_(EXP)\
  ((_mem_is_number_(EXP))\
    ? _FALSE_ : (_mem_get_tag_(EXP) == _TAB_TAG_))
 
#define _ag_is_TXT_(EXP)\
  ((_mem_is_number_(EXP))\
    ? _FALSE_ : (_mem_get_tag_(EXP) == _TXT_TAG_))
 
#define _ag_is_REF_(EXP)\
  ((_mem_is_number_(EXP))\
    ? _FALSE_ : (_mem_get_tag_(EXP) == _REF_TAG_))
 
#define _ag_is_VAR_(EXP)\
  ((_mem_is_number_(EXP))\
    ? _FALSE_ : (_mem_get_tag_(EXP) == _VAR_TAG_))
 
#define _ag_is_VOI_(EXP)\
  ((_mem_is_number_(EXP))\
    ? _FALSE_ : (_mem_get_tag_(EXP) == _VOI_TAG_))
 
#define _ag_succ_NBR_(NBR)\
  _mem_succ_number_(NBR)                            
                               
#define _ESCAPE_\
  if (!_DLY_--) _escape_()

/*------------------------- public types --------------------------*/

typedef enum { _VOI_TAG_ =  0,
               _REF_TAG_ =  1,
               _APL_TAG_ =  2,
               _TBL_TAG_ =  3,
               _QUA_TAG_ =  4,
               _DCL_TAG_ =  5,
               _DEF_TAG_ =  6,
               _SET_TAG_ =  7,
               _CST_TAG_ =  8,
               _VAR_TAG_ =  9,
               _CNT_TAG_ = 10,
               _NAT_TAG_ = 11,
               _FUN_TAG_ = 12,
               _TAB_TAG_ = 13,
               _TXT_TAG_ = 14,
               _FRC_TAG_ = 15,
               _QUO_TAG_ = 16,
               _NBR_TAG_ = 32  } _TAG_TYPE_;

typedef unsigned long _UNS_TYPE_;
typedef   signed long _SGN_TYPE_; 
typedef     long long _LNG_TYPE_; 
typedef unsigned char _BYT_TYPE_;
typedef       jmp_buf _EXI_TYPE_;
typedef       va_list _ARG_TYPE_;
typedef unsigned long _EXP_TYPE_;

typedef char _BUF_TYPE_[_BUF_SIZE_];

typedef _NIL_TYPE_ (*_CNT_TYPE_)(_NIL_TYPE_);

typedef _EXP_TYPE_ (*_INI_TYPE_)(_NIL_TYPE_);

typedef struct { _SES_TYPE_ ses;
                 _RES_TYPE_ res;
                 _EXI_TYPE_ exi;
                 _EXP_TYPE_ dct; 
                 _EXP_TYPE_ esc; } _DSC_TYPE_;

/*----------------------- public variables ------------------------*/

extern _BUF_TYPE_ _BUF_;
extern _BYT_TYPE_ _DLY_;
extern _UNS_TYPE_ _MAX_;
extern _EXP_TYPE_ _NAM_;
extern _EXP_TYPE_ _NAT_;
extern _EXP_TYPE_ _RST_;

/*---------------------- public prototypes ------------------------*/

_NIL_TYPE_      _print_(const _STR_TYPE_);

_NIL_TYPE_      _error_(const _RES_TYPE_);

_NIL_TYPE_    _reclaim_(const _UNS_TYPE_);

_EXP_TYPE_  _error_str_(const _RES_TYPE_,
                        const _STR_TYPE_);
                       
_NIL_TYPE_  _error_nam_(const _RES_TYPE_,
                        const _EXP_TYPE_);
                       
_NIL_TYPE_  _error_exp_(const _RES_TYPE_,
                        const _EXP_TYPE_);
                       
_NIL_TYPE_  _error_fun_(const _RES_TYPE_,
                        const _EXP_TYPE_);
                       
_NIL_TYPE_       _mark_(const _POS_TYPE_ ,
                        const _POS_TYPE_);
                       
_NIL_TYPE_     _escape_(      _NIL_TYPE_);

_NIL_TYPE_     _accept_(      _NIL_TYPE_);

_NIL_TYPE_       _load_(const _STR_TYPE_);

_NIL_TYPE_    _display_(const _STR_TYPE_);
