/*************************************************************************************
**Autor:Ruben Romero
**Description: Simple scanner for a simplified sintax of C
**************************************************************************************/


#include <stdio.h>  
#include <stdlib.h>
#include <string.h>
#include "vector.h"



//_________________________________________________________________________________________________________________________________
//Functions to process text

//getLinePos return the number of line of a text counting '/n's
int getLinePos(char * text, int size, int pos){
	int i = 0 ;
	int line = 1; 
	if(pos < size && text != NULL){
		while ( i < pos ){ // < because if in pos there is a '\n', it belong to the current line.
			if(text[i] == '\n')
				line++;
			i++;
		}
	}
	return line;
}

//getPosLine return the first pos of the specified line. Min line == 1
int getPosLine(char *text, int size, int line){
	int i=0;
	int pos=-1;
	if(text != NULL && line > 0 && line < size ){
		while(i < size && line != 1){
			if(text[i] != '\n'){
				line--;
			}
			i++;
		}
		pos = i;
	}
	return pos;
}
//it exchange every char between n and m, both included, for blanks. Return -1 if the function did not change nothing
int blankFromNtoM(char * text, int size, int n, int m){
	int i = 0 ;
	int line= 0;
	//First go to the begining of line.
	if( text != NULL &&  m < size && n<=m  ){
		for(i=n; i<=m ; i++){
			text[i]=' ';
		}

		return 0;
	}
	return -1;

}

//It returns the position of the next "find" in text from pos, not including pos.
int FindNext(char *text, int size, int pos, char find){
	int i = pos+1;
	if( text != NULL && pos >= 0 && pos < size  ){
		while(text[i]!=find && i<size){
			i++;
		}

		return i;
	}
	return -1;
}

//Return the content of text from ini till end
char * getContent(char *text, int size, int ini, int end){
	int tam= end-ini+1, i=0;
	char *res;
	if( text != NULL && ini >= 0 && end < size  ){
		res = malloc((tam+1)*sizeof(char));
		while(i < tam){
			res[i]=text[ini+i];
			i++;
		}
		res[i]='\0';
	}
	//printf("next word  %s\n",res);
	return res;
}

//Return next word in the text. If it did not find a word returns NULL. Also it return ini and end of the word in the file
char *nextWord(char *text, int size,int pos, int *ini, int *end){
	int i=pos, j=-1,c, flagEnd=-1;
	char *res=NULL;
	//case 0
	if(text==NULL || size==0 || pos >= size || pos < 0){
		flagEnd=0;
	}
	// case 1
	else if(size==1 && flagEnd==-1){
		if(text[i]!=' ' && text[i]!='\n'&& text[i]!='\t' && text[i]!='\0'){
			*ini=0;
			*end=0;		
		}
		flagEnd=0;
	}
	// case 2 basic
	else if(flagEnd==-1){
		//advance till next separator 
		if(pos != 0){
			while(size>i && flagEnd != 0){
				if( (text[i]==' ' || text[i]=='\n' || text[i]=='\t' || text[i]=='\0') ){
					flagEnd=0;
				}
				i++;
			}
			flagEnd=-1;
		}
		while(size>i && flagEnd != 0){
			//advance till first character
			if(text[i]!=' ' && text[i]!='\n' && text[i]!='\t' && text[i]!='\0' && j==-1){
				j=i;
			}
			//finding character that closes the word
			if( (text[i]==' ' || text[i]=='\n' || text[i]=='\t' || text[i]=='\0') && j!=-1){
				flagEnd=0;
				i=i-2;
			}
			i++;
		}

		*ini=j;
		*end=i;
	}
	if(i>=i){
		res=malloc((i-j+1)*sizeof(char));
		c=0;
		while(c <= (i-j)){
			res[c]= text[j+c];
			c++;
		}
		res[c]='\0';
	}

	return res;

}

//Return 0 if word is contain in text as a word. It means word is contain in text and is separate by NULL or ' ' \t' or '\n'
int containAsWord(char *text, int size, char *word ){
	int res= -1;
	int i = 0 , ini, end; 
	int flagFind=-1;
	char *w;
	// Case 0
	if(text == NULL || word == NULL || size < 0){
		flagFind=-1;
	}
	// Case 1 base
	else{
		
		while(i < size && flagFind == -1){
			w =nextWord(text,size,i,&ini,&end);
			printf("-----Contain word -%s-   ",w);
			
			if(w==NULL){ //Finish return -1
				i = size; 
			}else if( strcmp(w,word)==0){ // Finish return 0
				flagFind=0;
			}
			else{ // We did not find anything, go on.
				i = end;
			}
		}
	}
	return flagFind;
}

