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
#include <stdbool.h>


void check_for_knowledge_base();

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
	// Check if knowledgebase exists
	check_for_knowledge_base();

	// Resets the response buffer to empty
	// printf("Clearing response buffer\n");
	// memset(response,0,0);
	// printf("Cleared response buffer\n");

	// Check if intent is valid
	if(chatbot_is_question(intent) == 1){
		// Gets the assoc response
		
		if ( retrieve_chat_entry(knowledge_base, intent, entity) == NULL) {
			snprintf(response, n, "I don't recognise %s.", intent);
			return KB_NOTFOUND;
		}
		else {
			chat_entry *chatEntry= retrieve_chat_entry(knowledge_base, intent, entity);
			snprintf(response, n, "%s", chatEntry->response);
		}
	}
	else{
		snprintf(response, n, "I don't recognise %s.", intent);
		return KB_INVALID;
	}
	// Check if response found
	// if yes, return KB_OK
	if(response != NULL){
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
	if (insert_into_hash_table(knowledge_base, chatEntry) == 0) {
		return KB_OK;
	}
	else {
		return KB_NOMEM;
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
    int entitycount = 0;
    char entity[64];
    char intent[32];
    char response[256];
    char *strsplit;
    char *backstr;
    while(getline(&line,&sz,f) != -1) {
        
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
            knowledge_put(intent,entity,response);
            entitycount += 1; // inside here because count is added only when entity is found.
        }
		
    }
    if (line) {
		free(line);
	}
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

    // Checking for knowledge base   
    check_for_knowledge_base();
	chat_entry 	*whatList[1000];
	chat_entry 	*whoList[1000]; 
	chat_entry 	*whereList[1000];
	int 	whatC = 0;
	int		whoC = 0;
	int 	whereC = 0;

	// ht_dump(knowledge_base);
	// Loop through whole table
    for(int i=0; i<1000; i++){
		
        chat_entry * entry;
		
		entry = malloc(sizeof(chat_entry));
		
		entry = knowledge_base -> chat_entries[i];
		
		
        if(entry != NULL){
			if(strcmp(entry->intent, "WHAT") == 0 || strcmp(entry->intent, "What") == 0 || strcmp(entry->intent, "what") == 0){
				whatList[whatC] = entry;
				whatC++;
			}
			else if(strcmp(entry->intent, "WHO") == 0 || strcmp(entry->intent, "Who") == 0 || strcmp(entry->intent, "who") == 0){
				whoList[whoC] = entry;
				whoC++;
			}
			else if(strcmp(entry->intent, "WHERE") == 0 || strcmp(entry->intent, "Where") == 0 || strcmp(entry->intent, "where") == 0){
				whereList[whereC] = entry;
				whereC++;
			}
		}
	}

	// printf("[WHAT]\n");
	
	// for(int i=0; i<whatC; i++){
	//  	printf("%s=%s\n", whatList[i]->entity, whatList[i]->response);
	// 	}
		
	// printf("\n- - - - - - - - - -\n[WHERE]\n");

	// for(int i=0; i<whereC; i++){
	//  	printf("%s=%s\n", whereList[i]->entity, whereList[i]->response);
	//  	}
	
	// printf("\n- - - - - - - - - -\n[WHO]\n");
	
	// for(int i=0; i<whoC; i++){
	// 	printf("%s=%s\n", whoList[i]->entity, whoList[i]->response);
	// 	 }


	/******************************************************************/


	// Write to file
	fprintf(f,"[what]\n");
	
	for(int i=0; i<whatC; i++){
	 	fprintf(f,"%s=%s\n", whatList[i]->entity, whatList[i]->response);
		}
		
	fprintf(f,"\n[where]\n");

	for(int i=0; i<whereC; i++){
	 	fprintf(f,"%s=%s\n", whereList[i]->entity, whereList[i]->response);
	 	}
	
	fprintf(f,"\n[who]\n");
	
	for(int i=0; i<whoC; i++){
		fprintf(f,"%s=%s\n", whoList[i]->entity, whoList[i]->response);
		}

	

	

	
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

void knowledgedump(){
	ht_dump(knowledge_base);
}
