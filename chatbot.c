/*
 * ICT1002 (C Language) Group Project.
 *
 * This file implements the behaviour of the chatbot. The main entry point to
 * this module is the chatbot_main() function, which identifies the intent
 * using the chatbot_is_*() functions then invokes the matching chatbot_do_*()
 * function to carry out the intent.
 *
 * chatbot_main() and chatbot_do_*() have the same method signature, which
 * works as described here.
 *
 * Input parameters:
 *   inc      - the number of words in the question
 *   inv      - an array of pointers to each word in the question
 *   response - a buffer to receive the response
 *   n        - the size of the response buffer
 *
 * The first word indicates the intent. If the intent is not recognised, the
 * chatbot should respond with "I do not understand [intent]." or similar, and
 * ignore the rest of the input.
 *
 * If the second word may be a part of speech that makes sense for the intent.
 *    - for WHAT, WHERE and WHO, it may be "is" or "are".
 *    - for SAVE, it may be "as" or "to".
 *    - for LOAD, it may be "from".
 * The word is otherwise ignored and may be omitted.
 *
 * The remainder of the input (including the second word, if it is not one of the
 * above) is the entity.
 *
 * The chatbot's answer should be stored in the output buffer, and be no longer
 * than n characters long (you can use snprintf() to do this). The contents of
 * this buffer will be printed by the main loop.
 *
 * The behaviour of the other functions is described individually in a comment
 * immediately before the function declaration.
 *
 * You can rename the chatbot and the user by changing chatbot_botname() and
 * chatbot_username(), respectively. The main loop will print the strings
 * returned by these functions at the start of each line.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "chat1002.h"



/*
 * Get the name of the chatbot.
 *
 * Returns: the name of the chatbot as a null-terminated string
 */
const char *chatbot_botname() {

	return "Harwinder";

}


/*
 * Get the name of the user.
 *
 * Returns: the name of the user as a null-terminated string
 */
const char *chatbot_username() {

	return "User";

}


/*
 * Get a response to user input.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop (i.e. it detected the EXIT intent)
 */
int chatbot_main(int inc, char *inv[], char *response, int n) {
	/* check for empty input */
	if (inc < 1) {
		snprintf(response, n, "");
		return 0;
	}

	/* look for an intent and invoke the corresponding do_* function */
	if (chatbot_is_exit(inv[0]))
		return chatbot_do_exit(inc, inv, response, n);
	else if (chatbot_is_smalltalk(inv[0]))
		return chatbot_do_smalltalk(inc, inv, response, n);
	else if (chatbot_is_load(inv[0]))
		return chatbot_do_load(inc, inv, response, n);
	else if (chatbot_is_question(inv[0]))
		return chatbot_do_question(inc, inv, response, n);
	else if (chatbot_is_reset(inv[0]))
		return chatbot_do_reset(inc, inv, response, n);
	else if (chatbot_is_save(inv[0]))
		return chatbot_do_save(inc, inv, response, n);
	else {
		snprintf(response, n, "I don't understand \"%s\".", inv[0]);
		return 0;
	}

}


/*
 * Determine whether an intent is EXIT.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "exit" or "quit"
 *  0, otherwise
 */
int chatbot_is_exit(const char *intent) {

	return compare_token(intent, "exit") == 0 || compare_token(intent, "quit") == 0;

}


/*
 * Perform the EXIT intent.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_exit(int inc, char *inv[], char *response, int n) {

	snprintf(response, n, "Goodbye!");

	return 1;

}


/*
 * Determine whether an intent is LOAD.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "load"
 *  0, otherwise
 */
int chatbot_is_load(const char *intent) {
    if (strcmp(intent, "load") == 0) {
        return 1;
    }
    return 0;
}



/*
 * Load a chatbot's knowledge base from a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after loading knowledge)
 */
int chatbot_do_load(int inc, char *inv[], char *response, int n) {

	/* to be implemented */
	int count = 0;
	
	if(inv[1]== NULL){
		strcpy(response,"No file input");
	}
	else{
		FILE * f;
		f = fopen(inv[1],"r");
		if(f==NULL){
			strcpy(response,"error");
		}
		else {
			
			count = knowledge_read(f);
			// snprintf params (buffer , buffer size,format similar to printf, filename <- i think so)
			snprintf(response, n," Read %d responses in %s", count, inv[1]);
			fclose(f);

		}
	}

	return 0;

}


