// reads a sentence from the user.
// identifies each words grammar role.
// if word is unknown, asks the user. remembers next time.
// e.g.
// I gave the red toy to a very ugly dog and it shat blood.
// n v def_art adj n prep indef_art adv adj n conj n v n.
// or
// n:subj v:past art:def adj n_dir_obj prep art:indef adv adj n:ind_obj conj n:subj v:past n:dir_obj
//
// Something like that.
// Then after that, I could maybe work out noun phrases, subclauses, etc.

// grammar parts
// pronouns
//  personal
//  relative
//  question
//  demonstrative
//  interrogative
//  indefinite
// conjunctions
//   coordinating
//   correlative
//   subordinating
// prepositions
// determiner
//   numbers
//     cardinals
//     ordinals
//     multipliers
//  articles
//  demonstrative and interrogative words
//  posessives
//  quantifying words
// nouns
//  singular
//  plural
// compound nouns
// prep phrases
// adjective phrases
// noun phrases
//   Determiner + Pre-modifiers + NOUN + Postmodifiers/Complement
// adjectives
//   quantifiers
//   regular
//   comparative
//   superlative
// adverbs
// verbs
//  tenses
//  gerund
// articles
// verb phrases
//
// OTHER STUFF
// word order
//   questions
//   clauses
//   imperatives
// it's possible that the same word can be a verb or a noun. Then what?
// I could have a probability score that says what a word is most likely to be, and it could be based on where it is in the sentence. For example
// "the kill was fresh" I might misidentify 'kill' as a verb, but because its preceded by 'the' it's either going to be a noun, adverb, or adj.
// It's kind of like that 'guess the animal' program but instead of guessing the animal, it's guessing the type of word and the clues are everything else in the sentence.
// if it gets something wrong, you can correct it. eventually, it'll learn to do it better.
// instead of having a definitive answer though, it could adjust probability scores. it's basically a deconstructed neural net.
// i might even scan a dictionary. I'd have two entries for kill, noun and verb. the program has to decide which one it is. preceded by an article is a big clue. followed by a verb is another.
// but how does it know was is a verb? the dictionary I assume would only have one entry for that, so it's 100% likely a verb. but let's say it wasn't. say it was 65% likely to be a verb. I can either go with the most likely thing or calculate for both but display the most likely thing first. could I neural net this? have the network have as input the whole sentence and the output is what each thing is. but for that I need training data. so...
// I don't know.
//
//
// For now, assume everything is lowercase because dealing with 'the' and 'The' is annoying. I can fix it later when proper nouns might be an issue.

// next time, finish str_in_list function.
// also throw some free functions in there sometime.
//
// next time: problem with str_in_list() is that if I'm searching for a multipart word in the list, it won't work because of spaces .fix that. e.g.
// "to eat" "to eat to read to fart". it won't find it. also searching for "read to" matches. I need a tsv or something instead.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum gclass {unknown, noun, pronoun, verb, adjective, adverb, preposition, conjunction, article};

char *enum_names[] = { "unknown", "noun", "pronoun", "verb", "adjective", "adverb", "preposition", "conjunction", "article" };
typedef struct _word{
    char* w;            // the word.
    enum gclass type;   // what its grammatical type is.
    struct _word *next;
    struct _word *prev;
}_word;

char *PRONOUNS = "all another any anybody anyone anything aught both each each other either enough everybody everyone everything few he her hers herself him himself his I it itself many me mine most myself naught neither no one nobody none nothing nought one one another other others ought ours ourself ourselves several she some somebody someone something such suchlike that thee theirs theirself theirselves them themself themselves there these they thine this those thou thy thyself us we what whatever whatsoever whether which whichever whichsoever who whoever whom whomever whomso whomsoever whose whosever whosesoever whoso whosoever ye yon you yours yourself yourselves";

char *PREPOSITIONS = "aboard about above across after against along amid among anti around as at before behind below beneath beside besides between beyond but by concerning considering despite down during except excepting excluding following for from in inside into like minus near of off on onto opposite outside over past per plus regarding round save since than through to toward towards under underneath unlike until up upon versus via with within without";

char *CONJUNCTIONS = "and but or because nor for yet so although since";

