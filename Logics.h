#pragma once
#include<iostream>
#include<algorithm>
#include<vector>
#include<conio.h>
#include<string>
#include<fstream>
#include<time.h>
#include<io.h>
#include<Windows.h>
using namespace std;

enum Type_Account {
	admin,teacher,student
};

class Account {
	string login, password;
	string FirstName;
	string LastName;
	string PatronName;
	string Telephone;
	string Secret_Question;
	string Answer;
	Type_Account type;
	long data;
	long activity;
	bool activ;
	
public:
	Account(string login, string password, string lastname, string firstname, string patronname,
		string telephone, string Secr_question, string answer, Type_Account type) {
		this->login = login;
		this->password = password;
		LastName = lastname;
		FirstName = firstname;
		PatronName = patronname;
		Telephone = telephone;
		this->type = type;
		Secret_Question = Secr_question;
		Answer = answer;
		data = time(0);
		activity = data;
		activ = true;
	}

	string Get_Log() {
		return login;
	}

	string Get_Pass() {
		return password;
	}

	string Get_LastName(){
		return LastName;
	}

	string Get_FirstName() {
		return FirstName;
	}

	string Get_PatronName() {
		return PatronName;
	}

	string Get_Telephone() {
		return Telephone;
	}
	Type_Account Get_Type() {
		return type;
	}

	void Show() {
		cout << login << endl;
		cout << LastName << " " << FirstName << " " << PatronName << endl;
		cout << Telephone << endl;
		cout << endl;
	}

	long Get_data() {
		//дата регистрации
		return data;
	}

	void Set_data(long t) {
		//используется только для записи из файла
		data = t;
	}

	long Get_Activity() {
		//время последнего входа в аккаунт
		return activity;
	}

	void Set_activity(long t) {
		//используется только при записи из файла
		activity = t;
	}
	string Get_Secret_Question() {
		//секретный вопрос
		return Secret_Question;
	}

	string Get_Answer() {
		//ответ на секретный вопрос
		return Answer;
	}

	bool Get_activ() {
		//активность аккаунта (блокирован/разблокирован)
		return activ;
	}

	void Set_activ(bool act) {
		activ = act;
	}

	void Set_pass(string pass) {
		password = pass;
	}
};


class Question {
	string question;//вопрос
	vector<string> Answer_options;//варианты ответа
	vector<string> Answers;//правильные ответы
	double value;//оценка за вопрос
public:
	Question(string question, vector<string> answer_options, vector<string> answers) {
		value = -1;
		this->question = question;
		auto it = answer_options.begin();
		while (it != answer_options.end()) {
			Answer_options.push_back(&(it->front()));
			it++;
		}
		it = answers.begin();
		while (it != answers.end()) {
			Answers.push_back(&(it->front()));
			it++;
		}
	}

	void Show() {
		cout << "Вопрос: " << question << endl;
		cout << "Варианты ответа: " << endl;
		int i = 1;
		for_each(Answer_options.begin(), Answer_options.end(), [&i](string str) {
			cout << i++ << ") " << str << endl;
		});
	}

	void Set_answer(string answers) {
		int count = 0;//количество правильных ответов в вопросе
		vector<string> answers_tmp;//временный вектор, хранящий правильные варианты ответа
		for_each(Answers.begin(), Answers.end(), [&answers_tmp, &count](string a) {
			answers_tmp.push_back(a);
			count++;
		});
		double price = 100 / (double)count;//цена одного правильного ответа
		string ans = answers;
		char tmp;
		int n;
		int right_answer = 0, all_answer = 0;
		string buf;
		vector<string>::iterator it;
		while (size(ans) != 0) {
			buf.clear();
			it = Answer_options.begin();
			tmp = ans[0];
			ans.erase(0, 2);
			n = atoi(&tmp);
			if (n) {
				//вариант ответа удалось преобразовать в тип int
				n--;
				buf = *(it + n);
				vector<string>::iterator it_tmp;
				it_tmp = find_if(answers_tmp.begin(),answers_tmp.end(), [buf](string e) {
					return buf == e;
				});
				if (it_tmp != answers_tmp.end()) {
					//указанный ответ есть в векторе правильных ответов
					right_answer++;
					answers_tmp.erase(it_tmp);//удаление правильного ответа из вектора правильных ответов
				}
				else {
					//указанного ответа нет в векторе правильных ответов
				}
			}
			else {
				//вариант ответа не удалось преобразовать в тип int
			}
			all_answer++;
		}
		value = price*right_answer;
		if (all_answer > right_answer) {
			//ответов было дано больше, чем есть правильных ответов
			while (all_answer>0 && value>0) {
				value -= price;
			}
		}
		answers_tmp.~vector();
	}

	double Get_value() {
		//получить оценку за вопрос
		return value;
	}

};

class Test {
	string category; //категория теста
	string name; //название теста
	//string log_teacher;
	string log_student;
	int rating;//оценка преподавателя
	double value;// процент правильных ответов
	int count;//количество попыток сдачи теста
	vector<Question> Questions;
public:
	Test(string category, string name, string path, string log) {
		log_student = log;
		//log_teacher += '0';
		const char c = '#';
		this->category = category;
		this->name = name;
		count = 0;
		rating = 0;
		value = 0;
		ifstream fin(path, ios::in);
		if (!fin) cout << "Ошибка открытия файла! Проверьте правильность указания пути. Тест не загружен" << endl;
		else {
			string question;
			string buf;
			vector<string> answer_option;
			vector<string> answers;
			while (!fin.eof()) {
				buf.clear();
				question.clear();
				answers.clear();
				answer_option.clear();
				getline(fin, question, '\n');//question
				getline(fin, buf, '\n');//1-st answer option
				answer_option.push_back(buf);
				buf.clear();
				getline(fin, buf, '\n');//2-nd answer option
				answer_option.push_back(buf);
				buf.clear();
				getline(fin, buf, '\n');//3-rd answer option
				answer_option.push_back(buf);
				buf.clear();
				getline(fin, buf, '\n');//4-th answer option
				answer_option.push_back(buf);
				buf.clear();
				while (fin.peek() != c) {
					getline(fin, buf, '\n');
					answers.push_back(buf);//1-st ... answers
					buf.clear();
				}
				getline(fin, buf, '\n');
				Questions.push_back(Question(question, answer_option, answers));
			}
			answer_option.~vector();//ПРОВЕРИТЬ ПАМЯТЬ
			answers.~vector();//ПРОВЕРИТЬ ПАМЯТЬ
		}
		fin.close();
	}

	void Show_all() {
		for_each(Questions.begin(), Questions.end(), [](Question tmp) {
			tmp.Show();
		});
	}

