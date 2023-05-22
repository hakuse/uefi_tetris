#ifndef BOOT_SERVICES_H
#define BOOT_SERVICES_H

#include "type.h"

#define EVT_TIMER 0x80000000
#define EVT_RUNTIME 0x40000000
#define EVT_NOTIFY_WAIT 0x00000100
#define EVT_NOTIFY_SIGNAL 0x00000200
#define EVT_SIGNAL_EXIT_BOOT_SERVICES 0x00000201
#define EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE 0x60000202

#define TPL_APPLICATION 4
#define TPL_CALLBACK 8
#define TPL_NOTIFY 16
#define TPL_HIGH_LEVEL 31

#define EFI_SHIFT_STATE_VALID 0x80000000
#define EFI_RIGHT_SHIFT_PRESSED 0x00000001
#define EFI_LEFT_SHIFT_PRESSED 0x00000002
#define EFI_RIGHT_CONTROL_PRESSED 0x00000004
#define EFI_LEFT_CONTROL_PRESSED 0x00000008
#define EFI_RIGHT_ALT_PRESSED 0x00000010
#define EFI_LEFT_ALT_PRESSED 0x00000020
#define EFI_RIGHT_LOGO_PRESSED 0x00000040
#define EFI_LEFT_LOGO_PRESSED 0x00000080
#define EFI_MENU_KEY_PRESSED 0x00000100
#define EFI_SYS_REQ_PRESSED 0x00000200

typedef UINT8 EFI_KEY_TOGGLE_STATE;
#define EFI_TOGGLE_STATE_VALID 0x80
#define EFI_KEY_STATE_EXPOSED 0x40
#define EFI_SCROLL_LOCK_ACTIVE 0x01
#define EFI_NUM_LOCK_ACTIVE 0x02
#define EFI_CAPS_LOCK_ACTIVE 0x04

/*EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL*/
struct _EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL;
typedef struct {
    UINT16 ScanCode;
    CHAR16 UnicodeChar;
} EFI_INPUT_KEY;
typedef struct EFI_KEY_STATE {
    UINT32 KeyShiftState;
    EFI_KEY_TOGGLE_STATE KeyToggleState;
} EFI_KEY_STATE;
typedef struct {
    EFI_INPUT_KEY Key;
    EFI_KEY_STATE KeyState;
} EFI_KEY_DATA;
typedef EFI_STATUS (EFIAPI *EFI_KEY_NOTIFY_FUNCTION) (IN EFI_KEY_DATA *KeyData);
typedef EFI_STATUS (EFIAPI *EFI_REGISTER_KEYSTROKE_NOTIFY) (struct _EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL *This, IN EFI_KEY_DATA *KeyData, IN EFI_KEY_NOTIFY_FUNCTION KeyNotificationFunction, OUT VOID **NotifyHandle);
typedef struct _EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL{
/*
    EFI_INPUT_RESET_EX Reset;
    EFI_INPUT_READ_KEY_EX ReadKeyStrokeEx;
    EFI_EVENT WaitForKeyEx;
    EFI_SET_STATE SetState;
*/
    void *a[4];
    EFI_REGISTER_KEYSTROKE_NOTIFY RegisterKeyNotify;
//  EFI_UNREGISTER_KEYSTROKE_NOTIFY UnregisterKeyNotify;
} EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL;    //p.504

typedef enum {TimerCancel, TimerPeriodic, TimerRelative} EFI_TIMER_DELAY;
typedef VOID (EFIAPI *EFI_EVENT_NOTIFY)(IN EFI_EVENT Event, IN VOID *Context);     //use EFI_CREATE_EVENT
typedef EFI_STATUS (EFIAPI *EFI_CREATE_EVENT) (IN UINT32 Type, IN EFI_TPL NotifyTpl, IN EFI_EVENT_NOTIFY NotifyFunction, OPTIONAL IN VOID *NotifyContext, OPTIONAL OUT EFI_EVENT *Event);
typedef EFI_STATUS (EFIAPI *EFI_SET_TIMER) (IN EFI_EVENT Event, IN EFI_TIMER_DELAY Type, IN UINT64 TriggerTime);
typedef EFI_STATUS (EFIAPI *EFI_CHECK_EVENT) (IN EFI_EVENT Event);
typedef EFI_STATUS (EFIAPI *EFI_SIGNAL_EVENT) (IN EFI_EVENT Event);
typedef EFI_STATUS (EFIAPI *EFI_WAIT_FOR_EVENT) (IN UINTN NumberOfEvents,IN EFI_EVENT *Event,OUT UINTN *Index);
typedef EFI_STATUS (EFIAPI *EFI_CLOSE_EVENT) (IN EFI_EVENT Event);

