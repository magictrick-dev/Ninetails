#include <engine/objformat.h>

typedef struct wavefront_parser
{
    ccptr source;
    u64 offset;
} wavefront_parser;

typedef enum class wavefront_entry_type
{
    VERTEX,
    NORMAL,
    TCOORD,
    FACE,
    COMMENT,
    EMPTY,
    INVALID,
    ENDFILE,
} wavefront_entry_type;

static inline wavefront_entry_type get_entry_type(wavefront_parser *parser);
static inline void move_to_next_entry(wavefront_parser *parser);
static inline void move_until_character(wavefront_parser *parser);

static inline void 
move_until_character(wavefront_parser *parser)
{

    char c = parser->source[parser->offset];
    while (c == ' ' || c == '\t')
        c = parser->source[++parser->offset];

}

static inline wavefront_entry_type 
get_entry_type(wavefront_parser *parser)
{

    move_until_character(parser);
    char leading_character = parser->source[parser->offset];
    switch (leading_character)
    {
        case '#': return wavefront_entry_type::COMMENT;
        case 'f': return wavefront_entry_type::FACE;
        case 'v':
        {
            char next = parser->source[parser->offset + 1];
            if (next == ' ') return wavefront_entry_type::VERTEX;
            if (next == 'n') return wavefront_entry_type::NORMAL;
            if (next == 't') return wavefront_entry_type::TCOORD;
        };

        case '\r':
        case '\n':  return wavefront_entry_type::EMPTY;
        case '\0':  return wavefront_entry_type::ENDFILE;
        default:    return wavefront_entry_type::INVALID;
    }

}

static inline void 
move_to_next_entry(wavefront_parser *parser)
{

}

b32 
wavefront_object_parse_file(ccptr object_file, wavefront_object_mesh *mesh, memory_arena *arena)
{

    wavefront_parser parser = {0};
    parser.source = object_file;
    parser.offset = 0;

    wavefront_entry_type current_entry_type = get_entry_type(&parser);
    while (current_entry_type != wavefront_entry_type::ENDFILE)
    {

    }

    return true;
}


