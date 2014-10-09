#include <iostream>
#include <fstream>
#include <map>
#include <String>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

using namespace std;

typedef unsigned char byte;

class Vm{
	public:
		byte mem[1024];
		byte instr[1024];
		char test[100];
		 Vm(){
			pc = 0;
			running = true;
		}
	
		byte* getAccess(bool instr_mem){
			byte * temp;
			if(instr_mem)
				temp =  instr;
			else
				temp = mem;
			return temp;
		}
		
		void run(bool debug = false){
				while(running){
					if(debug)
					cout << (int)instr[pc] << " " << (int)instr[pc + 1] << " " << (int)instr[pc + 2] << endl;
					switch(instr[pc]){
						//trp
						case 0:
							switch(instr[pc + 1]){
								//Terminate the program
								case 0:
									running = false;
									break;
								//write an int
								case 1:
									cout << regs[0];
									break;
								//read an int
								case 2:
									cin >> regs[0];
									break;
								//write a char
								case 3:
									cout << (char)regs[0];
									break;
								//read a char
								case 4:
									cin >> regs[0];
									break;
								default:	
									break;	
							}
							pc += INSTR;
							break;
						//jmp
						case 1:
							pc = ((int)instr[pc + 1] + ((int)(instr[pc + 2]) << 8) + ((int)(instr[pc + 3]) << 16) + ((int)(instr[pc + 4]) << 24));
							break;
						//jmr
						case 2:
							pc = regs[instr[pc + 1]];
							break;
						//bnz
						case 3:
							if(regs[instr[pc + 1]] != 0)
								pc = instr[pc + 2];
							else
								pc += INSTR;
							break;
						//bgt
						case 4:
							if(regs[instr[pc + 1]] > 0)
								pc = instr[pc + 2];
							else
								pc += INSTR;
							break;
						//blt
						case 5:
							if(regs[instr[pc + 1]] < 0)
								pc = instr[pc + 2];
							else
								pc += INSTR;
							break;
						//brz
						case 6:
							if(regs[instr[pc + 1]] == 0)
								pc = instr[pc + 2];
							else
								pc += INSTR;
							break;
						//mov
						case 7:
							regs[instr[pc + 1]] = regs[instr[pc + 2]];
							pc += INSTR;
							break;
						//lda
						case 8:
							regs[instr[pc + 1]] = ((int)instr[pc + 2] + (int)(instr[pc + 3] << 8) + (int)(instr[pc + 4] << 16) + (int)(instr[pc + 5] << 24));
							pc += INSTR;
							break;
						//str
						case 9:
							mem[instr[pc + 2]] = (regs[instr[pc + 1]] & 0x000000ff);
							mem[instr[pc + 3]] = (regs[instr[pc + 1]] & 0x0000ff00) >> 8;
							mem[instr[pc + 4]] = (regs[instr[pc + 1]] & 0x00ff0000) >> 16;
							mem[instr[pc + 5]] = (regs[instr[pc + 1]] & 0xff000000) >> 24;
							pc += INSTR;
							break;
						//ldr	
						case 10:
							regs[instr[pc + 1]] = (int)mem[instr[pc + 2]] + (int)(mem[instr[pc + 3]] << 8) + (int)(mem[instr[pc + 4]] << 16) + (int)(mem[instr[pc + 5]] << 24);
							pc += INSTR;
							break;
						//stb
						case 11:
							mem[instr[pc + 2]] = regs[instr[pc + 1]];
							pc += INSTR;
							break;
						//ldb
						case 12:
							regs[instr[pc + 1]] = mem[instr[pc + 2]];
							pc += INSTR;
							break;
						//add
						case 13:
							regs[instr[pc + 1]] += regs[instr[pc + 2]];
							pc += INSTR;
							break;
						//adi
						case 14:
							//should use int to byte type thing
							regs[instr[pc + 1]] += ((int)(instr[pc + 2]) + (int)(instr[pc + 3] << 8) + (int)(instr[pc + 4] << 16) + (int)(instr[pc + 5] << 24));
							pc += INSTR;
							break;
						//sub
						case 15:
							regs[instr[pc + 1]] -= regs[instr[pc + 2]];
							pc += INSTR;
							break;
						//mul
						case 16:
							regs[instr[pc + 1]] *= regs[instr[pc + 2]];
							pc += INSTR;
							break;
						//div
						case 17:
							regs[instr[pc + 1]] = regs[instr[pc + 1]] / regs[instr[pc + 2]];
							pc += INSTR;
							break;
						//and
						case 18:
							if(regs[instr[pc + 1]] && regs[instr[pc + 2]])
								regs[instr[pc + 1]] = 1;
							else
								regs[instr[pc + 1]] = 0;
							pc += INSTR;
							break;
						//or
						case 19:
							if(regs[instr[pc + 1]] || regs[instr[pc + 2]])
								regs[instr[pc + 1]] = 1;
							else
								regs[instr[pc + 1]] = 0;
							pc += INSTR;
							break;
						//cmp
						case 20:
							if(regs[instr[pc + 1]] > regs[instr[pc + 2]])
								regs[instr[pc + 1]] = 1;
							else if(regs[instr[pc + 1]] < regs[instr[pc + 2]])
								regs[instr[pc + 1]] = -1;
							else
								regs[instr[pc + 1]] = 0;
							pc += INSTR;						
							break;
						default:
							pc += INSTR;
					}//end switch statement
				}//end while loop	
		}//end of function
	
