#ifndef TYPE_H
#define TYPE_H

#define DEF
#define IN
#define OUT
#define OPTIONAL
#define EFIAPI

typedef char BOOLEAN;
typedef unsigned short CHAR16;
typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef unsigned long long UINT64;
typedef unsigned long long UINTN;
typedef void VOID;
typedef void *EFI_EVENT;
typedef UINTN EFI_TPL;
typedef void *EFI_PHYSICAL_ADDRESS;
typedef unsigned long long EFI_STATUS;
typedef void *EFI_HANDLE;

typedef struct {
  UINT32  Data1;
  UINT16  Data2;
  UINT16  Data3;
  UINT8   Data4[8];
} EFI_GUID;

#endif