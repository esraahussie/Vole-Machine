#include <bits/stdc++.h>
#include "Machine.h"
using namespace std;

void app() {
    Machine machine;
    MainUI ui;
    while (true) {
        ui.disPlayMenu();
        char choice = ui.inputChoice();

        switch (choice) {
            case '1': 
                if (ui.getFileOrlnstructions()) {
                    string filename = ui.inputFileName();
                    machine.loadProgramFile(filename);
                    machine.outputState();
                }
                else {
                    string instruction = ui.inputInstruction();
                    machine.loadProgram(instruction);
                }
                break;
            case '2':  
                cout << "Run:" << endl;
                cout << "1.Run the Whole program" << endl;
                cout << "2.Run one step"<<endl;
                char c;
                cin >> c;
                if(c == '1'){
                   cout << "----------Warning!!!--------\nif there is no C000 it will be infinty loop" << endl;
                   while(true){
                    machine.runNextStep();
                    machine.outputState();
                   }
                }
                else if(c == '2'){
                    machine.runNextStep();
                    machine.outputState();
                }
                else cout << "Invalid choice" << endl;
                break;
            case '3':  
                cout << "Outputting Machine State:" << endl;
                machine.outputState();
                break;
            case '4':  
                cout << "Exiting program." << endl;
                return;
            default:
                cout << "Invalid choice, please try again." << endl;
        }
    }
}

int main() {
  app();
  return 0;
}
