             /*-----------------------------------*/
             /*           >>>Pico 2.0<<<          */
             /*            Theo D'Hondt           */
             /*   VUB Programming Technology Lab  */
             /*             (c) 2002              */
             /*-----------------------------------*/
             /*          Pico header file         */
             /*-----------------------------------*/

/*-----------------------------------------------------------------*/
/*   _DO_ESCAPE_  ->  escape requested                             */
/*   _DO_LOAD_    ->  load requested                               */
/*   _DO_ACCEPT_  ->  accept requested                             */
/*   _PICO_DONE_  ->  done                                         */
/*   _AGR_ERROR_  ->  abstract grammar violation                   */
/*   _ALS_ERROR_  ->  argument list size incompatibility           */
/*   _ALT_ERROR_  ->  argument list type violation                 */
/*   _ARF_ERROR_  ->  application requires function                */
/*   _ATC_ERROR_  ->  argument type conflict                       */
/*   _BUF_ERROR_  ->  text buffer overflow                         */
/*   _CAL_ERROR_  ->  call mismatch                                */
/*   _CAS_ERROR_  ->  case argument error                          */
/*   _CCA_ERROR_  ->  case clauses absent                          */
/*   _CCH_ERROR_  ->  cache overflow                               */
/*   _CTL_ERROR_  ->  control violation                            */
/*   _DCT_ERROR_  ->  duplicate case tag                           */
/*   _DDC_ERROR_  ->  duplicate default clause                     */
/*   _DIG_ERROR_  ->  digit required                               */
/*   _DPS_ERROR_  ->  duplicate session identifier                 */
/*   _ETI_ERROR_  ->  empty tabulation index                       */
/*   _EXP_ERROR_  ->  expression violation                         */
/*   _EXT_ERROR_  ->  excess token(s)                              */
/*   _FRC_ERROR_  ->  fraction too large                           */
/*   _IAG_ERROR_  ->  invalid argument                             */
/*   _IBS_ERROR_  ->  index beyond size                            */
/*   _ICC_ERROR_  ->  invalid case clause                          */
/*   _ICT_ERROR_  ->  illegal case tag                             */
/*   _IDX_ERROR_  ->  table index violation                        */
/*   _IIX_ERROR_  ->  invalid index                                */
/*   _ILL_ERROR_  ->  illegal character                            */
/*   _INF_ERROR_  ->  infinite value                               */
/*   _INI_ERROR_  ->  illegal native index                         */
/*   _INV_ERROR_  ->  invocation required                          */
/*   _IPM_ERROR_  ->  invalid parameter                            */
/*   _IPT_ERROR_  ->  illegal parameter type                       */
/*   _MEM_ERROR_  ->  storage overflow                             */
/*   _MTI_ERROR_  ->  malformed tabulation index                   */
/*   _NAB_ERROR_  ->  not a boolean                                */
/*   _NAC_ERROR_  ->  not a constant                               */
/*   _NAD_ERROR_  ->  not a dictionary                             */
/*   _NAF_ERROR_  ->  not a function                               */
/*   _NAT_ERROR_  ->  not a table                                  */
/*   _NBR_ERROR_  ->  number too large                             */
/*   _NEG_ERROR_  ->  negative argument                            */
/*   _NMA_ERROR_  ->  non-matching argument list                   */
/*   _NNI_ERROR_  ->  non numeric index                            */
/*   _NNS_ERROR_  ->  non numeric size                             */
/*   _NPI_ERROR_  ->  non positive index                           */
/*   _NPS_ERROR_  ->  non positive size                            */
/*   _OPD_ERROR_  ->  operand expected                             */
/*   _PLE_ERROR_  ->  parameter list not empty                     */
/*   _PLT_ERROR_  ->  parameter list type violation                */
/*   _QRO_ERROR_  ->  qualification requires operation             */
/*   _RBC_ERROR_  ->  right brace expected                         */
/*   _RBR_ERROR_  ->  right bracket expected                       */
/*   _REE_ERROR_  ->  reentrancy violation (obsolete)              */
/*   _RNG_ERROR_  ->  range error                                  */
/*   _RPR_ERROR_  ->  right parenthesis expected                   */
/*   _SET_ERROR_  ->  invalid assignment                           */
/*   _SIZ_ERROR_  ->  invalid size                                 */
/*   _SNA_ERROR_  ->  session not active                           */
/*   _STK_ERROR_  ->  stack error                                  */
/*   _TAG_ERROR_  ->  invalid tag                                  */
/*   _TIR_ERROR_  ->  tabulation into raw data                     */
/*   _TMN_ERROR_  ->  too many names                               */
/*   _TMS_ERROR_  ->  too many sessions                            */
/*   _TRI_ERROR_  ->  tabulation requires index                    */
/*   _TXT_ERROR_  ->  non-terminated text                          */
/*   _UDI_ERROR_  ->  undefined identifier                         */
/*   _USR_ERROR_  ->  user error                                   */
/*   _VAR_ERROR_  ->  variable required                            */
/*   _ZDV_ERROR_  ->  zero division                                */
/*   _ZTI_ERROR_  ->  zero tabulation index                        */
/*   _NGS_ERROR_  ->  negative size                                */
/*-----------------------------------------------------------------*/

