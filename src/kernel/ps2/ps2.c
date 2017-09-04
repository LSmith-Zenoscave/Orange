/* ===========================================================================
 * Orange 0.1.0
 *
 * Please refer to LICENSE for copyright information
 *
 *              Orange: A hobby OS designed for studying OS development.
 *
 *              Kernel: The main kernel
 *
 *      File    : ps2.c
 *      Purpose : kernel ps2 initializer
 *
 *      Notes   :
 *      Author  : Luke Smith
 * ===========================================================================
 */

#include <assert.h>
#include <ps2/ps2.h>
#include <misc/utils.h>

#define PS2_FIRST_PORT 0xAD
#define PS2_SECOND_PORT 0xA7

void initialize_ps2(void) {

  outb(PS2_COMMAND, PS2_FIRST_PORT);
  outb(PS2_COMMAND, PS2_SECOND_PORT);

  while(inb(PS2_COMMAND) & PS2_OUTPUT_FULL) {
    inb(PS2_DATA);
  }

  outb(PS2_COMMAND, PS2_FIRST_PORT+1);
  outb(PS2_COMMAND, PS2_SECOND_PORT+1);
}
