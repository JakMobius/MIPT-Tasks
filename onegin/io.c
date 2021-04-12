
#include "i

void write_file(const char* filepath, char* buffer) {
    FILE* output = fopen(filepath, "w");
    
    if(output == NULL) {
        perror("Unable to write to file");
        return;
    }
    
    fprintf(output, "%s", buffer);
    fclose(output);
}
