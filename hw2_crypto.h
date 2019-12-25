#ifndef HW2_CRYPTO_H
#define HW2_CRYPTO_H

char* encrypt(char word[])
{
  srand(723637564);

  // if the word has fewer than 20 characters, append random printable characters
  char plain_text[21];
  int  word_len = strlen(word);
  int  ascii = (int) word[word_len - 1];  // of the last character

  strncpy(plain_text, word, word_len);
  for (int i=word_len; i < 20; i++)
    {
      plain_text[i] = (char) ((rand() / 2 + ascii) % 94) + 33;
      ascii += rand() % 200;
    }
  plain_text[20] = '\0';

  // generate an encrpted word of length 40 with printable characters
  // each plain character generates two random printable characters
  static char encrypted_word[41];

  ascii = (int) plain_text[19];
  for (int i = 0; i < 20 ; i++)
  {  
    ascii += (int) plain_text[i];

    encrypted_word[i*2] = (char) ((rand() / 2 + ascii) % 94) + 33;
    encrypted_word[i*2+1] = (char) ((rand() / 2 + ascii) % 94) + 33;
  }
  encrypted_word[40] = '\0';

  return encrypted_word;
}
  
#endif
