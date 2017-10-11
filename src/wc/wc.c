/*
 ============================================================================
 Name        : wc.c
 Author      : Nathaniel Churchill
 Professor   : Dr. David Smith
 Description :  C, Ansi-style progam to read text file and print how many times
 	 	 	 	all the words occur in the given file
 ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct node {
  int count;
  char *word;
  struct node *next;
} Node;

/*
*   This function accepts a pointer to a node and pointer to a character.
*   The created node gets placed after the passed node.
*
*   head: a pointer to a node
*   word: a pointer to a string
*
*   returns: a pointer to the created node
*/
Node *makeNode ( Node *head, char *word ) { //inserts node after a given node
		 Node *current = NULL;
		 current = malloc(sizeof(Node));
		 current->word = malloc(strlen(word) + 1);
		 strcpy(current->word, word);
		 current->next = NULL;
		 current->count = 0;
		 head->next = current;
		 return current;
}

/*
*   This function accepts a pointer to a node and recursively prints the nodes
*   their counts
*
*   head: a pointer to a node
*/
static void printList (Node *head){
	if (head != NULL){
		printf ( "%-10s   %d\n", head->word,  head->count);
		head = head->next;
		printList(head);
	}else {
		printf("List has ended\n");
	}

}

/*
*   This function accepts a pointer to a node and pointer to a character.
*   The function then finds the node or creates a new node recursively in
*   ascending order
*
*   head: a pointer to a node
*   word: a pointer to a string
*
*   returns: a pointer to the found or created node
*/
Node *findNodeForWord(Node *head, char *word){
	if (head->next == NULL){
		Node *insertNode = makeNode(head, word); //insert after the head
		return insertNode;
	}else if (strcmp(head->next->word, word) == 0){//stuff in the list
		return head->next;
	}else if (strcmp(head->next->word, word) < 0){ // list word is less than given word
		head = head->next;
		findNodeForWord(head, word);
	}else if (strcmp(head->next->word, word) > 0){
		Node *linkNode = head->next;
		Node *insertedNode = makeNode(head, word);
		insertedNode->next = linkNode;
		return insertedNode;
	}

}

/*
*   addWord handles the adding and incrementing of a word
*
*   head: a pointer to a Node
*   word: a pointer to a string
*/
static void addWord(Node *head, char *word){
	Node *nodeForWord = findNodeForWord(head, word);
	nodeForWord->count++;
}


int main (void){
  char c;
  int charCount = 0;
  char buffer[100];
  //initialize the list with appropriate values
  Node *list = malloc(sizeof(Node));
  list->next = NULL;
  list->count = 0;

  // read words and add them to the list
  while ( (c = getchar()) != EOF ) {
    if (isalpha(c))
      buffer[charCount++] = tolower(c);
    else {
      buffer[charCount++] = '\0';
      addWord(list, buffer);
      charCount = 0;
    }
  }

  /*
  * print the list, skip the two nodes as the first is null and the second
  * contains "" which is allowed by isalpha()
  */
  printList(list->next->next); 
  return 0;
}
