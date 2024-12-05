#ifndef MACHINE_H
#define MACHINE_H
#include <bits/stdc++.h>
using namespace std;
int hexToDec(const string &hexStr) {
    int decimalValue;
    stringstream ss;
    ss << hex << hexStr;
    ss >> decimalValue;
    return decimalValue;
}

string decToHex(int decimal_number) {
    stringstream stream;
    stream << uppercase << setw(2) << setfill('0') << hex << decimal_number;
    return stream.str();
}
class ALU;

class CU;

class Machine;

class Memory;

class Register;

class CPU;

class MainUI;

class Register {
private:
    int size = 16;
    int memory[16];
    vector<string> reg;
public:
    Register() {
      reg = vector<string>(size, "00");
    }

    string getCell(int address) {
      if (address >= 0 && address < size) {
        return reg[address];
      }
      return "";
    }

    void setCell(int address, string value) {
      if (address >= 0 && address < size) {
        reg[address] = value;
      }
    }

    int getSize() { return size; }
};


class ALU {
public:
    bool isVaild(string instruction){
        if (instruction.size() != 4) {
            return false;
        }
        if (instruction[0] == 'C') {
            return instruction == "C000";
        }
        vector<char> valid_start = {'1', '2', '3', '4', '5', '6', 'B'};
        if (find(valid_start.begin(), valid_start.end(), instruction[0]) == valid_start.end()) {
            return false;
        }

        string sub_string = instruction.substr(1, 3);
        int start_index = 0;
        if (instruction[0] == '4') {
            start_index = 1;
            if (sub_string[0] != '0') {
                return false;
            }
        }

        for (int i=0;i<instruction.size();i++){
            if (!isxdigit(instruction[i])) {
                return false;
            }
        }
        return true;
    }

    void addIntegral(int idx1, int idx2, int idx3, Register &reg) {
        int a = hexToDec(reg.getCell(idx2));
        int b = hexToDec(reg.getCell(idx3));
        int result = (a + b)% 256;
        reg.setCell(idx1, decToHex(result));
    }

    float hexToFloat(string hex_value) {
        int int_value = hexToDec(hex_value);
        int exponent = ((int_value >> 4) & 7) - 8;
        float mantissa = (int_value & 15);
        return mantissa * pow(2.0, exponent);
    }

    string floatToHex(float value) {
        const float max = hexToFloat("FF");
        const float min = hexToFloat("00");

        if (value > max) return "FF";
        if (value < min) return "00";

        int integer = int(value * 16);
        int exponent = 0;

        while (integer > 15) {
            integer >>= 1;
            exponent++;
        }

        int hex_val = (exponent << 4) | integer;
        return decToHex(hex_val);
    }

    void addFloating(int idx1, int idx2, int idx3, Register &reg) {
        float a = hexToFloat(reg.getCell(idx2));
        float b = hexToFloat(reg.getCell(idx3));
        string result = floatToHex(a + b);
        reg.setCell(idx1, result);
    }

    void bitwiseAND(int idx1, int idx2, int idx3, Register &reg){
        int value1 = stoi(reg.getCell(idx2), nullptr, 16);
        int value2 = stoi(reg.getCell(idx3), nullptr, 16);
        int result = value1 & value2;
        string hexValue;
        if (result == 0) {
            hexValue = "0";
        }
        else {
            while (result > 0) {
                int remainder = result % 16;
                hexValue = "0123456789ABCDEF"[remainder] + hexValue;
                result /= 16;
            }
        }
        reg.setCell(idx1, hexValue);
    }

    void bitwiseXOR(int idx1, int idx2, int idx3, Register &reg){
        int value1 = stoi(reg.getCell(idx2), nullptr, 16);
        int value2 = stoi(reg.getCell(idx3), nullptr, 16);
        int result = value1 ^ value2;
        string hexValue;
        if (result == 0) {
            hexValue = "0";
        }
        else {
            while (result > 0) {
                int remainder = result % 16;
                hexValue = "0123456789ABCDEF"[remainder] + hexValue;
                result /= 16;
            }
        }
        reg.setCell(idx1, hexValue);
    }

    void bitwiseOR(int idx1, int idx2, int idx3, Register &reg){
        int value1 = stoi(reg.getCell(idx2), nullptr, 16);
        int value2 = stoi(reg.getCell(idx3), nullptr, 16);
        int result = value1 | value2;
        string hexValue;
        if (result == 0) {
            hexValue = "0";
        }
        else {
            while (result > 0) {
                int remainder = result % 16;
                hexValue = "0123456789ABCDEF"[remainder] + hexValue;
                result /= 16;
            }
        }
        reg.setCell(idx1, hexValue);
    }

