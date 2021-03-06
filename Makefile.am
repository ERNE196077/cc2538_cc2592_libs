#
#  Copyright (c) 2016, The OpenThread Authors.
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions are met:
#  1. Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#  3. Neither the name of the copyright holder nor the
#     names of its contributors may be used to endorse or promote products
#     derived from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
#  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
#  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
#  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
#  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
#  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
#  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
#  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#  POSSIBILITY OF SUCH DAMAGE.
#

include $(abs_top_nlbuild_autotools_dir)/automake/pre.am

lib_LIBRARIES                             = libopenthread-cc2538.a

libopenthread_cc2538_a_CPPFLAGS           = \
    -I$(top_srcdir)/include                 \
    -I$(top_srcdir)/examples/platforms      \
    -I$(top_srcdir)/src/core                \
    $(NULL)

PLATFORM_SOURCES                          = \
    source/alarm.c                                 \
    source/diag.c                                  \
    source/flash.c                                 \
    source/misc.c                                  \
    system.c                              \
    source/radio.c                                 \
    source/random.c                                \
    startup-gcc.c                           \
    source/logging.c                               \
    source/uart.c                                  \
    source/gptimer.c                               \
    source/i2c.c                               \
    source/ssim.c                               \
    source/rgbled.c                               \
    source/udma.c                               \
    source/vcnl4100.c                               \
    source/rgbled_coap.c                               \
    $(NULL)

libopenthread_cc2538_a_SOURCES            = \
    $(PLATFORM_SOURCES)                     \
    $(NULL)

noinst_HEADERS                            = \
    include/cc2538-reg.h                            \
    platform-cc2538.h                       \
    include/rom-utility.h                           \
    $(NULL)

PRETTY_FILES                             = \
    $(PLATFORM_SOURCES)                    \
    $(noinst_HEADERS)                      \
    $(NULL)

Dash                                      = -
libopenthread_cc2538_a_LIBADD             = \
    $(shell find $(top_builddir)/examples/platforms/utils $(Dash)type f $(Dash)name "*.o")

include $(abs_top_nlbuild_autotools_dir)/automake/post.am
