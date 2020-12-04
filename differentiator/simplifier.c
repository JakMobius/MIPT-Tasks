#include <zconf.h>
#include "simplifier.h"
#include "tree.h"

// MARK: Context management

bool simplify_context_can_perform_action(s_simplifier_context* context) {
    return context->current_operations < context->config->max_operations_per_step;
}

void simplify_context_perform_action(s_simplifier_context* context) {
    context->current_operations++;
}

// MARK: Utils

bool __unused tree_node_is_number(s_tree_node* node) {
    return node->type == TREE_NODE_TYPE_NUMBER;
}

bool tree_node_is_unary_minus(s_tree_node* node) {
    return tree_node_is_operator(node, TREE_OPERATOR_TYPE_MINUS) && node->left == NULL;
}

s_tree_node** tree_get_pure_member(s_tree_node** member, s_tree_node** power) {

    bool powered = false;

    while(true) {
        if(tree_node_is_operator(*member, TREE_OPERATOR_TYPE_DIVIDE)) {
            member = &(*member)->left;
        } else if(tree_node_is_operator(*member, TREE_OPERATOR_TYPE_POW)) {
            if(powered) break;
            powered = true;
            if(power != NULL) *power = (*member)->right;
            member = &(*member)->left;
        } else break;
    }
    return member;
}

int tree_count_ordered_terms(s_tree_node* expression) {
    if(!tree_node_is_operator(expression, TREE_OPERATOR_TYPE_PLUS)) return 1;
    int result = 2;
    s_tree_node* runner = expression;
    while(runner->right && tree_node_is_operator(runner->right, TREE_OPERATOR_TYPE_PLUS)) {
        result++;
        runner = runner->right;
    }
    return result;
}

bool tree_subtree_depends_on_variable(s_tree_node* subtree, unsigned long variable_index) {
    if(!subtree) return false;
    if(subtree->type == TREE_NODE_TYPE_VARIABLE) {
        return ((s_tree_node_variable*)subtree)->variable_index == variable_index;
    }
    if(subtree->type == TREE_NODE_TYPE_OPERATOR) {
        return tree_subtree_depends_on_variable(subtree->left, variable_index) ||
               tree_subtree_depends_on_variable(subtree->right, variable_index);
    }
    return false;
}

bool tree_subtree_is_simple_value(s_tree_node* subtree) {
    if(!subtree) return false;
    if(subtree->type == TREE_NODE_TYPE_VARIABLE) {
        return false;
    }
    if(subtree->type == TREE_NODE_TYPE_OPERATOR) {
        switch(((s_tree_node_operator*)subtree)->operator_type) {
            case TREE_OPERATOR_TYPE_MULTIPLY:
            case TREE_OPERATOR_TYPE_DIVIDE:
            case TREE_OPERATOR_TYPE_PLUS:
            case TREE_OPERATOR_TYPE_MINUS:
                if(tree_node_is_unary_minus(subtree)) return tree_subtree_is_simple_value(subtree->right);
                return tree_subtree_is_simple_value(subtree->left) &&
                       tree_subtree_is_simple_value(subtree->right);
            default:
                return false;
        }
    }
    if(subtree->type == TREE_NODE_TYPE_NUMBER) {
        return true;
    }
    return false;
}

e_tree_error tree_node_multiply_node_by_one(s_tree_node** node) {
    s_tree_node* amplifier = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_MULTIPLY);
    if(!amplifier) return TREE_ERROR_OUT_OF_MEMORY;
    amplifier->left = *node;
    *node = amplifier;
    amplifier->right = (s_tree_node*) tree_create_number_from_double(1);
    if(!amplifier->right) return TREE_ERROR_OUT_OF_MEMORY;
    return TREE_OK;
}

e_tree_error tree_node_multiply_one_by_node(s_tree_node** node) {
    s_tree_node* amplifier = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_MULTIPLY);
    if(!amplifier) return TREE_ERROR_OUT_OF_MEMORY;
    amplifier->right = *node;
    *node = amplifier;
    amplifier->left = (s_tree_node*) tree_create_number_from_double(1);
    if(!amplifier->left) return TREE_ERROR_OUT_OF_MEMORY;
    return TREE_OK;
}

double tree_subtree_constant_value(s_tree_node* node) {
    if(node->type == TREE_NODE_TYPE_NUMBER) return (double) ((s_tree_node_number*)node)->float_value;
    if(tree_node_is_operator(node, TREE_OPERATOR_TYPE_MULTIPLY))
        return tree_subtree_constant_value(node->left) * tree_subtree_constant_value(node->right);
    if(tree_node_is_operator(node, TREE_OPERATOR_TYPE_PLUS))
        return tree_subtree_constant_value(node->left) + tree_subtree_constant_value(node->right);
    if(tree_node_is_operator(node, TREE_OPERATOR_TYPE_MINUS)) {
        if(!node->left) return -tree_subtree_constant_value(node->right);
        return tree_subtree_constant_value(node->left) - tree_subtree_constant_value(node->right);
    }
    if(tree_node_is_operator(node, TREE_OPERATOR_TYPE_DIVIDE))
        return tree_subtree_constant_value(node->left) / tree_subtree_constant_value(node->right);
    return false;
}

// MARK: Derivatives

e_tree_error tree_simplify_appliance(s_tree_node_operator** node_link, s_simplifier_context* context) {
    s_tree_node_operator* expression = *node_link;

    // Handling ((d)/(d u))... constructions
    // Fetching u index, if there is

    unsigned long differential_variable = tree_find_differential_variable(expression->tree_node.left);
    if(differential_variable == -1) return TREE_OK;

    // We'd better like to differentiate simplified
    // expressions

    while(tree_subtree_simplify(&expression->tree_node.right, context, expression));
    if(!simplify_context_can_perform_action(context)) return TREE_OK;

    // If there are any differentials in subexpression,
    // skipping them since this would lead to an infinite
    // complication of expression in some cases

    if(tree_node_contains_differential(expression->tree_node.right)) return TREE_OK;

    s_tree_node* derivative = NULL;
    e_tree_error error = tree_subtree_derivative(&expression->tree_node.right, &derivative, differential_variable, context);
    if(error) {
        tree_release_subtree(derivative);
        if(error == TREE_ERROR_CANNOT_TAKE_DERIVATIVE)
            return TREE_OK;
        return error;
    }
    tree_release_subtree((s_tree_node*) expression);

    *node_link = (s_tree_node_operator*) derivative;

    return TREE_OK;
}

e_tree_error tree_node_wrap_derivative(s_tree_node** node_link, s_tree_node** target_link, unsigned long variable_index) {
    s_tree_node* node = *node_link;

    e_tree_error error = TREE_OK;

    *target_link = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_CALL);
    if(!*target_link) return TREE_ERROR_OUT_OF_MEMORY;
    s_tree_node* wrapper = *target_link;

    // d/dx
    wrapper->left = (s_tree_node*)tree_create_node_operator(TREE_OPERATOR_TYPE_DIVIDE);
    if(!wrapper->left) return TREE_ERROR_OUT_OF_MEMORY;
    s_tree_node* differential_fraction = wrapper->left;

    // d
    differential_fraction->left = (s_tree_node*)tree_create_node_operator(TREE_OPERATOR_TYPE_DIFFERENTIAL);
    if(!differential_fraction->left) return TREE_ERROR_OUT_OF_MEMORY;

    // dx
    differential_fraction->right = (s_tree_node*)tree_create_node_operator(TREE_OPERATOR_TYPE_DIFFERENTIAL);
    if(!differential_fraction->right) return TREE_ERROR_OUT_OF_MEMORY;

    // x
    s_tree_node_variable* variable = tree_create_node_variable();
    variable->variable_index = variable_index;
    differential_fraction->right->right = (s_tree_node*)variable;

    wrapper->right = node;

    return error;
}

