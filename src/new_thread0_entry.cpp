#define TRIAL_1
//#define TRIAL_2
//#define TRIAL_3

#ifdef TRIAL_3

#include "new_thread0.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "FreeRTOS_IP_Private.h"
#include "NetworkBufferManagement.h"
#include "FreeRTOS_DHCP.h"
#include "NetworkInterface.h"
#include "FreeRTOSConfig.h"

BaseType_t xEndPointCount = 0;

volatile bool network_up = false;

const uint8_t ucIPAddress[ 4 ]        = { 192, 168, 1, 118 };
const uint8_t ucNetMask[ 4 ]          = { 255, 255, 255, 0 };
const uint8_t ucGatewayAddress[ 4 ]   = { 192, 168, 1, 1 };
const uint8_t ucDNSServerAddress[ 4 ] = { 192, 168, 1, 1  };
const uint8_t ucMACAddress[ 6 ]       = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55 };

static NetworkInterface_t xInterfaces[1];
static NetworkEndPoint_t xEndPoints[1];

extern NetworkInterface_t * pxFillInterfaceDescriptor( BaseType_t xEMACIndex, NetworkInterface_t * pxInterface );



void vApplicationIPNetworkEventHook( eIPCallbackEvent_t eNetworkEvent)
{
    if(eNetworkEvent == eNetworkUp)
    {
        network_up = true;
    }
}




void new_thread0_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);

    BaseType_t xStatus;

    pxFillInterfaceDescriptor(0, &(xInterfaces[0]));

    FreeRTOS_FillEndPoint( &(xInterfaces[0]),
                           &(xEndPoints[xEndPointCount]),
                           ucIPAddress,
                           ucNetMask,
                           ucGatewayAddress,
                           ucDNSServerAddress,
                           ucMACAddress );

    xEndPointCount += 1;

    xStatus = FreeRTOS_IPInit_Multi();

    if(xStatus == pdFALSE)
    {
        __BKPT(0);
    }

    while(false == network_up)
    {
        vTaskDelay (1);
    }

    while (1)
    {
        vTaskDelay (100);
    }
}

#endif

#ifdef TRIAL_2

#include "new_thread0.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "FreeRTOS_IP_Private.h"
#include "NetworkBufferManagement.h"
#include "FreeRTOS_DHCP.h"
#include "NetworkInterface.h"
#include "FreeRTOSConfig.h"


#if( ipconfigUSE_DHCP != 0 )

static const uint8_t ucIPAddress[4]  = { 0, 0, 0, 0 };
static const uint8_t ucNetMask[4]    = { 0, 0, 0, 0 };
static const uint8_t ucGatewayAddress[4]    = { 0, 0, 0, 0 };
static const uint8_t ucDNSServerAddress[4]  = { 0, 0, 0, 0 };
const uint8_t ucMACAddress[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};

#else

const uint8_t ucIPAddress[4] = {192, 168, 1, 118};
const uint8_t ucNetMask[4] = {255, 255, 255, 0};
const uint8_t ucGatewayAddress[4] = {192, 168, 1, 1};
const uint8_t ucDNSServerAddress[4] = {192, 168, 1, 1};
const uint8_t ucMACAddress[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};

#endif






volatile bool network_up = false;



void vApplicationIPNetworkEventHook( eIPCallbackEvent_t eNetworkEvent)
{
    if(eNetworkEvent == eNetworkUp)
    {
        network_up = true;
    }
}

const char *pcApplicationHostnameHook(void)
{
    return "RA6M2_Device";  // Dilediğin bir isim
}








void new_thread0_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);

    BaseType_t status;

    status = FreeRTOS_IPInit_Multi();

    if(status == pdFALSE)
    {
        __BKPT(0);
    }

    while(false == network_up)
    {
        vTaskDelay (1);
    }

    while (1)
    {
        vTaskDelay (1);
    }
}

#endif

#ifdef TRIAL_1

#include "new_thread0.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "FreeRTOS_IP_Private.h"
#include "NetworkBufferManagement.h"
#include "FreeRTOS_DHCP.h"
#include "NetworkInterface.h"
#include "FreeRTOSConfig.h"


#if( ipconfigUSE_DHCP != 0 )

static const uint8_t ucIPAddress[4]  = { 0, 0, 0, 0 };
static const uint8_t ucNetMask[4]    = { 0, 0, 0, 0 };
static const uint8_t ucGatewayAddress[4]    = { 0, 0, 0, 0 };
static const uint8_t ucDNSServerAddress[4]  = { 0, 0, 0, 0 };
const uint8_t ucMACAddress[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};

#else

const uint8_t ucIPAddress[4] = {192, 168, 1, 118};
const uint8_t ucNetMask[4] = {255, 255, 255, 0};
const uint8_t ucGatewayAddress[4] = {192, 168, 1, 1};
const uint8_t ucDNSServerAddress[4] = {192, 168, 1, 1};
const uint8_t ucMACAddress[6] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};

#endif






volatile bool network_up = false;



void vApplicationIPNetworkEventHook( eIPCallbackEvent_t eNetworkEvent)
{
    if(eNetworkEvent == eNetworkUp)
    {
        network_up = true;
    }
}

const char *pcApplicationHostnameHook(void)
{
    return "RA6M2_Device";  // test name
}








void new_thread0_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);

    BaseType_t status;

    status = FreeRTOS_IPInit(ucIPAddress, ucNetMask, ucGatewayAddress, ucDNSServerAddress, ucMACAddress);

    if(status == pdFALSE)
    {
        __BKPT(0);
    }

    while(false == network_up)
    {
        vTaskDelay (1);
    }

    while (1)
    {
        vTaskDelay (1);
    }
}

#endif

