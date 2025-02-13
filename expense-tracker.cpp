#include "Expense_trcaker.h"
#include <deque>
#include <cstring>
#include <fstream>
#include <nlohmann/json.hpp>

deque<Expense_tracker> expenses;
const string filename = "expenses.json";
fstream file(filename);
nlohmann::json json;

int main(int argc, char* args[]) {
  try {
    try {
      file.open(filename, ios::in);
      file >> json;
    } catch(const exception& msg) {}
    file.close();
    for(const auto& root : json["expense"])
      expenses.emplace_back(root["id"], root["description"], root["amount"], root["date"]);
    
    if (argc == 6 && !(strcmp(args[1], "add") && strcmp(args[2], "--description") && strcmp(args[4], "--amount"))) {
      Expense_tracker expense(to_string(expenses.size() + 1), args[3], args[5]);
      expenses.push_back(expense);
      file.open(filename, ios::in);
      try {
        file >> json;
      } catch(const exception& msg) {}
      file.close();
      json["expense"].push_back({
        {"amount", expense.get_amount()},
        {"date", expense.get_date()},
        {"description", expense.get_description()},
        {"id", expense.get_id()}
      });
      file.open(filename, ios::out);
      file << json.dump(2);
      file.close();
      cout << "# Expense added successfully (ID: " << expense.get_id() << ")\n";
    }
    else if (argc == 4) {
      if (!(strcmp(args[1], "delete") && strcmp(args[2], "--id"))) {
        if (expenses.empty()) throw runtime_error ("List is empty!");
        deque<Expense_tracker>::iterator it = expenses.begin();
        const string id = args[3];
        for (const char& c : id)  if (!isdigit(c)) throw std::runtime_error("id not digit.");
        size_t uid = stoull(id);
        if (uid && expenses.size() >= uid) {
          it += uid - 1;
          expenses.erase(it);
          json["expense"].erase(json["expense"].begin() + uid - 1);
          if (uid != expenses.size() + 1) {
            for (size_t i = 0; i < expenses.size(); ++i) { 
              expenses.at(i).changeID(to_string(i + 1));
              json["expense"].at(i)["id"] = to_string(i + 1);
            }
          }
          cout << "Expense deleted successfully\n";
        }
        else throw runtime_error("not found id.");
        
        file.open(filename, ios::out);
        file << json.dump(2);
        file.close();
      }
      else if (!strcmp(args[1], "summary")) {
        const int m = stoi(string(args[3]));
        if (!(m > 0 && m < 13)) throw runtime_error("12 Monthes.");
        if (!strcmp(args[2], "--month")) {
          double result = 0;
          bool start = false;
          for(const auto& expense : expenses) {
            string target = expense.get_date().substr(expense.get_date().find('-') + 1, 2);
            if (stoi(target) == m) {
              start = true;
              result += stod(expense.get_amount());
            }
            else if (start && stoi(target) != m) break;
          }
          string month_name;
          switch (m) {
            case 1: 
            month_name = "January";
            break;
            case 2:
            month_name = "February";
            break;
            case 3:
            month_name = "March";
            break;
            case 4:
            month_name = "April";
            break;
            case 5:
            month_name = "May";
            break;
            case 6:
            month_name = "June";
            break;
            case 7:
            month_name = "July";
            break;
            case 8:
            month_name = "August";
            break;
            case 9:
            month_name = "September";
            break;
            case 10:
            month_name = "October";
            break;
            case 11:
            month_name = "November";
            break;
            case 12:
            month_name = "December";
            break;
          }
            if (!start) throw runtime_error("Not found expenses for " + month_name + '.');
            cout << "# Total expenses for " << month_name << ": $" << result << std::endl;
        }
      }
    }
    else if (argc == 2) {
      if (!strcmp(args[1], "list")) {
        if (expenses.empty()) throw runtime_error("List is empty!");
        cout << "# ID" << "\tDate" << "\t\tDescription" << "\tAmount\n";
        for(const Expense_tracker& expense : expenses) expense.display();
      }
      else if (!strcmp(args[1], "summary")) {
        if (expenses.empty()) throw runtime_error("List is empty!");
        double result = 0;
        for(const Expense_tracker& expense: expenses) result += stod(expense.get_amount());
        cout << "# Total expenses: $" << result << std::endl;
      }
      else if (!strcmp(args[1], "--help")) {
        cout << "Add expense:- \n\tadd --description ... --amount ...\n"
        "Delete expense:- \n\tdelete --id ...\n"
        "List expenses:- \n\tlist\n"
        "Summary expenses:- \n\tsummary\n";
      }
    }
    else cout << "Use --help";
  } catch(const exception& msg) {
    cerr << "Error!, " << msg.what();
  }
  return 0;
}