#define ENUM_NAME(a) a
#define COMMAND_NAME(a) a
#define COMMAND_FLAGS(a) a
#define COMPILE(a) a
#define PROCESS(a) a
#define DECOMPILE(a) a

#define CONVERT(in, out) {                                 \
    in input = 0;                                          \
    s_processor_stack_pop(thou, &input, sizeof(in));       \
    out output = (out) input;                              \
    if(thou -> state != PROC_STATE_RUNNING) return;        \
    s_processor_stack_push(thou, &output, sizeof(output)); \
}

#define OPCODE(opcode) {                                    \
    char code = opcode;                                     \
    compiler_write_byte(thou, code);                        \
}

#define OPCODE_FLAG(opcode) {                               \
    char code = opcode;                                     \
    compiler_write_byte(thou, code);                        \
    compiler_write_byte(thou, flag);                        \
}

#define DECOMPILE_OPCODE_FLAG(name) {                       \
    unsigned char flag = decompiler_next_char(thou);        \
    fprintf(thou -> config -> output_file, name ".%s ", COMPILER_FLAG_NAMES[flag]); \
}

#define DECOMPILE_OPCODE(name) fprintf(thou -> config -> output_file, name);

#define DECOMPILE_JMP_INSTRUCTION(name) {                   \
    fprintf(thou -> config -> output_file, "jmp 0x%08llx", decompiler_read_integer(thou, 8));\
}

