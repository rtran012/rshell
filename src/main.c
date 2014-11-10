#include "myCString.h"
#include <stdio.h>
int main(){
	char str[50] = {'a', 'b', 'c', 'd', 'e', 'f' , 'g' , 'h', 'i', 'j', 'k'};
	
	/* This is the length of the string*/
	char des[50];
	int len;	
	len = my_strlen(str);
	printf("\nLength of str %s is %d\n", str, len);
        
	/* This is strcpy which copies from one array of characters to the destination of an array of charcters*/
	len = my_strlen(des);
	printf("\nArray des without copying str %s with size %d\n", des, len);
	my_strcpy(des, str);
	len = my_strlen(des);
	printf("\nCopying from str to des %s with size %d\n", des, len);

	/* This is strncpy. It operates the same way as strcpy but it only copies the amount of characters you need.*/
       	len = my_strlen(des);
	printf("\nLength of des with out copying %s is %d\n", des, len);
	my_strncpy(des, str, 5);
	len = my_strlen(des);
	printf("\nLength of des %s is %d\n", des, len);

	/*This is the strncat.*/
	my_strncat(des, str, 5);
	len = my_strlen(des);
	printf("\nConcatation of des and str with buff size 5: %s\n", des);

	/*This is strcat*/
	my_strcat(des,str);
	printf("\nConcatation of des and str: %s\n", des);

        /*This is strcmp*/
	int ans;
 	ans = my_strcmp(des, str);
	printf("comparing des to str");
	if(ans == 0)
	  printf("\nThey are equal to each other.\n");
	else
	  printf("\nThey are not equal to each other.\n");
	printf("comparing str to copy str");
	
	char strcpy[50];
	my_strcpy(strcpy, str);
	ans = my_strcmp(strcpy, str);
	if(ans == 0)
	  printf("\nThey are equal to each other.\n");
	else
	  printf("\nThey are not equal to each other.\n");
	
	/*This is strchar()*/
	char* string1="Hello World";
	printf("\nThe first index of o is %d\n", my_strchr(string1,'o')-string1);	
	
	/*This is strstr() */
	char * string2="Hello World what's up";
	printf("\nThe first occurence of Hello World is at %d\n",my_strstr(string2,string1)-string2);
	return 0;
}
