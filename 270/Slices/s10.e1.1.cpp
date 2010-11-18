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
 * For this we're assuming that size is always at least 1 less
 * than capacity 
 */
void insert_record(Speaker &speaker, Speaker *ary, int size)
{
  ary[size+1] = speaker;

}

/*
 * Here we depend on size being correct
 */
void write_records(fstream &ofile, Speaker *speakers, int num_records)
{
  ofile.clear();
  ofile.seekp(0);
  ofile.write(reinterpret_cast<char *>(&num_records), sizeof(int));
  cout << "Writing records";
  for (int i = 0; i < num_records; i++) {
    ofile.write(reinterpret_cast<char *>(&speakers[i]), sizeof(speakers[i]));   
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

void print_record(Speaker *speakers, int record_num)
{
  cout << "Speaker data:\n"
       << "NAME:  " << speakers[record_num].name << endl
       << "PHONE: " << speakers[record_num].phone << endl
       << "TOPIC: " << speakers[record_num].topic << endl
       << "FEE:   " << speakers[record_num].fee << endl;
}

void print_records(Speaker *speakers)
{
  for (int i = 0; &speakers[i] != NULL; i++)
    print_record(speakers, i);
}

char print_menu()
{

  char choice;
  cout << "1) Add record" << endl
       << "2) Save records" << endl
       << "3) Read records" << endl
       << "4) Print a record" << endl
       << "5) EOF" << endl
       << endl << "> ";

  cin >> choice;

  return choice;
   
}

Speaker *read_records(fstream &file, int &num_records)
{
  file.clear();
  file.seekg(0);
  num_records = 0;
  int size = 0;
  file.read(reinterpret_cast<char *>(&num_records), sizeof(num_records));
  Speaker *speakers = new Speaker[num_records+10];
  if (file.eof())
    return speakers;

  Speaker s;
  cout << "Reading records";
  while (file.eof() != 1)
    {
      file.read(reinterpret_cast<char *>(&s), sizeof(s));
      insert_record(s, speakers, size-1);
      cout << '.';
      size++;
    }
  cout << endl;
  return speakers;
}

int report(fstream &file,int quiet=0)
{
  file.clear();
  file.seekg(0);
  int num_records;
  file.read(reinterpret_cast<char *>(&num_records), sizeof(int));
  cout << "INT SIZE: " << sizeof(int) << endl
       << "NUM RECS: " << num_records << endl;

  return num_records;
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
  
  Speaker *speakers = read_records(speakers_file, num_records);

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
      new_speaker();
      break;
    case '2':
      write_records(speakers_file, speakers, num_records);
      break;
    case '3':
      read_records(speakers_file, num_records);
      break;
    case '4':
      cout << "Which record? > ";
      cin >> option;
      print_record(speakers, option);
      break;
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