//________________________________________________________________________________________________________________________________
//Source is usefull to read a file and create an array with its content
typedef struct Source {
	char nameFile[100];
	int size;
	FILE* file;
	char *text;
} Source;

//OpenFile opens the file call nameFile in Source
int openFile(Source *source){
	source->file = fopen(source->nameFile,"r");
	if(source->file == NULL) {
        perror("File opening failed");
        return EXIT_FAILURE;
    }
	return 0;
}

// Read the file and write the content in text
int readAllFile(Source *source){
	fpos_t ini;
	int size=0,c,i=0; 
	if(source->file == NULL){
		perror("File is not open");
		return -1;
	}
	
	fgetpos(source->file, &ini); // save start file's position 
	//Get size
    while ((fgetc(source->file)) != EOF) { // standard C I/O file reading loop
       size++;
    }
    size++; // We need to add '\0'
    source->size=size;
    fsetpos(source->file,&ini); // set start position again
    source->text = malloc((size)*sizeof(char)); // reserve memory
    while ((c = fgetc(source->file)) != EOF) { // standard C I/O file reading loop
       source->text[i]=c;
       i++;
    }
    // add end delimiter
    source->text[i] = '\0';
    return 0;
}

// newSource initializes a source.
int newSource(Source *source, char *nameFile){
	strcpy(source->nameFile, nameFile);
	openFile(source);
	readAllFile(source);
	return 0;
}


//delete release the memory reseved by a source.
void source_free(Source *source){
		free(source->text);
		close(source->file);
	
}


//________________________________________________________________________________________________________________________

//struct to save the information about a variable 
typedef struct Variable{
	int ini;
	char name[20];
	char type[20];
	vector usages;
	char scope[50];
	int endScope;

} Variable;

//struct to save the information about a function
typedef struct Function{
	int ini;
	int end;
	char name[20];
	vector parameters;
	char returnType[20];
	vector usages;
	char scope[50];
	int endScope;
} Function;




//__________________________________________________________________________________________________

/*ScanC safes a text that will contain a subset of C code and also it will save in vars, functions, 
structs datas about the declaration and usages of those*/
typedef struct ScanC{
	vector vars;
	vector functions;
	char *text;
	int currentPos;
	int size;
} ScanC;

//it will initializes a ScanC. I return -1 if there is any problem
int newScan(ScanC *scan, char * text, int size){
	if( size < 1 || text == NULL){
		return -1;
	}
	scan->text = malloc(size*sizeof(char));
	strcpy(scan->text,text);
	scan->size = size;
	scan->currentPos=0;
	vector_init(&(scan->vars));
	vector_init(&(scan->functions));
	
	return 0;
}

//Delete scan
int scanC_free(ScanC *scan){
	free(scan->text);
	vector_free(&scan->vars);
	vector_free(&scan->functions);

}

//Function textPreprocess will preprocess the text deleting comenters, includes
int textPreprocess(ScanC *scan){
	int i = 0 ,j;
	int flagEnd=1;

	while(scan->text[i] != '\0' && scan->size >= 2  && scan->text[i+1] != '\0'){
		//Delete includes
		if(scan->text[i] == '#'){
			blankFromNtoM(scan->text,scan->size,i,FindNext(scan->text,scan->size,i,'\n')-1);
		}
		//Delete from "/*"" till "*/"
		if(scan->text[i]=='/'){
			if(scan->text[i+1]=='*'){ // /*
				j=i+1;
				do{
					j =FindNext(scan->text,scan->size,j+1,'*'); 

					if(j != -1 && (j+1)<scan->size && scan->text[j+1]=='/'){
						//blankFromNtoM(scan->text,scan->size,i,j+1);
						
						while(i<=(j+1)){
							if(scan->text[i] != '\n')
								scan->text[i] = ' ';
							i++;
						}
						flagEnd=0;
					}
				}while(flagEnd);

			}
		}

		//Delete from for double slash till the end of the line
		if(scan->text[i]=='/'){
			if(scan->text[i+1]=='/'){
				blankFromNtoM(scan->text,scan->size,i,FindNext(scan->text,scan->size,i,'\n')-1);
			}
		}

		i++;
	}
}


// Return 0 if char is between the regular characters or -1 otherwise
int validChar(char c){
	//valid char
	if(c == '_' || (c >= 48 && c <= 57) || (c >= 65 && c <= 90) || (c >= 97 && c <= 122) ){
		return 0;
	}
	else{
		return -1;
	}
}

//charIsIn finds if c is an element of group and returns its position, or -1 if it was not found.
int charIsIn(char *group, int size, char c){
	int i = 0, res=-1; 
	while(i<size){
		if(c == group[i])
			res=i;
		i++;
	}
	return res;
}

