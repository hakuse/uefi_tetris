#ifndef TETRIS_H
#define TETRIS_H

#include "type.h"
#include "runtime_services.h"
#include "boot_services.h"

#define EFI_SUCCESS 0

#define UNIT 35 //vm 35
#define LINE_SIZE 2
#define X_DISPLAY_ANCHOR 50
#define Y_DISPLAY_ANCHOR 20
#define XLINE 10
#define YLINE 20
#define X_BUF_LINE XLINE
#define Y_BUF_LINE YLINE+4

//#define EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID {0x9042a9de,0x23dc,0x4a38,{0x96,0xfb,0x7a,0xde,0xd0,0x80,0x51,0x6a}}

//構造体のプロトタイプ宣言
struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
struct _EFI_GRAPHICS_OUTPUT_PROTOCOL;

//構造体、列挙型宣言
typedef struct {
 UINT32 RedMask;
 UINT32 GreenMask;
 UINT32 BlueMask;
 UINT32 ReservedMask;
} EFI_PIXEL_BITMASK;    //use EFI_GRAPHICS_OUTPUT_MODE_INFORMATION

typedef enum {
 PixelRedGreenBlueReserved8BitPerColor,
 PixelBlueGreenRedReserved8BitPerColor,
 PixelBitMask,
 PixelBltOnly,
 PixelFormatMax
} EFI_GRAPHICS_PIXEL_FORMAT;    //use EFI_GRAPHICS_OUTPUT_MODE_INFORMATION

typedef struct {
 UINT32 Version;
 UINT32 HorizontalResolution;
 UINT32 VerticalResolution;
 EFI_GRAPHICS_PIXEL_FORMAT PixelFormat;
 EFI_PIXEL_BITMASK PixelInformation;
 UINT32 PixelsPerScanLine;
} EFI_GRAPHICS_OUTPUT_MODE_INFORMATION; //use EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE

typedef struct {
    UINT8 Blue;
    UINT8 Green;
    UINT8 Red;
    UINT8 Reserved;
} EFI_GRAPHICS_OUTPUT_BLT_PIXEL;    //use EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT

typedef enum {
    EfiBltVideoFill,
    EfiBltVideoToBltBuffer,
    EfiBltBufferToVideo,
    EfiBltVideoToVideo,
    EfiGraphicsOutputBltOperationMax
} EFI_GRAPHICS_OUTPUT_BLT_OPERATION;//use *EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT

typedef struct {
 UINT32 MaxMode;
 UINT32 Mode;
 EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *Info;
 UINTN SizeOfInfo;
 EFI_PHYSICAL_ADDRESS FrameBufferBase;
 UINTN FrameBufferSize;
} EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;//use EFI_GRAPHICS_OUTPUT_PROTOCOL

//関数ポインタ宣言
typedef EFI_STATUS (EFIAPI *EFI_TEXT_RESET) (IN struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,IN BOOLEAN ExtendedVerification);
typedef EFI_STATUS (EFIAPI *EFI_TEXT_STRING)(IN struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *this,IN CHAR16 *String);

typedef EFI_STATUS (EFIAPI *EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE) (IN struct _EFI_GRAPHICS_OUTPUT_PROTOCOL *This,IN UINT32 ModeNumber,OUT UINTN *SizeOfInfo, OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION **Info);
typedef EFI_STATUS (EFIAPI *EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE) (IN struct _EFI_GRAPHICS_OUTPUT_PROTOCOL *This, IN UINT32 ModeNumber);
typedef EFI_STATUS (EFIAPI *EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT) (IN struct _EFI_GRAPHICS_OUTPUT_PROTOCOL *This,IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL   *BltBuffer, IN EFI_GRAPHICS_OUTPUT_BLT_OPERATION   BltOperation,IN UINTN SourceX,IN UINTN SourceY,IN UINTN DestinationX,IN UINTN DestinationY,IN UINTN Width,IN UINTN Height);
typedef EFI_STATUS (EFIAPI *EFI_TEXT_SET_CURSOR_POSITION) (IN struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *This,IN UINTN Column,IN UINTN Row);

//構造体
typedef struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL {    //p.511
 //EFI_INPUT_RESET Reset;
 //EFI_INPUT_READ_KEY ReadKeyStroke;
 //EFI_EVENT WaitForKey;
 void *a[3];
} EFI_SIMPLE_TEXT_INPUT_PROTOCOL;

typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL{    
	EFI_TEXT_RESET Reset;                            
	EFI_TEXT_STRING OutputString;
    void *a[5];
    EFI_TEXT_SET_CURSOR_POSITION SetCursorPosition;
	}EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

typedef struct{
	char 				            EFI_TABLE_HEADER[24];   //p.164
    CHAR16                          *FirmwareVendor;
    UINT32                          FirmwareRevision;
    EFI_HANDLE                      ConsoleInHandle;
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL  *ConIn;
	EFI_HANDLE			            ConsoleOutHandle;
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL	*ConOut;
    EFI_HANDLE                      StandardErrorHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *StdErr;
    EFI_RUNTIME_SERVICES            *RuntimeServices;
    EFI_BOOT_SERVICES               *BootServices;
	}EFI_SYSTEM_TABLE;  //p.165

typedef struct _EFI_GRAPHICS_OUTPUT_PROTOCOL {
    EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE QueryMode;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE SetMode;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT Blt;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE *Mode;
    } EFI_GRAPHICS_OUTPUT_PROTOCOL; //p.551



typedef struct {
	EFI_GRAPHICS_OUTPUT_PROTOCOL *GOP;
	EFI_GRAPHICS_OUTPUT_BLT_PIXEL *FBB;
	UINT32 height;
	UINT32 width;
}Console;

void DrawRectangle(Console *console, UINT32 x, UINT32 y, UINT32 xsize, UINT32 ysize, EFI_GRAPHICS_OUTPUT_BLT_PIXEL color);
CHAR16 *itoa(int value, CHAR16 *str);

#endif