e_tree_error tree_node_pow_derivative(s_tree_node** node_link, s_tree_node** target_link, unsigned long variable_index, s_simplifier_context* context) {
    s_tree_node* node = *node_link;

    e_tree_error error = TREE_OK;

    bool left_depends = tree_subtree_depends_on_variable(node->left, variable_index);
    bool right_depends = tree_subtree_depends_on_variable(node->right, variable_index);

    if(!left_depends && !right_depends) {
        *target_link = (s_tree_node*) tree_create_number_from_double(0);
        if(*target_link == NULL) return TREE_ERROR_OUT_OF_MEMORY;
        return TREE_OK;
    }

    *target_link = (s_tree_node*)tree_create_node_operator(TREE_OPERATOR_TYPE_MULTIPLY);
    if(!*target_link) return TREE_ERROR_OUT_OF_MEMORY;
    s_tree_node* target = *target_link;

    if(right_depends) {
        if(left_depends) {
            // (u^v)' = (u^v)(v'ln(u)+((v/u)*u'))

            target->left = tree_node_clone_deep(node);
            if(!target->left) return TREE_ERROR_OUT_OF_MEMORY;
            target->right = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_PLUS);

            // (v'ln(u)+((v/u)*u'))
            s_tree_node* multiplier = target->right;

            multiplier->left = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_MULTIPLY);
            if(!multiplier->left) return TREE_ERROR_OUT_OF_MEMORY;

            // v'ln(u)
            s_tree_node* multiplier_left = multiplier->left;
            if((error = tree_subtree_derivative(&node->right, &multiplier_left->left, variable_index, context))) return error;

            multiplier_left->right = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_CALL);
            if(!multiplier_left->right) return TREE_ERROR_OUT_OF_MEMORY;
            multiplier_left->right->left = (s_tree_node*) tree_create_node_function(TREE_FUNCTION_LN);
            if(!multiplier_left->right->left) return TREE_ERROR_OUT_OF_MEMORY;

            // ln(u)
            multiplier_left->right->right = tree_node_clone_deep(node->left);
            if(!multiplier_left->right->right) return TREE_ERROR_OUT_OF_MEMORY;

            // ((v/u)*u')
            multiplier->right = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_MULTIPLY);
            if(!multiplier->right) return TREE_ERROR_OUT_OF_MEMORY;
            s_tree_node* multiplier_right = multiplier->right;

            // (v/u)
            multiplier_right->left = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_DIVIDE);
            if(!multiplier_left->right->left) return TREE_ERROR_OUT_OF_MEMORY;
            multiplier_right->left->left = tree_node_clone_deep(node->right);
            if(!multiplier_right->left->left) return TREE_ERROR_OUT_OF_MEMORY;
            multiplier_right->left->right = tree_node_clone_deep(node->left);
            if(!multiplier_right->left->right) return TREE_ERROR_OUT_OF_MEMORY;

            // u'
            return tree_subtree_derivative(&node->left, &multiplier_right->right, variable_index, context);
        }
        // (c^x)' = x'*(ln(c)*c^x)

        // x'
        if((error = tree_subtree_derivative(&node->right, &target->left, variable_index, context))) return error;

        // (ln(c)*c^x)
        target->right = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_MULTIPLY);
        if(!target->right) return TREE_ERROR_OUT_OF_MEMORY;
        s_tree_node* derivative = target->right;

        // c^x
        derivative->right = tree_node_clone_deep(node);
        if(!derivative->right) return TREE_ERROR_OUT_OF_MEMORY;

        // ln(c)
        derivative->left = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_CALL);
        if(!derivative->left) return TREE_ERROR_OUT_OF_MEMORY;
        derivative->left->left = (s_tree_node*) tree_create_node_function(TREE_FUNCTION_LN);
        if(derivative->left->left) return TREE_ERROR_OUT_OF_MEMORY;

        derivative->left->right = tree_node_clone_deep(node->left);
        if(!derivative->left->right) return TREE_ERROR_OUT_OF_MEMORY;

    } else {
        // (x^c)' = x'*(c*(x^(c-1)))

        // x'
        if((error = tree_subtree_derivative(&node->left, &target->left, variable_index, context))) return error;

        // (c*(x^(c-1)))
        target->right = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_MULTIPLY);
        if(!target->right) return TREE_ERROR_OUT_OF_MEMORY;
        s_tree_node* derivative = target->right;

        // c
        derivative->left = tree_node_clone_deep(node->right);
        if(!derivative->left) return TREE_ERROR_OUT_OF_MEMORY;

        // (x^(c-1))
        derivative->right  = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_POW);
        if(!derivative->right) return TREE_ERROR_OUT_OF_MEMORY;
        s_tree_node* right = derivative->right;

        right->left = tree_node_clone_deep(node->left);
        if(!right->left) return TREE_ERROR_OUT_OF_MEMORY;

        // c-1

        // checking common case if c is a number
        if(node->right->type == TREE_NODE_TYPE_NUMBER) {
            s_tree_node_number* number = (s_tree_node_number*) node->right;
            right->right = (s_tree_node*) tree_create_number_from_double((double) number->float_value - 1);
            if(!right->right) return TREE_ERROR_OUT_OF_MEMORY;

        } else {
            right->right = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_MINUS);
            if(!right->right) return TREE_ERROR_OUT_OF_MEMORY;

            s_tree_node* indicator = right->right;
            indicator->left = tree_node_clone_deep(node->right);
            if(!indicator->left) return TREE_ERROR_OUT_OF_MEMORY;
            indicator->right = (s_tree_node*) tree_create_number_from_double(1);
            if(!indicator->right) return TREE_ERROR_OUT_OF_MEMORY;
        }
    }
    return error;
}

e_tree_error tree_node_sqrt_derivative(s_tree_node** node_link, s_tree_node** target_link, unsigned long variable_index, s_simplifier_context* context) {
    s_tree_node* node = *node_link;

    // sqrt(x)' = x'/(2*sqrt(x))

    e_tree_error error = TREE_OK;

    *target_link = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_DIVIDE);
    s_tree_node* target = *target_link;
    if(!target) return TREE_ERROR_OUT_OF_MEMORY;

    // x'
    if((error = tree_subtree_derivative(&node->right, &target->left, variable_index, context))) return error;

    target->right = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_MULTIPLY);
    if(!target->right) return TREE_ERROR_OUT_OF_MEMORY;
    s_tree_node* derivative = target->right;

    // 2
    derivative->left = (s_tree_node*) tree_create_number_from_double(2);
    if(!derivative->left) return TREE_ERROR_OUT_OF_MEMORY;

    // sqrt(x)

    derivative->right = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_CALL);
    if(!derivative->right) return TREE_ERROR_OUT_OF_MEMORY;
    derivative->right->left = (s_tree_node*) tree_create_node_function(TREE_FUNCTION_SQRT);
    if(!derivative->right->left) return TREE_ERROR_OUT_OF_MEMORY;
    derivative->right->right = tree_node_clone_deep(node->right);
    if(!derivative->right->right) return TREE_ERROR_OUT_OF_MEMORY;

    return error;
}

e_tree_error tree_node_function_derivative(s_tree_node** node_link, s_tree_node** target_link, unsigned long variable_index, s_simplifier_context* context) {
    s_tree_node* node = *node_link;

    *target_link = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_MULTIPLY);
    s_tree_node* target = *target_link;
    if(!target) return TREE_ERROR_OUT_OF_MEMORY;

    e_tree_error error = TREE_OK;

    if((error = tree_subtree_derivative(&node->right, &target->left, variable_index, context))) return error;

    e_tree_function function_type = ((s_tree_node_function*)node->left)->function;

    if(function_type == TREE_FUNCTION_LN) {

        target->right = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_DIVIDE);
        if(!target->right) return TREE_ERROR_OUT_OF_MEMORY;
        s_tree_node* function_derivative = target->right;
        // (ln(x))' = 1/x
        function_derivative->left = (s_tree_node*) tree_create_number_from_double(1);
        if(!function_derivative->left) return TREE_ERROR_OUT_OF_MEMORY;
        function_derivative->right = tree_node_clone_deep(node->right);
        if(!function_derivative->right) return TREE_ERROR_OUT_OF_MEMORY;
        return TREE_OK;
    }
    if(function_type == TREE_FUNCTION_SIN) {

        target->right = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_CALL);
        if(!target->right) return TREE_ERROR_OUT_OF_MEMORY;
        s_tree_node* function_derivative = target->right;

        function_derivative->left = (s_tree_node*) tree_create_node_function(TREE_FUNCTION_COS);
        if(!function_derivative->left) return TREE_ERROR_OUT_OF_MEMORY;

        function_derivative->right = tree_node_clone_deep(node->right);
        if(!function_derivative->right) return TREE_ERROR_OUT_OF_MEMORY;
    }
    if(function_type == TREE_FUNCTION_COS) {

        target->right = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_MULTIPLY);
        if(!target->right) return TREE_ERROR_OUT_OF_MEMORY;
        s_tree_node* function_derivative = target->right;

        function_derivative->left = (s_tree_node*) tree_create_number_from_double(-1);
        if(!function_derivative->left) return TREE_ERROR_OUT_OF_MEMORY;

        function_derivative->right = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_CALL);
        if(!function_derivative->right) return TREE_ERROR_OUT_OF_MEMORY;

        function_derivative->right->left = (s_tree_node*) tree_create_node_function(TREE_FUNCTION_SIN);
        if(!function_derivative->right->left) return TREE_ERROR_OUT_OF_MEMORY;

        function_derivative->right->right = tree_node_clone_deep(node->right);
        if(!function_derivative->right->right) return TREE_ERROR_OUT_OF_MEMORY;
    }

    return error;
}

e_tree_error tree_node_division_derivative(s_tree_node** node_link, s_tree_node** target_link, unsigned long variable_index, s_simplifier_context* context) {
    s_tree_node* node = *node_link;

    e_tree_error error = TREE_OK;

    s_tree_node* numerator = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_MINUS);
    if(!numerator) return TREE_ERROR_OUT_OF_MEMORY;

    s_tree_node* denominator = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_POW);
    if(!denominator) return TREE_ERROR_OUT_OF_MEMORY;

    *target_link = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_DIVIDE);
    s_tree_node* target = *target_link;
    if(!target) return TREE_ERROR_OUT_OF_MEMORY;

    target->left = numerator;
    target->right = denominator;

    numerator->left = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_MULTIPLY);
    if(!numerator->left) return TREE_ERROR_OUT_OF_MEMORY;

    numerator->right = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_MULTIPLY);
    if(!numerator->right) return TREE_ERROR_OUT_OF_MEMORY;

    numerator->right->left = tree_node_clone_deep(node->left);
    if(!numerator->right->left) return TREE_ERROR_OUT_OF_MEMORY;

    numerator->left->right = tree_node_clone_deep(node->right);
    if(!numerator->left->right) return TREE_ERROR_OUT_OF_MEMORY;

    if((error = tree_subtree_derivative(&node->right, &numerator->right->right, variable_index, context))) return error;
    if((error = tree_subtree_derivative(&node->left, &numerator->left->left, variable_index, context))) return error;

    denominator->left = tree_node_clone_deep(node->right);
    if(!denominator->left) return TREE_ERROR_OUT_OF_MEMORY;

    denominator->right = (s_tree_node*) tree_create_number_from_double(2);
    if(!denominator->right) return TREE_ERROR_OUT_OF_MEMORY;

    return error;
}

