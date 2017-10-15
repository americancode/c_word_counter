/*
 ============================================================================
 Name        : wcExtraCredit1.c
 Author      : Nathaniel Churchill
 Professor   : Dr. David Smith
 Description : Program to read a text file and print how many times
 	 	 	 	all the words occur in the given file.  Has the ability to process
 	 	 	 	command line arguments as well.
 ============================================================================
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <getopt.h>

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
Node *makeNode ( Node *head, char *word ) {
	Node *current = NULL;
	current = malloc(sizeof(Node));
	current->word = malloc(strlen(word) + 1);
	strcpy(current->word, word);
	current->next = NULL;
	current->count = 0; // initialize the count to be null
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
 *   This function accepts a pointer to a node and recursively prints the nodes
 *   their counts to a file
 *
 *	 outpu: a pointer to a file
 *   head: a pointer to a node
 */
static void printListFile (Node *head, FILE *output){
	if (head != NULL){
		fprintf (output, "%-10s   %d\n", head->word,  head->count);
		head = head->next;
		printListFile(head, output);
	}else {
		fprintf(output, "List has ended\n");
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


int main ( int argc, char **argv) {
	char c;
	int i, j = 0;
	char buffer[100];
	//initialize the list with appropriate values
	Node *list = malloc(sizeof(Node));
	list->next = NULL;
	list->count = 0;

	int fileOutput = 0;
	int fileInput = 0;

	char *fileName = NULL;
	FILE *src = NULL;
	FILE *output = NULL;

	int opt;
	//get the command line options
	while ((opt = getopt (argc, argv, "o:")) != -1){
		switch (opt){
		case 'o':
			fileName = optarg;
			fileOutput = 1;
			if (argv[3] != NULL) {
				fileInput = 1;
				src = fopen(argv[3], "r");
			}
			output = fopen (fileName, "w" );
			break;

		}
	}

	//check what parameter combination we have
	if ((argv[1] != NULL) && (src == NULL)) {
		fileInput = 1;
		src = fopen(argv[1], "r");
	}

	if (fileInput == 1) {
		for (i = 0; (c = fgetc(src)) != EOF; ++i) {
			if (isalpha(c))
				buffer[j++] = tolower(c);
			else {
				buffer[j++] = '\0';
				addWord(list, buffer);
				j = 0;
			}
		}
	} else {
		while ((c = getchar()) != EOF) {
			if (isalpha(c))
				buffer[j++] = tolower(c);
			else {
				buffer[j++] = '\0';
				addWord(list, buffer);
				j = 0;
			}
		}
	}




	/*
	 * print the list, skip the two nodes as the first is null and the second
	 * contains "" which is allowed by isalpha()
	 */
	if(fileOutput == 1) {
		printListFile(list->next->next, output);
	}else {
		printList(list->next->next);
	}
	fclose (src); // close the file
	fclose (output);
	return 0;
}