	private:
		int pc;
		int regs[7];	
		const static int INSTR = 8;
		bool running;
			
};

class Assembler{
		
	private:
		char loc[24];
		ifstream reader;
		byte instrCounter;
		byte memCounter;
		byte temp;
		const static int BYTE = 1;
		const static int INTEGER = 4;
		const static int INSTRUCTION = 8;
	    map <char*, byte>* symbTable;
		char* token;
		char* temp1;
		int temp2;
		
        pair<map<char*,byte>::iterator,bool> ret;

	public:
		
		
		Assembler(){
					
			symbTable = new map<char*, byte>;
		}
			//We are going to just deal with lower case characters
	void lower(char* s){
		int i = 0;
		char * temp;
		temp = s;
		while(temp[i] != '\0'){
			
			temp[i] = tolower(temp[i]);
			i++;
		}	
	}
	
	void showMem(byte * mem){
				
		int j = 0;
		while (j < 100){
			
			cout << (int)mem[j] << " " << (int)mem[j + 1] << " " << (int)mem[j + 2] << " " << (int)mem[j + 3] << " " << (int)mem[j + 4] << " " << endl;
			j += 5;
		}
		
		cout << endl << endl;
		
	}

	byte getReg(char* op){
		lower(op);
		if(*op == 'r') {
			if((*(op+1)) <= 56 && (*(op+1)) >= 48){
				if(*(op+2) == '\0' || *(op+2) == ',' || *(op+2) == '\n' || *(op+2) == ' '){
					return (byte)(*(op+1)) - 48;
				}
			}
		}
	}
	