#define DECOMPILE_JMP_INSTRUCTION_FLAG(name) {              \
    DECOMPILE_OPCODE_FLAG(name);                            \
    fprintf(thou -> config -> output_file, "0x%08llx", decompiler_read_integer(thou, 8)); \
}

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_PUSH),
    COMMAND_NAME("push"),
    COMMAND_FLAGS(PROC_FLAG_MASK_DECIMALS | PROC_FLAG_MASK_NONE),
    COMPILE({
        char code = PROC_COMMAND_PUSH;
    
        compiler_write_byte(thou, code);

        s_stack_argument argument = {0};
    
        proc_parse_stack_argument(thou, flag, &argument, PROC_COMMAND_PUSH);
    
        if(thou -> state != COMPILER_STATE_RUNNING) return;
    
        compiler_write_byte(thou, argument.flag);
    
        if(argument.register_met) {
            compiler_write_byte(thou, argument.reg);
        }
        
        if(argument.number_met) {
            if(argument.memory_read) {
                compiler_write_number(thou, PROC_FLAG_INT64, argument.number);
            } else if(argument.register_met) {
                compiler_write_number(thou, argument.flag & 0x0F, argument.number);
            } else {
                compiler_write_number(thou, flag, argument.number);
            }
        }
    }),
    PROCESS({
        s_proc_stack_argument argument = {0};
        proc_read_stack_argument(thou, &argument);
    
        if(argument.memory_access) {
            unsigned long long buffer = 0;
            memcpy(&buffer, thou -> memory + argument.value, argument.data_length);
            
            s_processor_stack_push(thou, &buffer, argument.data_length);
            
            //printf("readen %d\n", buffer);
            
            //printf("reading from address [%d]\n", argument.value);
        } else {
            s_processor_stack_push(thou, &argument.value, argument.data_length);
            //printf("reading %d %d\n", argument.value, argument.data_length);
        }
    }),
    DECOMPILE({
        unsigned char flag = decompiler_next_char(thou);
    
        fprintf(thou -> config -> output_file, "push.%s ", COMPILER_FLAG_NAMES[flag & 0x0F]);

        decompiler_print_stack_argument(thou, flag);
    })
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_POP),
    COMMAND_NAME("pop"),
    COMMAND_FLAGS(PROC_FLAG_MASK_DECIMALS | PROC_FLAG_MASK_NONE),
    COMPILE({
        char code = PROC_COMMAND_POP;

        compiler_write_byte(thou, code);

        s_stack_argument argument = {0};

        proc_parse_stack_argument(thou, flag, &argument, PROC_COMMAND_POP);

        if(thou -> state != COMPILER_STATE_RUNNING) return;

        compiler_write_byte(thou, argument.flag);

        if(argument.register_met) compiler_write_byte(thou, argument.reg);
        
        if(argument.number_met) {
            if(argument.memory_read) {
                compiler_write_number(thou, PROC_FLAG_INT64, argument.number);
            } else {
                compiler_write_number(thou, flag, argument.number);
            }
        }
    }),
    PROCESS({
        s_proc_stack_argument argument = {0};
        proc_read_stack_argument(thou, &argument);

        if(argument.memory_access) {
            unsigned long long buffer = 0;
            s_processor_stack_pop(thou, &buffer, argument.data_length);
            
            memcpy(thou -> memory + argument.value, &buffer, argument.data_length);
        } else {
            unsigned long long register_offset = PROC_REGISTER_OFFSETS[argument.reg];
            s_processor_stack_pop(thou, (char*)&(thou -> registers) + register_offset, argument.data_length);
        }
    }),
    DECOMPILE({
        unsigned char flag = decompiler_next_char(thou);

        fprintf(thou -> config -> output_file, "pop.%s ", COMPILER_FLAG_NAMES[flag & 0x0F]);

        decompiler_print_stack_argument(thou, flag);
    })
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_ADD),
    COMMAND_NAME("add"),
    COMMAND_FLAGS(PROC_FLAG_MASK_DECIMALS),
    COMPILE(OPCODE_FLAG(PROC_COMMAND_ADD)),
    PROCESS({
        proc_math_instruction_handler(thou, 0);
    }),
    DECOMPILE(DECOMPILE_OPCODE_FLAG("add"))
 )

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_SUBTRACT),
    COMMAND_NAME("sub"),
    COMMAND_FLAGS(PROC_FLAG_MASK_DECIMALS),
    COMPILE(OPCODE_FLAG(PROC_COMMAND_SUBTRACT)),
    PROCESS({
        proc_math_instruction_handler(thou, 1);
    }),
    DECOMPILE(DECOMPILE_OPCODE_FLAG("sub"))
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_MULTIPLY),
    COMMAND_NAME("mul"),
    COMMAND_FLAGS(PROC_FLAG_MASK_DECIMALS),
    COMPILE(OPCODE_FLAG(PROC_COMMAND_MULTIPLY)),
    PROCESS({
        proc_math_instruction_handler(thou, 2);
    }),
    DECOMPILE(DECOMPILE_OPCODE_FLAG("mul"))
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_DIVIDE),
    COMMAND_NAME("div"),
    COMMAND_FLAGS(PROC_FLAG_MASK_DECIMALS),
    COMPILE(OPCODE_FLAG(PROC_COMMAND_DIVIDE)),
    PROCESS({
        proc_math_instruction_handler(thou, 3);
    }),
    DECOMPILE(DECOMPILE_OPCODE_FLAG("div"))
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_SQRT),
    COMMAND_NAME("sqrt"),
    COMMAND_FLAGS(PROC_FLAG_MASK_FLOATS),
    COMPILE(OPCODE_FLAG(PROC_COMMAND_SQRT)),
    PROCESS({
        unsigned char flag = compiler_next_char(thou);
        unsigned long flag_size = PROC_FLAG_SIZES[flag];
        
        unsigned long long number;
        
        s_processor_stack_pop(thou, &number, flag_size);
        
        if(thou -> state != PROC_STATE_RUNNING) return;
        
        unsigned long long result;
        
        unsigned long long trimmer = 0;
        
        if(flag_size == sizeof(trimmer)) {
            trimmer = -1;
        } else {
            trimmer = ((unsigned long long)1 << (flag_size * 8)) - 1;
        }
        
        if(flag == PROC_FLAG_FLOAT32) {
            float casted = *((float*)&number);
            if(casted < 0) {
                proc_error(thou, PROC_STATE_MATHEMATICAL_EXCEPTION);
                return;
            }
            *((float*)&result) = sqrtf(casted);
        } else if(flag == PROC_FLAG_FLOAT64) {
            double casted = *((double*)&number);
            if(casted < 0) {
                proc_error(thou, PROC_STATE_MATHEMATICAL_EXCEPTION);
                return;
            }
            *((double*)&result) = sqrt(casted);
        }
        
        s_processor_stack_push(thou, &result, flag_size);
    }),
    DECOMPILE(DECOMPILE_OPCODE_FLAG("sqrt"))
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_FLOAT_TO_DOUBLE),
    COMMAND_NAME("ftod"),
    COMMAND_FLAGS(0),
    COMPILE(OPCODE(PROC_COMMAND_FLOAT_TO_DOUBLE)),
    PROCESS(CONVERT(float, double)),
    DECOMPILE(DECOMPILE_OPCODE("sqrt"))
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_DOUBLE_TO_FLOAT),
    COMMAND_NAME("dtof"),
    COMMAND_FLAGS(0),
    COMPILE(OPCODE(PROC_COMMAND_DOUBLE_TO_FLOAT)),
    PROCESS(CONVERT(double, float)),
    DECOMPILE(DECOMPILE_OPCODE("dtof"))
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_EXTEND_SIGNED_VALUE),
    COMMAND_NAME("xtnd"),
    COMMAND_FLAGS(PROC_FLAG_MASK_EXTENDABLE_INTS),
    COMPILE({
        char code = PROC_COMMAND_EXTEND_SIGNED_VALUE;
        
        compiler_write_byte(thou, code);
        compiler_write_byte(thou, flag);
        
        s_compiler_position position = thou -> position;
        long long number = read_number(thou, true).signed_value;
        
        if(thou -> state != COMPILER_STATE_RUNNING) return;
        
        if(number != 2 && number != 4 && number != 8) {
            compiler_error_positioned(thou, &position, "value can be extended to either 2, 4 or 8 bytes");
            return;
        }
        
        if(number <= PROC_FLAG_SIZES[flag]) {
            compiler_error_positioned(thou, &position, "%d-byte value cannot be extended to %d bytes", PROC_FLAG_SIZES[flag], number);
            return;
        }
        
        compiler_write_byte(thou, number);
    }),
    PROCESS({
        unsigned char flag = compiler_next_char(thou);
        unsigned long flag_size = PROC_FLAG_SIZES[flag];
        unsigned long result_size = compiler_next_char(thou);
        
        if(thou -> state != PROC_STATE_RUNNING) return;
        
        unsigned long long buffer = 0;
        s_processor_stack_pop(thou, &buffer, flag_size);
        
        if(thou -> state != PROC_STATE_RUNNING) return;
        
        unsigned long long trimmer = 0;
        
        if(flag_size == sizeof(trimmer)) {
            trimmer = -1;
        } else {
            trimmer = ((unsigned long long)1 << (flag_size * 8)) - 1;
        }
        
        if(buffer & ((unsigned long long) 1 << (flag_size * 8 - 1))) {
            buffer |= (~trimmer);
        } else {
            buffer &= trimmer;
        }
        
        s_processor_stack_push(thou, &buffer, result_size);
    }),
    DECOMPILE({
        e_proc_flags flag = decompiler_next_char(thou);
        unsigned char result_size = decompiler_next_char(thou);
        
        fprintf(thou -> config -> output_file, "xtnd.%s %d", COMPILER_FLAG_NAMES[flag], result_size);
    })
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_FLOAT_TO_UNSIGNED_LONG),
    COMMAND_NAME("ftou"),
    COMMAND_FLAGS(0),
    COMPILE(OPCODE(PROC_COMMAND_FLOAT_TO_UNSIGNED_LONG)),
    PROCESS(CONVERT(float, unsigned long long)),
    DECOMPILE(DECOMPILE_OPCODE("ftou"))
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_FLOAT_TO_LONG),
    COMMAND_NAME("ftol"),
    COMMAND_FLAGS(0),
    COMPILE(OPCODE(PROC_COMMAND_FLOAT_TO_LONG)),
    PROCESS(CONVERT(float, long long)),
    DECOMPILE(DECOMPILE_OPCODE("ftol"))
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_DOUBLE_TO_UNSIGNED_LONG),
    COMMAND_NAME("dtou"),
    COMMAND_FLAGS(0),
    COMPILE(OPCODE(PROC_COMMAND_DOUBLE_TO_UNSIGNED_LONG)),
    PROCESS(CONVERT(double, unsigned long long)),
    DECOMPILE(DECOMPILE_OPCODE("dtou"))
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_DOUBLE_TO_LONG),
    COMMAND_NAME("dtol"),
    COMMAND_FLAGS(0),
    COMPILE(OPCODE(PROC_COMMAND_DOUBLE_TO_LONG)),
    PROCESS(CONVERT(double, long long)),
    DECOMPILE(DECOMPILE_OPCODE("dtol"))
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_UNSIGNED_LONG_TO_FLOAT),
    COMMAND_NAME("utof"),
    COMMAND_FLAGS(0),
    COMPILE(OPCODE(PROC_COMMAND_UNSIGNED_LONG_TO_FLOAT)),
    PROCESS(CONVERT(unsigned long long, float)),
    DECOMPILE(DECOMPILE_OPCODE("utof"))
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_LONG_TO_FLOAT),
    COMMAND_NAME("ltof"),
    COMMAND_FLAGS(0),
    COMPILE(OPCODE(PROC_COMMAND_LONG_TO_FLOAT)),
    PROCESS(CONVERT(long long, float)),
    DECOMPILE(DECOMPILE_OPCODE("ltof"))
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_UNSIGNED_LONG_TO_DOUBLE),
    COMMAND_NAME("utod"),
    COMMAND_FLAGS(0),
    COMPILE(OPCODE(PROC_COMMAND_UNSIGNED_LONG_TO_DOUBLE)),
    PROCESS(CONVERT(unsigned long long, double)),
    DECOMPILE(DECOMPILE_OPCODE("utod"))
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_LONG_TO_DOUBLE),
    COMMAND_NAME("ltod"),
    COMMAND_FLAGS(0),
    COMPILE(OPCODE(PROC_COMMAND_LONG_TO_DOUBLE)),
    PROCESS(CONVERT(long long, double)),
    DECOMPILE(DECOMPILE_OPCODE("ltof"))
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_IN),
    COMMAND_NAME("in"),
    COMMAND_FLAGS(PROC_FLAG_MASK_DECIMALS),
    COMPILE(OPCODE_FLAG(PROC_COMMAND_IN)),
    PROCESS({
        unsigned char flag = compiler_next_char(thou);
        unsigned long flag_size = PROC_FLAG_SIZES[flag];
        
        if(thou -> state != PROC_STATE_RUNNING) return;
        
        unsigned long long buffer = 0;
            
        unsigned long long mask = (unsigned long long)1 << flag;
        
        if(mask & PROC_FLAG_MASK_SIGNED_INTS) {
            scanf("%lld", &buffer);
        } else if(mask & PROC_FLAG_MASK_UNSIGNED_INTS) {
            scanf("%llu", &buffer);
        } else if(flag == PROC_FLAG_FLOAT32) {
            scanf("%f", ((float*)&buffer));
        } else if(flag == PROC_FLAG_FLOAT64) {
            scanf("%lg", ((double*)&buffer));
        }
        
        s_processor_stack_push(thou, &buffer, flag_size);
            
        if(thou -> state != PROC_STATE_RUNNING) return;
    }),
    DECOMPILE(DECOMPILE_OPCODE_FLAG("in"))
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_OUT),
    COMMAND_NAME("out"),
    COMMAND_FLAGS(PROC_FLAG_MASK_DECIMALS),
    COMPILE(OPCODE_FLAG(PROC_COMMAND_OUT)),
    PROCESS({
        unsigned char flag = compiler_next_char(thou);
        unsigned long long flag_size = PROC_FLAG_SIZES[flag];
        
        unsigned long long buffer = 0;
    
        s_processor_stack_top(thou, &buffer, flag_size);
    
        if(thou -> state != PROC_STATE_RUNNING) return;
            
        unsigned long long mask = (unsigned long long)1 << flag;
        unsigned long long trimmer = 0;
        
        if(flag_size == sizeof(trimmer)) {
            trimmer = -1;
        } else {
            trimmer = ((unsigned long long)1 << (flag_size * 8)) - 1;
        }
        
        if(mask & PROC_FLAG_MASK_SIGNED_INTS) {
            // Extending signed value if needed
            
            if(buffer & ((unsigned long long)1 << (flag_size * 8 - 1))) {
                buffer |= (~trimmer);
            } else {
                buffer &= trimmer;
            }
            printf("%lld\n", buffer);
        } else if(mask & PROC_FLAG_MASK_UNSIGNED_INTS) {
            printf("%llu\n", (unsigned long long) buffer & trimmer);
        } else if(flag == PROC_FLAG_FLOAT32) {
            printf("%f\n", *((float*)&buffer));
        } else if(flag == PROC_FLAG_FLOAT64) {
            printf("%lg\n", *((double*)&buffer));
        }
    }),
    DECOMPILE(DECOMPILE_OPCODE_FLAG("out"))
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_HLT),
    COMMAND_NAME("hlt"),
    COMMAND_FLAGS(0),
    COMPILE(OPCODE(PROC_COMMAND_HLT)),
    PROCESS({
        thou -> state = PROC_STATE_HALTED;
    }),
    DECOMPILE(DECOMPILE_OPCODE("hlt"))
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_NO_OPERATION),
    COMMAND_NAME("nop"),
    COMMAND_FLAGS(0),
    COMPILE(OPCODE(PROC_COMMAND_NO_OPERATION)),
    PROCESS({
        asm("nop");
    }),
    DECOMPILE(DECOMPILE_OPCODE("nop"))
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_JUMP),
    COMMAND_NAME("jmp"),
    COMMAND_FLAGS(0),
    COMPILE(
        proc_add_jmp_instruction(thou, PROC_COMMAND_JUMP, PROC_FLAG_NONE);
    ),
    PROCESS({
        unsigned long long argument = proc_read_long_long(thou);
        thou -> registers.rip = argument;
    }),
    DECOMPILE(DECOMPILE_JMP_INSTRUCTION("jmp"))
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_JUMP_IF_ABOVE_EQUAL),
    COMMAND_NAME("jae"),
    COMMAND_FLAGS(PROC_FLAG_MASK_DECIMALS),
    COMPILE(
        proc_add_jmp_instruction(thou, PROC_COMMAND_JUMP_IF_ABOVE_EQUAL, flag);
    ),
    PROCESS({
        unsigned char flag = compiler_next_char(thou);
        if(proc_argument_relation(thou, flag) == -1) {
            thou -> registers.rip += 8;
            break;
        }
        unsigned long long argument = proc_read_long_long(thou);
        thou -> registers.rip = argument;
    }),
    DECOMPILE(DECOMPILE_JMP_INSTRUCTION_FLAG("jae"))
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_JUMP_IF_BELOW_EQUAL),
    COMMAND_NAME("jbe"),
    COMMAND_FLAGS(PROC_FLAG_MASK_DECIMALS),
    COMPILE(
        proc_add_jmp_instruction(thou, PROC_COMMAND_JUMP_IF_BELOW_EQUAL, flag);
    ),
    PROCESS({
        unsigned char flag = compiler_next_char(thou);
        if(proc_argument_relation(thou, flag) == 1) {
            thou -> registers.rip += 8;
            break;
        }
        unsigned long long argument = proc_read_long_long(thou);
        thou -> registers.rip = argument;
    }),
    DECOMPILE(DECOMPILE_JMP_INSTRUCTION_FLAG("jbe"))
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_JUMP_IF_ABOVE),
    COMMAND_NAME("ja"),
    COMMAND_FLAGS(PROC_FLAG_MASK_DECIMALS),
    COMPILE(
        proc_add_jmp_instruction(thou, PROC_COMMAND_JUMP_IF_ABOVE, flag);
    ),
    PROCESS({
        unsigned char flag = compiler_next_char(thou);
        if(proc_argument_relation(thou, flag) != 1) {
            thou -> registers.rip += 8;
            break;
        }
        unsigned long long argument = proc_read_long_long(thou);
        thou -> registers.rip = argument;
    }),
    DECOMPILE(DECOMPILE_JMP_INSTRUCTION_FLAG("ja"))
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_JUMP_IF_BELOW),
    COMMAND_NAME("jb"),
    COMMAND_FLAGS(PROC_FLAG_MASK_DECIMALS),
    COMPILE(
        proc_add_jmp_instruction(thou, PROC_COMMAND_JUMP_IF_BELOW, flag);
    ),
    PROCESS({
        unsigned char flag = compiler_next_char(thou);
        if(proc_argument_relation(thou, flag) != -1) {
            thou -> registers.rip += 8;
            break;
        }
        unsigned long long argument = proc_read_long_long(thou);
        thou -> registers.rip = argument;
    }),
    DECOMPILE(DECOMPILE_JMP_INSTRUCTION_FLAG("jb"))
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_JUMP_IF_EQUALS),
    COMMAND_NAME("je"),
    COMMAND_FLAGS(PROC_FLAG_MASK_DECIMALS),
    COMPILE(
        proc_add_jmp_instruction(thou, PROC_COMMAND_JUMP_IF_EQUALS, flag);
    ),
    PROCESS({
        unsigned char flag = compiler_next_char(thou);
        if(proc_argument_relation(thou, flag) != 0) {
            thou -> registers.rip += 8;
            break;
        }
        unsigned long long argument = proc_read_long_long(thou);
        thou -> registers.rip = argument;
    }),
    DECOMPILE(DECOMPILE_JMP_INSTRUCTION_FLAG("je"))
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_JUMP_IF_NOT_EQUALS),
    COMMAND_NAME("jne"),
    COMMAND_FLAGS(PROC_FLAG_MASK_DECIMALS),
    COMPILE(
        proc_add_jmp_instruction(thou, PROC_COMMAND_JUMP_IF_NOT_EQUALS, flag);
    ),
    PROCESS({
        unsigned char flag = compiler_next_char(thou);
        if(proc_argument_relation(thou, flag) == 0) {
            thou -> registers.rip += 8;
            break;
        }
        unsigned long long argument = proc_read_long_long(thou);
        thou -> registers.rip = argument;
    }),
    DECOMPILE(DECOMPILE_JMP_INSTRUCTION_FLAG("jne"))
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_JUMP_IF_ZERO),
    COMMAND_NAME("jz"),
    COMMAND_FLAGS(PROC_FLAG_MASK_DECIMALS),
    COMPILE(
        proc_add_jmp_instruction(thou, PROC_COMMAND_JUMP_IF_ZERO, flag);
    ),
    PROCESS({
        unsigned char flag = compiler_next_char(thou);
        if(!proc_argument_zero(thou, flag)) {
            thou -> registers.rip += 8;
            break;
        }
        if(thou -> state != PROC_STATE_RUNNING) return;
        unsigned long long argument = proc_read_long_long(thou);
        thou -> registers.rip = argument;
    }),
    DECOMPILE(DECOMPILE_JMP_INSTRUCTION_FLAG("jz"))
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_JUMP_IF_NOT_ZERO),
    COMMAND_NAME("jnz"),
    COMMAND_FLAGS(PROC_FLAG_MASK_DECIMALS),
    COMPILE(
        proc_add_jmp_instruction(thou, PROC_COMMAND_JUMP_IF_NOT_ZERO, flag);
    ),
    PROCESS({
        unsigned char flag = compiler_next_char(thou);
        if(proc_argument_zero(thou, flag)) {
            thou -> registers.rip += 8;
            break;
        }
        if(thou -> state != PROC_STATE_RUNNING) return;
        unsigned long long argument = proc_read_long_long(thou);
        thou -> registers.rip = argument;
    }),
    DECOMPILE(DECOMPILE_JMP_INSTRUCTION_FLAG("jnz"))
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_JUMP_IF_MONDAY),
    COMMAND_NAME("jm"),
    COMMAND_FLAGS(0),
    COMPILE(
        proc_add_jmp_instruction(thou, PROC_COMMAND_JUMP_IF_MONDAY, flag);
    ),
    PROCESS({
        time_t t;
        t = time(NULL);
        struct tm* time_st = localtime(&t);
        if(time_st -> tm_wday != 1) {
            thou -> registers.rip += 8;
            break;
        }
        unsigned long long argument = proc_read_long_long(thou);
        thou -> registers.rip = argument;
    }),
    DECOMPILE(DECOMPILE_JMP_INSTRUCTION("jm"))
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_FLAG_LINE),
    COMMAND_NAME("line"),
    COMMAND_FLAGS(0),
    COMPILE({
        if(!thou -> line_warning_emitted) {
            thou -> line_warning_emitted = true;
            compiler_warning(thou, "'line' instructions are ignored in source code. "
                                    "If this file is decompiled, use '--decompile-no-line-flags'"
                                    "flag to ignore 'line' instructions. This warning will "
                                    "be emitted once.");
        }
    }),
    PROCESS({
        thou -> debug_line = (int)proc_read_integer(thou, sizeof(int));
    }),
    DECOMPILE({
        int line = (int)decompiler_read_integer(thou, sizeof(int));
        fprintf(thou -> config -> output_file, "line %d", line);
    })
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_WINDOW),
    COMMAND_NAME("win"),
    COMMAND_FLAGS(0),
    COMPILE({
        char command = PROC_COMMAND_WINDOW;
            
        compiler_write_byte(thou, command);
    
        u_compiler_number width = read_number(thou, true);
    
        if(width.signed_value < 8 || width.signed_value > 128) {
            compiler_error(thou, "invalid window width");
            return;
        }
    
        compiler_skip_whitespaces(thou);
    
        u_compiler_number height = read_number(thou, true);
    
        if(height.signed_value < 8 || height.signed_value > 128) {
            compiler_error(thou, "invalid window height");
            return;
        }
    
        compiler_write_number(thou, PROC_FLAG_INT16, width);
        compiler_write_number(thou, PROC_FLAG_INT16, height);
    }),
    PROCESS({
    
        if(thou -> video_memory -> has_window) {
            proc_error(thou, PROC_ERROR_WINDOW_ALREADY_OPEN);
            return;
        }
    
        int width = (int)proc_read_integer(thou, 2);
        int height = (int)proc_read_integer(thou, 2);
        proc_vm_init_window(thou -> video_memory, width, height, thou -> memory + thou -> video_memory_offset);
    }),
    DECOMPILE({
        int width = (int)decompiler_read_integer(thou, 2);
        int height = (int)decompiler_read_integer(thou, 2);
    
        fprintf(thou -> config -> output_file, "win %d %d", width, height);
    })
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_DRAW),
    COMMAND_NAME("draw"),
    COMMAND_FLAGS(0),
    COMPILE({
        char command = PROC_COMMAND_DRAW;
            
        compiler_write_byte(thou, command);
    }),
    PROCESS({
        if(!thou -> video_memory -> has_window) {
            proc_error(thou, PROC_ERROR_NO_WINDOW);
            return;
        }
    
        proc_vm_redraw(thou -> video_memory);
    
        if(thou -> video_memory -> quit_event_emitted) {
            proc_error(thou, PROC_STATE_HALTED);
        };
    }),
    DECOMPILE({
        fprintf(thou -> config -> output_file, "draw");
    })
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_CALL),
    COMMAND_NAME("call"),
    COMMAND_FLAGS(0),
    COMPILE({
        proc_add_jmp_instruction(thou, PROC_COMMAND_CALL, PROC_FLAG_NONE);
    }),
    PROCESS({
        unsigned long long argument = proc_read_integer(thou, 8);
        unsigned long long return_address = thou -> registers.rip;
        thou -> registers.rip = argument;
        s_processor_stack_push(thou, &return_address, 8);
    }),
    DECOMPILE({
        fprintf(thou -> config -> output_file, "call 0x%08llx", decompiler_read_integer(thou, 8));
    })
)

ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_RET),
    COMMAND_NAME("ret"),
    COMMAND_FLAGS(0),
    COMPILE({
        char command = PROC_COMMAND_RET;
            
        compiler_write_byte(thou, command);
    }),
    PROCESS({
        unsigned long long return_address = 0;
        s_processor_stack_pop(thou, &return_address, 8);
        thou -> registers.rip = return_address;
    }),
    DECOMPILE(DECOMPILE_OPCODE("ret"))
)


ADD_COMMAND
(
    ENUM_NAME(PROC_COMMAND_MEOW),
    COMMAND_NAME("meow"),
    COMMAND_FLAGS(0),
    COMPILE(OPCODE(PROC_COMMAND_MEOW)),
    PROCESS({
    
    unsigned long long amount;
    s_processor_stack_pop(thou, &amount, 8);
    for(int i = 0; i < amount; i++) {
        printf("meow\n");
        system("say мяу");
    }
    }),
    DECOMPILE(DECOMPILE_OPCODE("meow"))
)

#undef ENUM_NAME
#undef COMMAND_NAME
#undef COMMAND_FLAGS
#undef COMPILE
#undef PROCESS
#undef CONVERT
#undef OPCODE

#undef ADD_COMMAND_TRAILING
#undef ADD_COMMAND