e_tree_error tree_node_multiplication_derivative(s_tree_node** node_link, s_tree_node** target_link, unsigned long variable_index, s_simplifier_context* context) {
    s_tree_node* node = *node_link;

    e_tree_error error = TREE_OK;

    if(node->left->type == TREE_NODE_TYPE_NUMBER && node->right->type == TREE_NODE_TYPE_NUMBER) {
        *target_link = (s_tree_node*) tree_create_number_from_double(0);
        if(!*target_link) return TREE_ERROR_OUT_OF_MEMORY;
        return TREE_OK;
    }

    if(node->left->type == TREE_NODE_TYPE_NUMBER || node->right->type == TREE_NODE_TYPE_NUMBER) {
        s_tree_node_number* number = NULL;
        s_tree_node* not_number = NULL;

        *target_link = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_MULTIPLY);
        s_tree_node* target = *target_link;

        if(node->left->type == TREE_NODE_TYPE_NUMBER) {
            number = (s_tree_node_number*) node->left;
            not_number = node->right;
        } else {
            not_number = node->left;
            number = (s_tree_node_number*) node->right;
        }

        target->left = (s_tree_node*) tree_create_number_from_double((double) number->float_value);
        if((error = tree_subtree_derivative(&not_number, &target->right, variable_index, context))) return error;

        return TREE_OK;
    }

    *target_link = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_PLUS);
    s_tree_node* target = *target_link;
    if(!target) return TREE_ERROR_OUT_OF_MEMORY;

    target->right = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_MULTIPLY);
    if(!target->right) return TREE_ERROR_OUT_OF_MEMORY;

    target->left = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_MULTIPLY);
    if(!target->left) return TREE_ERROR_OUT_OF_MEMORY;

    target->left->left = tree_node_clone_deep(node->left);
    target->right->right = tree_node_clone_deep(node->right);

    if(!target->left->left || !target->right->right) return TREE_ERROR_OUT_OF_MEMORY;

    if((error = tree_subtree_derivative(&node->right, &target->left->right, variable_index, context))) return error;
    if((error = tree_subtree_derivative(&node->left, &target->right->left, variable_index, context))) return error;

    return error;
}

e_tree_error tree_subtree_derivative(s_tree_node** node_link, s_tree_node** target, unsigned long variable_index, s_simplifier_context* context) {
    s_tree_node* subtree = *node_link;

    if(subtree == NULL) return TREE_ERROR_NULL;

    if(subtree->type == TREE_NODE_TYPE_FUNCTION) {
        return TREE_ERROR_CANNOT_TAKE_DERIVATIVE;
    }

    if(!simplify_context_can_perform_action(context)) {
        subtree = tree_node_clone_deep(subtree);
        if(!subtree) return TREE_ERROR_OUT_OF_MEMORY;
        return tree_node_wrap_derivative(&subtree, target, variable_index);
    }

    simplify_context_perform_action(context);

    if(subtree->type == TREE_NODE_TYPE_NUMBER) {
        *target = (s_tree_node*) tree_create_number_from_double(0);
        if(!*target) return TREE_ERROR_OUT_OF_MEMORY;
        return TREE_OK;
    }
    if(subtree->type == TREE_NODE_TYPE_VARIABLE) {
        if(((s_tree_node_variable*)subtree)->variable_index == variable_index) {
            *target = (s_tree_node*) tree_create_number_from_double(1);
        } else {
            *target = (s_tree_node*) tree_create_number_from_double(0);
        }
        if(!*target) return TREE_ERROR_OUT_OF_MEMORY;
        return TREE_OK;
    }

    if(subtree->type == TREE_NODE_TYPE_OPERATOR) {
        s_tree_node_operator* operator = (s_tree_node_operator*) subtree;

        switch(operator->operator_type) {
            case TREE_OPERATOR_TYPE_PLUS:
            case TREE_OPERATOR_TYPE_MINUS:
                *target = (s_tree_node*) tree_create_node_operator(operator->operator_type);
                if(!*target) return TREE_ERROR_OUT_OF_MEMORY;

                if(subtree->left) {
                    e_tree_error error = TREE_OK;
                    if((error = tree_subtree_derivative(&subtree->left, &(*target)->left, variable_index, context)))
                        return error;
                } else (*target)->left = NULL;

                return tree_subtree_derivative(&subtree->right, &(*target)->right, variable_index, context);
            case TREE_OPERATOR_TYPE_MULTIPLY:
                return tree_node_multiplication_derivative(&subtree, target, variable_index, context);
            case TREE_OPERATOR_TYPE_DIVIDE:
                return tree_node_division_derivative(&subtree, target, variable_index, context);
            case TREE_OPERATOR_TYPE_POW:
                return tree_node_pow_derivative(&subtree, target, variable_index, context);
            case TREE_OPERATOR_TYPE_CALL:
                if(subtree->left->type == TREE_NODE_TYPE_FUNCTION) {
                    s_tree_node_function* function = (s_tree_node_function*)subtree->left;
                    switch(function->function) {
                        case TREE_FUNCTION_SIN:
                        case TREE_FUNCTION_COS:
                        case TREE_FUNCTION_LN:
                            return tree_node_function_derivative(&subtree, target, variable_index, context);
                        case TREE_FUNCTION_SQRT:
                            return tree_node_sqrt_derivative(&subtree, target, variable_index, context);
                    }
                }

                return TREE_ERROR_CANNOT_TAKE_DERIVATIVE;
            case TREE_OPERATOR_TYPE_DIFFERENTIAL:
                return TREE_ERROR_CANNOT_TAKE_DERIVATIVE;
        }
    }

    return TREE_OK;
}

bool tree_is_differential(s_tree_node* expression) {
    if(!expression) return false;
    if(expression->type != TREE_NODE_TYPE_OPERATOR) return false;

    s_tree_node_operator* operator = (s_tree_node_operator*) expression;

    if(operator->operator_type != TREE_OPERATOR_TYPE_DIVIDE) return false;
    if(expression->left->type != TREE_NODE_TYPE_OPERATOR) return false;
    if(expression->right->type != TREE_NODE_TYPE_OPERATOR) return false;
    if(((s_tree_node_operator*)expression->left)->operator_type != TREE_OPERATOR_TYPE_DIFFERENTIAL) return false;
    if(((s_tree_node_operator*)expression->right)->operator_type != TREE_OPERATOR_TYPE_DIFFERENTIAL) return false;
    if(expression->left->right != NULL) return false;
    if(expression->right->right == NULL) return false;
    return true;
}

unsigned long tree_find_differential_variable(s_tree_node* expression) {
    if(!tree_is_differential(expression)) return -1;
    if(expression->right->right->type != TREE_NODE_TYPE_VARIABLE) return -1;
    return ((s_tree_node_variable*)expression->right->right)->variable_index;
}

bool tree_node_contains_differential(s_tree_node* expression) {
    if(expression == NULL) return false;
    if(expression->type == TREE_NODE_TYPE_OPERATOR) {
        if(tree_node_is_operator(expression, TREE_OPERATOR_TYPE_DIFFERENTIAL)) return true;
        if(expression->left && tree_node_contains_differential(expression->left)) return true;
        if(expression->right && tree_node_contains_differential(expression->right)) return true;
    }
    return false;
}

// MARK: Value distribution

e_tree_error tree_node_wrap_multiply(s_tree_node* value, s_tree_node* amplifier, s_tree_node** target) {
    e_tree_error error = TREE_OK;

    *target = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_MULTIPLY);
    s_tree_node* multiplier = *target;
    if(!multiplier) return TREE_ERROR_OUT_OF_MEMORY;
    multiplier->left = amplifier;
    multiplier->right = value;

    return error;
}

// Distributes a*(b+c) into (a*b + a*c)
// value is a, group is (b+c)
e_tree_error tree_node_distribute_value(s_tree_node* value, s_tree_node* group, s_simplifier_context* context) {
    if(!tree_node_is_operator(group, TREE_OPERATOR_TYPE_PLUS)) return TREE_OK;
    e_tree_error error = TREE_OK;

    s_tree_node* runner = group;

    while(runner->right && tree_node_is_operator(runner->right, TREE_OPERATOR_TYPE_PLUS)) {
        if(!simplify_context_can_perform_action(context)) {
            return TREE_OK;
        }

        s_tree_node* member = runner->left;
        runner->left = NULL;
        if((error = tree_node_wrap_multiply(member, tree_node_clone_deep(value), &runner->left))) return error;

        runner = runner->right;
    }

    if(tree_node_is_operator(runner, TREE_OPERATOR_TYPE_PLUS)) {
        s_tree_node* member = runner->left;
        runner->left = NULL;
        if((error = tree_node_wrap_multiply(member, tree_node_clone_deep(value), &runner->left))) return error;
        member = runner->right;
        runner->right = NULL;
        if((error = tree_node_wrap_multiply(member, tree_node_clone_deep(value), &runner->right))) return error;
    }

    return error;
}

// MARK: Member reordering/sorting

bool tree_node_reorder_members(s_tree_node* expression) {
    if(expression->type != TREE_NODE_TYPE_OPERATOR) return false;

    s_tree_node_operator* operator = (s_tree_node_operator*) expression;

    if(!(TREE_NODE_OPERATOR_FLAGS[operator->operator_type] & TREE_OPERATOR_FLAG_ASSOCIATIVE)) return false;
    e_tree_node_operator_type type = operator->operator_type;

    s_tree_node* left_leap_parent = expression;
    s_tree_node* left_leap = left_leap_parent->left;

    bool moved = false;

    if(left_leap) {
        while(left_leap->left) {
            if(!tree_node_is_operator(left_leap, type)) break;
            left_leap_parent = left_leap;
            left_leap = left_leap->left;
        }

        if(left_leap_parent != expression) {
            s_tree_node* excessive = expression->right;
            expression->right = expression->left;
            left_leap_parent->left = left_leap_parent->right;
            left_leap_parent->right = excessive;
            expression->left = left_leap;
            moved = true;
        }
    }

    if(tree_node_is_operator(expression->right, type)) {
        if(tree_node_reorder_members(expression->right)) moved = true;
    }
    return moved;
}

int tree_node_compare(s_tree_node* left, s_tree_node* right) {
    return tree_node_compare_signed(left, right, false);
}

