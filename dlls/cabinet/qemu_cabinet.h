/*
 * Copyright 2017 André Hentschel
 * Copyright 2018 Stefan Dösinger for CodeWeavers
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

#ifndef QEMU_CABINET_H
#define QEMU_CABINET_H

enum cabinet_calls
{
    CALL_DLLGETVERSION = 0,
    CALL_EXTRACT,
    CALL_FCIADDFILE,
    CALL_FCICREATE,
    CALL_FCIDESTROY,
    CALL_FCIFLUSHCABINET,
    CALL_FCIFLUSHFOLDER,
    CALL_FDICOPY,
    CALL_FDICREATE,
    CALL_FDIDESTROY,
    CALL_FDIISCABINET,
    CALL_FDITRUNCATECABINET,
    CALL_INIT_DLL,
};

/* Taken from dlls/cabinet/cabinet.h */
struct FILELIST
{
    char *FileName;
    struct FILELIST *next;
    BOOL DoExtract;
};

#define EXTRACT_FILLFILELIST  0x00000001

typedef struct
{
    INT FileSize;
    ERF Error;
    struct FILELIST *FileList;
    INT FileCount;
    INT Operation;
    CHAR Destination[MAX_PATH];
    CHAR CurrentFile[MAX_PATH];
    CHAR Reserved[MAX_PATH];
    struct FILELIST *FilterList;
} SESSION;

struct qemu_FILELIST
{
    qemu_ptr FileName;
    qemu_ptr next;
    BOOL DoExtract;
};

/* The chaining is handled by the caller. */
static inline void FILELIST_g2h(struct FILELIST *host, const struct qemu_FILELIST *guest)
{
    host->FileName = (char *)(ULONG_PTR)guest->FileName;
    host->next = (struct FILELIST *)(ULONG_PTR)guest->next;
    host->DoExtract = guest->DoExtract;
}

static inline void FILELIST_h2g(struct qemu_FILELIST *guest, const struct FILELIST *host)
{
    guest->FileName = (ULONG_PTR)host->FileName;
    guest->next = (ULONG_PTR)host->next;
    guest->DoExtract = host->DoExtract;
}

struct qemu_SESSION
{
    INT FileSize;
    ERF Error;
    qemu_ptr FileList;
    INT FileCount;
    INT Operation;
    CHAR Destination[MAX_PATH];
    CHAR CurrentFile[MAX_PATH];
    CHAR Reserved[MAX_PATH];
    qemu_ptr FilterList;
};

static inline void SESSION_g2h(SESSION *host, const struct qemu_SESSION *guest)
{
    host->FileSize = guest->FileSize;
    host->Error = guest->Error;
    host->FileList = (struct FILELIST *)(ULONG_PTR)guest->FileList;
    host->FileCount = guest->FileCount;
    host->Operation = guest->Operation;
    memcpy(host->Destination, guest->Destination, sizeof(host->Destination));
    memcpy(host->CurrentFile, guest->CurrentFile, sizeof(host->CurrentFile));
    memcpy(host->Reserved, guest->Reserved, sizeof(host->Reserved));
    host->FilterList = (struct FILELIST *)(ULONG_PTR)guest->FilterList;
}

static inline void SESSION_h2g(struct qemu_SESSION *guest, const SESSION *host)
{
    guest->FileSize = host->FileSize;
    guest->Error = host->Error;
    guest->FileList = (ULONG_PTR)host->FileList;
    guest->FileCount = host->FileCount;
    guest->Operation = host->Operation;
    memcpy(guest->Destination, host->Destination, sizeof(guest->Destination));
    memcpy(guest->CurrentFile, host->CurrentFile, sizeof(guest->CurrentFile));
    memcpy(guest->Reserved, host->Reserved, sizeof(guest->Reserved));
    guest->FilterList = (ULONG_PTR)host->FilterList;
}

#ifdef QEMU_DLL_GUEST

struct FCI_alloc_cb;
struct FCI_free_cb;
struct FCI_open_info_cb;
struct FCI_read_cb;
struct FCI_close_cb;
struct FCI_progress_cb;
struct FCI_dest_cb;
struct FCI_open_cb;
struct FCI_temp_cb;
struct FCI_delete_cb;
struct FCI_seek_cb;

struct FDI_read_cb;
struct FDI_open_cb;
struct FDI_close_cb;
struct FDI_progress_cb;
struct FDI_seek_cb;

void * __fastcall fci_alloc_guest(struct FCI_alloc_cb *call);
INT_PTR __fastcall fci_open_info_guest(struct FCI_open_info_cb *call);
UINT __fastcall fci_readwrite_guest(struct FCI_read_cb *call);
int __fastcall fci_close_guest(struct FCI_close_cb *call);
LONG __fastcall fci_progress_guest(struct FCI_progress_cb *call);
int __fastcall fci_dest_guest(struct FCI_dest_cb *call);
INT_PTR __fastcall fci_open_guest(struct FCI_open_cb *call);
void __fastcall fci_free_guest(struct FCI_free_cb *call);
BOOL __fastcall fci_temp_guest(struct FCI_temp_cb *call);
int __fastcall fci_delete_guest(struct FCI_delete_cb *call);
LONG __fastcall fci_seek_guest(struct FCI_seek_cb *call);