/*Function to get the next world, and its starting and ending pos. For instance, a possible world:
	* , home , int, static, struct,  { , }...
	"*", "{", "}", "(" , ")", ";" could be a word but these char cannot be combined with another to form longer words.
	Char to separate word
	-group1: ' ' '\n' '\t'
	-group2: '=' '+' '-' '/' '.' ','  ':' '&' '%' '$' '-' '"' ''' '!' '¡' '?' '¿' '|' '@' '#'
	'~' '½' '¬' 'Ç' '^' '`' or any letter with accentuation 

	To sum up, the valid charact to form word (itself or combinen each another) are : 48-57 && 75-90 && 97-122 && '_'
	and the charact that just can form word without combine with another are: "*", "{", "}", "(" , ")", ";"

	it returns -1 if the value returned are not valid.

*/
// GetNextWord is function of scan that obtain the next valid word in the text. Return 0 if it finds the word and -1 otherwise
int getNextWord(ScanC *scan,int startPos, int *ini, int *end){
	int i = startPos, j=0, start=-1, ret=-1;
	char res[100];
	char separators[]= {' ','\n','\t','\0'};
	char specialChars[] = {'*', '{', '}', '(' , ')', ';','/','+','-','\0'}; // IF SOMETHING IS WORKING WORNG CHECK IT 

	if( scan->text != NULL && startPos < scan->size && startPos >= 0 ){
		ret=0;


		//advance till valid possition to start a new word
		//Case 0: Current pos contain a valid char
		if( validChar(scan->text[i]) == 0 ){
			
			//we are at the beginning of our text  or previous char in the text is a special char or a separator
			if( startPos == 0 || charIsIn(separators,strlen(separators),scan->text[i-1]) != -1 
			|| charIsIn(specialChars,strlen(specialChars),scan->text[i-1]) != -1 ){

				// we can start from here
				start=0;
			}

		}

		//we need a ' ' || '\n' || '\t'  || '{'  || '}'  || '(' || ')' || ';' || '*'
		while(i < scan->size && charIsIn(separators,strlen(separators),scan->text[i]) == -1 
			&& charIsIn(specialChars,strlen(specialChars),scan->text[i]) == -1 && start == -1){

			if(scan->text[i] == '"'){ // skip " "
				i = FindNext(scan->text,scan->size,i,'"')+1;
			}

			i++;
		}
		start=0;
		*ini=i;
		

		//If we have a ' ' || '\n' || '\t' , then advance till next validChar skipping separators, ','s...
		if(charIsIn(separators,strlen(separators),scan->text[i]) > -1){
			// then advance till next validChar
			while(i < scan->size && charIsIn(specialChars,strlen(specialChars),scan->text[i]) == -1 && validChar(scan->text[i]) == -1) {
				//If we find '"'
				if(scan->text[i] == '"'){ // skip "  "
					i = FindNext(scan->text,scan->size,i,'"')+1;
				}
				i++;
			}
			*ini=i;
		}

		// Now we have a validChar to combine or a specialChar
		// If it is a specialChar we can return it
		if(charIsIn(specialChars,strlen(specialChars),scan->text[i]) > -1){
			*end=i;			
		}
		else if(i < scan->size){ // it is a validChar, so we will included validChars till find another character
			while(i<scan->size && validChar(scan->text[i])==0){
				i++;				
			}
			*end=i-1; // last valid char
		}
		else{
			ret=-1;
		}
		
	}

	return ret;
}

// Return 0 is the word is equal to any type : char, int, shot, long, float, double, long double, void, struct
int isType(char *word){
	char *types[]={"char","int","short","long","float","double","void","struct", "\0"};
	int i = 0 ;

	if( word != NULL){
		while(types[i] != "\0"){
			if(strcmp(word,types[i])==0)
				return 0;
			i++;
		}
	}
	return -1;

}

/* It will return 0 if succesful and -1 otherwise. Ini and Find will save the begining and the end of the type in text .Type coudl contain static, 
const, unsigned, signed, and always a type. Lasty it can contain '*'. The declaration of a type
always end with a normal word that define the name of this declaration. To make it easier and knowing that the code should compile
previously we can asumme that if we find static or const, unsigned, signed, or a type, every word between it and the name is part of the 
type*/
int getType(ScanC *scan, int startPos, int *ini, int *fin){
	
	int tam=100, size2=100, resSize=0,i, end, ini2;
	int flagEnd=1, flagFindNameDecl=1; // flags
	char *word;
	//printf("Start \n");
	i=startPos;
	while(i < scan->size && flagEnd != 0 && getNextWord(scan,i,&ini2,&end)== 0){

		word = getContent(scan->text,scan->size,ini2,end);
		//printf(" word %s\n",word);
		//It could start by static const, type ...

		if( ( strcmp(word,"static") == 0) || (strcmp(word,"const")==0 )|| (strcmp(word,"unsigned")==0 )|| (strcmp(word,"signed")==0) || (strcmp(word,"volatile")==0) ||
			 (isType(word))==0 ) {
			
			if(flagFindNameDecl==1){ // First element
				flagFindNameDecl=0;
				*ini = ini2;
				//printf("Find begining  %d\n", *ini);
			}
			*fin = end;
		}
		else if(flagFindNameDecl==0){ 
			if(strcmp(word,"*")==0){
				*fin = end;
			}
			else{ // we reach the namDec and we can end.
				flagEnd=0;
			}
		}
		
		i=end+1; // advance pos till end of last found word
		free(word);
	}
	// adjust the parameters to return

	return flagEnd;
}

