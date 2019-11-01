#include "Dictionary.h"
#include <iostream>
using namespace std;


int main(){
	Dictionary dict;

	string strs[] = {"Fred Astaire", "Lauren Bacall", "Brigitte Bardot", "John Belushi", "Ingmar Bergman", "Daniel Kang", "Chaewon Shin", "Matt Kana", "Jaewon Yun", "Sean Kang", "Will Nguyen", "micah haskins", "penis skinny", "skinny penis", "noel miller", "you freaking idiot", "i hate this lab", "this lab fucking sucks", "you got a small dick", "sadskfjasld"};
	int n = 20;

	dict.bulkInsert(n, strs);

	
	dict.printHash();

	return 0;
}

