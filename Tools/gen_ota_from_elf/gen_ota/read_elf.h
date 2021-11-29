
/* 
* @Author: Xiaokang Yin
* @Date:   2017-05-21 22:03:36
* @Last Modified by:   Xiaokang Yin
* @Last Modified time: 2017-05-21 22:04:49
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <vector>
#include <memory>

using namespace std;

/** e_ident identification indexes */
#define EI_NIDENT 		16		//Size of e_ident[]
#define EI_MAG0     		0		//File identification 	0x7f
#define EI_MAG1     		1		//File identification	'E'
#define EI_MAG2     		2		//File identification	'L'
#define EI_MAG3     		3		//File identification	'F'
#define EI_CLASS    		4		//file class
#define EI_DATA			5		//Data encoding
#define EI_VERSION		6		//File version
#define EI_OSABI			7		//Os
#define EI_ABIVERSION 	8	
#define EI_PAD			9		//Start of padding bytes

/** EI_CLASS */
#define ELFCLASSNONE		0		//Invalid class
#define ELFCLASS32		1		//32-bit objects
#define ELFCLASS64		2		//64-bit objects

/** EI_DATA */
#define ELFDATANONE		0		//Invalid data encoding
#define ELFDATA2LSB		1		//Little endian, the high bit is in low address
#define ELFDATA2MSB 		2		//Big endian, the high bit is in high address

/** EI_VERSION */
#define EV_NONE			0		//Invalid version
#define EV_CURRENT		1		//Current version

/** Common types */
/** 32-bit ELF base types */
typedef unsigned char 	u_char;  			//Unsigned small integer
typedef unsigned int 		elf32_Addr;		//Unsigned program address
typedef unsigned short	elf32_Half;		//Unsigned medium integer
typedef	unsigned int 		elf32_Off;		//Unsigned file offset
typedef signed int 		elf32_Sword;	//Signed large interger
typedef unsigned int 		elf32_Word;		//Unsigned large integer

/** 64-bit ELF base types */
typedef unsigned long long 		elf64_Addr;
typedef	unsigned short 			elf64_Half;
typedef signed short				elf64_SHalf;
typedef unsigned long long		elf64_Off;
typedef signed int 				elf64_Sword;
typedef unsigned int 				elf64_Word;
typedef unsigned long long 		elf64_Xword;
typedef signed long long 			elf64_Sxword;

/** e_type */
#define ET_NONE		0		//No file type
#define ET_REL		1 		//Relocatable file
#define	ET_EXEC		2		//Executable file
#define	ET_DYN		3		//Shared object file
#define ET_CORE		4		//Core file
#define ET_LOPROC	0xff00	//Processor-specific
#define	ET_HIPROC	0xffff	//Processor-specofic

/** e_machine */
#define EM_NONE		0		//No machine
#define EM_M32		1		//AT&T WE 32100
#define EM_SPARC		2		//SPARC
#define	EM_386		3		//Intel 80386
#define	EM_68K		4		//Motorola 68000
#define EM_88K		5		//Motorola 88000
#define EM_860		7		//Intel 80860
#define EM_MIPS 		8  		//MIPS RS3000

/** section header tables */
#define	SHN_UNDEF		0			//Undefined, missing, irrelevant 
#define SHN_LORESERVE	0xff00		//The lower bound of the range of reserved indexes
#define SHN_LOPROC		0xff00		//Reserved for processor-specific semantics
#define SHN_HIPROC		0xff1f		//Reserved for processor-specific semantics
#define SHN_ABS			0xfff1		//This value specifies absolute values for the corresponding reference
#define SHN_COMMON		0xfff2		//Symbols defined relative to this section are common symbols
#define SHN_HIRESERVE	0xffff		//This value specifies the upper bound of the range of reserved indexes

