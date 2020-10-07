    //
    //  main.c
    //  mortal-stack
    //
    //  Created by Alexander Makhov on 8/10/20.
    //


    // WARNING! THIS VERSION IS FAR FROM PERFECTION AND IS TO CHANGE IN THE FUTURE
#include "stack.h"

int main()
{
    Stack stack = {};
    сonstrStack( &stack, 1 );
    
    for ( int i = 0; i < 100; ++i )
    {
        stackPush( &stack, i * i );
    }
    
    stackDump( &stack );
    for ( int i = 0; i < 30; ++i )
    {
        stackPop( &stack );
    }
    
    stackPush( &stack, 45 );
    stack.data[1] = 13;
    
    stackDump( &stack );
    return 0;
}
