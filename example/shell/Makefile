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


include $(CBB_BASE)/make/Make.generic

subdirs += test

target_path = $(CBB_BIN)

READLINE_PATH = $(CBB_BASE)/tools/readline/readline
NCURSES_PATH = $(CBB_BASE)/tools/ncurses/ncurse

CBB_INCLUDE_PATH += -I $(READLINE_PATH)/include

LDFLAGS += -L $(CBB_LIB) -lshell 
LDFLAGS += -lncurses
#LDFLAGS += -L $(READLINE_PATH)/lib -lreadline 
#LDFLAGS += -L $(NCURSES_PATH)/lib -lncurses
#other_libs_objs += $(CBB_LIB)/libshell.a
#other_libs_objs += $(CBB_LIB)/libcore.a


include $(CBB_BASE)/make/Make.target

include $(CBB_BASE)/make/Make.subdirs


