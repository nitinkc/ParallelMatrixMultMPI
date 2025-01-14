/*
 * guess.c
 *
 *  Created on: Sep 11, 2014
 *      Author: nitin
 */




int GUESSRANGE(int imin, int imax)
{
      // calculate midpoint to cut set in half
      int guess = (imin+imax)/2;

      // three-way comparison
      if (CHECK(p) == 1)
        // guess is in lower subset (left side)
        return GUESSRANGE(imin, guess-1);

      else if (CHECK(p) == -1)
        // guess is in upper subset (Right Side)
        return GUESSRANGE(guess+1, imax);
      else
        // guess has been found
        return guess;
}

CHECK(p)
	if p == secret return 0;
	if p < secret return -1;
	if p > secret return 1;

	main(){
		//Let Secret be 49
		GUESSRANGE(1,100);//if n=100
	}
