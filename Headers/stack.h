    //
    //  stack.h
    //  mortal-stack
    //
    //  Created by Alexander Makhov on 8/10/20.
    //

#ifndef stack_h
#define stack_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <time.h>

#define EPS     1e-9
#define NOLL    NAN

#define сonstrStack( pseudo_address, start_size )                  \
char* var_name = (char*) calloc( 72, sizeof ( char ) );            \
strcpy( var_name, # pseudo_address );                              \
constrStack( pseudo_address, start_size, var_name + 1 );           \
free( var_name )

#define randnum(min, max) \
((rand() % (int)(((max) + 1) - (min))) + (min))

typedef double elem_t;

typedef enum OPERATION
{
    PUSH,
    POP
} OPERATION;

typedef enum ERROR_MESSAGE
{
    OK,
    NULL_POINTER_STACK,
    STACK_CANARY_A,
    STACK_CANARY_B,
    DATA_CANARY_A,
    DATA_CANARY_B,
    NULL_POINTER_DATA,
    ZERO_CAPACITY,
    ZERO_SIZE,
    SIZE_GR_CAPACITY,
    CHECKSUM_INVALID,
    NAN_ELEMENT
} ERROR_MESSAGE;

typedef struct Stack
{
    long int* stack_canarry_a;
    
    size_t  size;
    size_t  capacity;
    elem_t  * data;
    char    * var_name;
    int     checksum;
    
    long int* stack_canary_a;
    long int* stack_canary_b;
    
    long int* stack_canarry_b;
} Stack;

    //! Calculates checksum of stack structure
    //! @param [in] stack Pointer to stack structure
    //! @return Returns the checksum in int
int checkSum( Stack* stack );

    //! A stack constructor
    //! @param stack Pointer to stack, allocated by newStack function
    //! @param start_capacity Describes starting capacity of stack
    //! @param var_name Name of stack structure
void constrStack( Stack* stack, const size_t start_capacity, const char* var_name );

    //! Stack allocator
    //! @param start_capacity Defines capacity of allocated memory for stack structure
Stack* newStack( const size_t start_capacity );

    //! Function to add a new element to the end of stack
    //! @param stack Pointer to stack structure
    //! @param value An element to push
void stackPush( Stack* stack, const elem_t value );

    //! Returns the top of the stack, without popping it
    //! @param stack Pointer to stack structure
    //! @return Element of elem_t type, sitting on top of the stack
elem_t topStack( Stack* stack );

    //! Removes the element on top of the stack, what else did you expect?
    //! @param stack Pointer to stack structure
void stackPop( Stack* stack );

    //! Deletes stack, freeing used memory
    //! @param stack Pointer to stack structure
void deleteStack( Stack* stack );

    /// Bool-typed validator of Stack structure
    /// @param [in] stack Pointer to stack structure
    /// @return Returns an enumeration of possible errors
ERROR_MESSAGE stackOK( Stack* stack );

    //! Dumps every call of stack functions to STDIN
    //!
    //! @param [in] stack  Pointer to Stack structure
    //!
    //! @todo Route this to file
void stackDump( Stack* stack );

#endif /* stack_h */