typedef EFI_STATUS(EFIAPI *EFI_LOCATE_PROTOCOL) (IN EFI_GUID *Protocol, IN VOID *Registration OPTIONAL, OUT VOID **Interface);

typedef struct {
    char EFI_TABLE_HEADER[24];   //p.164
    /*
    //
    // Task Priority Services
    //
    EFI_RAISE_TPL RaiseTPL;
    EFI_RESTORE_TPL RestoreTPL;
    //
    // Memory Services
    //
    EFI_ALLOCATE_PAGES AllocatePages;
    EFI_FREE_PAGES FreePages;
    EFI_GET_MEMORY_MAP GetMemoryMap;
    EFI_ALLOCATE_POOL AllocatePool;
    EFI_FREE_POOL FreePool;
    //
    // Event & Timer Services
    //
    */
    void *a[7];
    EFI_CREATE_EVENT CreateEvent;
    EFI_SET_TIMER SetTimer;
    EFI_WAIT_FOR_EVENT WaitForEvent;
    EFI_SIGNAL_EVENT SignalEvent;
    EFI_CLOSE_EVENT CloseEvent;
    EFI_CHECK_EVENT CheckEvent;
    /*
    //
    // Protocol Handler Services
    //
    EFI_INSTALL_PROTOCOL_INTERFACE InstallProtocolInterface;
    EFI_REINSTALL_PROTOCOL_INTERFACE ReinstallProtocolInterface;
    EFI_UNINSTALL_PROTOCOL_INTERFACE UninstallProtocolInterface;
    EFI_HANDLE_PROTOCOL HandleProtocol;
    VOID* Reserved;
    EFI_REGISTER_PROTOCOL_NOTIFY RegisterProtocolNotify;
    EFI_LOCATE_HANDLE LocateHandle;
    EFI_LOCATE_DEVICE_PATH LocateDevicePath;
    EFI_INSTALL_CONFIGURATION_TABLE InstallConfigurationTable; 
    //
    // Image Services
    //
    EFI_IMAGE_LOAD LoadImage;
    EFI_IMAGE_START StartImage;
    EFI_EXIT Exit;
    EFI_IMAGE_UNLOAD UnloadImage;
    EFI_EXIT_BOOT_SERVICES ExitBootServices;
    //
    // Miscellaneous Services
    //
    EFI_GET_NEXT_MONOTONIC_COUNT GetNextMonotonicCount;
    EFI_STALL Stall;
    EFI_SET_WATCHDOG_TIMER SetWatchdogTimer;
    //
    // DriverSupport Services
    //
    EFI_CONNECT_CONTROLLER ConnectController;
    EFI_DISCONNECT_CONTROLLER DisconnectController;
    //
    // Open and Close Protocol Services
    //
    EFI_OPEN_PROTOCOL OpenProtocol;
    EFI_CLOSE_PROTOCOL CloseProtocol;
    EFI_OPEN_PROTOCOL_INFORMATION OpenProtocolInformation; 
    //
    // Library Services
    //
    EFI_PROTOCOLS_PER_HANDLE ProtocolsPerHandle;
    EFI_LOCATE_HANDLE_BUFFER LocateHandleBuffer;
    */
    void *d[24];
    EFI_LOCATE_PROTOCOL LocateProtocol;
    /*
    EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES
    InstallMultipleProtocolInterfaces;
    EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES
    UninstallMultipleProtocolInterfaces;
    // 32-bit CRC Services
    //
    EFI_CALCULATE_CRC32 CalculateCrc32;
    //
    // Miscellaneous Services
    //
    EFI_COPY_MEM CopyMem;
    EFI_SET_MEM SetMem;
    EFI_CREATE_EVENT_EX CreateEventEx;
    */
} EFI_BOOT_SERVICES;


#endif