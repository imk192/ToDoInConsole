#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <windows.h>
class ToDo
{
public:
	ToDo();
	void AddNote();
	void DeleteNote(int index);
	void PrintNote();
	void NoteStatus(int index);
	void InitNote();
	void DeleteAllNotes();
	void SaveAndQuit();
	void Err();
	void Warning();
	bool IsEmpty();
private:
	struct Notes
	{
		bool done = false;
		std::string note;
	};
	std::vector<Notes> notes;
	std::string path;
	std::fstream fs;
	friend std::ostream& operator<<(std::ostream& os, const ToDo::Notes& note);
	friend std::istream& operator>>(std::istream& is, ToDo::Notes& note);
};

ToDo::ToDo()
{
	notes = {};
	path = "ToDoBase.txt";
}

void ToDo::AddNote()
{
	std::cout << "Write a note: " << std::endl;
	std::string str;
	Notes note;
	SetConsoleCP(1251);
	getline(std::cin, str);
	getline(std::cin, str);
	note.note = str;
	SetConsoleCP(866);
	notes.push_back(note);
}

void ToDo::DeleteNote(int index)
{
	index -= 1;
	if (index > notes.size() - 1)
	{
		Warning();
		return;
	}
	notes.erase(notes.begin() + index);
}

void ToDo::PrintNote()
{
	std::string st = "";
	char ch;
	std::cout << "-------------------------------\n";
	for (size_t i = 0; i != notes.size(); ++i)
	{
		ch = notes[i].done ? 'v' : 'x';
		st = std::to_string(i + 1) + ".| ";
		std::cout << st << notes[i].note << "\t" << ch << std::endl;
	}
	std::cout << "-------------------------------\n";
}

void ToDo::NoteStatus(int index)
{
	index -= 1;
	if (index > notes.size() - 1)
	{
		Warning();
		return;
	}
	notes[index].done = !notes[index].done;
}

void ToDo::InitNote()
{
	fs.open(path, std::fstream::in | std::fstream::app);
	if (!fs.is_open())
	{
		Err();
		return;
	}
	else
	{
		while (!fs.eof())
		{
			std::string str = "";
			getline(fs, str);
			if (str != "")
			{
				Notes note;
				note.done = static_cast<int>(str.at(str.size() - 1)) - '0';
				str.erase(str.end() - 1);
				note.note = str;
				notes.push_back(note);
			}
		}
	}
	fs.close();
}

void ToDo::DeleteAllNotes()
{
	notes.clear();
}

void ToDo::SaveAndQuit()
{
	fs.open(path, std::fstream::out);
	if (!fs.is_open())
	{
		Err();
		return;
	}
	else
	{
		for (size_t i = 0; i != notes.size(); ++i)
		{
			if (i < notes.size() - 1)
			{
				fs << notes.at(i) << '\n';
			}
			else
			{
				fs << notes.at(i);
			}
		}
	}
	fs.close();
}

void ToDo::Err()
{
	std::cout << "Error" << std::endl;
}

void ToDo::Warning()
{
	std::cout << "Warning" << std::endl;
}

bool ToDo::IsEmpty()
{
	return notes.empty();
}

int main()
{
	setlocale(LC_ALL, "ru");
	ToDo note;
	note.InitNote();
	bool flag = true;
	while (flag)
	{
		if (note.IsEmpty())
		{
			std::cout << "The list is empty" << std::endl;
		}
		note.PrintNote();
		std::cout << "Select action:\n1. Add note\n2. Delete note\n3. Change note status\n4. Save and quit\n5. Delete all notes\n";
		char num;
		int index;
		std::cin >> num;
		switch (num)
		{
		case '1':
			note.AddNote();
			break;
		case '2':
			note.PrintNote();
			std::cout << "Enter the number of the note you want to delete: ";
			std::cin >> index;
			note.DeleteNote(index);
			break;
		case '3':
			note.PrintNote();
			std::cout << "Enter the number of the note you want to mark: ";
			std::cin >> index;
			note.NoteStatus(index);
			break;
		case '4':
			note.SaveAndQuit();
			flag = false;
			break;
		case '5':
			char accept;
			std::cout << "Are you sure you want to delete everything? Y/y: ";
			std::cin >> accept;
			if (accept == 'Y' || accept == 'y')
			{
				note.DeleteAllNotes();
			}
			break;
		default:
			note.Err();
			note.SaveAndQuit();
			flag = false;
			break;
		}
		system("cls");
	}
	return {};
}

std::ostream& operator<<(std::ostream& os, const ToDo::Notes& note)
{
	os << note.note << note.done;
	return os;
}

std::istream& operator>>(std::istream& is, ToDo::Notes& note)
{
	is >> note.note >> note.done;
	return is;
}
