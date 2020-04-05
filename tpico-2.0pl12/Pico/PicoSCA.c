             /*-----------------------------------*/
             /*           >>>Pico 2.0<<<          */
             /*            Theo D'Hondt           */
             /*   VUB Programming Technology Lab  */
             /*             (c) 2002              */
             /*-----------------------------------*/
             /*              Scanning             */
             /*-----------------------------------*/

#include "Pico.h"
#include "PicoMAI.h"
#include "PicoSCA.h"

/*---------------------- private macros ---------------------------*/

#define SCAN_CHECK(allowed)\
  ((allowed >> scan_char_tab[scan_char]) & 1)

#define SCAN_CHAR()\
  if (scan_char) scan_char = _BUF_[++scan_stop]

#define SCAN_CHAR_RET(TKN)\
  { SCAN_CHAR(); return TKN; }

#define TEXT_OPEN()\
  scan_text = &_BUF_[scan_stop]

#define TEXT_CLOSE()\
  _BUF_[scan_stop] = 0

#define TEXT_CLOSE_RET(TKN)\
  { TEXT_CLOSE(); return TKN; }

#define TEXT_RESTORE()\
  _BUF_[scan_stop]= scan_char

/*----------------------- private types ---------------------------*/

typedef enum { Aop =  0,
               Apo =  1,
               Bkq =  2,
               Cat =  3,
               Col =  4,
               Com =  5,
               Dgt =  6,
               Eol =  7,
               Eql =  8,
               Exp =  9,
               Ill = 10,
               Lbc = 11,
               Lbr = 12,
               Lpr = 13,
               Ltr = 14,
               Mns = 15,
               Mop = 16,
               Per = 17,
               Pls = 18,
               Quo = 19,
               Rbc = 20,
               Rbr = 21,
               Rop = 22,
               Rpr = 23,
               Smc = 24,
               Wsp = 25,
               Xop = 26,
               Zap = 27 } scan_fun_index;

typedef _TKN_TYPE_ scan_fun(_NIL_TYPE_);

/*-------------------- private prototypes -------------------------*/

static _TKN_TYPE_ scan_Aop(_NIL_TYPE_);
static _TKN_TYPE_ scan_Apo(_NIL_TYPE_);
static _TKN_TYPE_ scan_Bkq(_NIL_TYPE_);
static _TKN_TYPE_ scan_Cat(_NIL_TYPE_);
static _TKN_TYPE_ scan_Col(_NIL_TYPE_);
static _TKN_TYPE_ scan_Com(_NIL_TYPE_);
static _TKN_TYPE_ scan_Dgt(_NIL_TYPE_);
static _TKN_TYPE_ scan_Ill(_NIL_TYPE_);
static _TKN_TYPE_ scan_Ltr(_NIL_TYPE_);
static _TKN_TYPE_ scan_Lbc(_NIL_TYPE_);
static _TKN_TYPE_ scan_Lbr(_NIL_TYPE_);
static _TKN_TYPE_ scan_Lpr(_NIL_TYPE_);
static _TKN_TYPE_ scan_Mop(_NIL_TYPE_);
static _TKN_TYPE_ scan_Per(_NIL_TYPE_);
static _TKN_TYPE_ scan_Quo(_NIL_TYPE_);
static _TKN_TYPE_ scan_Rbc(_NIL_TYPE_);
static _TKN_TYPE_ scan_Rbr(_NIL_TYPE_);
static _TKN_TYPE_ scan_Rop(_NIL_TYPE_);
static _TKN_TYPE_ scan_Rpr(_NIL_TYPE_);
static _TKN_TYPE_ scan_Smc(_NIL_TYPE_);
static _TKN_TYPE_ scan_Wsp(_NIL_TYPE_);
static _TKN_TYPE_ scan_Xop(_NIL_TYPE_);
static _TKN_TYPE_ scan_Zap(_NIL_TYPE_);

/*--------------------- private variables -------------------------*/

static _CHA_TYPE_ scan_char;
static _STR_TYPE_ scan_text;
static _BYT_TYPE_ scan_mark;
static _POS_TYPE_ scan_start;
static _POS_TYPE_ scan_stop;

