#ifndef EXPENSE_TRACKER
#define EXPENSE_TRACKER
#include <string>
#include <stdexcept>
#include <iostream>
#include <cctype>
#include <chrono>

using namespace std;

class Expense_tracker {
  private:
  string id, date, description, amount;

  public:
  explicit Expense_tracker(const string& id, const string& description, const string& amount, const string& date = format("{:%Y-%m-%d}", chrono::system_clock::now())) {
    for (const char& c : id)
      if (!isdigit(c)) throw std::runtime_error("id not digit.\n");
    for (const char& c : amount)
      if (!(isdigit(c) || c == '.')) throw std::runtime_error("amount not floating.\n");
    this->id = id;
    this->date = date;
    this->description = description;
    this->amount = amount;
  }

  inline void changeID(const string& id) {
    for (const char& c : id)
      if (!isdigit(c)) throw std::runtime_error("id not digit.\n");
    this->id = id;
  }

  inline string get_amount() const { return amount; }
  inline string get_id() const { return id; }
  inline string get_description() const { return description; }
  inline string get_date() const { return date; }
  inline void display() const {
    cout << "# " << id << '\t' << date << '\t' << description << "\t\t" << '$' << amount << '\n';
  }
};


#endif