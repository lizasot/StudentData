#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <time.h>
//#pragma warning(disable : 4996)
using namespace std;

struct Profile //анкета студента
{
	string fullName;
	char sex;
	unsigned short int group;
	unsigned short int numberList;
	int term[8];
	char depart[9];
	string date; // Дата внесения изменнеия в запись (post/update) 
};

//удаление лишних пробелов (для дат)
string DelSpaces(string s)
{
	for (size_t j = 0; j < s.length(); j++)
	{
		if (s[j] == ' ')
		{
			while (s[j + 1] == ' ') s.erase(j + 1, 1);
		}
	}
	if (s[0] == ' ') s.erase(0, 1);
	if (s[s.length() - 1] == ' ') s.erase(s.length() - 1, 1);
	return s;
}

//считывание с экрана информации о студенте
void newStudent()
{
	Profile Student;
	cin.clear();
	char trash;
	cout << "ФИО: ";
	cin >> trash;
	getline(cin, Student.fullName);
	Student.fullName = trash + Student.fullName;
	cout << "Группа: ";
	cin >> Student.group;
	cout << "Номер в списке: ";
	cin >> Student.numberList;
	cout << "Пол (F/M): ";
	cin >> Student.sex;
	cout << "Форма обучения (day/evening/distance): ";
	cin >> Student.depart;
	cout << "Оценки: ";
	bool temp = false; //имеются ли двойки?
	for (int i = 0; i < 8; i++)
	{
		cin >> Student.term[i];
		if (Student.term[i] == 2) { temp = true; } //да, двойка имеется
	}

	//запоминание даты считывания
	struct tm* loctime;
	time_t curtime;
	time(&curtime);
	loctime = localtime(&curtime);
	Student.date = asctime(loctime);
	Student.date = DelSpaces(Student.date);

	if (temp) { cout << '\n' << "Этот студент будет исключен. Профиль не будет сохранен в базе данных."; }
	else
	{
		/*Попытка создать файл с введёнными данными*/
		ofstream fout("students.txt", ios_base::app);
		if (!fout.is_open()) { cout << '\n' << "Ошибка сохранения!"; }
		else
		{
			/* == Вывод записи == */
			fout << Student.fullName << "\n";
			fout << Student.group << "\n";
			fout << Student.numberList << "\n";
			fout << Student.sex << "\n";
			fout << Student.depart << "\n";
			for (int i = 0; i < 8; i++)
			{
				fout << Student.term[i] << " ";
			}
			fout << "\n";
			fout << Student.date;
			fout.close();
		}
	}
}

// Функция посчёта количества студентов
int countStudents()
{
	ifstream fin("students.txt");
	if (fin.is_open())
	{
		int temp = 0; //количество строк
		string data;
		while (!fin.eof()) //пока указатель потока не достигнет конца файла
		{
			getline(fin, data); //считытвается строка
			temp++;
		}
		fin.close();
		int n;
		n = temp / 7; //количество строк поделить на кол-во строк одной анкеты студента = кол-во анкет студента
		return n;
	}
	else return 0;
}

//вывод файла всей информации о студентах
void outputStudents()
{
	ifstream fin("students.txt");
	if (!fin.is_open()) // если файл не открыт
		cout << "Файл не был открыт\n"; // сообщить об этом
	else
	{
		int temp;
		temp = countStudents();
		if (temp == 0)
			cout << "Файл пуст\n";
		else
		{
			string data; // буфер промежуточного хранения считываемого из файла текста
			while (!fin.eof())
			{
				getline(fin, data); // Считываем очередную строчку
				cout << data << '\n'; // Выводим строку на экран
			}
			fin.close();
		}
	}
}

