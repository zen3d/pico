             /*-----------------------------------*/
             /*           >>>Pico 2.0<<<          */
             /*            Theo D'Hondt           */
             /*   VUB Programming Technology Lab  */
             /*             (c) 2002              */
             /*-----------------------------------*/
             /*              Scanning             */
             /*-----------------------------------*/

/* public types */

typedef enum  { _AOP_TOKEN_ =  1,
                _CAT_TOKEN_ =  2,
                _CCL_TOKEN_ =  3,
                _CEQ_TOKEN_ =  4,
                _COL_TOKEN_ =  5,
                _COM_TOKEN_ =  6,
                _END_TOKEN_ =  7,
                _FRC_TOKEN_ =  8,
                _LBC_TOKEN_ =  9,
                _LBR_TOKEN_ = 10,
                _LPR_TOKEN_ = 11,
                _MOP_TOKEN_ = 12,
                _NAM_TOKEN_ = 13,
                _NBR_TOKEN_ = 14,
                _PER_TOKEN_ = 15,
                _RBC_TOKEN_ = 16,
                _RBR_TOKEN_ = 17,
                _ROP_TOKEN_ = 18,
                _RPR_TOKEN_ = 19,
                _SMC_TOKEN_ = 20,
                _TXT_TOKEN_ = 21,
                _XOP_TOKEN_ = 22,
                _QUO_TOKEN_ = 23 } _TKN_TYPE_;
                

/* public prototypes */

_NIL_TYPE_     _scan_init_(const _STR_TYPE_,
                           const _BYT_TYPE_);

_TKN_TYPE_          _scan_(      _NIL_TYPE_);

_NIL_TYPE_    _scan_error_(const _RES_TYPE_);

_LNG_TYPE_   _scan_number_(      _NIL_TYPE_);

_FLO_TYPE_ _scan_fraction_(      _NIL_TYPE_);

_STR_TYPE_   _scan_string_(      _NIL_TYPE_);
