#ifndef STROBESENSE_H
#define STROBESENSE_H


#define SS_STROBES 5
#define SS_SENSES  3

#define SS_KEY_PREV  0x0001
#define SS_KEY_RIGHT 0x0002
#define SS_KEY_NEXT  0x0004

#define SS_KEY_UP    0x0008
#define SS_KEY_ENTER 0x0010
#define SS_KEY_DOWN  0x0020

#define SS_KEY_SLOW  0x0040
#define SS_KEY_LEFT  0x0080

#define SS_KEY_PLOT  0x0200
#define SS_KEY_IDENT 0x0400
#define SS_KEY_BACK  0x0800

#define SS_KEY_FAST  0x1000
#define SS_KEY_BEAM  0x2000
#define SS_KEY_MENU  0x4000


#ifdef NO_XPORT
inline unsigned short SSKeys()
{
    return 0x0000;
}

#else //!NO_XPORT
# define SS_KEYS   *((volatile short *)0x9ffcc00)
inline unsigned short SSKeys()
{
    unsigned short ss_keys = SS_KEYS;
    if (ss_keys == 0xffff) {
        ss_keys = 0x0000;
    }
    return ss_keys;
}
#endif //!NO_XPORT

#endif

