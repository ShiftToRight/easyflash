/*
 *
 * (c) 2011 Thomas Giesel
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * Thomas Giesel skoe@directbox.com
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "ef3xfer.h"

static void usage(const char* p_str_prg);
static void log_str_stdout(const char* str);
static void log_progress_stdout(int percent);
static void log_complete_stdout(void);

/*****************************************************************************/
static int m_argc;
static char** m_argv;
static int m_n_next_arg;


/*****************************************************************************/
static void usage(const char* p_str_prg)
{
    printf("\n"
           "ef3xfer version %s\n\n", VERSION);
    printf("Transfer a file to an EasyFlash 3 over USB.\n\n");
    printf("Usage: %s <action>\n", p_str_prg);
    printf("Actions:\n"
           "  -h       --help           print this and exit\n"
           "  -c FILE  --crt FILE       write a CRT image to flash\n"
           "  -x FILE  --exec FILE      send a PRG file and execute it\n"
           "  -w FILE  --write FILE     send a PRG file\n"
           "  -k KEY   --key KEY        send a key to the keyboard buffer\n"
           //"  -w  --write-disk  write a disk image (d64)\n"
           //"\nOptions to be used with --write-disk:\n"
           //"  -f  --format      format disk before write\n"
           "\n"
          );
}


/*****************************************************************************/
static void log_str_stdout(const char* str)
{
    printf("%s", str);
    fflush(stdout);
}


/*****************************************************************************/
/*
 */
static void log_progress_stdout(int percent)
{
    if (percent < 0)
        percent = 0;
    if (percent > 100)
        percent = 100;

    printf("\r%3d%%", percent);
    fflush(stdout);
}


/*****************************************************************************/
/*
 * Tell the main thread that we're done.
 */
static void log_complete_stdout(void)
{
}

/*****************************************************************************/
static const char* get_next_arg(void)
{
    if (m_n_next_arg < m_argc)
        return m_argv[m_n_next_arg++];
    else
        return NULL;
}

/*****************************************************************************/
int main(int argc, char** argv)
{
    const char* p_str_type = NULL;
    const char* p_filename = NULL;
    const char* arg;
    int i;

    m_argc = argc;
    m_argv = argv;
    m_n_next_arg = 1;

    /* default callback functions for stdout */
    ef3xfer_set_callbacks(log_str_stdout, log_progress_stdout,
                          log_complete_stdout);

    while ((arg = get_next_arg()) != NULL)
    {
        if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0)
        {
            usage(argv[0]);
        }
        else if (strcmp(arg, "-c") == 0 || strcmp(arg, "--crt") == 0)
        {
        }
        else if (strcmp(arg, "-x") == 0 || strcmp(arg, "--exec") == 0)
        {
            ef3xfer_transfer_prg(get_next_arg(), 1);
        }
        else if (strcmp(arg, "-w") == 0 || strcmp(arg, "--write") == 0)
        {
            ef3xfer_transfer_prg(get_next_arg(), 0);
        }
        else if (strcmp(arg, "-k") == 0 || strcmp(arg, "--key") == 0)
        {
            ef3xfer_transfer_keys(get_next_arg());
        }
        else
        {
            fprintf(stderr, "Unknown action: %s\n", arg);
            return 1;
        }
    }
}
