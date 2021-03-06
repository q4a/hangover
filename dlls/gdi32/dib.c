/*
 * Copyright 2017 André Hentschel
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

/* NOTE: The guest side uses mingw's headers. The host side uses Wine's headers. */

#include <windows.h>
#include <stdio.h>
#include <usp10.h>

#include "thunk/qemu_windows.h"

#include "windows-user-services.h"
#include "dll_list.h"
#include "qemu_gdi32.h"

#ifndef QEMU_DLL_GUEST
#include <wine/debug.h>
#include <ddk/d3dkmthk.h>
WINE_DEFAULT_DEBUG_CHANNEL(qemu_gdi32);
#endif


struct qemu_StretchDIBits
{
    struct qemu_syscall super;
    uint64_t hdc;
    uint64_t xDst;
    uint64_t yDst;
    uint64_t widthDst;
    uint64_t heightDst;
    uint64_t xSrc;
    uint64_t ySrc;
    uint64_t widthSrc;
    uint64_t heightSrc;
    uint64_t bits;
    uint64_t bmi;
    uint64_t coloruse;
    uint64_t rop;
};

#ifdef QEMU_DLL_GUEST

WINBASEAPI INT WINAPI StretchDIBits(HDC hdc, INT xDst, INT yDst, INT widthDst, INT heightDst, INT xSrc, INT ySrc, INT widthSrc, INT heightSrc, const void *bits, const BITMAPINFO *bmi, UINT coloruse, DWORD rop)
{
    struct qemu_StretchDIBits call;
    call.super.id = QEMU_SYSCALL_ID(CALL_STRETCHDIBITS);
    call.hdc = (ULONG_PTR)hdc;
    call.xDst = (ULONG_PTR)xDst;
    call.yDst = (ULONG_PTR)yDst;
    call.widthDst = (ULONG_PTR)widthDst;
    call.heightDst = (ULONG_PTR)heightDst;
    call.xSrc = (ULONG_PTR)xSrc;
    call.ySrc = (ULONG_PTR)ySrc;
    call.widthSrc = (ULONG_PTR)widthSrc;
    call.heightSrc = (ULONG_PTR)heightSrc;
    call.bits = (ULONG_PTR)bits;
    call.bmi = (ULONG_PTR)bmi;
    call.coloruse = (ULONG_PTR)coloruse;
    call.rop = (ULONG_PTR)rop;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_StretchDIBits(struct qemu_syscall *call)
{
    struct qemu_StretchDIBits *c = (struct qemu_StretchDIBits *)call;
    WINE_TRACE("\n");
    c->super.iret = StretchDIBits(QEMU_G2H(c->hdc), c->xDst, c->yDst, c->widthDst, c->heightDst, c->xSrc, c->ySrc, c->widthSrc, c->heightSrc, QEMU_G2H(c->bits), QEMU_G2H(c->bmi), c->coloruse, c->rop);
}

#endif

struct qemu_SetDIBits
{
    struct qemu_syscall super;
    uint64_t hdc;
    uint64_t hbitmap;
    uint64_t startscan;
    uint64_t lines;
    uint64_t bits;
    uint64_t info;
    uint64_t coloruse;
};

#ifdef QEMU_DLL_GUEST

WINBASEAPI INT WINAPI SetDIBits(HDC hdc, HBITMAP hbitmap, UINT startscan, UINT lines, LPCVOID bits, const BITMAPINFO *info, UINT coloruse)
{
    struct qemu_SetDIBits call;
    call.super.id = QEMU_SYSCALL_ID(CALL_SETDIBITS);
    call.hdc = (ULONG_PTR)hdc;
    call.hbitmap = (ULONG_PTR)hbitmap;
    call.startscan = startscan;
    call.lines = lines;
    call.bits = (ULONG_PTR)bits;
    call.info = (ULONG_PTR)info;
    call.coloruse = coloruse;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_SetDIBits(struct qemu_syscall *call)
{
    struct qemu_SetDIBits *c = (struct qemu_SetDIBits *)call;
    WINE_TRACE("\n");
    c->super.iret = SetDIBits(QEMU_G2H(c->hdc), QEMU_G2H(c->hbitmap), c->startscan, c->lines, QEMU_G2H(c->bits), QEMU_G2H(c->info), c->coloruse);
}

#endif

struct qemu_SetDIBitsToDevice
{
    struct qemu_syscall super;
    uint64_t hdc;
    uint64_t xDest;
    uint64_t yDest;
    uint64_t cx;
    uint64_t cy;
    uint64_t xSrc;
    uint64_t ySrc;
    uint64_t startscan;
    uint64_t lines;
    uint64_t bits;
    uint64_t bmi;
    uint64_t coloruse;
};

#ifdef QEMU_DLL_GUEST

WINBASEAPI INT WINAPI SetDIBitsToDevice(HDC hdc, INT xDest, INT yDest, DWORD cx, DWORD cy, INT xSrc, INT ySrc, UINT startscan, UINT lines, LPCVOID bits, const BITMAPINFO *bmi, UINT coloruse)
{
    struct qemu_SetDIBitsToDevice call;
    call.super.id = QEMU_SYSCALL_ID(CALL_SETDIBITSTODEVICE);
    call.hdc = (ULONG_PTR)hdc;
    call.xDest = (ULONG_PTR)xDest;
    call.yDest = (ULONG_PTR)yDest;
    call.cx = (ULONG_PTR)cx;
    call.cy = (ULONG_PTR)cy;
    call.xSrc = (ULONG_PTR)xSrc;
    call.ySrc = (ULONG_PTR)ySrc;
    call.startscan = (ULONG_PTR)startscan;
    call.lines = (ULONG_PTR)lines;
    call.bits = (ULONG_PTR)bits;
    call.bmi = (ULONG_PTR)bmi;
    call.coloruse = (ULONG_PTR)coloruse;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_SetDIBitsToDevice(struct qemu_syscall *call)
{
    struct qemu_SetDIBitsToDevice *c = (struct qemu_SetDIBitsToDevice *)call;
    WINE_TRACE("\n");
    c->super.iret = SetDIBitsToDevice(QEMU_G2H(c->hdc), c->xDest, c->yDest, c->cx, c->cy, c->xSrc, c->ySrc, c->startscan, c->lines, QEMU_G2H(c->bits), QEMU_G2H(c->bmi), c->coloruse);
}

#endif

struct qemu_SetDIBColorTable
{
    struct qemu_syscall super;
    uint64_t hdc;
    uint64_t startpos;
    uint64_t entries;
    uint64_t colors;
};

#ifdef QEMU_DLL_GUEST

WINBASEAPI UINT WINAPI SetDIBColorTable(HDC hdc, UINT startpos, UINT entries, const RGBQUAD *colors)
{
    struct qemu_SetDIBColorTable call;
    call.super.id = QEMU_SYSCALL_ID(CALL_SETDIBCOLORTABLE);
    call.hdc = (ULONG_PTR)hdc;
    call.startpos = (ULONG_PTR)startpos;
    call.entries = (ULONG_PTR)entries;
    call.colors = (ULONG_PTR)colors;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_SetDIBColorTable(struct qemu_syscall *call)
{
    struct qemu_SetDIBColorTable *c = (struct qemu_SetDIBColorTable *)call;
    WINE_TRACE("\n");
    c->super.iret = SetDIBColorTable(QEMU_G2H(c->hdc), c->startpos, c->entries, QEMU_G2H(c->colors));
}

#endif

struct qemu_GetDIBColorTable
{
    struct qemu_syscall super;
    uint64_t hdc;
    uint64_t startpos;
    uint64_t entries;
    uint64_t colors;
};

#ifdef QEMU_DLL_GUEST

WINBASEAPI UINT WINAPI GetDIBColorTable(HDC hdc, UINT startpos, UINT entries, RGBQUAD *colors)
{
    struct qemu_GetDIBColorTable call;
    call.super.id = QEMU_SYSCALL_ID(CALL_GETDIBCOLORTABLE);
    call.hdc = (ULONG_PTR)hdc;
    call.startpos = (ULONG_PTR)startpos;
    call.entries = (ULONG_PTR)entries;
    call.colors = (ULONG_PTR)colors;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_GetDIBColorTable(struct qemu_syscall *call)
{
    struct qemu_GetDIBColorTable *c = (struct qemu_GetDIBColorTable *)call;
    WINE_TRACE("\n");
    c->super.iret = GetDIBColorTable(QEMU_G2H(c->hdc), c->startpos, c->entries, QEMU_G2H(c->colors));
}

#endif

struct qemu_GetDIBits
{
    struct qemu_syscall super;
    uint64_t hdc;
    uint64_t hbitmap;
    uint64_t startscan;
    uint64_t lines;
    uint64_t bits;
    uint64_t info;
    uint64_t coloruse;
};

#ifdef QEMU_DLL_GUEST

WINBASEAPI INT WINAPI GetDIBits(HDC hdc, HBITMAP hbitmap, UINT startscan, UINT lines, LPVOID bits, BITMAPINFO * info, UINT coloruse)
{
    struct qemu_GetDIBits call;
    call.super.id = QEMU_SYSCALL_ID(CALL_GETDIBITS);
    call.hdc = (ULONG_PTR)hdc;
    call.hbitmap = (ULONG_PTR)hbitmap;
    call.startscan = (ULONG_PTR)startscan;
    call.lines = (ULONG_PTR)lines;
    call.bits = (ULONG_PTR)bits;
    call.info = (ULONG_PTR)info;
    call.coloruse = (ULONG_PTR)coloruse;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

void qemu_GetDIBits(struct qemu_syscall *call)
{
    struct qemu_GetDIBits *c = (struct qemu_GetDIBits *)call;
    WINE_TRACE("\n");
    c->super.iret = GetDIBits(QEMU_G2H(c->hdc), QEMU_G2H(c->hbitmap), c->startscan, c->lines, QEMU_G2H(c->bits), QEMU_G2H(c->info), c->coloruse);
}

#endif

struct qemu_CreateDIBitmap
{
    struct qemu_syscall super;
    uint64_t hdc;
    uint64_t header;
    uint64_t init;
    uint64_t bits;
    uint64_t data;
    uint64_t coloruse;
};

#ifdef QEMU_DLL_GUEST

WINBASEAPI HBITMAP WINAPI CreateDIBitmap(HDC hdc, const BITMAPINFOHEADER *header, DWORD init, LPCVOID bits, const BITMAPINFO *data, UINT coloruse)
{
    struct qemu_CreateDIBitmap call;
    call.super.id = QEMU_SYSCALL_ID(CALL_CREATEDIBITMAP);
    call.hdc = (ULONG_PTR)hdc;
    call.header = (ULONG_PTR)header;
    call.init = (ULONG_PTR)init;
    call.bits = (ULONG_PTR)bits;
    call.data = (ULONG_PTR)data;
    call.coloruse = (ULONG_PTR)coloruse;

    qemu_syscall(&call.super);

    return (HBITMAP)(ULONG_PTR)call.super.iret;
}

#else

void qemu_CreateDIBitmap(struct qemu_syscall *call)
{
    struct qemu_CreateDIBitmap *c = (struct qemu_CreateDIBitmap *)call;
    WINE_TRACE("\n");
    c->super.iret = (ULONG_PTR)CreateDIBitmap(QEMU_G2H(c->hdc), QEMU_G2H(c->header), c->init, QEMU_G2H(c->bits), QEMU_G2H(c->data), c->coloruse);
}

#endif

struct qemu_CreateDIBSection
{
    struct qemu_syscall super;
    uint64_t hdc;
    uint64_t bmi;
    uint64_t usage;
    uint64_t bits;
    uint64_t section;
    uint64_t offset;
};

#ifdef QEMU_DLL_GUEST

WINBASEAPI HBITMAP WINAPI CreateDIBSection(HDC hdc, const BITMAPINFO *bmi, UINT usage, void **bits, HANDLE section, DWORD offset)
{
    struct qemu_CreateDIBSection call;
    call.super.id = QEMU_SYSCALL_ID(CALL_CREATEDIBSECTION);
    call.hdc = (ULONG_PTR)hdc;
    call.bmi = (ULONG_PTR)bmi;
    call.usage = usage;
    call.section = (ULONG_PTR)section;
    call.offset = offset;

    qemu_syscall(&call.super);

    if (bits)
        *bits = (void *)(ULONG_PTR)call.bits;

    return (HBITMAP)(ULONG_PTR)call.super.iret;
}

#else

void qemu_CreateDIBSection(struct qemu_syscall *call)
{
    struct qemu_CreateDIBSection *c = (struct qemu_CreateDIBSection *)call;
    void *bits;

    WINE_TRACE("\n");
    /* FIXME: Write to bits on 32 bit. */
    c->super.iret = (ULONG_PTR)CreateDIBSection((HDC)c->hdc, QEMU_G2H(c->bmi), c->usage, &bits, (HANDLE)c->section, c->offset);
    c->bits = QEMU_H2G(bits);
}

#endif

struct qemu_D3DKMTCreateDCFromMemory
{
    struct qemu_syscall super;
    uint64_t desc;
};

#ifdef QEMU_DLL_GUEST

WINBASEAPI NTSTATUS WINAPI D3DKMTCreateDCFromMemory(void *desc)
{
    struct qemu_D3DKMTCreateDCFromMemory call;
    call.super.id = QEMU_SYSCALL_ID(CALL_D3DKMTCREATEDCFROMMEMORY);
    call.desc = (ULONG_PTR)desc;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

struct qemu_D3DKMT_CREATEDCFROMMEMORY
{
    qemu_ptr        pMemory;
    D3DDDIFORMAT    Format;
    UINT            Width;
    UINT            Height;
    UINT            Pitch;
    qemu_ptr        hDeviceDc;
    qemu_ptr        pColorTable;
    qemu_ptr        hDc;
    qemu_ptr        hBitmap;
};

static inline void D3DKMT_CREATEDCFROMMEMORY_g2h(D3DKMT_CREATEDCFROMMEMORY *host,
        const struct qemu_D3DKMT_CREATEDCFROMMEMORY *guest)
{
    host->pMemory = (void *)(ULONG_PTR)guest->pMemory;
    host->Format = guest->Format;
    host->Width = guest->Width;
    host->Height = guest->Height;
    host->Pitch = guest->Pitch;
    host->hDeviceDc = (HDC)(ULONG_PTR)guest->hDeviceDc;
    host->pColorTable = (PALETTEENTRY *)(ULONG_PTR)guest->pColorTable;
    host->hDc = (HDC)(ULONG_PTR)guest->hDc;
    host->hBitmap = (HANDLE)(ULONG_PTR)guest->hBitmap;
}

static inline void D3DKMT_CREATEDCFROMMEMORY_h2g(struct qemu_D3DKMT_CREATEDCFROMMEMORY *guest,
        D3DKMT_CREATEDCFROMMEMORY *host)
{
    guest->pMemory = (ULONG_PTR)host->pMemory;
    guest->Format = host->Format;
    guest->Width = host->Width;
    guest->Height = host->Height;
    guest->Pitch = host->Pitch;
    guest->hDeviceDc = (ULONG_PTR)host->hDeviceDc;
    guest->pColorTable = (ULONG_PTR)host->pColorTable;
    guest->hDc = (ULONG_PTR)host->hDc;
    guest->hBitmap = (ULONG_PTR)host->hBitmap;
}

void qemu_D3DKMTCreateDCFromMemory(struct qemu_syscall *call)
{
    struct qemu_D3DKMTCreateDCFromMemory *c = (struct qemu_D3DKMTCreateDCFromMemory *)call;
    D3DKMT_CREATEDCFROMMEMORY stack, *desc = &stack;
    WINE_TRACE("\n");

#if GUEST_BIT == HOST_BIT
    desc = QEMU_G2H(c->desc);
#else
    if (c->desc)
        D3DKMT_CREATEDCFROMMEMORY_g2h(desc, QEMU_G2H(c->desc));
    else
        desc = NULL;
#endif

    c->super.iret = D3DKMTCreateDCFromMemory(desc);

#if GUEST_BIT != HOST_BIT
    if (c->desc)
        D3DKMT_CREATEDCFROMMEMORY_h2g(QEMU_G2H(c->desc), desc);
#endif
}

#endif

struct qemu_D3DKMTDestroyDCFromMemory
{
    struct qemu_syscall super;
    uint64_t desc;
};

#ifdef QEMU_DLL_GUEST

WINBASEAPI NTSTATUS WINAPI D3DKMTDestroyDCFromMemory(const void *desc)
{
    struct qemu_D3DKMTDestroyDCFromMemory call;
    call.super.id = QEMU_SYSCALL_ID(CALL_D3DKMTDESTROYDCFROMMEMORY);
    call.desc = (ULONG_PTR)desc;

    qemu_syscall(&call.super);

    return call.super.iret;
}

#else

struct qemu_D3DKMT_DESTROYDCFROMMEMORY
{
    qemu_ptr hDc;
    qemu_ptr hBitmap;
};

static inline void D3DKMT_DESTROYDCFROMMEMORY_g2h(D3DKMT_DESTROYDCFROMMEMORY *host,
        const struct qemu_D3DKMT_DESTROYDCFROMMEMORY *guest)
{
    host->hDc = (HDC)(ULONG_PTR)guest->hDc;
    host->hBitmap = (HANDLE)(ULONG_PTR)guest->hBitmap;
}

void qemu_D3DKMTDestroyDCFromMemory(struct qemu_syscall *call)
{
    struct qemu_D3DKMTDestroyDCFromMemory *c = (struct qemu_D3DKMTDestroyDCFromMemory *)call;
    D3DKMT_DESTROYDCFROMMEMORY stack, *desc = &stack;
    WINE_TRACE("\n");

#if GUEST_BIT == HOST_BIT
    desc = QEMU_G2H(c->desc);
#else
    if (c->desc)
        D3DKMT_DESTROYDCFROMMEMORY_g2h(desc, QEMU_G2H(c->desc));
    else
        desc = NULL;
#endif

    c->super.iret = D3DKMTDestroyDCFromMemory(desc);
}

#endif

