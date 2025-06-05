#include <iostream>
#include <ostream>
#include <string>
#include "sqlite3.h"
#include <vector>
#include <memory>

using namespace std;

class Database {
public:
  sqlite3* db;
  vector<Database> dbList;

  Database(const char* filename) : db(nullptr) {
    int exit = sqlite3_open(filename, &db);
    if (exit) {
      cerr << "Error opening DB: " << sqlite3_errmsg(db) << endl;
      db = nullptr;
    } else {
      cout << "Database opened successfully!" << endl;
    }
  }

  //Destructor
  
  ~Database() {
    if (db) {
      sqlite3_close(db);
      cout << "Database closed" << endl;
    }
  }

  //Check if database opened successfully
  bool isOpen() const {
    return db != nullptr;
  }

  //Execute a SQL command (create table, insert, etc.)
  bool execute(const string& sql) {
    if (!db) return false;

    char* errMsg = nullptr;
    int exit = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);

    if (exit != SQLITE_OK) {
      cerr << "SQL error: " << errMsg << endl;
      sqlite3_free(errMsg);
      return false;
    }

    return true;
  }

};

class App {
public:
  bool programRunning = true;
  string inp;
  vector<unique_ptr<Database>> dbList;

  App() {
    
  }

  void takeInput(string value) {
    inp = value;
  }

  void checkQuit() {
    if (inp == "quit" or inp == "exit") {
      programRunning = false;
      cout << "Exiting program..." << endl;
    }
  }

  void checkCommand(string input) {
    if (input == "create") {
      string name;

      cout << "input database name: ";
      cin >> name;
   
      auto db = make_unique<Database>((name + ".db").c_str());
      dbList.push_back(std::move(db));
    }
  }

};

int main() {

  App app;

  while( app.programRunning ) {
    string input;
    cin >> input;

    app.takeInput(input);
    app.checkQuit();

  }

  return 0;

}
