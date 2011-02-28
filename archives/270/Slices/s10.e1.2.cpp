/******************************************************************************
 * CSCI 270 S10.E1 revision 2
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
#include <cctype>
#include <limits>
#include <fstream>

using namespace std;

#define NAME_SIZE 100
#define PHONE_SIZE 12
#define TOPIC_SIZE 255
#define MAX_SPEAKERS 10
#define DEFAULT_FILE "speakers.dat"

struct Speaker {
  char name[100], 
    phone[12], /* stored like 17122745423. No hypens */
    topic[255]; 
  double fee;
};

/*
 * clear_cin()
 * This doesn't work really well. It was *supposed* to clear everything
 * out of the cin buffer and reset any flags.
 */

void clear_cin()
{
  if (cin.eof())
    cin.clear();
  if (cin.fail())
    {
      cin.clear();
      cin.ignore(std::numeric_limits<std::streamsize>::max());
    }
}

/*
 * print_menu() - Print the root menu
 */

char print_menu()
{
  cin.clear();
  cout << "\n\n";
  cout << "A) Add a record" << endl
       << "D) Delete a record" << endl
       << "E) Edit a record" << endl
       << "P) Print a record" << endl
       << "L) List all records" << endl
       << "R) [Beta] Read records from file" << endl
       << "S) [Beta] Save records to file" << endl
       << "Q) Quit the program" << endl
       << endl 
       << "> ";

  char choice;
  cin >> choice;
  return toupper(choice);
}

/*
 * print_record(Speaker **, int record) - print out a given record
 */

void print_record(Speaker **sp, const int &num_records, int record)
{
  if (record < 0)
    {
      cout << "Which record would you like to view? > ";
      cin >> record;
      print_record(sp, num_records, record);
      return;
    }
  if (record >= num_records)
    {
      cout << "!!  No such record." << endl;
      return;
    }

  cout << "\nRecord #" << record << endl
       << "       Name:  " << sp[record]->name << endl
       << "       Phone: " << sp[record]->phone << endl
       << "       Topic: " << sp[record]->topic << endl
       << "       Fee:   $" << sp[record]->fee << endl;
}

void print_record(Speaker **sp);

/* 
 * add_record(Speaker **, int &num_speakers) - prompt and handle
 * adding a given record to the array. 
 */

void add_record(Speaker **sp, int &num_speakers)
{
  if (num_speakers >= MAX_SPEAKERS)
    return;

  cin.ignore();
  cin.clear();
  Speaker *s = new Speaker;

  cout << "\n\nNew Speaker:\n\n";

  cout << "Name: ";
  cin.getline(s->name,NAME_SIZE);
  clear_cin();

  cout << "Phone: "; 
  cin.getline(s->phone,PHONE_SIZE);
  clear_cin();

  cout << "Topic: ";
  cin.getline(s->topic,TOPIC_SIZE);
  clear_cin();

  cout << "Fee: $";
  cin >> s->fee;
  sp[num_speakers++] = s;
  
}

/*
 * delete_record(Speaker**, int &num_speakers) - delete a given record,
 * freeing its memory. Replace its "hole" in our NULL-terminated array
 * with the last item on the array. Decrement the num_speakers counter.
 */

void delete_record(Speaker **sp, int &num_speakers)
{
  clear_cin();
  cout << endl
       << "=== ====== ====== ===" << endl
       << "=== DELETE RECORD ===" << endl
       << "=== ====== ====== ===" << endl
       << endl;

  cout << "Record number to delete? (negative number to abort) > ";
  int record;
  cin >> record;
  if (record < 0) 
    return;
  
  if (sp[record] == NULL)
    return;

  num_speakers--;
  delete sp[record];
  sp[record] = sp[num_speakers];
  sp[num_speakers] = NULL;
  
}

/*
 * edit_record(Speaker **, int &num_speakers) - prompt for and edit
 * a given record.
 */

