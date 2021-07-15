#pragma once

#include "_COMPONENTS"
#include "_COLORS"
#include "_FLAGS"
#include "types.hh"
#include "window.hh"
#include "classes.hh"
#include "component.hh"
#include "screen.hh"

#define __LUIC__SHDWCLR 247

#define __LUIC__WNDCLR  250
#define __LUIC__WNDDEF  249
#define __LUIC__WNDGRN  245
#define __LUIC__SYSGRN  207
#define __LUIC__BCKGRN  208
#define __LUIC__WBACLR  240
#define __LUIC__WBBCLR  230

#define __LUIC__HGLTCLR 251
#define __LUIC__BTNCLR  252
#define __LUIC__BTNPCLR 246
#define __LUIC__BPRSCLR 253
#define __LUIC__BARCLR  254
#define __LUIC__HGHLGHT 241
#define __LUIC__ABCKCLR 202
#define __LUIC__BCKCLR  255
#define __LUIC__SYSCLR  222
#define __LUIC__COMMON  200
#define __LUIC__TXTCLR  233
#define __LUIC__TXTNFCS 232

#define LUIC_TYPE_FRAME   "frame"
#define LUIC_TYPE_LABEL   "label"
#define LUIC_TYPE_BUTTON  "button"
#define LUIC_TYPE_TOPBAR  "topbar"
#define LUIC_TYPE_TEXTBAR "textbar"
#define LUIC_TYPE_EDITOR  "editor"

namespace LUIC {
    class iohandle {
    public:

        iohandle ();
        iohandle (str p_ttl, flags p_flags);

        ~iohandle ();

        void draw  ();
        i16  input ();

        void addclr   (ui8 p_id, color p_bg, color p_fg);
        void addchild (component* p_child);
        i16  getchild (component* p_child);
        void remchild (ui16 p_idx);
        void pbchild  (component* p_child);

        ui16 getwsizex ();
        ui16 getwsizey ();

        void initthm (bool p_shdw);

        void setflgs (flags p_flags);
        void setttl  (str p_ttl);
        void setbody (str p_body);

        bool __hasshdw    ();

        bool __gchldfcsed ();
        void __schldfcsed (bool p_fcsed);

        ui32 gettick();

    private:

        component* wndtopbar;

        vector <component*> children;

        ui16 wsizex,
             wsizey;

        bool shdw,
             chldfcsed;

        ui16 xprev, 
             yprev;

        str   ttl,
              body;
        flags flgs;

        ui32 tick;
    };
};