//подсчёт количества студентов разных полов
void F_and_M()
{
	ifstream fin("students.txt");
	if (!fin.is_open()) { cout << "Ошибка!\n"; }
	else
	{
		int size;
		size = countStudents();
		if (size == 0) { cout << "База данных пуста.\n" << endl; }
		else
		{
			Profile* student = new Profile[size];
			char trash;
			for (int i = 0; i < size; i++) // Считываем данные всех студентов в массив структур
			{
				getline(fin, student[i].fullName, '\n');
				fin >> student[i].group >> student[i].numberList >> student[i].sex;
				fin >> student[i].depart;
				for (int j = 0; j < 8; j++) {
					fin >> student[i].term[j];
				}
				fin >> trash;
				getline(fin, student[i].date, '\n');
			}
			fin.close();

			//подсчёт и вывод
			int f = 0,
				m = 0;
			for (int i = 0; i < size; i++)
			{
				if (student[i].sex == 'F') { f++; }
				if (student[i].sex == 'M') { m++; }
			}
			cout << "\nСтудентов женского пола: " << f << "\n";
			cout << "Студентов мужского пола: " << m << "\n";
			//конец подсчёта и вывода

			delete[] student;
		}
	}
}

//Количество студентов со стипендией
void stipend()
{
	ifstream fin("students.txt");
	if (!fin.is_open()) { cout << "Ошибка!\n"; }
	else
	{
		int size;
		size = countStudents();
		if (size == 0) { cout << "База данных пуста.\n" << endl; }
		else
		{
			Profile* student = new Profile[size];
			char trash;
			bool three; //*наличие троек
			int s = 0; //*количество студентов со стипендией
			for (int i = 0; i < size; i++) // Считываем данные всех студентов в массив структур
			{
				getline(fin, student[i].fullName);
				fin >> student[i].group >> student[i].numberList >> student[i].sex;
				fin >> student[i].depart;
				three = false; //*по умолчанию троек нет
				for (int j = 0; j < 8; j++) {
					fin >> student[i].term[j];
					if (student[i].term[j] == 3) { three = true; } //*если встречена тройка, запоминаем это
				}
				fin >> trash;
				getline(fin, student[i].date);
				student[i].date = trash + student[i].date;
				if (!(three) && (student[i].depart[1] == 'a')) { s++; } //*подсчёт студентов со стипендией
			}
			fin.close();

			cout << "\nВсего студентов со стипендией: " << s << '\n'; //*вывод

			delete[] student;
		}
	}
}

//Вывод данных о студентах без стипендии
void notStipend()
{
	ifstream fin("students.txt");
	if (!fin.is_open()) { cout << "Ошибка!\n"; }
	else
	{
		int size;
		size = countStudents();
		if (size == 0) { cout << "База данных пуста.\n" << endl; }
		else
		{
			Profile* student = new Profile[size];
			char trash;
			bool check; //*true - надо вывести информацию. Степендия не может быть получена
			for (int i = 0; i < size; i++) // Считываем данные всех студентов в массив структур
			{
				check = false; //*по умолчанию false
				getline(fin, student[i].fullName);
				fin >> student[i].group >> student[i].numberList >> student[i].sex;
				fin >> student[i].depart;
				if (!(student[i].depart[1] == 'a')) { check = true; } //*если не дневное обучение - степендии нет
				for (int j = 0; j < 8; j++) {
					fin >> student[i].term[j];
					if (student[i].term[j] == 3) { check = true; } //*если встречена тройка, запоминаем это
				}
				fin >> trash;
				getline(fin, student[i].date);
				student[i].date = trash + student[i].date;

				//вывод
				if (check)
				{
					cout << student[i].fullName << "\n";
					cout << student[i].group << "\n";
					cout << student[i].numberList << "\n";
					cout << student[i].sex << "\n";
					cout << student[i].depart << "\n";
					for (int j = 0; j < 8; j++) { cout << student[i].term[j] << " "; }
					cout << "\n" << student[i].date << "\n";
				}
				//конец вывода
			}
			fin.close();

			delete[] student;
		}
	}
}