/*----------------------- public constants ------------------------*/

enum { _PICO_MAX_ = 10 };

/*------------------------- public types --------------------------*/

typedef              void   _NIL_TYPE_;
typedef              void * _ADR_TYPE_;
typedef      unsigned int   _POS_TYPE_;
typedef      unsigned int   _SES_TYPE_;
typedef      unsigned int   _SIZ_TYPE_;
typedef      unsigned int   _CLL_TYPE_;
typedef               int   _INT_TYPE_;                    
typedef     unsigned char   _CHA_TYPE_;  
typedef            double   _FLO_TYPE_;    
typedef            double * _FLP_TYPE_;                    
typedef              char * _STR_TYPE_;

typedef enum { _DO_ESCAPE_   = -3,
               _DO_LOAD_     = -2,
               _DO_ACCEPT_   = -1,
               _PICO_DONE_   =  0,
               _AGR_ERROR_   =  1,
               _ALS_ERROR_   =  2,
               _ALT_ERROR_   =  3,
               _ARF_ERROR_   =  4,
               _ATC_ERROR_   =  5,
               _BUF_ERROR_   =  6,
               _CAL_ERROR_   =  7,
               _CAS_ERROR_   =  8,
               _CCA_ERROR_   =  9,
               _CCH_ERROR_   = 10,
               _CTL_ERROR_   = 11,
               _DCT_ERROR_   = 12,
               _DDC_ERROR_   = 13,
               _DIG_ERROR_   = 14,
               _DPS_ERROR_   = 15,
               _ETI_ERROR_   = 16,
               _EXP_ERROR_   = 17,
               _EXT_ERROR_   = 18,
               _FRC_ERROR_   = 19,
               _IAG_ERROR_   = 20,
               _IBS_ERROR_   = 21,
               _ICC_ERROR_   = 22,
               _ICT_ERROR_   = 23,
               _IDX_ERROR_   = 24,
               _IIX_ERROR_   = 25,
               _ILL_ERROR_   = 26,
               _INF_ERROR_   = 27,
               _INI_ERROR_   = 28,
               _INV_ERROR_   = 29,
               _IPM_ERROR_   = 30,
               _IPT_ERROR_   = 31,
               _MEM_ERROR_   = 32,
               _MTI_ERROR_   = 33,
               _NAB_ERROR_   = 34,
               _NAC_ERROR_   = 35,
               _NAD_ERROR_   = 36,
               _NAF_ERROR_   = 37,
               _NAT_ERROR_   = 38,
               _NBR_ERROR_   = 39,
               _NEG_ERROR_   = 40,
               _NMA_ERROR_   = 41,
               _NNI_ERROR_   = 42,
               _NNS_ERROR_   = 43,
               _NPI_ERROR_   = 44,
               _NPS_ERROR_   = 45,
               _OPD_ERROR_   = 46,
               _PLE_ERROR_   = 47,
               _PLT_ERROR_   = 48,
               _QRO_ERROR_   = 49,
               _RBC_ERROR_   = 50,
               _RBR_ERROR_   = 51,
               _REE_ERROR_   = 52,
               _RNG_ERROR_   = 53,
               _RPR_ERROR_   = 54,
               _SET_ERROR_   = 55,
               _SIZ_ERROR_   = 56,
               _SNA_ERROR_   = 57,
               _STK_ERROR_   = 58,
               _TAG_ERROR_   = 59,
               _TIR_ERROR_   = 60,
               _TMN_ERROR_   = 61,
               _TMS_ERROR_   = 62,
               _TRI_ERROR_   = 63,
               _TXT_ERROR_   = 64,
               _UDI_ERROR_   = 65,
               _USR_ERROR_   = 66,
               _VAR_ERROR_   = 67,
               _ZDV_ERROR_   = 68,
               _ZTI_ERROR_   = 69,
               _NGS_ERROR_   = 70 } _RES_TYPE_;

