#include <stdio.h>
#include <stdlib.h>

extern void vm_run(const char *filename);

int main(int argc, char **argv) {
    printf("====================================\n");
    printf("    Azaadi Virtual Machine v1.0\n");
    printf("====================================\n\n");
    
    if (argc < 2) {
        printf("Usage: %s <bytecode.azb>\n", argv[0]);
        printf("\nExample:\n");
        printf("  %s program.azb\n", argv[0]);
        return 1;
    }
    
    const char *bytecode_file = argv[1];
    
    printf("Loading bytecode: %s\n", bytecode_file);
    printf("Executing...\n\n");
    printf("--- Output ---\n");
    
    vm_run(bytecode_file);
    
    printf("\n--- End of execution ---\n");
    
    return 0;
}