#
#   Copyright (C) 2015-2019 CASM Organization <https://casm-lang.org>
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

# LIBCJEL_IR_FOUND        - system has found the package
# LIBCJEL_IR_INCLUDE_DIRS - the package include directories
# LIBCJEL_IR_LIBRARY      - the package library

include( LibPackage )

libfind_pkg_check_modules( LIBCJEL_IR_PKGCONF libcjel-ir )

find_path( LIBCJEL_IR_INCLUDE_DIR
  NAMES libcjel-ir/libcjel-ir.h
  PATHS ${LIBCJEL_IR_PKGCONF_INCLUDE_DIRS}
)

find_library( LIBCJEL_IR_LIBRARY
  NAMES libcjel-ir cjel-ir
  PATHS ${LIBCJEL_IR_PKGCONF_LIBRARY_DIRS}
)

set( LIBCJEL_IR_PROCESS_INCLUDES LIBCJEL_IR_INCLUDE_DIR )
set( LIBCJEL_IR_PROCESS_LIBS     LIBCJEL_IR_LIBRARY )

libfind_process( LIBCJEL_IR )

if( EXISTS "${LIBCJEL_IR_INCLUDE_DIR}" AND
    EXISTS "${LIBCJEL_IR_LIBRARY}" AND
    ${LIBCJEL_IR_INCLUDE_DIR} AND
    ${LIBCJEL_IR_LIBRARY}
    )
  set( LIBCJEL_IR_FOUND TRUE PARENT_SCOPE )
else()
  set( LIBCJEL_IR_FOUND FALSE PARENT_SCOPE )
endif()
