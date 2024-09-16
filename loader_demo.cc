#include <stdio.h>
#include <stdint.h>
#include <../inc/loader.h>

int main(int argc, char *argv[]) {
	size_t i; 	//argc = size
	Binary bin;	//argv[1] = binary object
	Section *sec;
	Symbol *sym;
	std::string fname;

	if(argc < 2){
		printf("Usage: %s <binary>\n", argv[0])
		return 1;
	};

	fname.assign(argv[1]);
	if(load_binary(fname, &bin, Binary::BIN_TYPE_AUTO) < 0) {
		return 1;
	};

	printf("loaded binary filename '%s' type/architecture %s/%s bit type (%u bits) entry address for the program entry@0x%016jx\n",
		 bin.filename.c_str(), //C string vwersion of the binary file name
		 bin.type_str.c_str(), //C string version of the binary type (ELF, PE)
		 bin.arch_str.c_str(), //C string version of the architecture type for the binary
		 bin.bits,	       //The bit type for the binary (32 bit, 64 bit)
		 bin.entry);	       //Entry point address for the program

	for(i = 0; i < bin.sections.size(); i++) {
		sec = &bin.sections[i]; //Pointer to the current indexed section in the sections vector
		printf(" 0x%016jx %-8ju %-20s %s\n",
		sec->vma,	       //Virtual memory address for the current section
		sec->size,	       //8 bit Size in bytes of section
		sec->name.c_str(),     //C string version of the section name
		sec->type == Section::SEC_TYPE_CODE ? "CODE" : "DATA"); //Section type, but also checks if it's of type code, else data
	};

	if(bin.symbols.size() > 0) {
		printf("scanned symbol tables\n");
		for(i = 0; i < bin.symbols.size(); i++) {
			sym = &bin.symbols[i];
			printf(" %-40s 0x%016jx %s\n",
				sym->name.c_str(), //C string version of the current symbol name
				sym->addr,	   //The address of the current symbol
				(sym->type & Symbol::SYM_TYPE_FUNC) ? "FUNC" : "");	//The symbol type, but also checks if it's of type func, else ""
		};
	};

	unload_binary(&bin);

	return 0;
};
