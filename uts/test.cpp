
#include <stdio.h>

#include "libnovel.h"
using namespace libnovel;

#include "analyze/NovelDumpPass.h" // just to debug some objects
NovelDumpPass dumper;


int main( int argc, char** argv )
{
    Block* seq = new SequentialScope();
    
    TrivialStatement* s = new TrivialStatement( seq );
    s->add( new AddSignedInstruction( seq, seq ) );
    
    Function* f = new Function( "test" );
    f->setContext( seq );
    
    s->add( new CallInstruction( f ) );

    printf( "===--- DUMP ---===\n" );
    
    f->dump();

    printf( "===--- ITERATE ---===\n" );
    
    f->iterate
    ( Traversal::PREORDER
    , &dumper
    , []( Value* v )
      {
          v->debug();
      }      
    );
    
    return 0;
}

