#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int stack[1000];
int sp = -1;

typedef struct {
    char name[32];
    int value;
} Variable;

Variable vars[100];
int varCount = 0;

int getVarIndex(const char *name) {
    for (int i = 0; i < varCount; i++) {
        if (strcmp(vars[i].name, name) == 0)
            return i;
    }
    strcpy(vars[varCount].name, name);
    return varCount++;
}

int main() {
    FILE *f = fopen("program.byte", "r");
    if (!f) {
        perror("program.byte");
        return 1;
    }

    char line[100];
    while (fgets(line, sizeof(line), f)) {
        char op[10], arg[32];
        if (sscanf(line, "%s %s", op, arg) >= 1) {
            if (strcmp(op, "PUSH") == 0) {
                stack[++sp] = atoi(arg);
            } else if (strcmp(op, "ADD") == 0) {
                int b = stack[sp--], a = stack[sp--];
                stack[++sp] = a + b;
            } else if (strcmp(op, "STORE") == 0) {
                int idx = getVarIndex(arg);
                vars[idx].value = stack[sp--];
            } else if (strcmp(op, "LOAD") == 0) {
                int idx = getVarIndex(arg);
                stack[++sp] = vars[idx].value;
            } else if (strcmp(op, "PRINT") == 0) {
                printf("%d\n", stack[sp--]);
            }
        }
    }

    fclose(f);
    return 0;
}
