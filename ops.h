/* Sun Mar 27 09:58:49 JST 2016
 kk@ise.chuo-u.ac.jp
*/
enum opcode {
 	OP_NOP   =      0x00,
 	OP_INP   =      0x10,
 	OP_ADD   =      0x23,
 	OP_ADDL  =      0x22,
 	OP_ADDR  =      0x21,
 	OP_SUB   =      0x33,
 	OP_SUBL  =      0x32,
 	OP_SUBR  =      0x31,
 	OP_MUL   =      0x43,
 	OP_MULL  =      0x42,
 	OP_MULR  =      0x41,
 	OP_DIV   =      0x53,
 	OP_DIVL  =      0x52,
 	OP_DIVR  =      0x51,
 	OP_UPLUS =      0x61,
 	OP_UMINUS=      0x71,
 	OP_SQRT  =      0x81,
 	OP_EXP   =      0x91,
 	OP_LOG   =      0xa1,
 	OP_FABS  =      0xb1,
 	OP_MIN   =      0xc3,
 	OP_MINL  =      0xc2,
 	OP_MINR  =      0xc1,
 	OP_MAX   =      0xd3,
 	OP_MAXL  =      0xd2,
 	OP_MAXR  =      0xd1,

 	F_InActive =    0,
 	F_Active   =    1,
};
