#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"


extern int hash_function(const char* word);

int read_word(FILE* fp, char* buf) {

    //printf("%s\n", "Reading word");

    char c;
    int strindex = 0;
    int read_count = 0;
    while((c = fgetc(fp))) { //look ma, we can read infinity here
        if(c == ' ' || c == '\n' || c == '\t' || c == EOF) { //we found a space on an EOF
            if ( strindex <= LENGTH + 1 && strindex > 0) //aint nobody overflowing this buff
                buf[strindex] = '\0'; // terminate the string
            if (c == EOF && strindex == 0)
                return EOF;
            else if ( strindex == 0)
                continue;
            else
                return strindex;
        }
        else // this is not a space or EOF so we gonna treatz it as partz of da word here
            if (strindex <= LENGTH) //look ma, we wanna make sure nobody overflows the buff
                buf[strindex]=c;

        strindex++;
        read_count++;
    }
}

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {
    for (int i=0; i<HASH_SIZE; i++) //init the hashtable to null
        hashtable[i]=NULL;

    FILE* word_list;
    word_list = fopen(dictionary_file, "r"); //Try to open the wordlist file
    if (word_list == NULL) {
        //printf("%s", "Failed to open dictionary file\n");
        return false;
    }
    int count=0;
    int bucket=0;
    node* n = malloc(sizeof(node));
    n->next=NULL;
    while (read_word(word_list, n->word) != EOF) {
        if (n->word == NULL) {
            //printf("%s\n", "NULL word detected..fishy-fishy..moving on");
            continue;
        }
        count++;
        bucket = hash_function(n->word);
        //printf("%s: %s %s: %d\n", "Loaded word", n->word, "bucket", bucket);
        if (hashtable[bucket] != NULL) {
            n->next=hashtable[bucket];
            hashtable[bucket]=n;
        } else {
            hashtable[bucket]=n;
        }
        n = malloc(sizeof(node));
        n->next=NULL;
}

    //printf ("%s%d\n", "Loaded dictionary word count: ", count);
    fclose(word_list);
    return true;
}

bool check_word(const char* word, hashmap_t hashtable[]) {
    //We assume that the word we are checking has already been prepared and all we need is a dictionary compare

    //Lower the string
    int word_len=strlen(word);
    char lower_word[word_len];
    for(int i = 0; i<word_len; i++)
        lower_word[i] = tolower(word[i]);
    lower_word[word_len]='\0';

    //which bucket will we search
    int bucket = hash_function(lower_word);

    //search the bucket linked list for the word
    node* list_item = hashtable[bucket];
    while (list_item != NULL) {
        if (strcmp(list_item->word, lower_word) == 0)
            return true;
        list_item=list_item->next;
    }
    return false;
}

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]) {
    int bad_word_count=0;


    //reading our file one word at a time
    char word[LENGTH];
    while (read_word(fp, word) != EOF) {
        //now lets prepare the word we will be checking
        //printf("%s: %s\n", "Original  word: ", word);

        char final_word[LENGTH];


        //inspect the string left to right
        int word_len=strlen(word);
        char lr_stage_word[word_len];
        int j=0;
        int alpha_flag1=0;
        for(int i = 0; i<word_len; i++) {
            if (!isalpha(word[i]) && alpha_flag1 == 0 ) //first chars are not alpha, ignoring
                continue;
            else
                alpha_flag1=1;

            lr_stage_word[j++]=word[i];
        }
        lr_stage_word[j]='\0';


        //inspect the string right to left

        int lr_stage_word_len=strlen(lr_stage_word);
        char rl_stage_word[lr_stage_word_len];
        int y=0;
        int alpha_flag2=0;
        for (int x = lr_stage_word_len - 1; x>=0; x--) {
            if (!isalpha(lr_stage_word[x]) && alpha_flag2 == 0 ) //last chars are not alpha, ignoring
                continue;
            else
                alpha_flag2=1;

            rl_stage_word[y++]=lr_stage_word[x];
        }
        rl_stage_word[y]='\0';

        //ok now reverse the string and ready
        int rl_stage_word_len=strlen(rl_stage_word);
        int b = 0 ;
        for ( int c = rl_stage_word_len -1; c>=0; c--){
            final_word[b++]=rl_stage_word[c];
        }
        final_word[b]='\0';

        //printf("%s: %s\n", "Final word: ", final_word);

        if (check_word(final_word, hashtable) == true) {
            //printf("%s %s\n", final_word, "is correct");
        }
        else {
            //printf("%s %s\n", final_word, "is wrong");
            misspelled[bad_word_count]=malloc(sizeof(char) * strlen(final_word));
            strcpy(misspelled[bad_word_count], final_word);
            bad_word_count++;
        }

    }

    return bad_word_count;
}
