
#include <stdio.h>

#include "libnovel.h"
using namespace libnovel;

int main( int argc, char** argv )
{
    Value* a = new Value( "a", 0, Value::ID::USER );
    Value* b = new Value( "b", 0, Value::ID::USER );

    Instruction* x = new AddSignedInstruction( a, b );

    Block* seq = new SequentialScope();
    
    TrivialStatement* s = new TrivialStatement( seq );
    s->add( x );

    Function* f = new Function( "test" );
    f->setContext( s );
    
    return 0;
}

