
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

    Function* f = new Function( "func" );
    Reference* ra = new Reference( "a", &TypeB32, f );
    Reference* rb = new Reference( "b", &TypeB32, f );
    Reference* rc = new Reference( "c", &TypeB32, f, false );
    m->add( f );
    
    Component* c = new Component( "test" );
    c->addParameter( new Reference( "b", &TypeB32 ) );
    c->addParameter( new Reference( "a", &TypeB32 ) );
    c->addParameter( new Reference( "c", &TypeB32, 0, false ) );
    m->add( c );
    
    Block* seq0 = new SequentialScope();
    f->setContext( seq0 );
    
    TrivialStatement* s0 = new TrivialStatement( seq0 );
    s0->add( new AddSignedInstruction( seq0, seq0 ) );
    
    
    Block* seq1 = new SequentialScope();
    c->setContext( seq1 );
    TrivialStatement* s1 = new TrivialStatement( seq1 );
    s1->add( new CallInstruction( f ) );
    
    printf( "===--- DUMP ---===\n" );
    
    m->iterate
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

    printf( "===--- EMIT VHDL ---===\n" );
    
    m->iterate
    ( Traversal::PREORDER
    , &n2v
    );
    
    return 0;
}