    void rotate(int idx, int x, Register &reg) {
        int value = stoi(reg.getCell(idx), nullptr, 16);
        x = x % 16;
        int rotatedValue = (value << x) | (value >> (16 - x));
        rotatedValue &= 0xFFFF;
        stringstream stream;
        stream << hex << uppercase << rotatedValue;
        string hexValue = stream.str();
        while (hexValue.size() < 4) {
            hexValue = "0" + hexValue;
        }
        reg.setCell(idx, hexValue);
    }
};

class Memory {
private:
    int size = 256;
    string memory[256];
public:
    Memory() {
        for (int i = 0; i < size; ++i) {
        memory[i] = "00";
     }
    }
    string getCell(int address) {
       if (address >= 0 && address < size) {
       return memory[address];
    }
    throw out_of_range("Invalid memory address access.");
}

    void setCell(int address, string val) {
      if (address >= 0 && address < size) {
        memory[address] = val;
      }
    }

};
class CU {
public:
    void load(int idxReg, int intMem, Register &reg, Memory &mem) {
      reg.setCell(idxReg, mem.getCell(intMem));
    }

    void load(int idxReg, string val, Register &reg) {
      reg.setCell(idxReg, val);
    }

    void store(int idxReg, int idxMem, Register &reg, Memory &mem) {
      mem.setCell(idxMem, reg.getCell(idxReg));
    }

    void move(int idxReg1, int idxReg2, Register &reg) {
      reg.setCell(idxReg1, reg.getCell(idxReg2));
    }

    void jump(int idxReg, int idxMem, Register &reg, int &PC) {
        if(reg.getCell(idxReg) == reg.getCell(0)){
            PC = idxMem;
        }
    }

    void jump_(int idxReg, int idxMem, Register &reg, int &PC){
        if(reg.getCell(idxReg) > reg.getCell(0)){
            PC = idxMem;
        }
    }
};
class CPU : public ALU, public CU, public Register{
private:
    int programCounter;
    string instructionRegister;
    Register *aRegister;
    ALU *alu;
    CU *cu;
    Memory *memory;

public:
    CPU(Register *reg, Memory *mem, ALU *al, CU *controlUnit)
            : aRegister(reg), memory(mem), alu(al), cu(controlUnit), programCounter(0) {}

    void runNextStep(Memory &mem,Register &reg) {
      fetch(mem);
      outputState();
      execute(reg, mem);
    }

    void fetch(Memory &mem) {
      instructionRegister = mem.getCell(programCounter++);
      instructionRegister += mem.getCell(programCounter++);
    }

    void execute(Register &reg, Memory &mem) {
    cout << instructionRegister << endl;
    string opcode = instructionRegister.substr(0, 1);
    string operand1 = instructionRegister.substr(1, 1);
    string operand2 = instructionRegister.substr(2, 1);
    string operand3 = instructionRegister.substr(3, 1);
    cout << "Opcode: " << opcode << ", Operand1: " << operand1 << ", Operand2: " << operand2 << ", Operand3: " << operand3 << endl;
      if(opcode == "1"){
        int ireg = hexToDec(operand1);
        string xy = operand2 + operand2;
        int imem = hexToDec(xy);
        cu->load(ireg,imem,reg,mem);
      }
      else if(opcode == "2"){
        int ireg = hexToDec(operand1);
        string xy = operand2 + operand3;
        cu->load(ireg,xy,reg);
      }
      else if(opcode == "3"){
        int ireg = hexToDec(operand1);
        string xy = operand2 + operand3;
        int imem = hexToDec(xy);
        cu->store(ireg,imem,reg,mem);
        if(operand2 == "0" &&operand3 == "0"){
          cout << "Output \nDecimal: " << hexToDec(mem.getCell(0)) << " ASCII Equivalent: "<< char(hexToDec(mem.getCell(0))) << endl;
        }
      }
      else if(opcode == "4"){
        int ireg1 = hexToDec(operand2);
        int ireg2 = hexToDec(operand3);
        cu->move(ireg1,ireg2,reg);
      }
      else if(opcode == "5"){
        int ireg1 = hexToDec(operand1);
        int ireg2 = hexToDec(operand2);
        int ireg3 = hexToDec(operand3);
        alu->addIntegral(ireg1,ireg2,ireg3,reg);
      }
      else if(opcode == "6"){
        int ireg1 = hexToDec(operand1);
        int ireg2 = hexToDec(operand2);
        int ireg3 = hexToDec(operand3);
        alu->addFloating(ireg1,ireg2,ireg3,reg);
      }
      else if(opcode == "7"){
        int ireg1 = hexToDec(operand1);
        int ireg2 = hexToDec(operand2);
        int ireg3 = hexToDec(operand3);
        alu->bitwiseOR(ireg1,ireg2,ireg3,reg);
      }
      else if(opcode == "8"){
        int ireg1 = hexToDec(operand1);
        int ireg2 = hexToDec(operand2);
        int ireg3 = hexToDec(operand3);
        alu->bitwiseAND(ireg1,ireg2,ireg3,reg);
      }
      else if(opcode == "9"){
        int ireg1 = hexToDec(operand1);
        int ireg2 = hexToDec(operand2);
        int ireg3 = hexToDec(operand3);
        alu->bitwiseXOR(ireg1,ireg2,ireg3,reg);
      }
      else if(opcode == "A"){
        int ireg1 = hexToDec(operand1);
        int x = hexToDec(operand3);
        alu->rotate(ireg1,x,reg);
      }
      else if(opcode == "B"){
        int ireg = hexToDec(operand1);
        string xy = operand2 + operand3;
        int imem = hexToDec(xy);
        cu->jump(ireg,imem,reg,programCounter);
      }
      else if(opcode == "C"){
        cout << "Program Halted!" << endl;
        exit(0);
      }
      else if(opcode == "D"){
        int ireg = hexToDec(operand1);
        string xy = operand2 + operand2;
        int imem = hexToDec(xy);
        cu->jump_(ireg,imem,reg,programCounter);
      }
    }

