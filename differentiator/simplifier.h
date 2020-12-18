
#ifndef DIFFERENTIATOR_SIMPLIFIER_H
#define DIFFERENTIATOR_SIMPLIFIER_H

#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "tree.h"

typedef struct simplifier_config {
    int max_operations_per_step;
    FILE* actions_log_file;
} s_simplifier_config;

typedef struct simplifier_context {
    s_simplifier_config* config;
    int current_operations;
    bool taken_derivative;
    s_tree_node* before_simplification;
    s_tree_node* after_simplification;
    s_tree* tree;
} s_simplifier_context;

typedef struct member_degree_pool {
    int members_count;
    int members_checked;
    s_tree_node** powers;
} s_member_degree_pool;

void simplifier_context_init(s_simplifier_context* ctx);
void simplifier_context_destruct(s_simplifier_context* ctx);

void member_degree_pool_init(s_member_degree_pool* pool, int capacity);
void member_degree_pool_release(s_member_degree_pool* pool);
void member_degree_pool_add(s_member_degree_pool* pool, s_tree_node* node);
void member_degree_pool_reset(s_member_degree_pool* pool);
e_tree_error member_degree_pool_get_degree(s_member_degree_pool* pool, s_tree_node** result);

bool tree_subtree_depends_on_variable(s_tree_node* subtree, unsigned long variable_index);

bool tree_is_differential(s_tree_node* expression);
bool tree_node_is_unary_minus(s_tree_node* node);

unsigned long tree_find_differential_variable(s_tree_node* expression);
bool tree_node_contains_differential(s_tree_node* expression);
e_tree_error tree_node_pow_derivative(s_tree_node** node_link, s_tree_node** target, unsigned long variable_index, s_simplifier_context* context);
e_tree_error tree_node_sqrt_derivative(s_tree_node** node_link, s_tree_node** target, unsigned long variable_index, s_simplifier_context* context);
e_tree_error tree_node_function_derivative(s_tree_node** node_link, s_tree_node** target, unsigned long variable_index, s_simplifier_context* context);
e_tree_error tree_node_multiplication_derivative(s_tree_node** node_link, s_tree_node** target, unsigned long variable_index, s_simplifier_context* context);
e_tree_error tree_node_division_derivative(s_tree_node** node_link, s_tree_node** target, unsigned long variable_index, s_simplifier_context* context);
e_tree_error tree_subtree_derivative(s_tree_node** node_link, s_tree_node** target, unsigned long variable_index, s_simplifier_context* context);
e_tree_error tree_node_wrap_derivative(s_tree_node** node_link, s_tree_node** target, unsigned long variable_index);

int tree_node_compare(s_tree_node* left, s_tree_node* right);
int tree_node_compare_signed(s_tree_node* left, s_tree_node* right, bool ignore_sign);
bool tree_node_reorder_members(s_tree_node* expression);

bool __unused tree_node_is_number(s_tree_node* node);

e_tree_error tree_power_wrap_terms_if_needed(s_tree_node** node);
void tree_node_intersect(s_tree_node** sum, s_tree_node* mask);

bool tree_node_factor_of_term(s_tree_node* base, s_tree_node* term, s_tree_node** factor);
bool tree_node_factor_of_term_in_expression(s_tree_node* expression, s_tree_node* term_base, s_tree_node** result);
e_tree_error tree_node_sort_members(s_tree_node* expression, s_simplifier_context* context);
e_tree_error tree_cite_merge_similar(s_tree_node_operator** node_link, bool is_nested, s_simplifier_context* context);
e_tree_error tree_cite_similar_members(s_tree_node** expression_link, s_simplifier_context* context);
e_tree_error tree_power_wrap_term_if_needed(s_tree_node** node);
bool tree_can_move_factor_from_term(s_tree_node* expression, s_tree_node* member, s_member_degree_pool* pool);
e_tree_error tree_remove_degreed_member_from_term(s_tree_node** term_link, s_tree_node** member, s_tree_node* member_degree, s_tree_node* reduce_degree);
e_tree_error tree_remove_factor_from_term(s_tree_node** expression_link, s_tree_node* degree, s_tree_node* member, bool* success);
e_tree_error tree_remove_factor_from_expression(s_tree_node** expression_link, s_tree_node* degree, s_tree_node* member);
e_tree_error tree_try_move_term(s_tree_node** expression_link, s_tree_node* member, s_simplifier_context* context);
e_tree_error tree_try_move_factors_of_term(s_tree_node** expression, s_tree_node* member, s_simplifier_context* context);
e_tree_error tree_try_move_same_factors(s_tree_node** expression_link, s_simplifier_context* context);

s_tree_node** tree_get_pure_member(s_tree_node** member, s_tree_node** power);

e_tree_error tree_simplify_plus_minus(s_tree_node_operator** expression_link, s_simplifier_context* context);
e_tree_error tree_simplify_power(s_tree_node** expression_link, s_simplifier_context* context);
e_tree_error tree_simplify_division(s_tree_node** expression_link, s_simplifier_context* context);
e_tree_error tree_simplify_multiplication(s_tree_node** expression_link, s_simplifier_context* context);

bool tree_simplify(s_simplifier_context* context, s_tree* expression, s_simplifier_config* config);
e_tree_error tree_subtree_simplify(s_tree_node** expression_link, s_simplifier_context* context, s_tree_node_operator* parent);


#endif //DIFFERENTIATOR_SIMPLIFIER_H