	double Get_rating() {
		int i = 0;
		double ret = 0;
		for_each(Questions.begin(), Questions.end(), [&i, &ret](Question a) {
			ret += a.Get_value();
			i++;
		});
		value = ret / i;
		return value;
	}

	void RunTest() {
		string buf;
		cout << "Введите все правильные варианты ответов, по вашему мнению, ЧЕРЕЗ ПРОБЕЛ!!!" << endl;
		cout << "После ввода всех вариантов нажмите Enter" << endl;
		auto it = Questions.begin();
		cin.ignore();
		while (it != Questions.end()) {
			buf.clear();
			it->Show();
			getline(cin, buf);
			//cin >> buf;
			it->Set_answer(buf);
			it++;
		}
	}
};

class Statistic {
	string name;//login

	vector<string> Tests;

	int *counts;//попыток сдачи
	
	int *value;//оценка, выставленная преподавателем
	
	double *rating;//оценка, поставленная автоматически после сдачи теста
	
	int kol;
public:

	Statistic(string log, string fullpath, double rating) {
		name = log;
		kol = 0;
		Tests.push_back(fullpath);
		counts = (int*)malloc((kol + 1) * sizeof(int));
		counts[kol] = 1;//первый раз сдает этот тест
		this->rating = (double*)malloc((kol + 1) * sizeof(double));
		this->rating[kol] = rating;
		value = (int*)malloc((kol + 1) * sizeof(int));
		value[kol] = 0;
		kol++;
	}

	Statistic(string log, vector<string> tests, int &mas_count, int &mas_value, double &mas_rating) {
		kol = 0;
		name = log;
		counts = nullptr;
		value = nullptr;
		rating = nullptr;
		kol = 0;
		auto it = tests.begin();
		while (it != tests.end()) {
			Tests.push_back(*(it));
			counts = (int*)realloc(counts, (kol + 1) * sizeof(int));
			*(counts + kol) = *(&mas_count + kol);
			value = (int*)realloc(value, (kol + 1) * sizeof(int));
			*(value + kol) = *(&mas_value + kol);
			rating = (double*)realloc(rating, (kol + 1) * sizeof(double));
			*(rating + kol) = *(&mas_rating + kol);
			kol++;
			it++;
		}
	}

	Statistic(const Statistic &obj) {
		this->kol = obj.kol;
		this->name = obj.name;
		auto it = obj.Tests.begin();
		while (it != obj.Tests.end()) {
			this->Tests.push_back(*(it));
			it++;
		}
		counts = (int*)malloc(kol * sizeof(int));
		value = (int*)malloc(kol * sizeof(int));
		rating = (double*)malloc(kol * sizeof(double));
		for (int i = 0; i < kol; i++) {
			counts[i] = obj.counts[i];
			value[i] = obj.value[i];
			rating[i] = obj.rating[i];
		}
		
	}

	string Get_log() {
		return name;
	}

	void Add_Test(string fullpath, double rating) {
		//добавить тест
		vector<string>::iterator it;
		int i = 0;
		it = find_if(Tests.begin(), Tests.end(), [&i, fullpath](string path) {
			if (fullpath == path) return true;
			else {
				i++;
				return false;
			}
		});
		if (it != Tests.end()) {
			//нашли тест в списке сдаваемых
			counts[i] += 1;
			this->rating[i] = rating;
		}
		else {
			//не нашли тест в списке сдаваемых
			Tests.push_back(fullpath);
			counts = (int*)realloc(counts, (kol + 1) * sizeof(int));
			counts[kol] = 1;
			this->rating = (double*)realloc(this->rating, (kol + 1) * sizeof(double));
			this->rating[kol] = rating;
			value = (int*)realloc(value, (kol + 1) * sizeof(int));
			value[kol] = 0;
			kol++;
		}
	}

	int Get_count(string fullpath) {
		//получить количество попыток сдачи теста
		int i = 0;
		vector<string>::iterator it = find_if(Tests.begin(), Tests.end(), [&i, fullpath](string tmp) {
			if (fullpath == tmp) return true;
			else {
				i++;
				return false;
			}
		});

		return (it == Tests.end() ? 0 : counts[i]);
	}

	int Get_value(string fullpath) {
		//получить оценку за тест
		int i = 0;
		auto it = find_if(Tests.begin(), Tests.end(), [&i, fullpath](string tmp) {
			if (fullpath == tmp) return true;
			else {
				i++;
				return false;
			}
		});

		return (it == Tests.end() ? 0 : value[i]);
	}

	void Set_value(string fullpath, int val){
		//выставить оценку за тест
		int i = 0;
		auto it = find_if(Tests.begin(), Tests.end(), [&](string str) {
			if (fullpath == str) {
				//тест есть в базе
				value[i] = val;
				return true;
			}
			else {
				i++;
				return false;
			}
		});

	}

	vector<string> &Get_Tests() {
		return Tests;
	}

	int *Get_counts() {
		return counts;
	}

	int *Get_value() {
		return value;
	}

	double*Get_rating() {
		return rating;
	}

	int Get_size() {
		//получение размера массивов
		return kol;
	}

	string Get_TestForVerification() {
		//получение теста на проверку (если есть непроверенные тесты)
		//если все тесты у студента проверенны, вернется пустая строка
		int i = 0;
		vector<string>::iterator it = Tests.begin();
		string buf = "";
		for (i; i < kol; i++) {
			if (value[i] == 0) {
				//если оценка за тест не выставлена
				buf = *(it + i);
				break;
			}
		}
		return buf;
	}
	
	double Get_RatingForTest(string fullpath) {
		//получение оценки, выставленной автоматически, за конкретный тест
		int i = 0;
		auto it = find_if(Tests.begin(), Tests.end(), [&i, &fullpath](string str) {
			if (fullpath == str) return true;
			else {
				i++;
				return false;
			}
		});
		return (it == Tests.end() ? 0 : rating[i]);
	}

	double Get_AllRating() {
		//получение рейтинговой оценки за все тесты
		double sum = 0;
		for (int i = 0; i < kol; i++)
			sum += rating[i];
		return (sum / kol);
	}
	double Get_AllValue() {
		//средний бал по тестам, в которых препод поставил оценку
		double sum = 0;
		int count = 0;
		for (int i = 0; i < kol; i++) {
			if (value[i] != 0) {
				sum += value[i];
				count++;
			}
		}
		return ((double)sum / count);
	}
	~Statistic() {
		free(counts);
		free(value);
		free(rating);	
	}
};

class Logics {
	int popitka = 3;//количество попыток сдать тест
	string path_account, path_tests;
	string path_acc = "Accounts.bin";
	string path_test = "*";
	