UINT __fastcall fdi_readwrite_guest(struct FDI_read_cb *call);
INT_PTR __fastcall fdi_open_guest(struct FDI_open_cb *call);
int __fastcall fdi_close_guest(struct FDI_close_cb *call);
INT_PTR __fastcall fdi_progress_guest(struct FDI_progress_cb *call);
LONG __fastcall fdi_seek_guest(struct FDI_seek_cb *call);

#else

extern const struct qemu_ops *qemu_ops;

extern uint64_t fci_dest_guest;
extern uint64_t fci_alloc_guest;
extern uint64_t fci_free_guest;
extern uint64_t fci_open_guest;
extern uint64_t fci_readwrite_guest;
extern uint64_t fci_close_guest;
extern uint64_t fci_seek_guest;
extern uint64_t fci_delete_guest;
extern uint64_t fci_temp_guest;
extern uint64_t fci_open_info_guest;
extern uint64_t fci_progress_guest;

extern uint64_t fdi_readwrite_guest;
extern uint64_t fdi_open_guest;
extern uint64_t fdi_close_guest;
extern uint64_t fdi_progress_guest;
extern uint64_t fdi_seek_guest;

void qemu_FCIAddFile(struct qemu_syscall *call);
void qemu_FCICreate(struct qemu_syscall *call);
void qemu_FCIDestroy(struct qemu_syscall *call);
void qemu_FCIFlushCabinet(struct qemu_syscall *call);
void qemu_FCIFlushFolder(struct qemu_syscall *call);
void qemu_FDICopy(struct qemu_syscall *call);
void qemu_FDICreate(struct qemu_syscall *call);
void qemu_FDIDestroy(struct qemu_syscall *call);
void qemu_FDIIsCabinet(struct qemu_syscall *call);
void qemu_FDITruncateCabinet(struct qemu_syscall *call);

void * CDECL host_alloc(ULONG cb);
void CDECL host_free(void *memory);

struct qemu_fxi
{
    union
    {
        HFCI fci;
        HFDI fdi;
    } host;

    uint64_t dest, alloc, free, open, read, write, close, seek, del, temp, open_info, progress;
};

/* Use posix TLS TlsGetValue and TlsSetValue mess with the last error. __thread
 * doesn't work for some reason and corrupts the call stack in our callbacks. */
extern pthread_key_t cabinet_tls;

extern HFCI (CDECL *p_FCICreate)(PERF perf, PFNFCIFILEPLACED pfnfiledest, PFNFCIALLOC pfnalloc, PFNFCIFREE pfnfree,
        PFNFCIOPEN pfnopen, PFNFCIREAD pfnread, PFNFCIWRITE pfnwrite, PFNFCICLOSE pfnclose, PFNFCISEEK pfnseek,
        PFNFCIDELETE pfndelete, PFNFCIGETTEMPFILE pfnfcigtf, PCCAB pccab, void *pv);
extern BOOL (CDECL *p_FCIAddFile)(HFCI hfci, char *pszSourceFile, char *pszFileName, BOOL fExecute,
        PFNFCIGETNEXTCABINET pfnfcignc, PFNFCISTATUS pfnfcis, PFNFCIGETOPENINFO pfnfcigoi, TCOMP typeCompress);
extern BOOL (CDECL *p_FCIFlushFolder)(HFCI hfci, PFNFCIGETNEXTCABINET pfnfcignc, PFNFCISTATUS pfnfcis);
extern BOOL (CDECL *p_FCIFlushCabinet)(HFCI hfci, BOOL fGetNextCab, PFNFCIGETNEXTCABINET pfnfcignc, PFNFCISTATUS pfnfcis);
extern BOOL (CDECL *p_FCIDestroy)(HFCI hfci);
extern HFDI (CDECL *p_FDICreate)(PFNALLOC pfnalloc, PFNFREE pfnfree, PFNOPEN pfnopen, PFNREAD pfnread,
        PFNWRITE pfnwrite, PFNCLOSE pfnclose, PFNSEEK pfnseek, int cpuType, PERF perf);
extern BOOL (CDECL *p_FDIIsCabinet)(HFDI hfdi, INT_PTR hf, PFDICABINETINFO pfdici);
extern BOOL (CDECL *p_FDICopy)(HFDI hfdi, char *pszCabinet, char *pszCabPath, int flags, PFNFDINOTIFY pfnfdin,
        PFNFDIDECRYPT pfnfdid, void *pvUser);
extern BOOL (CDECL *p_FDIDestroy)(HFDI hfdi);
extern BOOL (CDECL *p_FDITruncateCabinet)(HFDI hfdi, char *pszCabinetName, USHORT iFolderToDelete);


#endif

#endif
