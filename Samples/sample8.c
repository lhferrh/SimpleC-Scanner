
#include <stdio.h> 
#include <math.h> 

int STUDENT = 10; 

int asssingSchool(int student){ 
	int generateRand(int n){ 
		return n*rand(); 
	} 

	int school = generateRand(student)*100; 

	return school; 
} 

int assingClass( int student, int school){ 
	int generateRand(int n){ 
		return n*rand(); 
	} 

	int class = generateRand(student*school)*200; 
	return class; 
} 

int main(int argn, char ** argc){ 
	int student = 10; 
	int school = asssingSchool(student); 
	int class = assingClass(student, school); 
 /*asdfasdfsad   */ 
	printf("The student %d will go to school %d and class %d \n", student , school , class ); 

}