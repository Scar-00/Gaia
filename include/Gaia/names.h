#ifndef _GAIA_NAMES_H_
#define _GAIA_NAMES_H_

#include <Gaia/gaia.h>

//io.h
#define file_read gaia_file_read
#define file_close gaia_file_close
#define file_check gaia_file_check
#define setting_file_set gaia_setting_file_set
#define setting_write gaia_setting_write
#define setting_u32_get gaia_setting_u32_get
#define setting_u32_set gaia_setting_u32_set
#define setting_s32_get gaia_setting_s32_get
#define setting_s32_set gaia_setting_s32_set
#define setting_f32_get gaia_setting_f32_get
#define setting_f32_set gaia_setting_f32_set
#define setting_string_get gaia_setting_string_get
#define setting_string_set gaia_setting_string_set

//array.h
#define array_header gaia_array_header
#define array_length gaia_array_length
#define array_capacity gaia_array_capacity
#define array gaia_array
#define array_create gaia_array_create
#define array_destroy gaia_array_destroy
#define array_free gaia_array_free
#define array_pushback gaia_array_pushback
#define array_pop gaia_array_pop
#define array_put gaia_array_put
#define array_loop gaia_array_loop

//memory.h
#define arena_init gaia_arena_init

//string.h
#define string_init gaia_string_init
#define string_init_va gaia_string_init_va
#define string_cmp gaia_string_cmp
#define string_char_append gaia_string_char_append
#define string_append gaia_string_append

#define stringstream_init gaia_stringstream_init 
#define stringstream_init_va gaia_stringstream_init_va
#define stringstream_destroy gaia_stringstream_destroy
#define stringstream_cmp gaia_stringstream_cmp
#define stringstream_char_append gaia_stringstream_char_append
#define stringstream_append gaia_stringstream_append

#define stringstream_to_string gaia_stringstream_to_string
#define string_to_stringstream gaia_string_to_stringstream

//stack.h

#define stack_header gaia_stack_header
#define stack_length gaia_stack_length
#define stack_capacity gaia_stack_capacity
#define stack gaia_stack
#define stack_create gaia_stack_create
#define stack_destroy gaia_stack_destroy
#define stack_free gaia_stack_free
#define stack_push gaia_stack_push
#define stack_pop gaia_stack_pop
#define stack_peek gaia_stack_peek

#endif