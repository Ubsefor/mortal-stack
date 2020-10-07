    //
    //  stack.c
    //  mortal-stack
    //
    //  Created by Alexander Makhov on 8/10/20.
    //

#include "stack.h"

#ifdef DEBUG
#define ASSERT_OK( stack )              \
if ( stackOK( stack ) != OK )           \
{                                       \
stackDump( stack );                     \
assert( 0 );                            \
}
#else ASSERT_OK( stack ) {}
#endif

int isZero( elem_t value )
{
    return ( fabs( value ) < EPS );
}

int checkSum( Stack* stack )
{
    const int hash = 15487457 * 23;
    
    int       check_sum = (int) stack;
    check_sum += (int) stack->capacity;
    check_sum += (int) stack->size;
    
    for ( size_t i = 0; i < stack->size * sizeof ( elem_t ); ++i )
    {
        check_sum += ( (int) ( (char*) stack->data + i ) * ( i * ( i + 1 ) ) ) % hash;
    }
    
    for ( size_t i = 0; i < stack->size; ++i )
    {
        check_sum += (int) stack->data[i] * ( i + 1 );
        
        if ( i % 100 == 0 )
        {
            check_sum %= hash;
        }
    }
    
    check_sum += (int) stack->data;
    
    return check_sum;
}

void enzeroStack( elem_t* start, elem_t* end )
{
    for ( int i = 0; i < end - start + 1; ++i )
    {
        start[i] = NOLL;
    }
}

void constrStack( Stack* stack, const size_t start_capacity, const char* var_name )
{
        //Fill data canaries
    stack->stack_canary_a = (long int*) calloc( 1, sizeof ( elem_t ) * start_capacity + sizeof ( long int ) * 2 );
    stack->data           = (elem_t*) ( (char*) stack->stack_canary_a + sizeof ( long int ) );
    stack->stack_canary_b = (long int*) ( (char*) stack->data + sizeof ( elem_t ) * start_capacity );
    
        //Fill stack canaries
    stack->stack_canarry_a = (long int*) stack;
    stack->stack_canarry_b = (long int*) stack;
    
    stack->capacity = start_capacity;
    stack->size     = 0;
    stack->var_name = var_name;
    
    enzeroStack( stack->data + stack->size, stack->data + stack->capacity );
    
    stack->checksum = checkSum( stack );
    
    ASSERT_OK( stack );
}

Stack* newStack( const size_t start_capacity )
{
    char* var_name = (char*) calloc( 72, sizeof ( char ) );
    strcpy( var_name, "<Name of variable is not available when called newStack_simple>" );
    
    long int* stack_canarry_a = (long int*) calloc( 1, sizeof ( Stack ) + sizeof ( long int ) * 2 );
    Stack   * new_stack       = (Stack*) ( (char*) stack_canarry_a + sizeof ( long int ) );
    
    new_stack->stack_canarry_a = (long int*) stack_canarry_a;
    new_stack->stack_canarry_b = (long int*) ( (char*) new_stack + sizeof ( Stack ) - sizeof ( long int ) * 2 );
    
    constrStack( new_stack, start_capacity, var_name );
    
    ASSERT_OK( new_stack );
    
    free( var_name );
    return new_stack;
}

void changeCapacity( Stack* stack, const double factor, const OPERATION operation )
{
    if ( operation == PUSH )
    {
        stack->capacity = factor * stack->capacity;
    }
    else if ( operation == POP )
    {
        stack->capacity = (size_t) ( stack->capacity / factor );
    }
    else
    {
        return;
    }
    
    stack->stack_canary_a = (long int*) realloc( stack->stack_canary_a, sizeof ( elem_t ) * stack->capacity + sizeof ( long int ) * 2 );
    stack->data           = (elem_t*) ( (char*) stack->stack_canary_a + sizeof ( long int ) );
    stack->stack_canary_b = (long int*) ( (char*) stack->data + sizeof ( elem_t ) * stack->capacity );
}

void checkSizeCapacity( Stack* stack, const OPERATION operation )
{
    if ( stack->size + 1 > stack->capacity && operation == PUSH )
    {
        changeCapacity( stack, 2, operation );
    }
    else if ( stack->size < stack->capacity / 4 && operation == POP )
    {
        changeCapacity( stack, 4, operation );
        
        enzeroStack( stack->data + stack->size, stack->data + stack->capacity );
    }
}

void stackPush( Stack* stack, const elem_t value )
{
    ASSERT_OK( stack );
    
    checkSizeCapacity( stack, PUSH );
    
    stack->data[stack->size] = value;
    ++stack->size;
    
    stack->checksum = checkSum( stack );
    enzeroStack( stack->data + stack->size, stack->data + stack->capacity );
    
    ASSERT_OK( stack );
}

elem_t topStack( Stack* stack )
{
    ASSERT_OK( stack );
    
    return stack->data[stack->size - 1];
}

void stackPop( Stack* stack )
{
    ASSERT_OK( stack );
    
    checkSizeCapacity( stack, POP );
    
    stack->data[stack->size - 1] = NOLL;
    --( stack->size );
    
    stack->checksum = checkSum( stack );
    
    
    ASSERT_OK( stack );
}

void deleteStack( Stack* stack )
{
    ASSERT_OK( stack );
    
    free( stack->data );
    free( stack );
}