//Вывод данных о студентах-отличниках
void excellentTerm()
{
	ifstream fin("students.txt");
	if (!fin.is_open()) { cout << "Ошибка!\n"; }
	else
	{
		int size;
		size = countStudents();
		if (size == 0) { cout << "База данных пуста.\n" << endl; }
		else
		{
			Profile* student = new Profile[size];
			char trash;
			bool check; //*true - надо вывести информацию. Является отличником
			for (int i = 0; i < size; i++) // Считываем данные всех студентов в массив структур
			{
				check = true; //*по умолчанию true
				getline(fin, student[i].fullName);
				fin >> student[i].group >> student[i].numberList >> student[i].sex;
				fin >> student[i].depart;
				for (int j = 0; j < 8; j++) {
					fin >> student[i].term[j];
					if ((student[i].term[j] == 3) || (student[i].term[j] == 4)) { check = false; } //*если встречена 3 или 4, запоминаем это
				}
				fin >> trash;
				getline(fin, student[i].date);
				student[i].date = trash + student[i].date;

				//вывод
				if (check)
				{
					cout << student[i].fullName << "\n";
					cout << student[i].group << "\n";
					cout << student[i].numberList << "\n";
					cout << student[i].sex << "\n";
					cout << student[i].depart << "\n";
					for (int j = 0; j < 8; j++) { cout << student[i].term[j] << " "; }
					cout << "\n" << student[i].date << "\n";
				}
				//конец вывода
			}
			fin.close();

			delete[] student;
		}
	}
}

//Вывод данных о хорошистах
void B_GradeTerm()
{
	ifstream fin("students.txt");
	if (!fin.is_open()) { cout << "Ошибка!\n"; }
	else
	{
		int size;
		size = countStudents();
		if (size == 0) { cout << "База данных пуста.\n" << endl; }
		else
		{
			Profile* student = new Profile[size];
			char trash;
			bool check; //*true - надо вывести информацию. Является отличником
			for (int i = 0; i < size; i++) // Считываем данные всех студентов в массив структур
			{
				check = true; //*по умолчанию true
				getline(fin, student[i].fullName);
				fin >> student[i].group >> student[i].numberList >> student[i].sex;
				fin >> student[i].depart;
				for (int j = 0; j < 8; j++) {
					fin >> student[i].term[j];
					if (student[i].term[j] == 3) { check = false; } //*если встречена 3, запоминаем это
				}
				fin >> trash; //без него почему-то ломается
				getline(fin, student[i].date);
				student[i].date = trash + student[i].date;

				//вывод
				if (check)
				{
					cout << student[i].fullName << "\n";
					cout << student[i].group << "\n";
					cout << student[i].numberList << "\n";
					cout << student[i].sex << "\n";
					cout << student[i].depart << "\n";
					for (int j = 0; j < 8; j++) { cout << student[i].term[j] << " "; }
					cout << "\n" << student[i].date << "\n";
				}
				//конец вывода
			}
			fin.close();

			delete[] student;
		}
	}
}

//Вывод данных о студентах группы N
void groupN(unsigned short int n)
{
	ifstream fin("students.txt");
	if (!fin.is_open()) { cout << "Ошибка!\n"; }
	else
	{
		int size;
		size = countStudents();
		if (size == 0) { cout << "База данных пуста.\n" << endl; }
		else
		{
			Profile* student = new Profile[size];
			char trash;
			bool check; //*true - надо вывести информацию. Является студентом группы N
			int k = 0;
			for (int i = 0; i < size; i++) // Считываем данные всех студентов в массив структур
			{
				check = false; //*по умолчанию false
				getline(fin, student[i].fullName);
				fin >> student[i].group >> student[i].numberList >> student[i].sex;
				if (student[i].group == n) { check = true; }
				fin >> student[i].depart;
				for (int j = 0; j < 8; j++) { fin >> student[i].term[j]; }
				fin >> trash;
				getline(fin, student[i].date);
				student[i].date = trash + student[i].date;

				//вывод
				if (check)
				{
					cout << student[i].fullName << "\n";
					cout << student[i].group << "\n";
					cout << student[i].numberList << "\n";
					cout << student[i].sex << "\n";
					cout << student[i].depart << "\n";
					for (int j = 0; j < 8; j++) { cout << student[i].term[j] << " "; }
					cout << "\n" << student[i].date << "\n";
					k++;
				}
				//конец вывода
			}
			if (k == 0)
			{
				cout << "Таких студентов не было найдено\n";
			}
			fin.close();

			delete[] student;
		}
	}
}