/** sh_type */
#define SHT_NULL			0			//Marks the section header as inactive
#define SHT_PROGBITS		1			//Hold information defined by the program, whose format and meaning are determined solely
#define SHT_SYMTAB		2			//Hold a symbol table. Provides symblos for link editing
#define SHT_STRTAB		3			//
#define SHT_RELA			4			//
#define SHT_HASH			5			//
#define SHT_DYNAMIC		6			//
#define SHT_NOTE			7			//
#define SHT_NOBITS 		8			//
#define SHT_REL 			9			//
#define SHT_SHLIB		10			//
#define SHT_DYNSYM		11			//
#define SHT_LOPROC		0x70000000	//
#define SHT_HIPROC		0x7fffffff	//
#define SHT_LOUSER		0x80000000	//
#define SHT_HIUSER		0xffffffff	//

/** sh_flags */
#define SHF_WRITE				0x1
#define SHF_ALLOC				0x2
#define SHF_EXECINSTR			0x4
#define SHF_RELA_LIVEPATCH		0x00100000
#define SHF_RO_AFTER_INIT		0x00200000
#define SHF_MASKPROC				0xf0000000

/** st_info */
#define ELF32_ST_BIND(i)			((i)>>4)
#define ELF32_ST_TYPE(i)			((i)&0xf)
#define ELF32_ST_INFO(b, t)		(((b)<<4) + ((t)&0xf))

/** stb bind type */
#define STB_LOCAL		0
#define STB_GLOBAL		1
#define STB_WEAK			2
#define STB_LOPROC		13
#define STB_HIPROC		15

/** ELF32_ST_TYPE */
#define STT_NOTYPE		0
#define STT_OBJECT		1
#define STT_FUNC			2
#define STT_SECTION		3
#define STT_FILE			4
#define STT_LOPROC		13
#define STT_HIPROC		15

/** Segment types */
#define PT_NULL			0
#define PT_LOAD			1
#define PT_DYNAMIC		2
#define PT_INTERP		3
#define PT_NOTE			4
#define PT_SHLIB			5
#define PT_PHDR			6
#define PT_TLS    			7    
#define PT_NUM    			8    
#define PT_LOOS   			0x60000000 
#define PT_HIOS   			0x6fffffff 
#define PT_LOPROC 		0x70000000
#define PT_HIPROC		0x7fffffff
#define PT_GNU_EH_FRAME 	(PT_LOOS + 0x474e550) 
#define PT_GNU_STACK    	(PT_LOOS + 0x474e551) 
#define PT_GNU_RELRO    	(PT_LOOS + 0x474e552) 
#define PT_L4_STACK     	(PT_LOOS + 0x12) 
#define PT_L4_KIP       		(PT_LOOS + 0x13) 
#define PT_L4_AUX       		(PT_LOOS + 0x14) 

/** Dynamic Section */
#define DT_NULL			0
#define DT_NEEDED		1
#define DT_PLTRELSZ		2
#define DT_PLTGOT		3
#define DT_HASH			4
#define DT_STRTAB		5
#define DT_SYMTAB		6
#define DT_RELA			7
#define DT_RELASZ		8
#define DT_RELAENT		9
#define DT_STRSZ			10
#define DT_SYMENT		11
#define DT_INIT 			12
#define DT_FINI			13
#define DT_SONAME		14
#define DT_RPATH			15
#define DT_SYMBOLIC		16
#define DT_REL 			17
#define DT_RELSZ			18
#define DT_RELENT 		19
#define DT_PLTREL 		20
#define DT_DEBUG 			21
#define DT_TEXTREL 		22
#define DT_JMPREL		23
#define DT_LOPROC		0x70000000
#define DT_HIPROC 		0x7fffffff

/** Relocation Types */
#define R_386_GOT32		3
#define R_386_PLT32		4
#define R_386_COPY		5
#define R_386_GLOB_DAT	6
#define R_386_JMP_SLOT	7
#define R_386_RELATRIVE	8
#define R_386_GOTOFF		9
#define R_386_GOTPC		10

#define PERMISSION_X    0x01
#define PERMISSION_W    0x02
#define PERMISSION_R    0x04

