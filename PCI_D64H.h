#ifndef _PCI_D64H_h_
#define _PCI_D64H_h_

#include <windows.h>

#ifndef _APPDEV_DEFINITION_
#define _APPDEV_DEFINITION_

typedef signed char      S8;
typedef unsigned char    U8;
typedef signed short     S16;
typedef unsigned short   U16;
typedef signed long      S32;
typedef unsigned long    U32;

typedef	char			   I8;
typedef	short			   I16;
typedef	long			   I32;
typedef	float			   F32;
typedef	double		       F64;
   
#endif // _APPDEV_DEFINITION_

#define PARAMETER_IGNORED                         0xFFFF

#define INVALID_CARD_ID                           0xFF
#define INVALID_CARD_TYPE                         0xFFFF

#define ASYNC_OP_STOPPED                          0x01
#define ASYNC_OP_IN_PORGRESS                      0x00

#define DOUBLE_BUFFER_MODE_ENABLE                 0x01
#define DOUBLE_BUFFER_MODE_DISABLE                0x00

//Clock Mode
#define TRIG_SOURCE_INT_PACER                     1
#define TRIG_SOURCE_EXT_STROBE                    2
#define TRIG_SOURCE_HANDSHAKE                     3

//Virtual Sampling Rate for using external clock as the clock source
#define CLKSRC_EXT_SampRate                       10000

// Event type for the event message
#define DIEnd       0
#define DOEnd       0
#define DBEvent     1
#define TrigEvent   2

// 
#define ADD_NOTIFICATION                          0x01
#define REMOVE_NOTIFICATION                       0x00

//InputMode
#define DI_WAITING                                0x02
#define DI_NOWAITING                              0x00

#define DI_TRIG_RISING                            0x04
#define DI_TRIG_FALLING                           0x00

#define IREQ_RISING                               0x08
#define IREQ_FALLING                              0x00

//Output Mode
#define OREQ_ENABLE                               0x10
#define OREQ_DISABLE                              0x00

#define OTRIG_HIGH                                0x20
#define OTRIG_LOW                                 0x00

//Synchronous Mode
#define SYNCH_OP                                  0x01
#define ASYNCH_OP                                 0x02

//DI DO Status
#define DI_OVERRUN_STATUS                         0x01
#define DO_UNDERRUN_STATUS                        0x02

#define D64H_MaxCards                             16  /* for the Hardware ID */

// Error code

#define SUCCESS_NO_ERROR                           0

// indicate the unsuccessful routine-execution
#define ERROR_ROUTINE_FAIL_BASE                   -100

#define ERROR_DEVICE_OPEN                         -102
#define ERROR_DEVICE_CLOSE                        -103
#define ERROR_DOUBLE_BUFFER_MODE                  -104
#define ERROR_DI_CONFIG                           -105
#define ERROR_DI_ASYNC_CLEAR                      -106
#define ERROR_DI_ASYNC_CHECK                      -107
#define ERROR_DI_ASYNC_HALF_READY                 -108
#define ERROR_DI_PIO_READ                         -109
#define ERROR_DO_CONFIG                           -110
#define ERROR_DO_ASYNC_CLEAR                      -111
#define ERROR_DO_ASYNC_CHECK                      -112
#define ERROR_DO_PIO_READ                         -113
#define ERROR_DO_PIO_WRITE                        -114
#define ERROR_DO_PIO_LINE_WRITE                   -115
#define ERROR_CONTINUE_DI_START                   -116
#define ERROR_CONTINUE_DO_START                   -117
#define ERROR_FIFO_STATUS_GET                     -118
#define ERROR_DO_EXT_TRIGLINE_WRITE               -119
#define ERROR_DO_ASYNC_HALF_READY                 -120
#define ERROR_DIGITAL_FILTER_SET                  -121
#define ERROR_OREQ_WIDTH_SET                      -122