//Вывод данных о студентах номера k
void numberListK(unsigned short int k)
{
	ifstream fin("students.txt");
	if (!fin.is_open()) { cout << "Ошибка!\n"; }
	else
	{
		int size;
		size = countStudents();
		if (size == 0) { cout << "База данных пуста.\n" << endl; }
		else
		{
			Profile* student = new Profile[size];
			char trash;
			bool check; //*true - надо вывести информацию. Является студентом номера k
			int n = 0;
			for (int i = 0; i < size; i++) // Считываем данные всех студентов в массив структур
			{
				check = false; //*по умолчанию false
				getline(fin, student[i].fullName);
				fin >> student[i].group >> student[i].numberList >> student[i].sex;
				if (student[i].numberList == k) { check = true; }
				fin >> student[i].depart;
				for (int j = 0; j < 8; j++) { fin >> student[i].term[j]; }
				fin >> trash;
				getline(fin, student[i].date);
				student[i].date = trash + student[i].date;

				//вывод
				if (check)
				{
					cout << student[i].fullName << "\n";
					cout << student[i].group << "\n";
					cout << student[i].numberList << "\n";
					cout << student[i].sex << "\n";
					cout << student[i].depart << "\n";
					for (int j = 0; j < 8; j++) { cout << student[i].term[j] << " "; }
					cout << "\n" << student[i].date << "\n";
					n++;
				}
				//конец вывода
			}
			if (n == 0)
			{
				cout << "Таких студентов не было найдено\n";
			}
			fin.close();

			delete[] student;
		}
	}
}

//Вывод данных до полудня
void tillNoon()
{
	ifstream fin("students.txt");
	if (!fin.is_open()) { cout << "Ошибка!\n"; }
	else
	{
		int size;
		size = countStudents();
		if (size == 0) { cout << "База данных пуста.\n" << endl; }
		else
		{
			Profile* student = new Profile[size];
			char trash;
			bool check; //*true - надо вывести информацию. Запись сделана до 12:00
			int k = 0;
			for (int i = 0; i < size; i++) // Считываем данные всех студентов в массив структур
			{
				check = false; //*по умолчанию false
				getline(fin, student[i].fullName);
				fin >> student[i].group >> student[i].numberList >> student[i].sex;
				fin >> student[i].depart;
				for (int j = 0; j < 8; j++) { fin >> student[i].term[j]; }
				fin >> trash;
				getline(fin, student[i].date);
				student[i].date = trash + student[i].date;

				//получаем время, когда была сделана запись
				string date1 = student[i].date;
				char date[9];
				for (int i = 0; i < 8; i++)
				{
					date[i] = date1[i + date1.size() - 13];
				}
				date[8] = '\0'; //получается время в формате hh:mm:ss

				int hh = (int)date[0] - (int)'0'; //преобразуем из string в int, вырезая ненужное и готовя для сравнения
				hh *= 10 + ((int)date[1] - (int)'0');
				if (hh < 12) { check = true; } //если запись сделана раньше 12 часов, выводим

				//вывод
				if (check)
				{
					cout << student[i].fullName << "\n";
					cout << student[i].group << "\n";
					cout << student[i].numberList << "\n";
					cout << student[i].sex << "\n";
					cout << student[i].depart << "\n";
					for (int j = 0; j < 8; j++) { cout << student[i].term[j] << " "; }
					cout << "\n" << student[i].date << "\n";
					k++;
				}
				//конец вывода
			}
			if (k == 0)
			{
				cout << "Таких записей не было найдено.\n";
			}
			fin.close();

			delete[] student;
		}
	}
}

