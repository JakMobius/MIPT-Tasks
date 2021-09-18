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
    "zero roots",
    "one root",
    "two roots",
    "infinite roots",
    "solver error"
};

typedef enum RootCount RootCount;

const double epsilon = 1E-50;

/**
 * @brief Determines if specified double is tiny (modulo less than epsilon)
 * @returns 1 if double is tiny, 0 otherwise
 */

uint8_t double_is_tiny(double a) {
    return a >= -epsilon && a <= epsilon;
}

/**
 * @brief Solves the square equation ax^2 + bx + c = 0
 * @param [in] a the a coefficient
 * @param [in] b the b coefficient
 * @param [in] c the c coefficient
 * @param [out] roots buffer for the roots of the equation. If the specified equation has one root, only roots[0] is written.
 * @returns Root count, if equation is correct. If given arguments are invalid, RootCount_ERROR is returned.
 */

RootCount solveEquation(double a, double b, double c, double roots[2]) {
    
    if(isnan(a) || isnan(b) || isnan(c) || roots == NULL) {
        return RootCount_ERROR;
    }
    
    if(a == 0 && b == 0 && c == 0) return RootCount_INFINITY;
    
    double discriminant = b * b - 4 * a * c;
    
    if(double_is_tiny(discriminant)) {
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

/**
 * @brief Tests if two RootCount enums match. Prints error message if they doesn't
 * @returns 1 if they match, 0 otherwise
 */
uint8_t testResponse(RootCount response, RootCount expected, int line) {
    if(response == expected) return 1;
    FORMAT_TEST("%s expected, got %s\n", line, RootCount_descriptions[expected], RootCount_descriptions[response]);
    return 0;
}

/**
 * @brief Tests if specified square equation has exactly one root. Prints error message if it doesn't
 * @param [in] line test line. This argument is usally provided by macro
 * @param [in] a the a coefficient
 * @param [in] b the b coefficient
 * @param [in] c the c coefficient
 * @param [in] root the expected root
*/
void testSolverOneRoot(int line, double a, double b, double c, double root) {
    double roots[2];
    RootCount result = solveEquation(a, b, c, roots);
    if(testResponse(result, RootCount_ONE, line)) {
        if(root != roots[0])
            FORMAT_TEST("single %lf root expected, got %lf\n", line, root, roots[0]);
    }
}

/**
 * @brief Tests if specified square equation has two roots. Prints error message if it doesn't
 * @param [in] line test line. This argument is usally provided by macro
 * @param [in] a the a coefficient
 * @param [in] b the b coefficient
 * @param [in] c the c coefficient
 * @param [in] root1 the root expected to be first
 * @param [in] root2 the root expected to be second
*/
void testSolverTwoRoots(int line, double a, double b, double c, double root1, double root2) {
    double roots[2];
    RootCount result = solveEquation(a, b, c, roots);
    if(testResponse(result, RootCount_TWO, line)) {
        if(root1 != roots[0] || root2 != roots[1])
            FORMAT_TEST("expected roots %lf, %lf, got %lf, %lf\n", line, root1, root2, roots[0], roots[1]);
    }
}

/**
 * @brief Tests if specified square equation leads to exact solver response. Prints error message if it doesn't.
 * @param [in] line test line. This argument is usally provided by macro
 * @param [in] a the a coefficient
 * @param [in] b the b coefficient
 * @param [in] c the c coefficient
 * @param [in] response expected solver response
 */

void testSolverResponse(int line, double a, double b, double c, RootCount response) {
    double roots[2];
    RootCount result = solveEquation(a, b, c, roots);
    testResponse(result, response, line);
}

/**
 * @brief Performs unit test
 */

void test() {
    CALL_TEST(testSolverOneRoot, 1, 2, 1, -1);
    CALL_TEST(testSolverOneRoot, 1, -2, 1, 1);
    CALL_TEST(testSolverTwoRoots, -2, -2, 4, -2, 1);
    CALL_TEST(testSolverTwoRoots, 4, 2, -2, 0.5, -1);
    CALL_TEST(testSolverResponse, 0, 0, 0, RootCount_INFINITY);
    CALL_TEST(testSolverResponse, 5, 5, 5, RootCount_NONE);
    CALL_TEST(testSolverResponse, 1, 1, 1, RootCount_NONE);
}

/**
 * @brief Solves equation from user input
 */

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


/**
 * @brief Entry point
 *
 * Execution of the program
 * starts here.
 *
 * @param [in] argc Number of arguments
 * @param [in] argv List of arguments
 *
 * @return Program exit status
*/

int main(int argc, const char * argv[]) {
    
    test();
    
    for(;;) solveUserEquation();
    
    return 0;
}
