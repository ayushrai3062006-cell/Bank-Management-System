#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;

class BankAccount {
private:
    int accountNumber;
    string name;
    int pin;
    double balance;

public:
    // Account Create karne ka function
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

        // File me save karna
        ofstream outFile("bank_accounts.txt", ios::app);
        if (outFile.is_open()) {
            outFile << accountNumber << " " << pin << " " << balance << " " << name << endl;
            outFile.close();
            cout << "\nAccount Created Successfully!\n";
        } else {
            cout << "\nError saving account details!\n";
        }
    }

    // Money Deposit karne ka function
    void depositMoney(int accNum) {
        ifstream inFile("bank_accounts.txt");
        ofstream tempFile("temp.txt");
        bool found = false;

        int acc, p;
        double bal;
        string accName;

        while (inFile >> acc >> p >> bal) {
            getline(inFile >> ws, accName);
            if (acc == accNum) {
                found = true;
                double depositAmt;
                cout << "\nCurrent Balance: Rs " << bal << endl;
                cout << "Enter Amount to Deposit: ";
                cin >> depositAmt;

                if (depositAmt > 0) {
                    bal += depositAmt;
                    cout << "Deposit Successful! Updated Balance: Rs " << bal << endl;
                } else {
                    cout << "Invalid Deposit Amount!\n";
                }
            }
            tempFile << acc << " " << p << " " << bal << " " << accName << endl;
        }

        inFile.close();
        tempFile.close();

        remove("bank_accounts.txt");
        rename("temp.txt", "bank_accounts.txt");

        if (!found) cout << "\nAccount Number Not Found!\n";
    }

    // Money Withdraw karne ka function
    void withdrawMoney(int accNum) {
        ifstream inFile("bank_accounts.txt");
        ofstream tempFile("temp.txt");
        bool found = false;

        int acc, p;
        double bal;
        string accName;

        while (inFile >> acc >> p >> bal) {
            getline(inFile >> ws, accName);
            if (acc == accNum) {
                found = true;
                int enteredPin;
                cout << "Enter Security PIN: ";
                cin >> enteredPin;

                if (enteredPin == p) {
                    double withdrawAmt;
                    cout << "\nCurrent Balance: Rs " << bal << endl;
                    cout << "Enter Amount to Withdraw: ";
                    cin >> withdrawAmt;

                    if (withdrawAmt > 0 && withdrawAmt <= bal) {
                        bal -= withdrawAmt;
                        cout << "Withdrawal Successful! Remaining Balance: Rs " << bal << endl;
                    } else {
                        cout << "Insufficient Balance or Invalid Amount!\n";
                    }
                } else {
                    cout << "Incorrect PIN! Access Denied.\n";
                }
            }
            tempFile << acc << " " << p << " " << bal << " " << accName << endl;
        }

        inFile.close();
        tempFile.close();

        remove("bank_accounts.txt");
        rename("temp.txt", "bank_accounts.txt");

        if (!found) cout << "\nAccount Number Not Found!\n";
    }

    // All Accounts View karne ka function (Admin View)
    void displayAllAccounts() {
        ifstream inFile("bank_accounts.txt");
        if (!inFile.is_open()) {
            cout << "\nNo accounts found in database!\n";
            return;
        }

        int acc, p;
        double bal;
        string accName;

        cout << "\n========================================================\n";
        cout << left << setw(12) << "Acc No" << setw(20) << "Name" << setw(15) << "Balance (Rs)" << endl;
        cout << "========================================================\n";

        while (inFile >> acc >> p >> bal) {
            getline(inFile >> ws, accName);
            cout << left << setw(12) << acc << setw(20) << accName << setw(15) << fixed << setprecision(2) << bal << endl;
        }
        cout << "========================================================\n";
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
        showBanner(); // Har baar screen clear hoke banner aur menu dikhega

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
        }

        if (choice != 5) {
            cout << "\n\n  Press Enter to continue...";
            cin.ignore();
            cin.get();
        }

    } while (choice != 5);

    return 0;
}