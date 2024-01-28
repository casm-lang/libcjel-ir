//
//  Copyright (C) 2015-2024 CASM Organization <https://casm-lang.org>
//  All rights reserved.
//
//  Developed by: Philipp Paulweber et al.
//  <https://github.com/casm-lang/libcjel-ir/graphs/contributors>
//
//  This file is part of libcjel-ir.
//
//  libcjel-ir is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  libcjel-ir is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with libcjel-ir. If not, see <http://www.gnu.org/licenses/>.
//
//  Additional permission under GNU GPL version 3 section 7
//
//  libcjel-ir is distributed under the terms of the GNU General Public License
//  with the following clarification and special exception: Linking libcjel-ir
//  statically or dynamically with other modules is making a combined work
//  based on libcjel-ir. Thus, the terms and conditions of the GNU General
//  Public License cover the whole combination. As a special exception,
//  the copyright holders of libcjel-ir give you permission to link libcjel-ir
//  with independent modules to produce an executable, regardless of the
//  license terms of these independent modules, and to copy and distribute
//  the resulting executable under terms of your choice, provided that you
//  also meet, for each linked independent module, the terms and conditions
//  of the license of that module. An independent module is a module which
//  is not derived from or based on libcjel-ir. If you modify libcjel-ir, you
//  may extend this exception to your version of the library, but you are
//  not obliged to do so. If you do not wish to do so, delete this exception
//  statement from your version.
//

#include <stdio.h>

#include "libcjel-ir.h"
using namespace libcjel_ir;

#include "analyze/CjelIRDumpPass.h" // just to debug some objects
CjelIRDumpPass dumper;

#include "transform/CjelIRToLLPass.h"
CjelIRToLLPass n2ll;

#include "transform/CjelIRToC11Pass.h"
CjelIRToC11Pass n2c11;

#include "transform/CjelIRToVHDLPass.h"
CjelIRToVHDLPass n2v;

int main( int argc, char** argv )
{
    Module* m = new Module( "myTestModule" );

    Function* f = new Function( "func" );
    Reference* ra = new Reference( "a", &TypeB32, f );
    // Reference* rb = new Reference( "b", &TypeB32, f );
    // Reference* rc = new Reference( "c", &TypeB32, f, false );
    // m->add( f );

    // Component* c = new Component( "test" );
    // c->addParameter( new Reference( "b", &TypeB32 ) );
    // c->addParameter( new Reference( "a", &TypeB32 ) );
    // c->addParameter( new Reference( "c", &TypeB32, 0, false ) );
    // m->add( c );

    // Block* seq0 = new SequentialScope();
    // f->setContext( seq0 );

    // TrivialStatement* s0 = new TrivialStatement( seq0 );
    // s0->add( new AddSignedInstruction( seq0, seq0 ) );

    // Block* seq1 = new SequentialScope();
    // c->setContext( seq1 );
    // TrivialStatement* s1 = new TrivialStatement( seq1 );
    // s1->add( new CallInstruction( f ) );

    // printf( "===--- DUMP ---===\n" );

    // m->iterate( Traversal::PREORDER, &dumper );

    // printf( "===--- EMIT LL ---===\n" );

    // m->iterate( Traversal::PREORDER, &n2ll );

    // printf( "===--- EMIT C11 ---===\n" );

    // m->iterate( Traversal::PREORDER, &n2c11 );

    // printf( "===--- EMIT VHDL ---===\n" );

    // m->iterate( Traversal::PREORDER, &n2v );

    return 0;
}