/*Function scanC_findUsages complete scan with the usages of variables and functions*/
int scanC_findUsages(ScanC *scan){
	int i, j ,k, ini, end, res=0, *usage;
	char *word=NULL;
	Function *fun;
	Variable *var;
	i=0;
	//Iterate over all functions
	while(i< vector_total(&scan->functions) && res==0){
		fun = vector_get(&scan->functions,i);

		vector_init(&fun->usages);

		//Find next '{'
		j=FindNext(scan->text,scan->size,fun->ini,'{');
		//printf("Scaning     %s    %d   %d\n",fun->name,getLinePos(scan->text,scan->size, fun->ini),getLinePos(scan->text,scan->size, j));
		if( j == -1)
			res==-1;
		j++;
		//Start looking till the end of scan->text
		while( j < fun->endScope && res == 0 ){
			if(getNextWord(scan, j, &ini, &end)==0){
				word = getContent(scan->text,scan->size,ini,end);
				j=end+1;
				if( strcmp(word,fun->name)==0){
					usage = malloc(sizeof(int));
					*usage = getLinePos(scan->text,scan->size, ini);
					vector_add(&fun->usages,usage);
					//printf("     %s == %s                   usage %d *usage %d   \n",word,fun->name,usage, *usage);
				}
			}
			else 
				res = -1;

		}
		if( j != -1)
			res=0;

		i++;
		
	}

	//Iterate over all variables
	i=0;
	while(i< vector_total(&scan->vars) && res==0){
		var = vector_get(&scan->vars,i);

		vector_init(&var->usages);
		//Find next 
		j=FindNext(scan->text,scan->size,var->ini,';');
		k=FindNext(scan->text,scan->size,var->ini,')');
		if(j>k){
			j=k;
		}
		
		//printf("Scaning     %s    %d   %d\n",var->name, getLinePos(scan->text,scan->size, var->ini) , getLinePos(scan->text,scan->size, j));
		if( j == -1)
			res==-1;
		
		j++;
		printf("var %s   endS %d\n",var->name, getLinePos(scan->text,scan->size, var->endScope));
		//Start looking till the end of scan->text
		while( j < var->endScope && res == 0 ){
			if(getNextWord(scan, j, &ini, &end) == 0){
				free(word);
				word = getContent(scan->text,scan->size,ini,end);
				j=end+1;
				if( strcmp(word,var->name)==0){
					printf("uso %d\n",getLinePos(scan->text,scan->size, ini));
					usage = malloc(sizeof(int));
					*usage = getLinePos(scan->text,scan->size, ini);
					vector_add(&var->usages,usage);
					//printf("     %s == %s                   usage %d *usage %d   \n",word,var->name,usage, *usage);
				}
			}
			else 
				res = -1;

		}
		if( j != -1)
			res=0;

		i++;
		
	}

	return res;
}

/*Function setEndScopes set the end of all variables and function in scan that belong to scope*/
void setEndScopes(ScanC *scan, char *scope, int end){
	int i ;
	char tmp[100];
	Variable * var;
	Function * fun;
	//First set end of the function that call the method
	//set end Scopes Functions
	i=0;
	while(i < vector_total(&scan->functions)){
		fun = vector_get(&scan->functions,i);
		//sprinf(tmp,"%s %d",)
		//If scope name i
		//printf("setting scope  %s   %s\n",fun->scope, scope);
		if(fun!=NULL && strcmp(fun->scope,scope)==0){
			//printf("setting scope\n");
			fun->endScope=end;
		}
		i++;
	}

	//set end Scopes Variables
	i=0;
	while(i < vector_total(&scan->vars)){
		var = vector_get(&scan->vars,i);

		if(var!=NULL){
			strcpy(tmp,scope);
			strcat(tmp," param");
			if( strcmp(var->scope,scope)==0 || strcmp(var->scope,tmp)==0){
				var->endScope=end;
			}
		}
		i++;
	}
}

