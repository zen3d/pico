             /*-----------------------------------*/
             /*           >>>Pico 2.0<<<          */
             /*            Theo D'Hondt           */
             /*   VUB Programming Technology Lab  */
             /*             (c) 2002              */
             /*-----------------------------------*/
             /*            Chunk store            */
             /*-----------------------------------*/
             
/*---------------------- private constants ------------------------*/

#define mem_strbP 0x00000001
#define mem_strBp 0x00000002
#define mem_stRbp 0x00000004
#define mem_sTrbp 0x000000F8
#define mem_STRbp 0xFFFFFFFC

#define mem_b_SHFT 1
#define mem_r_SHFT 2
#define mem_t_SHFT 3
#define mem_s_SHFT 8

/*----------------------- private macros --------------------------*/

#define mem_ADR(PTR) ((PTR) & mem_STRbp)
  
#define mem_CEL(ADR) (*(_EXP_TYPE_ *)(ADR))
  
#define mem_SIZ(HDR) ((HDR) >> mem_s_SHFT)
    
#define mem_TAG(HDR) (((HDR) & mem_sTrbp) >> mem_t_SHFT)
    
/*----------------------- public contants -------------------------*/

#define _NBR_MAX_ 0x3FFFFFFF

/*------------------------ public macros --------------------------*/

#define _mem_claim_SIZ_(SIZ)\
  if (_mem_FRE_ + _EXP_SIZE_ * (SIZ + _MEM_CLAIM_) >= _mem_TOP_)\
    _reclaim_(_EXP_SIZE_ * (SIZ + _MEM_CLAIM_))
     
#define _mem_claim_STR_(STR)\
  if (_mem_FRE_ + strlen(STR) + _EXP_SIZE_ * _MEM_CLAIM_ >= _mem_TOP_)\
    _reclaim_(strlen(STR) + _EXP_SIZE_ * _MEM_CLAIM_)
     
#define _mem_claim_()\
  if (_mem_FRE_ + _EXP_SIZE_ * _MEM_CLAIM_ > _mem_TOP_)\
    _reclaim_(_EXP_SIZE_ * _MEM_CLAIM_)
     
#define _mem_make_number_(SGN)\
  (_EXP_TYPE_)(SGN << mem_b_SHFT)
   
#define _mem_succ_number_(NBR)\
  (_EXP_TYPE_)((_SGN_TYPE_)NBR + mem_strBp)
   
#define _mem_pred_number_(NBR)\
  (_EXP_TYPE_)((_SGN_TYPE_)NBR - mem_strBp)
   
#define _mem_chunk_size_(EXP)\
  (_UNS_TYPE_)mem_SIZ(mem_CEL(mem_ADR(EXP)))

#define _mem_get_tag_(EXP)\
  (_TAG_TYPE_)mem_TAG(mem_CEL(mem_ADR(EXP)))
  
#define _mem_is_same_(EX1, EX2)\
  (EX1 == EX2)

#define  _mem_get_SGN_(EXP)\
  ((_SGN_TYPE_)EXP >> mem_b_SHFT)
   
#define  _mem_get_UNS_(EXP)\
  ((_UNS_TYPE_)EXP >> mem_b_SHFT)
   
#define _mem_is_number_(EXP)\
  (!(EXP & mem_strbP))

#define _mem_get_exp_(EXP, IDX)\
  mem_CEL(mem_ADR(EXP) + (IDX << mem_r_SHFT))
  
#define _mem_set_exp_(EXP, IDX, VAL)\
  _mem_get_exp_(EXP, IDX) = VAL

#define _mem_cast_raw_(EXP,TAG)\
  (void *)(mem_ADR(EXP) + mem_stRbp)

#define _mem_is_raw_(EXP)\
 (mem_CEL(mem_ADR(EXP)) & mem_stRbp)

/*----------------------- public variables ------------------------*/

extern _UNS_TYPE_ _mem_FRE_;
extern _UNS_TYPE_ _mem_MEM_;
extern _UNS_TYPE_ _mem_TOP_;

/*---------------------- public prototypes ------------------------*/

_NIL_TYPE_       _mem_setup_(const _ADR_TYPE_,
                             const _UNS_TYPE_);
 
_EXP_TYPE_  _mem_make_chunk_(const _TAG_TYPE_,
                             const _UNS_TYPE_);
                                         
_EXP_TYPE_  _mem_fill_chunk_(const _TAG_TYPE_,
                             const _UNS_TYPE_,
                                          ...);
                                         
_EXP_TYPE_    _mem_make_raw_(const _TAG_TYPE_,
                             const _UNS_TYPE_);
                                         
_EXP_TYPE_    _mem_fill_raw_(const _TAG_TYPE_,
                             const _UNS_TYPE_,
                             const _ADR_TYPE_);

_BYT_TYPE_     _mem_reclaim_(const _UNS_TYPE_);

_NIL_TYPE_    _mem_set_root_(const _EXP_TYPE_);

_NIL_TYPE_ _mem_enter_cache_(const _EXP_TYPE_);
					  
_BYT_TYPE_  _mem_make_cache_(const _TAG_TYPE_  ,
                                   _EXP_TYPE_ *);

_BYT_TYPE_   _mem_set_cache_(const _EXP_TYPE_,
                             const _EXP_TYPE_,
                             const _EXP_TYPE_);

_BYT_TYPE_   _mem_get_cache_(const _EXP_TYPE_  ,
                             const _EXP_TYPE_  ,
                                   _EXP_TYPE_ *);

