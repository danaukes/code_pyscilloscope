/***************************************************************************//**
* \file myUSB_vnd.c
* \version 3.20
*
* \brief
*  This file contains the  USB vendor request handler.
*
********************************************************************************
* \copyright
* Copyright 2008-2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "myUSB_pvt.h"
#include "cyapicallbacks.h"

#if(myUSB_EXTERN_VND == myUSB_FALSE)

/***************************************
* Vendor Specific Declarations
***************************************/

/* `#START VENDOR_SPECIFIC_DECLARATIONS` Place your declaration here */

/* `#END` */


/*******************************************************************************
* Function Name: myUSB_HandleVendorRqst
****************************************************************************//**
*
*  This routine provide users with a method to implement vendor specific
*  requests.
*
*  To implement vendor specific requests, add your code in this function to
*  decode and disposition the request.  If the request is handled, your code
*  must set the variable "requestHandled" to TRUE, indicating that the
*  request has been handled.
*
* \return
*  requestHandled.
*
* \reentrant
*  No.
*
*******************************************************************************/
uint8 myUSB_HandleVendorRqst(void) 
{
    uint8 requestHandled = myUSB_FALSE;

    /* Check request direction: D2H or H2D. */
    if (0u != (myUSB_bmRequestTypeReg & myUSB_RQST_DIR_D2H))
    {
        /* Handle direction from device to host. */
        
        switch (myUSB_bRequestReg)
        {
            case myUSB_GET_EXTENDED_CONFIG_DESCRIPTOR:
            #if defined(myUSB_ENABLE_MSOS_STRING)
                myUSB_currentTD.pData = (volatile uint8 *) &myUSB_MSOS_CONFIGURATION_DESCR[0u];
                myUSB_currentTD.count = myUSB_MSOS_CONFIGURATION_DESCR[0u];
                requestHandled  = myUSB_InitControlRead();
            #endif /* (myUSB_ENABLE_MSOS_STRING) */
                break;
            
            default:
                break;
        }
    }

    /* `#START VENDOR_SPECIFIC_CODE` Place your vendor specific request here */

    /* `#END` */

#ifdef myUSB_HANDLE_VENDOR_RQST_CALLBACK
    if (myUSB_FALSE == requestHandled)
    {
        requestHandled = myUSB_HandleVendorRqst_Callback();
    }
#endif /* (myUSB_HANDLE_VENDOR_RQST_CALLBACK) */

    return (requestHandled);
}


/*******************************************************************************
* Additional user functions supporting Vendor Specific Requests
********************************************************************************/

/* `#START VENDOR_SPECIFIC_FUNCTIONS` Place any additional functions here */

/* `#END` */


#endif /* myUSB_EXTERN_VND */


/* [] END OF FILE */