static const scan_fun_index scan_char_tab[]  
   /*NUL SOH STX ETX EOT ENQ ACK BEL BS  HT  LF  VT  FF  CR  SO  SI */
  = {Zap,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Eol,Eol,Ill,Ill,Eol,Ill,Ill,
   /*DLE DC1 DC2 DC3 DC4 NAK SYN ETB CAN EM  SUB ESC FS  GS  RS  US */
     Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,
   /*     !   "   #   $   %   &   '   (   )   *   +   ,   -   .   / */
     Wsp,Xop,Quo,Rop,Aop,Aop,Mop,Apo,Lpr,Rpr,Mop,Pls,Com,Mns,Per,Mop,
   /* 0   1   2   3   4   5   6   7   8   9   :   ;   <   =   >   ? */
     Dgt,Dgt,Dgt,Dgt,Dgt,Dgt,Dgt,Dgt,Dgt,Dgt,Col,Smc,Rop,Eql,Rop,Xop,
   /* @   A   B   C   D   E   F   G   H   I   J   K   L   M   N   O */
     Cat,Ltr,Ltr,Ltr,Ltr,Exp,Ltr,Ltr,Ltr,Ltr,Ltr,Ltr,Ltr,Ltr,Ltr,Ltr,
   /* P   Q   R   S   T   U   V   W   X   Y   Z   [   \   ]   ^   _ */
     Ltr,Ltr,Ltr,Ltr,Ltr,Ltr,Ltr,Ltr,Ltr,Ltr,Ltr,Lbr,Mop,Rbr,Xop,Ltr,
   /* `   a   b   c   d   e   f   g   h   i   j   k   l   m   n   o */
     Bkq,Ltr,Ltr,Ltr,Ltr,Exp,Ltr,Ltr,Ltr,Ltr,Ltr,Ltr,Ltr,Ltr,Ltr,Ltr,
   /* p   q   r   s   t   u   v   w   x   y   z   {   |   }   ~  DEL*/
     Ltr,Ltr,Ltr,Ltr,Ltr,Ltr,Ltr,Ltr,Ltr,Ltr,Ltr,Lbc,Aop,Rbc,Rop,Ill,
   /*                                                               */
     Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,
   /*                                                               */
     Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,
   /*                                                               */
     Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,
   /*                                                               */
     Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,
   /*                                                               */
     Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,
   /*                                                               */
     Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,
   /*                                                               */
     Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,
   /*                                                               */
     Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill,Ill};
     
static scan_fun *scan_fun_tab[] = 
  { scan_Aop,
    scan_Apo,
    scan_Bkq,
    scan_Cat,
    scan_Col,
    scan_Com,
    scan_Dgt,
    scan_Wsp,
    scan_Rop,
    scan_Ltr,
    scan_Ill,
    scan_Lbc,
    scan_Lbr,
    scan_Lpr,
    scan_Ltr,
    scan_Aop,
    scan_Mop,
    scan_Per,
    scan_Aop,
    scan_Quo,
    scan_Rbc,
    scan_Rbr,
    scan_Rop,
    scan_Rpr,
    scan_Smc,
    scan_Wsp,
    scan_Xop,
    scan_Zap };

static const _UNS_TYPE_ apo_mask = (1<<Apo);
static const _UNS_TYPE_ apx_mask = (1<<Apo)+(1<<Eol)+(1<<Zap);
static const _UNS_TYPE_ bkq_mask = (1<<Bkq);
static const _UNS_TYPE_ bkx_mask = (1<<Bkq)+(1<<Eol)+(1<<Zap);
static const _UNS_TYPE_ col_mask = (1<<Col);
static const _UNS_TYPE_ dgt_mask = (1<<Dgt);
static const _UNS_TYPE_ eql_mask = (1<<Eql);
static const _UNS_TYPE_ exp_mask = (1<<Exp);
static const _UNS_TYPE_ nam_mask = (1<<Dgt)+(1<<Exp)+(1<<Ltr);
static const _UNS_TYPE_ opr_mask = (1<<Aop)+(1<<Eql)+(1<<Mns)+
                                   (1<<Mop)+(1<<Pls)+(1<<Rop)+
                                   (1<<Xop);