char *VERBS = "am are is was be were been has have had do did does done say said says go goes went gone get gets got gotten make makes made know knows knew known think thinks thought take takes took taken see sees saw seen come comes came want wants wanted look looks looked use uses used find finds found give gives gave given tell tells told work works worked call calls called try tries tried ask asks asked need needs needed feel feels felt become becomes became";

char *NOUNS = "cat cats dog dogs shit shits table tables time year years people person way ways day days man men thing things woman women life lives child children world worlds school schools state states family families student students group groups country countries problem problems hand hands part parts place places case cases week weeks company companies system systems program programs question questions work government governments number numbers night nights point points home homes water fish fishes";

char *ADJECTIVES = "other new good high old great big American small large national young different black long little important political bad white real best right social only public sure low early able human local late hard major better economic strong possible whole free military true federal international full special easy clear recent certain personal open red difficult available likely short single medical current wrong private past foreign fine common poor natural significant similar hot dead central happy serious ready simple left physical general environmental financial blue democratic dark various entire close legal religious cold final main green nice huge popular traditional cultural";

// print a linked list given a pointer to the root node.
void print_list(_word* rt);

// decide the grammatical types of words.
void parse_list(_word* rt);

// find given string in list of strings. Return 1 if found, 0 otherwise.
// Differs from strstr() because string can't be a subword, has to be a whole
// word.
int str_in_list(char* str, char* list);

int main(int argc, char **argv){
    unsigned i;

    _word* root = NULL;
    _word* curr = NULL;
    _word* temp = NULL;

    if(argc == 1){
        printf("USAGE: %s <English sentence>\n", argv[0]);
        puts("I will tell you what each grammatical part is.");
        exit(0);
    }

    // build double-linked list and copy words from argv[] into it.
    for(i = 1; i < argc; i++){

        temp = malloc(sizeof(_word));

        if(root == NULL){
            root = temp;
            curr = root;
            curr->prev = NULL;
        }

        else{
            curr->next = temp;
            curr->next->prev = curr;
            curr = curr->next;
        }

        temp->w = malloc(strlen(argv[i]) + 1);
        strcpy(curr->w, argv[i]);
        curr->type = unknown;
    }
    curr->next = NULL;

    // check by printing list.
    //print_list(root);

    // do a preliminary ugly rule-based parse just to see.
    curr = root;
    parse_list(root);
    print_list(root);

    // next tasks:
    // work on importing a list of data to determine grammar types.
    // sketch out how to figure out which type it is if more than one.
    // deal with phrases.

}



void print_list(_word* rt){
    while(rt){
        //printf("address of this node: %p\n", rt);
        printf("word: %s\n", rt->w);
        printf("type: %s\n\n", enum_names[rt->type]);
        //printf("prev: %p\n", rt->prev);
        //printf("next: %p\n\n", rt->next);

        rt = rt->next;
    }
}



void parse_list(_word* rt){
    while(rt){
            if(str_in_list(rt->w, "the a an"))
                rt->type = article;
            else if(str_in_list(rt->w, CONJUNCTIONS))
                rt->type = conjunction;
            else if(str_in_list(rt->w, PREPOSITIONS))
                rt->type = preposition;
            else if(str_in_list(rt->w, PRONOUNS))
                rt->type = pronoun;
            else if(str_in_list(rt->w, VERBS))
                rt->type = verb;
            else if(str_in_list(rt->w, NOUNS))
                rt->type = noun;
            else if(str_in_list(rt->w, ADJECTIVES))
                rt->type = adjective;

        rt = rt->next;
    }
}



int str_in_list(char* str, char* list){
    unsigned a, b;
    a = b = 0;

    while(1){
        if(str[a] == '\0'){
            if(list[b] == ' ' || list[b] == '\0') return 1;
            else{
                a = 0;
                while(list[b] != ' ' && list[b] != '\0') b++;
                if(list[b] == '\0') return 0;
            }
        }
        else{
            if(list[b] == '\0') return 0;
            else if(list[b] == ' '){
                a = 0;
                b++;
            }
            else{
                if(str[a] == list[b]) {
                    a++;
                    b++;
                }
                else {
                    a = 0;
                    while(list[b] != ' ' && list[b] != '\0') b++;
                    if(list[b] == '\0') return 0;
                }
            }
        }
    }
}
