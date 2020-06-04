#include <iostream>
#include "stack.h"
#include "person.h"
#include "Queue.h"
#include <string>
#include <conio.h>

using namespace std;

int firstTrainArr[50], secondTrainArr[50], trainNum[2];
person* pArr[50];

person* pHead = nullptr;

typedef struct train {
	int station;
	person* pRoot;
	struct train *next;
}train;
train* tHead[2];

typedef struct skipList {
	int station;
	struct skipList * next;
	train * down;
}skipList;
skipList * sHead;


Queue qPerson1, qPerson2;



void input(int n, int l)
{
	int m, nof;
	person * pTemp;
	train * tTemp;
	string s;
	cout << "Number of passengers in Train" << l + 1 << ": \n";
	cin >> trainNum[l];

	for (int i = 0; i < n; i++)
	{
		train * newtrain = (train *)malloc(sizeof(train));
		if (l == 1)
		{
			cout << "station :\n";
			cin >> newtrain->station;
			cout << "number of passengers who order food in station " << newtrain->station << "\n";
		}
		else
		{
			newtrain->station = i + 1;
			cout << "number of passengers who order food in station " << i + 1 << "\n";

		}
		newtrain->next = NULL;

		cin >> m;
		pHead = nullptr;
		for (int j = 0; j < m; j++)
		{

			string s;
			person * newperson = new person;
			cout << "passenger number" << j + 1 << ": enter your id \n";
			cin >> s;
			newperson->id = s;
			newperson->next = nullptr;
			cout << "number of orders & food :\n";
			cin >> nof;
			for (int k = 0; k < nof; k++)
			{
				cout << k + 1 << "-> enter food name:\n"   ;
				cin >> s;
				newperson->food.push(s);
			}
			if (!pHead)
			{
				pHead = newperson;
				pTemp = pHead;
			}
			else
			{
				pTemp->next = newperson;
				pTemp = newperson;
			}
			if (l == 0)
				qPerson1.push(newperson);
		}

		newtrain->pRoot = pHead;
		if (!tHead[l])
		{
			tHead[l] = newtrain;
			tTemp = tHead[l];
		}
		else
		{
			tTemp->next = newtrain;
			tTemp = newtrain;
		}
	}
}

train* moveOnSkiplist(int findThisStation)
{
	skipList* tempSkip = sHead;
	if (tempSkip->next != NULL)
	{
		//cout << tempSkip->station;
		while (tempSkip->next->station <= findThisStation)
		{
			tempSkip = tempSkip->next;
		}
	}
	train* tempT = tempSkip->down;
	if (tempT->next != NULL)
	{
		while (tempT->next != NULL && tempT->next->station <= findThisStation)
		{
			tempT = tempT->next;
		}
	}
	return tempT;

}

void printTheAnwser(int n)
{
	int a = trainNum[0], b = trainNum[1];
	cout << "Number of passengers in train 1 & 2 after passengers change their trains in stations: \n";
	for (int i = 0; i <= n - 1; i++) // counterasho bastegi be code por cardanesh dare!
	{
		a += firstTrainArr[i];
		b += secondTrainArr[i];
		cout << "station" << i + 1 << ": " << a << "\t" << b << endl;
	}
	cout << " => Number of passengers in train 1 & 2 in station" << n << " :\n" << a << "\t" << b << endl;

	cout << "Queue of food for train 1: \n";
	while (qPerson1.size() > 0)
	{
		person * pTemp = qPerson1.front();
		cout << pTemp->id << ":\n";
		while (pTemp->food.size() > 0)
		{
			cout << pTemp->food.peek() << "\t";
			pTemp->food.pop();
		}
		qPerson1.pop();
	}

	cout << "Queue of food for train 2: \n";
	while (qPerson2.size() > 0)
	{
		person * pTemp = qPerson2.front();
		cout << pTemp->id << ":\n";
		while (pTemp->food.size() > 0)
		{
			cout << pTemp->food.peek() << "\t";
			pTemp->food.pop();
		}
		cout << endl;
		qPerson2.pop();
	}
}
void makeSkipList(int n, int l)
{
	// baray yek radif!!
	int m = sqrt(n);
	skipList * sTemp;
	train * tTemp = tHead[l];
	for (int i = 1; i <= n; i++)
	{
		if ((i % m) == 1 || i == 1)
		{
			skipList * newskipList = (skipList *)malloc(sizeof(skipList));
			newskipList->next = NULL;
			newskipList->down = tTemp;
			newskipList->station = tTemp->station;
			if (i == 1)
			{
				sHead = newskipList;
				sTemp = newskipList;
			}
			else
			{
				sTemp->next = newskipList;
				sTemp = newskipList;
			}
			//cout <<"newskipList->station : " <<newskipList->station <<"   " << tTemp->station << endl;
		}
		tTemp = tTemp->next;
	}
}
int countPeople(person* head)
{
	int count = 0;
	while (head != nullptr)
	{
		count++;
		head = head->next;
	}
	return count;

}

int main()
{
	int n = 50;
	cin >> n;

	cout << "\n first train \n";
	input(n, 0);
	cout << "\n second train \n";
	input(n, 1);

	makeSkipList(n, 0);
	for (train * tTemp = tHead[1]; tTemp != nullptr; tTemp = tTemp->next)
	{
		int index = tTemp->station;
		//cout << "woooooo  tTemp->station" << tTemp->station << endl;
		int first, second;
		first = countPeople(moveOnSkiplist(index)->pRoot);
		second = countPeople(tTemp->pRoot);

		cout << "The number of passengers who order food in station " << index << " train1 : " << first << "train2 : " << second << " => sum : "<< first + second<< endl;

		//vase mosbat manfias in 2 khat
		firstTrainArr[index - 1] = second - first;
		secondTrainArr[index - 1] = first - second;

		person* tempPerson = moveOnSkiplist(index)->pRoot;
		//araye queue dovomi ke maskharas
		pArr[index - 1] = tTemp->pRoot;

	}
	//push mikonim to queue dovomie
	for (int i = 0; i<n; i++)
	{
		person* temp = pArr[i];
		while (temp != nullptr)
		{
			qPerson2.push(temp);
			temp = temp->next;
		}
	}

	printTheAnwser(n);
	cout << "\n hello patric \n";
	getch();

	return 0;
}