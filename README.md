# RISC-V Branch Predictor Simulator

This project implements a simple **branch prediction simulator** for RISC-V assembly code. It simulates a branch predictor using configurable history bits and entries, showing prediction accuracy and mispredictions in a step-by-step manner.

---

## Overview

- The program reads RISC-V assembly code from `riscv_code.txt`.
- Supports basic instructions: `beq`, `li`, `addi`, and `add`.
- Implements a **2-bit saturating counter branch predictor** per entry with customizable history length (`hbn`) and number of entries (`en`).
- For each branch instruction (`beq`), it predicts whether the branch is taken or not, compares against actual behavior, updates predictor state, and tracks mispredictions.
- Displays detailed output showing predictor state, prediction outcome, and misprediction count.

---

## Input File Format (`riscv_code.txt`)

- Each line is either a label or an instruction.
- Labels are in the format: `label_name:`
- Instructions support these formats (all lowercase, no tabs):  
  - Branch: `beq reg1,reg2,label`  
  - Immediate load: `li reg,value`  
  - Immediate add: `addi reg,reg,value`  
  - Add: `add reg,reg,reg`  
- Registers can be written with or without prefix `R` or `x` (these are stripped during parsing).

Example:
  loop:
    li 1,5
    addi 2,1,3
    beq 1,2,loop
    add 3,1,2

---

## Features

### Branch Predictor

- Configurable **history bits** (`hbn`): number of bits to track recent branch outcomes per entry.
- Configurable **number of entries** (`en`): number of separate branch predictor entries indexed by instruction number modulo `en`.
- Each entry maintains a history register (`hb`) and a table of 2-bit saturating counters (`tbc`), each representing a prediction state:
  - `SN` = Strongly Not Taken  
  - `WN` = Weakly Not Taken  
  - `WT` = Weakly Taken  
  - `ST` = Strongly Taken  
- Prediction is based on the current state of the saturating counter indexed by the history bits.
- The predictor updates states and histories after each branch and tracks mispredictions.

---

## How It Works

1. The program reads and parses the assembly instructions and labels.
2. It asks the user for the number of history bits and the number of entries.
3. For each branch (`beq`), the simulator:
   - Uses the branch instruction index modulo entries as the predictor entry index.
   - Uses the current history bits for that entry to index the prediction state table.
   - Predicts taken or not taken based on the saturating counter state.
   - Compares prediction to actual branch outcome (`reg[rs1] == reg[rs2]`).
   - Updates predictor state and history bits accordingly.
   - Prints detailed info about the prediction and misprediction count.
4. Non-branch instructions (`li`, `addi`, `add`) update registers as expected.
5. The process continues until the end of the code.
6. The user can repeat the process by entering different history bits and entries or enter `0` to exit.

---

## Output Example

  How many bits(history bits)?
  2
  How many entries?
  4
  entry: 0 beq R1,R2,loop
  (00, SN, SN, SN, SN) N N misprediction: 0


The output shows:

- Current history bits for the entry.
- Predictor states for all possible history patterns.
- Prediction outcome (Taken or Not Taken).
- Actual branch outcome.
- Total mispredictions so far.

---

## Usage

- Place your RISC-V assembly code in `riscv_code.txt`.
- Compile and run the program.
- Enter desired history bits and number of entries when prompted.
- Observe prediction details and misprediction counts.
- Enter `0` for history bits to exit.

---

## Limitations

- Supports only a small subset of RISC-V instructions (`beq`, `li`, `addi`, `add`).
- Branch prediction is limited to the 2-bit saturating counter scheme.
- No support for other branch types or complex instructions.
- Assumes correct input format as described.

---
