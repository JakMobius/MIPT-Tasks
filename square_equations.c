#include <stdio.h>
#include <math.h>

#define CALL_TEST(method, args...) method(__LINE__, args)
#define FORMAT_TEST(string, line, args...) printf("Test failed on line %d: " string, line, args)

enum RootCount {
    RootCount_NONE,
    RootCount_ONE,
    RootCount_TWO,
    RootCount_INFINITY,
    RootCount_ERROR
};

const char* RootCount_descriptions[] = {
    "one root",
    "two roots",
    "zero roots",
    "infinite roots",
    "solver error"
};

typedef enum RootCount RootCount;

RootCount solveEquation(double a, double b, double c, double roots[2]) {
    
    if(isnan(a) || isnan(b) || isnan(c) || roots == NULL) {
        return RootCount_ERROR;
    }
    
    if(a == 0 && b == 0 && c == 0) return RootCount_INFINITY;
    
    double discriminant = b * b - 4 * a * c;
    
    if(discriminant >= -1E-50 && discriminant <= 1E-50) {
        roots[0] = -(b / (a * 2));
        return RootCount_ONE;
    }
    
    if(discriminant < 0) {
        return RootCount_NONE;
    }
    
    discriminant = sqrt(discriminant);
    
    roots[0] = (-b + discriminant) / (a * 2);
    roots[1] = (-b - discriminant) / (a * 2);
    
    return RootCount_TWO;
}

const char* responseToStr(RootCount response) {
    return RootCount_descriptions[response];
}

uint8_t testResponse(RootCount response, RootCount expected, int line) {
    if(response == expected) return 1;
    FORMAT_TEST("%s expected, got %s\n", line, responseToStr(expected), responseToStr(response));
    return 0;
}


void testSolverOneRoot(int line, double a, double b, double c, double root) {
    double roots[2];
    RootCount result = solveEquation(a, b, c, roots);
    if(testResponse(result, RootCount_ONE, line)) {
        if(root != roots[0])
            FORMAT_TEST("single %lf root expected, got %lf\n", line, root, roots[0]);
    }
}

void testSolverTwoRoots(int line, double a, double b, double c, double root1, double root2) {
    double roots[2];
    RootCount result = solveEquation(a, b, c, roots);
    if(testResponse(result, RootCount_TWO, line)) {
        if(root1 != roots[0] || root2 != roots[1])
            FORMAT_TEST("expected roots %lf, %lf, got %lf, %lf\n", line, root1, root2, roots[0], roots[1]);
    }
}

void testSolverResponse(int line, double a, double b, double c, RootCount response) {
    double roots[2];
    RootCount result = solveEquation(a, b, c, roots);
    testResponse(result, response, line);
}

void test() {
    CALL_TEST(testSolverOneRoot, 1, 2, 1, -1);
    CALL_TEST(testSolverOneRoot, 1, -2, 1, 1);
    CALL_TEST(testSolverTwoRoots, -2, -2, 4, -2, 1);
    CALL_TEST(testSolverTwoRoots, 4, 2, -2, 0.5, -1);
    CALL_TEST(testSolverResponse, 0, 0, 0, RootCount_INFINITY);
    CALL_TEST(testSolverResponse, 5, 5, 5, RootCount_NONE);
    CALL_TEST(testSolverResponse, 1, 1, 1, RootCount_NONE);
}

void solveUserEquation() {
    double a = 0, b = 0, c = 0;
    double result[2] = {0, 0};
    
    while(1) {
        printf("Enter a, b, c: ");
        if(scanf("%lf %lf %lf", &a, &b, &c) == 3) {
            break;
        }
    }
    
    RootCount rootCount = solveEquation(a, b, c, result);
    
    if(rootCount == RootCount_NONE) {
        printf("no roots\n");
    } else if(rootCount == RootCount_ONE) {
        printf("root: %lf\n", result[0]);
    } else if(rootCount == RootCount_TWO) {
        printf("roots: %lf, %lf\n", result[0], result[1]);
    } else if(rootCount == RootCount_INFINITY) {
        printf("infinite roots\n");
    } else if(rootCount == RootCount_ERROR) {
        printf("solver error\n");
    }
}

int main(int argc, const char * argv[]) {
    
    test();
    
    for(;;) solveUserEquation();
    
    return 0;
}
