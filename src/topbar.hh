#pragma once

#include "_COMPONENTS"
#include "_COLORS"
#include "types.hh"
#include "window.hh"
#include "component.hh"
#include "iohandle.hh"

#define LUIC_TOPBAR_OPTNOTCHOSEN -1

namespace LUIC {
    class suboption {
    public:

        suboption ();
        suboption (str p_name, str p_kb);

        str          name,
                     keybind;
    };

    class option {
    public:

        option ();
        option (str p_name, vector <suboption> p_subopts);

        str name;
        vector <suboption> subopts;
    };

    class topbar: public component {
    public:

        topbar ();
        topbar (str p_ttl, vector <option> p_options);

        virtual void draw  ();
        virtual void input (i16 p_in, MEVENT* p_evt);

        void setttl     (str p_ttl);
        i8   getsboptst (ui8 p_idx);

    private:

        str    ttl;
        bool   prsd;
        window ownd;

        i16 ovr,
            subovr,
            chc,
            subchc;

        vector <option> options;

        ch  walker_sprites[4];
        i16 walker;
        i8  walker_state;
    };
};