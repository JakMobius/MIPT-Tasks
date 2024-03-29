#include "bonk/compiler.hpp"
#include "bonk/bonk_cli_arguments.hpp"
#include "bonk/backend/ede/ede_backend.hpp"
#include "bonk/backend/x86/x86_backend.hpp"
#include "bonk/backend/x86/test/encoding_tester.hpp"

void init_fatal_error(const char *format, ...) {
    va_list ap;

    va_start (ap, format);
    fprintf (stderr, "fatal error: ");
    vfprintf (stderr, format, ap);
    va_end (ap);
    fputc ('\n', stderr);
}

void warning(const char* reason) {
    fprintf(stderr, "warning: %s\n", reason);
}

int main(int argc, const char* argv[]) {

    if(argc <= 1) {
        args::argument_list::print_help(args::TYPES);
        return 0;
    }

    const char* input = argv[1];

    args::argument_list* arguments = args::argument_list::create(argc, argv, args::TYPES);

    if(!arguments) {
        printf("fatal error: out of memory");
        return -1;
    }

    args::argument* help_flag = arguments->search(&args::TYPES[E_ARGV_NAME_HELP]);
    args::argument* ast_flag = arguments->search(&args::TYPES[E_ARGV_NAME_AST]);
    args::argument* output_file_flag = arguments->search(&args::TYPES[E_ARGV_NAME_OUTPUT_FILE]);
    args::argument* target_flag = arguments->search(&args::TYPES[E_ARGV_NAME_TARGET]);
    args::argument* log_file_flag = arguments->search(&args::TYPES[E_ARGV_NAME_LOG_FILE]);

    if(help_flag != nullptr || input[0] == '-') {
        args::argument_list::print_help(args::TYPES);
        arguments->release();
        return 0;
    }

    const char* output_file_path = nullptr;

    if(output_file_flag) output_file_path = output_file_flag->value;
    if(!output_file_path) output_file_path = "out";

    bonk::compiler_config config = {};
    config.error_file = stderr;

    if(!target_flag || strcmp(target_flag->value, "ede") == 0) {
        config.compile_backend = bonk::ede_backend::backend::create();
    } else if(strcmp(target_flag->value, "x86") == 0) {
        config.compile_backend = bonk::x86_backend::backend::create();
    } else {
        init_fatal_error("unknown compile target: %s", target_flag->value);
        arguments->release();
        return 1;
    }

    FILE* output_file = fopen(output_file_path, "wb");
    if(!output_file) {
        init_fatal_error("failed to open input file\n");
        config.compile_backend->release();
        arguments->release();
        return 1;
    }

    if(log_file_flag) {
        config.listing_file = fopen(log_file_flag->value, "w");
        if(!config.listing_file) {
            warning("failed to open log file\n");
            config.listing_file = nullptr;
        }
    }

    bonk::compiler* compiler = bonk::compiler::create(&config);

    if(!compiler) {
        init_fatal_error("failed to initialize compiler\n");
        config.compile_backend->release();
        arguments->release();
        fclose(output_file);
        return -1;
    }

    bonk::tree_node_list<bonk::tree_node*>* ast = compiler->get_ast_of_file_at_path(input);

    if(ast) {
        if(ast_flag) {
            json_serializer* serializer = json_serializer::create(output_file);
            ast->serialize(serializer);
            serializer->release();
        } else {
            compiler->compile_ast(ast, output_file);
        }
    }

//    bonk::x86_backend::test_encodings(output_file);

    fclose(output_file);

    if(!ast_flag) {
        if(chmod(output_file_path, 511) < 0) {
            warning("failed to add execution permissions to file\n");
        }
    }

    arguments->release();
    config.compile_backend->release();

    if(compiler->state) {
        compiler->release();
        return 1;
    }

    compiler->release();
    return 0;
}
