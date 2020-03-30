#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TABLE_SIZE 1000

typedef struct chat_entry {
	/*
	note that a key in this context, merely refers to the concatenated stringsintent+entity 
	this would be psuedounique, as oneintent can have many entities,
	and one entity can have many intents 
	*/
	char *key;
	char intent;
	char *entity;
	char *response;
	struct chat_entry *next; //in case of collision
} chat_entry;

typedef struct {
	chat_entry **chat_entries;
} hash_table;

/*
Function to create the hash table with the specified TABLE_SIZE
*/
hash_table *create_hash_table(){
	//create pointer to table for reference
	hash_table *hashtable = malloc(sizeof(hash_table) * 1);

	//create null chat_entries in hash_table 
	hashtable->chat_entries = calloc(TABLE_SIZE,sizeof(chat_entry));

	return hashtable;

}

unsigned long hash(const char * str){
    //Hash function using djb2 algorithm
    unsigned long magic_number = 5381;
    int i;
    for(i = 0; i < strlen(str); i++){
        int c = tolower(str[i]);
        magic_number = (magic_number<<5) + magic_number + c;
    }
    
    return magic_number % TABLE_SIZE;
}

void insert_into_hash_table(hash_table *hashTable, chat_entry *chatEntry) {
	//generate hash for array position
	unsigned long position = hash(chatEntry->key);
	printf("Position = %lu\n", position);
	//get pointer to position in hashtable
	chat_entry *entry = hashTable->chat_entries[position];
	//if entry is null, means that no prev entry, insertion possible
	if (entry == NULL) {
		hashTable->chat_entries[position] = chatEntry;
	}

	//in the event theres a collision
	chat_entry *prev;
	while (entry != NULL) {
		//means that collision occurred
		//if keys are same, replace
		if (strcmp(entry->key, chatEntry->key) == 0) {
			free(entry->response);
			entry->response = calloc(strlen(chatEntry->response) +1, 1 );
			strcpy(entry->response, chatEntry->response);
			return;
		}
		//traverse entries
		prev = entry;
		entry = prev->next;
	}
	//collision between two distinctintent and entity found
	prev->next = chatEntry;
}

void ht_dump(hash_table *hashtable) {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        chat_entry *entry = hashtable->chat_entries[i];

        if (entry == NULL) {
            continue;
        }

        printf("slot[%4d]: ", i);

        for(;;) {
            printf("%s", entry->key);

            if (entry->next == NULL) {
                break;
            }

            entry = entry->next;
        }

        printf("\n");
    }
}

//Function to clear hashtable
void clearHashTable(hash_table *clearHashTable){
  free(clearHashTable);
}

chat_entry *create_chat_entry(char*intent,const char* entity, char* response) {
	chat_entry *chatEntry;
	chatEntry = malloc (sizeof(chat_entry));
	int sizeofKey = strlen(intent) + strlen(entity) + 1;
	char *key2 = (char*) malloc (sizeofKey);
	strcat(key2,intent);
	strcat(key2, entity);
	chatEntry->key = (char *) malloc(sizeofKey);
	strcpy(chatEntry->key, key2);
	
	chatEntry->intent = malloc (strlen(intent) +1);
	strcpy(chatEntry->intent,intent);
	
	chatEntry->entity = malloc (strlen(entity) +1);
	strcpy(chatEntry->entity, entity);
	
	chatEntry->response = malloc (strlen(response) +1);
	strcpy(chatEntry->response, response);
	
	chatEntry->next = NULL;

	return chatEntry;

}

// int main() {
// 	hash_table *hashTable = create_hash_table();
// 	chat_entry chatEntry1 = create_chat_entry("What", "SIT", "an institution");
	
// 	//insert into hashtable
// 	insert_into_hash_table(hashTable, &chatEntry1);
// 	ht_dump(hashTable);
// 	return 0;
// }


	
