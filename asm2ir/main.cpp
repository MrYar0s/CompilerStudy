#include <cstdint>
#include <fstream>
#include <iostream>
#include <stack>
#include <unordered_map>

#include "api.h"

#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

enum InsnId_t {
    DISPLAY,  // 0

    GENERATE,  // 1r
    B,         // imm

    SWAP,    // 2r
    SEXT_FROMBOOL,    // 2r
    ALLOCA,  // 1r imm
    INIT,    // imm imm

    LOAD,          // 3r
    STORE,         // 3r
    XOR,           // 3r
    MUL,           // 3r
    ADD,           // 3r
    GET_ELEM_PTR,  // 3r
    SET_PIXEL,     // 3r
    SELECT_FALSE,  // 3r
    OR,            // 3r

    MULi,           // 2r imm
    INC_EQ,         // 2r imm
    ADDi,           // 2r imm
    GET_ELEM_PTRi,  // 2r imm
    MODi,           // 2r imm
    ICMP_EQ,        // 2r imm

    BR_COND,  // 1r label1 label2
};

///////////////////////
// Model for simulation
///////////////////////

using RegId_t = uint8_t;
using RegVal_t = long int;

const int REG_FILE_SIZE = 40;
class CPU {
public:
    RegVal_t REG_FILE[REG_FILE_SIZE] = {};
    RegVal_t PC;
    RegVal_t NEXT_PC;
    uint32_t RUN;
    void dump()
    {
        // for (int i = 0; i < REG_FILE_SIZE; i++) {
        //     outs() << "[" << i << "] " << REG_FILE[i] << "\n";
        // }
        // outs() << "[" << 37 << "] " << REG_FILE[37] << "\n";
    }
};

/////////////////////
// Instructions class
/////////////////////

class Instr {
public:
    InsnId_t m_ID;
    void (*m_INSTR)(CPU *, Instr *);
    RegId_t m_rs1;
    RegId_t m_rs2;
    RegId_t m_rs3;
    RegVal_t m_imm;
    RegVal_t m_label1;
    RegVal_t m_label2;
    std::string m_name;
    Instr(InsnId_t ID, void (*do_INSTR)(CPU *, Instr *), std::string name) : m_ID(ID), m_INSTR(do_INSTR), m_name(name)
    {
    }
    Instr(InsnId_t ID, void (*do_INSTR)(CPU *, Instr *), std::string name, RegId_t rs1)
        : m_ID(ID), m_INSTR(do_INSTR), m_name(name), m_rs1(rs1)
    {
    }
    Instr(InsnId_t ID, void (*do_INSTR)(CPU *, Instr *), std::string name, RegVal_t imm)
        : m_ID(ID), m_INSTR(do_INSTR), m_name(name), m_imm(imm)
    {
    }
    Instr(InsnId_t ID, void (*do_INSTR)(CPU *, Instr *), std::string name, RegVal_t imm1, RegVal_t imm2)
        : m_ID(ID), m_INSTR(do_INSTR), m_name(name), m_label1(imm1), m_label2(imm2)
    {
    }
    Instr(InsnId_t ID, void (*do_INSTR)(CPU *, Instr *), std::string name, RegId_t rs1, RegId_t rs2, RegId_t rs3)
        : m_ID(ID), m_INSTR(do_INSTR), m_name(name), m_rs1(rs1), m_rs2(rs2), m_rs3(rs3)
    {
    }
    Instr(InsnId_t ID, void (*do_INSTR)(CPU *, Instr *), std::string name, RegId_t rs1, RegId_t rs2)
        : m_ID(ID), m_INSTR(do_INSTR), m_name(name), m_rs1(rs1), m_rs2(rs2)
    {
    }
    Instr(InsnId_t ID, void (*do_INSTR)(CPU *, Instr *), std::string name, RegId_t rs1, RegVal_t imm)
        : m_ID(ID), m_INSTR(do_INSTR), m_name(name), m_rs1(rs1), m_imm(imm)
    {
    }
    Instr(InsnId_t ID, void (*do_INSTR)(CPU *, Instr *), std::string name, RegId_t rs1, RegId_t rs2, RegVal_t imm)
        : m_ID(ID), m_INSTR(do_INSTR), m_name(name), m_rs1(rs1), m_rs2(rs2), m_imm(imm)
    {
    }
    Instr(InsnId_t ID, void (*do_INSTR)(CPU *, Instr *), std::string name, RegId_t rs1, RegVal_t imm1, RegVal_t imm2)
        : m_ID(ID), m_INSTR(do_INSTR), m_name(name), m_rs1(rs1), m_label1(imm1), m_label2(imm2)
    {
    }
    void dump()
    {
        // outs() << "m_name " << m_name << "\n";
        // outs() << "m_rs1 " << (RegVal_t)m_rs1 << "\n";
        // outs() << "m_rs2 " << (RegVal_t)m_rs2 << "\n";
        // outs() << "m_rs3 " << (RegVal_t)m_rs3 << "\n";
        // outs() << "m_imm " << (RegVal_t)m_imm << "\n";
        // outs() << "m_label1 " << (RegVal_t)m_label1 << "\n";
        // outs() << "m_label2 " << (RegVal_t)m_label2 << "\n";
    }
};

