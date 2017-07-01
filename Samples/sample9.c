


#include <stdio.h>



// Test C parser with a real set of code

/*

The set of code should fulfill all the condition establish in the project 

*/
int MAX=10;

/* Fibonacci Series c language */
int nextfibonacciNumber(int *first, int *second){
	int next;

	next = *first + *second ; 
	*first = *second;
	return next;

}


int fibonacciCalc(int N, int f, int s)
{
   int *first;
   int *second;
   int c;
 
 	first = &f;
 	second= &s;
   printf("First %d terms of Fibonacci series are :-\n", N);
 	
   for ( c=0  ; c < N && c < MAX ; c++ )
   {
      if ( c < 1 )
          printf("%d\n",*first);
      else if ( c == 1){
          printf("%d\n",*second);
      }
      else
      {
      	*second = nextfibonacciNumber(first, second);
      	printf("%d\n",*second);
      }
      
   }
 
   return 0;
}

int main(){
	fibonacciCalc(8,1,2);
}