typedef enum { _MARK_NO_     =  0,
               _MARK_YES_    =  1 } _MRK_TYPE_;

typedef enum { _ESCAPE_NO_   =  0,
               _ESCAPE_YES_  =  1 } _ESC_TYPE_;

typedef enum { _RECLAIM_NO_  =  0,
               _RECLAIM_YES_ =  1 } _RCL_TYPE_;
                         
/*--------------------- callback prototypes -----------------------*/

/*-----------------------------------------------------------------*/
/*  _PICO_PRINT_:                                                  */
/*     called when *print* peforms output                          */
/*                                                                 */
/*  arguments:                                                     */
/*     _SES_TYPE_             ->     session identity              */
/*     _STR_TYPE_             ->     print string                  */
/*                                                                 */
/*  returns: none                                                  */
/*-----------------------------------------------------------------*/

_NIL_TYPE_ _PICO_PRINT_(const _SES_TYPE_,
                        const _STR_TYPE_);

/*-----------------------------------------------------------------*/
/*  _PICO_ESCAPE_:                                                 */
/*     periodically called by *eval* to check for escape           */
/*                                                                 */
/*  arguments:                                                     */
/*     _SES_TYPE_             ->     session identity              */
/*                                                                 */
/*  returns:                                                       */
/*     _ESC_TYPE_             ->     signals request for escape    */
/*-----------------------------------------------------------------*/

_ESC_TYPE_ _PICO_ESCAPE_(const _SES_TYPE_);

/*-----------------------------------------------------------------*/
/*  _PICO_DISPLAY_:                                                */
/*     called by *eval* to display a string                        */
/*                                                                 */
/*  arguments:                                                     */
/*     _SES_TYPE_             ->     session identity              */
/*     _STR_TYPE_             ->     string to be displayed        */
/*                                                                 */
/*  returns:                                                       */
/*     _ESC_TYPE_             ->     signals request for escape    */
/*-----------------------------------------------------------------*/

_ESC_TYPE_ _PICO_DISPLAY_(const _SES_TYPE_,
                          const _STR_TYPE_);

/*-----------------------------------------------------------------*/
/*  _PICO_MARK_:                                                   */
/*     called by *read* to mark a token                            */
/*                                                                 */
/*  arguments:                                                     */
/*     _SES_TYPE_             ->     session identity              */
/*     _POS_TYPE_             ->     start position of token       */
/*     _POS_TYPE_             ->     end position of token         */
/*                                                                 */
/*  returns: none                                                  */
/*-----------------------------------------------------------------*/

_NIL_TYPE_ _PICO_MARK_(const _SES_TYPE_, 
                       const _POS_TYPE_,
                       const _POS_TYPE_);

/*-----------------------------------------------------------------*/
/*  _PICO_MESSAGE_:                                                */
/*     called by *read* or *eval* to set an optional error message */
/*                                                                 */
/*  arguments:                                                     */
/*     _SES_TYPE_             ->     session identity              */
/*     _STR_TYPE_             ->     message string                */
/*                                                                 */
/*  returns: none                                                  */
/*-----------------------------------------------------------------*/

_NIL_TYPE_ _PICO_MESSAGE_(const _SES_TYPE_, 
                          const _STR_TYPE_);

