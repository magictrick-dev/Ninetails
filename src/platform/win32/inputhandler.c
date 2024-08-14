#include <platform/win32/inputhandler.h>

static input_state states[2];
static input_state *current_state;
static input_state *previous_state;
static u8           virtual_map[256];

input_state*    
get_current_input_state()
{

    return current_state;

}

input_state*    
get_previous_input_state()
{

    return previous_state;

}

void            
swap_input_states()
{

    input_state *temp = current_state;
    current_state = previous_state;
    previous_state = temp;
    return;

}

u32             
convert_keycode(u32 code)
{

    u32 result = virtual_map[code];
    return result;

}

void            
initialize_input_states()
{

    static b32 virtual_initialized = false;

    // Only needs to initialize once.
    if (virtual_initialized == true) return;
    virtual_initialized = true;

    current_state = &states[0];
    previous_state = &states[1];

    virtual_map[0x00] = NxKeyNull;
    virtual_map[0x01] = NxKeyNull;
    virtual_map[0x02] = NxKeyNull;
    virtual_map[0x03] = NxKeyNull;
    virtual_map[0x04] = NxKeyNull;
    virtual_map[0x05] = NxKeyNull;
    virtual_map[0x06] = NxKeyNull;
    virtual_map[0x07] = NxKeyNull;
    virtual_map[0x08]       = NxKeyBack;
    virtual_map[0x09]       = NxKeyTab;
    virtual_map[0x0a] = NxKeyNull;
    virtual_map[0x0b] = NxKeyNull;
    virtual_map[0x0c] = NxKeyNull;
    virtual_map[0x0d]       = NxKeyEnter;
    virtual_map[0x0e] = NxKeyNull;
    virtual_map[0x0f] = NxKeyNull;
    virtual_map[0x10]       = NxKeyShift;
    virtual_map[0x11]       = NxKeyControl;
    virtual_map[0x12]       = NxKeyAlt;
    virtual_map[0x13] = NxKeyNull;
    virtual_map[0x14] = NxKeyNull;
    virtual_map[0x15] = NxKeyNull;
    virtual_map[0x16] = NxKeyNull;
    virtual_map[0x17] = NxKeyNull;
    virtual_map[0x18] = NxKeyNull;
    virtual_map[0x19] = NxKeyNull;
    virtual_map[0x1a] = NxKeyNull;
    virtual_map[0x1b]       = NxKeyEscape;
    virtual_map[0x1c] = NxKeyNull;
    virtual_map[0x1d] = NxKeyNull;
    virtual_map[0x1e] = NxKeyNull;
    virtual_map[0x1f] = NxKeyNull;
    virtual_map[0x20]       = NxKeySpace;
    virtual_map[0x21] = NxKeyNull;
    virtual_map[0x22] = NxKeyNull;
    virtual_map[0x23] = NxKeyNull;
    virtual_map[0x24] = NxKeyNull;
    virtual_map[0x25]       = NxKeyLeft;
    virtual_map[0x26]       = NxKeyUp;
    virtual_map[0x27]       = NxKeyRight;
    virtual_map[0x28]       = NxKeyDown;
    virtual_map[0x29] = NxKeyNull;
    virtual_map[0x2a] = NxKeyNull;
    virtual_map[0x2b] = NxKeyNull;
    virtual_map[0x2c] = NxKeyNull;
    virtual_map[0x2d] = NxKeyNull;
    virtual_map[0x2e]       = NxKeyDelete;
    virtual_map[0x2f] = NxKeyNull;
    virtual_map[0x30]       = NxKey0;
    virtual_map[0x31]       = NxKey1;
    virtual_map[0x32]       = NxKey2;
    virtual_map[0x33]       = NxKey3;
    virtual_map[0x34]       = NxKey4;
    virtual_map[0x35]       = NxKey5;
    virtual_map[0x36]       = NxKey6;
    virtual_map[0x37]       = NxKey7;
    virtual_map[0x38]       = NxKey8;
    virtual_map[0x39]       = NxKey9;
    virtual_map[0x3a] = NxKeyNull;
    virtual_map[0x3b] = NxKeyNull;
    virtual_map[0x3c] = NxKeyNull;
    virtual_map[0x3d] = NxKeyNull;
    virtual_map[0x3e] = NxKeyNull;
    virtual_map[0x3f] = NxKeyNull;
    virtual_map[0x40] = NxKeyNull;
    virtual_map[0x41]       = NxKeyA;
    virtual_map[0x42]       = NxKeyB;
    virtual_map[0x43]       = NxKeyC;
    virtual_map[0x44]       = NxKeyD;
    virtual_map[0x45]       = NxKeyE;
    virtual_map[0x46]       = NxKeyF;
    virtual_map[0x47]       = NxKeyG;
    virtual_map[0x48]       = NxKeyH;
    virtual_map[0x49]       = NxKeyI;
    virtual_map[0x4a]       = NxKeyJ;
    virtual_map[0x4b]       = NxKeyK;
    virtual_map[0x4c]       = NxKeyL;
    virtual_map[0x4d]       = NxKeyM;
    virtual_map[0x4e]       = NxKeyN;
    virtual_map[0x4f]       = NxKeyO;
    virtual_map[0x50]       = NxKeyP;
    virtual_map[0x51]       = NxKeyQ;
    virtual_map[0x52]       = NxKeyR;
    virtual_map[0x53]       = NxKeyS;
    virtual_map[0x54]       = NxKeyT;
    virtual_map[0x55]       = NxKeyU;
    virtual_map[0x56]       = NxKeyV;
    virtual_map[0x57]       = NxKeyW;
    virtual_map[0x58]       = NxKeyX;
    virtual_map[0x59]       = NxKeyY;
    virtual_map[0x5a]       = NxKeyZ;
    virtual_map[0x5b] = NxKeyNull;
    virtual_map[0x5c] = NxKeyNull;
    virtual_map[0x5d] = NxKeyNull;
    virtual_map[0x5e] = NxKeyNull;
    virtual_map[0x5f] = NxKeyNull;
    virtual_map[0x60]       = NxKey0;
    virtual_map[0x61]       = NxKey1;
    virtual_map[0x62]       = NxKey2;
    virtual_map[0x63]       = NxKey3;
    virtual_map[0x64]       = NxKey4;
    virtual_map[0x65]       = NxKey5;
    virtual_map[0x66]       = NxKey6;
    virtual_map[0x67]       = NxKey7;
    virtual_map[0x68]       = NxKey8;
    virtual_map[0x69]       = NxKey9;
    virtual_map[0x6a] = NxKeyNull;
    virtual_map[0x6b] = NxKeyNull;
    virtual_map[0x6c] = NxKeyNull;
    virtual_map[0x6d] = NxKeyNull;
    virtual_map[0x6e] = NxKeyNull;
    virtual_map[0x6f] = NxKeyNull;
    virtual_map[0x70]       = NxKeyF1;
    virtual_map[0x71]       = NxKeyF2;
    virtual_map[0x72]       = NxKeyF3;
    virtual_map[0x73]       = NxKeyF4;
    virtual_map[0x74]       = NxKeyF5;
    virtual_map[0x75]       = NxKeyF6;
    virtual_map[0x76]       = NxKeyF7;
    virtual_map[0x77]       = NxKeyF8;
    virtual_map[0x78]       = NxKeyF10;
    virtual_map[0x79]       = NxKeyF11;
    virtual_map[0x7a]       = NxKeyF12;
    virtual_map[0x7b]       = NxKeyNull;
    virtual_map[0x7c] = NxKeyNull;
    virtual_map[0x7d] = NxKeyNull;
    virtual_map[0x7e] = NxKeyNull;
    virtual_map[0x7f] = NxKeyNull;
    virtual_map[0x80] = NxKeyNull;
    virtual_map[0x81] = NxKeyNull;
    virtual_map[0x82] = NxKeyNull;
    virtual_map[0x83] = NxKeyNull;
    virtual_map[0x84] = NxKeyNull;
    virtual_map[0x85] = NxKeyNull;
    virtual_map[0x86] = NxKeyNull;
    virtual_map[0x87] = NxKeyNull;
    virtual_map[0x88] = NxKeyNull;
    virtual_map[0x89] = NxKeyNull;
    virtual_map[0x8a] = NxKeyNull;
    virtual_map[0x8b] = NxKeyNull;
    virtual_map[0x8c] = NxKeyNull;
    virtual_map[0x8d] = NxKeyNull;
    virtual_map[0x8e] = NxKeyNull;
    virtual_map[0x8f] = NxKeyNull;
    virtual_map[0x90] = NxKeyNull;
    virtual_map[0x91] = NxKeyNull;
    virtual_map[0x92] = NxKeyNull;
    virtual_map[0x93] = NxKeyNull;
    virtual_map[0x94] = NxKeyNull;
    virtual_map[0x95] = NxKeyNull;
    virtual_map[0x96] = NxKeyNull;
    virtual_map[0x97] = NxKeyNull;
    virtual_map[0x98] = NxKeyNull;
    virtual_map[0x99] = NxKeyNull;
    virtual_map[0x9a] = NxKeyNull;
    virtual_map[0x9b] = NxKeyNull;
    virtual_map[0x9c] = NxKeyNull;
    virtual_map[0x9d] = NxKeyNull;
    virtual_map[0x9e] = NxKeyNull;
    virtual_map[0x9f] = NxKeyNull;
    virtual_map[0xa0]       = NxKeyShift; // Left shift
    virtual_map[0xa1]       = NxKeyShift; // Right shift
    virtual_map[0xa2]       = NxKeyControl; // Left control
    virtual_map[0xa3]       = NxKeyControl; // Right control
    virtual_map[0xa4]       = NxKeyAlt; // Left alt
    virtual_map[0xa5]       = NxKeyAlt; // Right alt
    virtual_map[0xa6] = NxKeyNull;
    virtual_map[0xa7] = NxKeyNull;
    virtual_map[0xa8] = NxKeyNull;
    virtual_map[0xa9] = NxKeyNull;
    virtual_map[0xaa] = NxKeyNull;
    virtual_map[0xab] = NxKeyNull;
    virtual_map[0xac] = NxKeyNull;
    virtual_map[0xad] = NxKeyNull;
    virtual_map[0xae] = NxKeyNull;
    virtual_map[0xaf] = NxKeyNull;
    virtual_map[0xb0] = NxKeyNull;
    virtual_map[0xb1] = NxKeyNull;
    virtual_map[0xb2] = NxKeyNull;
    virtual_map[0xb3] = NxKeyNull;
    virtual_map[0xb4] = NxKeyNull;
    virtual_map[0xb5] = NxKeyNull;
    virtual_map[0xb6] = NxKeyNull;
    virtual_map[0xb7] = NxKeyNull;
    virtual_map[0xb8] = NxKeyNull;
    virtual_map[0xb9] = NxKeyNull;
    virtual_map[0xba]       = NxKeySemicolon;
    virtual_map[0xbb]       = NxKeyEqual;
    virtual_map[0xbc]       = NxKeyComma;
    virtual_map[0xbd]       = NxKeyMinus;
    virtual_map[0xbe]       = NxKeyPeriod;
    virtual_map[0xbf]       = NxKeyRightSlash;
    virtual_map[0xc0]       = NxKeyTilde;
    virtual_map[0xc1] = NxKeyNull;
    virtual_map[0xc2] = NxKeyNull;
    virtual_map[0xc3] = NxKeyNull;
    virtual_map[0xc4] = NxKeyNull;
    virtual_map[0xc5] = NxKeyNull;
    virtual_map[0xc6] = NxKeyNull;
    virtual_map[0xc7] = NxKeyNull;
    virtual_map[0xc8] = NxKeyNull;
    virtual_map[0xc9] = NxKeyNull;
    virtual_map[0xca] = NxKeyNull;
    virtual_map[0xcb] = NxKeyNull;
    virtual_map[0xcc] = NxKeyNull;
    virtual_map[0xcd] = NxKeyNull;
    virtual_map[0xce] = NxKeyNull;
    virtual_map[0xcf] = NxKeyNull;
    virtual_map[0xd0] = NxKeyNull;
    virtual_map[0xd1] = NxKeyNull;
    virtual_map[0xd2] = NxKeyNull;
    virtual_map[0xd3] = NxKeyNull;
    virtual_map[0xd4] = NxKeyNull;
    virtual_map[0xd5] = NxKeyNull;
    virtual_map[0xd6] = NxKeyNull;
    virtual_map[0xd7] = NxKeyNull;
    virtual_map[0xd8] = NxKeyNull;
    virtual_map[0xd9] = NxKeyNull;
    virtual_map[0xda] = NxKeyNull;
    virtual_map[0xdb]       = NxKeyLeftBracket;
    virtual_map[0xdc]       = NxKeyLeftSlash;
    virtual_map[0xdd]       = NxKeyRightBracket;
    virtual_map[0xde]       = NxKeyQuote;
    virtual_map[0xdf] = NxKeyNull;
    virtual_map[0xe0] = NxKeyNull;
    virtual_map[0xe1] = NxKeyNull;
    virtual_map[0xe2] = NxKeyNull;
    virtual_map[0xe3] = NxKeyNull;
    virtual_map[0xe4] = NxKeyNull;
    virtual_map[0xe5] = NxKeyNull;
    virtual_map[0xe6] = NxKeyNull;
    virtual_map[0xe7] = NxKeyNull;
    virtual_map[0xe8] = NxKeyNull;
    virtual_map[0xe9] = NxKeyNull;
    virtual_map[0xea] = NxKeyNull;
    virtual_map[0xeb] = NxKeyNull;
    virtual_map[0xec] = NxKeyNull;
    virtual_map[0xed] = NxKeyNull;
    virtual_map[0xee] = NxKeyNull;
    virtual_map[0xef] = NxKeyNull;
    virtual_map[0xf0] = NxKeyNull;
    virtual_map[0xf1] = NxKeyNull;
    virtual_map[0xf2] = NxKeyNull;
    virtual_map[0xf3] = NxKeyNull;
    virtual_map[0xf4] = NxKeyNull;
    virtual_map[0xf5] = NxKeyNull;
    virtual_map[0xf6] = NxKeyNull;
    virtual_map[0xf7] = NxKeyNull;
    virtual_map[0xf8] = NxKeyNull;
    virtual_map[0xf9] = NxKeyNull;
    virtual_map[0xfa] = NxKeyNull;
    virtual_map[0xfb] = NxKeyNull;
    virtual_map[0xfc] = NxKeyNull;
    virtual_map[0xfd] = NxKeyNull;
    virtual_map[0xfe] = NxKeyNull;
    virtual_map[0xff] = NxKeyNull;

}