/* Set the end of a function. Return 0 if it did the operation*/
int setEndFunction(ScanC *scan, char *name, int end){
	int flagEnd = -1;
	int i ;
	char nameC[100];
	Function * fun;
	//Find the function with the same name and end == -1
	i=0;
	while(i < vector_total(&scan->functions) && flagEnd == -1){
		fun = vector_get(&scan->functions,i);
		sprintf(nameC,"%s %d",fun->name, getLinePos(scan->text, scan->size,fun->ini));
		if(fun!=NULL && strcmp(nameC,name)==0 && fun->end == -1){
			fun->end = end;
			flagEnd = 0;
		}
		i++;
	}

	return flagEnd;
}

/*Function scanC_scan will scan the text, find the types, names, and so on. It will save the datas in the vectors vars, functions, structs */
int scanC_scan(ScanC *scan, int start, int final, char *scope){
	int  ini, end, ini2, end2, line, pos;
	int flagEnd=1;
	char *type=NULL, *type2=NULL, *name=NULL, *name2=NULL, *other=NULL, *other2=NULL, tmp[100];
	int i = 0;
	Function *fun =NULL;
	Variable *var =NULL;

	scan->currentPos=start;
	if( scan->text != NULL && final <= scan->size){
		//printf("\nI am in scan %s\n",scope);
		
		//From the start till final
		while(scan->currentPos < final && flagEnd != 0){
			//printf("\nI am in scan %s\n",scope);
			
			if(getNextWord(scan, scan->currentPos, &ini, &end)==0){
				
				free(other);
				other = getContent(scan->text,scan->size,ini,end);
				printf("%s  \n\n", other);
				/*Reflexion:
					Next word could be:
						}.....                          Case 1: End 
						if, else, while, do, for .....  Case 2: Analyze structure
						int, double .....               Case 3: Analyze variable, function
						ohter case                      Case 4: update currentPos
				*/
				//Case 1: The code start with '}' we will end.    END OF FUNCTION DEFINITION __________________________________________________________
				//A function declaration will call scan recursively, so here we will wait for '{'
				if(strcmp(other,"}")==0){
					scan->currentPos=end+1; // Update position to next
					printf("                                                    END OF RECURSIVITY %s   end  %d\n",scope,getLinePos(scan->text,scan->size,end));
					flagEnd=0;
				}
				//Case 2: if, else, while, do, for .....  Analyze structure and subcases _____________________________________________________
				else if(strcmp(other,"if")==0 || strcmp(other,"else")==0 || strcmp(other,"do")==0 || strcmp(other,"for")==0 || strcmp(other,"while")==0 ){	
					//A variable declaration will end with ; so here I wait for next word
					//Next word can be a type, ';', or a token (if, else, for, while, do)
					if(strcmp(other,"if")==0 || strcmp(other,"else")==0 || strcmp(other,"do")==0 || strcmp(other,"for")==0){
						//printf("                              we found a %s \n",other);
						
						scan->currentPos=end;// Update position
						
						strcpy(tmp,"\0");
						line = getLinePos(scan->text,scan->size, ini);

						if(strcmp(other,"else")==0){
							//int ini2, end2;
							//char *other2
							getNextWord(scan, scan->currentPos, &ini2, &end2);
							other2 = getContent(scan->text,scan->size,ini2,end2);

							if(strcmp(other2,"if")==0){
								sprintf(tmp, "else if line %d",line);
							}
							else{
								sprintf(tmp, "else line %d",line);
							}
						}
						else{
							sprintf(tmp, "%s line %d",other, line);
						}
						//If there is a if we need to find next '{' and change scope and call recursively to scan
						pos = FindNext(scan->text,scan->size, scan->currentPos,'{');
						scanC_scan(scan,pos+1,scan->size,tmp);
						
					}
					else if(strcmp(other,"while")==0){	
						//if we have a while we need to check if it is a while or the end of do while
						//Check next word after )
						ini2=ini;
						scan->currentPos=end;// Update position
						pos = FindNext(scan->text,scan->size, scan->currentPos,')');
						if(getNextWord(scan, pos+1, &ini, &end)==0){
							other = getContent(scan->text,scan->size,ini,end);
							//printf("           while  other %s\n",other);
							//  } WHILE(    );
							if( strcmp(other,";")==0){ 
								//printf("                              we found a %s from do  \n",other);
								scan->currentPos=ini+1;
							}
							// WHILE(   ){    }
							else{
								//printf("                              we found a %s \n",other);
								pos = FindNext(scan->text,scan->size, scan->currentPos,'{');
								strcpy(tmp,"\0");
								line = getLinePos(scan->text,scan->size, ini2);
								sprintf(tmp, "while line %d", line);
								scanC_scan(scan,pos+1,scan->size,tmp);
							}
						}
						else{
							flagEnd=0;
						}
					}
				}
				//Case 3: int, double .....        Analyze variable, function	
				//Case 3.2: Varible  ReadType -> GetNextWord = namedef -> GetNextWord != ( -> advance till next ;  
				//Case 3.1 Function	 ReadType -> GetNextWord = namedef -> GetNextWord == ( -> readParameters -> GetNextWord = ) ->  advance till next { -> call functino scan

				else if( ( strcmp(other,"static") == 0) || (strcmp(other,"const")==0 )|| (strcmp(other,"unsigned")==0 )|| (strcmp(other,"signed")==0) ||
				 (strcmp(other,"volatile")==0) || (isType(other))==0 ) {
					// Next word will form a type.

					if(getType(scan,scan->currentPos,&ini,&end)==0 && flagEnd != 0){
						type = getContent(scan->text,scan->size,ini,end);
						scan->currentPos=end+1; // Update position
						line = getLinePos(scan->text,scan->size, ini);
						if(type!=NULL ){
							//printf( "The type is %s  in line %d and scope %s\n", type,line,scope);
							// Always after a type there is a namedef, so now we will get the namedef, 
							if(getNextWord(scan, scan->currentPos, &ini, &end)==0 ){
								free(name);
								name = getContent(scan->text,scan->size,ini,end);
								scan->currentPos=end+1; // Update position
								//printf( "The name is %s, scope %s\n", name,scope);

								// Analizying next word. We already got type and name.
								/*Case 3.1: nextWord = ( . Analyze function     GetNextWord = namedef -> GetNextWord == ( -> readParameters -> GetNextWord = ) ->  advance till next { -> call functino scan
								  Case 3.2: nextWord = ; . Analyze variable    Varible  ReadType -> GetNextWord = namedef -> GetNextWord != ( -> advance till next ;  
								  
								*/
								if(getNextWord(scan, scan->currentPos, &ini, &end)==0 ){
									free(other);
									other = getContent(scan->text,scan->size,ini,end);
									scan->currentPos=end+1;
								    //printf( "the other is %s scope %s\n", other, scope);
									//Here we need to analyze 2 cases, Variable declaration or Function declaration.
									//If next word if it is '(' is is a function otherwise is a variable;
									
									//Case 3.1: nextWord = ( . Analyze function.            WE HAVE A FUNCTION
									if(strcmp(other,"(")==0 ){
								       //		printf("we found a function\n");
										fun = malloc(sizeof(Function));
										fun->ini = ini ;
										fun->end = -1;
										strcpy(fun->name,name);
										vector_init(&fun->parameters);
										strcpy(fun->returnType,type);
										sprintf(fun->scope,"%s",  scope);


										getNextWord(scan,scan->currentPos,&ini,&end);
										free(other);
										other = getContent(scan->text,scan->size,ini,end);
										//Case3.1.1                                          ANALYSIS OF PARAMETERS
										while(strcmp(other,")") != 0 && flagEnd != 0){
											//	printf( "looking of ) The next is %s \n", other);
											//if other is not ) there has to be a type.      
											getType(scan,scan->currentPos,&ini,&end);        //  GET TYPE PARAMETER
											ini2=ini;
											free(type2);
											type2 = getContent(scan->text,scan->size,ini,end);
											scan->currentPos=end+1;

											getNextWord(scan, scan->currentPos, &ini, &end) ; // GET NAMEDEF PARAMETER
											free(name2);
											name2 = getContent(scan->text,scan->size,ini,end);
											scan->currentPos=end+1;

											var = malloc(sizeof(Variable));
											var->ini= ini2;
											strcpy(var->name, name2); 
											strcpy(var->type,type2);
											sprintf(var->scope,"%s %d param",  fun->name, getLinePos(scan->text, scan->size, fun->ini));
											vector_add(&scan->vars,var);
											vector_add(&fun->parameters,var); // Add to parameters of function also

											
											// I cannot free type2 name 2
										    // Next word should be a ',' or ')'
										    getNextWord(scan,scan->currentPos,&ini,&end);
										    free(other);
										    other = getContent(scan->text,scan->size,ini,end);
											//			printf( "next other is %s \n", other);
										}
										//Last read char was ')' so next we will look for the '{' that open a new scope
										
										scan->currentPos=end+1;
										vector_add(&scan->functions,fun);
										
										//scan->currentPos=FindNext(scan->text,scan->size, scan->currentPos)+1;
										while( getNextWord(scan,scan->currentPos,&ini,&end)==0 && strcmp(other,"{") != 0){
											free(other);
											other = getContent(scan->text,scan->size,ini,end);
											scan->currentPos=end+1; 
										}

										char newScope[100]="\0";
										sprintf(newScope,"%s %d",fun->name, getLinePos(scan->text, scan->size, fun->ini));

										
										//                                                 ANALYZE NEW SCOPE (RECURSIVITY) 
										scanC_scan(scan,scan->currentPos+1,scan->size,newScope);

									}
									//Case 3.2:                                           WE HAVE A VARIABLE
									else{     
									//	printf("we found a variable\n");
										var = malloc(sizeof(Variable));
										var->ini= ini;
										strcpy(var->name, name); 
										strcpy(var->type,type);
										sprintf(var->scope,"%s",  scope);
										vector_add(&scan->vars,var);

										//Go till ';'  .    END OF VARIABLE DECLARATION
										//scan->currentPos=FindNext(scan->text,scan->size, scan->currentPos,';')+1;
										while( strcmp(other,";") != 0 && getNextWord(scan,scan->currentPos,&ini,&end)==0){
											free(other);
											other = getContent(scan->text,scan->size,ini,end);
											scan->currentPos=end+1; 
										}
										
									    //	printf( "                          found  %s after var %s              \n", other,var->name);
										scan->currentPos=end+1; //To avoid ; next

									}

								}
								else{
									flagEnd=0;
								}


							}
							else{
								flagEnd=0;
							}
						}
						else{ // if there is no thype we can end.
							flagEnd=0;
						}
					}
					else{ // if there is no thype we can end.
						flagEnd=0;
					}
				}
				//Case 4. Otherwise
				else{
					scan->currentPos=end+1; // Update position to next
				}
				
			}
			else{
				flagEnd=0;
			}

		}	
	}

	free(other);
	free(name);
	free(type);
	free(name2);
	free(type2);
	
	

	//Complete usages
	if(strcmp(scope,"global")==0){
		end=scan->size-1;
	}
	else{ // set end of this function
		setEndFunction(scan,scope,end);
	}

	setEndScopes(scan,scope,end);
	
	if(strcmp(scope,"global")==0){
		scanC_findUsages(scan);
	}	

	if(flagEnd==0)
		return -1;
	else
		return 0;
}




