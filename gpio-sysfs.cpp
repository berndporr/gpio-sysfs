/* Copyright (c) 2011, RidgeRun
 * Copyright (c) 2014-2022, Bernd Porr
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
 *    This product includes software developed by the RidgeRun and Bernd Porr.
 * 4. Neither the name of the RidgeRun nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY RIDGERUN AND BERND PORR ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL RIDGERUN OR BERND PORR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

#include "gpio-sysfs.h"

/****************************************************************
 * Constants
 ****************************************************************/

#define SYSFS_GPIO_DIR "/sys/class/gpio"

SysGPIO::SysGPIO(unsigned int gpioNumber)
{
  gpio = gpioNumber;
}

int SysGPIO::checkReadWriteStatus(int status, int expectedLen) const
{
  int ret;

  if (status == expectedLen)
  {
    ret = 0;
  }
  else if (status < 0)
  {
    ret = status;
  }
  else
  {
    ret = -2;
  }

  return ret;
}

/****************************************************************
 * gpio_export
 ****************************************************************/
int SysGPIO::gpio_export(void) const
{
  int statusWrite;
  int fd, len;
  char buf[MAX_BUF] = { 0 };

  fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
  if (fd < 0) {
    perror("gpio/export");
    return fd;
  }

  len = snprintf(buf, sizeof(buf), "%d", gpio);
  statusWrite = write(fd, buf, len);
  (void)close(fd);

  return checkReadWriteStatus(statusWrite, len);
}

/****************************************************************
 * gpio_unexport
 ****************************************************************/
int SysGPIO::gpio_unexport(void) const
{
  int statusWrite;
  int fd, len;
  char buf[MAX_BUF] = { 0 };

  fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
  if (fd < 0) {
    perror("gpio/export");
    return fd;
  }

  len = snprintf(buf, sizeof(buf), "%d", gpio);
  statusWrite = write(fd, buf, len);
  (void)close(fd);

  return checkReadWriteStatus(statusWrite, len);
}

/****************************************************************
 * gpio_set_dir
 ****************************************************************/
int SysGPIO::gpio_set_dir(bool out_flag) const
{
  int statusWrite;
  int len;
  int fd;
  char buf[MAX_BUF] = { 0 };

  (void)snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR  "/gpio%d/direction", gpio);

  fd = open(buf, O_WRONLY);
  if (fd < 0) {
    perror("gpio/direction");
    return fd;
  }

  if (out_flag)
  {
    len = 4;
    statusWrite = write(fd, "out", len);
  }
  else
  {
    len = 3;
    statusWrite = write(fd, "in", len);
  }

  (void)close(fd);

  return checkReadWriteStatus(statusWrite, len);
}

/****************************************************************
 * gpio_set_value
 ****************************************************************/
int SysGPIO::gpio_set_value(unsigned int value) const
{
  int statusWrite;
  static const int len = 2;
  int fd;
  char buf[MAX_BUF] = { 0 };

  (void)snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

  fd = open(buf, O_WRONLY);
  if (fd < 0) {
    perror("gpio/set-value");
    return fd;
  }

  if (value)
    statusWrite = write(fd, "1", len);
  else
    statusWrite = write(fd, "0", len);

  (void)close(fd);

  return checkReadWriteStatus(statusWrite, len);
}

/****************************************************************
 * gpio_get_value
 ****************************************************************/
int SysGPIO::gpio_get_value(unsigned int &value) const
{
  int statusRead;
  static const int len = 1;
  int fd;
  char buf[MAX_BUF] = { 0 };
  char ch;

  (void)snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

  fd = open(buf, O_RDONLY);
  if (fd < 0) {
    perror("gpio/get-value");
    return fd;
  }

  statusRead = read(fd, &ch, len);

  if (ch != '0') {
    value = 1;
  } else {
    value = 0;
  }

  (void)close(fd);

  return checkReadWriteStatus(statusRead, len);
}


/****************************************************************
 * gpio_set_edge
 ****************************************************************/

int SysGPIO::gpio_set_edge(const char *edge) const
{
  int statusWrite;
  int len;
  int fd;
  char buf[MAX_BUF] = { 0 };

  (void)snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", gpio);

  fd = open(buf, O_WRONLY);
  if (fd < 0) {
    perror("gpio/set-edge");
    return fd;
  }

  len = strlen(edge) + 1;
  statusWrite = write(fd, edge, len);
  (void)close(fd);

  return checkReadWriteStatus(statusWrite, len);
}

/****************************************************************
 * gpio_fd_open
 ****************************************************************/

int SysGPIO::gpio_fd_open(void) const
{
  int fd;
  char buf[MAX_BUF] = { 0 };

  (void)snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

  fd = open(buf, O_RDONLY | O_NONBLOCK );
  if (fd < 0) {
    perror("gpio/fd_open");
  }
  return fd;
}

/****************************************************************
 * gpio_poll
****************************************************************/

int SysGPIO::gpio_poll(int gpio_fd, int timeout) const
{
  struct pollfd fdset[1] = {};
  int nfds = 1;
  int rc;
  char buf[MAX_BUF] = { 0 };

  fdset[0].fd = gpio_fd;
  fdset[0].events = POLLPRI;

  rc = poll(fdset, nfds, timeout);

  if (fdset[0].revents & POLLPRI) {
    // dummy read
    (void)read(fdset[0].fd, buf, MAX_BUF);
  }

  return rc;
}
