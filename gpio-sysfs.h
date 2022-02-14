#ifndef __GPIOSYSFS
#define __GPIOSYSFS

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

/****************************************************************
 * Constants
 ****************************************************************/
#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define MAX_BUF 256

class SysGPIO {

 public:

	SysGPIO(unsigned int gpioNumber) {
		gpio = gpioNumber;
	}

	/**
	 * Starts accesss to the GPIO port
	 * \returns error code or zero on success
	 **/
	int gpio_export();
	
	/**
	 * Stops access to the GPIO port
	 * \returns error code or zero on success
	 **/
	int gpio_unexport();
	
	/**
	 * Sets direction
	 * \param out_flag is true for output
	 * \returns error code or zero on success
	 **/
	int gpio_set_dir(bool out_flag);
	
	/**
	 * Sets the value of a port. Can be one or Zero.
	 * \returns error code or zero on success
	 **/
	int gpio_set_value(unsigned int value);
	
	/**
	 * Gets the value of a port. Will be one or Zero.
	 * \param value is modified by the call to one or zero.
	 * \returns error code or zero on success
	 **/
	int gpio_get_value(unsigned int &value);
	
	/**
	 * Sets the change detection on the port. 
	 * \param edge can be "rising" or "falling".
	 * \returns error code or zero on success
	 **/	
	int gpio_set_edge(const char *edge);

	/**
	 * Gets a file descriptor on the "value" of the port.
	 * \returns The file descriptor
	 **/
	int gpio_fd_open();
	
	/**
	 * Puts the current thread to sleep until a change is detected
	 * on the file descriptor. Get the file descriptor
	 * by calling gpio_fd_open.
	 * \param gpio_fd file descriptor of value
	 * \param timeout Timeout in ms.
	 * \returns error code or one on success
	 **/
	int gpio_poll(int gpio_fd, int timeout);

 private:

	int gpio;
};

#endif
