#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "chat1002.h"

/*
Function to create the hash table with the specified TABLE_SIZE.
Returns a pointer to the hash_table created.
*/
hash_table *create_hash_table(){
	//create pointer to table for reference
	hash_table *hashtable = malloc(sizeof(hash_table) * 1);
	//create null chat_entries in hash_table 
	hashtable->chat_entries = malloc(sizeof(chat_entry) * TABLE_SIZE);
	//hashtable->chat_entries = calloc(TABLE_SIZE,sizeof(chat_entry));

	return hashtable;

}
/*
Hash function using djb2 algorithm
returns unsigned long
*/
unsigned long hash(const char * str){
    
    unsigned long magic_number = 5381;
    int i;
    for(i = 0; i < strlen(str); i++){
        int c = tolower(str[i]);
        magic_number = (magic_number<<5) + magic_number + c;
    }
    
    return magic_number % TABLE_SIZE;
}

/*
This function performs insertion into a specified hashtable.
It returns 0 on success.
*/
int insert_into_hash_table(hash_table *hashTable, chat_entry *chatEntry) {
	//generate hash for array position
	unsigned long position = hash(chatEntry->key);
	hashTable->chat_entries[position] = chatEntry;
	if (hashTable->chat_entries[position]->key != NULL){
		return 0;
	}
	else {
		return 1;
	}
}

/*
Useful debugging function to print everything stored in the specified hashtable.
*/
void ht_dump(hash_table *hashtable) {
  for (int i = 0; i < TABLE_SIZE; ++i) {
    chat_entry *entry = hashtable->chat_entries[i];
		if (entry !=NULL) {
			
      printf("%s ", entry->intent);
			
      printf("%s\t", entry->entity);
			
      printf("%s\n", entry->response);
		}
  }
}

/*
helper function to clear hashtable.
This is a void function.
*/
void clearHashTable(hash_table *clearHashTable){
  free(clearHashTable);
}

/*
Takes in hash table, intent and entity.
Performs a dynamic lookup on the hash_table.
Returns a pointer to the retrieved chat_entry.
*/
chat_entry *retrieve_chat_entry(hash_table *hashTable, const char *intent, const char *entity)
{
	char *key = (char *) malloc (strlen(intent) + strlen(entity) + 1);
	strcpy(key, intent);
	strcat(key, entity);
	chat_entry *chatEntry = hashTable->chat_entries[hash(key)];
	free(key);
	return chatEntry;
}

/*
Helper function that creates a chat entry based on the intent, entity and response.
Returns a pointer to the created chat_entry.
To be used in conjunction with insert_into_hash_table.
*/
chat_entry *create_chat_entry(const char* intent,const char* entity, const char* response) {
	chat_entry *chatEntry = malloc(sizeof(chat_entry));
	int sizeofKey = strlen(intent) + strlen(entity) + 1;
	char *key = (char*) malloc (sizeofKey);
	strcat(key, intent);
	strcat(key, entity);
	chatEntry->key = (char *) malloc(sizeofKey);
	strcpy(chatEntry->key, key);
	
	chatEntry->intent =  (char *) malloc (strlen(intent) +1);
	strcpy(chatEntry->intent,intent);
	
	chatEntry->entity = (char *) malloc (strlen(entity) +1);
	strcpy(chatEntry->entity, entity);
	
	chatEntry->response = (char *) malloc (strlen(response) +1);
	strcpy(chatEntry->response, response);

	return chatEntry;

}