	string path_statistic = "DB\\stat.bin";
	string path_letters = "DB\\Letters.txt";
	vector<string> Letters;
	vector<Account> Accounts;
	vector<Statistic> Statistics;

public:
	Logics(string path_acc, string path_tests) {
		path_account = path_acc;
		this->path_tests = path_tests;
		Accounts.push_back(Account("admin", "admin", "admin", "admin", "admin",
			"admin", "admin?", "admin", admin)); //встроенная учетная запись администратора
	}

	void SaveAccounts() {
		ofstream fout(path_account+path_acc, ios::out);
		if (!fout) cout << "Ошибка открытия файла, данные не сохранены!" << endl;
		else {
		
			for_each(Accounts.begin()+1, Accounts.end(), [&fout](Account a) {
				
				fout << " " << a.Get_Log() << " " << a.Get_Pass() << " " << a.Get_LastName() << " " <<
					a.Get_FirstName() << " " << a.Get_PatronName() << " " << a.Get_Telephone() << " " <<
					a.Get_Type() << " " << a.Get_data() << " " << a.Get_Activity() << " " <<
					a.Get_Secret_Question() << " " << a.Get_Answer() << " " << a.Get_activ();
			});
		}
		fout.close();
	}

	void LoadAccounts() {
		
		ifstream fin(path_account+path_acc, ios::in);
		if (!fin) cout << "Ошибка открытия файла. Данные не загружены!" << endl;
		else {
			while (!fin.eof()) {
				if (fin.peek() == -1) break;
				string login, password, lastname, firstname, patronname, telephone,
					secr_question, answer;
				bool act;
				int type;
				long data, activity;
				fin >> login >> password >> lastname >> firstname >> patronname >> telephone >> type >>
					data >> activity >> secr_question >> answer >> act;
				Accounts.push_back(Account(login, password, lastname, firstname, patronname, 
					telephone, secr_question,answer, (Type_Account)type));
				auto it = Accounts.end() - 1;
				it->Set_data(data);
				it->Set_activity(activity);
			}
		}
		fin.close();
	}

	void SaveLetters() {
		ofstream fout(path_letters, ios::out);
		if (!fout) cout << "Ошибка открытия файла писем. Данные не сохранены" << endl;
		else {
			for_each(Letters.begin(), Letters.end(), [&fout](string t) {
				fout << t << "\n";
			});
		}
		fout.close();
	}

	void LoadLetters() {
		ifstream fin(path_letters, ios::in);
		if (!fin) cout << "Ошибка открытия файла писем. Данные не загружены!" << endl;
		else {
			string buf;
			while (!fin.eof()) {
				if (fin.peek() == -1) break;
				buf.clear();
				getline(fin, buf, '\n');
				Letters.push_back(buf);
			}
		}
		fin.close();
	}

	void Show_all() {
		for_each(Accounts.begin(), Accounts.end(), [](Account tmp) {
			tmp.Show();
		});
	}

	void LetsGo() {
		
		string tests = "";
		string buf;
		string lastname, firstname, patronname, sect_ques, answer, telephone;
		LoadAccounts();
		LoadStatistic();
		LoadLetters();
		string login, password;
		char action;
		int login_attempt = 0;
		const int attempt = 3;//количество попыток угадать пароль
		vector<Account>::iterator it_acc;
		do {
			system("cls");
			cout << "1. Вход" << endl;
			cout << "2. Регистрация" << endl;
			cout << "Esc - Выход" << endl;
			action = _getch();
			switch (action) {
			case '1':
				//ВХОД В АККАУНТ
				system("cls");	
				cout << "Login: ";
				cin >> login;
				cin.ignore();	
				cout << "Password: ";
				cin >> password;
				cin.ignore();
				it_acc=find_if(Accounts.begin(),Accounts.end(),
					[login](Account a) {
					return a.Get_Log() == login;
				});

				if (it_acc != Accounts.end()) {
					//совпал логин
					if (it_acc->Get_Pass() == password) {
						//совпал пароль
						login_to_account(it_acc);
					}
					else {
						//не совпал пароль
						login_attempt++;
						cout << "Пароль неверный!" << endl;
						if (login_attempt == attempt) {
							//исчерпано количество попыток угадать пароль
							cout<<"Вы несколько раз указали неверный пароль" << endl;
							cout << "Ответьте на секретный вопрос, для восстановления доступа к смоему аккаунту" << endl;
							while (login_attempt) {
								cout << "У Вас осталось " << login_attempt << " попыток" << endl;
								cout << "Вопрос: ";
								cout << it_acc->Get_Secret_Question() << endl;
								cout << "Ответ: ";
								cin >> buf;
								cin.ignore();
								if (it_acc->Get_Answer() == buf) {
									//правильно ответил на секретный вопрос
									break;
								}
								//неправильно ответил на секретный вопрос
								login_attempt--;
								cout << "Ответ неверный" << endl;
							}
							if (login_attempt) {
								//ответ на секретный вопрос правильный
								login_to_account(it_acc);
							}
							else {
								//исчерпано количество попыток, ответ на секр.вопрос неверный
								it_acc->Set_activ(false);
								cout << "Ваш аккаунт заблокирован! Обратитесь к администратору!" << endl;
							}
						}
					}
				}
				else {
					//не совпал логин
					login_attempt = 0;
					cout << "Логин и(или) пароль указан(ы) неверно!" << endl;
				}
				system("pause");
				break;
			case '2':
				//регистрация нового аккаунта
				do {
					system("cls");
					cout << "Логин: ";
					cin >> login;
					cin.ignore();
					it_acc = find_if(Accounts.begin(), Accounts.end(), [login](Account t) {
						return login == t.Get_Log();
					});
					if (it_acc != Accounts.end()) {
						//логин уже существует в базе
						cout << "Пользователь с таким логином уже зарегистрирован в базе" << endl;
						cout << "Укажите другой логин" << endl;
						system("pause");
					}
					else if (size(login) == 0) {
						//указали пустой логин
						cout << "Логин не может быть пустым" << endl;
						cout << "Укажите другой логин" << endl;
						system("pause");
					}	
				} while (it_acc != Accounts.end() || size(login)==0);
				cout << "Пароль: ";
				cin >> password;
				cin.ignore();
				cout << "Фамилия: ";
				cin >> lastname;
				cin.ignore();
				cout << "Имя: ";
				cin >> firstname;
				cin.ignore();
				cout << "Отчество: ";
				cin >> patronname;
				cin.ignore();
				cout << "Номер телефона: ";
				cin >> telephone;
				//секретный вопрос работает только с одним словом
				//в случае использования getline НЕОБХОДИМО переделать запись/чтение в файл
				cout << "Секретный вопрос (для восстановления аккаунта): ";
				cin >> sect_ques;
				cin.ignore();
				cout << "Ответ на секретный вопрос: ";
				cin >> answer;
				cin.ignore();
				Accounts.push_back(Account(login, password, lastname, firstname, patronname,
					telephone, sect_ques, answer, student));
				it_acc = Accounts.end() - 1;
				//после добавления нового аккаунта сразу делаю вход
				login_to_account(it_acc);
				break;
			}
		} while (action != 27);
		////////////////////////////////////////////////////////
		//СОХРАНИТЬ ВСЕ ИЗМЕНЕНИЯ ВО ВСЕХ ВЕКТОРАХ IN THIS PLACE
		SaveAccounts();
		SaveStatistic();
		SaveLetters();
	}