static const _UNS_TYPE_ per_mask = (1<<Per);
static const _UNS_TYPE_ quo_mask = (1<<Quo);
static const _UNS_TYPE_ qux_mask = (1<<Eol)+(1<<Quo)+(1<<Zap);
static const _UNS_TYPE_ sgn_mask = (1<<Mns)+(1<<Pls);
static const _UNS_TYPE_ wsp_mask = (1<<Eol)+(1<<Wsp);

/*--------------------- private functions -------------------------*/

static _TKN_TYPE_ scan_operator(const _TKN_TYPE_ Tkn) 
  { TEXT_OPEN();
    do 
      SCAN_CHAR();
    while (SCAN_CHECK(opr_mask));
    TEXT_CLOSE_RET(Tkn); }
  
static _TKN_TYPE_ scan_name(const _TKN_TYPE_ Tkn)
  { TEXT_OPEN();
    do 
      SCAN_CHAR();
    while (SCAN_CHECK(nam_mask));
    TEXT_CLOSE_RET(Tkn); }
  
static _TKN_TYPE_ scan_exponent(_NIL_TYPE_)
  { SCAN_CHAR(); 
    if (SCAN_CHECK(sgn_mask))
      SCAN_CHAR(); 
    if (!SCAN_CHECK(dgt_mask))
      _scan_error_(_DIG_ERROR_);
    do 
      SCAN_CHAR(); 
    while (SCAN_CHECK(dgt_mask));
    TEXT_CLOSE_RET(_FRC_TOKEN_); }

static _TKN_TYPE_ scan_fraction(_NIL_TYPE_)
  { SCAN_CHAR(); 
    if (!SCAN_CHECK(dgt_mask))
      _scan_error_(_DIG_ERROR_);
    do 
      SCAN_CHAR(); 
    while (SCAN_CHECK(dgt_mask));
    if (SCAN_CHECK(exp_mask))
      return scan_exponent();
    TEXT_CLOSE_RET(_FRC_TOKEN_); }

static _TKN_TYPE_ scan_number(_NIL_TYPE_)
  { TEXT_OPEN();
    do 
      SCAN_CHAR();      
    while (SCAN_CHECK(dgt_mask));
    if (SCAN_CHECK(per_mask))
      return scan_fraction();
    if (SCAN_CHECK(exp_mask))
      return scan_exponent(); 
    TEXT_CLOSE_RET(_NBR_TOKEN_); }
        
static _TKN_TYPE_ scan_Aop(_NIL_TYPE_)
  { return scan_operator(_AOP_TOKEN_); }
  
static _TKN_TYPE_ scan_Apo(_NIL_TYPE_)
  { SCAN_CHAR_RET(_QUO_TOKEN_); }
  
static _TKN_TYPE_ scan_Bkq(_NIL_TYPE_)
  { do
      SCAN_CHAR();
    while (!SCAN_CHECK(bkx_mask));
    if (SCAN_CHECK(bkq_mask)) 
      SCAN_CHAR();
    return _scan_(); }
  
static _TKN_TYPE_ scan_Cat(_NIL_TYPE_)
  { SCAN_CHAR_RET(_CAT_TOKEN_); }
  
static _TKN_TYPE_ scan_Col(_NIL_TYPE_) 
  { SCAN_CHAR();
    if (SCAN_CHECK(eql_mask))
      SCAN_CHAR_RET(_CEQ_TOKEN_);
    if (SCAN_CHECK(col_mask))
      SCAN_CHAR_RET(_CCL_TOKEN_);
    return _COL_TOKEN_; }
  
static _TKN_TYPE_ scan_Com(_NIL_TYPE_) 
  { SCAN_CHAR_RET(_COM_TOKEN_); }

static _TKN_TYPE_ scan_Dgt(_NIL_TYPE_)
  { return scan_number(); }

static _TKN_TYPE_ scan_Ill(_NIL_TYPE_)
  { _scan_error_(_ILL_ERROR_); 
    return _END_TOKEN_; }
  
