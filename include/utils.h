#include "types.h"

#define ACCUM_FORMAT "%s\tA"
#define ABS_FORMAT   "%s\t$%d"
#define ABSX_FORMAT  "%s\t$%d,X"
#define ABSY_FORMAT  "%s\t$%d,Y"
#define IMM_FORMAT   "%s\t#$%d"
#define IMPL_FORMAT  "%s"
#define IND_FORMAT   "%s\t($%d)"
#define INDX_FORMAT  "%s\t($%d,X)"
#define INDY_FORMAT  "%s\t($%d,Y)"
#define REL_FORMAT   "%s\t$%d"
#define ZP_FORMAT    "%s\t$%d"
#define ZPX_FORMAT   "%s\t$%d,X"
#define ZPY_FORMAT   "%s\t$%d,Y"

int sign_extend_number(unsigned, unsigned);
Instruction parse_instruction(uint8_t, uint8_t*);
void print_instruction(char* name, Byte arg, ADDR_MODE addr_mode);
void handle_invalid_instruction(Instruction);
void handle_invalid_read(Address);
void handle_invalid_write(Address);
