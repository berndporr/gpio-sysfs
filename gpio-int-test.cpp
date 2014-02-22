/* Copyright (c) 2011, RidgeRun
 * Copyright (c) 2014, Bernd Porr, mail@berndporr.me.uk
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the RidgeRun.
 * 4. Neither the name of the RidgeRun nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY RIDGERUN ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL RIDGERUN BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * This code is based on the one written by RidgeRun. I've just moved
 * the poll code also in a function so that also the interrupt handling
 * is now hidden in a function. I've moved all functions in gpio-sysfs.c
 * so that this has become a proper collection of functions which can
 * be included via gpio-sysfs.h
 * Bernd Porr
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "gpio-sysfs.h"

#define POLL_TIMEOUT (3 * 1000) /* 3 seconds */


/****************************************************************
 * Main
 ****************************************************************/
int main(int argc, char **argv, char **envp)
{
  int gpio_fd;
  char *buf[MAX_BUF];
  unsigned int gpio;
  int len;
  int rc;
  int counter;

  if (argc < 2) {
    printf("Usage: sudo %s <gpio-pin>\n\n",argv[0]);
    printf("Waits for a change in the GPIO pin voltage level.\n");
    exit(-1);
  }

  gpio = atoi(argv[1]);

  // switches on the sysfs entry for pin number 'gpio'
  gpio_export(gpio);
  // sets direction to 'in'
  gpio_set_dir(gpio, 0);
  // detects a falling edge on pin 'gpio'
  gpio_set_edge(gpio, "falling");
  // get a file descriptor pointing to 'gpio'
  // which is needed for the poll command
  gpio_fd = gpio_fd_open(gpio);

  // loop waiting for interrupts
  // which should really reside in a thread which lives
  // until it is stopped.
  for(counter = 1;counter<1000;counter++) {
    printf("%d: ",counter);
    fflush(stdout);

    // wait for a falling edge at the pin gpio
    rc = gpio_poll(gpio_fd, POLL_TIMEOUT);

    // standard linux error codes in case of failure
    if (rc < 0) {
      printf("\npoll() failed!\n");
      return -1;
    }

    // success. An interrupt has occurred.
    if (rc > 0) {
      printf("interrupt!\n");
    }

    // timeout, poll returned after a timeout
    if (rc == 0) {
      printf("timeout\n");
    }

    fflush(stdout);
  }

  // closes file descriptor to pin number 'gpio'
  gpio_fd_close(gpio_fd);
  // removes the sysfs entry for pin number 'gpio'
  gpio_unexport(gpio);
  return 0;
}
