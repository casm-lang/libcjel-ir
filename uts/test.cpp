
#include <stdio.h>

#include "libnovel.h"
using namespace libnovel;

int main( int argc, char** argv )
{
    Value a( "a", 0, Value::ID::USER );
    Value b( "b", 0, Value::ID::USER );

    AddInstruction* x = new AddInstruction( &a, &b );
    
    return 0;
}

