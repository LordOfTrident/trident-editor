#pragma once

#include "_COMPONENTS"
#include "_COLORS"
#include "types.hh"
#include "window.hh"
#include "classes.hh"
#include "iohandle.hh"

namespace LUIC {
    class component {
    public:

        component ();

        virtual void draw  () = 0;
        virtual void input (i16 p_in, MEVENT* p_evt) = 0;

        void setparent (component* p_parent);
        void setioh    (iohandle* p_ioh);

        component* getparent ();
        iohandle*  getioh    ();

        void addchild (component* p_child);
        i16  getchild (component* p_child);
        void remchild (ui16 p_idx);
        void pbchild  (component* p_child);

        void setfcs  (bool p_fcs);
        bool isinfcs ();

        void totop ();

        void setvsble (bool p_vsble);
        bool getvsble ();

        void setpos  (i16 p_posx, i16 p_posy);
        ui16 getposx ();
        ui16 getposy ();

        void setsz   (ui16 p_szx, ui16 p_szy);
        ui16 getszx  ();
        ui16 getszy  ();

        void   setcolschm(colors p_colorscheme);
        colors getcolschm();

        str gettype ();

        void  setflags (flags p_flags);
        flags getflags ();

        void  __sttlps  (ui16 p_posx, ui16 p_posy);
        ui16 __gttlpsx ();
        ui16 __gttlpsy ();

        void __fcsprnts ();
        void __fxps     ();

        window* __gwndacs ();

    protected:

        str type;

        vector <component*> children;

        iohandle*  ioh;
        component* parent;
        window     wnd;

        ui16 posx,
             posy;

        colors colorscheme;

        bool infcs,
             vsble;

        flags flgs;
    };
};