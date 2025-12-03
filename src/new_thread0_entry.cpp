//#define TRIAL_1
//#define TRIAL_2
//#define TRIAL_3
#define TRIAL_4

#ifdef TRIAL_4
#include "new_thread0.h"
#include "FreeRTOS_IP.h"
#include "FreeRTOS_IP_Private.h"
#include "FreeRTOS_Sockets.h"
#include "common_utils.h"
#include "usr_app.h"
#include "SEGGER_RTT/SEGGER_RTT.h"


static  uint8_t ucMACAddress[ 6 ]       = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
static  uint8_t ucIPAddress[ 4 ]        = {192, 168, 1, 118};
static  uint8_t ucNetMask[ 4 ]          = {255, 255, 255, 0};
static  uint8_t ucGatewayAddress[ 4 ]   = {192, 168, 1, 1};
//static  uint8_t ucDNSServerAddress[ 4 ] = {10, 60, 1, 2};
static  uint8_t ucDNSServerAddress[ 4 ] = {192, 168, 1, 1};

uint8_t *gp_remote_ip_address = (uint8_t *) USR_TEST_PING_IP;
uint32_t  g_dhcp_in_use   = RESET_VALUE;
uint32_t  g_usrPingCount  = RESET_VALUE;
ping_data_t g_ping_data   = {RESET_VALUE, RESET_VALUE, RESET_VALUE};
static uint32_t usr_print_ability = RESET_VALUE;


void vApplicationPingReplyHook( ePingReplyStatus_t eStatus, uint16_t usIdentifier )
{
    (void)  usIdentifier;

    switch( eStatus )
    {
        /* A valid ping reply has been received */
        case eSuccess    :
            g_ping_data.received++;
            break;
            /* A reply was received but it was not valid. */
        case eInvalidData :
        default:
            g_ping_data.lost++;
            break;
    }
}

void print_ipconfig(void)
{
    APP_PRINT("\r\nEthernet adapter for Renesas EK-RA6M2:\r\n");

    APP_PRINT("\tDescription . . . . . . . . . . . : Renesas EK-RA6M2 Ethernet\r\n");
    APP_PRINT("\tPhysical Address. . . . . . . . . : %02x-%02x-%02x-%02x-%02x-%02x\r\n",
            ucMACAddress[0],ucMACAddress[1],ucMACAddress[2],ucMACAddress[3],ucMACAddress[4],ucMACAddress[5]);
    APP_PRINT("\tDHCP Enabled. . . . . . . . . . . : %s\r\n",g_dhcp_in_use?"Yes":"No");
    APP_PRINT("\tIPv4 Address. . . . . . . . . . . : %d.%d.%d.%d\r\n",ucIPAddress[0],ucIPAddress[1],ucIPAddress[2],ucIPAddress[3]);
    APP_PRINT("\tSubnet Mask . . . . . . . . . . . : %d.%d.%d.%d\r\n",ucNetMask[0],ucNetMask[1],ucNetMask[2],ucNetMask[3]);
    APP_PRINT("\tDefault Gateway . . . . . . . . . : %d.%d.%d.%d\r\n",ucGatewayAddress[0],ucGatewayAddress[1],ucGatewayAddress[2],ucGatewayAddress[3]);
    APP_PRINT("\tDNS Servers . . . . . . . . . . . : %d.%d.%d.%d\r\n",ucDNSServerAddress[0],ucDNSServerAddress[1],ucDNSServerAddress[2],ucDNSServerAddress[3]);
}



uint32_t isNetworkUp(void)
{
    fsp_err_t  eth_link_status = FSP_ERR_NOT_OPEN;
    BaseType_t networkUp = pdFALSE;
    uint32_t network_status = (IP_LINK_UP | ETHERNET_LINK_UP);

    networkUp = FreeRTOS_IsNetworkUp();
    eth_link_status = R_ETHER_LinkProcess(g_ether0.p_ctrl);

    if((FSP_SUCCESS == eth_link_status) && (pdTRUE == networkUp))
    {
        return network_status;
    }
    else
    {
        if(FSP_SUCCESS != eth_link_status)
        {
            network_status |= ETHERNET_LINK_DOWN;
        }
        else if(FSP_SUCCESS == eth_link_status)
        {
            network_status |= ETHERNET_LINK_UP;
        }

        if(pdTRUE != networkUp)
        {
             network_status |= IP_LINK_DOWN;
        }
        else if(pdTRUE == networkUp)
        {
             network_status |= IP_LINK_UP;
        }
        return network_status;
    }
}

