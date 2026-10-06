#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>

using namespace std;

// XOR Encryption & Decryption Helper Function
string encryptDecryptPIN(string pin) {
    char key = 'K'; // Secret Encryption Key
    string output = pin;
    for (size_t i = 0; i < pin.size(); i++) {
        output[i] = pin[i] ^ key;
    }
    return output;
}

// Global Helper: Invalid input handle karne ke liye
void clearInputBuffer() {
    cin.clear();
    cin.ignore(10000, '\n');
}

class BankAccount {
private:
    int accountNumber;
    string name;
    string pin;
    double balance;

public:
    // Account Create karne ka function (With PIN Encryption)
    void createAccount() {
        cout << "\n====================================\n";
        cout << "      NEW ACCOUNT REGISTRATION      \n";
        cout << "====================================\n";
        cout << "Enter Account Number (4-digits): ";
        cin >> accountNumber;
        cin.ignore();

        cout << "Enter Account Holder Name: ";
        getline(cin, name);

        cout << "Set 4-Digit Security PIN: ";
        cin >> pin;

        cout << "Enter Initial Deposit Amount (Min 500): ";
        cin >> balance;

        while (balance < 500) {
            cout << "Minimum initial deposit is 500! Re-enter amount: ";
            cin >> balance;
        }

        // File me ENCRYPTED PIN save kar rahe hain
        ofstream outFile("bank_accounts.txt", ios::app);
        if (outFile.is_open()) {
            string encryptedPIN = encryptDecryptPIN(pin);
            outFile << accountNumber << " " << encryptedPIN << " " << balance << " " << name << endl;
            outFile.close();
            cout << "\n[SUCCESS] Account Created Successfully! PIN has been encrypted.\n";
        } else {
            cout << "\n[ERROR] Error saving account details!\n";
        }
    }

    // Money Deposit karne ka function
    void depositMoney(int accNum) {
        ifstream inFile("bank_accounts.txt");
        ofstream tempFile("temp.txt");
        bool found = false;

        int acc;
        string encryptedPinFromFile;
        double bal;
        string accName;

        while (inFile >> acc >> encryptedPinFromFile >> bal) {
            getline(inFile >> ws, accName);
            if (acc == accNum) {
                found = true;
                double depositAmt;
                cout << "\nCurrent Balance: Rs " << bal << endl;
                cout << "Enter Amount to Deposit: ";
                cin >> depositAmt;

                if (depositAmt > 0) {
                    bal += depositAmt;
                    cout << "\n[SUCCESS] Deposit Successful! Updated Balance: Rs " << bal << endl;
                } else {
                    cout << "\n[ERROR] Invalid Deposit Amount!\n";
                }
            }
            tempFile << acc << " " << encryptedPinFromFile << " " << bal << " " << accName << endl;
        }

        inFile.close();
        tempFile.close();

        remove("bank_accounts.txt");
        rename("temp.txt", "bank_accounts.txt");

        if (!found) cout << "\n[ERROR] Account Number Not Found!\n";
    }

    // Money Withdraw karne ka function (With XOR Verification)
    void withdrawMoney(int accNum) {
        ifstream inFile("bank_accounts.txt");
        ofstream tempFile("temp.txt");
        bool found = false;

        int acc;
        string encryptedPinFromFile;
        double bal;
        string accName;

        while (inFile >> acc >> encryptedPinFromFile >> bal) {
            getline(inFile >> ws, accName);
            if (acc == accNum) {
                found = true;
                string enteredPin;
                cout << "Enter Security PIN: ";
                cin >> enteredPin;

                // Input PIN ko encrypt karke stored encrypted PIN se verify kar rahe hain
                if (encryptDecryptPIN(enteredPin) == encryptedPinFromFile) {
                    double withdrawAmt;
                    cout << "\nCurrent Balance: Rs " << bal << endl;
                    cout << "Enter Amount to Withdraw: ";
                    cin >> withdrawAmt;

                    if (withdrawAmt > 0 && withdrawAmt <= bal) {
                        bal -= withdrawAmt;
                        cout << "\n[SUCCESS] Withdrawal Successful! Remaining Balance: Rs " << bal << endl;
                    } else {
                        cout << "\n[ERROR] Insufficient Balance or Invalid Amount!\n";
                    }
                } else {
                    cout << "\n[SECURITY ALERT] Incorrect PIN! Access Denied.\n";
                }
            }
            tempFile << acc << " " << encryptedPinFromFile << " " << bal << " " << accName << endl;
        }

        inFile.close();
        tempFile.close();

        remove("bank_accounts.txt");
        rename("temp.txt", "bank_accounts.txt");

        if (!found) cout << "\n[ERROR] Account Number Not Found!\n";
    }

