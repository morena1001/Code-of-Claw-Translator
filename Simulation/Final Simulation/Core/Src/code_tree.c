/*
 * code_tree.c
 *
 *  Created on: Apr 16, 2026
 *      Author: josue
 */

#include "stdint.h"
#include "code_tree.h"

char* input_string;
int input_string_length;
trie_node* root;

trie_node* Tree_Init () {
	input_string = malloc (sizeof (char) * BASE_STRING_LENGTH);
	input_string_length = 0;

	return Trie_Init ();
}

void Add_Letter (char letter) {
	*(input_string + input_string_length++) = letter;
}

char Traverse_Tree (trie_node** current_node, char character) {
	// Assuming that character is '\\' by default
	uint8_t child = 0;

	if (character == '|')	child = 1;
	else if (character == '/')	child = 2;

	if (!(*current_node)->children[child]) {
		char letter = (*current_node)->data;
		Add_Letter ((*current_node)->data);
		*current_node = root;
		return letter;
	} else {
		*current_node = (*current_node)->children[child];
		return (*current_node)->data;
	}
}



trie_node* Make_Trie_Node (char data) {
    trie_node* node = (trie_node*) calloc (1, sizeof (trie_node));
    for (int i = 0; i < 3; i++)     node->children[i] = NULL;
    node->data = data;

    return node;
}



void Free_Trie_Node (trie_node* node) {
    for (int i = 0; i < 3; i++) {
        if (node->children[i])  Free_Trie_Node (node->children[i]);
        else    continue;
    }

    free (node);
}



trie_node* Trie_Init (void) {
    // Base layer : no input
	root = Make_Trie_Node ('-');

    trie_node* temp = root;

    // First layer : single input
    temp->children[0] = Make_Trie_Node ('e');
    temp->children[1] = Make_Trie_Node ('a');
    temp->children[2] = Make_Trie_Node ('i');

    // Second layer, click first input : two inputs
    temp = root->children[0];
    temp->children[0] = Make_Trie_Node ('p');
    temp->children[1] = Make_Trie_Node ('q');
    temp->children[2] = Make_Trie_Node ('r');

    // Second layer, tap first input : two inputs
    temp = root->children[2];
    temp->children[0] = Make_Trie_Node ('s');
    temp->children[1] = Make_Trie_Node ('t');
    temp->children[2] = Make_Trie_Node ('u');

    // Second layer, scratch first input: two inputs
    temp = root->children[1];
    temp->children[0] = Make_Trie_Node ('b');
    temp->children[1] = Make_Trie_Node ('c');
    temp->children[2] = Make_Trie_Node ('d');

    // Third layer, scratch first input, click second input : three inputs
    trie_node* scratch_temp = temp;
    temp = scratch_temp->children[0];
    temp->children[0] = Make_Trie_Node ('j');
    temp->children[1] = Make_Trie_Node ('k');
    temp->children[2] = Make_Trie_Node ('l');

    // Third layer, scratch first input, tap second input : three inputs
    temp = scratch_temp->children[2];
    temp->children[0] = Make_Trie_Node ('m');
    temp->children[1] = Make_Trie_Node ('n');
    temp->children[2] = Make_Trie_Node ('o');

    // Third layer, scratch first and second input : three inputs
    temp = scratch_temp->children[1];
    temp->children[0] = Make_Trie_Node ('f');
    temp->children[1] = Make_Trie_Node ('g');
    temp->children[2] = Make_Trie_Node ('h');

    // Fourth layer, scratch first and second input, click third input : four inputs
    temp->children[0]->children[0] = Make_Trie_Node ('v');

    // Fourth layer, scratch first and second input, tap third input : four inputs
    temp->children[2]->children[2] = Make_Trie_Node ('w');

    // Fourth layer, scratch fist, second and third inputs : four inputs
    temp = temp->children[1];
    temp->children[0] = Make_Trie_Node ('x');
    temp->children[1] = Make_Trie_Node ('z');
    temp->children[2] = Make_Trie_Node ('y');

    return root;

}
