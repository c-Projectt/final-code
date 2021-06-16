#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<windows.h>
#include<string.h>

int year = 0;
struct grades {
	int math;
	int electronics;
	int circuits;
	int programming;
	int lang;
	int totalMarks;
};
struct grades degree;         //the total scores of all students

struct studentNode {
	int BN;
	char id[15];
	char fullName[30];
	struct grades studentgrades;
	struct studentNode* next;
	struct studentNode* nextsucstudent;
};
struct studentNode* head = NULL;

struct data {
	int Eyear;
	struct studentNode all;
	struct data* next;
};
struct data* dhead = NULL;

void start();
void menu();
int check(char i[]);                     // check if the ID is already exist ir not
void checkrepeat(int, int);              // ask the user after finishing a task if he want to perform another task or exit
void gotoxy(int, int);                   // adjust the display of the program
int stringsz(char s[]);                  // determine the size of a string
int is_BN(char s[]);                     // convert a string to an integer
void recase(char cs[]);                  // capitalize the first letter of every name entered and if there are any capital letters within the name switch them to lowercase
void sortAlph(struct studentNode*);      // sort student names alphabetically
void sortBN();                           // sort the students BN
int gradecheck(int);                     // if the grade entered is greater than 100 return 100, if it is less than 0 return 0, else return the grade entered
void add();                              // add a new student record
void delete_record(int bn, char q);      // delete a student record
void search();                           // search for a record and ask if the user want to modify it 
void modify(int bn);                     // modify  a record ( ID, name, grades, all data)
void addgrades(int bn);                  // update the student's grades
void suc();                    // show the average mark of each course, the number of passsed students and determine the first student and his grades
void display();                // display all student records
void clear();                  // delete all student records
void quit(int, int);           // first, ask the user if he want to save the changes or discard and then end the program

