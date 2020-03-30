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
#include "hashDemo.c"
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
	check_for_knowledge_base();

	/* to be implemented */
	// Check if intent is valid
	if(chatbot_is_question(intent) !=1){
		return KB_INVALID;
	}
	
	// if file can't be opened
	if((File = fopen("ICT1002_Group_Project_Assignment_AY19_T2_Sample.ini" , "r")) == NULL){
		return KB_NOTFOUND;
	}

	// Concatenate Intent and entity
	char * token = strtok(entity, "");
	char finalEntity[1000] = "";

	// Check for useless words
	while(token != NULL){
		// If got useless words, replace with ""
		// Add Space to it then concatenate to finalEntity
		if(strcmp(token,"is") == 0 || strcmp(token,"are") == 0 || strcmp(token,"was") == 0 || strcmp(token,"were") == 0){
			token = "";
			strcat(token, " ");
			strcat(finalEntity, token);
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
	// Gets the assoc response
	chat_entry result = retrieve_chat_entry(knowledge_base, intent, finalEntity);

	// Check if response found
	// if yes, copy to response buffer and return KB_OK
	if(result.response != NULL){
		strcpy(response, result.response);
		return KB_OK;
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
int knowledge_read(FILE * f){
    char *line = NULL;
    size_t sz = 0;
    ssize_t len;
    int entitycount = 0;
    char entity[64];
    char intent[32];
    char response[256];
    char *strsplit;
    char *backstr;

    while(len = getline(&line,&sz,f) >= 0) {
        
        if (strstr(line,"what")){
            strcpy(intent,"WHAT");
            
        }
        else if(strstr(line,"where")){
            strcpy(intent,"WHERE");
            
        }

        else if(strstr(line,"who")){
            strcpy(intent,"WHO");
           
        }

        if(strstr(line,"=")){
            
            strsplit = strtok(line,"=");
            strcpy(entity,strsplit);
            backstr = strtok(NULL,"=");
            backstr =strtok(backstr,"\n");
            strcpy(response,backstr);
            //printf("%s\n%s\n%s\n",intent,entity,response);
            knowledge_put(intent,entity,response); <- commented so my program doesnt break for now.
            entitycount += 1; // inside here because count is added only when entity is found.
        }
    }
    free(line);

    return entitycount;
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

