#include <stdio.h>
#include <math.h>

#define CALL_TEST(method, args...) method(__LINE__, args)

enum RootCount {
    none, one, two, infinity, error
};

typedef enum RootCount RootCount;

const char* oneRoot = "one root";
const char* twoRoots = "two roots";
const char* noneRoots = "zero roots";
const char* infiniteRoots = "infinite roots";
const char* solverError = "solver error";

RootCount solveEquation(double a, double b, double c, double roots[2]) {
    
    if(isnan(a) || isnan(b) || isnan(c) || roots == NULL) {
        return error;
    }
    
    if(a == 0 && b == 0 && c == 0) return infinity;
    
    double discriminant = b * b - 4 * a * c;
    
    if(discriminant == 0) {
        roots[0] = -(b / (a * 2));
        return one;
    }
    
    if(discriminant < 0) {
        return none;
    }
    
    discriminant = sqrt(discriminant);
    
    roots[0] = (-b + discriminant) / (a * 2);
    roots[1] = (-b - discriminant) / (a * 2);
    
    return two;
}

const char* responseToStr(RootCount response) {
    if(response == one) return oneRoot;
    if(response == two) return twoRoots;
    if(response == none) return noneRoots;
    if(response == infinity) return infiniteRoots;
    return solverError;
}

short testResponse(RootCount response, RootCount expected, int line) {
    if(response == expected) return 1;
    printf("Test failed on line %d: %s expected, got %s\n", line, responseToStr(expected), responseToStr(response));
    return 0;
}


void testSolverOneRoot(int line, double a, double b, double c, double root) {
    double roots[2];
    RootCount result = solveEquation(a, b, c, roots);
    if(testResponse(result, one, line)) {
        if(root != roots[0])
            printf("Test failed on line %d: single %lf root expected, got %lf\n", line, root, roots[0]);
    }
}

void testSolverTwoRoots(int line, double a, double b, double c, double root1, double root2) {
    double roots[2];
    RootCount result = solveEquation(a, b, c, roots);
    if(testResponse(result, two, line)) {
        if(root1 != roots[0] || root2 != roots[1])
            printf("Test failed on line %d: expected roots %lf, %lf, got %lf, %lf\n", line, root1, root2, roots[0], roots[1]);
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
    CALL_TEST(testSolverResponse, 0, 0, 0, infinity);
    CALL_TEST(testSolverResponse, 5, 5, 5, none);
    CALL_TEST(testSolverResponse, 1, 1, 1, none);
}

void solveUserEquation() {
    double a, b, c;
    double result[2];
    
    printf("Enter a, b, c: ");
    scanf("%lf %lf %lf", &a, &b, &c);
    
    RootCount rootCount = solveEquation(a, b, c, result);
    
    if(rootCount == none) {
        printf("no roots\n");
    } else if(rootCount == one) {
        printf("root: %lf\n", result[0]);
    } else if(rootCount == two) {
        printf("roots: %lf, %lf\n", result[0], result[1]);
    } else if(rootCount == infinity) {
        printf("infinite roots\n");
    }
}

int main(int argc, const char * argv[]) {
    
    test();
    
    for(;;) solveUserEquation();
    
    return 0;
}