void sortAlph(struct studentNode* newNode);
int main() {
	gotoxy(20, 10);
	printf("<< Students Database Management System >>");
	gotoxy(40, 15);
	printf("Press any key to continue");
	_getch();               // pauses the Output Console untill a key is pressed & is defined in conio.h header file
	start();
	return 0;
}
void start() {
	char choice[15];
	system("cls");          // to clear the output screen & is defined in stdlib.h header file
	gotoxy(10, 5);
	printf("<< Electrical Engineering Department >>");
	gotoxy(20, 8);
	printf("1 : 1st Year Electronics and Communications");
	gotoxy(20, 9);
	printf("2 : 2nd Year Electronics and Communications");
	gotoxy(20, 10);
	printf("3 : 3rd Year Electronics and Communications");
	gotoxy(20, 11);
	printf("4 : 3rd Year Computer Engineering");
	gotoxy(20, 12);
	printf("5 : 4th Year Electronics and Communications");
	gotoxy(20, 13);
	printf("6 : 4th Year Computer Engineering");
	gotoxy(10, 16);
	printf("Enter your choice: ");
	scanf("%14s", choice);
	if (is_BN(choice) >= 1 && is_BN(choice) <= 6) {
		year = is_BN(choice);
		FILE* fptr;
		if ((fptr = fopen("data.txt", "r+")) == NULL) {
			gotoxy(25, 18);
			puts("File could not be opened.");
		}
		else {
			struct data* dprevious = dhead;
			while (!feof(fptr)) {
				struct data* newNode = malloc(sizeof(struct data));
				newNode->next = NULL;

				fscanf(fptr, "%d %d %s %d %d %d %d %d %d %19[^\n]%*1[\n]", &newNode->Eyear, &newNode->all.BN, newNode->all.id,
					&newNode->all.studentgrades.math, &newNode->all.studentgrades.electronics, &newNode->all.studentgrades.circuits,
					&newNode->all.studentgrades.programming, &newNode->all.studentgrades.lang, &newNode->all.studentgrades.totalMarks, newNode->all.fullName);

				if (feof(fptr)) break;

				if (dhead == NULL) {
					dhead = newNode;
					dprevious = dhead;
				}
				else {
					dprevious->next = newNode;
					dprevious = dprevious->next;
				}
			}

			struct data* dcurrent = dhead;
			dprevious = dhead;
			struct studentNode* previous = head;
			while (dcurrent != NULL) {
				if (dcurrent->Eyear == year) {
					struct studentNode* newNode = malloc(sizeof(struct studentNode));
					newNode->BN = dcurrent->all.BN;
					newNode->studentgrades.math = dcurrent->all.studentgrades.math;
					newNode->studentgrades.electronics = dcurrent->all.studentgrades.electronics;
					newNode->studentgrades.circuits = dcurrent->all.studentgrades.circuits;
					newNode->studentgrades.programming = dcurrent->all.studentgrades.programming;
					newNode->studentgrades.lang = dcurrent->all.studentgrades.lang;
					newNode->studentgrades.totalMarks = dcurrent->all.studentgrades.totalMarks;
					strcpy(newNode->fullName, dcurrent->all.fullName);
					strcpy(newNode->id, dcurrent->all.id);
					newNode->next = NULL;

					if (dcurrent->all.BN == 1) {
						head = newNode;
						previous = head;
					}
					else {
						previous->next = newNode;
						previous = previous->next;
					}

					if (dprevious == dhead) {
						dhead = dhead->next;
						dcurrent = dhead;
						free(dprevious);
						dprevious = dhead;
					}
					else {
						dprevious->next = dcurrent->next;
						free(dcurrent);
						dcurrent = dprevious->next;
					}
				}
				else {
					dprevious = dcurrent;
					dcurrent = dcurrent->next;
				}
			}
		}
		fclose(fptr);
		menu();
	}
	else start();
}
void menu() {
	char choice[15];
	system("cls");          // to clear the output screen & is defined in stdlib.h header file
	gotoxy(5, 2);
	puts("<< MENU >>");
	gotoxy(10, 4);
	puts("Choose a number to perform the task you want");
	gotoxy(15, 6);
	puts("1 : Add Record");
	gotoxy(15, 7);
	puts("2 : Delete Record");
	gotoxy(15, 8);
	puts("3 : Search and modify");
	gotoxy(15, 9);
	puts("4 : Display all students records");
	gotoxy(15, 10);
	puts("5 : show status");
	gotoxy(15, 11);
	puts("6 : Clear");
	gotoxy(15, 12);
	puts("7 : Exit");
	gotoxy(15, 13);
	puts("8 : Start Menu");
	gotoxy(10, 15);
	printf("Enter your choice : ");
	scanf("%14s", choice);
	if (is_BN(choice) >= 1 && is_BN(choice) <= 8) {
		switch (is_BN(choice)) {
		case 1:
			add();
			break;
		case 2:
			delete_record(0, 's');
			break;
		case 3:
			search();
			break;
		case 4:
			display();
			break;
		case 5:
			suc();
			break;
		case 6:
			clear();
			break;
		case 7:
			quit(15, 17);
			break;
		case 8:
			quit(15, 17);
			start();
			break;
		}
	}
	else menu();
}
int check(char d[]) {
	int i;
	struct studentNode* checkptr = head;
	if (head == NULL) return 3;
	else {
		while (checkptr != NULL) {
			if (checkptr->id[0] == d[0]) {
				for (i = 1; d[i] != '\0'; ++i) {
					if (checkptr->id[i] != d[i]) break;
				}
				if (i == stringsz(checkptr->id) && i == stringsz(d)) return 2;
			}
			checkptr = checkptr->next;
		}
		if (checkptr == NULL) return 3;
	}
}
void checkrepeat(int x, int y) {
	gotoxy(x, y);
	int c;
	printf("Enter 1 to perform another task or 0 to exit: ");
	scanf("%d", &c);
	if (c == 1) menu();
	else quit(x + 5, y + 2);
}
int gradecheck(int k) {
	if (k > 100) return 100;
	else if (k < 0) return 0;
	else return k;
}
void gotoxy(int x, int y) {                // is defined in windows.h header file
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
int stringsz(char s[]) {
	int i;
	for (i = 0; s[i] != '\0'; i++);
	return i;
}
int is_BN(char s[]) {
	int x = atoi(s);
	return x;
}
void recase(char cs[30]) {
	int i, j;
	if (cs[0] >= 97) cs[0] -= 32;
	for (int i = 1; cs[i] != NULL; i++) {
		if (cs[i] < 97 && cs[i] != ' ') cs[i] += 32;
		else if (cs[i] == ' ' && (cs[i + 1] != NULL && cs[i + 1] >= 97)) cs[i + 1] -= 32, i++;
	}
}
void sortBN() {
	struct studentNode* current = head;
	int b = 1;
	while (current != NULL) {
		current->BN = b;
		b++;
		current = current->next;
	}
}
void sortAlph(struct studentNode* newNode) {
	int i = 1, j = 0;
	struct studentNode* current = head, * previous = NULL;

	while (current != NULL && newNode->fullName[0] >= current->fullName[0]) {
		if (newNode->fullName[0] == current->fullName[0]) {
			// to compare every character for the first name of each node and will close when they are not equal
			for (i = 1; (newNode->fullName[i] == current->fullName[i]) && (newNode->fullName[i] != '\0') && (current->fullName[i] != '\0'); i++);

			//if the program end the for because they reached end of string and all character are equal, it will enter that if condition
			if ((stringsz(newNode->fullName) == i) && (stringsz(current->fullName) == i))  break;
			else if (newNode->fullName[i] == '\0')  break;
			// the same to that of the last name if it break through the because the two characters arent equal so it will break through the while loop if the last name of new node is less than that of current node
			else if (current->fullName[i] > newNode->fullName[i])  break;
		}
		previous = current;
		current = current->next;
	}

	if (previous == NULL) {
		newNode->next = head;
		head = newNode;
	}
	else {
		previous->next = newNode;
		newNode->next = current;
	}
	sortBN();
}

void add() {
	int ch;
	char fullName[30], l, id[15];
	gotoxy(25, 17);
	printf("Enter the student Name: ");
	l = getchar();
	gets(fullName);
	recase(fullName);
	gotoxy(25, 18);
	printf("Enter the student ID: ");
	scanf("%14s", id);
	if (check(id) == 2) {
		gotoxy(10, 20);
		puts("The student with this ID is already exist.");
		checkrepeat(25, 22);
	}
	else {
		struct studentNode* newNode = malloc(sizeof(struct studentNode));
		if (newNode != NULL) {
			newNode->studentgrades.lang = 0;
			newNode->studentgrades.programming = 0;
			newNode->studentgrades.circuits = 0;
			newNode->studentgrades.electronics = 0;
			newNode->studentgrades.math = 0;
			newNode->studentgrades.totalMarks = 0;
			newNode->BN = 1;
			strcpy(newNode->id, id);
			strcpy(newNode->fullName, fullName);
			newNode->next = NULL;
			gotoxy(25, 19);
			printf("Enter 1 to add student grades or 0 to continue without adding: ");
			scanf("%d", &ch);
			int x = 20;
			if (ch == 1) {
				gotoxy(25, 20);
				printf("Enter Mathematics grade: ");
				scanf("%d", &newNode->studentgrades.math);
				newNode->studentgrades.math = gradecheck(newNode->studentgrades.math);
				gotoxy(25, 21);
				printf("Enter Electronics grade: ");
				scanf("%d", &newNode->studentgrades.electronics);
				newNode->studentgrades.electronics = gradecheck(newNode->studentgrades.electronics);
				gotoxy(25, 22);
				printf("Enter Programming grade: ");
				scanf("%d", &newNode->studentgrades.programming);
				newNode->studentgrades.programming = gradecheck(newNode->studentgrades.programming);
				gotoxy(25, 23);
				printf("Enter Circuits grade: ");
				scanf("%d", &newNode->studentgrades.circuits);
				newNode->studentgrades.circuits = gradecheck(newNode->studentgrades.circuits);
				gotoxy(25, 24);
				printf("Enter Language grade: ");
				scanf("%d", &newNode->studentgrades.lang);
				newNode->studentgrades.lang = gradecheck(newNode->studentgrades.lang);
				newNode->studentgrades.totalMarks = newNode->studentgrades.math + newNode->studentgrades.electronics + newNode->studentgrades.programming + newNode->studentgrades.circuits + newNode->studentgrades.lang;
				x = 25;
			}

			if (head == NULL) head = newNode;
			else sortAlph(newNode);

			gotoxy(10, x + 2);
			puts("The record is added successfully.");
			checkrepeat(25, x + 4);
		}
		else {
			gotoxy(10, 21);
			puts("student is not inserted. No memory available.");
		}
	}
}
void delete_record(int bn, char q) {
	char full[30], l;
	if (!bn) {
		gotoxy(15, 17);
		printf("Enter student Name or BN: ");
		l = getchar();
		gets(full);
		recase(full);
		struct studentNode* current = head;
		bn = is_BN(full);
	}

	struct studentNode* current = head, * previous = head;
	if (head == NULL) {
		gotoxy(10, 19);
		puts("Record does not exist.");
	}
	else if (bn == 0) {
		while (current != NULL) {
			if (full[0] == current->fullName[0]) {
				int i;
				for (i = 1; full[i] != 0; i++) {
					if (full[i] != current->fullName[i]) break;
				}
				if (i == stringsz(full)) break;
			}
			previous = current;
			current = current->next;
		}
	}
	else {
		while (current != NULL && current->BN != bn) {
			previous = current;
			current = current->next;
		}
	}


	if (current == NULL) {
		gotoxy(10, 19);
		puts("Record does not exist.");
	}
	else if (current->BN == 1) {
		current = head;
		head = head->next;
		free(current);
		gotoxy(10, 19);
		if (q == 's') printf("Record is deleted successfully.");
		sortBN();
	}
	else {
		previous->next = current->next;
		free(current);
		gotoxy(10, 19);
		if (q == 's') printf("Record is deleted successfully.");
		sortBN();
	}
	if (q == 's') checkrepeat(25, 21);
}
void search() {
	int bn;
	char full[30], l;
	struct studentNode* current = head;
	if (head == NULL) {
		gotoxy(15, 17);
		printf("System is empty.");
		checkrepeat(25, 19);
	}
	else {
		gotoxy(15, 17);
		printf("Enter student Name or BN: ");
		l = getchar();
		gets(full);
		recase(full);
		bn = is_BN(full);
		if (bn) {
			while (current != NULL && current->BN != bn)  current = current->next;
		}
		else {
			while (current != NULL) {
				if (full[0] == current->fullName[0]) {
					int i;
					for (i = 1; full[i] != 0; i++) {
						if (full[i] != current->fullName[i]) break;
					}
					if (i == stringsz(full)) break;
				}
				current = current->next;
			}
		}
		int it;
		if (current != NULL) {
			gotoxy(3, 19);
			printf("%-3s %-20s %-15s %-13s %-13s %-13s %-13s %-13s %s\n", "BN", "Name", "ID", "Electronics", "Circuits", "Programming", "Mathematics", "Language", "TOTAL");
			gotoxy(3, 21);
			printf("%-3d %-20s %-15s %-13d %-13d %-13d %-13d %-13d %d\n", current->BN, current->fullName, current->id, current->studentgrades.electronics, current->studentgrades.circuits, current->studentgrades.programming, current->studentgrades.math, current->studentgrades.lang, current->studentgrades.totalMarks);
			gotoxy(15, 24);
			printf("<< Choose a number >>");
			gotoxy(20, 26);
			printf("1 : Modify record data");
			gotoxy(20, 27);
			printf("2 : Menu");
			gotoxy(15, 29);
			printf("Enter your choice: ");
			scanf("%d", &it);
			if (it == 1) modify(current->BN);
			else menu();
		}
		else {
			gotoxy(10, 19);
			printf("Student not found.");
			checkrepeat(25, 21);
		}
	}
}
void modify(int bn) {
	int mr = 0;
	char id[15];
	struct studentNode* current = head;
	while (current != NULL && current->BN != bn) current = current->next;
	gotoxy(15, 32);
	printf("<< Choose a number >>");
	gotoxy(25, 34);
	printf("1 : Modify ID");
	gotoxy(25, 35);
	printf("2 : Modify Name");
	gotoxy(25, 36);
	printf("3 : Modify Grades");
	gotoxy(25, 37);
	printf("4 : Modify all data");
	gotoxy(15, 39);
	printf("Enter your choice: ");
	scanf("%d", &mr);
	if (mr == 1) {
		gotoxy(25, 41);
		printf("Enter new ID: ");
		scanf("%s", id);
		if (check(id) == 2) {
			gotoxy(20, 43);
			puts("The student with this ID is already exist.");
			checkrepeat(25, 45);
		}
		else {
			strcpy(current->id, id);
			gotoxy(20, 43);
			printf("Record modified successfully.");
			checkrepeat(25, 45);
		}
	}
	else if (mr == 2) {
		char full[30], l;
		gotoxy(25, 41);
		printf("Enter new Name: ");
		l = getchar();
		gets(full);
		strcpy(current->fullName, full);
		struct studentNode* sr = head;
		if (bn != 1) {
			while (sr->BN != bn - 1) sr = sr->next;
			sr->next = current->next;
			sortAlph(current);
			gotoxy(20, 43);
			printf("Record modified successfully.");
			checkrepeat(25, 45);
		}
		else {
			head = current->next;
			sortAlph(current);
			gotoxy(20, 43);
			printf("Record modified successfully.");
			checkrepeat(25, 45);
		}
	}
	else if (mr == 3) {
		addgrades(bn);
		checkrepeat(25, 49);
	}
	else if (mr == 4) {
		char fullName[30], l, id[15];
		gotoxy(25, 41);
		printf("Enter the student name: ");
		l = getchar();
		gets(fullName);
		recase(fullName);
		gotoxy(25, 42);
		printf("Enter the student ID: ");
		scanf("%14s", id);
		if (check(id) == 2) {
			gotoxy(20, 44);
			puts("The student with this ID is already exist.");
			checkrepeat(25, 46);
		}
		else {
			strcpy(current->id, id);
			strcpy(current->fullName, fullName);
			gotoxy(25, 43);
			printf("Enter Mathematics grade: ");
			scanf("%d", &current->studentgrades.math);
			gotoxy(25, 44);
			printf("Enter Electronics grade: ");
			scanf("%d", &current->studentgrades.electronics);
			gotoxy(25, 45);
			printf("Enter Programming grade: ");
			scanf("%d", &current->studentgrades.programming);
			gotoxy(25, 46);
			printf("Enter Circuits grade: ");
			scanf("%d", &current->studentgrades.circuits);
			gotoxy(25, 47);
			printf("Enter Language grade: ");
			scanf("%d", &current->studentgrades.lang);
			current->studentgrades.totalMarks = current->studentgrades.math + current->studentgrades.electronics + current->studentgrades.programming + current->studentgrades.circuits + current->studentgrades.lang;
			struct studentNode* sr = head;
			if (bn != 1) {
				while (sr->BN != bn - 1) sr = sr->next;
				sr->next = current->next;
				sortAlph(current);
				gotoxy(20, 49);
				printf("Record modified successfully.");
				checkrepeat(25, 51);
			}
			else {
				head = current->next;
				sortAlph(current);
				gotoxy(20, 49);
				printf("Record modified successfully.");
				checkrepeat(25, 51);
			}
		}
	}
}
void addgrades(int bn) {
	struct studentNode* current = head;
	while (current != NULL && current->BN != bn) current = current->next;
	gotoxy(25, 41);
	printf("Enter Mathematics grade: ");
	scanf("%d", &current->studentgrades.math);
	current->studentgrades.math = gradecheck(current->studentgrades.math);
	gotoxy(25, 42);
	printf("Enter Electronics grade: ");
	scanf("%d", &current->studentgrades.electronics);
	current->studentgrades.electronics = gradecheck(current->studentgrades.electronics);
	gotoxy(25, 43);
	printf("Enter Programming grade: ");
	scanf("%d", &current->studentgrades.programming);
	current->studentgrades.programming = gradecheck(current->studentgrades.programming);
	gotoxy(25, 44);
	printf("Enter Circuits grade: ");
	scanf("%d", &current->studentgrades.circuits);
	current->studentgrades.circuits = gradecheck(current->studentgrades.circuits);
	gotoxy(25, 45);
	printf("Enter Language grade: ");
	scanf("%d", &current->studentgrades.lang);
	current->studentgrades.lang = gradecheck(current->studentgrades.lang);
	current->studentgrades.totalMarks = current->studentgrades.math + current->studentgrades.electronics + current->studentgrades.programming + current->studentgrades.circuits + current->studentgrades.lang;
	gotoxy(20, 47);
	printf("Grades are added successfully.");
}
void suc() {
	int sum = 0, bn;
	if (head == NULL) {
		gotoxy(15, 17);
		puts("The system is empty.");
		checkrepeat(25, 19);
		return;
	}
	struct studentNode* print = head;
	struct studentNode* max = head;        //pointer to the first student
	struct studentNode* begin = NULL;
	struct studentNode* end = NULL;       //head of the passed student
	degree.electronics = 0;
	degree.circuits = 0;
	degree.programming = 0;
	degree.math = 0;
	degree.lang = 0;
	degree.totalMarks = 0;

	while (print != NULL) {
		print->nextsucstudent = NULL;
		int flag = 0;

		degree.electronics += print->studentgrades.electronics;
		degree.circuits += print->studentgrades.circuits;
		degree.programming += print->studentgrades.programming;
		degree.math += print->studentgrades.math;
		degree.lang += print->studentgrades.lang;
		degree.totalMarks += print->studentgrades.totalMarks;
		if (max->studentgrades.totalMarks < print->studentgrades.totalMarks && print->studentgrades.totalMarks >= 250) max = print;   //the first student

		if (print->studentgrades.electronics < 50) flag++;
		if (print->studentgrades.circuits < 50) flag++;
		if (print->studentgrades.programming < 50) flag++;
		if (print->studentgrades.math < 50) flag++;
		if (print->studentgrades.lang < 50) flag++;

		if (flag <= 2) {  //the student passed
			sum++;
			if (begin == NULL) {
				begin = end = print;
				begin->nextsucstudent = NULL;
			}
			else {
				end->nextsucstudent = print;
				end = print;
				end->nextsucstudent = NULL;
			}
		}
		bn = print->BN;
		print = print->next;
	}

	gotoxy(37, 18);
	printf("<< STATUS OF ALL STUDENT RECORDS >>");
	gotoxy(5, 21);
	printf("Average of students degrees: \n");
	gotoxy(10, 23);
	printf("%-15s %-15s %-15s %-15s %-15s %s\n", "Electronics", "Circuits", "Programming", "Mathematics", "Language", "TOTAL");
	gotoxy(10, 24);
	printf("%-15.2f %-15.2f %-15.2f %-15.2f %-15.2f %.2f \n", ((double)degree.electronics / bn), ((double)degree.circuits / bn), ((double)degree.programming / bn), ((double)degree.math / bn), ((double)degree.lang / bn), ((double)degree.totalMarks / bn));
	gotoxy(5, 28);
	printf("The first student: \n");
	gotoxy(3, 30);
	printf("%-3s %-20s %-15s %-13s %-13s %-13s %-13s %-13s %s\n", "BN", "Name", "ID", "Electronics", "Circuits", "Programming", "Mathematics", "Language", "TOTAL");
	gotoxy(3, 31);
	printf("%-3d %-20s %-15s %-13d %-13d %-13d %-13d %-13d %d\n", max->BN, max->fullName, max->id, max->studentgrades.electronics, max->studentgrades.circuits, max->studentgrades.programming, max->studentgrades.math, max->studentgrades.lang, max->studentgrades.totalMarks);

	if (sum > 0) {
		gotoxy(5, 35);
		printf("The number of passed student is: %d", sum);
		gotoxy(3, 37);
		printf("%-3s %-20s %-15s %-13s %-13s %-13s %-13s %-13s %s\n", "BN", "Name", "ID", "Electronics", "Circuits", "Programming", "Mathematics", "Language", "TOTAL");
		end = begin;
		int x = 38;
		while (end != NULL) {
			gotoxy(3, x++);
			printf("%-3d %-20s %-15s %-13d %-13d %-13d %-13d %-13d %d\n", end->BN, end->fullName, end->id, end->studentgrades.electronics, end->studentgrades.circuits, end->studentgrades.programming, end->studentgrades.math, end->studentgrades.lang, end->studentgrades.totalMarks);
			end = end->nextsucstudent;
		}
		checkrepeat(25, x + 2);
	}
	else {
		gotoxy(20, 35);
		printf("There is no succeeded student.\n");
		checkrepeat(25, 38);
	}
}
void display() {
	gotoxy(3, 17);
	if (head == NULL) {
		puts("The system is empty.");
		checkrepeat(25, 19);
		return;
	}
	struct studentNode* print = head;
	printf("%-3s %-20s %-15s %-13s %-13s %-13s %-13s %-13s %s\n", "BN", "Name", "ID", "Electronics", "Circuits", "Programming", "Mathematics", "Language", "TOTAL");
	int x = 19;
	while (print != NULL) {
		gotoxy(3, x++);
		printf("%-3d %-20s %-15s %-13d %-13d %-13d %-13d %-13d %d\n", print->BN, print->fullName, print->id, print->studentgrades.electronics, print->studentgrades.circuits, print->studentgrades.programming, print->studentgrades.math, print->studentgrades.lang, print->studentgrades.totalMarks);
		print = print->next;
	}
	checkrepeat(25, x + 2);
}
void clear() {
	struct studentNode* p, * store;
	p = head;
	while (p != NULL) {
		store = p->next;
		free(p);
		p = store;
	}
	head = NULL;
	gotoxy(20, 17);
	puts("All records are deleted successfully.");
	checkrepeat(25, 19);
}
void quit(int x, int y) {
	int choice;
	gotoxy(x, y);
	printf("Do you want to save the changes?");
	gotoxy(x + 5, y + 2);
	printf("1 : Save");
	gotoxy(x + 5, y + 3);
	printf("2 : Discard");
	gotoxy(x, y + 5);
	printf("Enter your choice: ");
	scanf("%d", &choice);

	if (choice == 1) {
		struct studentNode* current = head;
		struct data* dcurrent = dhead;
		while (current != NULL) {
			struct data* newNode = malloc(sizeof(struct data));
			newNode->Eyear = year;
			newNode->all.BN = current->BN;
			newNode->all.studentgrades.math = current->studentgrades.math;
			newNode->all.studentgrades.electronics = current->studentgrades.electronics;
			newNode->all.studentgrades.circuits = current->studentgrades.circuits;
			newNode->all.studentgrades.programming = current->studentgrades.programming;
			newNode->all.studentgrades.lang = current->studentgrades.lang;
			newNode->all.studentgrades.totalMarks = current->studentgrades.totalMarks;
			strcpy(newNode->all.fullName, current->fullName);
			strcpy(newNode->all.id, current->id);
			newNode->next = NULL;

			if (newNode->all.BN == 1) {
				newNode->next = dhead;
				dhead = newNode;
				dcurrent = dhead;
			}
			else {
				newNode->next = dcurrent->next;
				dcurrent->next = newNode;
				dcurrent = newNode;
			}
			current = current->next;
		}
		FILE* fptr;
		dcurrent = dhead;
		if ((fptr = fopen("data.txt", "w")) == NULL) {
			gotoxy(x + 10, y + 7);
			puts("File could not be opened.");
		}
		else {
			while (dhead != NULL) {
				dcurrent = dhead;
				fprintf(fptr, "%d \t %d \t %15s \t %d \t %d \t %d \t %d \t %d \t %d \t %s \n", dcurrent->Eyear, dcurrent->all.BN, dcurrent->all.id, dcurrent->all.studentgrades.math, dcurrent->all.studentgrades.electronics, dcurrent->all.studentgrades.circuits, dcurrent->all.studentgrades.programming, dcurrent->all.studentgrades.lang, dcurrent->all.studentgrades.totalMarks, dcurrent->all.fullName);
				dhead = dcurrent->next;
				free(dcurrent);
			}
		}
		fclose(fptr);
	}
	else {
		struct data* dp, * dstore;
		dp = dhead;
		while (dp != NULL) {
			dstore = dp->next;
			free(dp);
			dp = dstore;
		}
	}
	struct studentNode* p, * store;
	p = head;
	while (p != NULL) {
		store = p->next;
		free(p);
		p = store;
	}
	head = NULL;
	dhead = NULL;
}
