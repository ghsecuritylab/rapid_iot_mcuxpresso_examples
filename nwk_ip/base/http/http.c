/*
 * Copyright (c) 2014 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/*!=================================================================================================
\file       http.c
\brief      This is a public source file for the HTTP module. It contains
            the implementation of the interface functions.
==================================================================================================*/

/*==================================================================================================
Include Files
==================================================================================================*/

#include "EmbeddedTypes.h"
#include "stack_config.h"

#include "http.h"

#include "FunctionLib.h"

#include <string.h>
/*==================================================================================================
Private macros
==================================================================================================*/


/*==================================================================================================
Private type definitions
==================================================================================================*/


/*==================================================================================================
Private prototypes
==================================================================================================*/


/*==================================================================================================
Private global variables declarations
==================================================================================================*/


/* Supported Methods */
const char* gapHttpMethods[]=
{
    "GET ",
    "HEAD ",
    "POST ",
    "PUT ",
    "DELETE "
};

/* Supported HTTP Versions */
const char* gapHttpVersion[]=
{
    "HTTP/1.1",
    "HTTP/1.0",
};

const char* gpHttpStrEnd = {"\r\n\r\n"};

const char* gpHttpNewLine = {"\r\n"};


const char* gpHttpAccept = {"Accept:"};

const char* gapHttpAcceptMediaRange[] =
{
    "text/html"
};

const httpString_t aHttpAllow[] =
{
    {"Allow:", 6U},
};

const httpString_t aHttpAllowMethod[] =
{
    {"GET", 9U},
    {",HEAD", 5U},
};

const char* gpHttpConnection = {"Connection:"};

const char* gpHttpConnectionClose ={"close"};

const char* gpHttpHost = "Host:";


const httpString_t aHttpContentLength =
{
    "Content-Length:", 15U
};

const httpString_t aHttpContentRange[] =
{
    {"Content-Range:", 14U}
};

const httpString_t aHttpContentType =
{
    "Content-Type:", 13U
};

const httpString_t aHttpContentTypeRange[] =
{
    {"text/html", 9U},        
};

const char* gpHttpUserAgent = {"User-Agent:"};

const httpString_t aHttpUserAgentInfo =
{
    "Freescale IP Mote Stack 2013.11.18", 34U
};

const httpString_t      aHttpContentTypeValue[]=
{
  {"/0",1U},
  {"text/plain",10U},
  {"text/html",9U},
  {"image/gif",9U}
};

/*HTTP Server reply strings*/
const httpString_t aHttpStatusReason[]=
{
    {"200 OK\r\n",6},
    {"201 Created\r\n",11},
    {"204 NoContent\r\n",13},
    {"400 Bad Request\r\n",16},
    {"403 Forbidden\r\n",13},
    {"404 Not Found\r\n",13},
    {"405 Method Not Allowed\r\n",22},
    {"411 Length Required\r\n",19},
    {"413 Request Entity Too Large\r\n",28},
    {"415 Unsupported Media Type\r\n",26},
    {"500 Internal Server Error\r\n",25},
    {"501 Not Implemented\r\n",19},
    {"505 HTTP Version not supported\r\n",30}  
};



/*==================================================================================================
Public global variables declarations
==================================================================================================*/


/*==================================================================================================
Public functions
==================================================================================================*/
void HTTP_Uint32ToAscii
(
    uint32_t value,
    char* pAsciiValue,
    uint8_t* pAsciiValueLen
)
{
    uint32_t crtPos = 0U;
    char tempVal[4];
    uint32_t index = 0;

    if(pAsciiValue)
    {
        do
        {
            /* Start from right to left  */
            tempVal[crtPos ++] = '0' + value % 10U;
            
            value /= 10U;
        }while(value > 0);
        
        *pAsciiValueLen = crtPos;

        /* Revert the resulting string */
        do
        {
            pAsciiValue[index ++] = tempVal[--crtPos];
        }while(crtPos > 0);               
    }
}

void HTTP_AddHeader
(
    char** ppCrtPos,
    const char* headerType,
    const char* headerValue    
)
{
    uint32_t headerTypeLen = strlen(headerType);
    uint32_t headerValueLen = strlen(headerValue);
    uint32_t length = strlen(gpHttpNewLine);
    
    /* Add header */
    FLib_MemCpy(*ppCrtPos, (void*)headerType, headerTypeLen);

    *ppCrtPos += headerTypeLen;

    /* Add ' ' */
    **ppCrtPos = ' ';
    (*ppCrtPos) ++;

    /* Add header content */
    FLib_MemCpy(*ppCrtPos, (void*)headerValue, headerValueLen);

    *ppCrtPos += headerValueLen;

    /* Add \r\n after header */
    FLib_MemCpy(*ppCrtPos, (void*)gpHttpNewLine, length);

    *ppCrtPos += length;

}


/*==================================================================================================
Private functions
==================================================================================================*/


/*==================================================================================================
Private debug functions
==================================================================================================*/