/*
 * ICT1002 (C Language) Group Project.
 *
 * This file implements the chatbot's knowledge base.
 *
 * knowledge_get() retrieves the response to a question.
 * knowledge_put() inserts a new response to a question.
 * knowledge_read() reads the knowledge base from a file.
 * knowledge_reset() erases all of the knowledge.
 * knowledge_write() saves the knowledge base in a file.
 *
 * You may add helper functions as necessary.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "chat1002.h"
#include "hashtable.c"
#include <stdbool.h>


hash_table *knowledge_base = NULL;
/*
 * Get the response to a question.
 *
 * Input:
 *   intent   - the question word
 *   entity   - the entity
 *   response - a buffer to receive the response
 *   n        - the maximum number of characters to write to the response buffer
 *
 * Returns:
 *   KB_OK, if a response was found for the intent and entity (the response is copied to the response buffer)
 *   KB_NOTFOUND, if no response could be found
 *   KB_INVALID, if 'intent' is not a recognised question word
 */
int knowledge_get(const char *intent, const char *entity, char *response, int n) {

	FILE *File = NULL;

	/* to be implemented */
	// Check if intent is valid
	if(chatbot_is_question(intent) !=1){
		return KB_INVALID;
	}
	
	// if file can't be opened
	if((File = fopen("ICT1002_Group_Project_Assignment_AY19_T2_Sample.ini" , "r")) == NULL){
		return KB_NOTFOUND;
	}

	// initialise response buffer
	unsigned long ulBufferLen = 1;			// Initial Buffer Length
	response = NULL;

	if(strcmp(intent, "who") == 0){
		// iterate through ini file for 'who' answers
		char *question = "";
		if(strcmp(entity, question)){
			// copy associated response to response buffer;
			response = (char*)malloc(ulBufferLen*sizeof(char));
			
			return KB_OK;
		}
	}
	
	if(strcmp(intent, "what") == 0){
		// iterate through ini file for 'what' answers
		char *question = "";
		if(strcmp(entity, question)){
			// copy associated response to response buffer;
			return KB_OK;
		}
	}
	
	if(strcmp(intent, "where") == 0){
		// iterate through ini file for 'where' answers
		char *question = "";
		if(strcmp(entity, question)){
			// copy associated response to response buffer;
			return KB_OK;
		}
	}

	return KB_NOTFOUND;

}


/*
 * Insert a new response to a question. If a response already exists for the
 * given intent and entity, it will be overwritten. Otherwise, it will be added
 * to the knowledge base.
 *
 * Input:
 *   intent    - the question word
 *   entity    - the entity
 *   response  - the response for this question and entity
 *
 * Returns:
 *   KB_FOUND, if successful
 *   KB_NOMEM, if there was a memory allocation failure
 *   KB_INVALID, if the intent is not a valid question word
 */
int knowledge_put(const char *intent, const char *entity, const char *response) {
	check_for_knowledge_base();
	chat_entry *chatEntry = create_chat_entry(intent, entity, response);
	insert_into_hash_table(knowledge_base, chatEntry);
	if (chatEntry == NULL)
	{
		return KB_NOMEM;
	}
	else {
		return KB_OK;
	}

}


/*
 * Read a knowledge base from a file.
 *
 * Input:
 *   f - the file
 *
 * Returns: the number of entity/response pairs successful read from the file
 */
int knowledge_read(FILE *f) {

	/* to be implemented */

	return 0;
}


/*
 * Reset the knowledge base, removing all know entitities from all intents.
 */
 
void knowledge_reset() {
  check_for_knowledge_base();               //check if knowledge base exists
  hash_table *clearHT = knowledge_base;     //create new pointer to point to current knowledge base
  knowledge_base = create_hash_table();     //move the global pointer to a new knowledge base
  clearHashTable(clearHT);                  //clear the current knowledge base

}


/*
 * Write the knowledge base to a file.
 *
 * Input:
 *   f - the file
 */
void knowledge_write(FILE *f) {

	/* to be implemented */

}

/*
* This function checks if the knowledge_base is NULL
* If it is, it runs the create_hash_table() function
* which should populate it with chat_entry structs.
* This is a void function.
*/


void check_for_knowledge_base() {
	if (knowledge_base == NULL) {
		knowledge_base = create_hash_table();
	}
}