int tree_node_compare_signed(s_tree_node* left, s_tree_node* right, bool ignore_sign) {
    if(ignore_sign) {
        while(tree_node_is_operator(right, TREE_OPERATOR_TYPE_MINUS) && !right->left) {
            right = right->right;
        }
        while(tree_node_is_operator(left, TREE_OPERATOR_TYPE_MINUS) && !left->left) {
            left = left->right;
        }
    }

    if(left->type != right->type) {
        return ((int)left->type) - ((int)right->type);
    }
    if(left->type == TREE_NODE_TYPE_NUMBER) {
        s_tree_node_number* left_number = (s_tree_node_number*) left;
        s_tree_node_number* right_number = (s_tree_node_number*) right;

        if(left_number->float_value > right_number->float_value) return 1;
        if(left_number->float_value < right_number->float_value) return -1;
        return 0;
    }
    if(left->type == TREE_NODE_TYPE_FUNCTION) {
        s_tree_node_function* left_function = (s_tree_node_function*) left;
        s_tree_node_function* right_function = (s_tree_node_function*) right;

        return ((int)left_function->function) - ((int)right_function->function);
    }
    if(left->type == TREE_NODE_TYPE_VARIABLE) {
        s_tree_node_variable* left_variable = (s_tree_node_variable*) left;
        s_tree_node_variable* right_variable = (s_tree_node_variable*) right;

        return ((int)left_variable->variable_index) - ((int)right_variable->variable_index);
    }
    if(left->type == TREE_NODE_TYPE_OPERATOR) {

        s_tree_node_operator* left_operator = (s_tree_node_operator*) left;
        s_tree_node_operator* right_operator = (s_tree_node_operator*) right;

        if(left_operator->operator_type != right_operator->operator_type) {
            return ((int)left_operator->operator_type) - ((int)right_operator->operator_type);
        }
        if(left->left && !right->left) return -1;
        if(left->right && !right->right) return -1;
        if(right->left && !left->left) return 1;
        if(right->right && !left->right) return 1;

        if(left->left) {
            int left_subtree_comparsion = tree_node_compare(left->left, right->left);
            if(left_subtree_comparsion != 0) return left_subtree_comparsion;
        }
        if(left->right) {
            int right_subtree_comparsion = tree_node_compare(left->right, right->right);
            if(right_subtree_comparsion != 0) return right_subtree_comparsion;
        }
    }
    return 0;
}

e_tree_error tree_node_sort_members(s_tree_node* expression, s_simplifier_context* context) {
    if(expression->type != TREE_NODE_TYPE_OPERATOR) return TREE_OK;

    s_tree_node_operator* operator = (s_tree_node_operator*) expression;

    if(!(TREE_NODE_OPERATOR_FLAGS[operator->operator_type] & TREE_OPERATOR_FLAG_ASSOCIATIVE)) return TREE_OK;
    // The expression is considered to have been already ordered
    // with tree_node_reorder_members function

    e_tree_node_operator_type type = operator->operator_type;

    bool moved = false;

    // Bubblesort

    int limit = -1;

    while(limit != 0) {
        s_tree_node* runner = expression;

        int path_length = 0;

        // Swapping nested sequence elements

        while(runner->right && tree_node_is_operator(runner->right, type)) {
            if(limit != -1 && path_length == limit) {
                break;
            }

            if(tree_node_compare(runner->left, runner->right->left) < 0) {
                s_tree_node* buffer = runner->left;
                runner->left = runner->right->left;
                runner->right->left = buffer;
                moved = true;
            }
            runner = runner->right;
            path_length++;
        }

        // Swapping last sequence element

        if(path_length != limit && tree_node_compare(runner->left, runner->right) < 0) {
            s_tree_node* buffer = runner->left;
            runner->left = runner->right;
            runner->right = buffer;
            moved = true;
        }

        if(limit == -1) limit = path_length;
        else limit--;
    }

    if(moved) {
        simplify_context_perform_action(context);
    }

    return TREE_OK;
}

e_tree_error tree_cite_merge_similar(s_tree_node_operator** node_link, bool is_nested, s_simplifier_context* context) {

    s_tree_node_operator* node = *node_link;

    s_tree_node* left = node->tree_node.left;
    s_tree_node* right = NULL;

    if(is_nested) {
        right = node->tree_node.right->left;
    } else {
        right = node->tree_node.right;
    }

    e_tree_node_operator_type group_operator = 0;
    if(node->operator_type == TREE_OPERATOR_TYPE_PLUS) {
        // a+a => a*2
        // (a*2)+a => a*3
        group_operator = TREE_OPERATOR_TYPE_MULTIPLY;
    } else if(node->operator_type == TREE_OPERATOR_TYPE_MULTIPLY) {
        // a*a => a^2
        // (a^2)*a => a^3
        group_operator = TREE_OPERATOR_TYPE_POW;
    } else {
        return TREE_OK;
    }

    // a*b or a+b

    if( (left->type == TREE_NODE_TYPE_NUMBER  || (tree_node_is_unary_minus(left) && left->right->type == TREE_NODE_TYPE_NUMBER)) &&
        (right->type == TREE_NODE_TYPE_NUMBER || (tree_node_is_unary_minus(right) && right->right->type == TREE_NODE_TYPE_NUMBER))) {

        s_tree_node_number *left_number = (s_tree_node_number*)  (tree_node_is_unary_minus(left)  ? left->right  : left);
        s_tree_node_number *right_number = (s_tree_node_number*) (tree_node_is_unary_minus(right) ? right->right : right);

        double right_value = (double)right_number->float_value;

        if(node->operator_type == TREE_OPERATOR_TYPE_MULTIPLY) {
            if(tree_node_is_unary_minus(right)) right_value = -right_value;
            left_number->float_value *= right_value;
        } else {
            if(tree_node_is_unary_minus(left) != tree_node_is_unary_minus(right)) {
                right_value = -right_value;
            }
            left_number->float_value += right_value;
        }

        if(left_number->float_value < 0) {
            left_number->float_value = -left_number->float_value;
            if(tree_node_is_unary_minus(left)) {
                tree_node_sift_right(&node->tree_node.left);
                left = node->tree_node.left;
            } else {
                tree_release_subtree((s_tree_node*) right_number);
                node->tree_node.left = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_MINUS);
                left = node->tree_node.left;
                if(!left) return TREE_ERROR_OUT_OF_MEMORY;
                left->right = (s_tree_node*) left_number;

            }
        }

        if(is_nested) tree_node_sift_right(&node->tree_node.right);
        else tree_node_sift_left((s_tree_node**)node_link);

        simplify_context_perform_action(context);
    } else if(tree_node_compare(left, right) == 0) {
        // a equals b
        // converting it to a*2 or a^2

        s_tree_node* power = (s_tree_node*) tree_create_node_operator(group_operator);
        if(!power) return TREE_ERROR_OUT_OF_MEMORY;
        power->left = node->tree_node.left;

        power->right = (s_tree_node*) tree_create_number_from_double(2);
        if(!power->right) return TREE_ERROR_OUT_OF_MEMORY;
        node->tree_node.left = power;

        if(is_nested) tree_node_sift_right(&node->tree_node.right);
        else tree_node_sift_left((s_tree_node**) node_link);
        simplify_context_perform_action(context);
    } else if(tree_node_is_operator(left, group_operator) || tree_node_is_operator(right, group_operator)) {
        if(tree_node_is_operator(left, group_operator) && tree_node_is_operator(right, group_operator)) {

            // having either:
            // (a^p)*(c^d)
            // (a*p)+(c*d)
            // b, d - numbers

            if(tree_node_compare(left->left, right->left) == 0) {
                // a equals c
                // converting it to a^(b+d) or a*(b+d)

                if(left->right->type == TREE_NODE_TYPE_NUMBER && right->right->type == TREE_NODE_TYPE_NUMBER) {

                    s_tree_node_number *left_number = (s_tree_node_number*)  left->right;
                    s_tree_node_number *right_number = (s_tree_node_number*) right->right;

                    left_number->float_value += right_number->float_value;
                    if(is_nested) tree_node_sift_right(&node->tree_node.right);
                    else tree_node_sift_left((s_tree_node**) node_link);
                    simplify_context_perform_action(context);
                } else {
                    s_tree_node* power_sum = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_PLUS);
                    if(!power_sum) return TREE_ERROR_OUT_OF_MEMORY;

                    power_sum->left = left->right;
                    power_sum->right = right->right;
                    right->right = NULL; // Prevent this node from being freed by tree_node_sift_*
                    left->right = power_sum;

                    if(is_nested) tree_node_sift_right(&node->tree_node.right);
                    else tree_node_sift_left((s_tree_node**) node_link);
                    simplify_context_perform_action(context);
                }
            }
        } else {

            // having either:
            // (a^p)*b || a*(b^p)
            // (a*p)+b || a+(b*p)
            // b, d - numbers

            bool is_right_group = tree_node_is_operator(right, group_operator);

            if(is_right_group && tree_node_compare(right->left, left) == 0) {
                s_tree_node* buffer = right;
                right = left;
                node->tree_node.left = buffer;
                if(is_nested) node->tree_node.right->left = left;
                else node->tree_node.right = left;
                left = buffer;
                is_right_group = false;
            }

            // reduced to (a^p)*b or (a*p)+b case here

            if(!is_right_group && tree_node_compare(left->left, right) == 0) {

                // a equals b, so we have (a^p)*a or (a*p)+a
                // converting it to a^(p+1) or a*(p+1)

                if(left->right->type == TREE_NODE_TYPE_NUMBER) {
                    ((s_tree_node_number*)left->right)->float_value += 1;
                } else {
                    s_tree_node* power_sum = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_PLUS);
                    if(!power_sum) return TREE_ERROR_OUT_OF_MEMORY;

                    power_sum->left = left->right;
                    power_sum->right = (s_tree_node*) tree_create_number_from_double(1);
                    if(!power_sum->right) return TREE_ERROR_OUT_OF_MEMORY;
                    left->right = power_sum;
                }

                if(is_nested) tree_node_sift_right(&node->tree_node.right);
                else tree_node_sift_left((s_tree_node**) node_link);

                simplify_context_perform_action(context);
            }
        }
    }

    node = *node_link;

    if(tree_node_is_operator((s_tree_node*) node, TREE_OPERATOR_TYPE_PLUS)) {
        if(tree_node_is_operator(left, TREE_OPERATOR_TYPE_DIVIDE) && tree_node_is_operator(right, TREE_OPERATOR_TYPE_DIVIDE)) {

            // Simplifying a/c + b/c to (a+b)/c

            if(tree_node_compare(left->right, right->right) == 0) {
                s_tree_node* numerator_sum = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_PLUS);
                if(!numerator_sum) return TREE_ERROR_OUT_OF_MEMORY;
                numerator_sum->left = left->left;
                numerator_sum->right = right->left;
                left->left = numerator_sum;
                right->left = NULL;

                if(is_nested) tree_node_sift_right(&node->tree_node.right);
                else tree_node_sift_left((s_tree_node**) node_link);

                simplify_context_perform_action(context);
            }
        }
    }

    return TREE_OK;
}