	void TestChecks() {
		//проверка теста(ов) и выставление оценки
		auto it = Statistics.begin();
		string buf;
		int r = 0;
		while (it != Statistics.end()) {
			buf.clear();
			buf=it->Get_TestForVerification();
			if (size(buf) != 0) {
				//в аккаунте есть непроверенные тесты
				cout << it->Get_log() << endl;
				cout << "Тест: " << buf << endl;
				cout << "Оценка, выставленная автоматически: " << it->Get_RatingForTest(buf) << endl;
				cout << "Ваша оценка: ";
				cin >> r;
				cin.ignore();
				it->Set_value(buf, r);
				char tmp;
				cout << "Проверить еще тест(ы)? Y/N: ";
				cin >> tmp;
				cin.ignore();
				if (tmp == 'n' || tmp == 'N') break;
			}
			else {
				it++;
			}
		}
		if (it == Statistics.end()) cout << "У Вас нет непроверенных тестов!" << endl;
	}

	void SaveStatistic() {
		//сохранение статистики в файл
		ofstream fout(path_statistic, ios::out);
		if (!fout) cout << "Ошибка открытия файла статистики. Данные не сохранены" << endl;
		else {
			auto it = Statistics.begin();
			vector<string> tmp;
			
			int i = 0;
			while (it!=Statistics.end()) {
				i = 0;
				fout << it->Get_log() << "\n";
				tmp.clear();
				tmp = it->Get_Tests();
				int *count = it->Get_counts();
				int *value = it->Get_value();
				double *rating = it->Get_rating();
				int razmer_mass = it->Get_size();
				fout << razmer_mass << "\n";
				auto it_tests = tmp.begin();
				while (it_tests != tmp.end()) {
					
					fout << *(it_tests) << " " << count[i] << " " << value[i] << " " << rating[i] << "\n";
					it_tests++;
					i++;
				}
				it++;
			}
		}
		fout.close();
	}

	void LoadStatistic() {
		//загрузка статистики из файла
		ifstream fin(path_statistic, ios::in);
		if (!fin) cout << "Ошибка загрузки файла статистики. Данные не загружены!" << endl;
		else {
			
			int kol = 0;
			int count, value;
			double rating;
			string log;
			string buf;
			int i;
			vector<string> tmp;
			
			while (!fin.eof()) {
				if (fin.peek() == -1) break;
				i = 0;
				log.clear();
				tmp.clear();
				fin >> log;
				fin >> kol;
				int *mas_count = (int*)malloc(kol * sizeof(int));
				int *mas_value = (int*)malloc(kol * sizeof(int));
				double *mas_rating = (double*)malloc(kol * sizeof(double));
				while (i < kol) {
					buf.clear();
					fin >> buf;
					tmp.push_back(buf);
					fin >> mas_count[i];
					fin >> mas_value[i];
					fin >> mas_rating[i];
					i++;
				}
				fin.get();
				Statistics.push_back(Statistic(log, tmp, *mas_count, *mas_value, *mas_rating));
				delete[]mas_count;
				delete[]mas_rating;
				delete[]mas_value;
			}
		}
		fin.close();
	}

