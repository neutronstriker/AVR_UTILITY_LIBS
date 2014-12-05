#ifndef _DATA_STRUCTURES_
#define _DATA_STRUCTURES_ 1

//Important : Please be carefull while using stack as multiple functions are using the same stack so use it carefully.
//This cannot be used as a alternative to the default stack of the processor, because multiple programs can 
//modify the TOP and BOTTOM value. But it can be used in functions where data processing involves use of 
//stack but it completes its all operation along with retrieving data from the stack without interruption.

//It means you can call a function which will load data into the stack and get back all the data within the same
//call. Or else make sure that no other stack based function is used.

#define max 20
unsigned char stack[max]; //stack size
int top = -1;  //Initialize stack pointer

void push(unsigned char);
unsigned char pop();

void push(unsigned char element)
{
	if(top == (max-1))
	{
		lcd_print("Stack is full" ,0x80);
		return;
	}
	top = top+1;
	stack[top] = element;
}

unsigned char pop()
{
	if(top == -1)
	{
		lcd_print("Stack is empty", 0x80);
		return 0;
	}
	return(stack[top--]);

}

//In future we are going to build queue,circular queue(fifo buffer), linkedlist here

#endif