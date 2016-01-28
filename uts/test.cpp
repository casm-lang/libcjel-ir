
#include <stdio.h>

#include "libnovel.h"
using namespace libnovel;

#include "analyze/NovelDumpPass.h" // just to debug some objects
NovelDumpPass dumper;

#include "transform/NovelToLLPass.h"
NovelToLLPass n2ll;

#include "transform/NovelToC11Pass.h"
NovelToC11Pass n2c11;

#include "transform/NovelToVHDLPass.h"
NovelToVHDLPass n2v;

int main( int argc, char** argv )
{
    Module* m = new Module( "myTestModule" );
    
    Function* f = new Function( "test" );
    m->add( f );

    new Reference( "a", &TypeB32, f );
    new Reference( "a", &TypeB32, f );
    
    Block* seq = new SequentialScope();
    f->setContext( seq );
    
    TrivialStatement* s = new TrivialStatement( seq );
    s->add( new AddSignedInstruction( seq, seq ) );
    s->add( new CallInstruction( f ) );
    
    
    printf( "===--- DUMP ---===\n" );
    
    f->dump();

    printf( "===--- ITERATE ---===\n" );
    
    f->iterate
    ( Traversal::PREORDER
    , &dumper
    );
    
    printf( "===--- EMIT LL ---===\n" );
    
    m->iterate
    ( Traversal::PREORDER
    , &n2ll
    );

    printf( "===--- EMIT C11 ---===\n" );
    
    m->iterate
    ( Traversal::PREORDER
    , &n2c11
    );

    // printf( "===--- EMIT VHDL ---===\n" );
    
    // m->iterate
    // ( Traversal::PREORDER
    // , &n2v
    // );
    
    return 0;
}

