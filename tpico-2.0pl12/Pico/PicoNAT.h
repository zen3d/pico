             /*-----------------------------------*/
             /*           >>>Pico 2.0<<<          */
             /*            Theo D'Hondt           */
             /*   VUB Programming Technology Lab  */
             /*             (c) 2002              */
             /*-----------------------------------*/
             /*          Native functions         */
             /*-----------------------------------*/

/*----------------------- public variables ------------------------*/

extern _EXP_TYPE_ _BEG_;
extern _EXP_TYPE_ _EMP_;
extern _EXP_TYPE_ _ESC_;
extern _EXP_TYPE_ _NUL_;
extern _EXP_TYPE_ _ONE_;
extern _EXP_TYPE_ _TAB_;
extern _EXP_TYPE_ _TWO_;
extern _EXP_TYPE_ _VOI_;
extern _EXP_TYPE_ _ZRO_;

extern _CNT_TYPE_ _eva_NAT_;

/*---------------------- public prototypes ------------------------*/

_NIL_TYPE_ _nat_interrupt_(      _NIL_TYPE_);

_EXP_TYPE_   _nat_in_cache(const _EXP_TYPE_);

_BYT_TYPE_   _nat_install_(      _NIL_TYPE_);

_BYT_TYPE_     _nat_reset_(      _NIL_TYPE_);