BaseType_t vSendPing( const char *pcIPAddress)
{
uint32_t ulIPAddress = RESET_VALUE;

    /*
     * The pcIPAddress parameter holds the destination IP address as a string in
     * decimal dot notation (for example, “192.168.0.200”). Convert the string into
     * the required 32-bit format.
     */
    ulIPAddress = FreeRTOS_inet_addr(pcIPAddress);

    /*
     * Send a ping request containing 8 data bytes.  Wait (in the Blocked state) a
     * maximum of 100ms for a network buffer into which the generated ping request
     * can be written and sent.
     */
    return(FreeRTOS_SendPingRequest(ulIPAddress, 8, 100 / portTICK_PERIOD_MS));
}

void print_pingResult(void)
{
    APP_PRINT("\r\n \r\nPing Statistics for %s :\r\n",(char *)gp_remote_ip_address);
    APP_PRINT("\r\nPackets: Sent  = %02d, Received = %02d, Lost = %02d \r\n",g_ping_data.sent,g_ping_data.received,g_ping_data.lost);
}


void new_thread0_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);

    BaseType_t status = pdFALSE;
    fsp_pack_version_t version = {RESET_VALUE};

    R_FSP_VersionGet (&version);

        APP_PRINT (BANNER_INFO, EP_VERSION, version.version_id_b.major, version.version_id_b.minor, version.version_id_b.patch);

        APP_PRINT(ETH_PREINIT);
        print_ipconfig();

        status = FreeRTOS_IPInit(ucIPAddress, ucNetMask, ucGatewayAddress, ucDNSServerAddress, ucMACAddress);

        if(pdFALSE == status)
        {
            APP_ERR_PRINT("FreeRTOS_IPInit Failed");
            APP_ERR_TRAP(status);
        }

        APP_PRINT(ETH_POSTINIT);

        while(true)
        {
            /* Check if Both the Ethernet Link and IP link are UP */
            if(SUCCESS == isNetworkUp())
            {
                /* usr_print_ability is added to avoid multiple UP messages or Down Messages repeating*/
                if(!(PRINT_UP_MSG_DISABLE & usr_print_ability))
                {
                    APP_PRINT("\r\nNetwork is Up");
                    usr_print_ability |= PRINT_UP_MSG_DISABLE;
                }

                if(!(PRINT_NWK_USR_MSG_DISABLE & usr_print_ability))
                {
                    APP_PRINT("\r\nPinging %s:\r\n\r\n",(char *)gp_remote_ip_address);
                }

                while (g_usrPingCount < USR_PING_COUNT)
                {
                    status =  vSendPing((char *)gp_remote_ip_address);
                    if(status != pdFALSE)
                    {
                        g_ping_data.sent++;
                        APP_PRINT("!");
                    }
                    else
                    {
                        g_ping_data.lost++;
                        APP_PRINT(".");
                    }
                    g_usrPingCount++;
                    vTaskDelay(10);
                }

                if(!(PRINT_NWK_USR_MSG_DISABLE & usr_print_ability))
                {
                    print_pingResult();
                    usr_print_ability |= PRINT_NWK_USR_MSG_DISABLE;
                }
            }
            else
            {
                if(!(PRINT_DOWN_MSG_DISABLE & usr_print_ability))
                {
                     APP_PRINT("\r\nNetwork is Down");
                     usr_print_ability |= PRINT_DOWN_MSG_DISABLE;
                }
                else
                {
                     APP_PRINT(".");
                }
            }
            vTaskDelay(100);
        }
}

#endif

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