// Parameters Error
#define ERROR_INVALID_PARAMETER_BASE              -200
#define ERROR_INVALID_CARD_ID                     -201
#define ERROR_INVALID_SCANNED_INDEX               -202
#define ERROR_CARD_ID_DUPLICATED                  -203
#define ERROR_INVILID_DOUBLE_BUFFER_MODE          -204
#define ERROR_INVALID_PORT_LINE                   -205
#define ERROR_INVALID_TRIGGER_SOURCE              -206
#define ERROR_INVALID_TRIGGER_ENABLE              -207
#define ERROR_INVALID_TRIGGER_POLARITY            -208
#define ERROR_INVALID_IREQ_POLARITY               -209
#define ERROR_INVALID_OTRIG_LEVEL                 -210
#define ERROR_INVALID_OREG_ENABLE                 -211
#define ERROR_INVALID_DOUBLE_BUFFER_MODE          -212
#define ERROR_INVALID_SYNCH_OP_MODE               -213
#define ERROR_SAMPLE_COUNT_IS_ODD                 -214
#define ERROR_INVALID_DO_ITERATIONS               -215
#define ERROR_INVALID_EVENT_ACTION                -216
#define ERROR_INVALID_CALLBACK_ADDRESS            -217
#define ERROR_INVALID_BUFFER_ADDRESS              -218
#define ERROR_INVALID_PATTERN_SIZE                -219
#define ERROR_INVALID_FILTER_SETTING              -220
#define ERROR_INVALID_OREG_WIDTH                  -221

// RunTime Error
#define ERROR_RUNTIME_BASE                        -300
#define	ERROR_NO_CARD_FOUND                       -316
#define ERROR_MEMORY_MAP                          -317
#define ERROR_MEMORY_UNMAP                        -318
#define ERROR_ACCESS_VIOLATION_DATA_COPY          -319
#define ERROR_VARIABLE_PITCH_SET                  -320
#define	ERROR_DI_EVENT_ATTACH                     -321
#define ERROR_DI_EVENT_DETACH                     -322
#define	ERROR_DO_EVENT_ATTACH                     -323
#define ERROR_DO_EVENT_DETACH                     -324
#define ERROR_EVENT_CREATE_FAILED                 -325
#define ERROR_OVERLAP_EVENT_CREATE                -326

#define ERROR_IOCTL_FAILED                        -361
#define ERROR_UNDEFINED_EXCEPTION                 -362

// DIO RunTime Error
#define ERROR_DIO_RUNTIME_BASE                    -500
#define ERROR_DEVICE_POWER_DOWN                   -501
#define ERROR_INVALID_MAPPED_ADDRESS              -502
#define ERROR_BUFFER_NOT_ENOUGH                   -503
#define ERROR_DMA_NOT_AVAILABLE                   -504
#define ERROR_DMA_NOT_COMPLETE                    -505
#define ERROR_DMA_NOT_STARTED                     -506
#define ERROR_DMA_NOT_PAUSED                      -507
#define ERROR_DMA_IS_PAUSED                       -508
#define ERROR_DMA_IN_PROGRESS                     -509
#define ERROR_INVALID_SAMPLE_RATE                 -510
#define ERROR_SAMPLE_COUNT_TOO_LARGE			        -511
#define ERROR_SYNCH_OP_WITH_DOUBLE_BUFFER_MODE    -512
#define ERROR_SYNCH_OP_WITH_ASYNC_CHECK           -513
#define ERROR_UNSUPPORTED_SETTING                 -514
#define ERROR_PATTERN_OUT_WITH_DOUBLE_BUFFER_MODE -515
#define ERROR_PATTERN_OUT_IN_PROGRESS             -516