/*
 * Determine whether an intent is a question.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_question(const char *intent) {

return compare_token(intent, "what") == 0 || compare_token(intent, "where") == 0 || compare_token(intent, "who") == 0;
}


/*
 * Answer a question.
 *
 * inv[0] contains the the question word.
 * inv[1] may contain "is" or "are"; if so, it is skipped.
 * The remainder of the words form the entity.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_question(int inc, char *inv[], char *response, int n) {

char *intent = inv[0];
char entity[MAX_ENTITY];
char removed[MAX_ENTITY];
char * ignorelist[] = {"is","are"};

if(knowledge_get(intent, entity, response, n) == KB_NOTFOUND){
  char ans [MAX_RESPONSE];
  prompt_user(ans, n, "I don't know. %s%s%s?",intent,removed,entity);
  char *newans = trim(ans);
  if (strlen(newans) !=0){
    knowledge_put(intent, entity, newans);
    snprintf(response, n, "Thank you for your input.");
  }
  else{
    snprintf(response,n,"Something went wrong");
  }
}
 return 0;
}


/*
 * Determine whether an intent is RESET.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "reset"
 *  0, otherwise
 */
 // Author: Clement Leow
 // Date: 28 Mar 2020
int chatbot_is_reset(const char *intent) {
	char *temp = strdup(intent);
	unsigned char *tptr = (unsigned char *)temp;
	while(*tptr) {
    *tptr = tolower(*tptr);
    tptr++;
	}
	if(strcmp(temp, "reset") == 0){
		free(temp);
		return 1;
	}
	free(temp);
	return 0;

}


/*
 * Reset the chatbot.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after beign reset)
 */
int chatbot_do_reset(int inc, char *inv[], char *response, int n) {

	knowledge_reset();
	snprintf(response, n,"%s resetted", chatbot_botname());

	return 0;

}


/*
 * Determine whether an intent is SAVE.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_save(const char *intent) {
if (compare_token(intent,"save")){       // Check if Intent is to SAVE
        return 0;                           // Return 0 if Intent is to SAVE
    }
    else {
        return 1;                      
    }
	

	

}


/*
 * Save the chatbot's knowledge to a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after saving knowledge)
 */
int chatbot_do_save(int inc,char *inv[], char *response,int n){
   if(inv[1]== NULL){
       strcpy(response,"no file specified");
   }
   else{
       FILE *file;
       file = fopen(inv[1],"w");
       if(file == NULL){
           strcpy(response,"Error");
       }
       else{

           knowledge_write(file);
           if(ferror(file)){
               snprintf(response,n,"failed to save to %s %s",inv[1]);
           }
           else{
               snprintf(response,n,"saved to %s",file);
           }
       }
       fclose(file);
   }
   return 0;
}


/*
 * Determine which an intent is smalltalk.
 *
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is the first word of one of the smalltalk phrases
 *  0, otherwise
 */
int chatbot_is_smalltalk(const char *intent) {

//Defining small talk items, including end phrases
char *smalltalkitems[] = {"Hi","Sup", "Hello","Yo","Greetings","goodday","talk","bye","goodbye", "quit","it's","it","its"};

//Calculating the total size of the length of the defined array
	size_t smalltalkLength = sizeof(smalltalkitems) / sizeof(smalltalkitems[0]);

//Using a for loop to loop throught the small talk array to check wehther intent matches any small talk phrases.
	for(int i = 0; i< smalltalkLength; i++)
	{
		if(compare_token(intent,smalltalkitems[i]) == 0)
		{
			return 1;
		}
	}


	return 0;

}


/*
 * Respond to smalltalk.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting, taking the to 
 *   1, if the chatbot should stop chatting (e.g. the smalltalk was "goodbye" etc.)
 */
int chatbot_do_smalltalk(int inc, char *inv[], char *response, int n) {

	char *smalltalkOutput[] = {"Hi", "good morning/afternoon/evening" ,"What's up?",  "Hi!", "Sup!", "How are you!", "Ni Hao!", "Yo!", "LOL!", "Greetings!","That's great"};

  //Calculating the total length of the defined small talk output array
	size_t smalltalkLength = sizeof(smalltalkOutput) / sizeof(smalltalkOutput[0]);

//Checking to see the user input any of the end phrases
if (compare_token("bye",inv[0])==0 || compare_token("goodbye",inv[0])==0 || compare_token("quit",inv[0])==0){
            snprintf(response,n,"bye!");
            return 1;
}
//Agreeing with the user
else if(compare_token("it",inv[0])==0 ||compare_token("it's",inv[0])==0 ||compare_token("its",inv[0])==0 ){
          snprintf(response,n,"Indeed it is.");
          return 0;
}

//If not any of the end phrases, return the normal small talk output
else{
  int randomOutput = rand() % smalltalkLength;
	snprintf(response, n, smalltalkOutput[randomOutput]);
  return 0;
}
}

/*
Remove trailing andleading whitespaces from a given string
*/
char *trim(char *inputStr)
{
    char *endStr;

    // Trim leading space
    while(isspace((unsigned char)*inputStr)){
        inputStr++;
    }

    // Trim trailing space
    endStr = inputStr + strlen(inputStr) - 1;
    while(endStr > inputStr && isspace((unsigned char)*endStr)) {
        endStr--;
    }

    // Write new null terminator character
    endStr[1] = '\0';

    return inputStr;
}