static _TKN_TYPE_ scan_Lbc(_NIL_TYPE_)
  { SCAN_CHAR_RET(_LBC_TOKEN_); }
  
static _TKN_TYPE_ scan_Lbr(_NIL_TYPE_)
  { SCAN_CHAR_RET(_LBR_TOKEN_); }
  
static _TKN_TYPE_ scan_Lpr(_NIL_TYPE_)
  { SCAN_CHAR_RET(_LPR_TOKEN_); }
  
static _TKN_TYPE_ scan_Ltr(_NIL_TYPE_)
  { return scan_name(_NAM_TOKEN_); }
  
static _TKN_TYPE_ scan_Mop(_NIL_TYPE_)
  { return scan_operator(_MOP_TOKEN_); }
  
static _TKN_TYPE_ scan_Per(_NIL_TYPE_)
  { SCAN_CHAR_RET(_PER_TOKEN_); }
  
static _TKN_TYPE_ scan_Quo(_NIL_TYPE_)
  { SCAN_CHAR();
    TEXT_OPEN();
    while (!SCAN_CHECK(qux_mask))
      SCAN_CHAR();
    if (!SCAN_CHECK(quo_mask))
      _scan_error_(_TXT_ERROR_);
    TEXT_CLOSE();
    SCAN_CHAR_RET(_TXT_TOKEN_); }
  
static _TKN_TYPE_ scan_Rbc(_NIL_TYPE_)
  { SCAN_CHAR_RET(_RBC_TOKEN_); }
    
static _TKN_TYPE_ scan_Rbr(_NIL_TYPE_)
  { SCAN_CHAR_RET(_RBR_TOKEN_); }
    
static _TKN_TYPE_ scan_Rop(_NIL_TYPE_)
  { return scan_operator(_ROP_TOKEN_); }
  
static _TKN_TYPE_ scan_Rpr(_NIL_TYPE_)
  { SCAN_CHAR_RET(_RPR_TOKEN_); }
  
static _TKN_TYPE_ scan_Smc(_NIL_TYPE_) 
  { SCAN_CHAR_RET(_SMC_TOKEN_); }
  
static _TKN_TYPE_ scan_Wsp(_NIL_TYPE_)
  { do 
      SCAN_CHAR();
    while (SCAN_CHECK(wsp_mask));
    if (scan_char) 
      return _scan_();
    return _END_TOKEN_; }
  
static _TKN_TYPE_ scan_Xop(_NIL_TYPE_)
  { return scan_operator(_XOP_TOKEN_); }
  
static _TKN_TYPE_ scan_Zap(_NIL_TYPE_)
  { return _END_TOKEN_; }
  
/*----------------------- public functions ------------------------*/

_NIL_TYPE_ _scan_init_(const _STR_TYPE_ Str, 
                       const _BYT_TYPE_ Tst)
  { strncpy(_BUF_, Str, _BUF_SIZE_);
    scan_start = 0;
    scan_stop  = 0;
    scan_mark  = Tst;
    scan_text  = Str;
    scan_char  = _BUF_[0]; }

_TKN_TYPE_ _scan_(_NIL_TYPE_)
  { scan_start = scan_stop;  
    TEXT_RESTORE();
    return (*scan_fun_tab[scan_char_tab[scan_char]])(); }
      
_NIL_TYPE_ _scan_error_(const _RES_TYPE_ Err)
  { if (scan_mark)
      _mark_(scan_start, scan_stop+1);
    _error_(Err); }

_LNG_TYPE_ _scan_number_(_NIL_TYPE_)
  { _LNG_TYPE_ lng;
    errno = 0;
    lng = atol(scan_text);
    if (errno == ERANGE)
      _scan_error_(_NBR_ERROR_);
    return lng; }

_FLO_TYPE_ _scan_fraction_(_NIL_TYPE_)
  { _FLO_TYPE_ flo;
    errno = 0;
    flo = atof(scan_text);
    if (errno == ERANGE)
      _scan_error_(_FRC_ERROR_);
    return flo; }
   
_STR_TYPE_ _scan_string_(_NIL_TYPE_)
  { return scan_text; }
