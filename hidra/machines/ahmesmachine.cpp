#include "ahmesmachine.h"

AhmesMachine::AhmesMachine()
{
    identifier = "AHD";

    //////////////////////////////////////////////////
    // Initialize registers
    //////////////////////////////////////////////////

    registers = QVector<Register*>(2);

    AC = registers[0] = new Register("AC", 8);
    PC = registers[1] = new Register("PC", 8);



    //////////////////////////////////////////////////
    // Initialize memory
    //////////////////////////////////////////////////

    memory = QVector<Byte*>(MEM_SIZE);
    assemblerMemory = QVector<Byte*>(MEM_SIZE);
    reserved = QVector<bool>(MEM_SIZE);

    correspondingLine = QVector<int>(MEM_SIZE, -1); // Each PC value may be associated with a line of code

    for (int i=0; i<memory.size(); i++)
        memory[i] = new Byte();

    for (int i=0; i<assemblerMemory.size(); i++)
        assemblerMemory[i] = new Byte();

    setBreakpoint(0); // Reset breakpoint



    //////////////////////////////////////////////////
    // Initialize flags
    //////////////////////////////////////////////////

    flags = QVector<Flag*>(5);

    N = flags[0] = new Flag("N");
    Z = flags[1] = new Flag("Z", true);
    V = flags[2] = new Flag("V");
    C = flags[3] = new Flag("C");
    B = flags[4] = new Flag("B");



    //////////////////////////////////////////////////
    // Initialize instructions
    //////////////////////////////////////////////////

//    instructions = QVector<Instruction*>(24);

//    instructions[0]  = new Instruction("nop",   0, 0, 1);
//    instructions[1]  = new Instruction("sta",  16, 1, 2);
//    instructions[2]  = new Instruction("lda",  32, 1, 2);
//    instructions[3]  = new Instruction("add",  48, 1, 2);
//    instructions[4]  = new Instruction( "or",  64, 1, 2);
//    instructions[5]  = new Instruction("and",  80, 1, 2);
//    instructions[6]  = new Instruction("not",  96, 0, 1);
//    instructions[7]  = new Instruction("sub", 112, 1, 2);
//    instructions[8]  = new Instruction("jmp", 128, 1, 2);
//    instructions[9]  = new Instruction( "jn", 144, 1, 2);
//    instructions[10] = new Instruction( "jp", 148, 1, 2);
//    instructions[11] = new Instruction( "jv", 152, 1, 2);
//    instructions[12] = new Instruction("jnv", 156, 1, 2);
//    instructions[13] = new Instruction( "jz", 160, 1, 2);
//    instructions[14] = new Instruction("jnz", 164, 1, 2);
//    instructions[15] = new Instruction( "jc", 176, 1, 2);
//    instructions[16] = new Instruction("jnc", 180, 1, 2);
//    instructions[17] = new Instruction( "jb", 184, 1, 2);
//    instructions[18] = new Instruction("jnb", 188, 1, 2);
//    instructions[19] = new Instruction("shr", 224, 0, 1);
//    instructions[20] = new Instruction("shl", 225, 0, 1);
//    instructions[21] = new Instruction("ror", 226, 0, 1);
//    instructions[22] = new Instruction("rol", 227, 0, 1);
//    instructions[23] = new Instruction("hlt", 240, 0, 1);

    clearCounters();
    running = false;
}

//void AhmesMachine::step()
//{
//    Byte *operand = NULL;
//    int jumpAddress = 0;

//    int previousAC = AC->getValue();

//    // Read first byte
//    const Instruction* currentInstruction = getInstructionFromValue(memory[PC->getValue()]->getValue());
//    accessCount++;
//    instructionCount++;

//    if (currentInstruction->getNumBytes() == 2)
//    {
//        // Read second byte, which contains either the operand's address (ALU, load and store) or the jump's destination address:
//        int currentByteValue;

//        PC->incrementValue(); // Go to next byte
//        currentByteValue = memory[PC->getValue()]->getValue(); // Read byte
//        operand = memory[currentByteValue]; // Pointer to operand
//        jumpAddress = currentByteValue; // Address to jump to
//        accessCount++;
//    }

//    PC->incrementValue(); // Prepare for the next step
//    if(PC->getValue() == breakpoint)
//        this->running = false;

//    switch (currentInstruction->getValue())
//    {
//        case 0x00: // NOP
//            break;

//        case 0x10: // STA
//            operand->setValue(AC->getValue());
//            accessCount++;
//            break;

//        case 0x20: // LDA
//            AC->setValue(operand->getValue());
//            accessCount++;
//            break;

//        case 0x30: // ADD
//            AC->setValue((AC->getValue() + operand->getValue()) & 0xFF);
//            C->setValue(previousAC + operand->getValue() > MAX_VALUE); // Unsigned carry flag