    void outputState() {
      cout << "Program Counter: " << programCounter << endl;
      cout << "Instruction Register: " << instructionRegister << endl;
    }

private:
    void checkRegister(int reg1, int reg2, int reg3) {
      if (reg1 < 0 || reg1 >= aRegister->getSize() ||
          reg2 < 0 || reg2 >= aRegister->getSize() ||
          reg3 < 0 || reg3 >= aRegister->getSize()) {
        throw out_of_range("Invalid register number.");
      }
    }
};


class Machine {
private:
    CPU *processor;
    Memory *memory;
    Register *reg;
    ALU *alu;
    CU *cu;
public:
    Machine() {
        memory = new Memory();
        reg = new Register();
        alu = new ALU();
        cu = new CU();
        processor = new CPU(reg, memory, alu, cu);
    }

    ~Machine() {
        delete processor;
        delete memory;
        delete reg;
        delete alu;
        delete cu;
    }


    void loadProgramFile(const string &filename) {
      ifstream file(filename);
      if (!file) {
        cerr << "Error opening file: " << filename << endl;
        return;
      }
      string line;
      int address = 0;
      while (getline(file, line) && address < 256) {
        for (size_t i = 0; i < line.size() && address < 256; i += 2) {
            string chunk = line.substr(i, 2);
            if (chunk.size() < 2) chunk += "0";
            memory->setCell(address++, chunk);
        }
    }
      file.close();
    }

    void loadProgram(const string &instructions) {
    int address = 0;
    for (size_t i = 0; i < instructions.size() && address < 256; i += 2) {
        string chunk = instructions.substr(i, 2);
        if (chunk.size() < 2) chunk += "0"; 
        memory->setCell(address++, chunk);  
    }
}

    void outputState() {
    cout << "Machine State:" << endl;
    processor->outputState();
     cout << "Registers:" << endl;
      for (int i = 0; i < reg->getSize(); ++i) {
        cout << "R" << setw(2) << setfill('0') << hex << uppercase << i << ": ";
        cout << reg->getCell(i) << endl;
    }
    cout << "Memory State:" << endl;
    for (int row = 0; row < 16; ++row) {
        for (int col = 0; col < 16; ++col) {
            int address = row * 16 + col;
            cout << " " << setw(2) << setfill('0') << hex << uppercase << address << ": ";
            cout << memory->getCell(address) << "  ";
        }
        cout << endl;
    }
}

    void runNextStep() {
      processor->runNextStep(*memory,*reg);
    }
};

class MainUI {
private:
    bool enterFileOrlnstructions;
    Machine *machine;
    ALU *alu;
public:
    MainUI() {
      enterFileOrlnstructions = false;
      machine = nullptr;
    }

    bool getFileOrlnstructions() {
    char choice;
    while (true) {
        cout << "Enter 'f' to load instructions from a file or 'm' to enter instructions manually: ";
        cin >> choice;
        if (choice == 'f' || choice == 'F' || choice == 'm' || choice == 'M') {
            enterFileOrlnstructions = (choice == 'f' || choice == 'F');
            return enterFileOrlnstructions;
        } else {
            cout << "Invalid input. Please enter 'f' or 'm'." << endl;
        }
    }
}

    void disPlayMenu() {
      cout << "*** CPU Simulator ***\n";
      cout << "1. Load Program\n";
      cout << "2. Run Next Step\n";
      cout << "3. Output State\n";
      cout << "4. Exit\n";
    }

    string inputFileName() {
      string filename;
      cout << "Enter the program file_name: ";
      cin >> filename;
      return filename;
    }

    string inputInstruction() {
      string instruction;
      cout << "Enter the instruction: ";
      cin.ignore();
      getline(cin, instruction);
        while (true) {
            if (alu->isVaild(instruction)) {
                return instruction;
                break;
            }
            else {
                cout << "Enter a valid instruction\n";
                getline(cin, instruction);
            }
        }
    }

    char inputChoice() {
      char choice;
      cout << "Enter your choice: ";
      cin >> choice;
      return choice;
    }
};
#endif
