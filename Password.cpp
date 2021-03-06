#include "Password.h"
using CSC2110::ListArrayIterator;

#include <iostream>

using namespace std;
using namespace CSC2110;

Password::Password()
{
	viable_words = new ListArray<String>();
	all_words = new ListArray<String>();
	len = 0;
}

Password::~Password() //(there is work to do here, delete the individual words)
{
	ListArrayIterator<String>* iter = all_words->iterator();
	while(iter->hasNext())
	{
		String* temp = iter->next();
		delete temp;
	}
	delete all_words;
	delete viable_words;
}
void Password::addWord(String* word) //add a word to the list of possible passwords
{
	if(all_words->size()==0)
	{
	all_words->add(word);
	viable_words->add(word);
	len = word->length();
	}
	
	else if(len==word->length())
	{
		all_words->add(word);
		viable_words->add(word);
	}
}

int Password::getNumMatches(String* curr_word, String* word_guess) 
{
	int counter = 0;
			for(int i = 0; i < len; i++)
			{
				char test_letter = curr_word->charAt(i);
				char letter = word_guess->charAt(i);
				if(test_letter == letter)
				{
					counter++;
				}
			}
	return counter;
}

void Password::guess(int try_password, int num_matches) //index of guessed word in the list of all words (1-based), number of matches reported by fallout 3, update viable passwords list
{
	String* temp = all_words->get(try_password); //gets word guessed

	ListArray<String>* temp_array = new ListArray<String>();

	ListArrayIterator<String>* iter = viable_words->iterator();
    
	String* yak = all_words->get(try_password);
	while(iter->hasNext())
	{
		String* blah = iter->next();
		
		int matches = getNumMatches(blah, yak);
        	//call getNumMatches

		if(num_matches == matches)
		{
			temp_array->add(blah);	
		}
	}
        delete viable_words;
	viable_words = temp_array; 
	
}

int Password::getNumberOfPasswordsLeft() //returns the number of possible passwords remaining
{
	int temp = viable_words->size();
	return temp;
}
void Password::displayViableWords() //display the current list of possible passwords
{
	ListArrayIterator<String>* display = viable_words->iterator();
	while(display->hasNext())
	{
		String* aha = display->next();
		aha->displayString();
		cout << endl;
	}
}

String* Password::getOriginalWord(int index) //get a word from the original list of all passwords, 1-based
{
	String* yes = all_words->get(index);
	return yes;
}
int Password::bestGuess()
{
   int best_guess_index = -1;
   int best_num_eliminated = -1;
   int num_viable_passwords = getNumberOfPasswordsLeft();

   //loop over ALL words, even if they have been eliminated as the password
   int count = 1;
   ListArrayIterator<String>* all_iter = all_words->iterator();
   while(all_iter->hasNext())
   {
      String* original_word = all_iter->next();

      //loop over only those words that could still be the password
      //count up the number of matches between a possible password and a word in the original list
      int* count_num_matches = new int[len + 1];

      for (int i = 0; i < len; i++) 
      {
         count_num_matches[i] = 0;
      }

      ListArrayIterator<String>* viable_iter = viable_words->iterator();
      while(viable_iter->hasNext())
      {
         String* viable_word = viable_iter->next();
         int num_matches = getNumMatches(viable_word, original_word);
         count_num_matches[num_matches]++;
      }
      delete viable_iter;

      //find the largest number in the count_num_matches array
      //the largest number indicates the guess that will generate the most eliminations
      int most_num_matches = 0;
      for (int j = 0; j < len; j++) 
      {
         int curr_num_matches = count_num_matches[j];
         if (curr_num_matches > most_num_matches)
         {
            most_num_matches = curr_num_matches;
         }
      }

      //compute the fewest that can possibly be eliminated by guessing the current word (original list)
      int num_eliminated = num_viable_passwords - most_num_matches;

      //select the word to guess that maximizes the minimum number of eliminations (minimax)
      if (num_eliminated > best_num_eliminated)
      {
         best_num_eliminated = num_eliminated;
         best_guess_index = count;
      }
      
      count++;
      delete[] count_num_matches;
   }

   delete all_iter;
   return best_guess_index;  //return a 1-based index into the all_words list of words (careful)
}