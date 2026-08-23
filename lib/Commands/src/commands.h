/*
 * Command processor
 */

#ifndef COMMANDS_H_INCLUDED
#define COMMANDS_H_INCLUDED

#include <Arduino.h>
#include <WString.h>
#include <profile.h>

void setup_commands();
int commandGetPayloadCount(String);

void test_linked_list();
void test_calling_the_callback_function();

class Command{
    public:
        //Command(String key, void (*handler2)(String));
        Command* next;              /* pointer to next command in the list */
        String key;                 /* string (usually single word) that is the command itself */
        void (*handler)(String);   /* as above but is a pointer to a function that requires a single parameter */
};

/* Class to hold a linked list of Command items */
class CommandList{
    public:
        CommandList();
        boolean add(String key, void(* handler)(String));
        boolean add(String key, void(* handler)(String),const char* only_if_profile);
        //void * getHandler(String);    /* search list for a command based on candidate string, return handler or NULL */ 
        void (* getHandler(String))(String);
        Command * getCommand(String); /* search list for a command based on candidate, return pointer to matched Command or NULL */
        boolean runCommand(String);
        void list();
        //bool profileNameIs(char* profile_name){ return true; };

    private:              /* everything public right now */
        String toCase(String);/* return string in desired case or original */
        Command* head;      /* head of the list */
        int length;         /* number of items in the list */
        boolean caseSensitive = false; /* if false -- *not* case sensitive -- all keys will be stored/compared lowercase */
        String keyFromString(String);
        String payloadFromString(String);
};

extern CommandList * commands;
//extern Profile profile;

#endif