e_tree_error tree_cite_similar_members(s_tree_node** expression_link, s_simplifier_context* context) {
    if((*expression_link)->type != TREE_NODE_TYPE_OPERATOR) return TREE_OK;

    s_tree_node_operator** operator_link = (s_tree_node_operator**) expression_link;

    if(!(TREE_NODE_OPERATOR_FLAGS[(*operator_link)->operator_type] & TREE_OPERATOR_FLAG_ASSOCIATIVE)) return TREE_OK;

    // The expression is considered to have been already sorted
    // with tree_node_sort_members function

    e_tree_node_operator_type type = (*operator_link)->operator_type;
    e_tree_error error = TREE_OK;

    s_tree_node_operator** runner = operator_link;

    while((*runner)->tree_node.right && tree_node_is_operator((*runner)->tree_node.right, type)) {
        if(!simplify_context_can_perform_action(context)) {
            return TREE_OK;
        }

        if((error = tree_cite_merge_similar(runner, true, context))) return error;
        runner = (s_tree_node_operator**) &(*runner)->tree_node.right;
    }

    if(tree_node_is_operator((s_tree_node*) *runner, type)) {
        if((error = tree_cite_merge_similar(runner, false, context))) return error;
    }

    return error;
}

// MARK: Member power pool

void member_degree_pool_init(s_member_degree_pool* pool, int capacity) {
    pool->members_checked = 0;
    pool->members_count = capacity;
    pool->powers = calloc(pool->members_count, sizeof(s_tree_node));
}

void member_degree_pool_release(s_member_degree_pool* pool) {
    if(pool->powers) free(pool->powers);
}

void member_degree_pool_reset(s_member_degree_pool* pool) {
    pool->members_checked = 0;
}

void member_degree_pool_add(s_member_degree_pool* pool, s_tree_node* node) {
    pool->powers[pool->members_checked] = node;
    pool->members_checked++;
}

e_tree_error member_degree_pool_get_degree(s_member_degree_pool* pool, s_tree_node** result) {
    e_tree_error error = TREE_OK;

    s_tree_node* current_power = NULL;

    for(int i = 0; i < pool->members_count; i++) {
        if(pool->powers[i]) {
            pool->powers[i] = tree_node_clone_deep(pool->powers[i]);
        } else {
            pool->powers[i] = (s_tree_node*) tree_create_number_from_double(1);
        }
        if(!pool->powers[i]) return TREE_ERROR_OUT_OF_MEMORY;
        if((error = tree_power_wrap_terms_if_needed(&pool->powers[i]))) return error;
    }
    current_power = tree_node_clone_deep(pool->powers[0]);
    if(!current_power) return TREE_ERROR_OUT_OF_MEMORY;

    for(int i = 1; i < pool->members_count; i++) {
        tree_node_intersect(&current_power, pool->powers[i]);
    }

    s_tree_node** runner = &current_power;

    while((*runner)->right && tree_node_is_operator((*runner)->right, TREE_OPERATOR_TYPE_PLUS)) {
        if(tree_node_is_exact_number((*runner)->left->right, 0)) {
            tree_node_sift_right(runner);
        } else {
            runner = &(*runner)->right;
        }
    }

    if(tree_node_is_operator(*runner, TREE_OPERATOR_TYPE_PLUS)) {
        if(tree_node_is_exact_number((*runner)->left->right, 0)) tree_node_sift_right(runner);
    }
    if(tree_node_is_operator(*runner, TREE_OPERATOR_TYPE_PLUS)) {
        if(tree_node_is_exact_number((*runner)->right->right, 0)) tree_node_sift_left(runner);
        *result = tree_node_clone_deep(current_power);
    } else {
        if(tree_node_is_exact_number((*runner)->right, 0)) {
            tree_release_subtree(*runner);
            *result = NULL;
        } else {
            *result = tree_node_clone_deep(*runner);
        }
    }

    for(int i = 0; i < pool->members_count; i++) {
        tree_release_subtree(pool->powers[i]);
    }

    return error;
}

// MARK: Moving members away from braces

// for term=3b, base=b returns true, factor will be set to 3
bool tree_node_factor_of_term(s_tree_node* base, s_tree_node* term, s_tree_node** factor) {
    if(tree_node_compare(base->left, term->left) == 0) {
        *factor = base->right;
        return true;
    }
    return false;
}

// for expression=(2a+3b+4c), term_base=b, returns 3
bool tree_node_factor_of_term_in_expression(s_tree_node* expression, s_tree_node* term_base, s_tree_node** result) {
    if(tree_node_is_operator(expression, TREE_OPERATOR_TYPE_PLUS)) {
        s_tree_node* runner = expression;

        while(runner->right && tree_node_is_operator(runner->right, TREE_OPERATOR_TYPE_PLUS)) {
            if(tree_node_factor_of_term(runner->left, term_base, result)) return true;
            runner = runner->right;
        }

        if(tree_node_is_operator(runner, TREE_OPERATOR_TYPE_PLUS)) {
            if(tree_node_factor_of_term(runner->left, term_base, result)) return true;
            if(tree_node_factor_of_term(runner->right, term_base, result)) return true;
        }
    } else {
        if(tree_node_factor_of_term(expression, term_base, result)) return true;
    }
    return false;
}

e_tree_error tree_power_wrap_terms_if_needed(s_tree_node** node) {
    e_tree_error error = TREE_OK;

    if(tree_node_is_operator(*node, TREE_OPERATOR_TYPE_PLUS)) {
        s_tree_node* runner = *node;

        while(runner->right && tree_node_is_operator(runner->right, TREE_OPERATOR_TYPE_PLUS)) {
            if((error = tree_power_wrap_term_if_needed(&runner->left))) return error;
            runner = runner->right;
        }

        if(tree_node_is_operator(runner, TREE_OPERATOR_TYPE_PLUS)) {
            if((error = tree_power_wrap_term_if_needed(&runner->left))) return error;
            if((error = tree_power_wrap_term_if_needed(&runner->right))) return error;
        }
    } else {
        tree_power_wrap_term_if_needed(node);
    }
    return TREE_OK;
}

void tree_node_intersect(s_tree_node** sum_link, s_tree_node* mask) {
    s_tree_node* sum = *sum_link;

    if(tree_node_is_operator(sum, TREE_OPERATOR_TYPE_PLUS)) {
        s_tree_node** runner = sum_link;
        s_tree_node* factor = NULL;

        while((*runner)->right && tree_node_is_operator((*runner)->right, TREE_OPERATOR_TYPE_PLUS)) {
            if(tree_node_factor_of_term_in_expression(mask, (*runner)->left, &factor)) {
                double right_value = (double)(((s_tree_node_number*)(*runner)->left->right)->float_value);
                double left_value = tree_subtree_constant_value(factor);

                if((right_value > 0) != (left_value > 0)) {
                    tree_node_sift_right(runner);
                } else if(fabs(left_value) < fabs(right_value)) {
                    tree_release_subtree((*runner)->left->right);
                    (*runner)->left->right = tree_node_clone_deep(factor);
                }
                runner = &(*runner)->right;
            } else {
                tree_node_sift_right(runner);
            }
        }

        if(tree_node_is_operator(*runner, TREE_OPERATOR_TYPE_PLUS)) {
            s_tree_node_number* current_factor = (s_tree_node_number*)(*runner)->left->right;

            if(tree_node_factor_of_term_in_expression(mask, (*runner)->left, &factor)) {
                double left_value = tree_subtree_constant_value(factor);
                double right_value = tree_subtree_constant_value(current_factor);

                if((right_value > 0) != (left_value > 0)) {
                    tree_release_subtree((*runner)->left->right);
                    (*runner)->left->right = tree_create_number_from_double(0.0);
                    // TODO handle calloc fail
                } else if(fabs(left_value) < fabs(right_value)) {
                    tree_release_subtree((*runner)->left->right);
                    (*runner)->left->right = tree_node_clone_deep(factor);
                }
            } else {
                tree_release_subtree((*runner)->left->right);
                (*runner)->left->right = tree_create_number_from_double(0.0);
                // TODO handle calloc fail
            }

            current_factor = (s_tree_node_number*)(*runner)->right->right;

            if(tree_node_factor_of_term_in_expression(mask, (*runner)->right, &factor)) {
                double left_value = tree_subtree_constant_value(factor);
                double right_value = tree_subtree_constant_value(current_factor);

                if((right_value > 0) != (left_value > 0)) {
                    tree_release_subtree((*runner)->right->right);
                    (*runner)->right->right = tree_create_number_from_double(0.0);
                    // TODO handle calloc fail
                } else if(fabs(left_value) < fabs(right_value)) {
                    tree_release_subtree((*runner)->right->right);
                    (*runner)->right->right = tree_node_clone_deep(factor);
                }
            } else {
                tree_release_subtree((*runner)->right->right);
                (*runner)->right->right = tree_create_number_from_double(0.0);
                // TODO handle calloc fail
            }
        }
    } else {
        s_tree_node* factor = NULL;

        s_tree_node_number* current_factor = (s_tree_node_number*)sum->right;

        if(tree_node_factor_of_term_in_expression(mask, sum, &factor)) {
            double left_value = tree_subtree_constant_value(factor);
            double right_value = tree_subtree_constant_value(current_factor);

            if((right_value > 0) != (left_value > 0)) {
                tree_release_subtree(sum->right);
                sum->right = tree_create_number_from_double(0.0);
                // TODO handle calloc fail
            } else if(fabs(tree_subtree_constant_value(factor)) < fabs(tree_subtree_constant_value(current_factor))) {
                tree_release_subtree(sum->right);
                sum->right = tree_node_clone_deep(factor);
            }
        } else {
            tree_release_subtree(sum->right);
            sum->right = tree_create_number_from_double(0.0);
            // TODO handle calloc fail
        }
    }
}