#ifdef __cplusplus
extern "C"
{
#endif

short __stdcall d64h_scan
    (
    short* pCardNum,         /* pointer to available PISO-VS600 cards */
    BYTE* pAvailCards        /* pointer to available cards array */
    );

short __stdcall d64h_cardinfo_get
    (
    U16 ScannedIndex,        /* PISO-VS600 scanned index, start with 0 */
    U8* pCardID			         /* pointer to Card ID */
    );

short __stdcall d64h_open
    (
    BYTE bCardID             /* the specific Card ID that is configured by onboard DIP-switch */
    );

short __stdcall d64h_close
    (
    BYTE bCardID             /* the specific Card ID that is configured by onboard DIP-switch */
    );

short __stdcall d64h_di_event_callback
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    U16 wEventEnable,     /* ADD_NOTIFICATION, REMOVE_NOTIFICATION */
    U16 wEventType,       /* DIEnd or DBEvent */
    void (*callbackAddr)()  /* The address of Callback Address */
    );

short __stdcall d64h_do_event_callback
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    U16 wEventEnable,     /* ADD_NOTIFICATION, REMOVE_NOTIFICATION */
    U16 wEventType,       /* DOEnd*/
    void (*callbackAddr)()  /* The address of Callback Address */
    );

short __stdcall d64h_di_config
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    U16 wTrigSource,      /* TRIG_SOURCE_INT_PACER, TRIG_SOURCE_EXT_STROBE, TRIG_SOURCE_HANDSHAKE */
    U16 wExtTrigEnable,   /* DI_WAITING, DI_NOWAITING */
    U16 wTrigPolarity,    /* DI_TRIG_RISING, DI_TRIG_FALLING, only for wExtTrigEnable = DI_WAITING */
    U16 wI_REQ_Polarity   /* IREQ_RISING, IREQ_FALLING, only for wTrigSource = TRIG_SOURCE_HANDSHAKE */
    );

short __stdcall d64h_di_buffer_get
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    U32 *pMemSize,
    PVOID* pLowBufAddr = NULL,
    PVOID* pHighBufAddr = NULL 
    );
    
short __stdcall d64h_di_available_memory
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    U32 *pMemSize
    );

short __stdcall d64h_di_async_clear
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    U32  *pAccessCount    /* the samples of DI acquisition */
    );

short __stdcall d64h_di_async_check
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    BOOLEAN *pStopped, 
    U32 *pAccessCount
    );

short __stdcall d64h_di_async_dblbuf_mode
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    U16  wDblBufEnable    /* DOUBLE_BUFFER_MODE_ENABLE, DOUBLE_BUFFER_MODE_DISABLE */
    );

short __stdcall d64h_di_async_dblbuf_halfready
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    BOOLEAN *pHalfReady,
    short *pBufIndex = NULL
    );

short __stdcall d64h_di_async_dblbuf_transfer
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    void *pBuffer
    );

short __stdcall d64h_di_readport
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    U32 *pValue		  /* point to the DI value  */
    );

short __stdcall d64h_di_readline
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    U16 wLine,         /* 0 ~ 31 */
    U16 *pValue        /* point to the DI value  */
    );

short __stdcall d64h_continue_readport
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    void *pBuffer,		  /*  */
    U32 dwSampleCount,    /* the number of acquisition sample */
    F64* pSampleRate,   /* the sample-rate of DI acquisition */
    U16 wSyncMode         /* SYNCH_OP, ASYNCH_OP */
    );

short __stdcall d64h_conti_di_status
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    U16 *pStatus
    );


short __stdcall d64h_do_config
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    U16 wTrigSource,      /* TRIG_SOURCE_INT_PACER, TRIG_SOURCE_HANDSHAKE */
    U16 wO_REQ_Enable,    /* OREQ_ENABLE, OREQ_DISABLE */
    U16 wOutTrigHigh      /* OTRIG_HIGH, OTRIG_LOW */
    );

short __stdcall d64h_do_buffer_get
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    U32 *pMemSize,		 /* available buffer for DO operation (in KB) */
    PVOID* pLowBufAddr = NULL,
    PVOID* pHighBufAddr = NULL     
    );
    