//////////////////////////////
// Interpreter function
//////////////////////////////

void do_display(CPU *cpu, Instr *instr)
{
    instr->dump();
    cpu->dump();
    display();
}
void do_generate(CPU *cpu, Instr *instr)
{
    instr->dump();
    cpu->dump();
    cpu->REG_FILE[instr->m_rs1] = generate();
}
void do_b(CPU *cpu, Instr *instr)
{
    instr->dump();
    cpu->dump();
    cpu->NEXT_PC = instr->m_imm;
}
void do_load(CPU *cpu, Instr *instr)
{
    instr->dump();
    cpu->dump();
    RegVal_t *ptr = (RegVal_t *)static_cast<intptr_t>(cpu->REG_FILE[instr->m_rs2] + cpu->REG_FILE[instr->m_rs3]*sizeof(RegVal_t));
    cpu->REG_FILE[instr->m_rs1] = *ptr;
}
void do_store(CPU *cpu, Instr *instr)
{
    instr->dump();
    cpu->dump();
    RegVal_t *ptr = (RegVal_t *)static_cast<intptr_t>(cpu->REG_FILE[instr->m_rs1] + cpu->REG_FILE[instr->m_rs2]*sizeof(RegVal_t));
    *ptr = cpu->REG_FILE[instr->m_rs3];
}
void do_swap(CPU *cpu, Instr *instr)
{
    instr->dump();
    cpu->dump();
    RegVal_t tmp = cpu->REG_FILE[instr->m_rs1];
    cpu->REG_FILE[instr->m_rs1] = cpu->REG_FILE[instr->m_rs2];
    cpu->REG_FILE[instr->m_rs2] = tmp;
}
void do_alloca(CPU *cpu, Instr *instr)
{
    instr->dump();
    cpu->dump();
    RegVal_t *ptr = new RegVal_t[instr->m_imm];
    cpu->REG_FILE[instr->m_rs1] = reinterpret_cast<intptr_t>(ptr);
}
void do_init(CPU *cpu, Instr *instr)
{
    instr->dump();
    cpu->dump();
    init(instr->m_label1, instr->m_label2);
}
void do_sextFromBool(CPU *cpu, Instr *instr)
{
    instr->dump();
    cpu->dump();
    if (cpu->REG_FILE[instr->m_rs2] == true) {
        cpu->REG_FILE[instr->m_rs1] = -1;
    } else {
        cpu->REG_FILE[instr->m_rs1] = 0;
    }
}
// 3r
void do_xor(CPU *cpu, Instr *instr)
{
    instr->dump();
    cpu->dump();
    cpu->REG_FILE[instr->m_rs1] = cpu->REG_FILE[instr->m_rs2] ^ cpu->REG_FILE[instr->m_rs3];
}
void do_mul(CPU *cpu, Instr *instr)
{
    instr->dump();
    cpu->dump();
    cpu->REG_FILE[instr->m_rs1] = cpu->REG_FILE[instr->m_rs2] * cpu->REG_FILE[instr->m_rs3];
}
void do_add(CPU *cpu, Instr *instr)
{
    instr->dump();
    cpu->dump();
    cpu->REG_FILE[instr->m_rs1] = cpu->REG_FILE[instr->m_rs2] + cpu->REG_FILE[instr->m_rs3];
    // outs() << "do_add" << "\n";
    // outs() << "cpu->REG_FILE[instr->m_rs1]: " << cpu->REG_FILE[instr->m_rs1] << " instr->m_rs1 " << (RegVal_t)instr->m_rs1 << "\n";
    // outs() << "cpu->REG_FILE[instr->m_rs2]: " << cpu->REG_FILE[instr->m_rs2] << " instr->m_rs2 " << (RegVal_t)instr->m_rs2 << "\n";
    // outs() << "cpu->REG_FILE[instr->m_rs3]: " << cpu->REG_FILE[instr->m_rs3] << " instr->m_rs3 " << (RegVal_t)instr->m_rs3 << "\n";
}
void do_gep(CPU *cpu, Instr *instr)
{
    instr->dump();
    cpu->dump();
    cpu->REG_FILE[instr->m_rs1] = cpu->REG_FILE[instr->m_rs2] + cpu->REG_FILE[instr->m_rs3]*sizeof(RegVal_t);
}
void do_setPixel(CPU *cpu, Instr *instr)
{
    instr->dump();
    cpu->dump();
    setPixel(cpu->REG_FILE[instr->m_rs1], cpu->REG_FILE[instr->m_rs2], cpu->REG_FILE[instr->m_rs3]);
}
void do_selectFalse(CPU *cpu, Instr *instr)
{
    instr->dump();
    cpu->dump();
    if (cpu->REG_FILE[instr->m_rs2] == 1) {
        cpu->REG_FILE[instr->m_rs1] = cpu->REG_FILE[instr->m_rs3];
    } else {
        cpu->REG_FILE[instr->m_rs1] = false;
    }
}
void do_or(CPU *cpu, Instr *instr)
{
    instr->dump();
    cpu->dump();
    cpu->REG_FILE[instr->m_rs1] = cpu->REG_FILE[instr->m_rs2] | cpu->REG_FILE[instr->m_rs3];
}

