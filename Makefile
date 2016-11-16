#   
#   Copyright (c) 2015-2016 Philipp Paulweber
#   All rights reserved.
#   
#   Developed by: Philipp Paulweber
#                 https://github.com/casm-lang/libnovel
#   
#   This file is part of libnovel.
#   
#   libnovel is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#   
#   libnovel is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#   GNU General Public License for more details.
#   
#   You should have received a copy of the GNU General Public License
#   along with libnovel. If not, see <http://www.gnu.org/licenses/>.
#   

AR=ar

CC=clang
CCFLAG += -std=c11
CCFLAG += -g -O0
CCFLAG += -Wall

CPP=clang
CPPFLAG += -std=c++11
CPPFLAG += -g -O0
CPPFLAG += -Wall
#CPPFLAG += -Wextra

TARGET += libnovel.a

CPPOBJECTS += obj/Block.o
CPPOBJECTS += obj/CallableUnit.o
CPPOBJECTS += obj/Constant.o
CPPOBJECTS += obj/Function.o
CPPOBJECTS += obj/Instruction.o
CPPOBJECTS += obj/Interconnect.o
CPPOBJECTS += obj/Intrinsic.o
CPPOBJECTS += obj/Memory.o
CPPOBJECTS += obj/Module.o
CPPOBJECTS += obj/Reference.o
CPPOBJECTS += obj/Scope.o
CPPOBJECTS += obj/Statement.o
CPPOBJECTS += obj/Structure.o
CPPOBJECTS += obj/Type.o
CPPOBJECTS += obj/User.o
CPPOBJECTS += obj/Value.o
CPPOBJECTS += obj/Variable.o
CPPOBJECTS += obj/Visitor.o

CPPOBJECTS += obj/analyze/NovelDumpPass.o
CPPOBJECTS += obj/transform/NovelToLLPass.o
CPPOBJECTS += obj/transform/NovelToC11Pass.o
CPPOBJECTS += obj/transform/NovelToVHDLPass.o

INCLUDE += -I ./
INCLUDE += -I ./src
INCLUDE += -I ./src/analyze
INCLUDE += -I ./src/transform

INCLUDE += -I ../stdhl
INCLUDE += -I ../pass

#LIBRARY += 

default: $(LIBRARY) obj $(TARGET)

all: clean test

obj:
	mkdir -p obj
	mkdir -p obj/analyze
	mkdir -p obj/transform

obj/%.o: src/%.cpp
	@echo "CPP " $<
	@$(CPP) $(CPPFLAG) $(INCLUDE) -c $< -o $@

obj/test.o: uts/test.cpp
	@echo "CPP " $<
	@$(CPP) $(CPPFLAG) $(INCLUDE) -c $< -o $@

libnovel.a: $(CPPOBJECTS) $(LIBRARY)
	@echo "AR  " $@
	@$(AR) rsc $@ $(filter %.o,$^)
#	@$(AR) rsc $@.a $(filter %.o,$^)
#	@$(AR) -rcT $@ $@.a $(filter %.a,$^)
	@ranlib $@
#	@rm -f $@.a

clean:
	@echo "RM  " obj
	@rm -rf obj
	@echo "RM  " $(TARGET)
	@rm -f $(TARGET)

test:	default obj/test.o 
	$(CPP) $(CPPFLAG) $(INCLUDE) obj/test.o libnovel.a -lstdc++ -o $@
	@echo "################################################################################"

run:	test
	@./test