e_tree_error tree_power_wrap_term_if_needed(s_tree_node** node) {
    e_tree_error error = TREE_OK;
    s_tree_node* power = *node;

    if(tree_subtree_is_simple_value(power)) {
        if((error = tree_node_multiply_one_by_node(node))) return error;
        return TREE_OK;
    }

    if(tree_node_is_operator(power, TREE_OPERATOR_TYPE_MULTIPLY)) {
        if(tree_subtree_is_simple_value(power->right)) return TREE_OK;
    }

    if((error = tree_node_multiply_node_by_one(node))) return error;
    return TREE_OK;

    return error;
}

bool tree_can_move_factor_from_term(s_tree_node* expression, s_tree_node* member, s_member_degree_pool* pool) {
    s_tree_node* local_power = NULL;

    if(tree_node_is_operator(expression, TREE_OPERATOR_TYPE_DIVIDE)) {
        expression = expression->left;
    }

    if(tree_node_is_operator(expression, TREE_OPERATOR_TYPE_MULTIPLY)) {
        s_tree_node* runner = expression;

        while(runner->right && tree_node_is_operator(runner->right, TREE_OPERATOR_TYPE_MULTIPLY)) {
            if(tree_node_compare(member, runner->left) == 0) {
                member_degree_pool_add(pool, NULL);
                return true;
            }
            else if(tree_node_compare(member, *tree_get_pure_member(&runner->left, &local_power)) == 0) {
                member_degree_pool_add(pool, local_power);
                return true;
            }
            runner = runner->right;
        }

        if(tree_node_is_operator(runner, TREE_OPERATOR_TYPE_MULTIPLY)) {
            if(tree_node_compare(member, runner->left) == 0) {
                member_degree_pool_add(pool, NULL);
                return true;
            }
            else if(tree_node_compare(member, *tree_get_pure_member(&runner->left, &local_power)) == 0) {
                member_degree_pool_add(pool, local_power);
                return true;
            }
            if(tree_node_compare(member, runner->right) == 0) {
                member_degree_pool_add(pool, NULL);
                return true;
            }
            else if(tree_node_compare(member, *tree_get_pure_member(&runner->right, &local_power)) == 0) {
                member_degree_pool_add(pool, local_power);
                return true;
            }
        }
    } else {
        if(tree_node_compare(member, expression) == 0) {
            member_degree_pool_add(pool, NULL);
            return true;
        }
        else if(tree_node_compare(member, *tree_get_pure_member(&expression, &local_power)) == 0) {
            member_degree_pool_add(pool, local_power);
            return true;
        }
    }

    return false;
}

e_tree_error tree_remove_degreed_member_from_term(s_tree_node** term_link, s_tree_node** member, s_tree_node* member_degree, s_tree_node* reduce_degree) {
    s_tree_node* term = *term_link;
    e_tree_error error = TREE_OK;
    s_tree_node* expression_copy = NULL;

    s_tree_node* member_to_move = *member;
    if(member_to_move == term) {
        *member = (s_tree_node*) tree_create_number_from_double(1);
        if(!*member) return TREE_ERROR_OUT_OF_MEMORY;
    }

    *member = (s_tree_node*) tree_create_number_from_double(1);
    if(!*member) return TREE_ERROR_OUT_OF_MEMORY;

    expression_copy = term;

    *term_link = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_MULTIPLY);
    term = *term_link;
    if(!term) return TREE_ERROR_OUT_OF_MEMORY;
    term->left = expression_copy;

    term->right = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_POW);
    if(!term->right) return TREE_ERROR_OUT_OF_MEMORY;

    s_tree_node* reduced_member_degree = term->right;
    reduced_member_degree->left = member_to_move;

    if(member_to_move == expression_copy) {
        term->left = (s_tree_node*) tree_create_number_from_double(1);
        if(!term->left) return TREE_ERROR_OUT_OF_MEMORY;
    }

    reduced_member_degree->right = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_MINUS);
    if(!reduced_member_degree->right) return TREE_ERROR_OUT_OF_MEMORY;

    if(member_degree) {
        reduced_member_degree->right->left = tree_node_clone_deep(member_degree);
    } else {
        reduced_member_degree->right->left = (s_tree_node*) tree_create_number_from_double(1);
    }
    if(!reduced_member_degree->right->left) return TREE_ERROR_OUT_OF_MEMORY;

    reduced_member_degree->right->right = tree_node_clone_deep(reduce_degree);
    if(!reduced_member_degree->right->right) return TREE_ERROR_OUT_OF_MEMORY;

    return error;
}

e_tree_error tree_remove_factor_from_term(s_tree_node** expression_link, s_tree_node* degree, s_tree_node* member, bool* success) {
    s_tree_node* expression = *expression_link;
    bool local_success = false;
    if(!success) success = &local_success;

    e_tree_error error = TREE_OK;
    s_tree_node* term_degree = NULL;
    s_tree_node** real_expression = tree_get_pure_member(expression_link, &term_degree);

    if(tree_node_is_operator(*real_expression, TREE_OPERATOR_TYPE_MULTIPLY)) {
        s_tree_node* runner = *real_expression;

        while(runner->right && tree_node_is_operator(runner->right, TREE_OPERATOR_TYPE_MULTIPLY)) {
            if(tree_node_compare(member, runner->left) == 0) {
                *success = true;
                return tree_remove_degreed_member_from_term(expression_link, &runner->left, term_degree, degree);
            }
            if(!term_degree) {
                if((error = tree_remove_factor_from_term(&runner->left, degree, member, success))) return error;
                if(*success) return error;
            }
            runner = runner->right;
        }

        if(tree_node_is_operator(runner, TREE_OPERATOR_TYPE_MULTIPLY)) {
            if(tree_node_compare(member, runner->left) == 0) {
                *success = true;
                return tree_remove_degreed_member_from_term(expression_link, &runner->left, term_degree, degree);
            }
            if(tree_node_compare(member, runner->right) == 0) {
                *success = true;
                return tree_remove_degreed_member_from_term(expression_link, &runner->right, term_degree, degree);
            }
            if(!term_degree) {
                if((error = tree_remove_factor_from_term(&runner->left, degree, member, success))) return error;
                if(*success) return error;
                if((error = tree_remove_factor_from_term(&runner->right, degree, member, success))) return error;
                if(*success) return error;
            }
        }
    } else {
        if(tree_node_compare(member, *real_expression) == 0) {
            *success = true;
            return tree_remove_degreed_member_from_term(expression_link, real_expression, term_degree, degree);
        }
    }

    *success = false;
    return error;
}

e_tree_error tree_remove_factor_from_expression(s_tree_node** expression_link, s_tree_node* degree, s_tree_node* member) {
    s_tree_node* expression = *expression_link;
    s_tree_node* runner = expression;
    e_tree_error error = TREE_OK;

    member = tree_node_clone_deep(member);
    if(!member) return TREE_ERROR_OUT_OF_MEMORY;

    while(runner->right && tree_node_is_operator(runner->right, TREE_OPERATOR_TYPE_PLUS)) {
        tree_remove_factor_from_term(&runner->left, degree, member, NULL);
        runner = runner->right;
    }

    if(tree_node_is_operator(runner, TREE_OPERATOR_TYPE_PLUS)) {
        tree_remove_factor_from_term(&runner->left, degree, member, NULL);
        tree_remove_factor_from_term(&runner->right, degree, member, NULL);
    }

    s_tree_node* old = *expression_link;
    *expression_link = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_MULTIPLY);
    expression = *expression_link;
    if(!expression) return TREE_ERROR_OUT_OF_MEMORY;

    expression->left = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_POW);
    if(!expression->left) return TREE_ERROR_OUT_OF_MEMORY;

    expression->left->left = member;
    expression->left->right = degree;
    expression->right = old;

    return error;
}

bool tree_add_term_factors_to_pool(s_tree_node* expression, s_tree_node* member, s_simplifier_context* context, s_member_degree_pool* pool) {
    s_tree_node* runner = expression;

    while(runner->right && tree_node_is_operator(runner->right, TREE_OPERATOR_TYPE_PLUS)) {
        if(!tree_can_move_factor_from_term(runner->left, member, pool)) {
            return false;
        }
        runner = runner->right;
    }

    if(tree_node_is_operator(runner, TREE_OPERATOR_TYPE_PLUS)) {
        if(!tree_can_move_factor_from_term(runner->left,  member, pool) ||
           !tree_can_move_factor_from_term(runner->right, member, pool)) {
            return false;
        }
    }
    return true;
}

// For each a, b, c, d, e multiplier in (a*b + c + d*e) expression,
// checks if this multiplier can be moved out of the parenthesis.
// calls tree_can_move_factor_from_term for  a*b, c, d*e expressions
e_tree_error tree_try_move_term(s_tree_node** expression_link, s_tree_node* member, s_simplifier_context* context) {
    e_tree_error error = TREE_OK;

    if(tree_node_is_exact_number(member, 1)) {
        return false;
    }

    if(tree_node_is_unary_minus(member) && tree_node_is_exact_number(member->right, 1)) {
        return false;
    }

    if(tree_node_is_operator(member, TREE_OPERATOR_TYPE_POW)) {
        // This expression is too complex.
        // It should be simplified first in order
        // to not check excessive edge-cases here.
        return TREE_OK;
    }

    s_tree_node* expression = *expression_link;

    s_member_degree_pool pool = {0};
    member_degree_pool_init(&pool, tree_count_ordered_terms(expression));

    if(!tree_add_term_factors_to_pool(expression, member, context, &pool)) {
        member_degree_pool_release(&pool);
        return false;
    }

    s_tree_node* redusable_degree = NULL;
    if((error = member_degree_pool_get_degree(&pool, &redusable_degree))) return error;
    if(redusable_degree != NULL) {
        if((error = tree_remove_factor_from_expression(expression_link, redusable_degree, member))) return error;

        simplify_context_perform_action(context);
    }

    return TREE_OK;
}

