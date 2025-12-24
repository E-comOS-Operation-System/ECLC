// eclc_fcef.c
#include "../../include/fcef/fcef.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 从 ECLC 输出创建 FCEF 文件
void *eclc_to_fcef(const eclc_output_t *output, size_t *out_size) {
    if (!output || !output->code || output->code_size == 0) {
        if (out_size) *out_size = 0;
        return NULL;
    }
    
    // 计算总大小: 头部 + 代码 + 只读数据 + 数据
    size_t total_size = sizeof(fcef_header_t) + 
                       output->code_size + 
                       output->rodata_size + 
                       output->data_size;
    
    // 分配内存
    uint8_t *buffer = (uint8_t *)malloc(total_size);
    if (!buffer) {
        if (out_size) *out_size = 0;
        return NULL;
    }
    
    // 初始化头部
    fcef_header_t *header = (fcef_header_t *)buffer;
    fcef_init_header(header);
    
    // 设置文件大小
    header->file_size = total_size;
    
    // 存储布局信息到保留字段
    // 字节 0-3: 入口点
    header->reserved[0] = (output->entry_point >> 24) & 0xFF;
    header->reserved[1] = (output->entry_point >> 16) & 0xFF;
    header->reserved[2] = (output->entry_point >> 8) & 0xFF;
    header->reserved[3] = output->entry_point & 0xFF;
    
    // 字节 4-7: 代码段地址
    header->reserved[4] = (output->text_addr >> 24) & 0xFF;
    header->reserved[5] = (output->text_addr >> 16) & 0xFF;
    header->reserved[6] = (output->text_addr >> 8) & 0xFF;
    header->reserved[7] = output->text_addr & 0xFF;
    
    // 字节 8-11: 数据段地址
    header->reserved[8] = (output->data_addr >> 24) & 0xFF;
    header->reserved[9] = (output->data_addr >> 16) & 0xFF;
    header->reserved[10] = (output->data_addr >> 8) & 0xFF;
    header->reserved[11] = output->data_addr & 0xFF;
    
    // 字节 12-15: 代码段大小
    header->reserved[12] = (output->code_size >> 24) & 0xFF;
    header->reserved[13] = (output->code_size >> 16) & 0xFF;
    header->reserved[14] = (output->code_size >> 8) & 0xFF;
    header->reserved[15] = output->code_size & 0xFF;
    
    // 复制代码段
    uint8_t *ptr = buffer + sizeof(fcef_header_t);
    memcpy(ptr, output->code, output->code_size);
    ptr += output->code_size;
    
    // 复制只读数据段
    if (output->rodata && output->rodata_size > 0) {
        memcpy(ptr, output->rodata, output->rodata_size);
        ptr += output->rodata_size;
    }
    
    // 复制数据段
    if (output->data && output->data_size > 0) {
        memcpy(ptr, output->data, output->data_size);
    }
    
    if (out_size) {
        *out_size = total_size;
    }
    
    return buffer;
}

// 保存 ECLC 输出为 FCEF 文件
bool eclc_save_fcef(const eclc_output_t *output, const char *filename) {
    if (!output || !filename) {
        return false;
    }
    
    size_t file_size;
    void *fcef_data = eclc_to_fcef(output, &file_size);
    
    if (!fcef_data) {
        return false;
    }
    
    FILE *file = fopen(filename, "wb");
    if (!file) {
        free(fcef_data);
        return false;
    }
    
    size_t written = fwrite(fcef_data, 1, file_size, file);
    fclose(file);
    free(fcef_data);
    
    return written == file_size;
}

// 打印 ECLC 输出信息
void eclc_print_output(const eclc_output_t *output) {
    if (!output) {
        printf("NULL output\n");
        return;
    }
    
    printf("┌─────────────────────────────────────────────┐\n");
    printf("│             ECLC 编译器输出                 │\n");
    printf("├─────────────────────────────────────────────┤\n");
    printf("│ 入口点:     0x%08X                     │\n", output->entry_point);
    printf("│ 代码段:     0x%08X (+%zu 字节)          │\n", 
           output->text_addr, output->code_size);
    printf("│ 只读数据:   0x%08X (+%zu 字节)          │\n", 
           output->rodata_addr, output->rodata_size);
    printf("│ 数据段:     0x%08X (+%zu 字节)          │\n", 
           output->data_addr, output->data_size);
    printf("│ BSS段:      (+%zu 字节)                   │\n", output->bss_size);
    printf("│ 总代码大小: %zu 字节                    │\n", output->code_size);
    printf("│ 总数据大小: %zu 字节                    │\n", 
           output->rodata_size + output->data_size);
    printf("└─────────────────────────────────────────────┘\n");
}

// 释放 ECLC 输出
void eclc_free_output(eclc_output_t *output) {
    if (!output) return;
    
    if (output->code) free(output->code);
    if (output->data) free(output->data);
    if (output->rodata) free(output->rodata);
    if (output->bss) free(output->bss);
    
    free(output);
}