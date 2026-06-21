/*
 * code_tree.h
 *
 *  Created on: Apr 16, 2026
 *      Author: josue
 */

#ifndef INC_CODE_TREE_H_
#define INC_CODE_TREE_H_

// Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Defines
#define LETTER_START_COUNTER 	251
#define LETTER_END_COUNTER		1
#define BASE_STRING_LENGTH 		182

// Private Variables
typedef struct Trie_Node trie_node;
extern char* input_string;
extern int input_string_length;
extern trie_node* root;

struct Trie_Node {
    char data;
    trie_node* children[3];
};

// Function Prototypes
trie_node* Tree_Init ();
void Add_Letter (char letter);
char Traverse_Tree (trie_node** current_node, char character);

trie_node* Make_Trie_Node (char data);
void Free_Trie_Node (trie_node* node);
trie_node* Trie_Init (void);

#endif /* INC_CODE_TREE_H_ */