// for (d*e*f) calls tree_bring_sub_member for d, e, f
// if expression is not a group, it is the actual member.
e_tree_error tree_try_move_factors_of_term(s_tree_node** expression_link, s_tree_node* member, s_simplifier_context* context) {
    if(tree_node_is_operator(member, TREE_OPERATOR_TYPE_DIVIDE)) {
        member = member->left;
    }

    e_tree_error error = TREE_OK;

    int old_operations = context->current_operations;

    if(tree_node_is_operator(member, TREE_OPERATOR_TYPE_MULTIPLY)) {
        s_tree_node* runner = member;

        while(runner->right && tree_node_is_operator(runner->right, TREE_OPERATOR_TYPE_MULTIPLY)) {
            s_tree_node* real_member = *tree_get_pure_member(&runner->right, NULL);
            if((error = tree_try_move_term(expression_link, real_member, context))) return error;
            if(context->current_operations > old_operations) return error;
            runner = runner->right;
        }

        if(tree_node_is_operator(runner, TREE_OPERATOR_TYPE_MULTIPLY)) {
            s_tree_node* real_member = *tree_get_pure_member(&runner->left, NULL);
            if((tree_try_move_term(expression_link, real_member, context))) return error;
            if(context->current_operations > old_operations) return error;
            real_member = *tree_get_pure_member(&runner->right, NULL);
            if((error = tree_try_move_term(expression_link, real_member, context))) return error;
        }
    } else {
        s_tree_node* real_member = *tree_get_pure_member(&member, NULL);
        if((error = tree_try_move_term(expression_link, real_member, context))) return error;
    }

    return error;
}

// For (a+b+c) calls tree_try_move_factors_of_term for a, b, c subexpressions

e_tree_error tree_try_move_same_factors(s_tree_node** expression_link, s_simplifier_context* context) {

    s_tree_node* expression = *expression_link;

    // assuming expression is already ordered with tree_node_reorder_members function
    if(!tree_node_is_operator(expression, TREE_OPERATOR_TYPE_PLUS)) return TREE_OK;

    e_tree_error error = TREE_OK;

    s_tree_node* runner = expression;

    int actions = context->current_operations;

    while(runner->right && tree_node_is_operator(runner->right, TREE_OPERATOR_TYPE_PLUS)) {
        if(!simplify_context_can_perform_action(context)) return TREE_OK;
        if((error = tree_try_move_factors_of_term(expression_link, runner->left, context))) return error;
        if(context->current_operations > actions) return TREE_OK;
        runner = runner->right;
    }

    if(tree_node_is_operator(runner, TREE_OPERATOR_TYPE_PLUS)) {
        if((error = tree_try_move_factors_of_term(expression_link, runner->left, context))) return error;
        if(context->current_operations > actions) return TREE_OK;
        if((error = tree_try_move_factors_of_term(expression_link, runner->right, context))) return error;
    }

    return error;
}

// MARK: Fraction reducing

e_tree_error tree_reduce_fraction_factor(s_tree_node* expression, s_tree_node* member, s_simplifier_context* context, s_member_degree_pool* pool) {
    e_tree_error error = TREE_OK;

    if(tree_node_is_exact_number(member, 1)) {
        return TREE_OK;
    }

    if(tree_node_is_unary_minus(member) && tree_node_is_exact_number(member->right, 1)) {
        return TREE_OK;
    }

    member_degree_pool_reset(pool);

    if(!tree_can_move_factor_from_term(expression->left, member, pool)) {
        return TREE_OK;
    }
    if(!tree_can_move_factor_from_term(expression->right, member, pool)) {
        return TREE_OK;
    }

    s_tree_node* redusable_degree = NULL;
    if((error = member_degree_pool_get_degree(pool, &redusable_degree))) return error;
    if(redusable_degree != NULL) {
        member = tree_node_clone_deep(member);
        if(!member) return TREE_ERROR_OUT_OF_MEMORY;
        if((error = tree_remove_factor_from_term(&expression->left, redusable_degree, member, NULL))) return error;
        member = tree_node_clone_deep(member);
        if(!member) return TREE_ERROR_OUT_OF_MEMORY;
        if((error = tree_remove_factor_from_term(&expression->right, redusable_degree, member, NULL))) return error;

        simplify_context_perform_action(context);
        return TREE_OK;
    }
    return TREE_OK;
}

e_tree_error tree_reduce_fraction(s_tree_node* expression, s_simplifier_context* context) {
    if(!tree_node_is_operator(expression, TREE_OPERATOR_TYPE_DIVIDE)) return TREE_OK;

    e_tree_error error = TREE_OK;

    int old_operations = context->current_operations;
    s_member_degree_pool pool = {0};
    member_degree_pool_init(&pool, 2);

    // Iterating through numerator

    if(tree_node_is_operator(expression->left, TREE_OPERATOR_TYPE_MULTIPLY)) {
        s_tree_node* runner = expression->left;

        while(runner->right && tree_node_is_operator(runner->right, TREE_OPERATOR_TYPE_MULTIPLY)) {
            s_tree_node* real_member = *tree_get_pure_member(&runner->right, NULL);
            if((error = tree_reduce_fraction_factor(expression, real_member, context, &pool))) return error;
            if(context->current_operations > old_operations) return error;
            runner = runner->right;
        }

        if(tree_node_is_operator(runner, TREE_OPERATOR_TYPE_MULTIPLY)) {
            s_tree_node* real_member = *tree_get_pure_member(&runner->left, NULL);
            if((tree_reduce_fraction_factor(expression, real_member, context, &pool))) return error;
            if(context->current_operations > old_operations) return error;
            real_member = *tree_get_pure_member(&runner->right, NULL);
            if((error = tree_reduce_fraction_factor(expression, real_member, context, &pool))) return error;
        }
    } else {
        s_tree_node* real_member = *tree_get_pure_member(&expression->left, NULL);
        if((error = tree_reduce_fraction_factor(expression, real_member, context, &pool))) return error;
    }

    return error;

    return TREE_OK;
}

// MARK: Basic simplifications

e_tree_error tree_simplify_plus_minus(s_tree_node_operator** expression_link, s_simplifier_context* context) {
    s_tree_node_operator* expression = *expression_link;

    e_tree_error error = TREE_OK;
    if(!expression->tree_node.left) {
        // Unary minus, probably.
        if(tree_node_is_exact_number(expression->tree_node.right, 0)) {
            tree_release_subtree(expression->tree_node.right);
            expression->tree_node.right = NULL;
            tree_release_subtree((s_tree_node*) expression);
            *expression_link = (s_tree_node_operator*) tree_create_number_from_double(0);
            if(!*expression_link) return TREE_ERROR_OUT_OF_MEMORY;
            simplify_context_perform_action(context);
        } else if(expression->tree_node.right->type == TREE_NODE_TYPE_OPERATOR) {
            s_tree_node* unary_minus = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_MINUS);
            if(!unary_minus) return TREE_ERROR_OUT_OF_MEMORY;
            expression->tree_node.left = unary_minus;
            if(!(unary_minus->right = (s_tree_node*) tree_create_number_from_double(1))) return TREE_ERROR_OUT_OF_MEMORY;
            expression->operator_type = TREE_OPERATOR_TYPE_MULTIPLY;
            simplify_context_perform_action(context);
        }
        return TREE_OK;
    }

    if(expression->operator_type == TREE_OPERATOR_TYPE_MINUS) {
        expression->operator_type = TREE_OPERATOR_TYPE_PLUS;
        s_tree_node_operator* unary_minus = tree_create_node_operator(TREE_OPERATOR_TYPE_MINUS);
        if(!unary_minus) return TREE_ERROR_OUT_OF_MEMORY;
        unary_minus->tree_node.right = expression->tree_node.right;
        expression->tree_node.right = (s_tree_node*) unary_minus;
        unary_minus->tree_node.type = TREE_NODE_TYPE_OPERATOR;
        unary_minus->operator_type = TREE_OPERATOR_TYPE_MINUS;
        simplify_context_perform_action(context);
        return TREE_OK;
    }


    if(tree_node_is_exact_number(expression->tree_node.left, 0)) {
        // Preventing 0-x from being simplified to x

        if(expression->operator_type == TREE_OPERATOR_TYPE_PLUS) {
            tree_node_sift_right((s_tree_node**) expression_link);
            simplify_context_perform_action(context);
        }
    } else if(tree_node_is_exact_number(expression->tree_node.right, 0)) {
        tree_node_sift_left((s_tree_node**) expression_link);
        simplify_context_perform_action(context);
    }

    return TREE_OK;
}

e_tree_error tree_node_amplify_degree(s_tree_node** expression_link, s_tree_node* degree) {
    s_tree_node* expression = *expression_link;

    if(tree_node_is_operator(expression, TREE_OPERATOR_TYPE_POW)) {
        s_tree_node* current_degree = expression->right;
        expression->right = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_MULTIPLY);
        if(!expression->right) return TREE_ERROR_OUT_OF_MEMORY;
        expression->right->left = current_degree;
        expression->right->right = tree_node_clone_deep(degree);
        if(!expression->right->right) return TREE_ERROR_OUT_OF_MEMORY;
    } else {
        s_tree_node* copy = expression;

        *expression_link = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_POW);
        expression = *expression_link;
        if(!expression) return TREE_ERROR_OUT_OF_MEMORY;

        expression->left = copy;
        expression->right = tree_node_clone_deep(degree);
        if(!expression->right) return TREE_ERROR_OUT_OF_MEMORY;
    }
    return TREE_OK;
}