/*-----------------------------------------------------------------*/
/*  _PICO_RECLAIM_:                                                */
/*      called before and after garbage collection                 */
/*                                                                 */
/*  arguments:                                                     */
/*     _RCL_TYPE_             ->     start/stop garbage collection */
/*                                                                 */
/*  returns: none                                                  */
/*-----------------------------------------------------------------*/

_NIL_TYPE_ _PICO_RECLAIM_(const _RCL_TYPE_);

/*--------------------- exported prototypes -----------------------*/

/*-----------------------------------------------------------------*/
/*  _PICO_INIT_:                                                   */
/*     set up storage for pico                                     */
/*                                                                 */
/*  arguments:                                                     */
/*     _ADR_TYPE_             ->     address of storage            */
/*     _SIZ_TYPE_             ->     size of storage               */
/*                                                                 */
/*  returns:                                                       */
/*     _RES_TYPE_             ->     result status                 */
/*-----------------------------------------------------------------*/

_RES_TYPE_ _PICO_INIT_(const _ADR_TYPE_,
                       const _SIZ_TYPE_);

/*-----------------------------------------------------------------*/
/*  _PICO_SESSION_:                                                */
/*     activation of pico session                                  */
/*                                                                 */
/*  arguments:                                                     */
/*     _SES_TYPE_             ->     session identity              */
/*                                                                 */
/*  returns:                                                       */
/*     _RES_TYPE_             ->     result status                 */
/*-----------------------------------------------------------------*/

_RES_TYPE_ _PICO_SESSION_(const _SES_TYPE_);

/*-----------------------------------------------------------------*/
/*  _PICO_DO_:                                                     */
/*     evaluation of a pico expression                             */
/*                                                                 */
/*  arguments:                                                     */
/*     _SES_TYPE_             ->     session identity              */
/*     _STR_TYPE_             ->     input string                  */
/*     _MRK_TYPE_             ->     error marking                 */
/*                                                                 */
/*  returns:                                                       */
/*     _RES_TYPE_             ->     result status                 */
/*-----------------------------------------------------------------*/

_RES_TYPE_ _PICO_DO_(const _SES_TYPE_,
                     const _STR_TYPE_,
                     const _MRK_TYPE_);

/*-----------------------------------------------------------------*/
/*  _PICO_ACCEPT_:                                                 */
/*     continuation of a pico session after accepted user input    */
/*                                                                 */
/*  arguments:                                                     */
/*     _SES_TYPE_             ->     session identity              */
/*     _STR_TYPE_             ->     input string                  */
/*                                                                 */
/*  returns:                                                       */
/*     _RES_TYPE_             ->     result status                 */
/*-----------------------------------------------------------------*/

_RES_TYPE_ _PICO_ACCEPT_(const _SES_TYPE_,
                         const _STR_TYPE_);

/*-----------------------------------------------------------------*/
/*  _PICO_LOAD_:                                                   */
/*     load of a pico expression                                   */
/*                                                                 */
/*  arguments:                                                     */
/*     _SES_TYPE_             ->     session identity              */
/*     _STR_TYPE_             ->     load string                   */
/*                                                                 */
/*  returns:                                                       */
/*     _RES_TYPE_             ->     result status                 */
/*-----------------------------------------------------------------*/

_RES_TYPE_ _PICO_LOAD_(const _SES_TYPE_,
                       const _STR_TYPE_);

/*-----------------------------------------------------------------*/
/*  _PICO_CONTINUE_:                                               */
/*     continuation of a pico session after interrupt              */
/*                                                                 */
/*  arguments:                                                     */
/*     _SES_TYPE_             ->     session identity              */
/*                                                                 */
/*  returns:                                                       */
/*     _RES_TYPE_             ->     result status                 */
/*-----------------------------------------------------------------*/

_RES_TYPE_ _PICO_CONTINUE_(const _SES_TYPE_);

/*-----------------------------------------------------------------*/
/*  _PICO_KILL_:                                                   */
/*     deactivation of a pico session                              */
/*                                                                 */
/*  arguments:                                                     */
/*     _SES_TYPE_             ->     session identity              */
/*                                                                 */
/*  returns:                                                       */
/*     _RES_TYPE_             ->     result status                 */
/*-----------------------------------------------------------------*/

_RES_TYPE_ _PICO_KILL_(const _SES_TYPE_);
