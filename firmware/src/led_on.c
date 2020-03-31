/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    led_on.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "led_on.h"
#include "system/ports/sys_ports.h"
#include "peripheral/ports/plib_ports.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

LED_ON_DATA led_onData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/
void APP_Tmr_Sys_Service_Callback_Delay1( uintptr_t context, uint32_t currTick)
{
    led_onData.delay1Expired = true;
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/
static void Update_LED (void)
{
    //----------   PIN of LED to be updated here ----------//
    PLIB_PORTS_PinWrite (PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_3, led_onData.ledvalue);
    PLIB_PORTS_PinWrite (PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_14, led_onData.ledvalue); // Assign same value as Led
}


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void LED_ON_Initialize ( void )

  Remarks:
    See prototype in led_on.h.
 */

void LED_ON_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    led_onData.state = LED_ON_STATE_INIT;

    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
    led_onData.delay1Expired = false;
    led_onData.ledvalue = true;
    
        //----------   PIN 7 of LED to be config here ----------//
    PLIB_PORTS_PinModeSelect (PORTS_ID_0, PORTS_ANALOG_PIN_5, PORTS_PIN_MODE_DIGITAL);
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_3);
    
    // Setup pin 25 as output for "Chip Select"
    PLIB_PORTS_PinModeSelect (PORTS_ID_0, PORTS_ANALOG_PIN_10, PORTS_PIN_MODE_DIGITAL);
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_14);
}


/******************************************************************************
  Function:
    void LED_ON_Tasks ( void )

  Remarks:
    See prototype in led_on.h.
 */

void LED_ON_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( led_onData.state )
    {
        /* Application's initial state. */
        case LED_ON_STATE_INIT:
        {
            bool appInitialized = true;
       
        
            if (appInitialized)
            {
            
                led_onData.state = LED_ON_STATE_SERVICE_TASKS;
                led_onData.sysTmrHandle1 = SYS_TMR_CallbackPeriodic(1000, 0, &APP_Tmr_Sys_Service_Callback_Delay1);// Delay in milli sec
                PLIB_PORTS_PinWrite (PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_14, 0); // Assert CS at initialization
            }
            break;
        }

        case LED_ON_STATE_SERVICE_TASKS:
        {
            if(led_onData.delay1Expired == true)
               {
                    led_onData.ledvalue = !led_onData.ledvalue; //toggle the LED
                    Update_LED();
                    led_onData.delay1Expired = false;
                }
            break;
        }

        /* TODO: implement your application state machine.*/
        

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

 

/*******************************************************************************
 End of File
 */