//            // Signed overflow flag (true on incorrect result):
//            V->setValue(getSignedInt(previousAC) + getSignedInt(operand->getValue()) != getSignedInt(AC->getValue()));

//            accessCount++;
//            break;

//        case 0x40: // OR
//            AC->setValue(AC->getValue() | operand->getValue());
//            accessCount++;
//            break;

//        case 0x50: // AND
//            AC->setValue(AC->getValue() & operand->getValue());
//            accessCount++;
//            break;

//        case 0x60: // NOT
//            AC->setValue(AC->getValue() ^ 0xFF);
//            break;

//        case 0x70: // SUB
//            AC->setValue((AC->getValue() - operand->getValue()) & 0xFF);
//            B->setValue(previousAC - operand->getValue() < 0); // Unsigned borrow flag

//            // Signed overflow flag (true on incorrect result):
//            V->setValue(getSignedInt(previousAC) - getSignedInt(operand->getValue()) != getSignedInt(AC->getValue()));

//            accessCount++;
//            break;

//        case 0x80: // JMP
//            PC->setValue(jumpAddress);
//            break;

//        case 0x90: // JN
//            if (N->getValue())
//                PC->setValue(jumpAddress);
//            break;

//        case 0x94: // JP
//            if (!N->getValue())
//                PC->setValue(jumpAddress);
//            break;

//        case 0x98: // JV
//            if (V->getValue())
//                PC->setValue(jumpAddress);
//            break;

//        case 0x9C: // JNV
//            if (!V->getValue())
//                PC->setValue(jumpAddress);
//            break;

//        case 0xA0: // JZ
//            if (Z->getValue())
//                PC->setValue(jumpAddress);
//            break;

//        case 0xA4: // JNZ
//            if (!Z->getValue())
//                PC->setValue(jumpAddress);
//            break;

//        case 0xB0: // JC
//            if (C->getValue())
//                PC->setValue(jumpAddress);
//            break;

//        case 0xB4: // JNC
//            if (!C->getValue())
//                PC->setValue(jumpAddress);
//            break;

//        case 0xB8: // JB
//            if (B->getValue())
//                PC->setValue(jumpAddress);
//            break;

//        case 0xBC: // JNB
//            if (!B->getValue())
//                PC->setValue(jumpAddress);
//            break;

//        case 0xE0: // SHR
//            AC->setValue(AC->getValue() >> 1);
//            C->setValue((previousAC & 0x01) != 0);
//            break;

//        case 0xE1: // SHL
//            AC->setValue(AC->getValue() << 1);
//            C->setValue((previousAC & 0x80) != 0);
//            break;

//        case 0xE2: // ROR
//            AC->setValue((AC->getValue() >> 1) & 0xFF);
//            if (C->getValue())
//                AC->setValue(AC->getValue() | 0x80);
//            C->setValue((previousAC & 0x01) != 0);
//            break;

//        case 0xE3: // ROL
//            AC->setValue((AC->getValue() << 1) & 0xFF);
//            if (C->getValue())
//                AC->setValue(AC->getValue() | 0x01);
//            C->setValue((previousAC & 0x80) != 0);
//            break;

//        case 0xF0: // HLT
//            this->running = false;
//            break;

//        default:
//            break;
//    }

//    // Update flags:
//    Z->setValue(AC->getValue() == 0);
//    N->setValue(AC->getValue() > MAX_SIGNED_VALUE);
//}

//void AhmesMachine::run() {
//    this->running = true;
//    while (this->running && this->PC->getValue() <= 255) {
//        this->step();
//    }
//}

// EXACT DUPLICATE OF NEANDER'S METHOD:
void AhmesMachine::buildInstruction(QString mnemonic, QString arguments, QHash<QString, int> &labelPCMap)
{
    Instruction *instruction = getInstructionFromMnemonic(mnemonic);
    QStringList argumentList = arguments.split(" ", QString::SkipEmptyParts);
    int numberOfArguments = instruction->getArguments().size();

    // Check if correct number of arguments:
    if (argumentList.size() != numberOfArguments)
        throw wrongNumberOfArguments;

    // Write instruction:
    assemblerMemory[getPCValue()]->setValue(instruction->getByteValue());
    PC->incrementValue();

    if (numberOfArguments == 1)
    {
        // Convert possible label to number:
        if (labelPCMap.contains(argumentList[0]))
            argumentList[0] = QString::number(labelPCMap.value(argumentList[0]));

        // Check if valid argument:
        if (!isValidAddress(argumentList[0]))
            throw invalidAddress;

        // Write argument:
        assemblerMemory[PC->getValue()]->setValue(argumentList[0].toInt(NULL, 0));
        PC->incrementValue();
    }
}

void AhmesMachine::setCarry(bool state)
{
    setFlagValue("C", state);
}

void AhmesMachine::setBorrowOrCarry(bool state)
{
    setFlagValue("B", state);
}

void AhmesMachine::setOverflow(bool state)
{
    setFlagValue("V", state);
}