/** 32-bit ELF Header */
typedef struct{
    u_char 			e_ident[EI_NIDENT];
    elf32_Half		e_type;				//Object file type
    elf32_Half		e_machine;			//Architecture
    elf32_Word 		e_version;			//Object file version
    elf32_Addr 		e_entry;			//Virtual address which the system first transfers control
    elf32_Off		e_phoff;			//Program header tables's file offset in bytes
    elf32_Off		e_shoff;			//Section header tables's file offset in bytes
    elf32_Word 		e_flags;
    elf32_Half 		e_ehsize;
    elf32_Half		e_phentsize;
    elf32_Half		e_phnum;
    elf32_Half		e_shentsize;
    elf32_Half		e_shnum;
    elf32_Half		e_shstrndx;
}elf32_Ehdr;

/** 32-bit ELF Section Header */
typedef struct{
    elf32_Word		sh_name;
    elf32_Word 		sh_type;
    elf32_Word   		sh_flags;
    elf32_Addr		sh_addr;
    elf32_Off		sh_offset;
    elf32_Word 		sh_size;
    elf32_Word  		sh_link;
    elf32_Word 		sh_info;
    elf32_Word 		sh_addralign;
    elf32_Word 		sh_entsize;
}elf32_Shdr;

/** 32-bit ELF Symbol Table */
typedef struct{
    elf32_Word 		st_name;
    elf32_Addr		st_value;
    elf32_Word 		st_size;
    u_char			st_info;
    u_char			st_other;
    elf32_Half		st_shndx;
}elf32_sym;

/** rellocate */
typedef struct{
    elf32_Addr 		r_offset;
    elf32_Word 		r_info;
}elf32_Rel;

typedef struct{
    elf32_Addr 		r_offset;
    elf32_Word 		r_info;
    elf32_Word 		r_addend;
}elf32_Rela;

/** Program Header */
typedef struct{
    elf32_Word 		p_type;
    elf32_Off		p_offset;
    elf32_Addr 		p_vaddr;
    elf32_Addr		p_paddr;
    elf32_Word 		p_filesz;
    elf32_Word 		p_memsz;
    elf32_Word 		p_flags;
    elf32_Word 		p_align;
}elf32_phdr;

/** Dynamic Structure */
typedef struct{
    elf32_Sword  	d_tag;
    union{
        elf32_Word 	d_val;
        elf32_Addr 	d_ptr;
    }d_un;
}elf32_Dyn;

extern elf32_Dyn _DYNAMIC[];

#define SECTION_NAME_LEN_MAX        256
#define ELF_INVALID_LEN             (-1)

typedef struct {
    elf32_Shdr header;
    char name[SECTION_NAME_LEN_MAX];
} section_info_t;

typedef struct {
    elf32_sym sheader;
    char section_name[SECTION_NAME_LEN_MAX];
    char symbol_name[SECTION_NAME_LEN_MAX];
} symbol_info_t;

typedef struct {
    unsigned char      *program_data;
    unsigned int       program_length;
    elf32_Ehdr         header;
    vector<elf32_phdr> program_headers;
    vector<section_info_t> section_infos;
    vector<symbol_info_t> symbol_infos;
} elf_info_t;




class ElfReader {
private:
    u_char *elf_file_data;
    int elf_file_length;

    bool read_program_headers();
    bool read_sections_info();
    bool read_symbols_info();

    bool string_equals(char *str1, char *str2) {
        if (str1 == NULL || str2 == NULL) {
            return false;
        }
        int len1 = strlen(str1);
        int len2 = strlen(str2);
        if (len1 != len2) {
            return false;
        }
        return !memcmp(str1, str2, len1);
    }

public: 
    ElfReader();
    ~ElfReader();

    elf_info_t elf_info;

    bool read_all();
    int read_elf_file(char *file_path);

    int read_constant(char *constant_name, char *data, unsigned int data_len_max);
    int read_fw_file_name(char *constant_name, char *data, int data_len_max);
};

int is_valid_name_var_len(int len);


