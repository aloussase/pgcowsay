#include <unistd.h>
#include <string.h>

#include "postgres.h"

#include "varatt.h"
#include "catalog/pg_type_d.h"
#include "fmgr.h"
#include "nodes/pg_list.h"
#include "parser/parse_func.h"
#include "utils/builtins.h"
#include "utils/datum.h"
#include "utils/json.h"
#include "utils/memutils.h"
#include "utils/regproc.h"

PG_MODULE_MAGIC;

static MemoryContext PGCowsayContext = NULL;


void _PG_init(void)
{
    PGCowsayContext = AllocSetContextCreate(
            TopMemoryContext,
            "PGCowsayContext",
            ALLOCSET_DEFAULT_SIZES);
}

PG_FUNCTION_INFO_V1(pgcowsay_say);
Datum pgcowsay_say(PG_FUNCTION_ARGS)
{
    MemoryContext oldctx;
    char *message;
    char *program;
    char *command;
    FILE *pipe;
    char buffer[4096];
    char *output;
    size_t output_len;
    text *result;
    int success;
    size_t program_len;
    size_t message_len;
    size_t command_len;

    oldctx = MemoryContextSwitchTo(PGCowsayContext);

    program = "cowsay";
    program_len = strlen(program);

    message = TextDatumGetCString(PG_GETARG_DATUM(0));
    message_len = strlen(message);

    command = palloc(program_len + message_len + 2);
    command_len = program_len + message_len + 2;

    memset(command, 0, command_len);
    memcpy(command, program, program_len);
    memcpy(command + program_len, " ", 1);
    memcpy(command + program_len + 1, message, message_len);

    success = 1;

    pipe = popen(command, "r");
    if (pipe == NULL) {
        success = 0;
        goto exit;
    }

    output = palloc(1);
    output_len = 0;

    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        size_t read = strlen(buffer);
        output = repalloc(output, output_len + read + 1);
        memcpy(output + output_len, buffer, read);
        output_len += read;
    }

    output[output_len] = '\0';
    pclose(pipe);

    result = cstring_to_text(output);

exit:

    MemoryContextSwitchTo(oldctx);

    if (success) {
        PG_RETURN_TEXT_P(result);
    } else {
        result = cstring_to_text("no cows");
        PG_RETURN_TEXT_P(result);
    }

    MemoryContextReset(PGCowsayContext);
}
