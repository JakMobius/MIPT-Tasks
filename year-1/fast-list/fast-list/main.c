
#include <stdio.h>
#include <math.h>
#include "list.h"
#include "list-tester.h"

bool is_square(double number) {
    double root = sqrt(number);
    return root == round(root);
}

int main(int argc, const char * argv[]) {
    
    // s_mlist* list = mlist_new(4);
    
//     s_mlist_dumper* dumper = mlist_dumper_new();
//     mlist_dumper_set_target_folder(dumper, "./list-dump");
//     mlist_set_dumper(list, dumper);
    
    // работа со списком
    
//     mlist_dumper_flush(dumper);
//     mlist_dumper_release(dumper);
    
//    mlist_release(list);

    // Пример 1: Продублировать каждый элемент, являющийся квадратом натурального числа
    // Важно то, что проверка, что число является квадратом - сложная операция.

    s_mlist* list = mlist_new(4);
    
    for(int i = 1; i <= 16; i++) mlist_insert_tail(list, i);

    for(mlist_index i = list -> head; i != list -> tail; i = list -> next_indices[i]) {
        double element = list -> element_array[i];
        
        if(is_square(element)) mlist_insert(list, i, element);
    }
    
    for(mlist_index i = list -> head; i != list -> tail; i = list -> next_indices[i]) printf("%g ", list -> element_array[i]);
    printf("\n");

    mlist_release(list);
    
    // Сложность: O(n)
    // При использовании массива сложность составила бы O(n^2)
    // При использовании массива и дублировании буфера сложность
    // составила бы O(n), однако константа была бы выше.
    // Реализация на массиве потребовала бы O(n) доп. памяти.
    
    // Пример 2: Удалить из списка элементы, являющиеся квадратами натурального числа
    // Важно то, что проверка, что число является квадратом - сложная операция.
    
    list = mlist_new(4);
    
    for(int i = 1; i <= 16; i++) mlist_insert_tail(list, i);

    for(mlist_index i = list -> head; i != list -> tail;) {
        double element = list -> element_array[i];
        
        mlist_index next = list -> next_indices[i];
        
        if(is_square(element)) mlist_delete(list, i, NULL);
        
        i = next;
    }
    
    for(mlist_index i = list -> head; i != list -> tail; i = list -> next_indices[i]) printf("%g ", list -> element_array[i]);
    printf("\n");
    
    mlist_release(list);
    
    // Сложность: O(n)
    // При использовании массива сложность составила бы O(n^2)
    // При использовании массива и дублировании буфера сложность
    // составила бы O(n), однако константа была бы выше.
    // Реализация на массиве потребовала бы O(n) доп. памяти.
    
    
    run_test_folder("./tests/");
    
    return 0;
}