	void login_to_account(vector<Account>::iterator it) {
		//произведен вход в аккаунт
		const int time_of_day = 86400;
		Type_Account type = it->Get_Type();
		char action;
		string buf;
		string path;
		_finddata_t *info = new _finddata_t;
		vector<string> category;
		vector<string> test;
		vector<string>::iterator it_tmp, it_tmp2;
		vector<Statistic>::iterator it_stat;
		long n;
		int s;
		int i = 1;
		int choice;
		int flag = 0;
		string login, password, firstname, lastname, patronname, telephone, sect_ques, answer;
		string thislogin;
		vector<Account>::iterator it_acc;
		char ch;
		switch (type) {
		case admin:
			//вошли как администратор
			
				if (it->Get_activ()) {
					//аккаунт не заблокирован
					thislogin.clear();
					thislogin = it->Get_Log();
					it->Set_activity(time(0));//время последнего входа в аккаунт

					do {

					system("cls");

					cout << "1. Добавить учетную запись администратора" << endl;
					cout << "2. Добавить учетную запись учителя" << endl;
					cout << "3. Добавить учетную запись студента" << endl;
					cout << "4. Удалить учетную запись" << endl;
					cout << "5. Отобразить все учетные записи" << endl;
					cout << "6. Просмотреть письма" << endl;
					cout << "7. Блокировать аккаунт" << endl;
					cout << "8. Разблокировать аккаунт" << endl;
					cout << "9. Очистить статистику" << endl;
					cout << "=. Показать активность аккаунтов" << endl;
					cout << "-. Изменить пароль" << endl;
					cout << "0. Выход" << endl;
					action = _getch();
					switch (action) {
					case '1':
						//регистрация нового аккаунта администратора
						system("cls");
						login.clear();
						password.clear();
						firstname.clear();
						lastname.clear();
						patronname.clear();
						telephone.clear();
						sect_ques.clear();
						answer.clear();
						do {
							system("cls");
							cout << "Логин: ";
							cin >> login;
							cin.ignore();
							it_acc = find_if(Accounts.begin(), Accounts.end(), [login](Account t) {
								return login == t.Get_Log();
							});
							if (it_acc != Accounts.end()) {
								//логин уже существует в базе
								cout << "Пользователь с таким логином уже зарегистрирован в базе" << endl;
								cout << "Укажите другой логин" << endl;
								system("pause");
							}
							else if (size(login) == 0) {
								//указали пустой логин
								cout << "Логин не может быть пустым" << endl;
								cout << "Укажите другой логин" << endl;
								system("pause");
							}
						} while (it_acc != Accounts.end() || size(login) == 0);
						cout << "Пароль: ";
						cin >> password;
						cin.ignore();
						cout << "Фамилия: ";
						cin >> lastname;
						cin.ignore();
						cout << "Имя: ";
						cin >> firstname;
						cin.ignore();
						cout << "Отчество: ";
						cin >> patronname;
						cin.ignore();
						cout << "Номер телефона: ";
						cin >> telephone;
						//секретный вопрос работает только с одним словом
						//в случае использования getline НЕОБХОДИМО переделать запись/чтение в файл
						cout << "Секретный вопрос (для восстановления аккаунта): ";
						cin >> sect_ques;
						cin.ignore();
						cout << "Ответ на секретный вопрос: ";
						cin >> answer;
						cin.ignore();
						Accounts.push_back(Account(login, password, lastname, firstname, patronname,
							telephone, sect_ques, answer, admin));
						cout << "Аккаунт добавлен" << endl;
						it = find_if(Accounts.begin(), Accounts.end(), [thislogin](Account tmp) {
							//эта функция здесь и ниже служит для того,
							//чтобы восстановить итератор на текущий аккаунт
							//после добавления,удаления и т.д. аккаунта(ов)
							//из вектора аккаунтов
							return thislogin == tmp.Get_Log();
						});
						
						system("pause");
						break;

					case '2':
						//создание учетной записи учителя
						system("cls");
						login.clear();
						password.clear();
						firstname.clear();
						lastname.clear();
						patronname.clear();
						telephone.clear();
						sect_ques.clear();
						answer.clear();
						do {
							system("cls");
							cout << "Логин: ";
							cin >> login;
							cin.ignore();
							it_acc = find_if(Accounts.begin(), Accounts.end(), [login](Account t) {
								return login == t.Get_Log();
							});
							if (it_acc != Accounts.end()) {
								//логин уже существует в базе
								cout << "Пользователь с таким логином уже зарегистрирован в базе" << endl;
								cout << "Укажите другой логин" << endl;
								system("pause");
							}
							else if (size(login) == 0) {
								//указали пустой логин
								cout << "Логин не может быть пустым" << endl;
								cout << "Укажите другой логин" << endl;
								system("pause");
							}
						} while (it_acc != Accounts.end() || size(login) == 0);
						cout << "Пароль: ";
						cin >> password;
						cin.ignore();
						cout << "Фамилия: ";
						cin >> lastname;
						cin.ignore();
						cout << "Имя: ";
						cin >> firstname;
						cin.ignore();
						cout << "Отчество: ";
						cin >> patronname;
						cin.ignore();
						cout << "Номер телефона: ";
						cin >> telephone;
						//секретный вопрос работает только с одним словом
						//в случае использования getline НЕОБХОДИМО переделать запись/чтение в файл
						cout << "Секретный вопрос (для восстановления аккаунта): ";
						cin >> sect_ques;
						cin.ignore();
						cout << "Ответ на секретный вопрос: ";
						cin >> answer;
						cin.ignore();
						Accounts.push_back(Account(login, password, lastname, firstname, patronname,
							telephone, sect_ques, answer, teacher));
						cout << "Аккаунт добавлен" << endl;
						it = find_if(Accounts.begin(), Accounts.end(), [thislogin](Account tmp) {
							return thislogin == tmp.Get_Log();
						});
						break;

					case '3':					
					//создание учетной записи студента
						system("cls");
						login.clear();
						password.clear();
						firstname.clear();
						lastname.clear();
						patronname.clear();
						telephone.clear();
						sect_ques.clear();
						answer.clear();
						do {
							system("cls");
							cout << "Логин: ";
							cin >> login;
							cin.ignore();
							it_acc = find_if(Accounts.begin(), Accounts.end(), [login](Account t) {
								return login == t.Get_Log();
							});
							if (it_acc != Accounts.end()) {
								//логин уже существует в базе
								cout << "Пользователь с таким логином уже зарегистрирован в базе" << endl;
								cout << "Укажите другой логин" << endl;
								system("pause");
							}
							else if (size(login) == 0) {
								//указали пустой логин
								cout << "Логин не может быть пустым" << endl;
								cout << "Укажите другой логин" << endl;
								system("pause");
							}
						} while (it_acc != Accounts.end() || size(login) == 0);
						cout << "Пароль: ";
						cin >> password;
						cin.ignore();
						cout << "Фамилия: ";
						cin >> lastname;
						cin.ignore();
						cout << "Имя: ";
						cin >> firstname;
						cin.ignore();
						cout << "Отчество: ";
						cin >> patronname;
						cin.ignore();
						cout << "Номер телефона: ";
						cin >> telephone;
						//секретный вопрос работает только с одним словом
						//в случае использования getline НЕОБХОДИМО переделать запись/чтение в файл
						cout << "Секретный вопрос (для восстановления аккаунта): ";
						cin >> sect_ques;
						cin.ignore();
						cout << "Ответ на секретный вопрос: ";
						cin >> answer;
						cin.ignore();
						Accounts.push_back(Account(login, password, lastname, firstname, patronname,
							telephone, sect_ques, answer, teacher));
						cout << "Аккаунт добавлен" << endl;
						it = find_if(Accounts.begin(), Accounts.end(), [thislogin](Account tmp) {
							return thislogin == tmp.Get_Log();
						});
						break;
					case '4':
						system("cls");
						buf.clear();
						cout << "Введите логин для удаления: ";
						cin >> buf;
						cin.ignore();
						it_acc = find_if(Accounts.begin(), Accounts.end(), [buf](Account a) {
							return buf == a.Get_Log();
						});
						if (it_acc != Accounts.end()) {
							//указанный аккаунт есть в базе
							if (it_acc != it) {
								Accounts.erase(it_acc);
								it = find_if(Accounts.begin(), Accounts.end(), [thislogin](Account a) {
									return thislogin == a.Get_Log();
								});
							}
							else cout << "Невозможно удалить текущий аккаунт" << endl;							
						}
						else {
							//указанного аккаунта нет в базе
							cout << "Указанный аккаунт не зарегистрирован в базе" << endl;
						}
						system("pause");
						break;
					case '5':
						system("cls");
						for_each(Accounts.begin(), Accounts.end(), [](Account a) {
							a.Show();
						});
						system("pause");
						break;
					case '6':
						system("cls");
						if (Letters.size()) {
							//письма есть в векторе
							char ch = 'y';
							while (ch != 'n' && ch != 'N' && Letters.size() > 0) {
								cout << *(Letters.end() - 1) << endl;
								Letters.pop_back();
								cout << "Еще одно письмо? Y/N ";
								cin >> ch;
								cin.ignore();
							}
						}
						else {
							//писем нет в векторе
							cout << "Корзина сообщений пуста" << endl;
						}
						system("pause");
						break;
					case '7':
						system("cls");
						buf.clear();
						cout << "Аккаунт для блокировки: ";
						cin >> buf;
						cin.ignore();
						it_acc = find_if(Accounts.begin(), Accounts.end(), [buf](Account c) {
							return buf == c.Get_Log();
						});
						if (it_acc != Accounts.end()) {
							//акк есть в базе
							if (it_acc != it) {
								//блокируемый аккаунт не совпадает с текущим
								if (it_acc->Get_activ()) {
									it_acc->Set_activ(false);
									cout << "Аккаунт успешно заблокирован!" << endl;
								}
								else cout << "Аккаунт был заблокирован ранее" << endl;
							}
							else {
								//попытка блокировки текущего аккаунта
								cout << "Невозможно заблокировать текущий аккаунт" << endl;
							}
						}
						else {
							//акк нет в базе
							cout << "Аккаунт не найден" << endl;
						}
						system("pause");
						break;
					case '8':
						system("cls");
						buf.clear();
						cout << "Аккаунт для разблокировки: ";
						cin >> buf;
						cin.ignore();
						it_acc = find_if(Accounts.begin(), Accounts.end(), [buf](Account c) {
							return buf == c.Get_Log();
						});
						if (it_acc != Accounts.end()) {
							//акк есть в базе						
							if (!it_acc->Get_activ()) {
								it_acc->Set_activ(true);
								cout << "Аккаунт успешно разблокирован" << endl;
							}
							else cout << "Аккаунт не является заблокированным" << endl;
						}
						else {
							//акк нет в базе
							cout << "Аккаунт не найден" << endl;
						}
						system("pause");
						break;
					case '9':
						//удаление статистики
						system("cls");
						Statistics.clear();
						cout << "Очищено" << endl;
						system("pause");
						break;
					case '=':
						system("cls");
						for_each(Accounts.begin(), Accounts.end(), [&](Account ac) {
							choice = 0;
							n = time(0);
							while (n - time_of_day > ac.Get_Activity()) {
								choice++;
								n -= time_of_day;
							}
							ac.Show();
							cout << "Последний вход: " << choice << " дней назад\n\n" << endl;
						});
						system("pause");
						break;
					case '-':
						system("cls");
						cout << "Введите старый пароль: ";
						cin >> buf;
						cin.ignore();
						if (buf == it->Get_Pass()) {
							//введенный пароль совпал с паролем аккаунта
							cout << "Введите новый пароль: ";
							buf.clear();
							cin >> buf;
							cin.ignore();
							it->Set_pass(buf);
							cout << "Пароль успешно изменен!" << endl;
						}
						else {
							//введенный пароль не совпал с паролем аккаунта
							cout << "Пароли не совпадают..." << endl;
						}
						system("pause");
						break;
					}
					} while (action != '0');
				}

				else {
					//аккаунт заблокирован
					cout << "Аккаунт заблокирован. Обратитесь к главному администратору" << endl;
					action = '0';
					system("pause");
				}
			
			break;
		case teacher:
			//вошли как учитель
			system("cls");
			do {
				if (it->Get_activ()) {
					//аккаунт не заблокирован
					path.clear();
					ifstream fin;
					ofstream fout;
					it->Set_activity(time(0));//установка времени последнего входа
					system("cls");
					cout << "1. Проверка тестов" << endl;
					cout << "2. Создание теста" << endl;
					cout << "3. Редактирование теста" << endl;
					cout << "4. Удаление теста" << endl;
					cout << "5. Письмо администратору" << endl;
					cout << "6. Изменить пароль" << endl;
					cout << "0. Выход" << endl;
					action = _getch();
					switch (action) {
					case '1':
						system("cls");
						TestChecks();
						system("pause");
						break;
					case '2':
						system("cls");
						path += path_tests;
						path += path_test;
						n = _findfirst(path.c_str(), info);
						flag = 0;
						i = 1;
						choice = 0;
						category.clear();
						if (n != -1) {
							s = n;
							while (s != -1) {
								if (_stricmp(info->name, ".") == 0 || _stricmp(info->name, "..") == 0) {
									s = _findnext(n, info);									
								}
								else {
									cout << i++ << ") " << info->name << endl;
									category.push_back(info->name);
									s = _findnext(n, info);
								}
								flag++;
							}
						}

						else if (n==-1 || flag==2) {
							cout << "Ни одной категории не обнаружено" << endl;
							break;
						}
						
						i--;
						do {
							cout << "Выберите категорию для создания теста: ";
							cin >> choice;
							cin.ignore();
						} while (choice<1 || choice>i);
						choice--;
						path.pop_back();
						it_tmp = category.begin() + choice;
						path += *(it_tmp);
						path += "\\";
						buf.clear();
						cout << "Название теста: ";
						cin >> buf;
						cin.ignore();
						path += buf;
						path += ".txt";
						fin.open ("DB\\CreateTest.txt",ios::in);
						buf.clear();
						while (!fin.eof()) {
							getline(fin, buf, '\n');
							cout << buf << endl;
							buf.clear();
						}
						fin.close();						
						do {
							cout << "Я подтверждаю что прочел руководство по созданию теста" << endl;
							cout << "И согласен с каждым его словом Y/N: ";
							cin >> ch;
						} while (ch != 'y' && ch != 'Y');
						system("cls");
						fout.open(path, ios::out);
						if (!fout) cout << "Ошибка. Тест не сохранен" << endl;
						else {
							buf.clear();
							cin.ignore();
							while (buf != "##") {
								cout << "Вопрос: ";
								getline(cin, buf);//вопрос
								fout << buf << "\n";
								buf.clear();
								cout << "1-й вариант ответа: ";
								getline(cin, buf);//1-й вар.ответа
								fout << buf << "\n";
								buf.clear();
								cout << "2-й вариант ответа: ";
								getline(cin, buf);//2-й вар.ответа
								fout << buf << "\n";
								buf.clear();
								cout << "3-й вариант ответа: ";
								getline(cin, buf);//3-й вар.ответа
								fout << buf << "\n";
								buf.clear();
								cout << "4-й вариант ответа: ";
								getline(cin, buf);//4-й вар.ответа
								fout << buf << "\n";
								buf.clear();
								cout << "Далее через Enter все правильные варианты ответа" << endl;
								while (buf != "#" && buf != "##") {
									getline(cin, buf);
									if (buf!="#" && buf!="##") fout << buf << "\n";
								}
								if (buf == "#") fout << '#' << "\n";
								else fout << "#";
							}
						}
						fout.close();
						system("pause");
						break;
					case '3':
						system("cls");
						path.clear();
						path += path_tests;
						path += path_test;
						n = _findfirst(path.c_str(), info);
						flag = 0;
						i = 1;
						choice = 0;
						category.clear();
						test.clear();
						if (n != -1) {
							s = n;
							cout << "Выбирайте категорию: " << endl;
							while (s != -1) {
								if (_stricmp(info->name, ".") == 0 || _stricmp(info->name, "..") == 0)
									s = _findnext(n, info);
								else {
									cout << i++ << ") " << info->name << endl;
									category.push_back(info->name);
									s = _findnext(n, info);
								}
								flag++;
							}
						}
						else if (n == -1 || flag == 2) {
							cout << "Нет подходящих категорий" << endl;
							break;
						}
						i--;
						do {
							cin >> choice;
							cin.ignore();
						} while (choice<1 && choice>i);
						choice--;
						path.pop_back();
						it_tmp = category.begin() + choice;
						path += *(it_tmp);
						path += "\\";
						buf.clear();
						path += path_test;
						n = _findfirst(path.c_str(), info);
						if (n != -1) {
							//в выбранной категории есть тесты
							s = n;
							i = 1;
							choice = 0;
							while (s != -1) {
								if (_stricmp(info->name, ".") == 0 || _stricmp(info->name, "..") == 0) {
									s = _findnext(n, info);
								}

								else {
									cout << i++ << ") " << info->name << endl;
									test.push_back(info->name);
									s = _findnext(n, info);
								}
								flag++;
							}
							i--;
							if (flag <= 2) {
								cout << "В выбранной категории нет тестов!" << endl;
								break;
							}
							do {
								cout << "Ваш выбор: ";
								cin >> choice;
								cin.ignore();
							} while (choice<1 || choice>i);

							choice--;
							it_tmp2 = test.begin() + choice;
							path.pop_back();
							path += *(it_tmp2);
						}
						
						cout << "Внимание! При редактировании теста старый вариант теста удаляется!!!" << endl;
						
						do {
							cout << "Я подтверждаю что осознанно собираюсь отредактировать выбранный тест Y/N" << endl;
							cin >> ch;
						} while (ch != 'y' && ch != 'Y' && ch != 'n' && ch != 'N');
						if (ch == 'y' || ch == 'Y') {
							
							fin.open("DB\\CreateTest.txt", ios::in);
							buf.clear();
							while (!fin.eof()) {
								getline(fin, buf, '\n');
								cout << buf << endl;
								buf.clear();
							}
							fin.close(); 
							fout.open(path, ios::out);
							if (!fout) cout << "Ошибка. Тест не сохранен" << endl;
							else {
								buf.clear();
								cin.ignore();
								while (buf != "##") {
									cout << "Вопрос: ";
									getline(cin, buf);//вопрос
									fout << buf << "\n";
									buf.clear();
									cout << "1-й вариант ответа: ";
									getline(cin, buf);//1-й вар.ответа
									fout << buf << "\n";
									buf.clear();
									cout << "2-й вариант ответа: ";
									getline(cin, buf);//2-й вар.ответа
									fout << buf << "\n";
									buf.clear();
									cout << "3-й вариант ответа: ";
									getline(cin, buf);//3-й вар.ответа
									fout << buf << "\n";
									buf.clear();
									cout << "4-й вариант ответа: ";
									getline(cin, buf);//4-й вар.ответа
									fout << buf << "\n";
									buf.clear();
									cout << "Далее через Enter все правильные варианты ответа" << endl;
									while (buf != "#" && buf != "##") {
										getline(cin, buf);
										if (buf != "#" && buf != "##") fout << buf << "\n";
									}
									if (buf == "#") fout << '#' << "\n";
									else fout << "#";
								}
							}
							fout.close();
						}
						else {
							cout << "Редактирование остановлено" << endl;
						}
							system("pause");
						break;
						case '4':
							system("cls");
							path.clear();
							path += path_tests;
							path += path_test;
							n = _findfirst(path.c_str(), info);
							flag = 0;
							i = 1;
							choice = 0;
							category.clear();
							test.clear();
							if (n != -1) {
								s = n;
								cout << "Выбирайте категорию: " << endl;
								while (s != -1) {
									if (_stricmp(info->name, ".") == 0 || _stricmp(info->name, "..") == 0)
										s = _findnext(n, info);
									else {
										cout << i++ << ") " << info->name << endl;
										category.push_back(info->name);
										s = _findnext(n, info);
									}
									flag++;
								}
							}
							else if (n == -1 || flag == 2) {
								cout << "Нет подходящих категорий" << endl;
								break;
							}
							i--;
							do {
								cin >> choice;
								cin.ignore();
							} while (choice<1 && choice>i);
							choice--;
							path.pop_back();
							it_tmp = category.begin() + choice;
							path += *(it_tmp);
							path += "\\";
							buf.clear();
							path += path_test;
							n = _findfirst(path.c_str(), info);
							if (n != -1) {
								//в выбранной категории есть тесты
								s = n;
								i = 1;
								choice = 0;
								while (s != -1) {
									if (_stricmp(info->name, ".") == 0 || _stricmp(info->name, "..") == 0) {
										s = _findnext(n, info);
									}

									else {
										cout << i++ << ") " << info->name << endl;
										test.push_back(info->name);
										s = _findnext(n, info);
									}
									flag++;
								}
								i--;
								if (flag <= 2) {
									cout << "В выбранной категории нет тестов!" << endl;
									break;
								}
								do {
									cout << "Ваш выбор: ";
									cin >> choice;
									cin.ignore();
								} while (choice<1 || choice>i);

								choice--;
								it_tmp2 = test.begin() + choice;
								path.pop_back();
								path += *(it_tmp2);
							}
							do {
								cout << "Вы действительно хотите удалить выбранный тест? Y/N: ";
								cin >> ch;
								cin.ignore();
							} while (ch != 'y' && ch != 'Y' && ch != 'n' && ch != 'N');
							
							if (ch == 'y' || ch == 'Y') {
								s = remove(path.c_str());
								if (!s) cout << "Тест успешно удален" << endl;
								else cout << "Ошибка удаления" << endl;
							}
							else cout << "Удаление прервано" << endl;
							system("pause");						
							break;
						case '5':
							buf.clear();
							system("cls");
							cout << "Введите текст письма: ";
							getline(cin, buf);
							Letters.push_back(it->Get_Log() + " " + buf);
							break;
						case '6':
							system("cls");
							cout << "Введите старый пароль: ";
							cin >> buf;
							cin.ignore();
							if (buf == it->Get_Pass()) {
								//введенный пароль совпал с паролем аккаунта
								cout << "Введите новый пароль: ";
								buf.clear();
								cin >> buf;
								cin.ignore();
								it->Set_pass(buf);
								cout << "Пароль успешно изменен!" << endl;
							}
							else {
								//введенный пароль не совпал с паролем аккаунта
								cout << "Пароли не совпадают..." << endl;
							}
							system("pause");
							break;
					}
				}
				else {
					//аккаунт заблокирован
					cout << "Аккаунт заблокирован. Обратитесь к администратору!" << endl;
					action = '0';
					system("pause");
				}
			} while (action != '0');
			
			break;
		case student:
			//вошли как студент
			system("cls");
			path.clear();
			path += path_tests;
			path += path_test;
			do {
				if (it->Get_activ()) {
					//если аккаунт не заблокирован
					i = 1;
					choice = 0;
					it->Set_activity(time(0));//установка времени после входа в аккаунт
					path = path_tests;
					path += path_test;
					system("cls");
					buf.clear();
					cout << "1. Пройти тест" << endl;
					cout << "2. Пересдача теста" << endl;
					cout << "3. Просмотр статистики" << endl;
					cout << "4. Изменить пароль" << endl;
					cout << "5. Письмо администратору" << endl;
					cout << "0. Выход из учетной записи" << endl;
					action = _getch();
					switch (action) {
						//реализация меню студента
					case '1':
					case '2':
						//прохождение теста
						//пересдача теста
						//сначала поиск по вектору пройденных тестов, вдруг студент уже сдавал этот тест
						system("cls");
						
						n = _findfirst(path.c_str(), info);
						
						if (n != -1) {
							//в папке с тестами есть категории
							category.clear();
							test.clear();
							s = n;
							cout << "Выбирайте категорию: " << endl;

							while (s != -1) {
								if (_stricmp(info->name, ".") == 0 || _stricmp(info->name, "..") == 0) {
									s = _findnext(n, info);
								}

								else {
									cout << i++ << ") " << info->name << endl;
									category.push_back(info->name);
									s = _findnext(n, info);
								}
								flag++;
							}
							i--;
							if (flag <= 2) {
								//эта проверка на случай, если в папке нет ни одной категории
								//(кроме скрытых папок)
								cout << "В папке с тестами нет подходящих категорий!" << endl;
								system("pause");
								break;
							}
							flag = 0;
							do {
								cout << "Ваш выбор: ";
								cin >> choice;
								cin.ignore();
							} while (choice<1 || choice>i);
							
							choice--;
							path.pop_back();
							it_tmp = category.begin() + choice;
							path += *(it_tmp);
							path += "\\";
							path += path_test;
							n = _findfirst(path.c_str(), info);
							if (n != -1) {
								//в выбранной категории есть тесты
								s = n;
								i = 1;
								choice = 0;
								while (s!=- 1) {
									if (_stricmp(info->name, ".") == 0 || _stricmp(info->name, "..") == 0) {
										s = _findnext(n, info);
									}

									else {
										cout << i++ << ") " << info->name << endl;
										test.push_back(info->name);
										s = _findnext(n, info);
									}
									flag++;
								}
								i--;
								if (flag <= 2) {
									cout << "В выбранной категории нет тестов!" << endl;
									system("pause");
									break;
								}
								do {
									cout << "Ваш выбор: ";
									cin >> choice;
								} while (choice<1 || choice>i);

								choice--;
								it_tmp2 = test.begin() + choice;
								path.pop_back();
								path += *(it_tmp2);

								it_stat = find_if(Statistics.begin(), Statistics.end(), [path,it](Statistic a) {
									//проверка существования логина в базе статистики
									return a.Get_log() == it->Get_Log();
								});

								Test obj(*(it_tmp), *(it_tmp2), path, it->Get_Log());

								if (it_stat == Statistics.end()) {
									//логин еще не сдавал ни один тест
									
									obj.RunTest();
									
									cout << "Вы сдали тест на: " << obj.Get_rating() << "%" << endl;
									Statistics.push_back(Statistic(it->Get_Log(), path, obj.Get_rating()));
									
									system("pause");
								}
								else {
									//логин уже сдавал хоть какой-то тест
									if (it_stat->Get_count(path) < popitka) {
										//еще не исчерпал лимит попыток сдачи выбранного теста
										obj.RunTest();

										cout << "Вы сдали тест на: " << obj.Get_rating() << "%" << endl;
										it_stat->Add_Test(path, obj.Get_rating());
										system("pause");
									}
									else {
										//студент исчерпал макс.кол-во попыток сдачи выбранного теста
										cout << "Превышен лимит попыток сдачи теста" << endl;
										system("pause");
									}
								}
							}
							else {
								//в выбранной категории нет тестов
								cout << "В выбранной категории нет ни одного теста" << endl;
							}
						}

						else {
							//в папке с тестами нет ни одной категории
							cout << "Ошибка! В папке с тестами нет ни одной категории" << endl;
						}
						
						break;
					
					case '3':
						//просмотр статистики
						system("cls");
						it_stat = find_if(Statistics.begin(), Statistics.end(), [&it](Statistic s) {
							return it->Get_Log() == s.Get_log();
						});
						if (it_stat != Statistics.end()) {
							//пользователь сдавал хоть 1 тест
							cout << "Средний балл по всем тестам, выставленный компьютером: ";
							cout << it_stat->Get_AllRating() << endl;
							cout << "Средний балл по всем тестам, выставленный преподавателем: ";
							cout << it_stat->Get_AllValue() << endl;
						}
						else {
							//пользователь еще не сдавал ни одного теста
							cout << "Статистика пуста" << endl;
						}
						system("pause");
						break;
					case '4':
						//изменить пароль
						system("cls");
						cout << "Введите старый пароль: ";
						cin >> buf;
						cin.ignore();
						if (buf == it->Get_Pass()) {
							//введенный пароль совпал с паролем аккаунта
							cout << "Введите новый пароль: ";
							buf.clear();
							cin >> buf;
							cin.ignore();
							it->Set_pass(buf);
							cout << "Пароль успешно изменен!" << endl;
						}
						else {
							//введенный пароль не совпал с паролем аккаунта
							cout << "Пароли не совпадают..." << endl;
						}
						system("pause");
						break;
					case '5':
						//письмо администратору
						buf.clear();
						system("cls");
						cout << "Введите текст письма: ";
						getline(cin, buf);
						Letters.push_back(it->Get_Log()+" "+buf);
						break;
					}
				}
				else {
					//аккаунт заблокирован
					cout << "Ваш аккаунт заблокирован. Обратитесь к администратору!" << endl;
					action = '0';
					system("pause");
				}

			} while (action != '0');
			
			break;
		}
		category.~vector();
		test.~vector();
		delete info;
	}
};
