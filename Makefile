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

.PHONY: all SRC 
.PHONY: distclean clean SRC_CLEAN

all:: SRC

SRC:
	$(MAKE) -C $(CBB_SRC)


clean:: SRC_CLEAN

distclean: clean 	

SRC_CLEAN:
	$(MAKE) -C $(CBB_SRC) clean

