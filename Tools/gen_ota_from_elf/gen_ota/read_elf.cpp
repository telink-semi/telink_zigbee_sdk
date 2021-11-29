

#include "read_elf.h"
#include <windows.h>
#include <shlwapi.h>

#include <iostream>

// add pre-define Macro: _CRT_SECURE_NO_WARNINGS

#pragma comment(lib, "Shlwapi.lib")

ElfReader::ElfReader() : 
    elf_file_data(NULL), 
    elf_file_length(0)
{
}

ElfReader::~ElfReader()
{
    if (elf_file_data != NULL) {
        delete elf_file_data;
    }
}

int ElfReader::read_elf_file(char *file_path)
{
    ////if (!PathFileExists(file_path)) {
    //    return -1;
    //}

    FILE *fp;
    if(fopen_s(&fp, file_path, "rb")){
        return -2;
    }else{
        fseek(fp, 0, SEEK_END);
        int length = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        elf_file_data = new u_char[length];
        elf_file_length = length;
        for(int i=0;i<elf_file_length;) {
            int remain = elf_file_length - i;
            i += fread(&elf_file_data[i], 1, (remain > 4096) ? 4096 : remain, fp);
        }
        fclose(fp);
    }

    return 0;
}

bool ElfReader::read_program_headers()
{
    u_char *data = elf_file_data;
    elf32_phdr *pheader;
    elf32_Ehdr *header;
    int num;
    header = (elf32_Ehdr*)data;
    num = header->e_phnum;

    for(int i=0;i<num;i++) {
        pheader = (elf32_phdr*)(data + sizeof(*header)+ (i) * sizeof(*pheader));
        elf_info.program_headers.push_back(*pheader);
    }

    //printf("elf_info: headers size: %d\n", elf_info.program_headers.size());
    // caculate file size before generating bin file.
    unsigned int bin_size = 0;
    for(unsigned int i=0; i<elf_info.program_headers.size(); i++) {
        elf32_phdr *hdr = &elf_info.program_headers[i];
        if (hdr->p_flags & PERMISSION_X) {
            unsigned int size = hdr->p_paddr + hdr->p_filesz;
            if (size > bin_size) {
                bin_size = size;
            }
        }
    }

    elf_info.program_data = new unsigned char[bin_size];
    memset(elf_info.program_data, 0, bin_size);

    for(unsigned int i=0; i<elf_info.program_headers.size(); i++) {
        elf32_phdr *hdr = &elf_info.program_headers[i];
        if (hdr->p_flags & PERMISSION_X) {
            memcpy(&elf_info.program_data[hdr->p_paddr], &elf_file_data[hdr->p_offset], hdr->p_filesz);
        }
    }

    elf_info.program_length = bin_size;

    return true;
}



bool ElfReader::read_sections_info()
{
    u_char *data = elf_file_data;
    elf32_Ehdr *header;
    elf32_Shdr *sheader;
    int num;
    int s_off;
    
    header = (elf32_Ehdr*)data;
    num =  header->e_shnum;
    s_off = header->e_shoff;
    sheader = (elf32_Shdr*)(data + s_off +  sizeof(*sheader) * (header->e_shstrndx));
    u_char *name = (u_char *)(data + sheader->sh_offset);
    for(int i = 0; i < num; ++i)
    {
        sheader = (elf32_Shdr*)(data + i * sizeof(*sheader) + s_off);

        section_info_t section_info;
        section_info.header = *sheader;
        memset(section_info.name, 0, sizeof(section_info.name));
        strcpy_s(section_info.name, (char *)(name + sheader->sh_name));
        elf_info.section_infos.push_back(section_info);
    }
    return true;
}




bool ElfReader::read_symbols_info()
{
    const u_char *data = elf_file_data;
    elf32_Ehdr *header;
    elf32_Shdr *sheader;
    elf32_sym    *symheader;
    int i, j;
    int num;
    int len;
    int s_off;
    u_char *secname;
    u_char *symname = NULL;
    header = (elf32_Ehdr*)data;
    num =  header->e_shnum;
    s_off = header->e_shoff;
    //printf("Total %d section header, start from address 0x%x:\n\n", num,s_off);
    sheader = (elf32_Shdr*)(data + s_off +  sizeof(*sheader) * (header->e_shstrndx ));
    secname = (u_char *)(data + sheader->sh_offset);
    for (i = 0; i < num; ++i)
    {
        sheader = (elf32_Shdr*)(data + i * sizeof(*sheader) + s_off);
        if (strcmp( (char *)(secname+sheader->sh_name), ".strtab")== 0)
        {
            symname = (u_char *)(data + sheader->sh_offset);
        }
    }

    for (i = 0; i < num; ++i)
    {
        sheader = (elf32_Shdr*)(data + i * sizeof(*sheader) + s_off);
		
        char *section_header_name = (char *)secname+sheader->sh_name;

        if (strlen(section_header_name) == 0) {
            continue;
        }

        if (0 != strcmp(section_header_name, ".dynsym") && 
            0 != strcmp(section_header_name, ".symtab")) {
            continue;
        }

        if (sheader->sh_size == 0 || sheader->sh_entsize == 0) {
            continue;
        }

        len = (sheader->sh_size / sheader->sh_entsize);
        
        for ( j = 0; j < len; ++j)
        {
            symheader = (elf32_sym*)(data + sheader->sh_offset + j * (sizeof(*symheader)));
            
            symbol_info_t symbol_info;
            memset(&symbol_info, 0, sizeof(symbol_info));
            symbol_info.sheader = *symheader;
            char *name = section_header_name;
            strcpy_s(symbol_info.section_name, name);
            name = (char *)(symname+symheader->st_name);
            strcpy_s(symbol_info.symbol_name, name);

            elf_info.symbol_infos.push_back(symbol_info);
        }
    }
    return true;
}



int ElfReader::read_constant(char *constant_name, char *data, unsigned int data_len_max)
{
    for (auto& i : elf_info.symbol_infos) {
        auto symheader = &i.sheader;
        if (ELF32_ST_TYPE(symheader->st_info) == STT_NOTYPE) {
            continue;
        }

        if (!string_equals(i.symbol_name, constant_name)) {
            continue;
        }
        unsigned int addr = symheader->st_value;
        unsigned int size = symheader->st_size;
        if (data_len_max < size) {
            size = data_len_max;
        }
        unsigned char *const_data = &elf_info.program_data[addr];
        memcpy(data, const_data, size);
        return size;
    }
    return -1;
}

int is_valid_name_var_len(int len)
{
    return ((len != ELF_INVALID_LEN) && (len != 0));
}

int ElfReader::read_fw_file_name(char *constant_name, char *data, int data_len_max)
{
    int len_no_extend = read_constant(constant_name, data, data_len_max);
    char extend_buf[] = {".bin"};
    if(is_valid_name_var_len(len_no_extend)){
        return len_no_extend;
        len_no_extend -= 1; // end of string
        int total_len = len_no_extend + sizeof(extend_buf);
        if(total_len <= data_len_max){
            memcpy(data + len_no_extend, extend_buf, sizeof(extend_buf));
            return total_len;
        }
    }
    return ELF_INVALID_LEN;
}

bool ElfReader::read_all()
{
    memcpy(&elf_info.header, elf_file_data, sizeof(elf_info.header));         // header

    read_program_headers();
    read_sections_info();
    read_symbols_info();

    return true;
}


