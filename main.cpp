#include <algorithm>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>
#include "sqlite3.h"
#include <vector>
#include <memory>
#include <cstdio>

using namespace std;

//________________________________________________________________

void newCmd() {
  cout << "________________________________________________________________" << endl;
}

class Database {
public:
  sqlite3* db;
  string name;

Database(const char* filename) : db(nullptr), name(filename) {
    int exit = sqlite3_open(filename, &db);
    if (exit) {
      cerr << "Error opening DB: " << sqlite3_errmsg(db) << endl;
      db = nullptr;
    } else {
      cout << "Database opened successfully!" << endl;
    }
}  //Destructor
  
  ~Database() {
    if (db) {
      sqlite3_close(db);
      cout << "Database closed" << endl;
    }
  }

  void close() {
    if (db) {
      sqlite3_close(db);
      db = nullptr;
      cout << "Database closed." << endl;
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


//_____________________________________________________________________


class App {
public:
  bool programRunning = true;
  string inp;
  vector<unique_ptr<Database>> dbList;
  vector<string> dbListNames;

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
      dbListNames.push_back(name);
   
      auto db = make_unique<Database>((name + ".db").c_str());
      dbList.push_back(std::move(db));
    }
    else if (input == "list") {
      cout << "\n";
      for (size_t i = 0; i < dbListNames.size(); ++i) {
        cout << i+1 << ". " << dbListNames[i] << endl;
      }
    }
    else if (input == "remove") {
      
      string filenameNoExt;
      
      cout << "input filename: ";
      cin >> filenameNoExt;

      string filename = filenameNoExt+".db";

        // 1. Find database object with matching name and close it
      auto it = std::find_if(dbList.begin(), dbList.end(),
        [&](const unique_ptr<Database>& db) {
          return db->name == filename;
        });

      if (it != dbList.end()) {
        (*it)->close();        // Close the DB connection
        dbList.erase(it);      // Remove from vector
      }

      dbListNames.erase(std::remove(dbListNames.begin(), dbListNames.end(), filenameNoExt), dbListNames.end());

      if (std::remove(filename.c_str()) == 0) {
        cout << "File deleted successfully!\n";

        std::string suffix = ".db";

        dbListNames.erase(std::remove(dbListNames.begin(), dbListNames.end(), filename), dbListNames.end());
      }
      else std::perror("Error deleting file");

    }
  }

};

//________________________________________________________________

int main() {

  App app;

  while( app.programRunning ) {
    string input;
    cin >> input;

    app.takeInput(input);
    app.checkQuit();
    app.checkCommand(input);
  
    cin.ignore();
    cin.get();

    newCmd();

  }

  return 0;

}