	void assemble(string fname, map<string, byte> instructions, bool firstPass, byte * memory = NULL, byte * instrMem = NULL,  bool debug = false){
	
		try{
			reader.open(fname.c_str());
			memCounter = 0;
			instrCounter = 0;
		}
		catch(exception e){
			cout << "Houston We have a problem" << endl;
		}
		
		while(!reader.eof()){
		
			reader.getline(loc, 24);
			token = strtok(loc, " \t");
			if(token == NULL){
				continue;}
			lower(token);
			if(instructions.count(token) != 1){
				temp1 = token;
				token = strtok(NULL, " \t");
				lower(token);
				
				if(firstPass){
					if(token[0] == '.'){
						cout << "The addy is " << (int)memCounter << endl;
					    ret = symbTable->insert(pair<char*, byte>(temp1, memCounter));
					    if(!ret.second)
					    	cout << "it didnt work" << endl;
					}
					else{
						symbTable->insert(make_pair(temp1, 10));
					}
				}
			//	if(debug)
					cout << "label is: " << temp1 << " value = " << (int)symbTable->find(temp1)->second << endl;
			}

			if(instructions.count(token) == 1){
				//If it is a load store or branch command 
				//it is in this format [op] [reg] [label]

				if(token[0] == 'l' || (token[0] == 's' && token[1] == 't') || token[0] == 'b'){
					
					if(firstPass)
						instrCounter += INSTRUCTION;
					
					else{		
						instrMem[instrCounter] = instructions[token];
										
						token = strtok(NULL, " \t");	
						temp = getReg(token);
							if(temp <= 7 || temp >= 0)
								instrMem[instrCounter+1] = temp;
					
						token = strtok(NULL, " \t");	
						instrMem[instrCounter + 2] = symbTable->find(token)->second ;
							
						if(debug)
								cout << (int)instrMem[instrCounter] << " " << (int)instrMem[instrCounter + 1] << " " << (int)instrMem[instrCounter+2] << endl;
						//fix instruction boundary
						instrCounter += 8;
					
					}
				}
				
				//if it is a move, divide, mul, cmp, ir, and, sub, add command
				//it is in this format [op] [reg] [reg]
				else if(token[0] == 'm' || token[0] == 'd' || token[0] == 'o' || token[0] == 'c' || (token[0] == 'a' && token[2] == 'd') || (token[0] == 's' && token[1] == 'u')){
					if(firstPass)
						instrCounter += INSTRUCTION;
					
					else{
						instrMem[instrCounter] = instructions[token];
						
						token = strtok(NULL, " \t");	
						temp = getReg(token);
						if(temp <= 7 || temp >= 0)
							instrMem[instrCounter + 1] = temp;
							
						token = strtok(NULL, " \t");	
						temp = getReg(token);
							if(temp <= 7 || temp >= 0)
								instrMem[instrCounter + 2] = temp;
						
						if(debug)
							cout << (int)instrMem[instrCounter] << " " << (int)instrMem[instrCounter + 1] << " " << (int)instrMem[instrCounter + 2] << endl;
						
						//fix instruction boundary
						instrCounter += 8;					
					}
				}
				
				//if it is a trp instruction
				//it is in this format [op] [imm]
				else if (token[0] == 't'){
					if(firstPass){
						instrCounter += INSTRUCTION;
					}
						
					
					else{
						
						instrMem[instrCounter] = instructions[token];
						
						token = strtok(NULL, " \t");	
						temp = token[0] - 48;
						if(temp >= 0 && temp <= 4)
							instrMem[instrCounter + 1] = temp;
							
						if(debug)
							cout << (int)instrMem[instrCounter] << " " << (int)instrMem[instrCounter + 1] << endl;
						//fix instruction boundary
						instrCounter += 8;
					}
				}
				
				//jmp instruction
				//it is in this format [op] [label]				
				else if(token[0] == 'j' && token[2] == 'p'){
					if(firstPass)
						instrCounter += INSTRUCTION;
						
					else{
						
						instrMem[instrCounter] = instructions[token];
						
						
						token = strtok(NULL, " \t");	
						instrMem[instrCounter + 1] = symbTable->find(token)->second;
						
						//fix instruction boundary
						instrCounter += 8;
					}
				}
				
				//jmr instruction
				//it is in this format [op] [reg]
				else if(token[0] == 'j'){
					if(firstPass)
						instrCounter += INSTRUCTION;
						
					else{
						
						instrMem[instrCounter] = instructions[token];
						
						token = strtok(NULL, " \t");	
						temp = getReg(token);
						if(temp <= 7 || temp >= 0)
							instrMem[instrCounter + 1] = temp;
							
						//fix instruction boundary
						instrCounter += 6;
					}
				}
				
				
				//if it is a directive
				//it is in this format [op] [imm]
				else if(token[0] == '.'){
					if(firstPass){
						if(token[1] == 'b'){
							memCounter += BYTE;
						}
						else if(token[1] == 'i'){					
							memCounter += INTEGER;
						}
					}
					
					else{
						if(token[1] == 'b'){
							token = strtok(NULL, " \t");
							if(token[0] == 39){
								memory[memCounter] = (byte)token[1];
							}
							else{
								temp2 = atoi(token);
								if(temp2 <= 255){
									memory[memCounter] = temp2 & 0x000000ff;
								}
							}
							memCounter += BYTE;
						}
				
						if(token[1] == 'i'){
							temp2 = atoi(strtok(NULL, " \t"));
							memory[memCounter] = (temp2 & 0xff000000) >> 24;
							memory[memCounter + 1] = (temp2 & 0x00ff0000) >> 16;
							memory[memCounter + 2] = (temp2 & 0x0000ff00) >> 8;
							memory[memCounter + 3] = (temp2 & 0x000000ff);							
							memCounter += INTEGER;
						}
					}
				}
				
				//it must be adi
				//in this format [op] [reg] [label]
				else{
					
					instrMem[instrCounter] = instructions[token];
					
					token = strtok(NULL, " \t");	
					temp = getReg(token);
						if(temp <= 7 || temp >= 0)
							instrMem[instrCounter + 1] = temp;
					if(debug)
						cout << (int)instrMem[instrCounter] << " " << (int)instrMem[instrCounter + 1] << endl;
					
					//fix instruction boundary
					instrCounter += 8;
				}
			}
		}
		reader.close();
		if(memory != NULL && debug)
		showMem(memory);
	//end of assemble function
	}
};

int main(int argc, char** argv) {

	map <string, byte> instrSet;
	//implemented instructions
	instrSet["trp"] = 0;
	instrSet["jmp"] = 1;
	instrSet["jmr"] = 2;
	instrSet["bnz"] = 3;
	instrSet["bgt"] = 4;
	instrSet["blt"] = 5;
	instrSet["brz"] = 6;
	instrSet["mov"] = 7;
	instrSet["lda"] = 8;
	instrSet["str"] = 9;
	instrSet["ldr"] = 10;
	instrSet["stb"] = 11;
	instrSet["ldb"] = 12;
	instrSet["add"] = 13;
	instrSet["adi"] = 14;
	instrSet["sub"] = 15;
	instrSet["mul"] = 16;
	instrSet["div"] = 17;
	instrSet["and"] = 18;
	instrSet["or"] = 19;
	instrSet["cmp"] = 20;
	instrSet[".int"] = 0;
	instrSet[".byt"] = 0;

	
	Vm computer = Vm();
	Assembler assemblinator;
	//first pass
	assemblinator.assemble("proj1.asm", instrSet, true);
	//second pass
	assemblinator.assemble("proj1.asm", instrSet, false, computer.getAccess(false), computer.getAccess(true));
	//lets do this
	computer.run();
	
};





