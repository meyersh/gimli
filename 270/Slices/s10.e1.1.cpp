/******************************************************************************
 * CSCI 270 S10.E1 
 * Shaun Meyer
 * 
 * DESCR:
 * Programming Challenge #9 from Chapter 11 in C++ book.
 * 
 * "Speakers' Bureau
 *
 * Write a program that keeps track of a speakers' bureau. The program should
 * use a structure to store the following data about a speaker:
 *   - Name
 *   - Telephone number
 *   - Speaking Topicx
 *   - Fee Required
 *
 * This will be a stand-alone app.
 */

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#define NAME_SIZE 100
#define PHONE_SIZE 12
#define TOPIC_SIZE 255

struct Speaker {
  char name[100], 
    phone[12], /* stored like 17122745423. No hypens */
    topic[255]; 
  double fee;
};

/*
 * Here we depend on size being correct
 */
void write_records(fstream &ofile, vector<Speaker> *speakers, int num_records)
{
  ofile.clear();
  ofile.seekp(0);
  cout << "Writing records";
  for (int i = 0; i < speakers->size(); i++) {
    ofile.write(reinterpret_cast<char *>(&((*speakers)[i])), sizeof((*speakers)[i]));   
    cout << '.';
  }
  cout << endl;

}

Speaker new_speaker()
{
  Speaker s;
  cin.ignore();
  cout << "Creating new speaker.\n";
  cout << "Speakers name? ";
  cin.clear();
  cin.getline(s.name, NAME_SIZE);
  cout << "Speakers phone? (17122745423) ";
  cin.getline(s.phone, PHONE_SIZE);
  cout << "Speakers topic? ";
  cin.getline(s.topic, TOPIC_SIZE);
  cout << "Speakers fee? $";
  cin >> s.fee;
  return s;
}

void print_record(vector<Speaker> *speakers, int record_num)
{
  Speaker *record = &((*speakers)[record_num]);
  cout << "Speaker data:\n"
       << "NAME:  " << record->name << endl
       << "PHONE: " << record->phone << endl
       << "TOPIC: " << record->topic << endl
       << "FEE:   " << record->fee << endl;
}

void print_records(vector<Speaker> *speakers)
{
  for (int i = 0; i < speakers->size(); i++)
    {
      cout << "Record " << i << endl;
      print_record(speakers, i);
    }
}

char print_menu()
{

  char choice;
  cout << "1) Add record" << endl
       << "2) Save records" << endl
       << "3) Read records" << endl
       << "4) Print a record" << endl
       << "5) Print all records" << endl
       << "6) Delete a record" << endl
       << endl << "> ";

  cin >> choice;

  return choice;
   
}

vector<Speaker> *read_records(fstream &file)
{
  file.clear();
  file.seekg(0);
  vector<Speaker> *speakers = new vector<Speaker>;
  if (file.eof())
    return speakers;

  Speaker new_speaker;
  cout << "Reading records";
  while (file.eof() != 1)
    {
      file.read(reinterpret_cast<char *>(&new_speaker), sizeof(Speaker));
      speakers->push_back(new_speaker);
      cout << '.';
    }
  cout << endl;
  return speakers;
}

int report(fstream &file,int quiet=0)
{
  return 1;
}

int main(int argc, char **argv)
{
  const char *default_file_path = "speakers.dat";
  
  fstream speakers_file;
  
  if (argc == 2) {
    /* they specified a path on the command-line */
    speakers_file.open(argv[1], ios::in | ios::out | ios::binary);
  } else {
    speakers_file.open(default_file_path, ios::in | ios::out | ios::binary);
  }
  
  if (speakers_file.fail()) {
    cout << "Failed to open the speakers file.\n" ;
    return -1;
  }

  int num_records = 0;
  
  vector<Speaker> *speakers = read_records(speakers_file);

  report(speakers_file);

  Speaker Shaun = {"Shaun Meyer", "17128992559", "Quantum Entagling", 2500.20};
  //insert_record(Shaun, speakers, num_records-1);
  //num_records++;

  //speakers_file.clear();

  // re-wind the file to over-write it:

  char choice = print_menu();
  int option;
  while (choice != 'Q') {
    option = 0;
    switch (choice) {
    case '1':
      speakers->push_back(new_speaker());
      break;
    case '2':
      write_records(speakers_file, speakers, num_records);
      break;
    case '3':
      read_records(speakers_file);
      break;
    case '4':
      cout << "Which record? > ";
      cin >> option;
      print_record(speakers, option);
      break;
    case '5':
      print_records(speakers);
      break;
    case '6':
      cout << "Which record? > ";
      cin >> option;
      speakers->erase(speakers->begin()+option);
    default:
      choice = print_menu();
      break;
    }

  choice = print_menu();
  }

  write_records(speakers_file, speakers, num_records);

  speakers_file.close();
  delete speakers;
  speakers = NULL;

  return 0;
}
