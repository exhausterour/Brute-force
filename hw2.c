/*
  Author: Anthony Garcia
  Email: anthony2018@my.fit.edu
  Course: CSE 2010
  Section: 02

  Description: Create an application that read words from a file that are already encrypeted and develop a brute force method that will decrypt the words.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "hw2_crypto.h"

//strucuture for the linked list
typedef struct encryption{
	char encrypted[41];
	bool found;
	struct encryption *next;
}ENCRYPTION;

/*add end function receives the address of the array, the element that wants to be added, and the length of the word. 
With that information it adds the element at the end of the word*/
void add_end(char word[], char element, int length);
/*The read file function receives the address of the file and the head of the linked list. We use the address of the file to read the 
encrypted word and its added to the linked list*/
void read_file(FILE *file_pointer, ENCRYPTION **head);
/*The print list function only receives the head of the linked list. It uses the head of the linked list to print all the word in it*/
void print_list(ENCRYPTION **head);
/*The strcmp_linkedlist receives the head of the linked list and the address of the array. What it does is compare the array when its encrypted to
the encrypted words in the linked list that are not found. It returns 1 if all the words are found and 0 if not all the words are founded*/
int strcmp_linkedlist(ENCRYPTION *head, char word[]);
/*The free_lisl function receives the head of the linked list and free's the whole linked list.*/
void free_list(ENCRYPTION **head);
/*The decryot fucntion receives the head of the linked list, a char array, the position of the string, the maximum position of the string at that moment, 
the maximum position of the string at all moments, and at what letter is the word at. What it does is recursively goes through all the possible words from 1 charcater
to the maximum amount of characters. It returns 1 if all the words were found and 0 if not all the words were found.*/
int decrypt(ENCRYPTION *head, char word[], int high, int current_high ,int always_high, int pos);
/*The all_equal function receives the length of the array and the array. It checks if all the letters are the same in the word. This function was created for the 
the combination all z's so it will end at that time and not before. Returns 1 if all are equal and 0 if not*/
int all_equal(char word[], int length);

int main(int argc, char* argv[]){
	//the maximum size of the encrypted words
	int size_word;
	//the file pointer that will have the address of the file
	FILE *file_pointer = NULL;
	file_pointer =fopen(argv[argc-1],"r");
	//the head of the linked list
	ENCRYPTION *encryption_head = NULL;

	//reads the maximum length from the file
	fscanf(file_pointer, "%d", &size_word);
	//creates a variable with the maximum length of the word +1 for the null character
	char word[size_word+1];
	//starts with no letters only with the null character
	word[0] = '\0';

	//reads the encrypted words from the file until we reach the end of the file and we added those words into the linked list
	while(feof(file_pointer) == 0)
		read_file(file_pointer, &encryption_head);
	//we close the file since we already read everything we needed
	fclose(file_pointer);
	
	//we decrypt all the words, we set the high into 1 since that the minimum and we do the same with the current high. 
	//We also set the pos to 0 since there's no letters in the word
	decrypt(encryption_head, word, 1, 1,size_word, 0);
	//We print the whole linked list
	print_list(&encryption_head);
	//we free the whole linked list
	free_list(&encryption_head);

	return 0;
}

void free_list(ENCRYPTION **head){
	//if the head next is not null then it decomposes until it gets to the base case case which is when you get to next null.
	//Then it composes be freeing everything and setting it to NULL
	if ((*head)->next != NULL)
		free_list(&((*head)->next));
	free(*head);
	*head = NULL;
}

void read_file(FILE *file_pointer, ENCRYPTION **head){
	//It sets a dynamic memory to the linked list node
	ENCRYPTION *new_node = (ENCRYPTION*)malloc(sizeof(ENCRYPTION));
	fscanf(file_pointer, "%s", new_node->encrypted);
	new_node->found = 0;
	new_node->next = NULL;
	//if the length of the string is 0 then do not add it to the list since is not the encrypted word
	if (strlen(new_node->encrypted) == 0)
		return;
	//if the head is null then the new node will be the head. Else will be added at the end of the linked list
	else if(*head == NULL)
		*head = new_node;
	else{
		ENCRYPTION *temp_node = *head;
		//traverse through the whole linked list until it gets to the end and then sets the new node to be the last one
		while(temp_node->next != NULL)
			temp_node = temp_node->next;
		temp_node->next = new_node;
	}
}

void print_list(ENCRYPTION **head){
	//if head is null then do not print
	if (*head != NULL)
	{
		//it traverse through the whole linked list until it gets to null but it prints while it traverse
		ENCRYPTION *temp_node = *head;
		do{
			printf("%s\n", temp_node->encrypted);
			temp_node = temp_node->next;
		}while(temp_node != NULL);
	}
}

int strcmp_linkedlist(ENCRYPTION *head, char word[]){
	int amount_nodes = 0;
	int amount_found = 0;
	if (head != NULL){
		ENCRYPTION *temp_node = head;
		//loop while temp_node is not null
		do{
			//checks if that node has a the word unencrypted
			if (temp_node->found == 0)
			{
				//it checks if they are the same and if they are then change found to 1 and copy the word to that position.
				if (strcmp(temp_node->encrypted, encrypt(word)) == 0){
					temp_node->found = 1;
					strcpy(temp_node->encrypted, word);
					return 0;
				}
			}
			//if its already found then add 1 to found
			else{
				amount_found++;
			}
			//always add 1 to amount node everytime we change position
			amount_nodes++;
			temp_node = temp_node->next;
		}while(temp_node != NULL);
	}
	//if amount found and amount nodes are equal then all the words were found and return 1. If not then return 0.
	if (amount_found == amount_nodes)
	{
		return 1;
	}
	return 0;
}

int decrypt(ENCRYPTION *head, char word[], int high, int current_high ,int always_high, int pos){
	int value = 0;
	//iterates from value 97 to 122 because those are the ascii value of the letters
	for (int i = 97;(i < 123) && (high != 0); i++){
		//adds the ascii value to the end of the word
		add_end(word, i, (int)strlen(word));
		
		//if the base case is true, if high is one, compares with the whole linked list
		if (high == 1){
			//if the whole linked list is found then it returns 1 and the recursion finishes
			if(strcmp_linkedlist(head, word))
				return 1;
		}
		//it decomposes the problem into a simpler problem by reducing the high
		else{
			//if pos is 0 then it means that the recursion just started for another current high
			if(pos == 0)
				value = decrypt(head, word, high-1, current_high, always_high, i-'a');
			else
				value = decrypt(head, word, high-1, current_high, always_high, pos);
			if (value == 1)
				return 1;
		}
		word[(int)strlen(word)-1] = '\0';
	}
	//goes here if the problem is solved for a certain size of characters. If we need a bigger size then will return decrypt but with currenthigh +1 so it can do the next size of characters
	if (current_high <= always_high && (current_high == 1 || pos == 25)){
		if(current_high == 1 || (pos = 25 && all_equal(word, strlen(word)))){
			word[0] = '\0';
			return decrypt(head, word, current_high+1, current_high+1, always_high, 0);
		}
	}
	return 0;
}

int all_equal(char word[], int length){
	int value = 0;
	//checks for the whole array if all the words are equal. If they are equal add 1 to value.
	for(int i = 0; i < length; i++){
		if (word[0] == word[i])
			value++;
	}
	//if value is equal to the length then return 1 becuase all the letters are equal. If not then return 0
	if(value == length)
		return 1;
	return 0;
}

void add_end(char word[], char element, int length){
	//makes the position of the null character be the element that we want to add and the null character will be the next position
	word[length] = element;
	word[length+1] = '\0';
}