//Вывод данных после полудня
void afterNoon()
{
	ifstream fin("students.txt");
	if (!fin.is_open()) { cout << "Ошибка!\n"; }
	else
	{
		int size;
		size = countStudents();
		if (size == 0) { cout << "База данных пуста.\n" << endl; }
		else
		{
			Profile* student = new Profile[size];
			char trash;
			bool check; //*true - надо вывести информацию. Запись сделана после 12:00
			int k = 0;
			for (int i = 0; i < size; i++) // Считываем данные всех студентов в массив структур
			{
				check = false; //*по умолчанию false
				getline(fin, student[i].fullName);
				fin >> student[i].group >> student[i].numberList >> student[i].sex;
				fin >> student[i].depart;
				for (int j = 0; j < 8; j++) { fin >> student[i].term[j]; }
				fin >> trash;
				getline(fin, student[i].date);
				student[i].date = trash + student[i].date;

				//получаем время, когда была сделана запись
				string date1 = student[i].date;
				char date[9];
				for (int i = 0; i < 8; i++)
				{
					date[i] = date1[i + date1.size() - 13];
				}
				date[8] = '\0'; //получается время в формате hh:mm:ss

				//часы
				int hh = (int)date[0] - (int)'0'; //преобразуем из string в int, вырезая ненужное и готовя для сравнения
				hh *= 10 + ((int)date[1] - (int)'0');
				//минуты
				int mm = (int)date[3] - (int)'0';
				mm *= 10 + ((int)date[4] - (int)'0');
				//секунды
				int ss = (int)date[6] - (int)'0';
				ss *= 10 + ((int)date[7] - (int)'0');

				if (hh >= 12)
				{
					if (hh == 12) { if ((mm != 0) || (ss != 0)) { check = true; } }
					else { check = true; }
				} //если запись сделана после 12 часов, выводим

				//вывод
				if (check)
				{
					cout << student[i].fullName << "\n";
					cout << student[i].group << "\n";
					cout << student[i].numberList << "\n";
					cout << student[i].sex << "\n";
					cout << student[i].depart << "\n";
					for (int j = 0; j < 8; j++) { cout << student[i].term[j] << " "; }
					cout << "\n" << student[i].date << "\n";
					k++;
				}
				//конец вывода
			}
			if (k == 0)
			{
				cout << "Таких записей не было найдено.\n";
			}
			fin.close();

			delete[] student;
		}
	}
}

void thatDate()
{
	cout << "\nВведите дату в формате mmm dd yyyy (например, Mar 6 2020): ";
	string Day;
	char trash;
	cin >> trash;
	getline(cin, Day);
	Day = trash + Day;
	ifstream fin("students.txt");
	if (!fin.is_open()) { cout << "Ошибка!\n"; }
	else
	{
		int size;
		size = countStudents();
		if (size == 0) { cout << "База данных пуста.\n" << endl; }
		else
		{
			Profile* student = new Profile[size];
			char trash;
			bool check; //*true - надо вывести информацию. Запись сделана в данный день
			int k = 0;
			for (int i = 0; i < size; i++) // Считываем данные всех студентов в массив структур
			{
				check = false; //*по умолчанию false
				getline(fin, student[i].fullName);
				fin >> student[i].group >> student[i].numberList >> student[i].sex;
				fin >> student[i].depart;
				for (int j = 0; j < 8; j++) { fin >> student[i].term[j]; }
				fin >> trash;
				getline(fin, student[i].date);
				student[i].date = trash + student[i].date;

				//сравниваем
				//24 символа, если дата с %dd, 23 если %d в полной дате
				//(Day).size(); 11 символов с %dd, 10 символов с %d
				//при совпадении разница размеров должна давать 13. Отсеиваем при несовпадении
				int daySize = (Day).size();
				if (((student[i].date).size() - daySize) == 13)
				{
					string date1 = student[i].date;
					bool same = true; //проверяем на совпадение месяцев и дней. По умолчанию true
					for (int i = 0; i < 6; i++)
					{
						if (Day[i] != date1[i + 4]) //у date1 надо пропустить первые 4 символа
						{
							same = false;
							break;
						}
					}
					for (int i = daySize - 1; i > 8; i--)
					{
						if (Day[i] != date1[i + 13])
						{
							same = false;
							break;
						}
					}
					if (same) //если месяца совпали
					{
						check = true;
					}
				}

				//вывод
				if (check)
				{
					cout << student[i].fullName << "\n";
					cout << student[i].group << "\n";
					cout << student[i].numberList << "\n";
					cout << student[i].sex << "\n";
					cout << student[i].depart << "\n";
					for (int j = 0; j < 8; j++) { cout << student[i].term[j] << " "; }
					cout << "\n" << student[i].date << "\n";
					k++;
				}
				//конец вывода
			}
			if (k == 0)
			{
				cout << "Таких записей не было найдено.\n";
			}
			fin.close();

			delete[] student;
		}
	}
}