ERROR_MESSAGE stackOK( Stack* stack )
{
    static size_t count = 0;
    count++;
    
    
    printf("FUNCTION CALL # %d\n", count);
    printf("->---<-\n");
     
    printf("CHECK SUM NATIVE: %d\n", stack->checksum);
    printf("CHECK SUM AFTER: %d\n", checkSum(stack));
    printf("->---<-\n");
    
    
    
    if ( stack == NULL )
    {
        return NULL_POINTER_STACK;
    }
    else if ( stack->stack_canarry_a != (long int*) stack /*|| stack->stack_canarry_a != (long int*)((char*)stack - sizeof(long int))*/ )
    {
        return STACK_CANARY_A;
    }
    else if ( stack->stack_canarry_b != (long int*) stack /*|| stack->stack_canarry_b != (long int*)((char*)stack + sizeof(Stack) - sizeof(long int) * 2)*/ )
    {
        return STACK_CANARY_B;
    }
    else if ( stack->data == NULL )
    {
        return NULL_POINTER_DATA;
    }
    else if ( stack->stack_canary_a != (long int*) ( (char*) stack->data - sizeof ( long int ) ) )
    {
        return DATA_CANARY_A;
    }
    else if ( stack->stack_canary_b != (long int*) ( (char*) stack->data + sizeof ( elem_t ) * stack->capacity ) )
    {
        return DATA_CANARY_B;
    }
    else if ( stack->size == 0 && stack->capacity == 0 )
    {
        return ZERO_SIZE;
    }
    else if ( stack->capacity == 0 )
    {
        return ZERO_CAPACITY;
    }
    else if ( stack->capacity < stack->size )
    {
        return SIZE_GR_CAPACITY;
    }
    
    const int test_check_sum = checkSum( stack );
    
    if ( stack->checksum != test_check_sum )
    {
        return CHECKSUM_INVALID;
    }
    
    else
    {
        for ( size_t i = 0; i < stack->size; ++i )
        {
            if ( isnan( stack->data[i] ) )
            {
                return NAN_ELEMENT;
            }
                // TODO: Add assert of eq for latter canary
        }
    }
    
    return OK;
}

    // TODO: Add writing this shit to file

void stackDump( Stack* stack )
{
    const ERROR_MESSAGE result = stackOK( stack );
    
    char* stack_status = (char*) calloc( 25, sizeof ( char ) );
    
    switch (result) {
        case NULL_POINTER_STACK:
        strcpy( stack_status, "NULL STACK POINTER" );
            break;
        case STACK_CANARY_A:
        strcpy( stack_status, "STACK CANARY A ERROR" );
            break;
        case STACK_CANARY_B:
        strcpy( stack_status, "STACK CANARY B ERROR" );
            break;
        case NULL_POINTER_DATA:
        strcpy( stack_status, "NULL POINTER DATA" );
            break;
        case DATA_CANARY_A:
        strcpy( stack_status, "DATA CANARRY A ERROR" );
            break;
        case DATA_CANARY_B:
        strcpy( stack_status, "DATA CANARRY B ERROR" );
            break;
        case ZERO_SIZE:
        strcpy( stack_status, "ZERO SIZE" );
            break;
        case ZERO_CAPACITY:
        strcpy( stack_status, "ZERO CAPACITY" );
            break;
        case SIZE_GR_CAPACITY:
        strcpy( stack_status, "SIZE GREATER THAN CAPACITY" );
            break;
        case NAN_ELEMENT:
        strcpy( stack_status, "NaN ELEMENT" );
            break;
        case CHECKSUM_INVALID:
        strcpy( stack_status, "CHECKSUM INVALID" );
            break;
        case OK:
        default:
        strcpy( stack_status, "OK" );
            break;
    }
    
    printf( "->---<-\n" );
    
    printf( "Stack(%s) [0x%p] \"%s\"\n", stack_status, stack, stack->var_name );
    
    if ( strcmp( stack_status, "NULL STACK" ) == 0 )
    {
        printf( "\nSTACK HAS NULL POINTER\n" );
        
        printf( "->---<-\n" );
        
        return;
    }
    
    printf( "{\n\tsize = %zu\n\tcapacity = %zu\n\tbuffer[0x%p]"
           "\n\tstack_canarry_a = 0x%p\n\tstack_canarry_b = 0x%p\n",
           stack->size, stack->capacity, stack->data, stack->stack_canarry_a, stack->stack_canarry_b );
    
    if ( strcmp( stack_status, "NULL BUFFER" ) == 0 )
    {
        printf( "\nSTACK BUFFER HAS NULL POINTER\n" );
        
        printf( "->---<-\n" );
        
        return;
    }
    
    printf( "\n\tbuffer_canarry_a = 0x%p\n\tbuffer_canarry_b = 0x%p\n",
           stack->stack_canary_a, stack->stack_canary_b );
    
    if ( strcmp( stack_status, "\nSIZE GREATER THAN CAPACITY\n" ) == 0 )
    {
        printf( "\nSIZE GREATER THAN CAPACITY\n" );
        
        printf( "->---<-\n" );
        
        return;
    }
    
    printf( "\t{\n" );
    for ( size_t i = 0; i < stack->capacity; ++i )
    {
        if ( !isnan( stack->data[i] ) )
        {
            printf( "\t\t+[%zu] = %lg\n", i, stack->data[i] );
        }
        else
        {
            printf( "\t\t [%zu] = NaN\n", i );
        }
    }
    printf( "\t}\n" );
    printf( "}\n" );
    
    
    printf( "->---<-\n" );
}
