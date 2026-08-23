/*
 * Command processor
 */

#include "commands.h"

extern Profile profile;

#define ENABLE_PROFILES

// definition of the callback
void example_callback(String s){
    //Serial.println("this is the callback function");
    Serial.println("[" + s + "]");
}

// function to test calling the callback
void test_calling_the_callback_function(){
    void (*callbackfn)(String) = NULL;
    int test_variant = 3;

    Serial.println("Running variant " + (String)test_variant);
    switch(test_variant){
        case 1:{
            // 1======
            // set the global callbackfn directly
            callbackfn = example_callback;
            break;
        }
        case 2:{
            // 2======
            // now build a "Command" object with pointer to the callback in the Command object
            // and then use this to call the callback...

            Command * command = new Command;
            command->key = "test2";
            command->handler = example_callback;

            // now set the local variable to the handler, which we'll call later...
            callbackfn = command->handler;

            break;
        }
        case 3:{
            // 3======
            // now build a short linked-list of Command objects
            // and then use the linked list to call the callback...

            String candidate = "test-variant-3";

            CommandList* commandList = new CommandList();
            commandList->add("some", NULL);
            commandList->add("another", NULL);
            commandList->add(candidate, example_callback);
            commandList->add("more", NULL);
            commandList->add("yetmore", NULL);

            // now get the matching command object
            Command * matched_command;
            matched_command = commandList->getCommand(candidate);

            if(matched_command != NULL){
                // A match was made - unpack and set the callback function
                callbackfn = matched_command->handler;

                Serial.println("Just to check... ");
                Serial.println("  Returned object has key = " + matched_command->key);
            }else{
                Serial.print("No match found for " + candidate);
            }
            
        }

    }

    String payload = "this is a string";

    if (callbackfn != NULL){
        Serial.println("About to test calling the callback function");
        callbackfn(payload);
        Serial.println("Returned from the callback function");
    }else{
        Serial.println("Callback variable is NULL -- no callback function to call");
    }
}


/* 
 *  Simple unit-test functions
 */
void test_linked_list_handler_function(String s){
    Serial.println("Within the test_linked_list_handler_function");
}

void test_linked_list_getCommand(CommandList* commandList, String candidate){
    Command* a;
    a = commandList->getCommand(candidate);
    if(a != NULL){
        Serial.println("Matched " + a->key + " with " + candidate);
    }else{
        Serial.println("No match for " + candidate);
    }
}

void test_linked_list(){

    Serial.println("Linked list test:");
    CommandList* commandList = new CommandList();

    commandList->add("some", NULL);
    commandList->add("another", NULL);
    commandList->add("more", NULL);
    //commandList->add("test", test_linked_list_handler_function());

    Serial.println("1. Simple test to output a list");
    commandList->list();

    Serial.println("");
    Serial.println("2. Testing matching candidate text against the list (getHandler)");
    commandList->getHandler("some"); // should pass: is in the list (first)
    commandList->getHandler("another"); // should pass: is in the list (not first/last)
    commandList->getHandler("more"); // should pass: is in the list (last)
    commandList->getHandler("som"); // should fail: candidate is subset of a list member
    commandList->getHandler("something"); // should fail: candidate is as superset of a list member
    
    Serial.println("Test completed");

    Serial.println("3. Testing matching candidate text against the list (getCommand)");
    
    test_linked_list_getCommand(commandList,"this");      // should fail: not in the list
    test_linked_list_getCommand(commandList,"some");;     // should pass: is in the list (first)
    test_linked_list_getCommand(commandList,"another");   // should pass: is in the list (not first/last)
    test_linked_list_getCommand(commandList,"more");      // should pass: is in the list (last)
    test_linked_list_getCommand(commandList,"som");       // should fail: candidate is subset of a list member
    test_linked_list_getCommand(commandList,"something"); // should fail: candidate is as superset of a list member

    Serial.println("4. Call the callback for 'test'");

    delay(10000);
}

/*
 * Display list of available commands
 */
void cbListCommands(String params){

    Serial.println("Available commands:");
    commands->list();
}

/*
 *  Command object
 */

/*
Command:Command(String key, void (*handler)(String)){
    this->key = key;
    this->handler = handler;
}
*/

CommandList* commands;