void topTerm()
{
	ifstream fin("students.txt");
	if (!fin.is_open()) { cout << "Ошибка!\n"; }
	else
	{
		int size;
		size = countStudents();
		if (size == 0) { cout << "База данных пуста.\n" << endl; }
		else
		{
			Profile* student = new Profile[size];
			float* term = new float[size]; //*массив со средними оценками
			char trash;
			for (int i = 0; i < size; i++) // Считываем данные всех студентов в массив структур
			{
				getline(fin, student[i].fullName, '\n');
				fin >> student[i].group >> student[i].numberList >> student[i].sex;
				fin >> student[i].depart;
				term[i] = 0;
				for (int j = 0; j < 8; j++) {
					fin >> student[i].term[j];
					term[i] += (float)(student[i].term[j]);
				}
				term[i] /= 8;
				fin >> trash;
				getline(fin, student[i].date, '\n');
			}
			fin.close();

			//сортировка и вывод
			for (int i = 0; i < size; i++)
			{
				for (int j = 0; j < size - 1; j++)
				{
					if (term[j] < term[j + 1])
					{
						float m = term[j];
						term[j] = term[j + 1];
						term[j + 1] = m;

						string name = student[j].fullName;
						student[j].fullName = student[j + 1].fullName;
						student[j + 1].fullName = name;
					}
				}
			}

			//вывод
			for (int i = 0; i < size; i++)
			{
				cout << i + 1 << ". " << student[i].fullName << " = " << term[i] << "\n";
			}

			//конец подсчёта и вывода
			delete[] term;
			delete[] student;
		}
	}
}

void changeFile()
{
	ifstream fin("students.txt");
	if (!fin.is_open()) { cout << "Ошибка!\n"; }
	else
	{
		int size;
		size = countStudents();
		if (size == 0) { cout << "База данных пуста.\n" << endl; }
		else
		{
			Profile* student = new Profile[size];
			char trash;
			for (int i = 0; i < size; i++) // Считываем данные всех студентов в массив структур
			{
				getline(fin, student[i].fullName, '\n');
				fin >> student[i].group >> student[i].numberList >> student[i].sex;
				fin >> student[i].depart;
				for (int j = 0; j < 8; j++) {
					fin >> student[i].term[j];
				}
				fin >> trash;
				getline(fin, student[i].date, '\n');
				student[i].date = trash + student[i].date;
			}
			fin.close();

			//изменение информации
			for (int i = 0; i < size; i++)
			{
				cout << i << ". " << student[i].fullName << "\n";
			}
			cout << "Введите номер, под которым находится студент, информацию о котором Вы хотите изменить: ";
			int numbStud;
			cin >> numbStud;
			if (numbStud >= size) { cout << "Такой студент не был найден\n"; return; }
			cout << "\n1. " << student[numbStud].fullName << "\n";
			cout << "2. " << student[numbStud].group << "\n";
			cout << "3. " << student[numbStud].numberList << "\n";
			cout << "4. " << student[numbStud].sex << "\n";
			cout << "5. " << student[numbStud].depart << "\n6. ";
			for (int j = 0; j < 8; j++) { cout << student[numbStud].term[j] << " "; }
			cout << "\nВведите номер, под которым находится информацию, которую Вы хотите изменить: ";
			int sw;
			cin >> sw;

			bool check = false; //имеются ли изменения?
			bool temp = false; //имеются ли двойки?
			switch (sw)
			{
			case 1:
				cout << "\nВведите новое ФИО студента: ";
				cin >> trash;
				getline(cin, student[numbStud].fullName);
				student[numbStud].fullName = trash + student[numbStud].fullName;
				check = true;
				break;
			case 2:
				cout << "\nВведите новый номер группы студента: ";
				cin >> student[numbStud].group;
				check = true;
				break;
			case 3:
				cout << "\nВведите новый номер студента в списке: ";
				cin >> student[numbStud].numberList;
				check = true;
				break;
			case 4:
				cout << "\nВведите пол студента (F/M): ";
				cin >> student[numbStud].sex;
				check = true;
				break;
			case 5:
				cout << "\nВведите новый формат обучения студента (day/evening/distance): ";
				cin >> student[numbStud].depart;
				check = true;
				break;
			case 6:
				cout << "\nВведите оценки студента: ";
				for (int i = 0; i < 8; i++)
				{
					cin >> student[numbStud].term[i];
					if (student[numbStud].term[i] == 2) { temp = true; } //да, двойка имеется
				}
				if (temp) { cout << "У студента имеются неудовлетворительные оценки, поэтому он будет удалён.\n"; }
				check = true;
				break;
			default:
				cout << "\nВы ничего не выбрали. Производится выход из редактора.\n";
			}
			//изменения внесены

			//вводим в файл
			if (check)
			{
				/*Попытка создать файл с введёнными данными*/
				ofstream fout("students.txt");
				if (!fout.is_open()) { cout << '\n' << "Ошибка сохранения!\n"; }
				else
				{
					for (int i = 0; i < size; i++)
					{

						if (i == numbStud)
						{
							if (!temp) {
								/* == Вывод записи == */
								fout << student[i].fullName << "\n";
								fout << student[i].group << "\n";
								fout << student[i].numberList << "\n";
								fout << student[i].sex << "\n";
								fout << student[i].depart << "\n";
								for (int j = 0; j < 8; j++)
								{
									fout << student[i].term[j] << " ";
								}
								fout << "\n";

								//запоминание даты считывания
								struct tm* loctime;
								time_t curtime;
								string dateChange;
								time(&curtime);
								loctime = localtime(&curtime);
								dateChange = asctime(loctime);
								dateChange = DelSpaces(dateChange);

								fout << dateChange;
							}
							cout << "Информация была успешно изменена.\n";
						}
						else
						{
							/* == Вывод записи == */
							fout << student[i].fullName << "\n";
							fout << student[i].group << "\n";
							fout << student[i].numberList << "\n";
							fout << student[i].sex << "\n";
							fout << student[i].depart << "\n";
							for (int j = 0; j < 8; j++)
							{
								fout << student[i].term[j] << " ";
							}
							fout << "\n";
							fout << student[i].date;
							fout << "\n";
						}
					}

				}
				fout.close();
			}
			//конец вывода в файл
			delete[] student;
		}
	}
}