void edit_record(Speaker **sp, int &num_speakers)
{

  cout << endl
       << "=== ==== ====== ===" << endl
       << "=== EDIT RECORD ===" << endl
       << "=== ==== ====== ===" << endl
       << endl
       << "Which record would you like to edit? > ";
  unsigned int record;
  cin >> record;
  if (record >= num_speakers)
    {
      cout << "\n   !! No such record. " << endl;
      return;
    }

  print_record(sp, num_speakers, record);
  cout << "\nWhich attribute do you wish to edit?\n\n"
       << "N) Name" << endl
       << "P) Phone" << endl
       << "T) Topic" << endl
       << "F) Fee" << endl
       << endl
       << " > ";
  char choice;
  cin >> choice;

  switch (toupper(choice))
    {
    case 'N':
      cout << "Please enter new name: > ";
      cin.ignore();
      cin.getline(sp[record]->name, NAME_SIZE);
      break;
    case 'P':
      cout << "Please enter the new phone number: > ";
      cin >> sp[record]->phone;
      break;
    case 'T':
      cout << "Please enter the new topic: > ";
      cin.ignore();
      cin.getline(sp[record]->topic, TOPIC_SIZE);
      break;
    case 'F':
      cout << "Please enter the new fee: $ ";
      cin.ignore();
      cin >> sp[record]->fee;
      break;
    default:
      cout << "!! No such attribute.\n";
      break;
    }
}

/*
 * list_records(Speaker**, num_speakers) - pretty print out the 
 * complete list of speakers 
 */

void list_records(Speaker **sp, const int num_speakers)
{
  for (int i = 0; i < num_speakers; i++)
    {
      print_record(sp, num_speakers, i);
    }
}

void save_records(Speaker **sp, const int num_speakers)
{
  fstream file;
  file.open(DEFAULT_FILE, ios::binary | ios::out | ios::trunc);
  if (!file)
    {
      cout << "Failed to write out file.\n";
      return;
    }
  cout << "Writing";
  for (int i = 0; i < num_speakers; i++)
    {
      file.write((char *)(sp[i]), sizeof(Speaker));
      cout << ".";
    }
  cout << endl;
  cout << "Wrote " << num_speakers << " records.\n";
  file.close();
}

void read_records(Speaker **sp, int &num_speakers)
{
  /* purge existing (if any) records */

  for (int i = 0; i < num_speakers; i++)
     if (sp[i] != NULL)
       delete sp[i];

  // delete *sp; /* This dies for some reason?? */
  num_speakers = 0;

  fstream file;
  file.open(DEFAULT_FILE, ios::binary | ios::in);
  if (!file)
    { 
      cout << "Failed to read in file.\n";
      return;
    }

  Speaker *s = new Speaker;
  while (!file.eof())
    {
      file.read((char*)s, sizeof(Speaker));
      num_speakers++;
    }
  delete s;

  cout << "Reading in " << num_speakers << " records ";
  file.seekg(0);

  for (int i = 0; i < num_speakers; i++)
    {
      sp[i] = new Speaker;
      file.read((char*)sp[i], sizeof(Speaker));
    }

  file.close();
  
}


int main()
{
  char c;
  int max_speakers = MAX_SPEAKERS;
  int num_speakers = 0;
  Speaker **sp = new Speaker*[max_speakers];

  cout << "You chose " << c << endl;
  
  while ((c = print_menu()) != 'Q') {
    switch (c) {
    case 'A':
      add_record(sp, num_speakers);
      break;
    case 'D':
      delete_record(sp, num_speakers);
      break;
    case 'E':
      edit_record(sp, num_speakers);
      break;
    case 'P':
      print_record(sp, num_speakers, -1);
      break;
    case 'L':
      list_records(sp, num_speakers);
      break;
    case 'S':
      save_records(sp, num_speakers);
      break;
    case 'R':
      read_records(sp, num_speakers);
      break;
    }
  }
  cout << "\nGoodbye!\n";

  delete [] sp;
  return 0;
}

