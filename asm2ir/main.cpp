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
    DISPLAY,  // 0                  lazy

    GENERATE,  // 1r                lazy
    B,         // imm               impl

    SWAP,           // 2r           impl
    SEXT_FROMBOOL,  // 2r           impl
    ALLOCA,         // 1r imm
    INIT,           // imm imm      lazy

    LOAD,          // 3r
    STORE,         // 3r
    XOR,           // 3r            impl
    MUL,           // 3r            impl
    ADD,           // 3r            impl
    SET_PIXEL,     // 3r            lazy
    SELECT_FALSE,  // 3r            impl
    OR,            // 3r            impl

    MULi,     // 2r imm             impl
    INC_EQ,   // 2r imm             impl
    ADDi,     // 2r imm             impl
    SMODi,    // 2r imm             impl
    UMODi,    // 2r imm             impl
    ICMP_EQ,  // 2r imm             impl

    BR_COND  // 1r label1 label2    impl
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
    RegVal_t *ptr =
        (RegVal_t *)static_cast<intptr_t>(cpu->REG_FILE[instr->m_rs2] + cpu->REG_FILE[instr->m_rs3] * sizeof(RegVal_t));
    cpu->REG_FILE[instr->m_rs1] = *ptr;
}
void do_store(CPU *cpu, Instr *instr)
{
    instr->dump();
    cpu->dump();
    RegVal_t *ptr =
        (RegVal_t *)static_cast<intptr_t>(cpu->REG_FILE[instr->m_rs1] + cpu->REG_FILE[instr->m_rs2] * sizeof(RegVal_t));
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

void *lazyFunctionCreator(const std::string &fnName)
{
    if (fnName == "do_display") {
        return reinterpret_cast<void *>(do_display);
    }
    if (fnName == "do_generate") {
        return reinterpret_cast<void *>(do_generate);
    }
    if (fnName == "do_b") {
        return reinterpret_cast<void *>(do_b);
    }
    if (fnName == "do_load") {
        return reinterpret_cast<void *>(do_load);
    }
    if (fnName == "do_store") {
        return reinterpret_cast<void *>(do_store);
    }
    if (fnName == "do_swap") {
        return reinterpret_cast<void *>(do_swap);
    }
    if (fnName == "do_alloca") {
        return reinterpret_cast<void *>(do_alloca);
    }
    if (fnName == "do_init") {
        return reinterpret_cast<void *>(do_init);
    }
    if (fnName == "do_sextFromBool") {
        return reinterpret_cast<void *>(do_sextFromBool);
    }
    if (fnName == "do_xor") {
        return reinterpret_cast<void *>(do_xor);
    }
    if (fnName == "do_mul") {
        return reinterpret_cast<void *>(do_mul);
    }
    if (fnName == "do_add") {
        return reinterpret_cast<void *>(do_add);
    }
    if (fnName == "do_setPixel") {
        return reinterpret_cast<void *>(do_setPixel);
    }
    if (fnName == "do_selectFalse") {
        return reinterpret_cast<void *>(do_selectFalse);
    }
    if (fnName == "do_or") {
        return reinterpret_cast<void *>(do_or);
    }
    if (fnName == "do_addi") {
        return reinterpret_cast<void *>(do_addi);
    }
    if (fnName == "do_muli") {
        return reinterpret_cast<void *>(do_muli);
    }
    if (fnName == "do_incEq") {
        return reinterpret_cast<void *>(do_incEq);
    }
    if (fnName == "do_smodi") {
        return reinterpret_cast<void *>(do_modi);
    }
    if (fnName == "do_umodi") {
        return reinterpret_cast<void *>(do_modi);
    }
    if (fnName == "do_icmpEq") {
        return reinterpret_cast<void *>(do_icmpEq);
    }
    if (fnName == "do_brCond") {
        return reinterpret_cast<void *>(do_brCond);
    }
    return nullptr;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        outs() << "[ERROR] Need 1 argument: file with assembler\n";
        return 1;
    }
    bool need_to_run_asm = false;
    bool need_to_run_ir = false;
    if (argc > 2) {
        need_to_run_asm = atoi(argv[2]);
        need_to_run_ir = !need_to_run_asm;
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
        if (!name.compare("xor") || !name.compare("mul") || !name.compare("muli") || !name.compare("incEq") ||
            !name.compare("brCond") || !name.compare("add") || !name.compare("addi") || !name.compare("setPixel") ||
            !name.compare("smodi") || !name.compare("icmpEq") || !name.compare("selectFalse") || !name.compare("or") ||
            !name.compare("load") || !name.compare("store") || !name.compare("umodi")) {
            input >> arg >> arg >> arg;
            pc++;
            continue;
        }
        // 2 args
        if (!name.compare("alloca") || !name.compare("swap") || !name.compare("init") ||
            !name.compare("sextFromBool")) {
            input >> arg >> arg;
            pc++;
            continue;
        }
        // 1 arg
        if (!name.compare("b") || !name.compare("generate")) {
            input >> arg;
            pc++;
            continue;
        }
        // 0 args
        if (!name.compare("display")) {
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
        if (!name.compare("display")) {
            Instructions.push_back(new Instr(InsnId_t::DISPLAY, do_display, name));
            outs() << "\n";
            continue;
        }

        // 1 register
        if (!name.compare("generate")) {
            input >> arg1;
            outs() << " " << arg1 << "\n";
            RegId_t rs1 = stoi(arg1.erase(0, 1));
            if (!name.compare("generate")) {
                Instructions.push_back(new Instr(InsnId_t::GENERATE, do_generate, name, rs1));
            }
            continue;
        }

        // 1 label
        if (!name.compare("b")) {
            input >> arg1;
            outs() << " " << arg1 << "\n";
            RegVal_t imm = BB_PC[arg1];
            if (!name.compare("b")) {
                Instructions.push_back(new Instr(InsnId_t::B, do_b, name, imm));
            }
            continue;
        }

        // 2 registers
        if (!name.compare("swap") || !name.compare("sextFromBool")) {
            input >> arg1 >> arg2;
            outs() << " " << arg1 << " " << arg2 << " " << arg3 << "\n";
            RegId_t rs1 = stoi(arg1.erase(0, 1));
            RegId_t rs2 = stoi(arg2.erase(0, 1));
            if (!name.compare("swap")) {
                Instructions.push_back(new Instr(InsnId_t::SWAP, do_swap, name, rs1, rs2));
            }
            if (!name.compare("sextFromBool")) {
                Instructions.push_back(new Instr(InsnId_t::SEXT_FROMBOOL, do_sextFromBool, name, rs1, rs2));
            }
            continue;
        }

        // 1 register 1 imm
        if (!name.compare("alloca")) {
            input >> arg1 >> arg2;
            outs() << " " << arg1 << " " << arg2 << "\n";
            RegId_t rs1 = stoi(arg1.erase(0, 1));
            RegVal_t imm = stoi(arg2);
            if (!name.compare("alloca")) {
                Instructions.push_back(new Instr(InsnId_t::ALLOCA, do_alloca, name, rs1, imm));
            }
            continue;
        }

        // 2 imms
        if (!name.compare("init")) {
            input >> arg1 >> arg2;
            outs() << " " << arg1 << " " << arg2 << "\n";
            RegVal_t imm1 = stoi(arg2);
            RegVal_t imm2 = stoi(arg2);
            if (!name.compare("init")) {
                Instructions.push_back(new Instr(InsnId_t::INIT, do_init, name, imm1, imm2));
            }
            continue;
        }

        // 3 registers
        if (!name.compare("load") || !name.compare("store") || !name.compare("xor") || !name.compare("mul") ||
            !name.compare("add") || !name.compare("setPixel") || !name.compare("selectFalse") || !name.compare("or")) {
            input >> arg1 >> arg2 >> arg3;
            outs() << " " << arg1 << " " << arg2 << " " << arg3 << "\n";
            RegId_t rs1 = stoi(arg1.erase(0, 1));
            RegId_t rs2 = stoi(arg2.erase(0, 1));
            RegId_t rs3 = stoi(arg3.erase(0, 1));
            if (!name.compare("load")) {
                Instructions.push_back(new Instr(InsnId_t::LOAD, do_load, name, rs1, rs2, rs3));
            }
            if (!name.compare("store")) {
                Instructions.push_back(new Instr(InsnId_t::STORE, do_store, name, rs1, rs2, rs3));
            }
            if (!name.compare("xor")) {
                Instructions.push_back(new Instr(InsnId_t::XOR, do_xor, name, rs1, rs2, rs3));
            }
            if (!name.compare("mul")) {
                Instructions.push_back(new Instr(InsnId_t::MUL, do_mul, name, rs1, rs2, rs3));
            }
            if (!name.compare("add")) {
                Instructions.push_back(new Instr(InsnId_t::ADD, do_add, name, rs1, rs2, rs3));
            }
            if (!name.compare("setPixel")) {
                Instructions.push_back(new Instr(InsnId_t::SET_PIXEL, do_setPixel, name, rs1, rs2, rs3));
            }
            if (!name.compare("selectFalse")) {
                Instructions.push_back(new Instr(InsnId_t::SELECT_FALSE, do_selectFalse, name, rs1, rs2, rs3));
            }
            if (!name.compare("or")) {
                Instructions.push_back(new Instr(InsnId_t::OR, do_or, name, rs1, rs2, rs3));
            }
            continue;
        }

        // 2 registers and imm
        if (!name.compare("muli") || !name.compare("incEq") || !name.compare("addi") || !name.compare("umodi") ||
            !name.compare("icmpEq") || !name.compare("smodi")) {
            input >> arg1 >> arg2 >> arg3;
            outs() << " " << arg1 << " " << arg2 << " " << arg3 << "\n";
            RegId_t rs1 = stoi(arg1.erase(0, 1));
            RegId_t rs2 = stoi(arg2.erase(0, 1));
            RegVal_t imm = stoi(arg3);
            if (!name.compare("muli")) {
                Instructions.push_back(new Instr(InsnId_t::MULi, do_muli, name, rs1, rs2, imm));
            }
            if (!name.compare("incEq")) {
                Instructions.push_back(new Instr(InsnId_t::INC_EQ, do_incEq, name, rs1, rs2, imm));
            }
            if (!name.compare("addi")) {
                Instructions.push_back(new Instr(InsnId_t::ADDi, do_addi, name, rs1, rs2, imm));
            }
            if (!name.compare("smodi")) {
                Instructions.push_back(new Instr(InsnId_t::SMODi, do_modi, name, rs1, rs2, imm));
            }
            if (!name.compare("umodi")) {
                Instructions.push_back(new Instr(InsnId_t::UMODi, do_modi, name, rs1, rs2, imm));
            }
            if (!name.compare("icmpEq")) {
                Instructions.push_back(new Instr(InsnId_t::ICMP_EQ, do_icmpEq, name, rs1, rs2, imm));
            }
            continue;
        }

        // register and 2 labels
        if (!name.compare("brCond")) {
            input >> arg1 >> arg2 >> arg3;
            outs() << " " << arg1 << " " << arg2 << " " << arg3 << "\n";
            RegId_t rs1 = stoi(arg1.erase(0, 1));
            RegVal_t imm1 = BB_PC[arg2];
            RegVal_t imm2 = BB_PC[arg3];
            if (!name.compare("brCond")) {
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

    if (need_to_run_asm) {
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
    }
    CPU cpu;
    // Build IR for application
    outs() << "#[LLVM IR] BEGIN\n";
    LLVMContext context;
    // ; ModuleID = 'main'
    // source_filename = "main"
    Module *module = new Module("main", context);
    IRBuilder<> builder(context);

    // declare void @main()
    FunctionType *funcType = FunctionType::get(builder.getVoidTy(), false);
    Function *mainFunc = Function::Create(funcType, Function::ExternalLinkage, "main", module);
    // main_entry:
    BasicBlock *main_entryBB = BasicBlock::Create(context, "main_entry", mainFunc);

    builder.SetInsertPoint(main_entryBB);

    // createCalleeFunctions(builder, module);
    FunctionType *CalleType = FunctionType::get(
        builder.getVoidTy(), std::vector<Type *>({builder.getInt8PtrTy(), builder.getInt8PtrTy()}), false);

    // Get poointer to CPU for function args
    Value *cpu_p = builder.getInt64((uint64_t)&cpu);
    ArrayType *regFileType = ArrayType::get(builder.getInt64Ty(), REG_FILE_SIZE);
    module->getOrInsertGlobal("regFile", regFileType);
    GlobalVariable *regFile = module->getNamedGlobal("regFile");

    std::unordered_map<RegVal_t, BasicBlock *> BBMap;

    for (auto &name : BB_PC) {
        BBMap[name.second] = BasicBlock::Create(context, name.first, mainFunc);
    }

    for (RegVal_t PC = 1; PC < Instructions.size(); PC++) {
        // Set IRBuilder to current BB
        if (BBMap.find(PC) != BBMap.end()) {
            builder.SetInsertPoint(BBMap[PC]);
        }
        // IR implementation for B instruction
        if (Instructions[PC]->m_ID == B) {
            builder.CreateBr(BBMap[Instructions[PC]->m_imm]);
            continue;
        }
        // IR implementation for BR_COND instruction
        if (Instructions[PC]->m_ID == BR_COND) {
            // arg
            Value *arg = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs1);
            Value *cond = builder.CreateLoad(builder.getInt64Ty(), arg);
            builder.CreateCondBr(cond, BBMap[Instructions[PC]->m_label1], BBMap[Instructions[PC]->m_label2]);
            continue;
        }
        // IR implementation for MUL instruction
        if (Instructions[PC]->m_ID == MUL) {
            // res
            Value *res_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs1);
            // arg1
            Value *arg1_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs2);
            // arg2
            Value *arg2_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs3);
            Value *add_arg1_arg2 = builder.CreateMul(builder.CreateLoad(builder.getInt64Ty(), arg1_p),
                                                     builder.CreateLoad(builder.getInt64Ty(), arg2_p));
            builder.CreateStore(add_arg1_arg2, res_p);
            continue;
        }
        // IR implementation for MULi instruction
        if (Instructions[PC]->m_ID == MULi) {
            // res
            Value *res_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs1);
            // arg1
            Value *arg1_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs2);
            // arg2
            Value *arg2 = builder.getInt64(Instructions[PC]->m_imm);
            Value *add_arg1_arg2 = builder.CreateMul(builder.CreateLoad(builder.getInt64Ty(), arg1_p), arg2);
            builder.CreateStore(add_arg1_arg2, res_p);
            continue;
        }
        // IR implementation for SELECT_FALSE instruction
        if (Instructions[PC]->m_ID == SELECT_FALSE) {
            // res
            Value *res_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs1);
            // arg1
            Value *arg1_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs2);
            // arg2
            Value *arg2_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs3);
            Value *res = builder.CreateSelect(builder.CreateLoad(builder.getInt1Ty(), arg1_p),
                                              builder.CreateLoad(builder.getInt1Ty(), arg2_p), builder.getFalse());
            builder.CreateStore(res, res_p);
            continue;
        }
        // IR implementation for OR instruction
        if (Instructions[PC]->m_ID == OR) {
            // res
            Value *res_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs1);
            // arg1
            Value *arg1_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs2);
            // arg2
            Value *arg2_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs3);
            Value *res = builder.CreateOr(builder.CreateLoad(builder.getInt1Ty(), arg1_p),
                                          builder.CreateLoad(builder.getInt1Ty(), arg2_p));
            builder.CreateStore(res, res_p);
            continue;
        }
        // IR implementation for XOR instruction
        if (Instructions[PC]->m_ID == XOR) {
            // res
            Value *res_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs1);
            // arg1
            Value *arg1_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs2);
            // arg2
            Value *arg2_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs3);
            Value *res = builder.CreateXor(builder.CreateLoad(builder.getInt64Ty(), arg1_p),
                                           builder.CreateLoad(builder.getInt64Ty(), arg2_p));
            builder.CreateStore(res, res_p);
            continue;
        }
        // IR implementation for ADD instruction
        if (Instructions[PC]->m_ID == ADD) {
            // res
            Value *res_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs1);
            // arg1
            Value *arg1_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs2);
            // arg2
            Value *arg2_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs3);
            Value *add_arg1_arg2 = builder.CreateAdd(builder.CreateLoad(builder.getInt64Ty(), arg1_p),
                                                     builder.CreateLoad(builder.getInt64Ty(), arg2_p));
            builder.CreateStore(add_arg1_arg2, res_p);
            continue;
        }
        // IR implementation for ADDi instruction
        if (Instructions[PC]->m_ID == ADDi) {
            // res
            Value *res_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs1);
            // arg1
            Value *arg1_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs2);
            // arg2
            Value *arg2 = builder.getInt64(Instructions[PC]->m_imm);
            Value *add_arg1_arg2 = builder.CreateAdd(builder.CreateLoad(builder.getInt64Ty(), arg1_p), arg2);
            builder.CreateStore(add_arg1_arg2, res_p);
            continue;
        }
        // IR implementation for SMODi instruction
        if (Instructions[PC]->m_ID == SMODi) {
            // res
            Value *res_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs1);
            // arg1
            Value *arg1_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs2);
            // arg2
            Value *arg2 = builder.getInt64(Instructions[PC]->m_imm);
            Value *smodi = builder.CreateSRem(builder.CreateLoad(builder.getInt64Ty(), arg1_p), arg2);
            builder.CreateStore(smodi, res_p);
            continue;
        }
        // IR implementation for UMODi instruction
        if (Instructions[PC]->m_ID == UMODi) {
            // res
            Value *res_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs1);
            // arg1
            Value *arg1_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs2);
            // arg2
            Value *arg2 = builder.getInt64(Instructions[PC]->m_imm);
            Value *smodi = builder.CreateURem(builder.CreateLoad(builder.getInt64Ty(), arg1_p), arg2);
            builder.CreateStore(smodi, res_p);
            continue;
        }
        // IR implementation for ICMP_EQ instruction
        if (Instructions[PC]->m_ID == ICMP_EQ) {
            // res
            Value *res_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs1);
            // arg1
            Value *arg1_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs2);
            // arg2
            Value *arg2 = builder.getInt64(Instructions[PC]->m_imm);
            Value *icmp_eq = builder.CreateICmpEQ(builder.CreateLoad(builder.getInt64Ty(), arg1_p), arg2);
            builder.CreateStore(icmp_eq, res_p);
            continue;
        }
        // IR implementation for INC_EQ instruction
        if (Instructions[PC]->m_ID == INC_EQ) {
            // res
            Value *res_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs1);
            // arg1
            Value *arg1_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs2);
            // arg2
            Value *arg2 = builder.getInt64(Instructions[PC]->m_imm);
            Value *incremented_val =
                builder.CreateAdd(builder.CreateLoad(builder.getInt64Ty(), arg1_p), builder.getInt64(1));
            builder.CreateStore(incremented_val, arg1_p);
            Value *icmp_eq = builder.CreateICmpEQ(builder.CreateLoad(builder.getInt64Ty(), arg1_p), arg2);
            builder.CreateStore(icmp_eq, res_p);
            continue;
        }
        // IR implementation for SEXT_FROMBOOL instruction
        if (Instructions[PC]->m_ID == SEXT_FROMBOOL) {
            // res
            Value *res_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs1);
            // arg1
            Value *arg1_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs2);
            Value *sext = builder.CreateSExt(builder.CreateLoad(builder.getInt1Ty(), arg1_p), builder.getInt64Ty());
            builder.CreateStore(sext, res_p);
            continue;
        }
        // IR implementation for SWAP instruction
        if (Instructions[PC]->m_ID == SWAP) {
            // arg1
            Value *arg1_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs1);
            // arg2
            Value *arg2_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs2);
            Value *tmp = builder.CreateLoad(builder.getInt64Ty(), arg1_p);
            Value *arg2 = builder.CreateLoad(builder.getInt64Ty(), arg2_p);
            builder.CreateStore(arg2, arg1_p);
            builder.CreateStore(tmp, arg2_p);
            continue;
        }
        // IR implementation for ALLOCA instruction
        if (Instructions[PC]->m_ID == ALLOCA) {
            // res
            Value *res_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs1);
            // imm
            Value *imm = builder.getInt64(Instructions[PC]->m_imm);
            Type *ArrayTy = ArrayType::get(builder.getInt64Ty(), static_cast<ConstantInt *>(imm)->getZExtValue());
            Value *alloc = builder.CreateAlloca(ArrayTy);
            // std::vector<Value *> Idxs = {builder.getInt64(0), builder.getInt64(0)};
            // Value *gep = builder.CreateInBoundsGEP(ArrayTy, alloc, Idxs);
            // builder.CreateStore(gep, res_p);
            builder.CreateStore(alloc, res_p);
            continue;
        }
        // IR implementation for LOAD instruction
        if (Instructions[PC]->m_ID == LOAD) {
            // res
            Value *res_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs1);
            // arg1
            Value *arg1_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs2);
            // arg2
            Value *arg2_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs3);

            Value *gep = builder.CreateInBoundsGEP(builder.getInt64Ty(),
                                                   builder.CreateLoad(Type::getInt64PtrTy(context), arg1_p),
                                                   builder.CreateLoad(builder.getInt64Ty(), arg2_p));
            Value *res = builder.CreateLoad(builder.getInt64Ty(), gep);
            builder.CreateStore(res, res_p);
            continue;
        }
        // IR implementation for STORE instruction
        if (Instructions[PC]->m_ID == STORE) {
            // res
            Value *res_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs1);
            // arg1
            Value *arg1_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs2);
            // arg2
            Value *arg2_p = builder.CreateConstGEP2_64(regFileType, regFile, 0, Instructions[PC]->m_rs3);

            Value *gep =
                builder.CreateInBoundsGEP(builder.getInt64Ty(), builder.CreateLoad(Type::getInt64PtrTy(context), res_p),
                                          builder.CreateLoad(builder.getInt64Ty(), arg1_p));
            builder.CreateStore(builder.CreateLoad(Type::getInt64Ty(context), arg2_p), gep);
            continue;
        }
        // Get poointer to instruction for function args
        Value *instr_p = builder.getInt64((uint64_t)Instructions[PC]);
        // Call simulation function for other instructions
        builder.CreateCall(module->getOrInsertFunction("do_" + Instructions[PC]->m_name, CalleType),
                           std::vector<Value *>({cpu_p, instr_p}));
    }
    outs() << "#[LLVM IR] DUMP\n";
    module->print(outs(), nullptr);
    outs() << "#[LLVM IR] END\n";
    if (need_to_run_ir) {
        for (int i = 0; i < REG_FILE_SIZE; i++) {
            cpu.REG_FILE[i] = 0;
        }

        // App simulation with execution engine
        outs() << "#[LLVM EE] RUN\n";
        InitializeNativeTarget();
        InitializeNativeTargetAsmPrinter();

        ExecutionEngine *ee = EngineBuilder(std::unique_ptr<Module>(module)).create();
        ee->InstallLazyFunctionCreator(lazyFunctionCreator);
        ee->addGlobalMapping(regFile, (void *)cpu.REG_FILE);
        ee->finalizeObject();
        ArrayRef<GenericValue> noargs;

        cpu.RUN = 1;
        cpu.PC = 0;
        ee->runFunction(mainFunc, noargs);
        outs() << "#[LLVM EE] END\n";

        // Registers dump after simulation with EE
        for (int i = 0; i < REG_FILE_SIZE; i++) {
            outs() << "[" << i << "] " << cpu.REG_FILE[i] << "\n";
        }
    }
    Instructions.clear();
    return 0;
}