/*
 * Helper functions
 */

// return number of parameters in the payload
int commandGetPayloadCount(String payload){
    char delimiter = ' ';
    const char * str = payload.c_str();

    int i = 0;
    char *pch=strchr(str, delimiter);
    while(pch!=NULL) {
        i++;
        pch=strchr(pch+1, delimiter);
    }
    //Serial.println("Number of instances = " + String(i));
    return(i+1);
}



void setup_commands(){
    commands = new CommandList();

    commands->add("test",example_callback);
    commands->add("list",cbListCommands);
}

/*
 * CommandList - LinkedList of Commands
 */

CommandList::CommandList(){
    this->length = 0;
    this->head = NULL;
}

String CommandList::toCase(String originalString){
    if(!this->caseSensitive){
        originalString.toLowerCase();
    }
    return(originalString);
}

String CommandList::keyFromString(String candidate){
    String outputString;

    // Look for valid delimiter within candidate
    int index = candidate.indexOf(" ");
    if(index==-1){
        // no space at all, entire candidate should be matched
        outputString = candidate;
    }else{
        // found a space, remove it
        outputString = candidate.substring(0, index);
    }
    return outputString;
}

String CommandList::payloadFromString(String candidate){
    String outputString;

    // Look for valid delimiter within candidate
    int index = candidate.indexOf(" ");
    if(index==-1){
        // no space at all, empty string to be returned
        outputString = "";
    }else{
        // found a space, remove it
        outputString = candidate.substring(index+1);
    }
    return outputString;
}

/*
String CommandList::payloadFromString(String candidate){
    String payload = this->payloadFromString(candidate);
    return outputString;
}
*/

boolean CommandList::add(String key, void(*handler)(String)){
    // Create a new node
    //Serial.print("Adding " + key + "...");
    Command* item = new Command();
    //Serial.print("created, ");
    item->key = this->toCase(key);
    item->handler = handler;
    item->next = this->head;
    this->head = item;
    this->length++;
    return(true);
}

/* add - but only if the this device's profile name matches the given string 

   This allows comnands to be applied per profile with minimal overhead required
   in each individual module - only a correct matching case-sensetive string.
*/
boolean CommandList::add(String key, void(* handler)(String),const char* only_if_profile){

#ifdef ENABLE_PROFILES
    if (profile.profileNameIs(only_if_profile)){
        return this->add(key,handler);
    }else{
        return false;
    }
#else
    return this->add(key,handler);
#endif
}

void CommandList::list(){
    
    Command* item = this->head;

    while(item!=NULL){
        Serial.println(item->key);
        item = item->next; 
    }
}


/*
 *  getCommand 
 *  - returns the command object based in <candidate> string
 */
Command* CommandList::getCommand(String candidate){
    String candidate_c = this->keyFromString(this->toCase(candidate));

    Command* item = this->head;
    Command* matched = NULL;

    while(item!=NULL){
        if(candidate_c == item->key){
            matched = item;
            break;
        }
        else{
        }
        item = item->next; 
    }
    return matched;
}

/*
 *  getHandler
 *  - returns the pointer to a <candidate> function
 */
void (* CommandList::getHandler(String candidate))(String){
    void (*ptr)(String) = NULL;
    Command *item = getCommand(candidate);
    if(item != NULL){
        ptr = item->handler;
    }
    return(ptr);
}

/*
 *  runCommand
 *  - executes the command specified in <candidate> string
 */
boolean CommandList::runCommand(String candidate){
    boolean success;
    unsigned long timeFinish, timeStart, timeTaken;
    
    Command *item = getCommand(candidate);

    // exit signalling, an invalid path, if no match
    if(item==NULL){
        return(false);
    }

    void(*ptr)(String) = item->handler;
 
    String key = item->key;

    String payload = "";

    if(ptr == NULL){
        success = false;
    }else{
        payload = this->payloadFromString(candidate);
        Serial.println("MQTT -> " + key + " [" + payload + "]");
        timeStart = millis();
        ptr(payload);
        timeFinish = millis();
        timeTaken = timeFinish - timeStart;
        Serial.println("MQTT -> " + key + " executed in " + String(timeTaken) + " ms");
        success = true;
    }

    return(success);
    
}   