int main()
{
	setlocale(0, "");

	bool check = true; //выход из меню
	bool check1 = false; //выход из подменю
	bool outp = false;
	//false - заканчивает цикл, приводя непосредственно к выходу
	do {
		//system("cls");
		char sw = ' '; //переключатель главного меню
		char sw1 = ' '; //переключатель саб-меню
		cout << "\nВыберите нужный раздел: \n";
		cout << "\x1b[32m[1]\x1b[0m Создание новой записи о студенте\n";
		cout << "\x1b[32m[2]\x1b[0m Изменить существующую запись \n";
		cout << "\x1b[32m[3]\x1b[0m Отобразить все записи о студентах\n";
		cout << "\x1b[32m[4]\x1b[0m Вывести данные о студентах, выполняющих поставленные условия\n";
		cout << "\x1b[32m[5]\x1b[0m Вывести топ студентов по среднему баллу за сессию\n";
		cout << "\x1b[32m[6]\x1b[0m Вывести число студентов, выполняющих поставленные условия\n";
		cout << "\x1b[32m[7]\x1b[0m Вывести данные о студентах в зависимости от даты сделанной записи\n";
		cout << "\x1b[32m[8]\x1b[0m Очистить экран консоли\n";
		cout << "\x1b[32m[0]\x1b[0m Выйти в главное меню\n";
		cout << "Пожалуйста, введите число, чтобы выполнить нужное действие: ";

		cin >> sw;
		while (cin.get() != '\n') { sw = ' '; }; //если строка содержит более одного символа, возвращается ошибка

		switch (sw)
		{

		case '1': //[1] новая запись о студенте
			newStudent();
			break;

		case '2': //[2] изменение записей
			changeFile();
			break;


		case '3': //[3] вывод всех данных
			outputStudents();
			break;


		case '4': //[4] данные о студентах, выполняющих поставленные условия
			do {
				check1 = false;
				sw1 = ' ';
				cout << "\nВыберите, что хотите сделать: \n";
				cout << "\x1b[32m[1]\x1b[0m Вывод информации о студентах группы N\n";
				cout << "\x1b[32m[2]\x1b[0m Вывод информации о студентах, которые не получат стипендию\n";
				cout << "\x1b[32m[3]\x1b[0m Вывод информации о студентах, которые имеют только оценки \"4\" и \"5\"\n";
				cout << "\x1b[32m[4]\x1b[0m Вывод информации о студентах, которые имеют только оценки \"5\"\n";
				cout << "\x1b[32m[5]\x1b[0m Вывод информации о студентах с номером k в списке\n";
				cout << "\x1b[32m[0]\x1b[0m Вернуться назад\n";
				cout << "Пожалуйста, введите число, чтобы выполнить нужное действие: ";

				cin >> sw1;
				while (cin.get() != '\n') { sw1 = ' '; };

				switch (sw1)
				{
				case '1': //[1] вывод данных студентов группы N
					unsigned short int n;
					cout << "\nВведите номер группы: ";
					cin >> n;
					groupN(n);
					break;
				case '2': //[2] вывод данных студентов без стипендии
					notStipend();
					break;
				case '3': //[3] вывод данных студентов-хорошистов
					B_GradeTerm();
					break;
				case '4': //[4] вывод данных студентов-отличников
					excellentTerm();
					break;
				case '5': //[5] вывод данных студентов номера k
					unsigned short int k;
					cout << "\nВведите номер студента в списке: ";
					cin >> k;
					numberListK(k); //
					break;
				case '0': //[0] Назад
					break;
				default:
					cout << "Ошибка! Пожалуйста, попробуйте снова\n";
					check1 = true; //цикл пойдёт заново
					break;
				}
			} while (check1);
			break;


		case '5': //[5] вывод топа студентов по средней оценке
			topTerm();
			break;

		case '6': //[6] число студентов, выполняющих поставленные условия
			do {
				check1 = false;
				sw1 = ' ';
				cout << "\nВыберите, что хотите сделать: \n";
				cout << "\x1b[32m[1]\x1b[0m Количество студентов мужского и женского пола\n";
				cout << "\x1b[32m[2]\x1b[0m Количество студентов, получающих стипендию\n";
				cout << "\x1b[32m[0]\x1b[0m Вернуться назад\n";
				cout << "Пожалуйста, введите число, чтобы выполнить нужное действие: ";

				cin >> sw1;
				while (cin.get() != '\n') { sw1 = ' '; };

				switch (sw1)
				{
				case '1': //[1] количество F и M
					F_and_M();
					break;
				case '2': //[2] количество студентов со стипендией
					stipend();
					break;
				case '0': //[0] Назад
					break;
				default:
					cout << "Ошибка! Пожалуйста, попробуйте снова\n";
					check1 = true; //цикл пойдёт заново
					break;
				}
			} while (check1);
			break;

		case '7': //[7] данные о студентах в зависимости от даты
			do {
				check1 = false;
				sw1 = ' ';
				cout << "\nВыберите, что хотите сделать: \n";
				cout << "\x1b[32m[1]\x1b[0m Вывод записей, сделанных в указанный день\n";
				cout << "\x1b[32m[2]\x1b[0m Вывод записей, сделанных после полудня\n";
				cout << "\x1b[32m[3]\x1b[0m Вывод записей, сделанных до полудня\n";
				cout << "\x1b[32m[0]\x1b[0m Вернуться назад\n";
				cout << "Пожалуйста, введите число, чтобы выполнить нужное действие: ";

				cin >> sw1;
				while (cin.get() != '\n') { sw1 = ' '; };

				switch (sw1)
				{
				case '1': //[1] вывод всех записей, сделанных в этот день
					thatDate();
					break;
				case '2': //[2] записи после полудня
					afterNoon();
					break;
				case '3': //[2] записи до полудня
					tillNoon();
					break;
				case '0': //[0] Назад
					break;
				default:
					cout << "Ошибка! Пожалуйста, попробуйте снова\n";
					check1 = true; //цикл пойдёт заново
					break;
				}
			} while (check1);
			break;

		case '8': //[8] Очистить экран
			system("cls");
			break;

		case '0': //[0] Закрыть программу
			cout << "Выход из программы...\n";
			check = false; //выход из цикла
			break;
		default: //в случае, если введено что-то иное
			cout << "Ошибка! Пожалуйста, попробуйте снова\n";
			break;
		}
	} while (check);

	system("Pause");
	return 0;
}