/*PrintScan print the datas about scan with printf */
int printScan(ScanC *scan){
	int i = 0, j = 0, k =0, *a;
	char parameters[500]="\0";
	char usages[500]="\0", line[4]="\0";
	Variable *var;
	Function *fun;
	

	printf("Functions:\n");
	printf("\t%8s\t|\t%20s\t|\t%35s\t|\t%23s\t|\t%23s\t|\t%23s\t\n","Line","Name","Parameters","Return","Usages","Scope");
	printf("\t_________________________________________________________________________________________________________________");
	printf("__________________________________________________________________\n");
	while(i < vector_total(&scan->functions)){
		fun = vector_get(&scan->functions,i);

		if(fun!=NULL){
			//Preparation of parameters
			j=0;
			while(j < vector_total(&fun->parameters)){
				var = vector_get(&fun->parameters,j);
				strcat(parameters,var->name);
				strcat(parameters,"  ");
				j++;
			}
			j=0;
			while(j < vector_total(&fun->usages)){
				a = vector_get(&fun->usages,j);
				sprintf(line,"%d ",*a);
				strcat(usages,line);
				j++;
			}
			printf("\t%8d\t|\t%20s\t|\t%35s\t|\t%23s\t|\t%23s\t|\t%23s  %d\t\n",getLinePos(scan->text,scan->size,fun->ini), fun->name, parameters, fun->returnType, usages, fun->scope, getLinePos(scan->text,scan->size,fun->end));
		}

		strcpy(parameters,"\0");
		strcpy(usages,"\0");
		i++;
	}

	printf("\nVariables:\n");
	printf("\t%8s\t|\t%15s\t|\t%23s\t|\t%30s\t|\t%23s\t\n","Line","Name","Type","Usages","Scope");
	printf("\t__________________________________________________________________________________________________________________\n");
	i=0;
	while(i < vector_total(&scan->vars)){
		var = vector_get(&scan->vars,i);
		if(var!=NULL){
			j=0;
			while(j < vector_total(&var->usages)){
				a = vector_get(&var->usages,j);
				sprintf(line,"%d ",*a);
				strcat(usages,line);
				j++;
			}
			printf("\t%8d\t|\t%15s\t|\t%23s\t|\t%30s\t|\t%23s %d\t\n",getLinePos(scan->text,scan->size,var->ini), var->name, var->type,usages, var->scope, getLinePos(scan->text,scan->size,var->endScope));
		}

		strcpy(parameters,"\0");
		strcpy(usages,"\0");
		i++;
	}

}

