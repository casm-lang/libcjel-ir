
#include <stdio.h>

#include "libnovel.h"
using namespace libnovel;

int main( int argc, char** argv )
{
    Value* a = new Value( "a", 0, Value::ID::USER );
    Value* b = new Value( "b", 0, Value::ID::USER );

    AddInstruction* x = new AddInstruction( a, b );

    Block* seq = new SequentialScope( 0 );
    
    TrivialStatement* s = new TrivialStatement( seq );
    s->add( x );
    
    
    return 0;
}

