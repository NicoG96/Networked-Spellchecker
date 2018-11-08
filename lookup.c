#include "network.h"

_Bool lookup(char *word) {
    _Bool match = 0;
    char buf[DICT_BUF];

    while((fgets(buf, DICT_BUF, DICTIONARY) != NULL)) {
        //printf("%s", buf);

        //get rid of '\n' first
        size_t len = strlen(buf);
        if(buf[len - 1] == '\n' && len > 1) {
            buf[len - 1] = '\0';
        }

        //if the word isn't a match, get next word
        if(strcmp(buf, word) != 0) {
            continue;
        }
        else {
            //printf("\"%s\" matched successfully!\n", word);
            return ++match;
        }
    }
    return match;
}