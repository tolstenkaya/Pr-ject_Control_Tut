#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <chrono>
using namespace std;
using namespace std::chrono;
const int len = 12;
class Player
{
public:
	char letter;
	int attempt = 0;
	char* ch=nullptr;
	int num = 0;
	char* saveWord()
	{
		ch = (char*)realloc(ch, (num + 1) * sizeof(char));
		ch[num] = letter;
		num++;
		return ch;
	}
	char input()
	{
		cout << "\nВведите букву (на русском языке верхнего регистра): ";
		cin >> letter;
		ch=saveWord();
		attempt++;
		return letter;
	}
	void displayAllLetter()
	{
		for (int i = 0; i < num; i++)
		{
			cout << ch[i] << " ";
		}
	}
};
class Game: public Player
{
public:
	int n = 0;
	string word[len];
	string rand_word;
	int num_rand_word=0;
	int err = 0;
	int death = 6;
	int count_letter = 0;
	void RandWord()
	{
		num_rand_word = rand() % len;
		if (num_rand_word == 0 || num_rand_word > 12) { num_rand_word = 1; }
		ifstream in;
		in.open("data.txt");
		if (in.is_open())
		{
			for (int i = 0; i < len; i++)
			{
				while (getline(in, word[i]))
				{
					n++;
					if (n == num_rand_word)
					{
						rand_word = word[i];
					}
				}
			}
		}
		else
		{
			cout << "File opening error!\n";
		}
		in.close();
	}
	void Decoding()
	{
		string dec_word=rand_word;
		for (int i = 0; i < rand_word.length(); i++)
		{
			char ch = rand_word[i];
			if (rand_word[i] >= 'А' && rand_word[i] <= 'Я')
			{
				if (ch - 3 < 'А')
				{
					ch = 'Я' - (ch - 3 - 'А') - 1;
				}
				else
				{
					ch = ch - 3;
				}
			}
			if (rand_word[i] >= 'а' && rand_word[i] <= 'я')
			{
				if (ch - 3 < 'а')
				{
					ch = 'я' - (ch - 3 - 'а') - 1;
				}
				else
				{
					ch = ch - 3;
				}
			}
			dec_word[i] = ch;
		}
		rand_word = dec_word;
	}
	void Begin()
	{
		cout << "Cлово: ";
		for (int i = 0; i < rand_word.length(); i++)
		{
			cout << "__" << " ";
		}
		cout << "\nОсталось попыток: " << death << endl;
	}
	void Print()
	{
		cout << "Cлово: ";
		int pos = 0;
		int f = 0;
		for (int j = 0; j < rand_word.length(); j++)
		{
			for (int k = 0; k < n; k++)
			{
				if (ch[k] == rand_word[j])
				{
					pos = j;
					f = 1;
					/*cout << rand_word[j] << " ";*/
				}
			}
			if (pos==j && f==1)
			{
				cout << rand_word[j] << " ";
			}
			else
			{
				cout << "__" << " ";
			}
		}
		cout << "\n";
	}
	void Play()
	{
		int f;
		int key = -1;
		while(err<=6 || death>=0 || count_letter<=rand_word.length())
		{
			f = 0;
			input();
			for (int i = 0; i < rand_word.length(); i++)
			{
				if (rand_word[i]==letter)
				{
					count_letter++;
					f = 1;
					for (int j = 0; j < num; j++)
					{
						if (letter != ch[j])
						{
							key = 1;
							break;
						}
						if (letter == ch[j])
						{
							key = 0;
							break;
						}
					}
				}
			}
			if (key == 1) {
				cout << "Ураа, ты угадал! Играем дальше)\n"; key = -1;
			}
			if (key == 0) {
				cout << "Такая буква уже использовалась:) Внимательнее, друг!\n"; key = -1;
			}
			if (f == 0) {
				cout << "\nБуквы " << letter << " в искомом слове нет!\n";
				++err;
				cout << "Количество допустимых ошибок: " << --death << "\n";
			}
			cout << "Count letter: " << count_letter << endl;
			if (count_letter  == rand_word.length())break;
			if (err == 6)break;
			if (death == -1)break;
			Print();
		}/* while (err < 6 && death > 0 && count_letter < rand_word.length());*/
	}
	void Result()
	{
		/*system("cls");*/
		if (count_letter == rand_word.length()) { cout << "\n\nМоё уважение! Ты победил!\nИскомое слово:" <<rand_word<<"\n\n"; }
		else { cout << "\n\nТы проиграл в этой игре, но не в этой жизни ;)\n\n"; }
	}
};
int main()
{
	srand(time(0));
	setlocale(LC_ALL, "rus");
	system("chcp 1251");
	system("cls");
	Game g;
	auto start = high_resolution_clock::now();
	g.RandWord();
	g.Decoding();
	g.Begin();
	g.Play();
	g.Result();
	auto end = high_resolution_clock::now();
	auto duration = duration_cast<seconds>(end - start);
	cout << "\nКоличество потраченного времени: " << duration.count()<<" секунд\n";
	cout << "Количество попыток: " << g.attempt << "\n";
	cout << "Количество ошибок: " << g.err << "\n";
	cout << "Искомое слово: " << g.rand_word << "\n";
	cout << "Буквы игрока: ";
	g.displayAllLetter();
	cout << "\n";
}