
#include <stdio.h>
#include "list.h"
#include "list-tester.h"

int main(int argc, const char * argv[]) {
    
//    s_mlist* list = mlist_new(4);
//    s_mlist_dumper* dumper = mlist_dumper_new();
//    mlist_dumper_set_target_folder(dumper, "./list-dump");
//
//    mlist_set_dumper(list, dumper);
//
//    mlist_insert_head(list, 1);
//    mlist_insert_tail(list, 2);
//    mlist_insert_head(list, 3);
//    mlist_insert_tail(list, 4);
//
//    mlist_insert_tail(list, 2);
//    mlist_insert_tail(list, 2);
//    mlist_insert_tail(list, 2);
//    mlist_insert_tail(list, 2);
//    mlist_insert_tail(list, 2);
//    mlist_insert_tail(list, 2);
//    mlist_insert_tail(list, 2);
//    mlist_insert_tail(list, 2);
//    mlist_insert_tail(list, 2);
//
//    mlist_engage_madness(list);
//
//    mlist_delete_tail(list, NULL);
//    mlist_delete_tail(list, NULL);
//    mlist_delete_tail(list, NULL);
//    mlist_delete_tail(list, NULL);
//
//    mlist_dumper_flush(dumper);
//    mlist_dumper_release(dumper);
//    mlist_release(list);
    
    run_test_folder("./tests/");
    
    return 0;
}