e_tree_error tree_node_distribute_degree(s_tree_node* expression, s_tree_node* degree) {
    e_tree_error error = TREE_OK;

    s_tree_node* runner = expression;

    while(runner->right && tree_node_is_operator(runner->right, TREE_OPERATOR_TYPE_MULTIPLY)) {
        if((error = tree_node_amplify_degree(&runner->left, degree))) return error;
        runner = runner->right;
    }

    if(tree_node_is_operator(runner, TREE_OPERATOR_TYPE_MULTIPLY)) {
        if((error = tree_node_amplify_degree(&runner->left, degree))) return error;
        if((error = tree_node_amplify_degree(&runner->right, degree))) return error;
    }

    return error;
}

e_tree_error tree_simplify_power(s_tree_node** expression_link, s_simplifier_context* context) {
    s_tree_node* expression = *expression_link;

    if(tree_node_is_exact_number(expression->right, 1)) {
        tree_node_sift_left(expression_link);
        simplify_context_perform_action(context);
        return TREE_OK;
    }

    if(tree_node_is_exact_number(expression->right, 0)) {
        tree_release_subtree(expression);
        *expression_link = (s_tree_node*) tree_create_number_from_double(1);
        if(!*expression_link) return TREE_ERROR_OUT_OF_MEMORY;
        simplify_context_perform_action(context);
        return TREE_OK;
    }

    if(tree_node_is_exact_number(expression->left, 1)) {
        tree_release_subtree(expression);
        *expression_link = (s_tree_node*) tree_create_number_from_double(1);
        if(!*expression_link) return TREE_ERROR_OUT_OF_MEMORY;
        simplify_context_perform_action(context);
        return TREE_OK;
    }

    if(tree_node_is_exact_number(expression->left, 0)) {
        tree_release_subtree(expression);
        *expression_link = (s_tree_node*) tree_create_number_from_double(0);
        if(!*expression_link) return TREE_ERROR_OUT_OF_MEMORY;
        simplify_context_perform_action(context);
        return TREE_OK;
    }

    if(tree_node_is_operator(expression->left, TREE_OPERATOR_TYPE_MULTIPLY)) {
        e_tree_error error = TREE_OK;
        if((error = tree_node_distribute_degree(expression->left, expression->right))) return error;
        tree_node_sift_left(expression_link);
        simplify_context_perform_action(context);
        return TREE_OK;
    }
    if(tree_node_is_operator(expression->left, TREE_OPERATOR_TYPE_POW)) {
        tree_node_amplify_degree(&expression->left, expression->right);
        tree_node_sift_left(expression_link);
        simplify_context_perform_action(context);
        return TREE_OK;
    }
    if(tree_node_is_number(expression->left) && tree_node_is_number(expression->right)) {

        double left_number = (double)((s_tree_node_number*) expression->left)->float_value;
        double right_number = (double)((s_tree_node_number*) expression->right)->float_value;

        if(left_number == round(left_number) && right_number == round(right_number) && right_number >= 0) {

            tree_release_subtree(expression);
            *expression_link = (s_tree_node*) tree_create_number_from_double(pow(left_number, right_number));
            if(!*expression_link) return TREE_ERROR_OUT_OF_MEMORY;
            simplify_context_perform_action(context);
            return TREE_OK;
        }
    }
    return TREE_OK;
}

e_tree_error tree_simplify_division(s_tree_node** expression_link, s_simplifier_context* context) {
    s_tree_node* expression = *expression_link;

    if(tree_node_is_exact_number(expression->right, 1)) {
        tree_node_sift_left(expression_link);
        simplify_context_perform_action(context);
        return TREE_OK;
    }

    if(tree_node_is_exact_number(expression->left, 0)) {
        tree_node_sift_left(expression_link);
        simplify_context_perform_action(context);
        return TREE_OK;
    }

    if(context->current_operations == 0 && simplify_context_can_perform_action(context)) {
        e_tree_error error = TREE_OK;
        if((error = tree_reduce_fraction(expression, context))) return error;
    }
    return TREE_OK;
}

e_tree_error tree_simplify_multiplication(s_tree_node** expression_link, s_simplifier_context* context) {
    s_tree_node* expression = *expression_link;
    e_tree_error error = TREE_OK;

    if(tree_node_is_exact_number(expression->left, 0) || tree_node_is_exact_number(expression->right, 0)) {
        tree_release_subtree(expression);
        *expression_link = (s_tree_node*) tree_create_number_from_double(0);
        if(!*expression_link) return TREE_ERROR_OUT_OF_MEMORY;
        simplify_context_perform_action(context);
    } else if(tree_node_is_exact_number(expression->left, 0)) {
        tree_node_sift_right(expression_link);
        simplify_context_perform_action(context);
    } else if(tree_node_is_exact_number(expression->right, 1)) {
        tree_node_sift_left(expression_link);
        simplify_context_perform_action(context);
    } else if(tree_node_is_operator(expression->left, TREE_OPERATOR_TYPE_MINUS) && !expression->left->left) {
        if(tree_node_is_operator(expression->right, TREE_OPERATOR_TYPE_PLUS)) {
            if((error = tree_node_distribute_value(expression->left, expression->right, context))) return error;
            tree_node_sift_right(expression_link);
            simplify_context_perform_action(context);
        }
    } else if(tree_node_is_operator(expression->right, TREE_OPERATOR_TYPE_MINUS) && !expression->right->left) {
        if(tree_node_is_operator(expression->left, TREE_OPERATOR_TYPE_PLUS)) {
            if((error = tree_node_distribute_value(expression->right, expression->left, context))) return error;
            tree_node_sift_left(expression_link);
            simplify_context_perform_action(context);
        }
    }

    expression = *expression_link;

    if(!tree_node_is_operator(expression, TREE_OPERATOR_TYPE_MULTIPLY)) return TREE_OK;

    if(!simplify_context_can_perform_action(context)) {
        return TREE_OK;
    }

    bool left_is_fraction = tree_node_is_operator(expression->left, TREE_OPERATOR_TYPE_DIVIDE);
    bool right_is_fraction = tree_node_is_operator(expression->right, TREE_OPERATOR_TYPE_DIVIDE);

    if(left_is_fraction && right_is_fraction) {
        // TODO
    } else if(left_is_fraction || right_is_fraction) {
        s_tree_node* multiplication = (s_tree_node*) tree_create_node_operator(TREE_OPERATOR_TYPE_MULTIPLY);
        if(!multiplication) return TREE_ERROR_OUT_OF_MEMORY;

        if(left_is_fraction) {
            s_tree_node* factor = expression->right;
            expression->right = NULL;
            multiplication->right = factor;
            multiplication->left = expression->left->left;
            expression->left->left = multiplication;
            tree_node_sift_left(expression_link);
        } else {
            s_tree_node* factor = expression->left;
            expression->left = NULL;
            multiplication->left = factor;
            multiplication->right = expression->right->left;
            expression->right->left = multiplication;
            tree_node_sift_right(expression_link);
        }
        simplify_context_perform_action(context);
    }

    return error;
}

e_tree_error tree_subtree_simplify(s_tree_node** expression_link, s_simplifier_context* context, s_tree_node_operator* parent) {
    s_tree_node* expression = *expression_link;
    e_tree_error error = TREE_OK;

    if(!simplify_context_can_perform_action(context)) {
        return TREE_OK;
    }

    if(expression->type == TREE_NODE_TYPE_OPERATOR) {
        s_tree_node_operator* operator = (s_tree_node_operator*) expression;
        if(expression->left) {
            if((error = tree_subtree_simplify(&expression->left, context, (s_tree_node_operator*) expression))) return error;
        }
        if(expression->right) {
            if((error = tree_subtree_simplify(&expression->right, context, (s_tree_node_operator*) expression))) return error;
        }

        switch(operator->operator_type) {
            case TREE_OPERATOR_TYPE_PLUS:
            case TREE_OPERATOR_TYPE_MINUS:
                if((error = tree_simplify_plus_minus((s_tree_node_operator**) expression_link, context))) return error;
                break;
            case TREE_OPERATOR_TYPE_MULTIPLY:
                if((error = tree_simplify_multiplication(expression_link, context))) return error;
                break;
            case TREE_OPERATOR_TYPE_POW:
                if((error = tree_simplify_power(expression_link, context))) return error;
                break;
            case TREE_OPERATOR_TYPE_DIVIDE:
                if((error = tree_simplify_division(expression_link, context))) return error;
                break;
            case TREE_OPERATOR_TYPE_CALL:
                if((error = tree_simplify_appliance((s_tree_node_operator**) expression_link, context))) return error;
                break;
            default:
                break;
        }
    }

    expression = *expression_link;

    tree_node_reorder_members(expression);

    if(!simplify_context_can_perform_action(context)) return TREE_OK;

    tree_node_sort_members(expression, context);

    if(simplify_context_can_perform_action(context)) {
        if((error = tree_cite_similar_members(expression_link, context))) return error;
        if(!simplify_context_can_perform_action(context)) return TREE_OK;
    }

    if(expression->type == TREE_NODE_TYPE_OPERATOR && (!parent || parent->operator_type != ((s_tree_node_operator*)expression)->operator_type)) {
        // This method might get looped when expression is not simplified
        if(simplify_context_can_perform_action(context) && context->current_operations == 0) {
            if((error = tree_try_move_same_factors(expression_link, context))) return error;
        }
    }

    return TREE_OK;
}
// MARK: Interface

bool tree_simplify(s_tree* expression, s_simplifier_config* config) {
    if(expression == NULL) return false;

    s_simplifier_context context = {0};

    context.current_operations = 0;
    context.config = config;

//    e_tree_error error = test(expression->root, &context);
//    if(error) return false;

    e_tree_error error = tree_subtree_simplify(&expression->root, &context, NULL);
    if(error) return false;

    return context.current_operations > 0;
}
