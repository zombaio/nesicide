#ifndef CSOURCEASSEMBLER_H
#define CSOURCEASSEMBLER_H

#include <QString>
#include "csources.h"
#include "csourceitem.h"
#include "cprgrombank.h"

typedef struct AssemblerInstrDetails
{
    uint opcode;
    uint cycles;
} AssemblerInstrDetails_s;

typedef struct AssemblerInstructionItem
{
    QString mnemonic;
    AssemblerInstrDetails_s impl;
    AssemblerInstrDetails_s rel;
    AssemblerInstrDetails_s imm;
    AssemblerInstrDetails_s zpage;
    AssemblerInstrDetails_s zpage_x;
    AssemblerInstrDetails_s abs;
    AssemblerInstrDetails_s abs_x;
    AssemblerInstrDetails_s abs_y;
    AssemblerInstrDetails_s ind_x;
    AssemblerInstrDetails_s ind_y;
    AssemblerInstrDetails_s zpage_y;
    AssemblerInstrDetails_s accum;
    AssemblerInstrDetails_s indirect;
} AssemblerInstructionItem_s;

#define INST_NOOP {0x00, 0}

const AssemblerInstructionItem AssemblerInstructionItems[] =
{
//   CODE    IMPL       REL        IMM        Z.PAGE     Z.PAGE,X   ABS        ABS,X      ABS,Y      (IND,X)    (IND),Y    Z.PAGE,Y   ACCUM.     INDIRECT
//--------------------------------------------------------------------------------------------------------------------------------------------------------
    {"LDA", INST_NOOP, INST_NOOP, {0xA9, 2}, {0xA5, 3}, {0xB5, 4}, {0xAD, 4}, {0xBD, 4}, {0xB9, 4}, {0xA1, 6}, {0xB1, 5}, INST_NOOP, INST_NOOP, INST_NOOP},
    {"STA", INST_NOOP, INST_NOOP, INST_NOOP, {0x85, 3}, {0x95, 4}, {0x8D, 4}, {0x9D, 5}, {0x99, 5}, {0x81, 6}, {0x91, 6}, INST_NOOP, INST_NOOP, INST_NOOP},
    {"LDX", INST_NOOP, INST_NOOP, {0xA2, 2}, {0xA6, 3}, INST_NOOP, {0xAE, 4}, INST_NOOP, {0xBE, 4}, INST_NOOP, INST_NOOP, {0xB6, 4}, INST_NOOP, INST_NOOP},
    {"STX", INST_NOOP, INST_NOOP, INST_NOOP, {0x86, 3}, INST_NOOP, {0x8E, 4}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, {0x96, 4}, INST_NOOP, INST_NOOP},
    {"LDY", INST_NOOP, INST_NOOP, {0xA0, 2}, {0xA4, 3}, {0xB4, 4}, {0xAC, 4}, {0xBC, 4}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"STY", INST_NOOP, INST_NOOP, INST_NOOP, {0x84, 3}, {0x94, 4}, {0x8C, 4}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"AND", INST_NOOP, INST_NOOP, {0x29, 2}, {0x25, 3}, {0x35, 4}, {0x2D, 4}, {0x3D, 4}, {0x39, 4}, {0x21, 6}, {0x31, 5}, INST_NOOP, INST_NOOP, INST_NOOP},
    {"ORA", INST_NOOP, INST_NOOP, {0x09, 2}, {0x05, 3}, {0x15, 4}, {0x0D, 4}, {0x1D, 4}, {0x19, 4}, {0x01, 6}, {0x11, 5}, INST_NOOP, INST_NOOP, INST_NOOP},
    {"EOR", INST_NOOP, INST_NOOP, {0x49, 2}, {0x45, 3}, {0x55, 4}, {0x4D, 4}, {0x5D, 4}, {0x59, 4}, {0x41, 6}, {0x51, 5}, INST_NOOP, INST_NOOP, INST_NOOP},
    {"BIT", INST_NOOP, INST_NOOP, INST_NOOP, {0x24, 3}, INST_NOOP, {0x2C, 4}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"CMP", INST_NOOP, INST_NOOP, {0xC9, 2}, {0xC5, 3}, {0xD5, 4}, {0xCD, 4}, {0xDD, 4}, {0xD9, 4}, {0xC1, 6}, {0xD1, 5}, INST_NOOP, INST_NOOP, INST_NOOP},
    {"CPX", INST_NOOP, INST_NOOP, {0xE0, 2}, {0xE4, 3}, INST_NOOP, {0xEC, 4}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"CPY", INST_NOOP, INST_NOOP, {0xC0, 2}, {0xC4, 3}, INST_NOOP, {0xCC, 4}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"ADC", INST_NOOP, INST_NOOP, {0x69, 2}, {0x65, 3}, {0x75, 4}, {0x6D, 4}, {0x7D, 4}, {0x79, 4}, {0x61, 6}, {0x71, 5}, INST_NOOP, INST_NOOP, INST_NOOP},
    {"SBC", INST_NOOP, INST_NOOP, {0xE9, 2}, {0xE5, 3}, {0xF5, 4}, {0xED, 4}, {0xFD, 4}, {0xF9, 4}, {0xE1, 6}, {0xF1, 5}, INST_NOOP, INST_NOOP, INST_NOOP},
    {"ASL", INST_NOOP, INST_NOOP, INST_NOOP, {0x06, 5}, {0x16, 6}, {0x0E, 6}, {0x1E, 7}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, {0x0A, 2}, INST_NOOP},
    {"LSR", INST_NOOP, INST_NOOP, INST_NOOP, {0x46, 5}, {0x56, 6}, {0x4E, 6}, {0x5E, 7}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, {0x4A, 2}, INST_NOOP},
    {"ROL", INST_NOOP, INST_NOOP, INST_NOOP, {0x26, 5}, {0x36, 6}, {0x2E, 6}, {0x3E, 7}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, {0x2A, 2}, INST_NOOP},
    {"ROR", INST_NOOP, INST_NOOP, INST_NOOP, {0x66, 5}, {0x76, 6}, {0x6E, 6}, {0x7E, 7}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, {0x6A, 2}, INST_NOOP},
    {"INC", INST_NOOP, INST_NOOP, INST_NOOP, {0xE6, 5}, {0xF6, 6}, {0xEE, 6}, {0xFE, 7}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"DEC", INST_NOOP, INST_NOOP, INST_NOOP, {0xC6, 5}, {0xD6, 6}, {0xCE, 6}, {0xDE, 7}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"JMP", INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, {0x4C, 3}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, {0xFF, 5}},
    {"JSR", INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, {0x20, 6}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"BRK", {0x00, 7}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"RTS", {0x60, 6}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"RTI", {0x40, 6}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"PHP", {0x08, 3}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"PLP", {0x28, 4}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"PHA", {0x48, 3}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"PLA", {0x68, 4}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"INX", {0xE8, 2}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"DEX", {0xCA, 2}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"INY", {0xC8, 2}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"DEY", {0x88, 2}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"TAX", {0xAA, 2}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"TXA", {0x8A, 2}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"TAY", {0xA8, 2}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"TYA", {0x98, 2}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"TSX", {0xBA, 2}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"TXS", {0x9A, 2}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"SED", {0xF8, 2}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"CLD", {0xD8, 2}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"SEI", {0x78, 2}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"CLI", {0x58, 2}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"SEC", {0x38, 2}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"CLC", {0x18, 2}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"CLV", {0xB8, 2}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"BEQ", INST_NOOP, {0xF0, 2}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"BNE", INST_NOOP, {0xD0, 2}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"BMI", INST_NOOP, {0x30, 2}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"BPL", INST_NOOP, {0x10, 2}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"BCC", INST_NOOP, {0x90, 2}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"BCS", INST_NOOP, {0xB0, 2}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"BVC", INST_NOOP, {0x50, 2}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"BVS", INST_NOOP, {0x70, 2}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"NOP", {0xEA, 2}, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
    {"",    INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP, INST_NOOP},
};

typedef struct LabelEntry
{
    QString labelName;
    int origin;
    int offset;
    int lineNumber;
    int byteSize;
    int bank;
} LabelEntry_s;



class CSourceAssembler
{
public:
    CSourceAssembler();
    bool assemble();

private:
    bool stripComments(QStringList *source);
    bool trimBlankLines(QStringList *source);
    bool convertOpcodesToDBs(QStringList *source);
    bool getLabels(QStringList *source);
    QList<LabelEntry_s> m_labelEntries;
    QList<LabelEntry_s> m_labelUseList;
    int getParamCount(QString sourceLine);
    QString getParamItem(QString sourceLine, int paramNum);
    uint numberToInt(bool *ok, QString number);
    bool isLabel(QString param);
    bool assembleSource(QStringList *source);
    bool resolveLabels();
};

#define BANK_WRITEBYTE(x) curBank->get_pointerToBankData()[bankPtr++] = x; \
if (bankPtr >= 0x4000) { \
    curBank = new CPRGROMBank(); \
    bankPtr = 0; \
    curBank->set_indexOfPrgRomBank(prgRomBanks->get_pointerToArrayOfBanks()->count()); \
    memset(curBank->get_pointerToBankData(), 0, sizeof(quint8) * 0x4000); \
    curBank->InitTreeItem(prgRomBanks); \
    prgRomBanks->appendChild(curBank); \
    prgRomBanks->get_pointerToArrayOfBanks()->append(curBank); \
}

#endif // CSOURCEASSEMBLER_H
