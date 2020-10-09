# Invulnerable stack

This task is dedicated to creating a data structure that would detect any unauthorized changes to its internal data structures.

The following protection measures have been applied:
- Stack data backup buffer duplication.
- The readability of pointers is checked during stack validation.
- The stack is accessed via a descriptor (weak security measure, because the attribute "static" does not hide fields in .h files)

The stack supports templates and can be included into a project several times with different types of stored data. All names of stack access functions are changed so that the type name at the end of the name is contained. Unfortunately, it makes it impossible to hide fields because the entire stack implementation must be placed in .h files.

In this task, a massive "templateization" of code through macros was experimentally performed. It turned out that this makes the code extremely illegible. This practice will not be used in future projects.

Links:
  - [Partner project](https://github.com/AlexRoar/MIPTProjects_1sem/tree/master/StackOnSteroids). 