    // All Accounts View karne ka function
    void displayAllAccounts() {
        ifstream inFile("bank_accounts.txt");
        if (!inFile) {
            cout << "\nNo records found." << endl;
            return;
        }

        int acc;
        string encryptedPinFromFile;
        double bal;
        string accName;

        cout << "\n=======================================================\n";
        cout << left << setw(12) << "Acc No" << setw(15) << "Balance (Rs)" << setw(20) << "Name" << endl;
        cout << "=======================================================\n";

        while (inFile >> acc >> encryptedPinFromFile >> bal) {
            getline(inFile >> ws, accName);
            cout << left << setw(12) << acc << setw(15) << fixed << setprecision(2) << bal << setw(20) << accName << endl;
        }
        cout << "=======================================================\n";
        inFile.close();
    }
};

void showBanner() {
    system("clear");

    cout << "=======================================================\n";
    cout << " ██████╗  █████╗ ███╗   ██╗██╗  ██╗    ██████╗ ██╗  ██╗\n";
    cout << " ██╔══██╗██╔══██╗████╗  ██║██║ ██╔╝    ██╔══██╗██║  ██║\n";
    cout << " ██████╔╝███████║██╔██╗ ██║█████═╝     ██████╔╝███████║\n";
    cout << " ██╔══██╗██╔══██║██║╚██╗██║██╔═██╗     ██╔═══╝ ██╔══██║\n";
    cout << " ██████╔╝██║  ██║██║ ╚████║██║  ██╗    ██║     ██║  ██║\n";
    cout << " ╚═════╝ ╚═╝  ╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝    ╚═╝     ╚═╝  ╚═╝\n";
    cout << "            -- SECURE BANKING SYSTEM --            \n";
    cout << "=======================================================\n\n";
}

int main() {
    BankAccount bank;
    int choice;

    do {
        showBanner();

        cout << "  ┌─────────────────────────────────────────┐\n";
        cout << "  │               MAIN MENU                 │\n";
        cout << "  ├─────────────────────────────────────────┤\n";
        cout << "  │  1. Create New Account                  │\n";
        cout << "  │  2. Deposit Amount                      │\n";
        cout << "  │  3. Withdraw Amount                     │\n";
        cout << "  │  4. Display All Accounts                │\n";
        cout << "  │  5. Exit System                         │\n";
        cout << "  └─────────────────────────────────────────┘\n";
        cout << "  Enter Choice [1-5]: ";
        cin >> choice;

        switch (choice) {
            case 1:
                bank.createAccount();
                break;
            case 2: {
                int accNo;
                cout << "\n  Enter Account Number for Deposit: ";
                cin >> accNo;
                bank.depositMoney(accNo);
                break;
            }
            case 3: {
                int accNo;
                cout << "\n  Enter Account Number for Withdraw: ";
                cin >> accNo;
                bank.withdrawMoney(accNo);
                break;
            }
            case 4:
                bank.displayAllAccounts();
                break;
            case 5:
                cout << "\n  Thank you for using our Banking System!\n";
                break;
            default:
                cout << "\n  Invalid Choice!";
                clearInputBuffer();
        }

        if (choice != 5) {
            cout << "\n\n  Press Enter to continue...";
            cin.ignore();
            cin.get();
        }

    } while (choice != 5);

    return 0;
}