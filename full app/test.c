// test.c - Sample C program for Project Azaadi

int main() {
    // Variable declarations
    int x = 10;
    int y = 20;
    int sum = x + y;
    
    // Print result
    print(sum);
    
    // Arithmetic operations
    int a = 5;
    int b = 3;
    print(a + b);
    print(a - b);
    print(a * b);
    print(a / b);
    
    // Conditional statement
    int num = 15;
    if (num > 10) {
        print(1);
    } else {
        print(0);
    }
    
    // Loop
    int i = 0;
    while (i < 5) {
        print(i);
        i = i + 1;
    }
    
    // Comparison operations
    int result = 5 < 10;
    print(result);
    
    result = 10 == 10;
    print(result);
    
    return 0;
}