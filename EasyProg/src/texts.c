/*
 * texts.c
 *
 *  Created on: 25.05.2009
 *      Author: skoe
 */

#include <stdio.h>

#include "texts.h"


const char* apStrLowHigh[] =
{
        "Low",
        "High"
};

const char* apStrAbout[] =
{
        "EasyProg Version 0.1",
        "",
        "(C) 2009 Thomas 'skoe' Giesel",
        NULL
};

const char* apStrUnsupportedCRTType[] =
{
        "Sorry, this CRT file type",
        "is not supported.",
        NULL
};

const char* apStrUnsupportedCRTData[] =
{
        "Sorry, this CRT file contains",
        "unsupported chip data.",
        NULL
};

const char* apStrWriteCRTFailed[] =
{
        "Failed to write the CRT image",
        "to flash.",
        NULL
};

const char* apStrWrongFlash[] =
{
        "A flash chip does not work",
        "or has a wrong type. Check",
        "your hardware, then try again.",
        NULL
};

const char* apStrBadRAM[] =
{
        "The cartridge RAM at $DF00",
        "doesn't work correctly.",
        NULL
};

const char* apStrAskErase[] =
{
        "This will erase the current",
        "flash content! Are you sure?",
        "Press <Stop> to cancel,",
        "<Enter> to continue.",
        NULL
};

const char* apStrEraseFailed[] =
{
        "Flash erase failed,",
        "check your hardware.",
        NULL
};

const char* apStrFileOpenError[] =
{
        "Cannot open cartridge image file.",
        NULL
};

const char* apStrHeaderReadError[] =
{
        "Failed to read the cartridge",
        "header. The file seems to",
        "have a wrong type or may",
        "be damaged.",
        NULL
};

const char* apStrChipReadError[] =
{
        "Failed to read cartridge",
        "data. The file seems to",
        "have a wrong type or may",
        "be damaged.",
        NULL
};

const char* pStrTestFailed = "Test failed";