void do_addi(CPU *cpu, Instr *instr)
{
    instr->dump();
    cpu->dump();
    cpu->REG_FILE[instr->m_rs1] = cpu->REG_FILE[instr->m_rs2] + instr->m_imm;
    // outs() << "do_addi" << "\n";
    // outs() << "cpu->REG_FILE[instr->m_rs1]: " << cpu->REG_FILE[instr->m_rs1] << " instr->m_rs1 " << (RegVal_t)instr->m_rs1 << "\n";
    // outs() << "cpu->REG_FILE[instr->m_rs2]: " << cpu->REG_FILE[instr->m_rs2] << " instr->m_rs2 " << (RegVal_t)instr->m_rs2 << "\n";
    // outs() << "instr->m_imm: " << instr->m_imm << "\n";
}
void do_muli(CPU *cpu, Instr *instr)
{
    instr->dump();
    cpu->dump();
    cpu->REG_FILE[instr->m_rs1] = cpu->REG_FILE[instr->m_rs2] * instr->m_imm;
}
void do_incEq(CPU *cpu, Instr *instr)
{
    instr->dump();
    cpu->dump();
    cpu->REG_FILE[instr->m_rs2] += 1;
    cpu->REG_FILE[instr->m_rs1] = cpu->REG_FILE[instr->m_rs2] == instr->m_imm;
}
void do_gepi(CPU *cpu, Instr *instr)
{
    instr->dump();
    cpu->dump();
    cpu->REG_FILE[instr->m_rs1] = cpu->REG_FILE[instr->m_rs2] + instr->m_imm*sizeof(RegVal_t);
}
void do_modi(CPU *cpu, Instr *instr)
{
    instr->dump();
    cpu->dump();
    cpu->REG_FILE[instr->m_rs1] = std::abs(cpu->REG_FILE[instr->m_rs2] % instr->m_imm);
}
void do_icmpEq(CPU *cpu, Instr *instr)
{
    instr->dump();
    cpu->dump();
    cpu->REG_FILE[instr->m_rs1] = cpu->REG_FILE[instr->m_rs2] == instr->m_imm;
}
void do_brCond(CPU *cpu, Instr *instr)
{
    instr->dump();
    cpu->dump();
    if (cpu->REG_FILE[instr->m_rs1] == 1) {
        cpu->NEXT_PC = instr->m_label1;
    } else {
        cpu->NEXT_PC = instr->m_label2;
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        outs() << "[ERROR] Need 1 argument: file with assembler\n";
        return 1;
    }
    std::ifstream input;
    input.open(argv[1]);
    if (!input.is_open()) {
        outs() << "[ERROR] Can't open " << argv[1] << "\n";
        return 1;
    }

    std::string name;
    std::string arg;
    std::unordered_map<std::string, RegVal_t> BB_PC;

    outs() << "\n#[FILE]:\nBBs:\n";
    RegVal_t pc = 0;
    while (input >> name) {
        // 3 args
        if (!name.compare("XOR") || !name.compare("MUL") || !name.compare("MULi") || !name.compare("INC_EQ") ||
            !name.compare("BR_COND") || !name.compare("ADD") || !name.compare("ADDi") ||
            !name.compare("GET_ELEM_PTR") || !name.compare("GET_ELEM_PTRi") || !name.compare("SET_PIXEL") ||
            !name.compare("MODi") || !name.compare("ICMP_EQ") || !name.compare("SELECT_FALSE") || !name.compare("OR") ||
            !name.compare("LOAD") || !name.compare("STORE")) {
            input >> arg >> arg >> arg;
            pc++;
            continue;
        }
        // 2 args
        if (!name.compare("ALLOCA") || !name.compare("SWAP") ||
            !name.compare("INIT") || !name.compare("SEXT_FROMBOOL")) {
            input >> arg >> arg;
            pc++;
            continue;
        }
        // 1 arg
        if (!name.compare("B") || !name.compare("GENERATE")) {
            input >> arg;
            pc++;
            continue;
        }
        // 0 args
        if (!name.compare("DISPLAY")) {
            pc++;
            continue;
        }
        outs() << " " << name << "(" << pc << ")"
               << "\n";
        BB_PC[name] = pc;
    }
    input.close();
    input.open(argv[1]);

    std::string arg1;
    std::string arg2;
    std::string arg3;
    std::vector<Instr *> Instructions;

    outs() << "#[FILE] BEGIN\n";
    while (input >> name) {
        outs() << name;
        // 0 registers
        if (!name.compare("DISPLAY")) {
            Instructions.push_back(new Instr(InsnId_t::DISPLAY, do_display, name));
            outs() << "\n";
            continue;
        }

        // 1 register
        if (!name.compare("GENERATE")) {
            input >> arg1;
            outs() << " " << arg1 << "\n";
            RegId_t rs1 = stoi(arg1.erase(0, 1));
            if (!name.compare("GENERATE")) {
                Instructions.push_back(new Instr(InsnId_t::GENERATE, do_generate, name, rs1));
            }
            continue;
        }

        // 1 label
        if (!name.compare("B")) {
            input >> arg1;
            outs() << " " << arg1 << "\n";
            RegVal_t imm = BB_PC[arg1];
            outs() << " " << imm << "\n";
            if (!name.compare("B")) {
                Instructions.push_back(new Instr(InsnId_t::B, do_b, name, imm));
            }
            continue;
        }

        // 2 registers
        if (!name.compare("SWAP") || !name.compare("SEXT_FROMBOOL")) {
            input >> arg1 >> arg2;
            outs() << " " << arg1 << " " << arg2 << " " << arg3 << "\n";
            RegId_t rs1 = stoi(arg1.erase(0, 1));
            RegId_t rs2 = stoi(arg2.erase(0, 1));
            if (!name.compare("SWAP")) {
                Instructions.push_back(new Instr(InsnId_t::SWAP, do_swap, name, rs1, rs2));
            }
            if (!name.compare("SEXT_FROMBOOL")) {
                Instructions.push_back(new Instr(InsnId_t::SEXT_FROMBOOL, do_sextFromBool, name, rs1, rs2));
            }
            continue;
        }

        // 1 register 1 imm
        if (!name.compare("ALLOCA")) {
            input >> arg1 >> arg2;
            outs() << " " << arg1 << " " << arg2 << "\n";
            RegId_t rs1 = stoi(arg1.erase(0, 1));
            RegVal_t imm = stoi(arg2);
            if (!name.compare("ALLOCA")) {
                Instructions.push_back(new Instr(InsnId_t::ALLOCA, do_alloca, name, rs1, imm));
            }
            continue;
        }

        // 2 imms
        if (!name.compare("INIT")) {
            input >> arg1 >> arg2;
            outs() << " " << arg1 << " " << arg2 << "\n";
            RegVal_t imm1 = stoi(arg2);
            RegVal_t imm2 = stoi(arg2);
            if (!name.compare("INIT")) {
                Instructions.push_back(new Instr(InsnId_t::INIT, do_init, name, imm1, imm2));
            }
            continue;
        }

        // 3 registers
        if (!name.compare("LOAD") || !name.compare("STORE") || !name.compare("XOR") || !name.compare("MUL") || !name.compare("ADD") || !name.compare("GET_ELEM_PTR") ||
            !name.compare("SET_PIXEL") || !name.compare("SELECT_FALSE") || !name.compare("OR")) {
            input >> arg1 >> arg2 >> arg3;
            outs() << " " << arg1 << " " << arg2 << " " << arg3 << "\n";
            RegId_t rs1 = stoi(arg1.erase(0, 1));
            RegId_t rs2 = stoi(arg2.erase(0, 1));
            RegId_t rs3 = stoi(arg3.erase(0, 1));
            if (!name.compare("LOAD")) {
                Instructions.push_back(new Instr(InsnId_t::LOAD, do_load, name, rs1, rs2, rs3));
            }
            if (!name.compare("STORE")) {
                Instructions.push_back(new Instr(InsnId_t::STORE, do_store, name, rs1, rs2, rs3));
            }
            if (!name.compare("XOR")) {
                Instructions.push_back(new Instr(InsnId_t::XOR, do_xor, name, rs1, rs2, rs3));
            }
            if (!name.compare("MUL")) {
                Instructions.push_back(new Instr(InsnId_t::MUL, do_mul, name, rs1, rs2, rs3));
            }
            if (!name.compare("ADD")) {
                Instructions.push_back(new Instr(InsnId_t::ADD, do_add, name, rs1, rs2, rs3));
            }
            if (!name.compare("GET_ELEM_PTR")) {
                Instructions.push_back(new Instr(InsnId_t::GET_ELEM_PTR, do_gep, name, rs1, rs2, rs3));
            }
            if (!name.compare("SET_PIXEL")) {
                Instructions.push_back(new Instr(InsnId_t::SET_PIXEL, do_setPixel, name, rs1, rs2, rs3));
            }
            if (!name.compare("SELECT_FALSE")) {
                Instructions.push_back(new Instr(InsnId_t::SELECT_FALSE, do_selectFalse, name, rs1, rs2, rs3));
            }
            if (!name.compare("OR")) {
                Instructions.push_back(new Instr(InsnId_t::XOR, do_or, name, rs1, rs2, rs3));
            }
            continue;
        }

        // 2 registers and imm
        if (!name.compare("MULi") || !name.compare("INC_EQ") || !name.compare("ADDi") ||
            !name.compare("GET_ELEM_PTRi") || !name.compare("MODi") || !name.compare("ICMP_EQ")) {
            input >> arg1 >> arg2 >> arg3;
            outs() << " " << arg1 << " " << arg2 << " " << arg3 << "\n";
            RegId_t rs1 = stoi(arg1.erase(0, 1));
            RegId_t rs2 = stoi(arg2.erase(0, 1));
            RegVal_t imm = stoi(arg3);
            if (!name.compare("MULi")) {
                Instructions.push_back(new Instr(InsnId_t::MULi, do_muli, name, rs1, rs2, imm));
            }
            if (!name.compare("INC_EQ")) {
                Instructions.push_back(new Instr(InsnId_t::INC_EQ, do_incEq, name, rs1, rs2, imm));
            }
            if (!name.compare("ADDi")) {
                Instructions.push_back(new Instr(InsnId_t::ADDi, do_addi, name, rs1, rs2, imm));
            }
            if (!name.compare("GET_ELEM_PTRi")) {
                Instructions.push_back(new Instr(InsnId_t::GET_ELEM_PTRi, do_gepi, name, rs1, rs2, imm));
            }
            if (!name.compare("MODi")) {
                Instructions.push_back(new Instr(InsnId_t::MODi, do_modi, name, rs1, rs2, imm));
            }
            if (!name.compare("ICMP_EQ")) {
                Instructions.push_back(new Instr(InsnId_t::ICMP_EQ, do_icmpEq, name, rs1, rs2, imm));
            }
            continue;
        }

        // register and 2 labels
        if (!name.compare("BR_COND")) {
            input >> arg1 >> arg2 >> arg3;
            outs() << " " << arg1 << " " << arg2 << " " << arg3 << "\n";
            RegId_t rs1 = stoi(arg1.erase(0, 1));
            RegVal_t imm1 = BB_PC[arg2];
            RegVal_t imm2 = BB_PC[arg3];
            if (!name.compare("BR_COND")) {
                Instructions.push_back(new Instr(InsnId_t::BR_COND, do_brCond, name, rs1, imm1, imm2));
            }
            continue;
        }

        if (BB_PC.find(name) == BB_PC.end()) {
            outs() << "\n[ERROR] Unknown instruction: " << name << "\n";
            Instructions.clear();
            return 1;
        }
        outs() << "\n";
    }
    outs() << "#[FILE] END\n";

    outs() << "\n#[EXEC] BEGIN\n";
    CPU cpu;
    for (int i = 0; i < REG_FILE_SIZE; i++) {
        cpu.REG_FILE[i] = 0;
    }
    cpu.RUN = 1;
    cpu.PC = 1;
    // Loop execution
    while (cpu.RUN) {
        cpu.NEXT_PC = cpu.PC + 1;
        Instructions[cpu.PC]->m_INSTR(&cpu, Instructions[cpu.PC]);
        cpu.PC = cpu.NEXT_PC;
    }
    outs() << "#[EXEC] END\n";

    // Dump registers after simulation
    for (int i = 0; i < REG_FILE_SIZE; i++) {
        outs() << "[" << i << "] " << cpu.REG_FILE[i] << "\n";
    }

    return 0;
}