short __stdcall d64h_do_available_memory
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    U32 *pMemSize		 /* available buffer for DO operation (in KB) */
    );
    
short __stdcall d64h_do_async_clear
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    U32  *pAccessCount    /* the samples of DI acquisition */
    );

short __stdcall d64h_do_async_check
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    BOOLEAN *pStopped, 
    U32 *pAccessCount
    );

short __stdcall d64h_do_async_dblbuf_mode
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    U16  wDblBufEnable    /* DOUBLE_BUFFER_MODE_ENABLE, DOUBLE_BUFFER_MODE_DISABLE */
    );

short __stdcall d64h_do_async_dblbuf_halfready
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    BOOLEAN *pHalfReady,
    short *pBufIndex = NULL
    );

short __stdcall d64h_do_async_dblbuf_transfer
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    void *pBuffer
    );
    
short __stdcall d64h_conti_do_status
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    U16 *pStatus
    );

short __stdcall d64h_do_readport
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    U32 *pValue		  /* of no use while double-buffer mode enabled */
    );

short __stdcall d64h_do_readline
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    U16 wLine, 
    U16 *pValue
    );

short __stdcall d64h_do_writeport
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    U32 dwValue		  /* of no use while double-buffer mode enabled */
    );

short __stdcall d64h_do_writeline
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    U16 wLine, 
    U16 wValue
    );

short __stdcall d64h_continue_writeport
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    void *pBuffer,		  /* of no use while double-buffer mode enabled */
    U32 dwSampleCount,    /* sample-count for output data */
    U16 wIterations,      /* this parameter will be ignored */
    F64* pSampleRate,   /* the sample-rate of DI acquisition */
    U16 wSyncMode         /* SYNCH_OP, ASYNCH_OP */
    );

short __stdcall d64h_continue_pattern_write
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    void *pBuffer,		  /* of no use while double-buffer mode enabled */
    U32 dwSampleCount,    /* sample-count for output data ( 2< dwSampleCount <2048) */
    F64* pSampleRate       /* the sample-rate of DI acquisition */
    );
    
short __stdcall d64h_do_ext_trigline_write
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    U16 wValue		  /* of no use while double-buffer mode enabled */
    );
    
short __stdcall d64h_DI_filter_set
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    U16  wDIFilter
    );

short __stdcall d64h_IREQ_filter_set
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    U16  wIREQFilter
    );

short __stdcall d64h_OACK_filter_set
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    U16  wOACKFilter
    );

short __stdcall d64h_ITRG_filter_set
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    U16  wITRGFilter
    );

short __stdcall d64h_OREQ_width_set
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    U16  wOREQWidth
    );

// --------------- Undocumented APIs -------------------       
        
short __stdcall d64h_conti_readport
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    void *pBuffer,		  /*  */
    U32 dwSampleCount,    /* the number of acquisition sample */
    F64 fSampleRate,   /* the sample-rate of DI acquisition */
    U16 wSyncMode         /* SYNCH_OP, ASYNCH_OP */
    );
            
short __stdcall d64h_conti_writeport
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    void *pBuffer,		  /* of no use while double-buffer mode enabled */
    U32 dwSampleCount,    /* sample-count for output data */
    U16 wIterations,      /* this parameter will be ignored */
    F64 fSampleRate,   /* the sample-rate of DI acquisition */
    U16 wSyncMode         /* SYNCH_OP, ASYNCH_OP */
    );

short __stdcall d64h_conti_pattern_write
    (
    BYTE bCardID,         /* the specific Card ID that is configured by onboard DIP-switch */
    void *pBuffer,		  /* of no use while double-buffer mode enabled */
    U32 dwSampleCount,    /* sample-count for output data ( 2< dwSampleCount <2048) */
    F64 fSampleRate       /* the sample-rate of DI acquisition */
    );
                
#ifdef __cplusplus
}
#endif

#endif // _PCI_D64H_h_