/*PrintScan print the datas about scan with fprintf in the archivo fp */
int printScanFile(ScanC *scan, FILE *fp){
	int i = 0, j = 0, k =0, *a;
	char parameters[500]="\0";
	char usages[500]="\0", line[4]="\0";
	Variable *var;
	Function *fun;
	
	if(!fp){ // check if failed is open to write.
		perror("Failed file\n");
		return -1;
	}

	fprintf(fp,"Functions:\n");
	fprintf(fp,"%8s\t|\t%20s\t|\t%30s\t|\t%23s\t|\t%23s\t|\t%23s\n","Line","Name","Parameters","Return","Usages","Scope");
	fprintf(fp,"\t_________________________________________________________________________________________________________________");
	fprintf(fp,"___________________________________________________________\n");
	while(i < vector_total(&scan->functions)){
		fun = vector_get(&scan->functions,i);

		if(fun!=NULL){
			//Preparation of parameters
			j=0;
			while(j < vector_total(&fun->parameters)){
				var = vector_get(&fun->parameters,j);
				strcat(parameters,var->name);
				strcat(parameters,"  ");
				j++;
			}
			j=0;
			while(j < vector_total(&fun->usages)){
				a = vector_get(&fun->usages,j);
				sprintf(line,"%d ",*a);
				strcat(usages,line);
				j++;
			}
			fprintf(fp,"%8d\t|\t%20s\t|\t%30s\t|\t%23s\t|\t%23s\t|\t%23s\n",getLinePos(scan->text,scan->size,fun->ini), fun->name, parameters, fun->returnType, usages, fun->scope);
		}

		strcpy(parameters,"\0");
		strcpy(usages,"\0");
		i++;
	}

	fprintf(fp,"\nVariables:\n");
	fprintf(fp,"\t%8s\t|\t%15s\t|\t%23s\t|\t%30s\t|\t%23s\t\n","Line","Name","Type","Usages","Scope");
	fprintf(fp,"\t__________________________________________________________________________________________________________________\n");
	i=0;
	while(i < vector_total(&scan->vars)){
		var = vector_get(&scan->vars,i);
		if(var!=NULL){
			j=0;
			while(j < vector_total(&var->usages)){
				a = vector_get(&var->usages,j);
				sprintf(line,"%d ",*a);
				strcat(usages,line);
				j++;
			}
			fprintf(fp,"\t%8d\t|\t%15s\t|\t%23s\t|\t%30s\t|\t%23s\t\n",getLinePos(scan->text,scan->size,var->ini), var->name, var->type,usages, var->scope);
		}

		strcpy(parameters,"\0");
		strcpy(usages,"\0");
		i++;
	}

}

