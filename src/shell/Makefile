# $Id: Makefile,v 0.0.1 CBB $

#
# Author:  Zengchao
# Contact: zengchao@cqu.edu.cn
# Licence: Belong to the author 
# Date:    2017.12.23
#

CBB_BASE:=$(shell if [ -n "$$CBB_BASE" ] ; then\
	echo $$CBB_BASE;\
	else\
	cd $(dir $(lastword $(MAKEFILE_LIST))); while /usr/bin/test ! -e RELEASE ; do \
	dir=`cd ../;pwd`;			\
	if [ "$$dir" = "/" ] ; then		\
	   echo Cannot find CBB_BASE in $(lastword $(MAKEFILE_LIST)) 1>&2;		\
	   exit 1;				\
	fi ;					\
	cd $$dir;				\
	done ;					\
	pwd;					\
	fi)


ifeq ($(CBB_BASE),)
$(error Please run this in a tree)
endif

export CBB_BASE

ifeq ($(MAKELEVEL),0)
TOPMAKE=1
endif

include $(CBB_BASE)/make/Make.generic
include $(CBB_BASE)/make/Make.local

READLINE_PATH = $(CBB_BASE)/../readline
NCURSES_PATH = $(CBB_BASE)/../ncurses
CBB_INCLUDE_PATH += -I $(READLINE_PATH)/include
#other_libs_objs += $(NCURSES_PATH)/lib/libncurses.a
other_libs_objs += $(READLINE_PATH)/lib/libhistory.a
other_libs_objs += $(READLINE_PATH)/lib/libreadline.a

other_libs_objs += $(CBB_LIB)/libsal.a
other_libs_objs += $(CBB_LIB)/libcommon.a


include $(CBB_BASE)/make/Make.lib
include $(CBB_BASE)/make/Make.subdirs

