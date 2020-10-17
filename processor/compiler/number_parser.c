
#include "errors.h"
#include "compiler.h"
#include "number_parser.h"

u_compiler_number read_number(s_compiler* thou, bool is_integer) {
    u_compiler_number result = {0};
    
    char c = compiler_next_char(thou);
    bool negative = c == '-';
    bool fraction = false;
    bool exponent = false;
    unsigned int exponent_value = 0;
    double factor = 1;
    
    if(negative) {
        c = compiler_next_char(thou);
        factor = -1;
    }
    
    int base = 10;
    
    if(c < '0' || c > '9') {
        compiler_error(thou, "signed number expected");
        return result;
    }
    
    if(c == '0') {
        c = compiler_next_char(thou);
        if(c == 'x') {
            if(!is_integer) {
                compiler_error(thou, "floating-point numbers may not be hexademical");
                return result;
            }
            
            // Reading hexademical number
            base = 16;
            c = compiler_next_char(thou);
        } else if(c == 'b') {
            if(!is_integer) {
                compiler_error(thou, "floating-point numbers may not be binary");
                return result;
            }
            
            // Reading binary number
            base = 2;
            c = compiler_next_char(thou);
        } else if(c == 'o') {
            if(!is_integer) {
                compiler_error(thou, "floating-point numbers may not be octal");
                return result;
            }
            
            // It's an octal number
            
            base = 8;
            c = compiler_next_char(thou);
        } else if(c == '.') {
            if(is_integer) {
                compiler_error(thou, "dots are invalid in integer literals");
                compiler_previous_char(thou);
                return result;
            }
            
            fraction = true;
            c = compiler_next_char(thou);
        } else if(is_whitespace(c) || c == '\n' || c == '\0') {
            // This is just 0
            
            result.unsigned_value = 0;
            return result;
        } else {
            compiler_error(thou, "unexpected symbols in number literal");
            compiler_previous_char(thou);
            return result;
        }
        
        c = tolower(c);
            
        if(is_whitespace(c) || c == '\n' || c == '\0') {
            // 0x, 0o or 0b is followed with whitespace
            compiler_error(thou, "number literal is incomplete");
            compiler_previous_char(thou);
            return result;
        } else if(!is_digit_16(c)) {
            // 0x, 0o or 0b is followed by some random symbol
            compiler_error(thou, "unexpected symbols in number literal");
            compiler_previous_char(thou);
            return result;
        }
    }
    
    do {
        if(exponent) {
            exponent_value *= base;
        } if(is_integer) {
            result.unsigned_value *= base;
        } else {
            if(fraction) {
                factor /= 10;
            }
            
            result.double_value *= base;
        }
        
        int digit;
        
        if(c < '0' || c > '9') digit = c - 'a' + 10;
        else digit = c - '0';
        
        if(digit >= base) {
            compiler_error(thou, "cannot use '%c' in base-%d numeric literal", c, base);
        } else {
            if(exponent) {
                exponent_value += digit;
            } else if(is_integer) {
                result.unsigned_value += digit;
            } else {
                result.double_value += digit;
            }
        }
        c = compiler_next_char(thou);
        c = tolower(c);
        
        if(!is_digit_16(c) || (c == 'e' && !is_integer)) {
            if(c == '.') {
                if(is_integer) {
                    compiler_error(thou, "dots are invalid in integer literals");
                    compiler_previous_char(thou);
                    return result;
                }
                if(exponent) {
                    compiler_error(thou, "exponent cannot contain '.'");
                    compiler_previous_char(thou);
                    return result;
                }
                if(fraction) {
                    compiler_error(thou, "duplicating '.' in number literal");
                    compiler_previous_char(thou);
                    return result;
                }
                fraction = true;
            } else if(c == 'e') {
                if(exponent) {
                    compiler_error(thou, "duplicating 'e' in number literal");
                    compiler_previous_char(thou);
                    return result;
                }
                exponent = true;
            } else {
                break;
            }
            
            c = compiler_next_char(thou);
            c = tolower(c);
            if(!is_digit_16(c)) {
                if(exponent) {
                    compiler_error(thou, "expected exponent value after 'e' character");
                } else {
                    compiler_error(thou, "expected fractional value after '.' character");
                }
                compiler_previous_char(thou);
                return result;
            }
        }
    } while(true);
    
    if(!is_whitespace(c) && c != '\n' && c != '\0') {
        thou -> state = COMPILER_STATE_ERROR;
        compiler_error(thou, "unexpected symbols in number literal");
    }
    
    compiler_previous_char(thou);
    
    double exponent_coeff = pow(10, exponent_value);
    
    if(is_integer) {
        result.unsigned_value *= (unsigned long long)(factor * exponent_coeff);
    } else {
        result.double_value *= factor * exponent_coeff;
    }
    
    return result;
}