int main(){
	int i, j, line;
	char file[100],name[100]="\0",scope[20]="global";
	Source source;
	ScanC scan;
	FILE *fp;
	

	printf("______________________________________________EXECUTING_______________________________________________________\n");
	
	printf("Type the name of the file where you want to save the samples' outputs\n->");
	//scanf("%s",file);

	strcpy(file,"tables.txt");
	fp= fopen(file,"w");

	printf("Type the name of the file that you want to analyse\n->");
	//scanf("%s",file);
	if(!fp){
		printf("Failed to open file %s\n",file);
		return -1;
	}

	i=1;
	while(i<=9){
		sprintf(name,"sample%d.c",i);
		printf("\n%s completed\n",name);
		if(newSource(&source,name)==0){
			fprintf(fp,"Analysis of %s\n\n",name);
			
			j=0;
			line=1;
			if(source.text[j] != '\0')
				fprintf(fp, "%d-  ", line);
			while( source.text[j] != '\0'){
				fprintf(fp, "%c", source.text[j]);
				if( source.text[j] =='\n'){
					line++;
					fprintf(fp, "%d-  ", line);
				}
				j++;
			}

			newScan(&scan,source.text,source.size);
			textPreprocess(&scan);

			/* Reprocess
			fprintf(fp,"\n\n");
			j=0;
			line=1;
			if(scan.text[j] != '\0')
				fprintf(fp, "%d-  ", line);
			while( scan.text[j] != '\0'){
				fprintf(fp, "%c", scan.text[j]);
				if( scan.text[j] =='\n'){
					line++;
					fprintf(fp, "%d-  ", line);
				}
				j++;
			}
			*/
			scanC_scan(&scan,0,scan.size,scope);
			fprintf(fp,"\n\nTABLES:\n\n");
			printScanFile(&scan,fp);
			source_free(&source);
			scanC_free(&scan);
			fprintf(fp,"\n\n_____________________________________________________________________________________________________\n\n\n");
		
		}
		i++;
	}
	
	
	fclose(fp);
	printf("\n______________________________________________COMPLETED_______________________________________________________\n");
	return 0;
}
