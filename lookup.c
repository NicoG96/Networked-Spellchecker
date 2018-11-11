#include "network.h"

_Bool lookup(char *word) {
    _Bool match = 0;

    //var to hold fetched dictionary word
    char buf[DICT_BUF];

    //get rid of '\r' first (words in dictionary already have '\n', saving time)
    size_t len = strlen(word);
    word[len - 2] = '\n';
    word[len - 1] = '\0';

    while(fgets(buf, DICT_BUF, DICTIONARY) != NULL) {
        //printf("%s", buf);

        //if the word isn't a match, get next word
        if(strcmp(buf, word) != 0) {
            continue;
        }
        else {
            //printf("\"%s\" matched successfully!\n", word);
            rewind(DICTIONARY);
            return ++match;
        }
    }
    return match;
}