/*
 * This file is generated from /home/mario/Documentos/Github_Test/1.ARQUEOPTERIX/FFMPEG-Control-API-Arqueopterix/arch_x86_linux/nasm/../../3rdplibs/nasm/asm/directiv.dat
 * by perfhash.pl; do not edit.
 */

#ifndef DIRECTIV_H
#define DIRECTIV_H 1

#include "perfhash.h"

enum directive {
    D_none,
    D_unknown,
    D_corrupt,
    D_ABSOLUTE,
    D_BITS,
    D_COMMON,
    D_CPU,
    D_DEBUG,
    D_DEFAULT,
    D_EXTERN,
    D_FLOAT,
    D_GLOBAL,
    D_LIST,
    D_SECTION,
    D_SEGMENT,
    D_WARNING,
    D_SECTALIGN,
    D_PRAGMA,
    D_EXPORT,
    D_GROUP,
    D_IMPORT,
    D_LIBRARY,
    D_MAP,
    D_MODULE,
    D_ORG,
    D_OSABI,
    D_SAFESEH,
    D_UPPERCASE,
    D_SUBSECTIONS_VIA_SYMBOLS,
    D_NO_DEAD_STRIP,
    D_MAXDUMP
};

extern const struct perfect_hash directive_hash;
extern const char * const directive_tbl[28];

static inline enum directive directive_find(const char *str)
{
    return perfhash_find(&directive_hash, str);
}

static inline const char * directive_name(enum directive x)
{
    size_t ix = (size_t)x - (3);
    if (ix >= 28)
        return NULL;
    return directive_tbl[ix];
}

static inline const char * directive_dname(enum directive x)
{
    const char *y = directive_name(x);
    return y ? y : invalid_enum_str(x);
}

#endif /* DIRECTIV_H */
