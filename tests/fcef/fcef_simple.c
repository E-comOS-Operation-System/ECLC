#include "../../include/fcef/fcef.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


eclc_output_t *generate_simple_program() {
    eclc_output_t *output = malloc(sizeof(eclc_output_t));
    if (!output) return NULL;
    
    memset(output, 0, sizeof(eclc_output_t));

    output->entry_point = 0x400000;  
    output->text_addr = 0x400000;    
    output->rodata_addr = 0x410000;  
    output->data_addr = 0x420000; 
    

    uint8_t code[] = {

        0xFD, 0x7B, 0xBF, 0xA9,  // stp x29, x30, [sp, #-16]!
        
 
        0xE0, 0x03, 0x00, 0x10,  // adr x0, .L.str
        

        0xE1, 0x03, 0x1F, 0xAA,  // mov x1, xzr
        0x00, 0x00, 0x00, 0x94,  // bl printf 
        

        0xFD, 0x7B, 0xC1, 0xA8,  // ldp x29, x30, [sp], #16
        0xC0, 0x03, 0x5F, 0xD6,  // ret
    };
    
    uint8_t rodata[] = "Hello from ECLC compiler!\n";
    
    output->code_size = sizeof(code);
    output->code = malloc(output->code_size);
    memcpy(output->code, code, output->code_size);
    
    output->rodata_size = sizeof(rodata);
    output->rodata = malloc(output->rodata_size);
    memcpy(output->rodata, rodata, output->rodata_size);
    
    return output;
}

eclc_output_t *generate_minimal_program() {
    eclc_output_t *output = malloc(sizeof(eclc_output_t));
    if (!output) return NULL;
    
    memset(output, 0, sizeof(eclc_output_t));

    output->entry_point = 0x400000;
    output->text_addr = 0x400000;

    uint8_t code[] = {
        0xC0, 0x03, 0x5F, 0xD6,  // ret
    };
    
    output->code_size = sizeof(code);
    output->code = malloc(output->code_size);
    memcpy(output->code, code, output->code_size);
    
    return output;
}

int main(int argc, char *argv[]) {
    printf("🔧 ECLC  (Make FCEF file)\n");
    printf("====================================\n\n");
    
    const char *output_file = "eclc_output.fcef";
    if (argc > 1) {
        output_file = argv[1];
    }
    
 
    eclc_output_t *program = generate_minimal_program();
    if (!program) {
        fprintf(stderr, "❌ Cannot make file\n");
        return 1;
    }
    

    printf("📄 File:\n");
    eclc_print_output(program);
    

    printf("\n💾 Save to FCEF: %s\n", output_file);
    if (eclc_save_fcef(program, output_file)) {
        printf("✅ OK\n");
    } else {
        fprintf(stderr, "❌ ERROR\n");
        eclc_free_output(program);
        return 1;
    }
    

    printf("\n🔍 looking...\n");
    FILE *file = fopen(output_file, "rb");
    if (file) {
        fseek(file, 0, SEEK_END);
        long size = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        uint8_t *buffer = malloc(size);
        fread(buffer, 1, size, file);
        fclose(file);
        
        if (fcef_validate(buffer, size)) {
            printf("✅ OK\n");
            
      
            const fcef_header_t *header = (const fcef_header_t *)buffer;
            fcef_print_header(header);
        } else {
            printf("❌ ERROR\n");
        }
        
        free(buffer);
    }
    

    eclc_free_output(program);
    
    printf("\n🎉OK!OK!OK!\n");
    return 0;
}