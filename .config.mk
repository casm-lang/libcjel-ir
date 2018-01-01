#
#   Copyright (C) 2015-2018 CASM Organization <https://casm-lang.org>
#   All rights reserved.
#
#   Developed by: Philipp Paulweber
#                 <https://github.com/casm-lang/libcjel-ir>
#
#   This file is part of libcjel-ir.
#
#   libcjel-ir is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   libcjel-ir is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with libcjel-ir. If not, see <http://www.gnu.org/licenses/>.
#
#   Additional permission under GNU GPL version 3 section 7
#
#   libcjel-ir is distributed under the terms of the GNU General Public License
#   with the following clarification and special exception: Linking libcjel-ir
#   statically or dynamically with other modules is making a combined work
#   based on libcjel-ir. Thus, the terms and conditions of the GNU General
#   Public License cover the whole combination. As a special exception,
#   the copyright holders of libcjel-ir give you permission to link libcjel-ir
#   with independent modules to produce an executable, regardless of the
#   license terms of these independent modules, and to copy and distribute
#   the resulting executable under terms of your choice, provided that you
#   also meet, for each linked independent module, the terms and conditions
#   of the license of that module. An independent module is a module which
#   is not derived from or based on libcjel-ir. If you modify libcjel-ir, you
#   may extend this exception to your version of the library, but you are
#   not obliged to do so. If you do not wish to do so, delete this exception
#   statement from your version.
#

ifndef TARGET
  $(error TARGET not defined!)
endif

OBJ = obj
.PHONY: $(OBJ)
.NOTPARALLEL: $(OBJ)

CLANG := $(shell clang --version 2> /dev/null)
ifdef CLANG
  CC=clang
  CXX=clang++
endif

ifdef ENV_CC
  CC=$(ENV_CC)
endif

ifdef ENV_CXX
  CXX=$(ENV_CXX)
endif


default: debug

help:
	@echo "TODO"


$(OBJ):
	@mkdir -p $(OBJ)
	@mkdir -p $(OBJ)/uts

clean:
ifneq ("$(wildcard $(OBJ)/CMakeCache.txt)","")
	@$(MAKE) $(MFLAGS) --no-print-directory -C $(OBJ) $@
endif

clean-all:
	@echo "-- Removing build directory" $(OBJ)
	@rm -rf $(OBJ)

TYPES = debug sanitize release

SYNCS = $(TYPES:%=%-sync)
TESTS = $(TYPES:%=%-test)
BENCH = $(TYPES:%=%-benchmark)
ALL   = $(TYPES:%=%-all)


$(OBJ)/Makefile: $(OBJ)
	@rm -f $(OBJ)/CMakeCache.txt
	@(\
	cd $(OBJ); \
	cmake \
	-D CMAKE_C_COMPILER=$(CC) \
	-D CMAKE_CXX_COMPILER=$(CXX) \
	-D CMAKE_BUILD_TYPE=$(TYPE) .. \
	)

$(SYNCS):%-sync: 
	@$(MAKE) $(MFLAGS) --no-print-directory \
	TYPE=$(patsubst %-sync,%,$@) $(OBJ)/Makefile

$(TYPES):%: %-sync
	@$(MAKE) $(MFLAGS) --no-print-directory -C $(OBJ) ${TARGET}


all: debug-all

$(ALL):%-all: %-sync
	@$(MAKE) $(MFLAGS) --no-print-directory -C $(OBJ)


test: debug-test

test-all: $(TYPES:%=%-test)

$(TESTS):%-test: %
	@$(MAKE) $(MFLAGS) --no-print-directory \
	-C $(OBJ) $(TARGET)-check
	@echo "-- Running unit test"
	@$(ENV_FLAGS) ./$(OBJ)/$(TARGET)-check --gtest_output=xml:obj/report.xml $(ENV_ARGS)


benchmark: debug-benchmark

benchmark-all: $(TYPES:%=%-benchmark)

$(BENCH):%-benchmark: %
	@$(MAKE) $(MFLAGS) --no-print-directory \
	-C $(OBJ) $(TARGET)-run
	@echo "-- Running benchmark"
	@$(ENV_FLAGS) ./$(OBJ)/$(TARGET)-run -o console -o json:obj/report.json $(ENV_ARGS)
