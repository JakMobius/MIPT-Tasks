
#include "akinator.h"
#include "akinator_image.h"

void main_akinator_loop(s_tree* tree) {

    printf("%s", AKINATOR_IMAGE);
    printf("Hey, how it's going?\nI am Akinator. What will be your wish?\n");
    akinator_help();

    while (true) {
        char answer[16];
        if(scanf("%15s", answer) == 1) {
            if(strcmp("guess", answer) == 0) akinator_guess(tree);
            else if(strcmp("describe", answer) == 0) akinator_describe(tree);
            else if(strcmp("difference", answer) == 0) akinator_difference(tree);
            else if(strcmp("bye", answer) == 0) break;
            else if(strcmp("help", answer) == 0) akinator_help();
            else if(strcmp("dump", answer) == 0) akinator_dump(tree);
            else {
                printf("Sorry, I don't understand you. Let's try again.\n");
                continue;
            }

            printf("What will we do next?\n");
        }
    }

    printf("Have a nice day!\n");
}

void akinator_dump(s_tree* tree) {
    FILE* file = fopen("./dump.txt", "w");
    if(file) {
        tree_parser_dump(tree, file);
        fclose(file);
        system("/usr/local/bin/dot -Tpng ./dump.txt -o ./dump.png -Gsize=0,0\\\\!");
        printf("I've dumped my database for you\n");
    } else {
        printf("Sorry, the gods forbade me to do the dump, saying so: %s\n", strerror(errno));
    }
}

void akinator_help() {
    printf("Type 'guess' if you want me to guess what you have wished for\n");
    printf("Type 'difference' if you want me to say the difference between objects\n");
    printf("Type 'bye' if you have already tired of me\n");
    printf("Type 'help' if you get lost in this terrible and not user-friendly interface.\n");
    printf("Type 'describe' if you want me to describe you some object\n");
}

void akinator_describe(s_tree* tree) {
    printf("What would you like me to describe?\n");

    const char* object_name;
    const bool* path;

    akinator_ask_for_object(tree, &path, &object_name);

    if(!path) {
        free((void*)object_name);
        return;
    }

    s_tree_node* current = tree->root;
    s_tree_node* parent = NULL;
    int path_index = 0;

    printf("%s", object_name);

    while(current->left) {
        if(parent != NULL) {
            printf(",");
        }

        parent = current;
        if(path[path_index++]) {
            current = current->right;
            if(!current->left) printf(" and");
            printf(" not %s", parent->value);
        } else {
            current = current->left;
            if(!current->left) printf(" and");
            printf(" %s", parent->value);
        }
    }

    printf("\n");

    free((void*)path);
    free((void*)object_name);
}

void akinator_guess(s_tree* tree) {
    while(true) {
        s_tree_node* question = tree -> root;

        while(true) {
            if(question->left == NULL) {
                printf("It is %s!\n", question->value);
                if(akinator_ask("Am I right?")) {
                    printf("I was sure!\n");
                } else {
                    printf("Oh. This makes me upset.\nWell, what was that then?\n> It was ");
                    const char* newEntry = akinator_ask_string();

                    printf("I will try to remember. And what distinguishes %s from %s?\n> It ", newEntry, question->value);
                    const char* distinction = akinator_ask_string();

                    s_tree_node* newEntryNode;
                    s_tree_node* oldEntryNode;

                    tree_get_free_node(tree, &newEntryNode);
                    tree_get_free_node(tree, &oldEntryNode);

                    oldEntryNode->value = question->value;
                    newEntryNode->value = newEntry;
                    question->value = distinction;
                    question->left = newEntryNode;
                    question->right = oldEntryNode;

                    FILE* file = fopen("./database.db", "w");
                    if(file) {
                        tree_parser_serialize(tree, file);
                        fclose(file);
                    }

                    printf("Well, now you won't fool me.\n");
                }
                break;
            }

            if(akinator_ask("It %s?", question->value)) question = question -> left;
            else question = question -> right;
        }

        if(!akinator_ask("Let's play once more?")) break;
    }
}

void akinator_difference(s_tree* tree) {
    printf("Which object shall we take for comparison?\n");

    const char* first_object;
    const bool* first_object_path;

    akinator_ask_for_object(tree, &first_object_path, &first_object);

    if(!first_object_path) {
        free((void*)first_object_path);
        return;
    }

    printf("What shall we compare with?\n");

    const char* second_object;
    const bool* second_object_path;

    akinator_ask_for_object(tree, &second_object_path, &second_object);

    bool equal = strcmp(first_object, second_object) == 0;

    if(equal) {
        printf("Are you asking me to tell difference between same objects?\n");
    }

    if(!second_object_path || equal) {
        free((void*)first_object);
        free((void*)first_object_path);
        free((void*)second_object);
        return;
    }

    printf("So what differs %s from %s...\n", first_object, second_object);

    s_tree_node* current = tree->root;

    for(int i = 0;;i++) {
        if(first_object_path[i] == second_object_path[i]) {
            if(first_object_path[i]) current = current->right;
            else current = current->left;
            continue;
        }

        if(first_object_path[i]) {
            printf("%s %s, while %s is not\n", first_object, current->value, second_object);
        } else {
            printf("%s %s, while %s is not\n", second_object, current->value, first_object);
        }
        break;
    }

    free((void*)first_object);
    free((void*)first_object_path);
    free((void*)second_object);
    free((void*)second_object_path);
}

bool akinator_ask(const char* format, ...) {
    while(true) {
        va_list ap;
        va_start (ap, format);
        vprintf(format, ap);
        va_end(ap);

        printf(" ");

        char answer[5];
        if(scanf("%4s", answer) == 1) {
            if(strcmp("yes", answer) == 0) return true;
            if(strcmp("no", answer) == 0) return false;
        }

        printf("Sorry, I don't understand you. Let's try again.\n");
    }
}

const char* akinator_ask_string(){
    size_t size = 64;

    char *str = malloc(sizeof(char) * size);
    char ch;
    size_t len = 0;

    if(!str) return str;

    while(scanf("%c", &ch) == 1){
        if(ch == '\n') {
            if(len > 0) break;
            continue;
        }
        str[len++] = ch;
        if(len == size){
            size += 64;
            str = realloc(str, sizeof(char) * size);
            if(!str) return str;
        }
    }
    str[len] = '\0';

    return str;
}

void akinator_ask_for_object(s_tree* tree, const bool** path_buffer, const char** name_buffer) {

    const char* object_name = akinator_ask_string();
    const bool* path = tree_searcher_object_path(tree, object_name);

    if(!path) {
        printf("I don't know anything about %s...\n", object_name);
    }

    *name_buffer = object_name;
    *path_buffer = path;
}