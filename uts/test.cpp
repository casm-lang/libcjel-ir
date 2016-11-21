//
//  Copyright (c) 2015-2016 Philipp Paulweber
//  All rights reserved.
//
//  Developed by: Philipp Paulweber
//                https://github.com/casm-lang/libcsel-ir
//
//  This file is part of libcsel-ir.
//
//  libcsel-ir is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  libcsel-ir is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with libcsel-ir. If not, see <http://www.gnu.org/licenses/>.
//

#include <stdio.h>

#include "libcsel-ir.h"
using namespace libcsel_ir;

#include "analyze/CselIRDumpPass.h" // just to debug some objects
CselIRDumpPass dumper;

#include "transform/CselIRToLLPass.h"
CselIRToLLPass n2ll;

#include "transform/CselIRToC11Pass.h"
CselIRToC11Pass n2c11;

#include "transform/CselIRToVHDLPass.h"
CselIRToVHDLPass n2v;

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
