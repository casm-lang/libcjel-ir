#   
#   Copyright (c) 2015 Philipp Paulweber
#   All rights reserved.
#   
#   Developed by: Philipp Paulweber
#                 https://github.com/ppaulweber/libnovel
#   
#   Permission is hereby granted, free of charge, to any person obtaining a 
#   copy of this software and associated documentation files (the "Software"), 
#   to deal with the Software without restriction, including without limitation 
#   the rights to use, copy, modify, merge, publish, distribute, sublicense, 
#   and/or sell copies of the Software, and to permit persons to whom the 
#   Software is furnished to do so, subject to the following conditions:
#   
#   * Redistributions of source code must retain the above copyright 
#     notice, this list of conditions and the following disclaimers.
#   
#   * Redistributions in binary form must reproduce the above copyright 
#     notice, this list of conditions and the following disclaimers in the 
#     documentation and/or other materials provided with the distribution.
#   
#   * Neither the names of the copyright holders, nor the names of its 
#     contributors may be used to endorse or promote products derived from 
#     this Software without specific prior written permission.
#   
#   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
#   OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
#   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
#   CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
#   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
#   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
#   WITH THE SOFTWARE.
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

CPPOBJECTS += obj/Value.o
CPPOBJECTS += obj/Type.o

CPPOBJECTS += obj/Memory.o
CPPOBJECTS += obj/Function.o

CPPOBJECTS += obj/Reference.o
CPPOBJECTS += obj/Constant.o

CPPOBJECTS += obj/Block.o

CPPOBJECTS += obj/Scope.o
CPPOBJECTS += obj/Statement.o
CPPOBJECTS += obj/Instruction.o

CPPOBJECTS += obj/Visitor.o

CPPOBJECTS += obj/analyze/NovelDumpPass.o
CPPOBJECTS += obj/transform/NovelToLLPass.o
#CPPOBJECTS += obj/transform/NovelToCPass.o
CPPOBJECTS += obj/transform/NovelToVHDLPass.o

INCLUDE += -I ./src
INCLUDE += -I ../
INCLUDE += -I ../pass/src

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
