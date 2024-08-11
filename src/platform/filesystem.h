#ifndef SRC_PLATFORM_FILESYSTEM_H
#define SRC_PLATFORM_FILESYSTEM_H
#include <core/definitions.h>

b32         file_exists(ccptr file_path);
b32         file_ready(ccptr file_path);
b32         file_is_directory(ccptr file_path);
b32         file_is_file(ccptr file_path);
u64         file_size(ccptr file_path);
u64         file_read_all(ccptr file_path, vptr buffer, u64 buffer_size);
u64         file_write_all(ccptr file_path, vptr buffer, u64 buffer_size);
b32         file_copy_all(ccptr source, ccptr destination);
u64         file_last_write_time(ccptr file_path);

vptr        file_stream_handle_create(ccptr file_path, u32 file_context);
void        file_stream_handle_close(vptr handle);
void        file_stream_reset_offset(vptr handle);
void        file_stream_set_offset_rel(vptr handle, u64 relative_offset);
void        file_stream_set_offset_abs(vptr handle, u64 absolute_offset);
u64         file_stream_get_offset(vptr handle);
u64         file_stream_read(vptr handle, u64 read_size, vptr dest, u64 dest_size);
b32         file_stream_is_eof(vptr handle);

#endif
