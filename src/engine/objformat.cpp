#include <engine/objformat.h>

typedef struct wavefront_parser         wavefront_parser;
typedef enum class wavefront_entry_type wavefront_entry_type;
typedef struct wavefront_vertex         wavefront_vertex;
typedef struct wavefront_normal         wavefront_normal;
typedef struct wavefront_texturecoord   wavefront_texturecoord;
typedef struct wavefront_node           wavefront_node;

static inline wavefront_entry_type      get_entry_type(wavefront_parser *parser);
static inline void                      move_to_next_entry(wavefront_parser *parser);
static inline void                      move_until_character(wavefront_parser *parser);

typedef struct wavefront_parser
{
    ccptr   source;
    u64     offset;
    u32     vertices;
    u32     normals;
    u32     texturecoords;
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

typedef struct wavefront_vertex
{
    vec3 position;
    vec3 color;
} wavefront_vertex;

typedef struct wavefront_normal
{
    vec3 normal;
} wavefront_normal;

typedef struct wavefront_texturecoord
{
    vec2 coordinate;
} wavefront_texturecoord;

typedef struct wavefront_face
{
    i32 vertex_index;
    i32 normal_index;
    i32 texturecoord_index;
} wavefront_face;

typedef struct wavefront_node
{

    wavefront_entry_type type;
    wavefront_node *next;

    union
    {
        wavefront_vertex vertex;
        wavefront_normal normal;
        wavefront_texturecoord texturecoord;
        wavefront_face face;
    };

} wavefront_node;

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

    char current = parser->source[parser->offset];
    while (current != '\0')
    {
            
        if (current == '\n') break;
        current = parser->source[++parser->offset];

    }

    if (current != '\0') parser->offset++;
    return;

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
        
        wavefront_entry_type type = get_entry_type(&parser);

        switch (type)
        {

            case wavefront_entry_type::EMPTY:
            case wavefront_entry_type::COMMENT:
            {

                move_to_next_entry(&parser);
                continue;

            } break;

            case wavefront_entry_type::INVALID:
            {

                printf("Encountered invalid entry.\n");
                continue;

            } break;

            case wavefront_entry_type::ENDFILE:
            {

                continue;       

            } break;

        };

    }

    return true;
}


