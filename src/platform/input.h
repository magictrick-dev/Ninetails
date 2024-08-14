#ifndef SRC_PLATFORM_INPUT_H
#define SRC_PLATFORM_INPUT_H
#include <core/definitions.h>

void input_release_all();

bool input_key_is_pressed(u32 key_code);
bool input_key_is_released(u32 key_code);
bool input_key_is_down(u32 key_code);
r64 input_key_time_down(u32 key_code);
r64 input_key_time_up(u32 key_code);

bool input_mouse_button_is_pressed(u32 mouse_code);
bool input_mouse_button_is_released(u32 mouse_code);
bool input_mouse_button_is_down(u32 mouse_code);
r64 input_mouse_button_time_down(u32 mouse_code);
r64 input_mouse_button_time_up(u32 mouse_code);

#define NxMouseLeft        0
#define NxMouseRight       1
#define NxMouseMiddle      2

#define NxKeyA             0
#define NxKeyB             1
#define NxKeyC             2
#define NxKeyD             3
#define NxKeyE             4
#define NxKeyF             5
#define NxKeyG             6
#define NxKeyH             7
#define NxKeyI             8
#define NxKeyJ             9
#define NxKeyK             10
#define NxKeyL             11
#define NxKeyM             12
#define NxKeyN             13
#define NxKeyO             14
#define NxKeyP             15
#define NxKeyQ             16
#define NxKeyR             17
#define NxKeyS             18
#define NxKeyT             19
#define NxKeyU             20
#define NxKeyV             21
#define NxKeyW             22
#define NxKeyX             23
#define NxKeyY             24
#define NxKeyZ             25

#define NxKey0             26
#define NxKey1             27
#define NxKey2             28
#define NxKey3             29
#define NxKey4             30
#define NxKey5             31
#define NxKey6             32
#define NxKey7             33
#define NxKey8             34
#define NxKey9             35

#define NxKeyShift         42
#define NxKeyControl       43
#define NxKeyAlt           44
#define NxKeySpace         45
#define NxKeyBack          46
#define NxKeyDelete        47
#define NxKeyTab           48
#define NxKeyComma         49
#define NxKeyPeriod        50
#define NxKeyRightSlash    51
#define NxKeyLeftSlash     52
#define NxKeySemicolon     53
#define NxKeyQuote         54
#define NxKeyLeftBracket   55
#define NxKeyRightBracket  56
#define NxKeyTilde         57
#define NxKeyMinus         58
#define NxKeyEqual         59
#define NxKeyEnter         60
#define NxKeyEscape        61

#define NxKeyF1            63
#define NxKeyF2            64
#define NxKeyF3            65
#define NxKeyF4            66
#define NxKeyF5            67
#define NxKeyF6            68
#define NxKeyF7            69
#define NxKeyF8            70
#define NxKeyF9            71
#define NxKeyF10           72
#define NxKeyF11           73
#define NxKeyF12           74

#define NxKeyLeft          80
#define NxKeyRight         81
#define NxKeyUp            82
#define NxKeyDown          83

#define NxKeyNull          255


#endif
